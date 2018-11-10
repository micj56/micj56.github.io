/*
 * symbol.h -- symbol management
 */


#ifndef _SYMBOL_H_
#define _SYMBOL_H_


#define INITIAL_HASH_SIZE	100	/* will be increased to next prime */


typedef struct symbol {
  char *string;			/* external representation of symbol */
  unsigned stamp;		/* unique random stamp for external use */
  unsigned hashValue;		/* hash value of string, internal use */
  struct symbol *next;		/* symbol chaining, internal use */
} Symbol;


Symbol *symbol(char *string);
char *symbolString(Symbol *symbol);
unsigned symbolStamp(Symbol *symbol);


#endif /* _SYMBOL_H_ */
