#include "Include.h"


u8 pt_execute(Tpt *this)
{
	if(!pt_enable_get(this))
	{
		return PT_DISABLED;
	}
	return this->thread(this);
}


void   pt_lc_set(Tpt *this,u16 t)
{
	this->lc=t;
}

u16 pt_lc_get(Tpt *this)
{
	u16 t;
	t=this->lc;
	return t;
}

void   pt_enable_set(Tpt *this,u8 t)
{
	this->enable=t;
}

u8  pt_enable_get(Tpt *this)
{
	u8 temp;
	temp=this->enable;
	return temp;
}

