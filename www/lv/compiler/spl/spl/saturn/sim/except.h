/*
 * except.h -- exception handling
 */


#ifndef _EXCEPT_H_
#define _EXCEPT_H_


#define EXC_BUS_ADDRESS	16	/* bus address exception */
#define EXC_BUS_TIMEOUT	17	/* bus timeout exception */
#define EXC_ILL_INSTRCT	18	/* illegal instruction exception */
#define EXC_PRV_INSTRCT	19	/* privileged instruction exception */
#define	EXC_TRAP	20	/* trap instruction exception */
#define EXC_DIVIDE	21	/* divide instruction exception */
#define EXC_TLB_MISS	22	/* TLB miss exception */
#define EXC_TLB_DBLHIT	23	/* TLB double hit exception */
#define EXC_PRV_ADDRESS	24	/* privileged address exception */
#define EXC_WRT_PROTECT	25	/* write protection exception */


char *exceptionToString(int exception);
void throwException(int exception);
void pushEnvironment(jmp_buf *environment);
void popEnvironment(void);


#endif /* _EXCEPT_H_ */
