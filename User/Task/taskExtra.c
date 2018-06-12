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

/* Includes ------------------------------------------------------------------*/
#include "Include.h"

/* define ------------------------------------------------------------------*/


#define RATIO_Warn_U	10 // 电压保护数值倍率
#define RATIO_Warn_I	10 // 电压保护数值倍率
#define RATIO_Warn_P	10 // 电流保护数值倍率
#define RATIO_Warn_PF	10 // 电压保护数值倍率
#define RATIO_Warn_F	1 // 电压保护数值倍率
#define RATIO_Warn_HARM	1 // 谐波保护数值倍率
#define RATIO_Warn_IMB  10//不平衡度保护数值倍率

/* Public variable -----------------------------------------------------------*/
u8 Out_Rem_Enble[3] = {FALSE,FALSE,FALSE};
u8 g_Din_Status[DIN_NUM];
u8 g_Din_BIT[DIN_NUM] = {0x01,0x02,0x04,0x08};
u8 DinStatus_Disp;

BOOL g_Out_Status[OUT_NUM];
u8 g_Out_BIT[DIN_NUM] = {0x01,0x02,0x04,0x08};
u8 OutStatus_Disp;
u8 RemOutCtrl;

Device_DataStruct    vg_DeviceData[PH_TH];

typedef enum
{
    Warn_TYPE_oFF = 0,
	Warn_TYPE_Ua_H = 1,
	Warn_TYPE_Ua_L = 2,
	Warn_TYPE_Ub_H = 3,
	Warn_TYPE_Ub_L = 4,
	Warn_TYPE_Uc_H = 5,
	Warn_TYPE_Uc_L = 6,
	Warn_TYPE_Uavg_H = 7,
	Warn_TYPE_Uavg_L = 8,
	Warn_TYPE_Uab_H = 9,
	Warn_TYPE_Uab_L = 10,
	Warn_TYPE_Ubc_H = 11,
	Warn_TYPE_Ubc_L = 12,
	Warn_TYPE_Uca_H = 13,
	Warn_TYPE_Uca_L = 14,
	Warn_TYPE_ULavg_H = 15,
	Warn_TYPE_ULavg_L = 16,

	Warn_TYPE_Ia_H = 17,
	Warn_TYPE_Ia_L = 18,
	Warn_TYPE_Ib_H = 19,
	Warn_TYPE_Ib_L = 20,
	Warn_TYPE_Ic_H = 21,
	Warn_TYPE_Ic_L = 22,
	Warn_TYPE_Iavg_H = 23,
	Warn_TYPE_Iavg_L = 24,

	Warn_TYPE_Pa_H = 25,
	Warn_TYPE_Pa_L = 26,
	Warn_TYPE_Pb_H = 27,
	Warn_TYPE_Pb_L = 28,
	Warn_TYPE_Pc_H = 29,
	Warn_TYPE_Pc_L = 30,
	Warn_TYPE_P_H = 31,
	Warn_TYPE_P_L = 32,

	Warn_TYPE_Qa_H = 33,
	Warn_TYPE_Qa_L = 34,
	Warn_TYPE_Qb_H = 35,
	Warn_TYPE_Qb_L = 36,
	Warn_TYPE_Qc_H = 37,
	Warn_TYPE_Qc_L = 38,
	Warn_TYPE_Q_H = 39,
	Warn_TYPE_Q_L = 40,

	Warn_TYPE_Sa_H = 41,
	Warn_TYPE_Sa_L = 42,
	Warn_TYPE_Sb_H = 43,
	Warn_TYPE_Sb_L = 44,
	Warn_TYPE_Sc_H = 45,
	Warn_TYPE_Sc_L = 46,
	Warn_TYPE_S_H = 47,
	Warn_TYPE_S_L = 48,

	Warn_TYPE_PFa_H = 49,
	Warn_TYPE_PFa_L = 50,
	Warn_TYPE_PFb_H = 51,
	Warn_TYPE_PFb_L = 52,
	Warn_TYPE_PFc_H = 53,
	Warn_TYPE_PFc_L = 54,
	Warn_TYPE_PF_H = 55,
	Warn_TYPE_PF_L = 56,

	Warn_TYPE_F_H = 57,
	Warn_TYPE_F_L = 58,

	Warn_TYPE_UTHa_H = 59,
	Warn_TYPE_UTHb_H = 60,
	Warn_TYPE_UTHc_H = 61,
	Warn_TYPE_UTH_H = 62,

    Warn_TYPE_ITHa_H = 63,
	Warn_TYPE_ITHb_H = 64,
	Warn_TYPE_ITHc_H = 65,
	Warn_TYPE_ITH_H = 66,

	Warn_TYPE_IMBU_H = 67,
	Warn_TYPE_IMBI_H = 68,

	Warn_TYPE_Io_H = 69,
	Warn_TYPE_Io_L = 70,

	Warn_TYPE_DI1_H = 71,
	Warn_TYPE_DI1_L = 72,
	Warn_TYPE_DI2_H = 73,
	Warn_TYPE_DI2_L = 74,
	Warn_TYPE_DI3_H = 75,
	Warn_TYPE_DI3_L = 76,
	Warn_TYPE_DI4_H = 77,
	Warn_TYPE_DI4_L = 78,
    Warn_NUM       =  79   // 总共保护数目
}PROTECT_TYPE;

typedef struct tagProtectStateStruct
{
    BOOL HappenDelayEnable;     // 是否启动延时
    BOOL RtnDelayEnable;        // 初次保护返回,用于启动延时
    BOOL HappenSure;            // 确定保护发生
    BOOL SwitchSure;            // 确定切除
    u32 DelayTickWhen;          // 延时时间
} svcProtectStateStruct;

svcProtectStateStruct g_svcProtectState[Warn_NUM];

