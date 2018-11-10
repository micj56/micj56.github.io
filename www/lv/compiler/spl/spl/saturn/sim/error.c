/*
 * error.c -- error handler
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "curslib.h"
#include "error.h"


void error(char *fmt, ...) {
  va_list ap;

  cExit();
  va_start(ap, fmt);
  fprintf(stderr, "Error: ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
  exit(1);
}
