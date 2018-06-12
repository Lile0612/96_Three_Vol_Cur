/*! @file
********************************************************************************
<PRE>
模 块 名     : 主程序
文 件 名     : sysMain.c
相关文件      :
文件实现功能 :
作者         : < 612 >
版本         : 1.0
--------------------------------------------------------------------------------
备注         : 数显表
--------------------------------------------------------------------------------
修改记录 :
  日 期        版本      修改人         修改内容
2018/02/01   1.0    < 612 >        创建
2018/03/17   1.1    < 612 >      LED显示开机画面黑屏问题
</PRE>
********************************************************************************

  * 版权所有(c) YYYY, <xxxx>, 保留所有权利

*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "Include.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

void App_Init(void)
{

}

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
#ifdef DEBUG
	debug();
#endif

  	Bsp_Init();
    PT_TimerDelay(50);
	Task_GUI_init();
	PT_TimerDelay(500);
	LoadParam();//读Flash 系统初始化标志位.
	bsp_InitRN8023();
    USART_Configuration();
#ifdef _GUILCD_
    LCD_BG_H;
#endif
    SCH_Init_T();

    // 加入任务
	SCH_Add_Task(Task_RS232_Function, 0, TASK_IDLE_PROTOCOL);		// 485串口
	SCH_Add_Task(Task_Power_Function, 500, TASK_IDLE_RNUpData);	// 数据刷新
	SCH_Add_Task(Task_Protect_Function, 1000, TASK_IDLE_SVC);	// 保护告警
	SCH_Add_Task(Task_Extra_Function, 200,TASK_IDLE_EXTRA);
	SCH_Add_Task(Task_GUI_Function, 500, TASK_IDLE_GUI);		// GUI

	// 开始调度
	SCH_Start();
	while(1)
	{
		SCH_RUN_Tasks();
	}
}


