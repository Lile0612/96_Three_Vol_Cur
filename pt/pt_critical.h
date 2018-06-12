#ifndef __PT_CRITICAL_H__
#define __PT_CRITICAL_H__


u8 PT_CriticalReadByte(volatile u8 * pdata);
u16 PT_CriticalReadWord(volatile u16 * pdata);
u32 PT_CriticalReadDword(volatile u32 * pdata);

void PT_CriticalWriteWord(volatile u16 * pdata,u16 data);
void PT_CriticalWriteByte(volatile u8 * pdata,u8 data);
void PT_CriticalWriteDword(volatile u32 * pdata,u32 data);

u8 __iSeiRetVal(void);
u8 __iCliRetVal(void);

#endif //__PT_CRITICAL_H__