void PwmPercentOutput_First(void)
{
    if(g_tParam.CtrlParam.AnaCalibFlag == FALSE)
    {
        return;
    }
    float Percenttemp;
    float AnalogConverTmp;
    float AnaValue;
    float DifferenceValue;
    s16 AnaNum;

    if(g_tParam.CtrlParam.AO1_Mode == AO_MODE_OFF)
    {
        TIM_SetCompare1(TIM17,0);
        return;
    }
    switch(g_tParam.CtrlParam.AO1_Item)
    {
#if(FUNCTION_VOLTAGE == 1)		
        case AO_ITEM_UA:
            AnalogConverTmp = vg_Power2_Val.Real22_Val[PH_A].U - g_tParam.CtrlParam.AO1_DS;
            break;
        case AO_ITEM_UB:
            AnalogConverTmp = vg_Power2_Val.Real22_Val[PH_B].U - g_tParam.CtrlParam.AO1_DS;
            break;
        case AO_ITEM_UC:
            AnalogConverTmp = vg_Power2_Val.Real22_Val[PH_C].U - g_tParam.CtrlParam.AO1_DS;
            break;
#endif
#if(FUNCTION_CURRENT == 1)		
        case AO_ITEM_IA:
            AnalogConverTmp = vg_Power2_Val.Real22_Val[PH_A].I - g_tParam.CtrlParam.AO1_DS;
            break;
        case AO_ITEM_IB:
            AnalogConverTmp = vg_Power2_Val.Real22_Val[PH_B].I - g_tParam.CtrlParam.AO1_DS;
            break;
        case AO_ITEM_IC:
            AnalogConverTmp = vg_Power2_Val.Real22_Val[PH_C].I - g_tParam.CtrlParam.AO1_DS;
            break;
#endif
        case AO_ITEM_F:
            AnalogConverTmp = vg_Power2_Val.Freq - g_tParam.CtrlParam.AO1_DS;
            break;
        default:
            break;
    }

    DifferenceValue = g_tParam.CtrlParam.AO1_FS - g_tParam.CtrlParam.AO1_DS;

    if(g_tParam.CtrlParam.AO1_Mode == AO_MODE_020)
    {
        Percenttemp = AnalogConverTmp / DifferenceValue;  //百分比
        if(Percenttemp >= 1)
            Percenttemp = 1;

        AnaValue = Percenttemp * 20;
        if(AnaValue > g_tParam.RN_Coefficients.Ana_Pwm1_5h)
        {
            AnaNum = 500 + (AnaValue - g_tParam.RN_Coefficients.Ana_Pwm1_5h)/g_tParam.RN_Coefficients.Ana_Pwm1_ki;
			if(AnaNum < 0)
			{
				AnaNum  = 0;
			}
        }
        else
        {
            AnaNum = 500 - (g_tParam.RN_Coefficients.Ana_Pwm1_5h - AnaValue)/g_tParam.RN_Coefficients.Ana_Pwm1_ki;
        }
    }
    else if(g_tParam.CtrlParam.AO1_Mode == AO_MODE_420)
    {
        Percenttemp = AnalogConverTmp / DifferenceValue;  //百分比
        if(Percenttemp >= 1)
            Percenttemp = 1;

        AnaValue = Percenttemp * 16;
        AnaNum = 500 + (AnaValue + 4 - g_tParam.RN_Coefficients.Ana_Pwm1_5h)/g_tParam.RN_Coefficients.Ana_Pwm1_ki;
    }
    else if(g_tParam.CtrlParam.AO1_Mode == AO_MODE_1220)
    {
        Percenttemp = AnalogConverTmp / DifferenceValue;  //百分比
        if(Percenttemp >= 1)
                Percenttemp = 1;
        if(Percenttemp <= -1)
                Percenttemp = -1;

        AnaValue = Percenttemp * 8;
        AnaNum = 500 + (AnaValue + 12 - g_tParam.RN_Coefficients.Ana_Pwm1_5h)/g_tParam.RN_Coefficients.Ana_Pwm1_ki;
    }
	AnaNum = (u16) (AnaNum * (47999 - 1) / 10000);
    TIM_SetCompare1(TIM17,AnaNum);

}

void PwmPercentOutput_Second(void)
{
    if(g_tParam.CtrlParam.AnaCalibFlag == FALSE)
    {
        return;
    }
    float Percenttemp;
    float AnalogConverTmp;
    float AnaValue;
    float DifferenceValue;
    u16 AnaNum;

    if(g_tParam.CtrlParam.AO2_Mode == AO_MODE_OFF)
    {
        TIM_SetCompare1(TIM16,0);
        return;
    }
    switch(g_tParam.CtrlParam.AO2_Item)
    {
#if(FUNCTION_VOLTAGE == 1)		
        case AO_ITEM_UA:
            AnalogConverTmp = vg_Power2_Val.Real22_Val[PH_A].U - g_tParam.CtrlParam.AO2_DS;
            break;
        case AO_ITEM_UB:
            AnalogConverTmp = vg_Power2_Val.Real22_Val[PH_B].U - g_tParam.CtrlParam.AO2_DS;
            break;
        case AO_ITEM_UC:
            AnalogConverTmp = vg_Power2_Val.Real22_Val[PH_C].U - g_tParam.CtrlParam.AO2_DS;
            break;
#endif
#if(FUNCTION_CURRENT == 1)		
        case AO_ITEM_IA:
            AnalogConverTmp = vg_Power2_Val.Real22_Val[PH_A].I - g_tParam.CtrlParam.AO2_DS;
            break;
        case AO_ITEM_IB:
            AnalogConverTmp = vg_Power2_Val.Real22_Val[PH_B].I - g_tParam.CtrlParam.AO2_DS;
            break;
        case AO_ITEM_IC:
            AnalogConverTmp = vg_Power2_Val.Real22_Val[PH_C].I - g_tParam.CtrlParam.AO2_DS;
            break;
#endif
        case AO_ITEM_F:
            AnalogConverTmp = vg_Power2_Val.Freq - g_tParam.CtrlParam.AO2_DS;
            break;
        default:
            break;
    }

    DifferenceValue = g_tParam.CtrlParam.AO2_FS - g_tParam.CtrlParam.AO2_DS;

    if(g_tParam.CtrlParam.AO2_Mode == AO_MODE_020)
    {
        Percenttemp = AnalogConverTmp / DifferenceValue;  //百分比
        if(Percenttemp >= 1)
            Percenttemp = 1;

        AnaValue = Percenttemp * 20;
        if(AnaValue > g_tParam.RN_Coefficients.Ana_Pwm2_5h)
        {
            AnaNum = 500 + (AnaValue - g_tParam.RN_Coefficients.Ana_Pwm2_5h)/g_tParam.RN_Coefficients.Ana_Pwm2_ki;
			if(AnaNum < 0)
			{
				AnaNum  = 0;
			}
        }
        else
        {
            AnaNum = 500 - (g_tParam.RN_Coefficients.Ana_Pwm2_5h - AnaValue)/g_tParam.RN_Coefficients.Ana_Pwm2_ki;
        }
    }
    else if(g_tParam.CtrlParam.AO2_Mode == AO_MODE_420)
    {
        Percenttemp = AnalogConverTmp / DifferenceValue;  //百分比
        if(Percenttemp >= 1)
            Percenttemp = 1;

        AnaValue = Percenttemp * 16;
        AnaNum = 500 + (AnaValue + 4 - g_tParam.RN_Coefficients.Ana_Pwm2_5h)/g_tParam.RN_Coefficients.Ana_Pwm2_ki;
    }
    else if(g_tParam.CtrlParam.AO2_Mode == AO_MODE_1220)
    {
        Percenttemp = AnalogConverTmp / DifferenceValue;  //百分比
        if(Percenttemp >= 1)
                Percenttemp = 1;
        if(Percenttemp <= -1)
                Percenttemp = -1;

        AnaValue = Percenttemp * 8;
        AnaNum = 500 + (AnaValue + 12 - g_tParam.RN_Coefficients.Ana_Pwm2_5h)/g_tParam.RN_Coefficients.Ana_Pwm2_ki;
    }
	AnaNum = (u16) (AnaNum * (47999 - 1) / 10000);
    TIM_SetCompare1(TIM16,AnaNum);

}

