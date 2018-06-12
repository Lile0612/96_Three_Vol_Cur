/*
*****************************************************************************************************
*
*	模块名称 : 应用程序参数模块
*	文件名称 : param.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*****************************************************************************************************
*/
#include "Macro.h"

#ifndef __PARAM_H
#define __PARAM_H


/* 下面2行宏只能选择其一 */
//#define PARAM_SAVE_TO_EEPROM			/* 参数存储到外部的EEPROM (AT24C128) */
#define PARAM_SAVE_TO_FLASH		    /* 参数存储到CPU内部Flash */

#ifdef PARAM_SAVE_TO_EEPROM
	#define PARAM_ADDR	0	    /* 参数区地址 */
#endif

#ifdef PARAM_SAVE_TO_FLASH
	//#define PARAM_ADDR	 ADDR_FLASH_SECTOR_3	    /* 0x0800C000 中间的16KB扇区用来存放参数 */
	//#define PARAM_ADDR	 ADDR_FLASH_SECTOR_11		/* 0x080E0000 Flash最后128K扇区用来存放参数 */
#endif

#define PARAM_VER   0x1001	    /* 参数版本 */

__packed typedef struct //校表系数
{
    float Kram[3];

    /* 模拟量 */
    float Ana_Pwm1_5h;
    float Ana_Pwm1_2k;
    float Ana_Pwm1_ki;

    float Ana_Pwm2_5h;
    float Ana_Pwm2_2k;
    float Ana_Pwm2_ki;       // 100字节
}
RN_COEFF;

/* 控制参数 */
__packed typedef struct
{
    u8 NetMode;         //基本控制参数
    u8 Addr485;
    u8 DataFormat;
    u8 DataProtocol;
    u8 Cycle;
    u8 BlackTime;
    u8 DefaultDisp;
    u8 tmpAlign;
    u16 PTNum;
    u16 CTNum;
    u16 Baud485;
    u16 Alarm;
    u16 Code;           // 18

    u8 DO1_Mode;        // DO1
    u8 DO1_Item;
    u16 DO1_Time;
    u16 DO1_UAL;
    u16 DO1_HYS;
    u16 DO1_Delay;      // 10

    u8 DO2_Mode;        // DO2    // 继电器工作模式
    u8 DO2_Item;                // 脉冲宽度
    u16 DO2_Time;               // 报警项目
    u16 DO2_UAL;                // 报警值
    u16 DO2_HYS;                // 回滞值
    u16 DO2_Delay;      // 10     // 延时

    u8 DO3_Mode;        // DO3
    u8 DO3_Item;
    u16 DO3_Time;
    u16 DO3_UAL;
    u16 DO3_HYS;
    u16 DO3_Delay;      // 10

    u8 DO4_Mode;        // DO4
    u8 DO4_Item;
    u16 DO4_Time;
    u16 DO4_UAL;
    u16 DO4_HYS;
    u16 DO4_Delay;      // 10

    u8 AO1_Mode;        // AO1
    u8 AO1_Item;
    u16 AO1_DS;
    u16 AO1_FS;

    u8 AO2_Mode;        // AO2
    u8 AO2_Item;
    u16 AO2_DS;
    u16 AO2_FS;         //6

	u16 INIT;
	u8 AdjustFinishFlag;    // 10 总74
	u8 AnaCalibFlag;
}SX_PARAM;


/* 全局参数 */
__packed typedef struct
{

    SX_PARAM CtrlParam;
	RN_COEFF RN_Coefficients;
}
PARAM_T;

/*------ ModeBus-RTU ------*/
enum EMPrimaryData_Index
{
	Index_1_Ua = 0x06,
	Index_1_Ub = 0x07,
	Index_1_Uc = 0x08,
	Index_1_Uab = 0x09,
	Index_1_Ubc = 0x0A,
	Index_1_Uca = 0x0B,
	Index_1_Ia = 0x0C,
	Index_1_Ib = 0x0D,
	Index_1_Ic = 0x0E,
	Index_1_Pa = 0x0F,
	Index_1_Pb = 0x10,
	Index_1_Pc = 0x11,
	Index_1_P  = 0x12,
	Index_1_Qa = 0x13,
	Index_1_Qb = 0x14,
	Index_1_Qc = 0x15,
	Index_1_Q  = 0x16,
	Index_1_Sa = 0x17,
	Index_1_Sb = 0x18,
	Index_1_Sc = 0x19,
	Index_1_S  = 0x1A,
	Index_1_PF = 0x1B,
	Index_1_Hz = 0x1C,
	Index_1_PEPT = 0x1D,
	Index_1_NEPT = 0x1E,
	Index_1_PEQT = 0x1F,
	Index_1_NEQT = 0x20,
	Index_1_Uavr = 0x21,
	Index_1_ULavr = 0x22,
	Index_1_Iavr = 0x23,
};

enum EMSecondaryData_Index
{
	Index_2_Ua = 0x06,
	Index_2_Ub = 0x07,
	Index_2_Uc = 0x08,
	Index_2_Uab = 0x09,
	Index_2_Ubc = 0x0A,
	Index_2_Uca = 0x0B,
	Index_2_Ia = 0x0C,
	Index_2_Ib = 0x0D,
	Index_2_Ic = 0x0E,

