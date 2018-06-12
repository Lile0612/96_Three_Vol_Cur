
#define __PT_BUF_C__
#include "Include.h"

u8 PT_BufPutChar(TBuf *this,u8 data)
{
	u8 *p=(u8 *)this->pin;
	p += 1;

	if( p > (u8*)this->ceiling)
	{
		p =(u8 *)this->floor;
	}

	if( p ==(u8 *)this->pout )
	{
		return 0;
	}
	//memcpy(this->pin, &data,1);
	*this->pin=data;

	this->pin = p;
	this->count++;

	return 1;

}

u8 PT_BufGetChar(TBuf *this,void *pdata)
{
	u8 *p;
	if( this->pin == this->pout )
	{
		return 0;
	}
	// memcpy(pdata, this->pout, 1);
	*(u8 *)pdata=*this->pout;

	p =(u8 *)this->pout;
	p += 1;

	if( p >(u8*)this->ceiling )
	{
		p = (u8 *)this->floor;
	}
	this->pout = p;
	this->count--;
	return 1;
}


u8  PT_BufPut(TBuf *this,const void *pdata,u16 size)
{
	u8 *p=(u8 *)pdata;
	u16 i;
	for(i=0;i<size;i++)
	{
		if(!PT_BufPutChar(this,p[i]))
		{
			return 0;
		}
	}
	return 1;
}

u8  PT_BufGet(TBuf *this,void *pdata,u16 size)
{
	u8 *p;
	u16 i;

	if(this->count<size)
	{
		return 0;
	}
	p=(u8 *)pdata;

	for(i=0;i<size;i++)
	{
		if(!PT_BufGetChar(this,&p[i]))
		{
			return 0;
		}
	}
	return 1;
}