void DinStatus(void)
{
    u8 i;
	g_Din_Status[0] = KEY_DIN_STATUS1;
	g_Din_Status[1] = KEY_DIN_STATUS2;
	g_Din_Status[2] = KEY_DIN_STATUS3;
	g_Din_Status[3] = KEY_DIN_STATUS4;

    for(i = 0; i<DIN_NUM; i++)
    {
        if(!g_Din_Status[i])
        {
            DinStatus_Disp |= g_Din_BIT[i];
        }
        else
        {
            DinStatus_Disp &= ~g_Din_BIT[i];
        }
    }
    RtuSecondaryData[Index_2_OUTS] = DinStatus_Disp;
}

/****************************告警处理程序**********************************/
// 取消判断
void svc_Protect_CancelJudge(svcProtectStateStruct *psvcProtectState)
{
    psvcProtectState->HappenDelayEnable = FALSE;
    psvcProtectState->RtnDelayEnable = FALSE;
    psvcProtectState->HappenSure = FALSE;
    psvcProtectState->SwitchSure = FALSE;
    psvcProtectState->DelayTickWhen = 0;
}

// 判断
void svc_Protect_ProtectHappen(svcProtectStateStruct *psvcProtectState, u32 wActTime)
{
    if (psvcProtectState->HappenDelayEnable == FALSE)
    {
        psvcProtectState->HappenDelayEnable = TRUE;
        psvcProtectState->RtnDelayEnable = FALSE;
        psvcProtectState->SwitchSure = FALSE;
        psvcProtectState->DelayTickWhen = PT_TimerGet();
    }
    else
    {
        if (PT_TimerDiff(psvcProtectState->DelayTickWhen) > wActTime)
        {
            psvcProtectState->HappenDelayEnable = FALSE;
            psvcProtectState->RtnDelayEnable = FALSE;
            psvcProtectState->HappenSure = TRUE;
            psvcProtectState->SwitchSure = TRUE;
            psvcProtectState->DelayTickWhen = 0;
        }
    }
}

void svc_Protect_ProtectRtn(svcProtectStateStruct *psvcProtectState, u32 wRtnTime)
{
    if (psvcProtectState->RtnDelayEnable == FALSE)   // 没有确定
    {
        psvcProtectState->HappenDelayEnable = FALSE;
        psvcProtectState->RtnDelayEnable = TRUE;
        psvcProtectState->SwitchSure = FALSE;
        psvcProtectState->DelayTickWhen = PT_TimerGet();
    }
    else
    {
        if (PT_TimerDiff(psvcProtectState->DelayTickWhen) > wRtnTime )
        {
            svc_Protect_CancelJudge(psvcProtectState);
        }
    }
}

/*------------------------------------------------------------------------
 * 函 数 名 : svc_Protect_JudgeOverProtectHappen
 * 函数功能 : 过上限保护
 * 时    间 : 2010年10月26日
 * 返 回 值 : void
 * 参数说明 : u8 ph-相位
              u16 OverProteVal-上限
              u16 BackLashVal-回差值
              u16 RealVal-实际值
              PROTECT_TYPE Protect_Type-保护类型
------------------------------------------------------------------------*/
BOOL Ddi_Protect_JudgeOverProtectHappen(u32 OverProteVal, u32 BackLashVal, u32 RealVal, PROTECT_TYPE Protect_Type,u32 HappenDlyTime,u32 CancelDlyTime)
{
    if (OverProteVal == 0)
    {
        svc_Protect_CancelJudge(&g_svcProtectState[Protect_Type]);
        return FALSE;
    }
    if (OverProteVal <= RealVal)
    {
        svc_Protect_ProtectHappen(&g_svcProtectState[Protect_Type], HappenDlyTime);
    }
    else
    {
        if ((OverProteVal - BackLashVal) >= RealVal)   // 可以取消保护
        {
            // 保护已经确定发生
            if (g_svcProtectState[Protect_Type].HappenSure == TRUE)
            {
                svc_Protect_ProtectRtn(&g_svcProtectState[Protect_Type], CancelDlyTime);
            }
            else
            {
                svc_Protect_CancelJudge(&g_svcProtectState[Protect_Type]);
            }
        }
        else     // 处于发生和返回之间,保护继续延时动作
        {
            // 保护已经确定发生
            if (g_svcProtectState[Protect_Type].HappenSure == TRUE)
            {
                svc_Protect_ProtectHappen(&g_svcProtectState[Protect_Type], HappenDlyTime);
            }
        }
    }
    return g_svcProtectState[Protect_Type].HappenSure;
}


/*------------------------------------------------------------------------
 * 函 数 名 : svc_Protect_JudgeLowProtectHappen
 * 函数功能 : 过下限保护
 * 时    间 : 2006年11月22日
 * 返 回 值 : void
 * 参数说明 : BYTE ph
------------------------------------------------------------------------*/
BOOL Ddi_Protect_JudgeLowProtectHappen( u32 LowerProteVal, u32 BackLashVal, u32 RealVal, PROTECT_TYPE Protect_Type,u32 HappenDlyTime,u32 CancelDlyTime)
{
    if (LowerProteVal == 0)
    {
        svc_Protect_CancelJudge(&g_svcProtectState[Protect_Type]);
        return FALSE;
    }
    if (LowerProteVal >= RealVal)
    {
        svc_Protect_ProtectHappen(&g_svcProtectState[Protect_Type], HappenDlyTime);
    }
    else
    {
        if ((LowerProteVal + BackLashVal) < RealVal)
        {
            // 保护已经确定发生
            if (g_svcProtectState[Protect_Type].HappenSure == TRUE)
            {
                svc_Protect_ProtectRtn(&g_svcProtectState[Protect_Type], CancelDlyTime);
            }
            else
            {
                svc_Protect_CancelJudge(&g_svcProtectState[Protect_Type]);
            }
        }
        else     // 处于发生和返回之间,保护继续延时动作
        {
            // 保护已经确定发生
            if (g_svcProtectState[Protect_Type].HappenSure == TRUE)
            {
                svc_Protect_ProtectHappen(&g_svcProtectState[Protect_Type], HappenDlyTime);
            }
        }
    }
    return g_svcProtectState[Protect_Type].HappenSure;
}

