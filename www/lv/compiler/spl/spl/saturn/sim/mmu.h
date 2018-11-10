/*
 * mmu.h -- MMU simulation
 */


#ifndef _MMU_H_
#define _MMU_H_


#define TLB_SIZE	32		/* total number of TLB entries */
#define TLB_MASK	(TLB_SIZE - 1)
#define TLB_FIXED	4		/* number of fixed TLB entries */


typedef struct {
  Word page;
  Word frame;
} TLB_Entry;


Word mmuReadWord(Word vAddr, Bool userMode);
Half mmuReadHalf(Word vAddr, Bool userMode);
Byte mmuReadByte(Word vAddr, Bool userMode);
void mmuWriteWord(Word vAddr, Word data, Bool userMode);
void mmuWriteHalf(Word vAddr, Half data, Bool userMode);
void mmuWriteByte(Word vAddr, Byte data, Bool userMode);

Word mmuGetIndex(void);
void mmuSetIndex(Word value);
Word mmuGetEntryHi(void);
void mmuSetEntryHi(Word value);
Word mmuGetEntryLo(void);
void mmuSetEntryLo(Word Value);
Word mmuGetBadAddr(void);
void mmuSetBadAddr(Word Value);

void mmuTbs(void);
void mmuTbwr(void);
void mmuTbri(void);
void mmuTbwi(void);

TLB_Entry mmuGetTLB(int index);
void mmuSetTLB(int index, TLB_Entry tlbEntry);

void mmuReset(void);
void mmuInit(void);
void mmuExit(void);


#endif /* _MMU_H_ */
