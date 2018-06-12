
#ifndef __LC_SWITCH_H__
#define __LC_SWITCH_H__

//typedef unsigned short lc_t;



//#define LC_INIT(s) ATOMIC(){ s = 0;}
//#define LC_RESUME(s) switch(CriticalReadWord(&s)) { case 0:
//#define LC_SET(s) ATOMIC(){ s = __LINE__;} case __LINE__:


#define LC_INIT(pt)   pt_lc_set(pt,0)
#define LC_SET(pt) pt_lc_set(pt,__LINE__);case __LINE__:


#define LC_END(s) }

#endif /* __LC_SWITCH_H__ */

/** @} */
