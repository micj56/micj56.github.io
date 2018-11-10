/*
 * disk.c -- disk simulation
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
#include "memory.h"
#include "timer.h"
#include "disk.h"


static Bool debug = FALSE;

static FILE *diskImage;
static long numSectors;

static Word diskCtrl;
static Word diskCnt;
static Word diskSct;
static Word diskDma;
static Word diskCap;

static long lastSct;


static Word readWord(Byte *p) {
  Word data;

  data = ((Word) *(p + 0)) << 24 |
         ((Word) *(p + 1)) << 16 |
         ((Word) *(p + 2)) <<  8 |
         ((Word) *(p + 3)) <<  0;
  return data;
}


static void writeWord(Byte *p, Word data) {
  *(p + 0) = (Byte) (data >> 24);
  *(p + 1) = (Byte) (data >> 16);
  *(p + 2) = (Byte) (data >>  8);
  *(p + 3) = (Byte) (data >>  0);
}


static void diskCallback(void) {
  Byte buffer[SECTOR_SIZE];
  int i, j;
  Word data;
  Word addr;

  if (debug) {
    cPrintf("\n**** DISK CALLBACK ****\n");
  }
  if (diskCap != 0 &&
      diskCnt != 0 &&
      diskSct < diskCap &&
      diskSct + diskCnt <= diskCap) {
    /* do the transfer */
    if (fseek(diskImage, diskSct * SECTOR_SIZE, SEEK_SET) != 0) {
      error("cannot position to sector in disk image");
    }
    addr = diskDma;
    if (diskCtrl & DISK_WRT) {
      /* memory --> disk */
      for (i = 0; i < diskCnt; i++) {
        for (j = 0; j < SECTOR_SIZE; j += 4) {
          data = memoryReadWord(addr);
          writeWord(buffer + j, data);
          if (debug) {
            cPrintf("\n**** m[0x%08X] --> 0x%08X ****\n",
                    addr, data);
          }
          addr += 4;
        }
        if (fwrite(buffer, SECTOR_SIZE, 1, diskImage) != 1) {
          error("cannot write to disk image");
        }
        diskSct++;
      }
    } else {
      /* disk --> memory */
      for (i = 0; i < diskCnt; i++) {
        if (fread(buffer, SECTOR_SIZE, 1, diskImage) != 1) {
          error("cannot read from disk image");
        }
        for (j = 0; j < SECTOR_SIZE; j += 4) {
          data = readWord(buffer + j);
          memoryWriteWord(addr, data);
          if (debug) {
            cPrintf("\n**** m[0x%08X] <-- 0x%08X ****\n",
                    addr, data);
          }
          addr += 4;
        }
        diskSct++;
      }
    }
    lastSct = diskSct;
  } else {
    /* sectors requested exceed disk capacity */
    /* or there were zero sectors requested */
    /* or we have no disk at all */
    diskCtrl |= DISK_ERR;
  }
  diskCtrl &= ~DISK_BUSY;
  if (diskCtrl & DISK_IEN) {
    /* raise disk interrupt */
    cpuInterrupt(IRQ_DISK);
  }
}


Word diskRead(Word addr) {
  Word data;

  if (debug) {
    cPrintf("\n**** DISK READ from 0x%08X", addr);
  }
  if (addr == DISK_CTRL) {
    data = diskCtrl;
  } else
  if (addr == DISK_CNT) {
    /* this register is write-only */
    throwException(EXC_BUS_TIMEOUT);
  } else
  if (addr == DISK_SCT) {
    /* this register is write-only */
    throwException(EXC_BUS_TIMEOUT);
  } else
  if (addr == DISK_DMA) {
    /* this register is write-only */
    throwException(EXC_BUS_TIMEOUT);
  } else
  if (addr == DISK_CAP) {
    data = diskCap;
  } else {
    /* illegal register */
    throwException(EXC_BUS_TIMEOUT);
  }
  if (debug) {
    cPrintf(", data = 0x%08X ****\n", data);
  }
  return data;
}


void diskWrite(Word addr, Word data) {
  if (debug) {
    cPrintf("\n**** DISK WRITE to 0x%08X, data = 0x%08X ****\n",
            addr, data);
  }
  if (addr == DISK_CTRL) {
    if (data & DISK_WRT) {
      diskCtrl |= DISK_WRT;
    } else {
      diskCtrl &= ~DISK_WRT;
    }
    if (data & DISK_IEN) {
      diskCtrl |= DISK_IEN;
    } else {
      diskCtrl &= ~DISK_IEN;
    }
    if (data & DISK_STRT) {
      diskCtrl |= DISK_BUSY;
      diskCtrl &= ~DISK_ERR;
      /* only start a disk operation if disk is present */
      if (diskCap != 0) {
        timerStart(DISK_DELAY +
                   (abs(diskSct - lastSct) * DISK_SEEK) / diskCap,
                   diskCallback);
      }
    }
  } else
  if (addr == DISK_CNT) {
    diskCnt = data;
  } else
  if (addr == DISK_SCT) {
    diskSct = data;
  } else
  if (addr == DISK_DMA) {
    diskDma = data;
  } else
  if (addr == DISK_CAP) {
    /* this register is read-only */
    throwException(EXC_BUS_TIMEOUT);
  } else {
    /* illegal register */
    throwException(EXC_BUS_TIMEOUT);
  }
}


void diskReset(void) {
  cPrintf("Resetting Disk...\n");
  diskCtrl = 0;
  diskCnt = 0;
  diskSct = 0;
  diskDma = 0;
  diskCap = numSectors;
  lastSct = 0;
  if (diskCap != 0) {
    cPrintf("Disk of size %ld sectors (%ld bytes) installed.\n",
            numSectors, numSectors * SECTOR_SIZE);
  }
}


void diskInit(char *diskImageName) {
  long numBytes;

  if (diskImageName == NULL) {
    /* do not install disk */
    diskImage = NULL;
    numSectors = 0;
  } else {
    /* try to install disk */
    diskImage = fopen(diskImageName, "r+b");
    if (diskImage == NULL) {
      error("cannot open disk image '%s'", diskImageName);
    }
    fseek(diskImage, 0, SEEK_END);
    numBytes = ftell(diskImage);
    fseek(diskImage, 0, SEEK_SET);
    numSectors = numBytes / SECTOR_SIZE;
  }
  diskReset();
}


void diskExit(void) {
  if (diskImage == NULL) {
    /* disk not installed */
    return;
  }
  fclose(diskImage);
}
