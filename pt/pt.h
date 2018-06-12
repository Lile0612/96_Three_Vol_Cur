
#ifndef __PT_H__
#define __PT_H__

#include "pt_config.h"
#include "pt_type.h"
#include "pt_timer.h"
#include "pt_lc.h"
#include "pt_critical.h"
#include "pt_sem.h"
#include "pt_buf.h"


struct _Spt
{
  volatile  lc_t lc;
  volatile  u8 enable;
  u8 (*thread) (struct _Spt *pt);
  //u8 (*execute)(struct _Spt *pt);
};
typedef struct _Spt Tpt;

u8  pt_execute(Tpt *pt);
void   pt_lc_set(Tpt *pt,u16 t);
u16 pt_lc_get(Tpt *pt);
void   pt_enable_set(Tpt*pt,u8 t);
u8  pt_enable_get(Tpt *pt);
u8  pt_thread (struct _Spt *pt);

typedef u8 (*TThread)(Tpt *pt);

#define PT_NAME(A) pt##_##A
#define PT(A)   (&PT_NAME(A))

#define THR_Create(A)\
  u8 A(Tpt *pt);\
    Tpt pt##_##A={0,1,A};

#define THR_Extern(A)\
 extern u8 A(Tpt *pt);\
 extern Tpt pt##_##A;



#define PT_WAITING   0
#define PT_DISABLED  1
#define PT_ENDED     2


#define PT_BEGIN(pt) {switch(pt_lc_get(pt)) { case 0:
#define PT_END(pt)   }  LC_INIT(pt); return PT_ENDED; }


#define PT_WAIT_UNTIL(pt, condition)	        \
  do {						\
    LC_SET(pt);				\
    if(!(condition)) {				\
      return PT_WAITING;			\
    }						\
  } while(0)


#define PT_WAIT_WHILE(pt, cond)  PT_WAIT_UNTIL((pt), !(cond))


#define PT_RESTART(pt)				\
  do {						\
    PT_INIT(pt);				\
    return PT_WAITING;			\
  } while(0)


#define PT_RETURN(pt)				\
  do {						\
    LC_SET(pt);		                       \
    return PT_WAITING;			\
  } while(0)


#define PT_SUSPEND(pt) \
  do(\
    (pt)->enable_set(0);\
    return PT_DISABLED;			\
  } while(0)


#define PT_RESUME(pt) \
  do(\
    (pt)->enable_set(1);\
  } while(0)



#endif /* __PT_H__ */