/******************************************************************/
void OutHandler_First(void)
{
    if(g_tParam.CtrlParam.DO1_Mode == DO_MODE_OFF)
    {
        g_Out_Status[Dout_ONE] = FALSE;
        return;
    }
    else if(g_tParam.CtrlParam.DO1_Mode == DO_MODE_ALR)
    {
        switch(g_tParam.CtrlParam.DO1_Item)
        {
#if(FUNCTION_VOLTAGE == 1)
            case DO_ITEM_Ua_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO1_UAL,g_tParam.CtrlParam.DO1_HYS,
                vg_Power2_Val.Real22_Val[PH_A].U,Warn_TYPE_Ua_H,g_tParam.CtrlParam.DO1_Delay * 10,g_tParam.CtrlParam.DO1_Delay * 10);

                g_Out_Status[Dout_ONE] = g_svcProtectState[Warn_TYPE_Ua_H].HappenSure;
                break;
            case DO_ITEM_Ua_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO1_UAL,g_tParam.CtrlParam.DO1_HYS,
                vg_Power2_Val.Real22_Val[PH_A].U,Warn_TYPE_Ua_L,g_tParam.CtrlParam.DO1_Delay * 10,g_tParam.CtrlParam.DO1_Delay * 10);

                g_Out_Status[Dout_ONE] = g_svcProtectState[Warn_TYPE_Ua_L].HappenSure;
                break;
            case DO_ITEM_Ub_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO1_UAL,g_tParam.CtrlParam.DO1_HYS,
                vg_Power2_Val.Real22_Val[PH_B].U,Warn_TYPE_Ub_H,g_tParam.CtrlParam.DO1_Delay * 10,g_tParam.CtrlParam.DO1_Delay * 10);

                g_Out_Status[Dout_ONE] = g_svcProtectState[Warn_TYPE_Ub_H].HappenSure;
                break;
            case DO_ITEM_Ub_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO1_UAL,g_tParam.CtrlParam.DO1_HYS,
                vg_Power2_Val.Real22_Val[PH_B].U,Warn_TYPE_Ub_L,g_tParam.CtrlParam.DO1_Delay * 10,g_tParam.CtrlParam.DO1_Delay * 10);

                g_Out_Status[Dout_ONE] = g_svcProtectState[Warn_TYPE_Ub_L].HappenSure;
                break;
            case DO_ITEM_Uc_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO1_UAL,g_tParam.CtrlParam.DO1_HYS,
                vg_Power2_Val.Real22_Val[PH_C].U,Warn_TYPE_Uc_H,g_tParam.CtrlParam.DO1_Delay * 10,g_tParam.CtrlParam.DO1_Delay * 10);

                g_Out_Status[Dout_ONE] = g_svcProtectState[Warn_TYPE_Uc_H].HappenSure;
                break;
            case DO_ITEM_Uc_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO1_UAL,g_tParam.CtrlParam.DO1_HYS,
                vg_Power2_Val.Real22_Val[PH_C].U,Warn_TYPE_Uc_L,g_tParam.CtrlParam.DO1_Delay * 10,g_tParam.CtrlParam.DO1_Delay * 10);

                g_Out_Status[Dout_ONE] = g_svcProtectState[Warn_TYPE_Uc_L].HappenSure;
                break;
            /*---------------------------------------------------------------------------------------------------------------------*/
#endif
#if(FUNCTION_CURRENT == 1)
            case DO_ITEM_Ia_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO1_UAL,g_tParam.CtrlParam.DO1_HYS,
                    vg_Power2_Val.Real22_Val[PH_A].I,Warn_TYPE_Ia_H,g_tParam.CtrlParam.DO1_Delay * 10,g_tParam.CtrlParam.DO1_Delay * 10);

                g_Out_Status[Dout_ONE] = g_svcProtectState[Warn_TYPE_Ia_H].HappenSure;
                break;
            case DO_ITEM_Ia_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO1_UAL,g_tParam.CtrlParam.DO1_HYS,
                    vg_Power2_Val.Real22_Val[PH_A].I,Warn_TYPE_Ia_L,g_tParam.CtrlParam.DO1_Delay * 10,g_tParam.CtrlParam.DO1_Delay * 10);

                g_Out_Status[Dout_ONE] = g_svcProtectState[Warn_TYPE_Ia_L].HappenSure;
                break;
            case DO_ITEM_Ib_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO1_UAL,g_tParam.CtrlParam.DO1_HYS,
                    vg_Power2_Val.Real22_Val[PH_B].I,Warn_TYPE_Ib_H,g_tParam.CtrlParam.DO1_Delay * 10,g_tParam.CtrlParam.DO1_Delay * 10);

                g_Out_Status[Dout_ONE] = g_svcProtectState[Warn_TYPE_Ib_H].HappenSure;
                break;
            case DO_ITEM_Ib_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO1_UAL,g_tParam.CtrlParam.DO1_HYS,
                    vg_Power2_Val.Real22_Val[PH_B].I,Warn_TYPE_Ib_L,g_tParam.CtrlParam.DO1_Delay * 10,g_tParam.CtrlParam.DO1_Delay * 10);

                g_Out_Status[Dout_ONE] = g_svcProtectState[Warn_TYPE_Ib_L].HappenSure;
                break;
            case DO_ITEM_Ic_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO1_UAL,g_tParam.CtrlParam.DO1_HYS,
                    vg_Power2_Val.Real22_Val[PH_C].I,Warn_TYPE_Ic_H,g_tParam.CtrlParam.DO1_Delay * 10,g_tParam.CtrlParam.DO1_Delay * 10);

                g_Out_Status[Dout_ONE] = g_svcProtectState[Warn_TYPE_Ic_H].HappenSure;
                break;
            case DO_ITEM_Ic_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO1_UAL,g_tParam.CtrlParam.DO1_HYS,
                    vg_Power2_Val.Real22_Val[PH_C].I,Warn_TYPE_Ic_L,g_tParam.CtrlParam.DO1_Delay * 10,g_tParam.CtrlParam.DO1_Delay * 10);

                g_Out_Status[Dout_ONE] = g_svcProtectState[Warn_TYPE_Ic_L].HappenSure;
                break;
            /*---------------------------------------------------------------------------------------------------------------------*/
#endif
            /*---------------------------------------------------------------------------------------------------------------------*/
            case DO_ITEM_F_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO1_UAL,g_tParam.CtrlParam.DO1_HYS,
                    vg_Power2_Val.Freq,Warn_TYPE_F_H,g_tParam.CtrlParam.DO1_Delay * 10,g_tParam.CtrlParam.DO1_Delay * 10);

                g_Out_Status[Dout_ONE] = g_svcProtectState[Warn_TYPE_F_H].HappenSure;
                break;
            case DO_ITEM_F_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO1_UAL,g_tParam.CtrlParam.DO1_HYS,
                    vg_Power2_Val.Freq,Warn_TYPE_F_L,g_tParam.CtrlParam.DO1_Delay * 10,g_tParam.CtrlParam.DO1_Delay * 10);

                g_Out_Status[Dout_ONE] = g_svcProtectState[Warn_TYPE_F_L].HappenSure;
                break;
            /*---------------------------------------------------------------------------------------------------------------------*/
            case DO_ITEM_DI1_H:
                if(g_tParam.CtrlParam.DO1_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_ONE] = !g_Din_Status[0];
                }
                break;
            case DO_ITEM_DI2_H:
                if(g_tParam.CtrlParam.DO1_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_ONE] = !g_Din_Status[1];
                }
                break;
            case DO_ITEM_DI3_H:
                if(g_tParam.CtrlParam.DO1_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_ONE] = !g_Din_Status[2];
                }
                break;
            case DO_ITEM_DI4_H:
                if(g_tParam.CtrlParam.DO1_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_ONE] = !g_Din_Status[3];
                }
                break;
            case DO_ITEM_DI1_L:
                if(g_tParam.CtrlParam.DO1_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_ONE] = g_Din_Status[0];
                }
                break;
            case DO_ITEM_DI2_L:
                if(g_tParam.CtrlParam.DO1_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_ONE] = g_Din_Status[1];
                }
                break;
             case DO_ITEM_DI3_L:
                if(g_tParam.CtrlParam.DO1_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_ONE] = g_Din_Status[2];
                }
                break;
            case DO_ITEM_DI4_L:
                if(g_tParam.CtrlParam.DO1_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_ONE] = g_Din_Status[3];
                }
                break;
            default:
                break;
		}
    }
    else if(g_tParam.CtrlParam.DO1_Mode == DO_MODE_REM)
    {
        if(RemOutCtrl & 0x01)
        {
            g_Out_Status[Dout_ONE] = TRUE;
        }
        else
        {
            g_Out_Status[Dout_ONE] = FALSE;
        }
    }
}

