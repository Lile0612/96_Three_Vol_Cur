#include "Include.h"

void PT_SemPost(TSem *this)
{
	this->count++;
}

u8 PT_SemPend(TSem *this)
{
	u8 result=0;
	if(this->count)
	{
		this->count--;
		result=1;
	}

	return result;
}

u8 PT_SemTest(TSem *this)
{
	u8 result=0;
	result=this->count;
	return result;
}

void PT_SemClear(TSem *this)
{
	this->count=0;
}

void PT_MutexSet(TMut *this)
{
	this->count=1;
}

void PT_MutexClear(TMut *this)
{
	this->count=0;
}

u8 PT_MutexTest(TMut *this)
{
	u8 ret;
	ret=this->count;
	return ret;
}

u8 PT_MutexPend(TMut *this)
{
	u8 ret=0;
	if(this->count)
	{
		this->count=0;
		ret=1;
	}
	return ret;
}

