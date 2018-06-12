
#define __PT_CRITICAL_C__
#include "Include.h"


void PT_CriticalIncByte(volatile u8 * pdata)
{
	pdata[0]++ ;
}

void PT_CriticalDecByte(volatile u8 * pdata)
{
	pdata[0]--;
}


u8 PT_CriticalReadByte(volatile u8 * pdata)
{
    u8 result;
	result=pdata[0];
    return result;
}

u16 PT_CriticalReadWord(volatile u16 * pdata)
{
	u16 result;
	result=pdata[0];
	return result;
}

u32 PT_CriticalReadDword(volatile u32 * pdata)
{
	u32 result;
	result=pdata[0];
	return result;
}

void PT_CriticalWriteByte(volatile u8 * pdata,u8 data)
{
	pdata[0]=data ;
}

void PT_CriticalWriteWord(volatile u16 * pdata,u16 data)
{
	pdata[0]=data ;
}


void PT_CriticalWriteDword(volatile u32 * pdata,u32 data)
{
	pdata[0]=data ;
}

