/*! @file
********************************************************************************
<PRE>
模块名       : 硬件驱动模块
文件名       : drvI2C.h
相关文件     :
文件实现功能 : I2C总线驱动
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


#ifndef _RN8302B_H
#define _RN8302B_H

/* Includes ------------------------------------------------------------------*/
#include "Macro.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define STDUn                                   220
#define STDIb                                   5
#define CalVoltate                              44000000         //220V电压下校表
#define CalCurrent                              50000000         //5A 电流下校表
#define STDP                                    262260437        //标准的有功功率
#define STDP_05L                                131130218        //标准的有功功率
#define RN8302_fosc                             8000000        //晶振频率 标准 8192000
#define RO                                      0x00
#define RW                                      0x01
#define bank0                                   0x00
#define bank1                                   0x01
#define bank2                                   0x02
#define bank3                                   0x03
#define bank4                                   0x04
#define WRENABLE                                0xE5
#define WRDISABLE                               0xDC
#define SAMPTEMPCLR                             0x10                //50Hz采样，缓冲区清零
#define CLREMMIFH                               0xFF                //清采样缓冲中断标志
#define CLREMMIFL                               0xFF
#define SAMPSTART                               0x20
#define SOFTRST                                 0xFA
#define EMMOD                                   0xA2                //EMM模式
#define SLPMOD                                  0x18
#define NVM1MOD                                 0xE1
#define NVM2MOD                                 0x5C
#define CHANNELEN                               0xFF
#define ADCGAINVAL                              0x00                //ADC 通道增益设定。电压通道1倍增益。
//#define HF6400                            0x031F                        //6400常数
#define EGYRDMODVAL                             0x40                //视在电能读取后清零。
#define LNWIREMODE                              0x00        //三相四线接线模式。
#define LLWIREMODE                              0x33        //三相三线接线模式。
#define GAIN_1                                  0x00
#define GAIN_2                                  0x01
#define GAIN_4                                  0x10
#define GAIN_8                                  0x11
#define CALIB_UIE                               0xb0
#define CALIB_ANGLE                             0xe8       
#define CALIB_IOFFSET                           0x1f       
#define CALIB_SECTION_I                         0xa0        
#define CALIB_AnoPwm                            0xc0        
#define CALIB_SetPwm							0xc1
#define CALIB_SET_DEFAULT                       0xf9
#define CALIB_SET_ZERO                          0x04
#define CALIB_WRITEDATA                         0x05
//#define HFCONST1                                      0x39
//#define HFCONST0                                      0x07                        //测试常数
//#define HFCONST1                                      0xCE


// 一次侧采样数据
__packed typedef struct
{
    // 单位:0.01V
    float U;          // 电压
    // 单位:0.01V
    float LineU;      // 线电压
    // 单位:0.0001A
    float I;          // 电流
    // 单位:0.0001A
    float P;          // 有功功率
    // 单位:0.00001kvar
    float Q;          // 无功功率
    // 单位:0.00001kVA
    float S;          // 视在功率
    // 单位:0.001
    float PF;         // 功率因数
    // 单位:0.1
}POWER_Real2_ValStruct;


__packed typedef struct
{
    POWER_Real2_ValStruct Real2_Val[PH_TH];

    float Pt;
    float Qt;
    float St;
    float PFTA;
    float Uavr;
    float ULavr;
    float Iavr;
    float Freq;
}POWER2_ValStruct;

// 二次侧采样数据
__packed typedef struct
{
    // 单位:0.01V
    int U;              // 相电压
    // 单位:0.01V
    int LineU;          // 线电压
    // 单位:0.0001A
    int I;              // 相电流
    // 单位:0.00001kW=0.1w
    int P;              // 有功功率
    // 单位:0.00001kvar
    int Q;              // 无功功率
    // 单位:0.00001kVA
    int S;              // 视在功率
    // 单位:0.001
    int PF;             // 功率因数
}POWER_Real22_ValStruct;


__packed typedef struct
{
    POWER_Real22_ValStruct Real22_Val[PH_TH];

    int Pt;
    int Qt;
    int St;
    int PFTA;
    int Uavr;
    int ULavr;
    int Iavr;
    int Io;
    int Pavr;
    int Qavr;
    int Savr;
    int Freq;
}POWER22_ValStruct;

