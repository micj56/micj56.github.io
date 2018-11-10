/*
 * cpu.c -- CPU simulation
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "common.h"
#include "curslib.h"
#include "error.h"
#include "except.h"
#include "instr.h"
#include "cpu.h"
#include "mmu.h"
#include "timer.h"


#define RR(n)		r[n]
#define WR(n,d)		((void) ((n) != 0 ? r[n] = (d) : 0))

#define UM		(psw & PSW_UM)
#define PUM		(psw & PSW_PUM)
#define OUM		(psw & PSW_OUM)
#define IE		(psw & PSW_IE)
#define PIE		(psw & PSW_PIE)
#define OIE		(psw & PSW_OIE)


static Bool debugIRQ = FALSE;

static Word pc;		/* program counter */
static Word psw;	/* processor status word */
static Word r[32];	/* general purpose registers */

static int instrCount;
static unsigned irqPending;

static Bool breakSet;
static Word breakAddr;

static Word total;

static Bool run;


static void handleRealTimeTasks(void) {
  /* handle 'real-time' tasks */
  if (++instrCount == INSTRS_PER_MSEC) {
    instrCount = 0;
    timerTick();
  }
}


static void handleInterrupts(void) {
  unsigned irqMask;
  unsigned irqSeen;
  int priority;

  /* handle exceptions and interrupts */
  if (irqPending == 0) {
    /* no exception or interrupt pending */
    return;
  }
  /* at least one exception or interrupt is pending */
  irqMask = ~PSW_IRQ_MASK | (psw & PSW_IRQ_MASK);
  if (debugIRQ) {
    cPrintf("**** IRQ  = 0x%08X ****\n", irqPending);
    cPrintf("**** MASK = 0x%08X ****\n", irqMask);
  }
  irqSeen = irqPending & irqMask;
  if (irqSeen == 0) {
    /* none that gets through */
    return;
  }
  /* determine the one with the highest priority */
  for (priority = 31; priority >= 0; priority--) {
    if ((irqSeen & ((unsigned) 1 << priority)) != 0) {
      /* highest priority among visible ones found */
      break;
    }
  }
  /* acknowledge exception, or interrupt if enabled */
  if (priority >= 16 || IE != 0) {
    /* clear corresponding bit in irqPending vector */
    irqPending &= ~((unsigned) 1 << priority);
    /* copy and reset interrupt enable bit in PSW */
    if (PIE != 0) {
      psw |= PSW_OIE;
    } else {
      psw &= ~PSW_OIE;
    }
    if (IE != 0) {
      psw |= PSW_PIE;
    } else {
      psw &= ~PSW_PIE;
    }
    psw &= ~PSW_IE;
    /* copy and reset user mode enable bit in PSW */
    if (PUM != 0) {
      psw |= PSW_OUM;
    } else {
      psw &= ~PSW_OUM;
    }
    if (UM != 0) {
      psw |= PSW_PUM;
    } else {
      psw &= ~PSW_PUM;
    }
    psw &= ~PSW_UM;
    /* reflect priority in PSW */
    psw &= ~PSW_PRIO_MASK;
    psw |= priority << PSW_PRIO_SHFT;
    /* save interrupt return address and start service routine */
    WR(30, pc);
    pc = 0xC0000004;
  }
}


