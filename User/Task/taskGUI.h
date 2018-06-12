/*! @file
********************************************************************************
<PRE>
模块名       : 任务模块
文件名       : taskGUI.h
相关文件     :
文件实现功能 : 人机交互,LED控制,键盘扫描
作者         : <--->
版本         : 1.0
--------------------------------------------------------------------------------
备注         :
--------------------------------------------------------------------------------
修改记录 :
日 期        版本   修改人         修改内容
2010/10/05   1.0    <xxxx>         创建
</PRE>
********************************************************************************

  * 版权所有(c) YYYY, <xxx>, 保留所有权利

*******************************************************************************/
#ifdef _GUILCD_
#ifndef _TASKGUI_H
#define _TASKGUI_H

/* Includes ------------------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define	N34  0x00
#define	N33  0x01           //接线方式
#define	n81  0x00           //数据格式 无校验 停止位1
#define	e81  0x01           //偶校验
#define	o81  0x02           //奇校验
#define	n82  0x03
#define Baud12  0x00        //波特率
#define Baud24  0x01
#define Baud48  0x02
#define Baud96  0x03
#define Baud192 0x04
#define rtu485  0x01
#define calib   0x02
#define CYCNO   0x00
#define CYCYES  0x01


enum EMLCD_DISP_DEFAULT
{
	DISP_UINT_U  = 0,
	DISP_UINT_I  = 1,
	DISP_UINT_P  = 2,
	DISP_UINT_PF = 3,
	DISP_UINT_EP = 4,
	DISP_UINT_THD = 5,
	DISP_UINT_DEM = 6,
};

enum EMLCD_DO_MODE
{
	DO_MODE_OFF = 0,
	DO_MODE_ALR = 1,
	DO_MODE_REM = 2,
};

enum EMLCD_AO_MODE
{
    AO_MODE_OFF = 0,
    AO_MODE_420 = 1,
	AO_MODE_020 = 2,
	AO_MODE_1220 = 3,
};

#ifdef _LEDHarm_
enum EMLCD_DO_ITEM
{
	DO_ITEM_Ua_H = 0,
	DO_ITEM_Ua_L = 1,
	DO_ITEM_Ub_H = 2,
	DO_ITEM_Ub_L = 3,
	DO_ITEM_Uc_H = 4,
	DO_ITEM_Uc_L = 5,
	DO_ITEM_Uavg_H = 6,
	DO_ITEM_Uavg_L = 7,
	DO_ITEM_Uab_H = 8,
	DO_ITEM_Uab_L = 9,
	DO_ITEM_Ubc_H = 10,
	DO_ITEM_Ubc_L = 11,
	DO_ITEM_Uca_H = 12,
	DO_ITEM_Uca_L = 13,
	DO_ITEM_ULavg_H = 14,
	DO_ITEM_ULavg_L = 15,

	DO_ITEM_Ia_H = 16,
	DO_ITEM_Ia_L = 17,
	DO_ITEM_Ib_H = 18,
	DO_ITEM_Ib_L = 19,
	DO_ITEM_Ic_H = 20,
	DO_ITEM_Ic_L = 21,
	DO_ITEM_Iavg_H = 22,
	DO_ITEM_Iavg_L = 23,

	DO_ITEM_Pa_H = 24,
	DO_ITEM_Pa_L = 25,
	DO_ITEM_Pb_H = 26,
	DO_ITEM_Pb_L = 27,
	DO_ITEM_Pc_H = 28,
	DO_ITEM_Pc_L = 29,
	DO_ITEM_P_H = 30,
	DO_ITEM_P_L = 31,

	DO_ITEM_Qa_H = 32,
	DO_ITEM_Qa_L = 33,
	DO_ITEM_Qb_H = 34,
	DO_ITEM_Qb_L = 35,
	DO_ITEM_Qc_H = 36,
	DO_ITEM_Qc_L = 37,
	DO_ITEM_Q_H = 38,
	DO_ITEM_Q_L = 39,

	DO_ITEM_Sa_H = 40,
	DO_ITEM_Sa_L = 41,
	DO_ITEM_Sb_H = 42,
	DO_ITEM_Sb_L = 43,
	DO_ITEM_Sc_H = 44,
	DO_ITEM_Sc_L = 45,
	DO_ITEM_S_H = 46,
	DO_ITEM_S_L = 47,

	DO_ITEM_PFa_H = 48,
	DO_ITEM_PFa_L = 49,
	DO_ITEM_PFb_H = 50,
	DO_ITEM_PFb_L = 51,
	DO_ITEM_PFc_H = 52,
	DO_ITEM_PFc_L = 53,
	DO_ITEM_PF_H = 54,
	DO_ITEM_PF_L = 55,

	DO_ITEM_F_H = 56,
	DO_ITEM_F_L = 57,
	DO_ITEM_UTHa_H = 58,
	DO_ITEM_UTHb_H = 59,
	DO_ITEM_UTHc_H = 60,
	DO_ITEM_UTH_H = 61,

    DO_ITEM_ITHa_H = 62,
	DO_ITEM_ITHb_H = 63,
	DO_ITEM_ITHc_H = 64,
	DO_ITEM_ITH_H = 65,

	DO_ITEM_IMBU_H = 66,
	DO_ITEM_IMBI_H = 67,

	//DO_ITEM_Io_H = 68,
	//DO_ITEM_Io_L = 69,

	DO_ITEM_DI1_H = 68,
	DO_ITEM_DI2_H = 69,
	DO_ITEM_DI3_H = 70,
	DO_ITEM_DI4_H = 71,
	DO_ITEM_DI1_L = 72,
	DO_ITEM_DI2_L = 73,
	DO_ITEM_DI3_L = 74,
	DO_ITEM_DI4_L = 75,
};
#else
enum EMLCD_DO_ITEM
{
	DO_ITEM_Ua_H = 0,
	DO_ITEM_Ua_L = 1,
	DO_ITEM_Ub_H = 2,
	DO_ITEM_Ub_L = 3,
	DO_ITEM_Uc_H = 4,
	DO_ITEM_Uc_L = 5,
	DO_ITEM_Uavg_H = 6,
	DO_ITEM_Uavg_L = 7,
	DO_ITEM_Uab_H = 8,
	DO_ITEM_Uab_L = 9,
	DO_ITEM_Ubc_H = 10,
	DO_ITEM_Ubc_L = 11,
	DO_ITEM_Uca_H = 12,
	DO_ITEM_Uca_L = 13,
	DO_ITEM_ULavg_H = 14,
	DO_ITEM_ULavg_L = 15,

	DO_ITEM_Ia_H = 16,
	DO_ITEM_Ia_L = 17,
	DO_ITEM_Ib_H = 18,
	DO_ITEM_Ib_L = 19,
	DO_ITEM_Ic_H = 20,
	DO_ITEM_Ic_L = 21,
	DO_ITEM_Iavg_H = 22,
	DO_ITEM_Iavg_L = 23,

	DO_ITEM_Pa_H = 24,
	DO_ITEM_Pa_L = 25,
	DO_ITEM_Pb_H = 26,
	DO_ITEM_Pb_L = 27,
	DO_ITEM_Pc_H = 28,
	DO_ITEM_Pc_L = 29,
	DO_ITEM_P_H = 30,
	DO_ITEM_P_L = 31,

	DO_ITEM_Qa_H = 32,
	DO_ITEM_Qa_L = 33,
	DO_ITEM_Qb_H = 34,
	DO_ITEM_Qb_L = 35,
	DO_ITEM_Qc_H = 36,
	DO_ITEM_Qc_L = 37,
	DO_ITEM_Q_H = 38,
	DO_ITEM_Q_L = 39,

	DO_ITEM_Sa_H = 40,
	DO_ITEM_Sa_L = 41,
	DO_ITEM_Sb_H = 42,
	DO_ITEM_Sb_L = 43,
	DO_ITEM_Sc_H = 44,
	DO_ITEM_Sc_L = 45,
	DO_ITEM_S_H = 46,
	DO_ITEM_S_L = 47,

	DO_ITEM_PFa_H = 48,
	DO_ITEM_PFa_L = 49,
	DO_ITEM_PFb_H = 50,
	DO_ITEM_PFb_L = 51,
	DO_ITEM_PFc_H = 52,
	DO_ITEM_PFc_L = 53,
	DO_ITEM_PF_H = 54,
	DO_ITEM_PF_L = 55,

	DO_ITEM_F_H = 56,
	DO_ITEM_F_L = 57,

	DO_ITEM_DI1_H = 58,
	DO_ITEM_DI2_H = 59,
	DO_ITEM_DI3_H = 60,
	DO_ITEM_DI4_H = 61,
	DO_ITEM_DI1_L = 62,
	DO_ITEM_DI2_L = 63,
	DO_ITEM_DI3_L = 64,
	DO_ITEM_DI4_L = 65,

	DO_ITEM_UTHa_H = 70,
	DO_ITEM_UTHb_H = 71,
	DO_ITEM_UTHc_H = 72,
	DO_ITEM_UTH_H = 73,

    DO_ITEM_ITHa_H = 74,
	DO_ITEM_ITHb_H = 75,
	DO_ITEM_ITHc_H = 76,
	DO_ITEM_ITH_H = 77,

	DO_ITEM_IMBU_H = 78,
	DO_ITEM_IMBI_H = 79,
};
#endif

enum EMLCD_AO_ITEM
{
	AO_ITEM_UA =  0,
	AO_ITEM_UB =  1,
	AO_ITEM_UC =  2,
	AO_ITEM_UAB = 3,
	AO_ITEM_UBC = 4,
	AO_ITEM_UCA = 5,
	AO_ITEM_IA =  6,
	AO_ITEM_IB =  7,
	AO_ITEM_IC =  8,
	AO_ITEM_PA =  9,
	AO_ITEM_PB =  10,
	AO_ITEM_PC =  11,
	AO_ITEM_P  =  12,
	AO_ITEM_QA =  13,
	AO_ITEM_QB =  14,
	AO_ITEM_QC =  15,
	AO_ITEM_Q  =  16,
	AO_ITEM_SA =  17,
    AO_ITEM_SB =  18,
	AO_ITEM_SC =  19,
	AO_ITEM_S  =  20,
	AO_ITEM_PFA = 21,
	AO_ITEM_PFB = 22,
	AO_ITEM_PFC = 23,
	AO_ITEM_PF  = 24,
	AO_ITEM_F   = 25,
};

#define BrightLevel_Off 0x00//亮度等级
#define BrightLevel_1 0x01
#define BrightLevel_2 0x02
#define BrightLevel_3 0x03
#define BrightLevel_4 0x04
#define BrightLevel_5 0x05

#define SaveYes    0x01
#define SaveNO     0x02
#define ReadMode   0x01
#define ProgMode   0x02


#define MAX_NetMode        1	  // 接线方式最大值
#define MIN_NetMode        0      // 接线方式最小值

#define MAX_PT          9999	  // PT变比最大值
#define MIN_PT		       1      // PT变比最小值

#define MAX_CT          9999	  // CT变比最大值
#define MIN_CT		       1      // CT变比最小值

#define MAX_Addr         247	  // 数显表地址最大值
#define MIN_Addr		   0      // 数显表地址最小值

#define MAX_Baud           4	  // 波特率最大值
#define MIN_Baud		   0      // 波特率最小值

#define MAX_DataFormat     3	  // 数据格式最大值
#define MIN_DataFormat	   0      // 数据格式最小值

#define MAX_DataProt       2	  // 数据格式最大值
#define MIN_DataProt	   1      // 数据格式最小值

#define MAX_Cycle          2	  // 显示切换时间最大值
#define MIN_Cycle		   1      // 显示切换时间最小值

#define MAX_Code        9999	  // 密码最大值
#define MIN_Code 		   1	  // 密码最小值

#define MAX_BlackTime    255	  // 亮度等级最大值
#define MIN_BlackTime	   1      // 亮度等级最小值

#define MAX_Alarm        200	  // 闪烁报警最大值
#define MIN_Alarm		   0      // 闪烁报警最小值

#define MAX_Disp           8	  // 默认显示最大值
#define MIN_Disp	       0      // 默认显示最小值

#define MAX_Ver         9999	  // 软件版本最大值
#define MIN_Ver	           0      // 软件版本最小值

#define MAX_DO_Mode       2
#define MIN_DO_Mode       0

#define MAX_DO_Time    9999
#define MIN_DO_Time       0

#define MAX_DO_Item      80
#define MIN_DO_Item       0

#define MAX_DO_Ual     9999
#define MIN_DO_Ual        0

#define MAX_DO_Hys     9999
#define MIN_DO_Hys        0

#define MAX_DO_Dely    9999
#define MIN_DO_Dely       0

#define MAX_AO_Mode       3
#define MIN_AO_Mode       0

#define MAX_AO_Item      26
#define MIN_AO_Item       0

#define MAX_AO_DS      9999
#define MIN_AO_DS         0

#define MAX_AO_FS      9999
#define MIN_AO_FS         0

/* Exported functions ------------------------------------------------------- */

extern BOOL g_fftEnable;
extern u8 DispMode;
extern u8 LEDLEVEL;
extern volatile u8 vg_Display_Value[16]; // 显示保存缓存

void Task_GUI_init(void);
void Task_Timer_init(void);
void GUI_Timer_On(void);
void DispChar(u8 num,u8 ch1, u8 ch2, u8 ch3, u8 ch4);
void DispBitChar(u8 Index, u8 ch);

void Task_GUI_Function(void);
void Task_Timer_Function(void);
void GUI_SetComFlag(void);
void GUI_SetDispCtrl(u8 Group, u8 State, u32 DlyNum);
void DispValue(u8 num,s32 DispNum, u8 Decimal, u8 DispWidth, BOOL bFlash, u8 FlashDec, BOOL bEnableDisp, BOOL bSet);
s32 JudgeSetNewValue(BOOL *pUpdataFlag, s32 OldValue, s32 *pNewValue, s32 MinValue, s32 MaxValue);
void GetSetNewValue(void);
void Disp_IO_Status(u8 InStatus,u8 OutStatus);

#endif /* _TASKGUI_H */
#endif

//===========================================================================
// No more.
//===========================================================================



