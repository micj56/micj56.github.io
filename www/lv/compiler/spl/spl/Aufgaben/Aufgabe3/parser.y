%{

/*
 * parser.y -- SPL parser specification
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "utils.h"
#include "scanner.h"
#include "parser.h"

%}

%union {
  NoVal noVal;
  IntVal intVal;
  StringVal stringVal;
}

%token			ARRAY

%start			program


%%


program			: ??
			;


%%


void yyerror(char *msg) {
  error("%s in line %d", msg, yylval.noVal.line);
}
