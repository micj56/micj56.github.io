/*
 * mmu.c -- MMU simulation
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "common.h"
#include "curslib.h"
#include "error.h"
#include "except.h"
#include "cpu.h"
#include "mmu.h"
#include "memory.h"


static Bool debugUse = FALSE;
static Bool debugWrite = FALSE;


static TLB_Entry tlb[TLB_SIZE];
static Word tlbIndex;
static Word tlbEntryHi;
static Word tlbEntryLo;
static Word tlbBadAddr;


static int assoc(Word page) {
  int n, i;

  n = -1;
  for (i = 0; i < TLB_SIZE; i++) {
    if (tlb[i].page == page) {
      if (n < 0) {
        /* this is the first match */
        n = i;
      } else {
        /* this is another match */
        throwException(EXC_TLB_DBLHIT);
      }
    }
  }
  return n;
}


static Word v2p(Word vAddr, Bool userMode, Bool writing) {
  Word pAddr;
  Word page, offset;
  int index;

  if (debugUse) {
    cPrintf("**** vAddr = 0x%08X", vAddr);
  }
  if ((vAddr & 0x80000000) != 0 && userMode) {
    tlbBadAddr = vAddr;
    throwException(EXC_PRV_ADDRESS);
  }
  if ((vAddr & 0xC0000000) == 0xC0000000) {
    /* unmapped address space */
    pAddr = vAddr & ~0xC0000000;
  } else {
    /* mapped address space */
    page = vAddr & ~OFFSET_MASK;
    offset = vAddr & OFFSET_MASK;
    index = assoc(page);
    if (index == -1) {
      /* TLB miss */
      tlbBadAddr = vAddr;
      throwException(EXC_TLB_MISS);
    }
    pAddr = tlb[index].frame | offset;
  }
  if (debugUse) {
    cPrintf(", pAddr = 0x%08X ****\n", pAddr);
  }
  return pAddr;
}


Word mmuReadWord(Word vAddr, Bool userMode) {
  return memoryReadWord(v2p(vAddr, userMode, FALSE));
}


Half mmuReadHalf(Word vAddr, Bool userMode) {
  return memoryReadHalf(v2p(vAddr, userMode, FALSE));
}


Byte mmuReadByte(Word vAddr, Bool userMode) {
  return memoryReadByte(v2p(vAddr, userMode, FALSE));
}


void mmuWriteWord(Word vAddr, Word data, Bool userMode) {
  memoryWriteWord(v2p(vAddr, userMode, TRUE), data);
}


void mmuWriteHalf(Word vAddr, Half data, Bool userMode) {
  memoryWriteHalf(v2p(vAddr, userMode, TRUE), data);
}


void mmuWriteByte(Word vAddr, Byte data, Bool userMode) {
  memoryWriteByte(v2p(vAddr, userMode, TRUE), data);
}


Word mmuGetIndex(void) {
  return tlbIndex;
}


void mmuSetIndex(Word value) {
  tlbIndex = value;
}


Word mmuGetEntryHi(void) {
  return tlbEntryHi;
}


void mmuSetEntryHi(Word value) {
  tlbEntryHi = value;
}


Word mmuGetEntryLo(void) {
  return tlbEntryLo;
}


void mmuSetEntryLo(Word value) {
  tlbEntryLo = value;
}


Word mmuGetBadAddr(void) {
  return tlbBadAddr;
}


void mmuSetBadAddr(Word value) {
  tlbBadAddr = value;
}


void mmuTbs(void) {
  int index;

  index = assoc(tlbEntryHi & ~OFFSET_MASK);
  if (index == -1) {
    tlbIndex = 0x80000000;
  } else {
    tlbIndex = index;
  }
}


void mmuTbwr(void) {
  int index;

  /* choose a random index, but don't touch fixed entries */
  do {
    index = rand() & TLB_MASK;
  } while (index < TLB_FIXED);
  tlb[index].page = tlbEntryHi & ~OFFSET_MASK;
  tlb[index].frame = tlbEntryLo & ~OFFSET_MASK;
  if (debugWrite) {
    cPrintf("**** TLB[%02d] <- 0x%08X 0x%08X ****\n",
            index, tlb[index].page, tlb[index].frame);
  }
}


void mmuTbri(void) {
  int index;

  index = tlbIndex & TLB_MASK;
  tlbEntryHi = tlb[index].page;
  tlbEntryLo = tlb[index].frame;
}


void mmuTbwi(void) {
  int index;

  index = tlbIndex & TLB_MASK;
  tlb[index].page = tlbEntryHi & ~OFFSET_MASK;
  tlb[index].frame = tlbEntryLo & ~OFFSET_MASK;
  if (debugWrite) {
    cPrintf("**** TLB[%02d] <- 0x%08X 0x%08X ****\n",
            index, tlb[index].page, tlb[index].frame);
  }
}


TLB_Entry mmuGetTLB(int index) {
  return tlb[index & TLB_MASK];
}


void mmuSetTLB(int index, TLB_Entry tlbEntry) {
  index &= TLB_MASK;
  tlb[index] = tlbEntry;
  if (debugWrite) {
    cPrintf("**** TLB[%02d] <- 0x%08X 0x%08X ****\n",
            index, tlb[index].page, tlb[index].frame);
  }
}


void mmuReset(void) {
  int i;

  cPrintf("Resetting MMU...\n");
  for (i = 0; i < TLB_SIZE; i++) {
    tlb[i].page = rand() & ~OFFSET_MASK;
    tlb[i].frame = rand() & ~OFFSET_MASK;
    if (debugWrite) {
      cPrintf("**** TLB[%02d] <- 0x%08X 0x%08X ****\n",
              i, tlb[i].page, tlb[i].frame);
    }
  }
  tlbIndex = rand();
  tlbEntryHi = rand();
  tlbEntryLo = rand();
  tlbBadAddr = rand();
}


void mmuInit(void) {
  mmuReset();
}


void mmuExit(void) {
}
