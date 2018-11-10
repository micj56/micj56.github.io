/*
 * except.c -- exception handling
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "error.h"
#include "except.h"


#define MAX_ENV_NEST_DEPTH	10


static jmp_buf *environments[MAX_ENV_NEST_DEPTH];
static int currentEnvironment = -1;


static char *exceptionStrings[] = {
  /* 16 */  "A bus address exception",
  /* 17 */  "A bus timeout exception",
  /* 18 */  "An illegal instruction exception",
  /* 19 */  "A privileged instruction exception",
  /* 20 */  "A trap instruction exception",
  /* 21 */  "A divide instruction exception",
  /* 22 */  "A TLB miss exception",
  /* 23 */  "A TLB double hit exception",
  /* 24 */  "A privileged address exception",
  /* 25 */  "A write protection exception"
};


char *exceptionToString(int exception) {
  exception -= 16;
  if (exception < 0 ||
      exception >= sizeof(exceptionStrings)/sizeof(exceptionStrings[0])) {
    error("exception number out of bounds");
  }
  return exceptionStrings[exception];
}


void throwException(int exception) {
  if (currentEnvironment < 0) {
    error("exception %d thrown while no environment active", exception);
  }
  longjmp(*environments[currentEnvironment], exception);
}


void pushEnvironment(jmp_buf *environment) {
  if (currentEnvironment == MAX_ENV_NEST_DEPTH - 1) {
    error("too many environments active");
  }
  currentEnvironment++;
  environments[currentEnvironment] = environment;
}


void popEnvironment(void) {
  if (currentEnvironment < 0) {
    error("cannot pop environment - none active");
  }
  currentEnvironment--;
}