static void execNextInstruction(void) {
  Word instr;
  Word next;
  int op, dst, src1, src2;
  Half immed;
  Word offset;
  int scnt;
  Word smsk;

  /* count the instruction */
  total++;
  /* fetch the instruction */
  instr = mmuReadWord(pc, UM);
  /* decode the instruction */
  op = (instr >> 26) & 0x3F;
  dst = (instr >> 21) & 0x1F;
  src1 = (instr >> 16) & 0x1F;
  src2 = (instr >> 11) & 0x1F;
  immed = instr & 0x0000FFFF;
  offset = instr & 0x03FFFFFF;
  next = pc + 4;
  /* execute the instruction */
  switch (op) {
    case OP_ADD:
      WR(dst, (signed int) RR(src1) + (signed int) RR(src2));
      break;
    case OP_ADDI:
      WR(dst, (signed int) RR(src1) + (signed int) SEXT16(immed));
      break;
    case OP_SUB:
      WR(dst, (signed int) RR(src1) - (signed int) RR(src2));
      break;
    case OP_SUBI:
      WR(dst, (signed int) RR(src1) - (signed int) SEXT16(immed));
      break;
    case OP_MUL:
      WR(dst, (signed int) RR(src1) * (signed int) RR(src2));
      break;
    case OP_MULI:
      WR(dst, (signed int) RR(src1) * (signed int) SEXT16(immed));
      break;
    case OP_MULU:
      WR(dst, RR(src1) * RR(src2));
      break;
    case OP_MULUI:
      WR(dst, RR(src1) * ZEXT16(immed));
      break;
    case OP_DIV:
      if (RR(src2) == 0) {
        throwException(EXC_DIVIDE);
      }
      WR(dst, (signed int) RR(src1) / (signed int) RR(src2));
      break;
    case OP_DIVI:
      if (SEXT16(immed) == 0) {
        throwException(EXC_DIVIDE);
      }
      WR(dst, (signed int) RR(src1) / (signed int) SEXT16(immed));
      break;
    case OP_DIVU:
      if (RR(src2) == 0) {
        throwException(EXC_DIVIDE);
      }
      WR(dst, RR(src1) / RR(src2));
      break;
    case OP_DIVUI:
      if (SEXT16(immed) == 0) {
        throwException(EXC_DIVIDE);
      }
      WR(dst, RR(src1) / ZEXT16(immed));
      break;
    case OP_REM:
      if (RR(src2) == 0) {
        throwException(EXC_DIVIDE);
      }
      WR(dst, (signed int) RR(src1) % (signed int) RR(src2));
      break;
    case OP_REMI:
      if (SEXT16(immed) == 0) {
        throwException(EXC_DIVIDE);
      }
      WR(dst, (signed int) RR(src1) % (signed int) SEXT16(immed));
      break;
    case OP_REMU:
      if (RR(src2) == 0) {
        throwException(EXC_DIVIDE);
      }
      WR(dst, RR(src1) % RR(src2));
      break;
    case OP_REMUI:
      if (SEXT16(immed) == 0) {
        throwException(EXC_DIVIDE);
      }
      WR(dst, RR(src1) % ZEXT16(immed));
      break;
    case OP_AND:
      WR(dst, RR(src1) & RR(src2));
      break;
    case OP_ANDI:
      WR(dst, RR(src1) & ZEXT16(immed));
      break;
    case OP_OR:
      WR(dst, RR(src1) | RR(src2));
      break;
    case OP_ORI:
      WR(dst, RR(src1) | ZEXT16(immed));
      break;
    case OP_XOR:
      WR(dst, RR(src1) ^ RR(src2));
      break;
    case OP_XORI:
      WR(dst, RR(src1) ^ ZEXT16(immed));
      break;
    case OP_XNOR:
      WR(dst, ~(RR(src1) ^ RR(src2)));
      break;
    case OP_XNORI:
      WR(dst, ~(RR(src1) ^ ZEXT16(immed)));
      break;
    case OP_SLL:
      scnt = RR(src2) & 0x1F;
      WR(dst, RR(src1) << scnt);
      break;
    case OP_SLLI:
      scnt = immed & 0x1F;
      WR(dst, RR(src1) << scnt);
      break;
    case OP_SLR:
      scnt = RR(src2) & 0x1F;
      WR(dst, RR(src1) >> scnt);
      break;
    case OP_SLRI:
      scnt = immed & 0x1F;
      WR(dst, RR(src1) >> scnt);
      break;
    case OP_SAR:
      scnt = RR(src2) & 0x1F;
      smsk = (RR(src1) & 0x80000000 ? 0xFFFFFFFF << (32 - scnt) : 0x00000000);
      WR(dst, smsk | (RR(src1) >> scnt));
      break;
    case OP_SARI:
      scnt = immed & 0x1F;
      smsk = (RR(src1) & 0x80000000 ? 0xFFFFFFFF << (32 - scnt) : 0x00000000);
      WR(dst, smsk | (RR(src1) >> scnt));
      break;
    case OP_LDHI:
      WR(dst, ZEXT16(immed) << 16);
      break;
    case OP_BEQ:
      if (RR(dst) == RR(src1)) {
        next += SEXT16(immed) << 2;
      }
      break;
    case OP_BNE:
      if (RR(dst) != RR(src1)) {
        next += SEXT16(immed) << 2;
      }
      break;
    case OP_BLE:
      if ((signed int) RR(dst) <= (signed int) RR(src1)) {
        next += SEXT16(immed) << 2;
      }
      break;
    case OP_BLEU:
      if (RR(dst) <= RR(src1)) {
        next += SEXT16(immed) << 2;
      }
      break;
    case OP_BLT:
      if ((signed int) RR(dst) < (signed int) RR(src1)) {
        next += SEXT16(immed) << 2;
      }
      break;
    case OP_BLTU:
      if (RR(dst) < RR(src1)) {
        next += SEXT16(immed) << 2;
      }
      break;
    case OP_BGE:
      if ((signed int) RR(dst) >= (signed int) RR(src1)) {
        next += SEXT16(immed) << 2;
      }
      break;
    case OP_BGEU:
      if (RR(dst) >= RR(src1)) {
        next += SEXT16(immed) << 2;
      }
      break;
    case OP_BGT:
      if ((signed int) RR(dst) > (signed int) RR(src1)) {
        next += SEXT16(immed) << 2;
      }
      break;
    case OP_BGTU:
      if (RR(dst) > RR(src1)) {
        next += SEXT16(immed) << 2;
      }
      break;
    case OP_J:
      next += SEXT26(offset) << 2;
      break;
    case OP_JR:
      next = RR(dst);
      break;
    case OP_JAL:
      WR(31, next);
      next += SEXT26(offset) << 2;
      break;
    case OP_JALR:
      WR(31, next);
      next = RR(dst);
      break;
    case OP_TRAP:
      throwException(EXC_TRAP);
      break;
    case OP_RFX:
      if (PIE != 0) {
        psw |= PSW_IE;
      } else {
        psw &= ~PSW_IE;
      }
      if (OIE != 0) {
        psw |= PSW_PIE;
      } else {
        psw &= ~PSW_PIE;
      }
      if (PUM != 0) {
        psw |= PSW_UM;
      } else {
        psw &= ~PSW_UM;
      }
      if (OUM != 0) {
        psw |= PSW_PUM;
      } else {
        psw &= ~PSW_PUM;
      }
      next = RR(30);
      break;
    case OP_LDW:
      WR(dst, mmuReadWord(RR(src1) + SEXT16(immed), UM));
      break;
    case OP_LDH:
      WR(dst, (signed int) (signed short)
              mmuReadHalf(RR(src1) + SEXT16(immed), UM));
      break;
    case OP_LDHU:
      WR(dst, mmuReadHalf(RR(src1) + SEXT16(immed), UM));
      break;
    case OP_LDB:
      WR(dst, (signed int) (signed char)
              mmuReadByte(RR(src1) + SEXT16(immed), UM));
      break;
    case OP_LDBU:
      WR(dst, mmuReadByte(RR(src1) + SEXT16(immed), UM));
      break;
    case OP_STW:
      mmuWriteWord(RR(src1) + SEXT16(immed), RR(dst), UM);
      break;
    case OP_STH:
      mmuWriteHalf(RR(src1) + SEXT16(immed), RR(dst), UM);
      break;
    case OP_STB:
      mmuWriteByte(RR(src1) + SEXT16(immed), RR(dst), UM);
      break;
    case OP_MVFS:
      switch (immed) {
        case 0:
          WR(dst, psw);
          break;
        case 1:
          WR(dst, mmuGetIndex());
          break;
        case 2:
          WR(dst, mmuGetEntryHi());
          break;
        case 3:
          WR(dst, mmuGetEntryLo());
          break;
        case 4:
          WR(dst, mmuGetBadAddr());
          break;
        default:
          throwException(EXC_ILL_INSTRCT);
          break;
      }
      break;
    case OP_MVTS:
      if (UM != 0) {
        throwException(EXC_PRV_INSTRCT);
      }
      switch (immed) {
        case 0:
          psw = RR(dst);
          break;
        case 1:
          mmuSetIndex(RR(dst));
          break;
        case 2:
          mmuSetEntryHi(RR(dst));
          break;
        case 3:
          mmuSetEntryLo(RR(dst));
          break;
        case 4:
          mmuSetBadAddr(RR(dst));
          break;
        default:
          throwException(EXC_ILL_INSTRCT);
          break;
      }
      break;
    case OP_TBS:
      if (UM != 0) {
        throwException(EXC_PRV_INSTRCT);
      }
      mmuTbs();
      break;
    case OP_TBWR:
      if (UM != 0) {
        throwException(EXC_PRV_INSTRCT);
      }
      mmuTbwr();
      break;
    case OP_TBRI:
      if (UM != 0) {
        throwException(EXC_PRV_INSTRCT);
      }
      mmuTbri();
      break;
    case OP_TBWI:
      if (UM != 0) {
        throwException(EXC_PRV_INSTRCT);
      }
      mmuTbwi();
      break;
    default:
      throwException(EXC_ILL_INSTRCT);
      break;
  }
  /* update PC */
  pc = next;
}


