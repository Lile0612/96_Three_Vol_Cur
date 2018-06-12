#ifndef __PT_BUF_H__
#define __PT_BUF_H__

struct _SBuf
{
   	u8 *pin;
	u8 *pout;
	u8 *floor;
	u8 *ceiling;
	u16 count;
};

typedef struct _SBuf TBuf;

u8 PT_BufPutChar(TBuf *this,u8 data);
u8 PT_BufGetChar(TBuf *this,void *pdata);

u8 PT_BufPut(TBuf *this,const void *pdata,u16 size);
u8 PT_BufGet(TBuf *this,void *pdata,u16 size);

u16 PT_BufQuery(TBuf *this);
void PT_BufFlush(TBuf *this);
//  TBuf(void *pdata,u16 size);


#define PT_BUF_NAME(A) PT_buf_##A
#define PT_BUF_Create(A, n)\
	u8 PT_BUF_NAME(A)[n+1]; \
        TBuf A={&PT_BUF_NAME(A)[0],&PT_BUF_NAME(A)[0],&PT_BUF_NAME(A)[0],&PT_BUF_NAME(A)[n],0};

#define PT_BUF_Extern(A, n)\
  extern  u8 PT_BUF_NAME(A)[n+1]; \
  extern  TBuf A


#define PT_BUF_Put(A,B,C)\
  PT_BufPut(A,B,C)

#define PT_BUF_Get(A,B,C)\
  PT_BufGet(A,B,C)

#define PT_BUF_PutChar(A,B)\
  PT_BufPutChar(A,B)

#define PT_BUF_GetChar(A,B)\
  PT_BufGetChar(A,B)



#ifdef __PT_BUF_C__

#endif


#endif //__PT_BUF_H__

