
#ifndef __PT_TIMER_H__
#define __PT_TIMER_H__
#include "pt_type.h"

struct _STimer
{
	u32 start;
	u32 interval;
};
typedef struct _STimer TTimer;

extern volatile u32 TTimer_ticks;

void PT_TimerIncrement(void);
u32 PT_TimerGet(void);
u8  PT_TimerExpired(TTimer *t);
void PT_TimerSet(TTimer *t,u32 interval);
u32 PT_TimerDiff(u32 TimeWhen);
void PT_TimerDelay(u32 Tick_Count);

void nus_delay(unsigned int num);

#define TMR_Create(A)\
        TTimer A

#define TMR_Extern(A)\
		extern TTimer A



#define PT_TIMER_DELAY(A,B)\
        PT_TimerSet(A,B);\
        PT_WAIT_UNTIL(pt,PT_TimerExpired(A))

#define TIMER_DELAY(A,B) PT_TIMER_DELAY(A,B)





#endif

