/*
 * timer.h -- timer simulation
 */


#ifndef _TIMER_H_
#define _TIMER_H_


#define TIMER_CTRL	0	/* timer control register */
#define TIMER_DIVISOR	4	/* timer divisor register */

#define TIMER_INT	0x01	/* timer interrupt is pending */
#define TIMER_IEN	0x02	/* enable timer interrupt */

#define NUMBER_TIMERS	10	/* total number of simulation timers */


Word timerRead(Word addr);
void timerWrite(Word addr, Word data);

void timerTick(void);
void timerStart(int msec, void (*callback)(void));

void timerReset(void);
void timerInit(void);
void timerExit(void);


#endif /* _TIMER_H_ */
