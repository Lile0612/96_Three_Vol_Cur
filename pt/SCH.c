/*! @file
********************************************************************************
<PRE>
Ä£¿éÃû       : µ÷¶ÈÆ÷ÄÚºËº¯Êý
ÎÄ¼þÃû       : SCH.c
Ïà¹ØÎÄ¼þ     :
ÎÄ¼þÊµÏÖ¹¦ÄÜ : ÈÎÎñµ÷¶È
×÷Õß         : <l¡¢>
°æ±¾         : 1.0
--------------------------------------------------------------------------------
±¸×¢         : µçÁ÷±í
--------------------------------------------------------------------------------
ÐÞ¸Ä¼ÇÂ¼ :
ÈÕ ÆÚ        °æ±¾   ÐÞ¸ÄÈË         ÐÞ¸ÄÄÚÈÝ
2017/03/22   1.0    <l¡¢>           ´´½¨
</PRE>
********************************************************************************

  * °æÈ¨ËùÓÐ(c) YYYY, <xxx>, ±£ÁôËùÓÐÈ¨Àû

*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "Include.h"

/*----------------------------------------------------------------------------*/

sTask SCH_tasks_G[SCH_MAX_TASKS];   //ÈÎÎñµ÷¶ÈÆ÷Êý×é

void SCH_Init_T(void)
{
	u8 i;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	for (i = 0; i < SCH_MAX_TASKS; i++)
    {
        SCH_Delete_Task(i);				//É¾³ý²»±ØÒªµÄÈÎÎñ
    }
	TIM_DeInit(TIM14);
	TIM_TimeBaseStructure.TIM_Period = 3999;       //¶¨Ê±Æ÷Öµ
	TIM_TimeBaseStructure.TIM_Prescaler = 11;       //Ê±ÖÓ·ÖÆµÊý ¶¨Ê±1ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
}

void SCH_Start(void)
{
	/* TIM enable counter */
	TIM_Cmd(TIM14, ENABLE);

	/* TIM IT enable */
	TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);
}

u16 g_IWDGCount = 0; //Î¹¹·¼ÆÊý
void SIGNAL_IRQHandler(void)
{
    u8 Index;
	if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
	{
		for ( Index = 0;  Index < SCH_MAX_TASKS;  Index++ )
		{
			if (SCH_tasks_G[Index].pTask)
			{
				if (SCH_tasks_G[Index].Delay == 0)
				{
					SCH_tasks_G[Index].RunMe += 1;

					if (SCH_tasks_G[Index].Period)
					{
						SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
					}
				}
				else
				{
					SCH_tasks_G[Index].Delay -= 1;
				}
			}
		}

		TIM_ClearITPendingBit(TIM14, TIM_IT_Update );
		g_IWDGCount++;
		if (g_IWDGCount>=120)
		{
			//IWDG_ReloadCounter(); //ÊÖ¶¯Î¹¹·
			g_IWDGCount = 0;
		}
	}
}

void SCH_RUN_Tasks(void)  //ÈÎÎñÖ´ÐÐ
 {
   u8 Index;

   for (Index = 0; Index < SCH_MAX_TASKS; Index++)
   {
      if (SCH_tasks_G[Index].RunMe > 0)
      {
         (*SCH_tasks_G[Index].pTask)();

         SCH_tasks_G[Index].RunMe -= 1;

         if (SCH_tasks_G[Index].Period == 0)
         {
            SCH_Delete_Task(Index);
         }
      }
   }
}

u8 SCH_Add_Task(void (*pFunction)(void),  //Ôö¼1Ó
                    u16 DELAY,
                    u16 PERIOD)
{
   u8 Index = 0;

   while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
   {
      Index++;
   }

   if (Index == SCH_MAX_TASKS)
   {
      return SCH_MAX_TASKS;
    }

   SCH_tasks_G[Index].pTask  = pFunction;
   SCH_tasks_G[Index].Delay  = DELAY;
   SCH_tasks_G[Index].Period = PERIOD;
   SCH_tasks_G[Index].RunMe  = 0;

   return Index;
}

u8 SCH_Delete_Task( u8 TASK_INDEX)        //É¾³ý
{
   SCH_tasks_G[TASK_INDEX].pTask   = 0x0000;
   SCH_tasks_G[TASK_INDEX].Delay   = 0;
   SCH_tasks_G[TASK_INDEX].Period  = 0;
   SCH_tasks_G[TASK_INDEX].RunMe   = 0;

   return 0;
}

