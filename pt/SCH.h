#ifndef _SCH_H_
#define _SCH_H_

#include "Macro.h"

#define RETURN_NORMAL             0
#define RETURN_ERROR              1

#define TASK_IDLE_GUI           40  //显示
#define TASK_IDLE_EXTRA         100 //附加功能
#define TASK_IDLE_PROTOCOL      20  //判断串口接收结束
#define TASK_IDLE_RNUpData      200 //读取数据
#define TASK_IDLE_Handle        500 //校表
#define TASK_IDLE_SVC           100 //保护

// 调度器支持的任务个数,用户在调用调度器的时候必须设置
#define SCH_MAX_TASKS             6
typedef struct data
{
   void (*pTask)(void);
   s16 Delay;
   u16 Period;
   u8 RunMe;
} sTask;

// 调度器内核
// 本文件所有函数
//*****************************************************
void SCH_Init_T(void);
void SCH_Start(void);
// 调度器对外部接口
void SIGNAL_IRQHandler(void);
void SCH_RUN_Tasks(void);
u8 SCH_Add_Task(void (*pFunction)(void), u16 DELAY, u16 PERIOD);
u8 SCH_Delete_Task(u8 TASK_INDEX);

#endif /* _SCH_H */

//===========================================================================
// No more.
//===========================================================================