	Index_2_Pa = 0x10,
	Index_2_Pb = 0x11,
	Index_2_Pc = 0x12,
	Index_2_P  = 0x13,
	Index_2_Qa = 0x14,
	Index_2_Qb = 0x15,
	Index_2_Qc = 0x16,
	Index_2_Q  = 0x17,
	Index_2_Sa = 0x18,
	Index_2_Sb = 0x19,
	Index_2_Sc = 0x1A,
	Index_2_S  = 0x1B,
	Index_2_PFa = 0x1C,
	Index_2_PFb = 0x1D,
	Index_2_PFc = 0x1E,
	Index_2_PF  = 0x1F,
	Index_2_Hz = 0x20,

	Index_2_PEPT = 0x30,
	Index_2_NEPT = 0x32,
	Index_2_PEQT = 0x34,
	Index_2_NEQT = 0x36,
	Index_2_EST  = 0x38,

	Index_2_Uavr = 0xA0,
	Index_2_ULavr = 0xA1,
	Index_2_Iavr = 0xA2,
	Index_2_Pavr = 0xA3,
	Index_2_Qavr = 0xA4,
	Index_2_Savr = 0xA5,
	Index_2_Io = 0xA6,

	Index_2_Uimb = 0xB4,
	Index_2_Iimb = 0xB5,

	Index_2_OUTS = 0xBC,
	Index_2_INS = 0xBE,
};

enum EMTHDData_Index
{
	Index_4_THD_Ua = 0x00,
	Index_4_THD_Ub = 0x01,
	Index_4_THD_Uc = 0x02,
	Index_4_THD_Ia = 0x03,
	Index_4_THD_Ib = 0x04,
	Index_4_THD_Ic = 0x05,
	Index_4_H_Ua   = 0x06,
	Index_4_H_Ub   = 0x07,
	Index_4_H_Uc   = 0x08,
	Index_4_H_Ia   = 0x09,
	Index_4_H_Ib   = 0x0A,
	Index_4_H_Ic   = 0x0B,

	Index_4_THD_3Ua = 0x0C, // 1
	Index_4_THD_3Ub = 0x0D,
	Index_4_THD_3Uc = 0x0E,
	Index_4_THD_3Ia = 0x0F,
	Index_4_THD_3Ib = 0x10,
	Index_4_THD_3Ic = 0x11,

	Index_4_THD_5Ua = 0x12,  // 2
	Index_4_THD_5Ub = 0x13,
	Index_4_THD_5Uc = 0x14,
	Index_4_THD_5Ia = 0x15,
	Index_4_THD_5Ib = 0x16,
	Index_4_THD_5Ic = 0x17,

	Index_4_THD_7Ua = 0x18,// 3
	Index_4_THD_7Ub = 0x19,
	Index_4_THD_7Uc = 0x1A,
	Index_4_THD_7Ia = 0x1B,
	Index_4_THD_7Ib = 0x1C,
	Index_4_THD_7Ic = 0x1D,

	Index_4_THD_9Ua = 0x1E,// 4
	Index_4_THD_9Ub = 0x1F,
	Index_4_THD_9Uc = 0x20,
	Index_4_THD_9Ia = 0x21,
	Index_4_THD_9Ib = 0x22,
	Index_4_THD_9Ic = 0x23,

	Index_4_THD_11Ua = 0x24,// 5
	Index_4_THD_11Ub = 0x25,
	Index_4_THD_11Uc = 0x26,
	Index_4_THD_11Ia = 0x27,
	Index_4_THD_11Ib = 0x28,
	Index_4_THD_11Ic = 0x29,

	Index_4_THD_13Ua = 0x2A,// 6
	Index_4_THD_13Ub = 0x2B,
	Index_4_THD_13Uc = 0x2C,
	Index_4_THD_13Ia = 0x2D,
	Index_4_THD_13Ib = 0x2E,
	Index_4_THD_13Ic = 0x2F,

	Index_4_THD_15Ua = 0x30,// 7
	Index_4_THD_15Ub = 0x31,
	Index_4_THD_15Uc = 0x32,
	Index_4_THD_15Ia = 0x33,
	Index_4_THD_15Ib = 0x34,
	Index_4_THD_15Ic = 0x35,

	Index_4_THD_17Ua = 0x36,// 8
	Index_4_THD_17Ub = 0x37,
	Index_4_THD_17Uc = 0x38,
	Index_4_THD_17Ia = 0x39,
	Index_4_THD_17Ib = 0x3A,
	Index_4_THD_17Ic = 0x3B,

	Index_4_THD_19Ua = 0x3C,// 9
	Index_4_THD_19Ub = 0x3D,
	Index_4_THD_19Uc = 0x3E,
	Index_4_THD_19Ia = 0x3F,
	Index_4_THD_19Ib = 0x40,
	Index_4_THD_19Ic = 0x41,

