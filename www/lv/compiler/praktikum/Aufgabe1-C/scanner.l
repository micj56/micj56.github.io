%{

/*
 * scanner.l -- SPL scanner specification
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "utils.h"
#include "scanner.h"
#include "preliminary.h"

YYSTYPE yylval;
static int lineNumber = 1;

%}


%%


%%


int yywrap(void) {
  return 1;
}


void showToken(int token) {
}
