/*
 * main.c -- ECO32 simulator
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "curslib.h"
#include "error.h"
#include "command.h"
#include "instr.h"
#include "cpu.h"
#include "mmu.h"
#include "memory.h"
#include "timer.h"
#include "term.h"
#include "disk.h"
#include "graph.h"


static void usage(char *myself) {
  fprintf(stderr, "Usage: %s\n", myself);
  fprintf(stderr, "         [-i]             interactive mode on\n");
  fprintf(stderr, "         [-l <prog>]      program file name\n");
  fprintf(stderr, "         [-r <rom>]       ROM image file name\n");
  fprintf(stderr, "         [-d <disk>]      disk image file name\n");
  fprintf(stderr, "         [-g]             graphics controller present\n");
  fprintf(stderr, "The options -l and -r are mutually exclusive.\n");
  fprintf(stderr, "If both are omitted, interactive mode is assumed.\n");
  exit(1);
}


int main(int argc, char *argv[]) {
  int i;
  char *argp;
  Bool interactive;
  char *progName;
  char *romName;
  char *diskName;
  Bool graphics;
  char command[20];
  char *line;

  interactive = FALSE;
  progName = NULL;
  romName = NULL;
  diskName = NULL;
  graphics = FALSE;
  for (i = 1; i < argc; i++) {
    argp = argv[i];
    if (*argp != '-') {
      usage(argv[0]);
    }
    argp++;
    switch (*argp) {
      case 'i':
        interactive = TRUE;
        break;
      case 'l':
        if (i == argc - 1 || progName != NULL || romName != NULL) {
          usage(argv[0]);
        }
        progName = argv[++i];
        break;
      case 'r':
        if (i == argc - 1 || romName != NULL || progName != NULL) {
          usage(argv[0]);
        }
        romName = argv[++i];
        break;
      case 'd':
        if (i == argc - 1 || diskName != NULL) {
          usage(argv[0]);
        }
        diskName = argv[++i];
        break;
      case 'g':
        graphics = TRUE;
        break;
      default:
        usage(argv[0]);
    }
  }
  printf("ECO32 Simulator started\n");
  cInit();
  cPrintf("ECO32 Simulator started\n");
  if (progName == NULL && romName == NULL && !interactive) {
    cPrintf("Neither a program to load nor a system ROM was\n");
    cPrintf("specified, so interactive mode is assumed.\n");
    interactive = TRUE;
  }
  initInstrTable();
  timerInit();
  termInit();
  diskInit(diskName);
  if (graphics) {
    graphInit();
  }
  memoryInit(romName, progName);
  mmuInit();
  cpuInit();
  if (progName != NULL) {
    cPrintf("Setting PC to 0xC0000000...\n");
    strcpy(command, "@ C0000000\n");
    execCommand(command);
  }
  if (!interactive) {
    cPrintf("Start executing...\n");
    strcpy(command, "c\n");
    execCommand(command);
  } else {
    while (1) {
      line = cGetLine("ECO32 > ");
      if (*line == '\0') {
        break;
      }
      cAddHist(line);
      if (execCommand(line)) {
        break;
      }
    }
  }
  cpuExit();
  mmuExit();
  memoryExit();
  timerExit();
  termExit();
  diskExit();
  graphExit();
  cPrintf("ECO32 Simulator finished\n");
  cExit();
  printf("ECO32 Simulator finished\n");
  return 0;
}