	Index_4_THD_21Ua = 0x42,// 10
	Index_4_THD_21Ub = 0x43,
	Index_4_THD_21Uc = 0x44,
	Index_4_THD_21Ia = 0x45,
	Index_4_THD_21Ib = 0x46,
	Index_4_THD_21Ic = 0x47,

	Index_4_THD_23Ua = 0x48,// 11
	Index_4_THD_23Ub = 0x49,
	Index_4_THD_23Uc = 0x4A,
	Index_4_THD_23Ia = 0x4B,
	Index_4_THD_23Ib = 0x4C,
	Index_4_THD_23Ic = 0x4D,

	Index_4_THD_25Ua = 0x4E,// 12
	Index_4_THD_25Ub = 0x4F,
	Index_4_THD_25Uc = 0x50,
	Index_4_THD_25Ia = 0x51,
	Index_4_THD_25Ib = 0x52,
	Index_4_THD_25Ic = 0x53,

	Index_4_THD_27Ua = 0x54,// 13
	Index_4_THD_27Ub = 0x55,
	Index_4_THD_27Uc = 0x56,
	Index_4_THD_27Ia = 0x57,
	Index_4_THD_27Ib = 0x58,
	Index_4_THD_27Ic = 0x59,

	Index_4_THD_29Ua = 0x5A,// 14
	Index_4_THD_29Ub = 0x5B,
	Index_4_THD_29Uc = 0x5C,
	Index_4_THD_29Ia = 0x5D,
	Index_4_THD_29Ib = 0x5E,
	Index_4_THD_29Ic = 0x5F,

	Index_4_THD_31Ua = 0x60,// 15
	Index_4_THD_31Ub = 0x61,
	Index_4_THD_31Uc = 0x62,
	Index_4_THD_31Ia = 0x63,
	Index_4_THD_31Ib = 0x64,
	Index_4_THD_31Ic = 0x65,
};

enum EMCTRLData_Index
{
	Index_8_Net  = 0x02,
	Index_8_Pt   = 0x03,
	Index_8_Ct   = 0x04,
	Index_8_Addr = 0x05,
	Index_8_Baud = 0x06,
	Index_8_Data = 0x07,
	Index_8_Cyc  = 0x08,
	Index_8_Ligh = 0x09,

	Index_8_AO1_Mode = 0x12,
	Index_8_AO1_Item = 0x13,
	Index_8_AO1_Fs = 0x14,
	Index_8_AO1_Ds = 0x15,

	Index_8_AO2_Mode = 0x16,
	Index_8_AO2_Item = 0x17,
	Index_8_AO2_Fs = 0x18,
	Index_8_AO2_Ds = 0x19,

	Index_8_DO1_Mode = 0x22,
	Index_8_DO1_Time = 0x23,
	Index_8_DO1_Item = 0x24,
	Index_8_DO1_Ual  = 0x25,
	Index_8_DO1_Hys  = 0x26,
	Index_8_DO1_Dely = 0x27,

	Index_8_DO2_Mode = 0x28,
	Index_8_DO2_Time = 0x29,
	Index_8_DO2_Item = 0x2A,
	Index_8_DO2_Ual  = 0x2B,
	Index_8_DO2_Hys  = 0x2C,
	Index_8_DO2_Dely = 0x2D,

	Index_8_DO3_Mode = 0x2E,
	Index_8_DO3_Time = 0x2F,
	Index_8_DO3_Item = 0x30,
	Index_8_DO3_Ual  = 0x31,
	Index_8_DO3_Hys  = 0x32,
	Index_8_DO3_Dely = 0x33,

	Index_8_DO4_Mode = 0x34,
	Index_8_DO4_Time = 0x35,
	Index_8_DO4_Item = 0x36,
	Index_8_DO4_Ual  = 0x37,
	Index_8_DO4_Hys  = 0x38,
	Index_8_DO4_Dely = 0x39,
};

extern PARAM_T g_tParam;
extern SX_PARAM DispCtrlParam;
extern u8 FourQuadtand;
extern u8 IbEnbleFlag;

extern float RtuPrimaryData[36];       // 一次侧数据
extern s16 RtuSecondaryData[192];      // 二次侧数据
extern s16 RtuTHDData[102];            // 谐波数据
extern s16 RtuCTRLData[64];            // 设置参数
extern s16 RtuCTRLDataTemp[64];        // 设置参数
extern s16 RtuOUTData[4];              // 继电器状态
extern s16 RtuINData[4];               // 开关量状态

void LoadParam(void);
void SaveParam(void);

#define RNPARAM_SIZE    76  /* 所有校准数据字节长度 */
#define RNPARAM_HFCONST1   2325//20347 //2325  /* 校准常数 // INT[(19073486*3.6*8.192*10^12)/ (32*3200*220*5*2^31)]*/
void InitCtrlParam(void);
void InitRNParam_Phase(u8 Phase);
void InitRNParam_PhaseToZero(u8 Phase);
void InitRNParam(void);
void InitRNParamToZero(void);
void WriteRNParam(u8 *pWriteData);

#endif

