/*
 * common.h -- common definitions
 */


#ifndef _COMMON_H_
#define _COMMON_H_


#define K		1024		/* Kilo */
#define M		(K * K)		/* Mega */

#define MEMORY_SIZE	(1 * M)		/* size of main memory */
#define ROM_BASE	0x3E000000	/* physical ROM base address */
#define ROM_SIZE	(64 * K)	/* ROM size */
#define IO_BASE		0x3F000000	/* physical I/O base address */

#define IO_DEV_MASK	0x3FFFF000	/* I/O device mask */
#define IO_REG_MASK	0x00000FFF	/* I/O register mask */
#define IO_GRAPH_MASK	0x003FFFFF	/* I/O graphics mask */

#define TIMER_BASE	0x3F000000	/* physical timer base address */
#define TERM_BASE	0x3F001000	/* physical terminal base address */
#define DISK_BASE	0x3F002000	/* physical disk base address */
#define GRAPH_BASE	0x3FC00000	/* physical grahics base address */
					/* extends to end of address space */

#define PAGE_SIZE	(4 * K)		/* size of a page and a page frame */
#define OFFSET_MASK	(PAGE_SIZE - 1)	/* mask for offset within a page */

#define INSTRS_PER_MSEC	5000		/* average execution speed */


typedef int Bool;			/* truth values */

#define FALSE		0
#define TRUE		1


typedef unsigned char Byte;		/* 8 bit quantities */
typedef unsigned short Half;		/* 16 bit quantities */
typedef unsigned int Word;		/* 32 bit quantities */


#endif /* _COMMON_H_ */
