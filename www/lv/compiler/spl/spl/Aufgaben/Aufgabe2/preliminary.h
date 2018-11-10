/*
 * preliminary.h -- preliminary definition of tokens and semantic values
 */

#define ARRAY 257
#define ELSE 258
#define IF 259
#define OF 260
#define PROC 261
#define REF 262
#define TYPE 263
#define VAR 264
#define WHILE 265
#define LPAREN 266
#define RPAREN 267
#define LBRACK 268
#define RBRACK 269
#define LCURL 270
#define RCURL 271
#define EQ 272
#define NE 273
#define LT 274
#define LE 275
#define GT 276
#define GE 277
#define ASGN 278
#define COLON 279
#define COMMA 280
#define SEMIC 281
#define PLUS 282
#define MINUS 283
#define STAR 284
#define SLASH 285
#define IDENT 286
#define INTLIT 287

typedef union {
  NoVal noVal;
  IntVal intVal;
  StringVal stringVal;
} YYSTYPE;

extern YYSTYPE yylval;