void OutHandler_Second(void)
{
    if(g_tParam.CtrlParam.DO2_Mode == DO_MODE_OFF)
    {
        g_Out_Status[Dout_TWO] = FALSE;
        return;
    }
    else if(g_tParam.CtrlParam.DO2_Mode == DO_MODE_ALR)
    {
        switch(g_tParam.CtrlParam.DO2_Item)
        {
#if(FUNCTION_VOLTAGE == 1)
            case DO_ITEM_Ua_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO2_UAL,g_tParam.CtrlParam.DO2_HYS,
                    vg_Power2_Val.Real22_Val[PH_A].U,Warn_TYPE_Ua_H,g_tParam.CtrlParam.DO2_Delay * 10,g_tParam.CtrlParam.DO2_Delay * 10);

                g_Out_Status[Dout_TWO] = g_svcProtectState[Warn_TYPE_Ua_H].HappenSure;
                break;
            case DO_ITEM_Ua_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO2_UAL,g_tParam.CtrlParam.DO2_HYS,
                    vg_Power2_Val.Real22_Val[PH_A].U,Warn_TYPE_Ua_L,g_tParam.CtrlParam.DO2_Delay * 10,g_tParam.CtrlParam.DO2_Delay * 10);

                g_Out_Status[Dout_TWO] = g_svcProtectState[Warn_TYPE_Ua_L].HappenSure;
                break;
            case DO_ITEM_Ub_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO2_UAL,g_tParam.CtrlParam.DO2_HYS,
                    vg_Power2_Val.Real22_Val[PH_B].U,Warn_TYPE_Ub_H,g_tParam.CtrlParam.DO2_Delay * 10,g_tParam.CtrlParam.DO2_Delay * 10);

                g_Out_Status[Dout_TWO] = g_svcProtectState[Warn_TYPE_Ub_H].HappenSure;
                break;
            case DO_ITEM_Ub_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO2_UAL,g_tParam.CtrlParam.DO2_HYS,
                    vg_Power2_Val.Real22_Val[PH_B].U,Warn_TYPE_Ub_L,g_tParam.CtrlParam.DO2_Delay * 10,g_tParam.CtrlParam.DO2_Delay * 10);

                g_Out_Status[Dout_TWO] = g_svcProtectState[Warn_TYPE_Ub_L].HappenSure;
                break;
            case DO_ITEM_Uc_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO2_UAL,g_tParam.CtrlParam.DO2_HYS,
                    vg_Power2_Val.Real22_Val[PH_C].U,Warn_TYPE_Uc_H,g_tParam.CtrlParam.DO2_Delay * 10,g_tParam.CtrlParam.DO2_Delay * 10);

                g_Out_Status[Dout_TWO] = g_svcProtectState[Warn_TYPE_Uc_H].HappenSure;
                break;
            case DO_ITEM_Uc_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO2_UAL,g_tParam.CtrlParam.DO2_HYS,
                    vg_Power2_Val.Real22_Val[PH_C].U,Warn_TYPE_Uc_L,g_tParam.CtrlParam.DO2_Delay * 10,g_tParam.CtrlParam.DO2_Delay * 10);

                g_Out_Status[Dout_TWO] = g_svcProtectState[Warn_TYPE_Uc_L].HappenSure;
                break;
#endif			
            /*---------------------------------------------------------------------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
            case DO_ITEM_Ia_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO2_UAL,g_tParam.CtrlParam.DO2_HYS,
                    vg_Power2_Val.Real22_Val[PH_A].I,Warn_TYPE_Ia_H,g_tParam.CtrlParam.DO2_Delay * 10,g_tParam.CtrlParam.DO2_Delay * 10);

                g_Out_Status[Dout_TWO] = g_svcProtectState[Warn_TYPE_Ia_H].HappenSure;
                break;
            case DO_ITEM_Ia_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO2_UAL,g_tParam.CtrlParam.DO2_HYS,
                    vg_Power2_Val.Real22_Val[PH_A].I,Warn_TYPE_Ia_L,g_tParam.CtrlParam.DO2_Delay * 10,g_tParam.CtrlParam.DO2_Delay * 10);

                g_Out_Status[Dout_TWO] = g_svcProtectState[Warn_TYPE_Ia_L].HappenSure;
                break;
            case DO_ITEM_Ib_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO2_UAL,g_tParam.CtrlParam.DO2_HYS,
                    vg_Power2_Val.Real22_Val[PH_B].I,Warn_TYPE_Ib_H,g_tParam.CtrlParam.DO2_Delay * 10,g_tParam.CtrlParam.DO2_Delay * 10);

                g_Out_Status[Dout_TWO] = g_svcProtectState[Warn_TYPE_Ib_H].HappenSure;
                break;
            case DO_ITEM_Ib_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO2_UAL,g_tParam.CtrlParam.DO2_HYS,
                    vg_Power2_Val.Real22_Val[PH_B].I,Warn_TYPE_Ib_L,g_tParam.CtrlParam.DO2_Delay * 10,g_tParam.CtrlParam.DO2_Delay * 10);

                g_Out_Status[Dout_TWO] = g_svcProtectState[Warn_TYPE_Ib_L].HappenSure;
                break;
            case DO_ITEM_Ic_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO2_UAL,g_tParam.CtrlParam.DO2_HYS,
                    vg_Power2_Val.Real22_Val[PH_C].I,Warn_TYPE_Ic_H,g_tParam.CtrlParam.DO2_Delay * 10,g_tParam.CtrlParam.DO2_Delay * 10);

                g_Out_Status[Dout_TWO] = g_svcProtectState[Warn_TYPE_Ic_H].HappenSure;
                break;
            case DO_ITEM_Ic_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO2_UAL,g_tParam.CtrlParam.DO2_HYS,
                    vg_Power2_Val.Real22_Val[PH_C].I,Warn_TYPE_Ic_L,g_tParam.CtrlParam.DO2_Delay * 10,g_tParam.CtrlParam.DO2_Delay * 10);

                g_Out_Status[Dout_TWO] = g_svcProtectState[Warn_TYPE_Ic_L].HappenSure;
                break;
#endif
            /*---------------------------------------------------------------------------------------------------------------------*/
            case DO_ITEM_F_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO2_UAL * RATIO_Warn_F,g_tParam.CtrlParam.DO2_HYS * RATIO_Warn_F,
                    vg_Power2_Val.Freq,Warn_TYPE_F_H,g_tParam.CtrlParam.DO2_Delay * 10,g_tParam.CtrlParam.DO2_Delay * 10);

                g_Out_Status[Dout_TWO] = g_svcProtectState[Warn_TYPE_F_H].HappenSure;
                break;
            case DO_ITEM_F_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO2_UAL * RATIO_Warn_F,g_tParam.CtrlParam.DO2_HYS * RATIO_Warn_F,
                    vg_Power2_Val.Freq,Warn_TYPE_F_L,g_tParam.CtrlParam.DO2_Delay * 10,g_tParam.CtrlParam.DO2_Delay * 10);

                g_Out_Status[Dout_TWO] = g_svcProtectState[Warn_TYPE_F_L].HappenSure;
                break;
            /*---------------------------------------------------------------------------------------------------------------------*/
            case DO_ITEM_DI1_H:
                if(g_tParam.CtrlParam.DO2_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_TWO] = !g_Din_Status[0];
                }
                break;
            case DO_ITEM_DI2_H:
                if(g_tParam.CtrlParam.DO2_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_TWO] = !g_Din_Status[1];
                }
                break;
            case DO_ITEM_DI3_H:
                if(g_tParam.CtrlParam.DO2_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_TWO] = !g_Din_Status[2];
                }
                break;
            case DO_ITEM_DI4_H:
                if(g_tParam.CtrlParam.DO2_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_TWO] = !g_Din_Status[3];
                }
                break;
            case DO_ITEM_DI1_L:
                if(g_tParam.CtrlParam.DO2_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_TWO] = g_Din_Status[0];
                }
                break;
            case DO_ITEM_DI2_L:
                if(g_tParam.CtrlParam.DO2_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_TWO] = g_Din_Status[1];
                }
                break;
             case DO_ITEM_DI3_L:
                if(g_tParam.CtrlParam.DO2_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_TWO] = g_Din_Status[2];
                }
                break;
            case DO_ITEM_DI4_L:
                if(g_tParam.CtrlParam.DO2_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_TWO] = g_Din_Status[3];
                }
                break;
            default:
                break;
		}
    }
    else if(g_tParam.CtrlParam.DO2_Mode == DO_MODE_REM)
    {
        if(RemOutCtrl & 0x02)
        {
            g_Out_Status[Dout_TWO] = TRUE;
        }
        else
        {
            g_Out_Status[Dout_TWO] = FALSE;
        }
    }
}

