/*
 * main.c -- SPL compiler
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "utils.h"
#include "scanner.h"


static void help(char *myself) {
  /* show some help how to use the program */
  printf("Usage: %s [options] <input file>\n", myself);
  printf("Options:\n");
  printf("  --help           show this help\n");
}


int main(int argc, char *argv[]) {
  int i;
  char *inFileName;
  int token;

  /* analyze command line */
  inFileName = NULL;
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      /* option */
      if (strcmp(argv[i], "--help") == 0) {
        help(argv[0]);
        exit(0);
      } else {
        error("unrecognized option '%s'; try '%s --help'",
              argv[i], argv[0]);
      }
    } else {
      /* file */
      if (inFileName != NULL) {
        error("more than one input file");
      }
      inFileName = argv[i];
    }
  }
  if (inFileName == NULL) {
    error("no input file");
  }
  yyin = fopen(inFileName, "r");
  if (yyin == NULL) {
    error("cannot open input file '%s'", inFileName);
  }
  do {
    token = yylex();
    showToken(token);
  } while (token != 0);
  fclose(yyin);
  return 0;
}