Word cpuGetPC(void) {
  return pc;
}


void cpuSetPC(Word addr) {
  pc = addr;
}


Word cpuGetReg(int regnum) {
  return RR(regnum & 0x1F);
}


void cpuSetReg(int regnum, Word value) {
  WR(regnum & 0x1F, value);
}


Word cpuGetPSW(void) {
  return psw;
}


void cpuSetPSW(Word value) {
  psw = value;
}


Bool cpuTestBreak(void) {
  return breakSet;
}


Word cpuGetBreak(void) {
  return breakAddr;
}


void cpuSetBreak(Word addr) {
  breakAddr = addr;
  breakSet = TRUE;
}


void cpuResetBreak(void) {
  breakSet = FALSE;
}


Word cpuGetTotal(void) {
  return total;
}


void cpuStep(void) {
  jmp_buf myEnvironment;
  int exception;

  exception = setjmp(myEnvironment);
  if (exception == 0) {
    /* initialization */
    pushEnvironment(&myEnvironment);
    handleRealTimeTasks();
    execNextInstruction();
    handleInterrupts();
  } else {
    /* an exception was thrown */
    cpuInterrupt(exception);
    handleInterrupts();
  }
  popEnvironment();
}


void cpuRun(void) {
  jmp_buf myEnvironment;
  int exception;

  run = TRUE;
  exception = setjmp(myEnvironment);
  if (exception == 0) {
    /* initialization */
    pushEnvironment(&myEnvironment);
  } else {
    /* an exception was thrown */
    cpuInterrupt(exception);
    handleInterrupts();
    if (breakSet && pc == breakAddr) {
      run = FALSE;
    }
  }
  while (run) {
    handleRealTimeTasks();
    execNextInstruction();
    handleInterrupts();
    if (breakSet && pc == breakAddr) {
      run = FALSE;
    }
  }
  popEnvironment();
}


void cpuHalt(void) {
  run = FALSE;
}


void cpuInterrupt(int priority) {
  irqPending |= ((unsigned) 1 << priority);
}


void cpuReset(void) {
  int i;

  cPrintf("Resetting CPU...\n");
  /* most registers are in a random state */
  for (i = 1; i < 32; i++) {
    r[i] = rand();
  }
  /* but not all */
  pc = 0xFE000000;
  r[0] = 0;
  psw = 0;
  /* reset simulator control variables */
  instrCount = 0;
  irqPending = 0;
  total = 0;
}


void cpuInit(void) {
  cpuReset();
}


void cpuExit(void) {
}