void OutHandler_Thirdly(void)
{
    if(g_tParam.CtrlParam.DO3_Mode == DO_MODE_OFF)
    {
        g_Out_Status[Dout_THREE] = FALSE;
        return;
    }
    else if(g_tParam.CtrlParam.DO3_Mode == DO_MODE_ALR)
    {
        switch(g_tParam.CtrlParam.DO3_Item)
        {
#if(FUNCTION_VOLTAGE == 1)
            case DO_ITEM_Ua_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO3_UAL,g_tParam.CtrlParam.DO3_HYS,
                    vg_Power2_Val.Real22_Val[PH_A].U,Warn_TYPE_Ua_H,g_tParam.CtrlParam.DO3_Delay * 10,g_tParam.CtrlParam.DO3_Delay * 10);

                g_Out_Status[Dout_THREE] = g_svcProtectState[Warn_TYPE_Ua_H].HappenSure;
                break;
            case DO_ITEM_Ua_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO3_UAL,g_tParam.CtrlParam.DO3_HYS,
                    vg_Power2_Val.Real22_Val[PH_A].U,Warn_TYPE_Ua_L,g_tParam.CtrlParam.DO3_Delay * 10,g_tParam.CtrlParam.DO3_Delay * 10);

                g_Out_Status[Dout_THREE] = g_svcProtectState[Warn_TYPE_Ua_L].HappenSure;
                break;
            case DO_ITEM_Ub_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO3_UAL,g_tParam.CtrlParam.DO3_HYS,
                    vg_Power2_Val.Real22_Val[PH_B].U,Warn_TYPE_Ub_H,g_tParam.CtrlParam.DO3_Delay * 10,g_tParam.CtrlParam.DO3_Delay * 10);

                g_Out_Status[Dout_THREE] = g_svcProtectState[Warn_TYPE_Ub_H].HappenSure;
                break;
            case DO_ITEM_Ub_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO3_UAL,g_tParam.CtrlParam.DO3_HYS,
                    vg_Power2_Val.Real22_Val[PH_B].U,Warn_TYPE_Ub_L,g_tParam.CtrlParam.DO3_Delay * 10,g_tParam.CtrlParam.DO3_Delay * 10);

                g_Out_Status[Dout_THREE] = g_svcProtectState[Warn_TYPE_Ub_L].HappenSure;
                break;
            case DO_ITEM_Uc_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO3_UAL,g_tParam.CtrlParam.DO3_HYS,
                    vg_Power2_Val.Real22_Val[PH_C].U,Warn_TYPE_Uc_H,g_tParam.CtrlParam.DO3_Delay * 10,g_tParam.CtrlParam.DO3_Delay * 10);

                g_Out_Status[Dout_THREE] = g_svcProtectState[Warn_TYPE_Uc_H].HappenSure;
                break;
            case DO_ITEM_Uc_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO3_UAL,g_tParam.CtrlParam.DO3_HYS,
                    vg_Power2_Val.Real22_Val[PH_C].U,Warn_TYPE_Uc_L,g_tParam.CtrlParam.DO3_Delay * 10,g_tParam.CtrlParam.DO3_Delay * 10);

                g_Out_Status[Dout_THREE] = g_svcProtectState[Warn_TYPE_Uc_L].HappenSure;
                break;