// 交流采样数据(读取数据用)
__packed typedef struct
{
    //单位:0.01V
    u16 U;          //二次侧电压 AC
    //单位:0.0001A
    u32 I;          //二次侧电流 B
    //单位:0.00001kW=0.1w
    s32 P;          //二次侧有功功率 A/B/C/总
    //单位:0.00001kvar
    s32 Q;          //二次侧无功功率 A/B/C/总
    //单位:0.00001kVA
    u32 S;          //二次侧视在功率
    //单位:0.001
    u16 PF;         //功率因数值
}POWER_Read_ValStruct;


// 校表可读
__packed typedef struct
{
    POWER_Read_ValStruct Real_Read_Val[PH_TH];

}POWER2_Read_ValStruct;


typedef struct
{
    unsigned char addr;
    unsigned char bytenum;
    unsigned char rw;
    unsigned int  data;
} RegStructure;

// RN8209 寄存器
typedef struct
{
    RegStructure        SYSCON;
    RegStructure        EMUCON;
    RegStructure        HFConst;
    RegStructure        PStart;
    RegStructure        DStart;
    RegStructure        GPQA;
    RegStructure        GPQB;
    RegStructure        PhsA;
    RegStructure        PhsB;
    RegStructure        QPhsCal;
    RegStructure        APOSA;
    RegStructure        APOSB;
    RegStructure        RPOSA;
    RegStructure        RPOSB;
    RegStructure        IARMSOS;
    RegStructure        IBRMSOS;
    RegStructure        IBGain;
    RegStructure        D2FPL;
    RegStructure        D2FPH;
    RegStructure        DCIAH;
    RegStructure        DCIBH;
    RegStructure        DCUH;
    RegStructure        DCL;
    RegStructure        EMUCON2;
    RegStructure        PFCnt;
    RegStructure        DFcnt;
    RegStructure        IARMS;
    RegStructure        IBRMS;
    RegStructure        URMS;
    RegStructure        UFreq;
    RegStructure        PowerPA;
    RegStructure        PowerPB;
    RegStructure        PowerQ;
    RegStructure        EnergyP;
    RegStructure        EnergyP2;
    RegStructure        EnergyD;
    RegStructure        EnergyD2;
    RegStructure        EMUStatus;
    RegStructure        SPL_IA;
    RegStructure        SPL_IB;
    RegStructure        SPL_U;
    RegStructure        UFreq2;
    RegStructure        IE;
    RegStructure        IF;
    RegStructure        RIF;
    RegStructure        SysStatus;
    RegStructure        RData;
    RegStructure        WData;
    RegStructure        DeviceID;
    RegStructure        WREN;
    RegStructure        WPRO;
    RegStructure        IAChoice;
    RegStructure        IBChoice;
    RegStructure        SOFT_RST;
} All_REG_Data;

struct RN_Coefficient
{
    unsigned short HFConst1;
    unsigned short HFConst2;
    unsigned short IStart_PS;
    unsigned short IStart_Q;
    unsigned short LostVoltT;
    unsigned short ZXOT;
    short GSUA;
    short GSUB;
    short GSUC;
    short GSIA;
    short GSIB;
    short GSIC;
    unsigned int PHSIA;
    unsigned int PHSIB;
    unsigned int PHSIC;
    unsigned int PA_PHSL;
    unsigned int PB_PHSL;
    unsigned int PC_PHSL;
};


/* Exported functions ------------------------------------------------------- */
extern POWER2_ValStruct vg_Power_Val;
extern POWER22_ValStruct vg_Power2_Val;
extern POWER2_Read_ValStruct vg_Read_Val;
extern u32 CheckSum;
extern void bsp_InitRN8023(void);
extern void RN8032_Adjust(u8 Cmd, u8 Phase, u8 *pAdjustData);
void Task_Power_Function(void);
extern void RN8032_Updata(void);
extern void InitRNParam(void);

//#define PI 3.1415926
#endif /* _DRVI2C_H */

//===========================================================================
// No more.
//===========================================================================


