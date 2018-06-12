/*! @file
********************************************************************************
<PRE>
模块名       : 附加功能
文件名       : taskExtra.h
相关文件     :
文件实现功能 : DI DO AO
作者         : <--->
版本         : 1.0
--------------------------------------------------------------------------------
备注         :
--------------------------------------------------------------------------------
修改记录 :
日 期        版本   修改人         修改内容
2017/10/28   1.0    <lile>           创建
</PRE>
********************************************************************************

  * 版权所有(c) YYYY, <xxx>, 保留所有权利

*******************************************************************************/

#include "Macro.h"

#ifndef _TASKEXTRA_H
#define _TASKEXTRA_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
#define DIN_NUM					0x04 // 开关量输入数量
#define OUT_NUM					0x04 // 开关量输出数量
//////////////////////////////////////////////////////////////////////////
// 以下为数据
__packed struct  AdjustVolState_Bit {   // bit  description
    // 报警状态
    u32 Un_H:1;         // 0    相电压上限
    u32 Un_L:1;         // 1    相电压下限
    u32 UL_H:1;         // 2    线电压上限
    u32 UL_L:1;         // 3    线电压下限
    u32 I_H:1;          // 4    电流上限
    u32 I_L:1;          // 5    电流下限
    u32 I0_H:1;         // 6    零线电流上限
    u32 I0_L:1;         // 7    零线电流下限
    u32 P_H:1;          // 8    有功上限
    u32 P_L:1;          // 9    有功下限
    u32 Q_H:1;          // 10   无功上限
    u32 Q_L:1;          // 11   无功下限
    u32 S_H:1;          // 12   视在上限
    u32 S_L:1;          // 13   视在下限
    u32 PF_H:1;         // 14   功率因数上限
    u32 PF_L:1;         // 15   功率因素下限
    u32 F_H:1;          // 16   频率上限
    u32 F_L:1;          // 17   频率下限
    u32 UTH_H:1;        // 18   电压谐波上限
    u32 UTH_L:1;        // 19   电压谐波下限
    u32 ITH_H:1;        // 20   电流谐波上限
    u32 ITH_L:1;        // 21   电流谐波下限
    u32 Align32:10;      // 对齐用

};

__packed union AdjustVolState_Reg
{
    u32               all;
    struct  AdjustVolState_Bit  Bit;
};

// 智能电容器数据
__packed typedef struct {
    u8 SVC_ActStatus[3];               // 3Cap
    u8 Signal_Status;                   // 信号状态
    union AdjustVolState_Reg AdjustVolState;
}Device_DataStruct;

typedef enum
{
	Dout_ONE = 0,
	Dout_TWO = 1,
	Dout_THREE = 2,
	Dout_FOUR = 3,
}emDoutNum;


extern u8 Out_Rem_Enble[3];
extern u8 DinStatus_Disp;
extern u8 OutStatus_Disp;
extern BOOL g_Out_Status[OUT_NUM];
extern u8 RemOutCtrl;
extern u8 g_Din_Status[DIN_NUM];
extern u8 g_Din_BIT[DIN_NUM];




void Task_Extra_Function(void);







#endif /* _TASKEXTRA_H */

//===========================================================================
// No more.
//===========================================================================

