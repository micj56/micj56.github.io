/*
 * curslib.h -- interface to curses library
 */


#ifndef _CURSLIB_H_
#define _CURSLIB_H_


#define LINE_SIZE		200
#define HISTORY_SIZE		50


int cGetChar(void);
char *cGetLine(char *prompt);
void cAddHist(char *line);

void cPrintf(char *format, ...);
void cPutChar(char c);

void cGotoXY(int x, int y);
void cClear(void);
void cHighlightOn(void);
void cHighlightOff(void);

void cInit(void);
void cExit(void);


#endif /* _CURSLIB_H_ */
