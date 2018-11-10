/*
 * varalloc.h -- variable allocation
 */


#ifndef _VARALLOC_H_
#define _VARALLOC_H_


int intByteSize(void);
int boolByteSize(void);
int refByteSize(void);

void allocVars(Absyn *program, Table *globalTable, boolean showVarAlloc);


#endif /* _VARALLOC_H_ */