#endif
            /*---------------------------------------------------------------------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
            case DO_ITEM_Ia_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO3_UAL ,g_tParam.CtrlParam.DO3_HYS,
                    vg_Power2_Val.Real22_Val[PH_A].I,Warn_TYPE_Ia_H,g_tParam.CtrlParam.DO3_Delay * 10,g_tParam.CtrlParam.DO3_Delay * 10);

                g_Out_Status[Dout_THREE] = g_svcProtectState[Warn_TYPE_Ia_H].HappenSure;
                break;
            case DO_ITEM_Ia_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO3_UAL,g_tParam.CtrlParam.DO3_HYS,
                    vg_Power2_Val.Real22_Val[PH_A].I,Warn_TYPE_Ia_L,g_tParam.CtrlParam.DO3_Delay * 10,g_tParam.CtrlParam.DO3_Delay * 10);

                g_Out_Status[Dout_THREE] = g_svcProtectState[Warn_TYPE_Ia_L].HappenSure;
                break;
            case DO_ITEM_Ib_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO3_UAL,g_tParam.CtrlParam.DO3_HYS,
                    vg_Power2_Val.Real22_Val[PH_B].I,Warn_TYPE_Ib_H,g_tParam.CtrlParam.DO3_Delay * 10,g_tParam.CtrlParam.DO3_Delay * 10);

                g_Out_Status[Dout_THREE] = g_svcProtectState[Warn_TYPE_Ib_H].HappenSure;
                break;
            case DO_ITEM_Ib_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO3_UAL,g_tParam.CtrlParam.DO3_HYS,
                    vg_Power2_Val.Real22_Val[PH_B].I,Warn_TYPE_Ib_L,g_tParam.CtrlParam.DO3_Delay * 10,g_tParam.CtrlParam.DO3_Delay * 10);

                g_Out_Status[Dout_THREE] = g_svcProtectState[Warn_TYPE_Ib_L].HappenSure;
                break;
            case DO_ITEM_Ic_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO3_UAL,g_tParam.CtrlParam.DO3_HYS,
                    vg_Power2_Val.Real22_Val[PH_C].I,Warn_TYPE_Ic_H,g_tParam.CtrlParam.DO3_Delay * 10,g_tParam.CtrlParam.DO3_Delay * 10);

                g_Out_Status[Dout_THREE] = g_svcProtectState[Warn_TYPE_Ic_H].HappenSure;
                break;
            case DO_ITEM_Ic_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO3_UAL ,g_tParam.CtrlParam.DO3_HYS,
                    vg_Power2_Val.Real22_Val[PH_C].I,Warn_TYPE_Ic_L,g_tParam.CtrlParam.DO3_Delay * 10,g_tParam.CtrlParam.DO3_Delay * 10);

                g_Out_Status[Dout_THREE] = g_svcProtectState[Warn_TYPE_Ic_L].HappenSure;
                break;
#endif
            /*---------------------------------------------------------------------------------------------------------------------*/
            case DO_ITEM_F_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO3_UAL * RATIO_Warn_F,g_tParam.CtrlParam.DO3_HYS * RATIO_Warn_F,
                    vg_Power2_Val.Freq,Warn_TYPE_F_H,g_tParam.CtrlParam.DO3_Delay * 10,g_tParam.CtrlParam.DO3_Delay * 10);

                g_Out_Status[Dout_THREE] = g_svcProtectState[Warn_TYPE_F_H].HappenSure;
                break;
            case DO_ITEM_F_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO3_UAL * RATIO_Warn_F,g_tParam.CtrlParam.DO3_HYS * RATIO_Warn_F,
                    vg_Power2_Val.Freq,Warn_TYPE_F_L,g_tParam.CtrlParam.DO3_Delay * 10,g_tParam.CtrlParam.DO3_Delay * 10);

                g_Out_Status[Dout_THREE] = g_svcProtectState[Warn_TYPE_F_L].HappenSure;
                break;
            /*---------------------------------------------------------------------------------------------------------------------*/
            case DO_ITEM_DI1_H:
                if(g_tParam.CtrlParam.DO3_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_THREE] = !g_Din_Status[0];
                }
                break;
            case DO_ITEM_DI2_H:
                if(g_tParam.CtrlParam.DO3_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_THREE] = !g_Din_Status[1];
                }
                break;
            case DO_ITEM_DI3_H:
                if(g_tParam.CtrlParam.DO3_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_THREE] = !g_Din_Status[2];
                }
                break;
            case DO_ITEM_DI4_H:
                if(g_tParam.CtrlParam.DO3_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_THREE] = !g_Din_Status[3];
                }
                break;
            case DO_ITEM_DI1_L:
                if(g_tParam.CtrlParam.DO3_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_THREE] = g_Din_Status[0];
                }
                break;
            case DO_ITEM_DI2_L:
                if(g_tParam.CtrlParam.DO3_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_THREE] = g_Din_Status[1];
                }
                break;
             case DO_ITEM_DI3_L:
                if(g_tParam.CtrlParam.DO3_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_THREE] = g_Din_Status[2];
                }
                break;
            case DO_ITEM_DI4_L:
                if(g_tParam.CtrlParam.DO3_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_THREE] = g_Din_Status[3];
                }
                break;
            default:
                break;
		}
    }
    else if(g_tParam.CtrlParam.DO3_Mode == DO_MODE_REM)
    {
        if(RemOutCtrl & 0x04)
        {
            g_Out_Status[Dout_THREE] = TRUE;
        }
        else
        {
            g_Out_Status[Dout_THREE] = FALSE;
        }
    }
}

void OutHandler_Fourthly(void)
{
    if(g_tParam.CtrlParam.DO4_Mode == DO_MODE_OFF)
    {
        g_Out_Status[Dout_FOUR] = FALSE;
        return;
    }
    else if(g_tParam.CtrlParam.DO4_Mode == DO_MODE_ALR)
    {
        switch(g_tParam.CtrlParam.DO4_Item)
        {
#if(FUNCTION_VOLTAGE == 1)
            case DO_ITEM_Ua_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO4_UAL,g_tParam.CtrlParam.DO4_HYS,
                    vg_Power2_Val.Real22_Val[PH_A].U,Warn_TYPE_Ua_H,g_tParam.CtrlParam.DO4_Delay * 10,g_tParam.CtrlParam.DO4_Delay * 10);

                g_Out_Status[Dout_FOUR] = g_svcProtectState[Warn_TYPE_Ua_H].HappenSure;
                break;
            case DO_ITEM_Ua_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO4_UAL,g_tParam.CtrlParam.DO4_HYS,
                    vg_Power2_Val.Real22_Val[PH_A].U,Warn_TYPE_Ua_L,g_tParam.CtrlParam.DO4_Delay * 10,g_tParam.CtrlParam.DO4_Delay * 10);

                g_Out_Status[Dout_FOUR] = g_svcProtectState[Warn_TYPE_Ua_L].HappenSure;
                break;
            case DO_ITEM_Ub_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO4_UAL,g_tParam.CtrlParam.DO4_HYS,
                    vg_Power2_Val.Real22_Val[PH_B].U,Warn_TYPE_Ub_H,g_tParam.CtrlParam.DO4_Delay * 10,g_tParam.CtrlParam.DO4_Delay * 10);

                g_Out_Status[Dout_FOUR] = g_svcProtectState[Warn_TYPE_Ub_H].HappenSure;
                break;
            case DO_ITEM_Ub_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO4_UAL,g_tParam.CtrlParam.DO4_HYS,
                    vg_Power2_Val.Real22_Val[PH_B].U,Warn_TYPE_Ub_L,g_tParam.CtrlParam.DO4_Delay * 10,g_tParam.CtrlParam.DO4_Delay * 10);

                g_Out_Status[Dout_FOUR] = g_svcProtectState[Warn_TYPE_Ub_L].HappenSure;
                break;
            case DO_ITEM_Uc_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO4_UAL,g_tParam.CtrlParam.DO4_HYS,
                    vg_Power2_Val.Real22_Val[PH_C].U,Warn_TYPE_Uc_H,g_tParam.CtrlParam.DO4_Delay * 10,g_tParam.CtrlParam.DO4_Delay * 10);

                g_Out_Status[Dout_FOUR] = g_svcProtectState[Warn_TYPE_Uc_H].HappenSure;
                break;
            case DO_ITEM_Uc_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO4_UAL,g_tParam.CtrlParam.DO4_HYS,
                    vg_Power2_Val.Real22_Val[PH_C].U,Warn_TYPE_Uc_L,g_tParam.CtrlParam.DO4_Delay * 10,g_tParam.CtrlParam.DO4_Delay * 10);

                g_Out_Status[Dout_FOUR] = g_svcProtectState[Warn_TYPE_Uc_L].HappenSure;
                break;
