/*
 * term.c -- terminal simulation
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
#include "timer.h"
#include "term.h"


static Bool debug = FALSE;

static Word rcvrCtrl;
static Word rcvrData;

static Word xmtrCtrl;
static Word xmtrData;


static void rcvrCallback(void) {
  int c;

  if (debug) {
    cPrintf("\n**** TERM RCVR CALLBACK ****\n");
  }
  timerStart(TERM_RCVR_MSEC, rcvrCallback);
  c = cGetChar();
  if (c == -1) {
    /* no character typed */
    return;
  }
  if (c == TERM_SIM_ESC) {
    /* simulation escape character typed */
    cpuHalt();
  } else {
    /* any other character typed */
    rcvrData = c & 0xFF;
    rcvrCtrl |= TERM_RCVR_RDY;
    if (rcvrCtrl & TERM_RCVR_IEN) {
      /* raise terminal rcvr interrupt */
      cpuInterrupt(IRQ_TERM_RCVR);
    }
  }
}


static void xmtrCallback(void) {
  if (debug) {
    cPrintf("\n**** TERM XMTR CALLBACK ****\n");
  }
  cPutChar(xmtrData);
  xmtrCtrl |= TERM_XMTR_RDY;
  if (xmtrCtrl & TERM_XMTR_IEN) {
    /* raise terminal xmtr interrupt */
    cpuInterrupt(IRQ_TERM_XMTR);
  }
}


Word termRead(Word addr) {
  Word data;

  if (debug) {
    cPrintf("\n**** TERM READ from 0x%08X", addr);
  }
  if (addr == TERM_RCVR_CTRL) {
    data = rcvrCtrl;
  } else
  if (addr == TERM_RCVR_DATA) {
    rcvrCtrl &= ~TERM_RCVR_RDY;
    data = rcvrData;
  } else
  if (addr == TERM_XMTR_CTRL) {
    data = xmtrCtrl;
  } else
  if (addr == TERM_XMTR_DATA) {
    /* this register is write-only */
    throwException(EXC_BUS_TIMEOUT);
  } else {
    /* illegal register */
    throwException(EXC_BUS_TIMEOUT);
  }
  if (debug) {
    cPrintf(", data = 0x%08X ****\n", data);
  }
  return data;
}


void termWrite(Word addr, Word data) {
  if (debug) {
    cPrintf("\n**** TERM WRITE to 0x%08X, data = 0x%08X ****\n",
            addr, data);
  }
  if (addr == TERM_RCVR_CTRL) {
    if (data & TERM_RCVR_IEN) {
      rcvrCtrl |= TERM_RCVR_IEN;
    } else {
      rcvrCtrl &= ~TERM_RCVR_IEN;
    }
  } else
  if (addr == TERM_RCVR_DATA) {
    /* this register is read-only */
    throwException(EXC_BUS_TIMEOUT);
  } else
  if (addr == TERM_XMTR_CTRL) {
    if (data & TERM_XMTR_IEN) {
      xmtrCtrl |= TERM_XMTR_IEN;
    } else {
      xmtrCtrl &= ~TERM_XMTR_IEN;
    }
  } else
  if (addr == TERM_XMTR_DATA) {
    xmtrData = data & 0xFF;
    xmtrCtrl &= ~TERM_XMTR_RDY;
    timerStart(TERM_XMTR_MSEC, xmtrCallback);
  } else {
    /* illegal register */
    throwException(EXC_BUS_TIMEOUT);
  }
}


void termReset(void) {
  cPrintf("Resetting Terminal...\n");
  rcvrCtrl = 0;
  rcvrData = 0;
  timerStart(TERM_RCVR_MSEC, rcvrCallback);
  xmtrCtrl = TERM_XMTR_RDY;
  xmtrData = 0;
}


void termInit(void) {
  termReset();
}


void termExit(void) {
}
