/*
 * a.out.h -- file header for executables
 */


#ifndef _A_OUT_H_
#define _A_OUT_H_


#define EXEC_MAGIC	0x3AE82DD4


typedef struct {
  unsigned int magic;		/* must be EXEC_MAGIC */
  unsigned int csize;		/* code size in bytes */
  unsigned int dsize;		/* initialized data size in bytes */
  unsigned int bsize;		/* uninitialized data size in bytes */
} ExecHeader;


#endif /* _A_OUT_H_ */