#endif
            /*---------------------------------------------------------------------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
            case DO_ITEM_Ia_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO4_UAL,g_tParam.CtrlParam.DO4_HYS,
                    vg_Power2_Val.Real22_Val[PH_A].I,Warn_TYPE_Ia_H,g_tParam.CtrlParam.DO4_Delay * 10,g_tParam.CtrlParam.DO4_Delay * 10);

                g_Out_Status[Dout_FOUR] = g_svcProtectState[Warn_TYPE_Ia_H].HappenSure;
                break;
            case DO_ITEM_Ia_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO4_UAL,g_tParam.CtrlParam.DO4_HYS,
                    vg_Power2_Val.Real22_Val[PH_A].I,Warn_TYPE_Ia_L,g_tParam.CtrlParam.DO4_Delay * 10,g_tParam.CtrlParam.DO4_Delay * 10);

                g_Out_Status[Dout_FOUR] = g_svcProtectState[Warn_TYPE_Ia_L].HappenSure;
                break;
            case DO_ITEM_Ib_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO4_UAL,g_tParam.CtrlParam.DO4_HYS,
                    vg_Power2_Val.Real22_Val[PH_B].I,Warn_TYPE_Ib_H,g_tParam.CtrlParam.DO4_Delay * 10,g_tParam.CtrlParam.DO4_Delay * 10);

                g_Out_Status[Dout_FOUR] = g_svcProtectState[Warn_TYPE_Ib_H].HappenSure;
                break;
            case DO_ITEM_Ib_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO4_UAL,g_tParam.CtrlParam.DO4_HYS,
                    vg_Power2_Val.Real22_Val[PH_B].I,Warn_TYPE_Ib_L,g_tParam.CtrlParam.DO4_Delay * 10,g_tParam.CtrlParam.DO4_Delay * 10);

                g_Out_Status[Dout_FOUR] = g_svcProtectState[Warn_TYPE_Ib_L].HappenSure;
                break;
            case DO_ITEM_Ic_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO4_UAL ,g_tParam.CtrlParam.DO4_HYS,
                    vg_Power2_Val.Real22_Val[PH_C].I,Warn_TYPE_Ic_H,g_tParam.CtrlParam.DO4_Delay * 10,g_tParam.CtrlParam.DO4_Delay * 10);

                g_Out_Status[Dout_FOUR] = g_svcProtectState[Warn_TYPE_Ic_H].HappenSure;
                break;
            case DO_ITEM_Ic_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO4_UAL,g_tParam.CtrlParam.DO4_HYS,
                    vg_Power2_Val.Real22_Val[PH_C].I,Warn_TYPE_Ic_L,g_tParam.CtrlParam.DO4_Delay * 10,g_tParam.CtrlParam.DO4_Delay * 10);

                g_Out_Status[Dout_FOUR] = g_svcProtectState[Warn_TYPE_Ic_L].HappenSure;
                break;
#endif 
            /*---------------------------------------------------------------------------------------------------------------------*/
            case DO_ITEM_F_H:
                Ddi_Protect_JudgeOverProtectHappen(g_tParam.CtrlParam.DO4_UAL * RATIO_Warn_F,g_tParam.CtrlParam.DO4_HYS * RATIO_Warn_F,
                    vg_Power2_Val.Freq,Warn_TYPE_F_H,g_tParam.CtrlParam.DO4_Delay * 10,g_tParam.CtrlParam.DO4_Delay * 10);

                g_Out_Status[Dout_FOUR] = g_svcProtectState[Warn_TYPE_F_H].HappenSure;
                break;
            case DO_ITEM_F_L:
                Ddi_Protect_JudgeLowProtectHappen(g_tParam.CtrlParam.DO4_UAL * RATIO_Warn_F,g_tParam.CtrlParam.DO4_HYS * RATIO_Warn_F,
                    vg_Power2_Val.Freq,Warn_TYPE_F_L,g_tParam.CtrlParam.DO4_Delay * 10,g_tParam.CtrlParam.DO4_Delay * 10);

                g_Out_Status[Dout_FOUR] = g_svcProtectState[Warn_TYPE_F_L].HappenSure;
                break;
            /*---------------------------------------------------------------------------------------------------------------------*/
            case DO_ITEM_DI1_H:
                if(g_tParam.CtrlParam.DO4_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_FOUR] = !g_Din_Status[0];
                }
                break;
            case DO_ITEM_DI2_H:
                if(g_tParam.CtrlParam.DO4_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_FOUR] = !g_Din_Status[1];
                }
                break;
            case DO_ITEM_DI3_H:
                if(g_tParam.CtrlParam.DO4_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_FOUR] = !g_Din_Status[2];
                }
                break;
            case DO_ITEM_DI4_H:
                if(g_tParam.CtrlParam.DO4_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_FOUR] = !g_Din_Status[3];
                }
                break;
            case DO_ITEM_DI1_L:
                if(g_tParam.CtrlParam.DO4_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_FOUR] = g_Din_Status[0];
                }
                break;
            case DO_ITEM_DI2_L:
                if(g_tParam.CtrlParam.DO4_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_FOUR] = g_Din_Status[1];
                }
                break;
             case DO_ITEM_DI3_L:
                if(g_tParam.CtrlParam.DO4_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_FOUR] = g_Din_Status[2];
                }
                break;
            case DO_ITEM_DI4_L:
                if(g_tParam.CtrlParam.DO4_Mode == DO_MODE_ALR)
                {
                    g_Out_Status[Dout_FOUR] = g_Din_Status[3];
                }
                break;
            default:
                break;
		}
    }
    else if(g_tParam.CtrlParam.DO4_Mode == DO_MODE_REM)
    {
        if(RemOutCtrl & 0x08)
        {
            g_Out_Status[Dout_FOUR] = TRUE;
        }
        else
        {
            g_Out_Status[Dout_FOUR] = FALSE;
        }
    }
}


void OutStatus(void)
{
    u8 i;
    for(i = 0; i < OUT_NUM; i++)
    {
        if(g_Out_Status[i])
        {
            OutStatus_Disp |= g_Din_BIT[i];
        }
        else
        {
            OutStatus_Disp &= ~g_Din_BIT[i];
        }
    }
    RtuSecondaryData[Index_2_INS] = OutStatus_Disp;
}

void Task_Extra_Function(void)
{	
    if(g_AoOutEnable)
    {
        TIM_PWM_Configuration_AO1();
        TIM_PWM_Configuration_AO2();
        g_AoOutEnable = FALSE;
    }
	DinStatus();
	OutHandler_First();
	OutHandler_Second();
	OutHandler_Thirdly();
	OutHandler_Fourthly();
	OutStatus();
	PwmPercentOutput_First();
	PwmPercentOutput_Second();
}

