
#ifndef __PT_SEM_H__
#define __PT_SEM_H__

#include "pt.h"

struct _SSem
{
   u8 count;
};
typedef struct _SSem TSem;

struct _SMut
{
   u8 count;
};
typedef struct _SMut TMut;


void  PT_SemPost(TSem *this);
u8 PT_SemPend(TSem *this);
u8 PT_SemTest(TSem *this);
void PT_SemClear(TSem *this);

void PT_MutexSet(TMut *this);
void PT_MutexClear(TMut *this);
u8 PT_MutexTest(TMut *this);
u8 PT_MutexPend(TMut *this);


#define PT_SEM_Extern(A)						\
		extern  TSem A

#define PT_SEM_Create(A, B)						\
		TSem A = {B};

#define PT_SEM_Pend(A)							\
		do {									\
			PT_WAIT_UNTIL(pt, PT_SemPend(&A));	\
		} while(0)

#define PT_SEM_Post(A)							\
		PT_SemPost(&A)

#define PT_SEM_Clear(A)							\
		PT_SemClear(&A)

#define PT_SEM_Wait(A)							\
		do {									\
			PT_WAIT_UNTIL(pt, PT_SemTest(&A));	\
		} while(0)


#define PT_MUT_Extern(A,B)						\
		extern  TMut A

 #define PT_MUT_Create(A,B)						\
		TMut A={B};

#define PT_MUT_Pend(A)							\
		do {									\
			PT_WAIT_UNTIL(pt, PT_MutexPend(A));\
		} while(0)

#define PT_MUT_Post(A)							\
		PT_MutexSet(A)


#endif /* __PT_SEM_H__ */


