/*! @file
********************************************************************************
<PRE>
模块名       : 任务模块
文件名       : taskProtect.c
相关文件     :
文件实现功能 : 无功补偿
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

/* Includes ------------------------------------------------------------------*/
#include "Include.h"
/* Private typedef -----------------------------------------------------------*/

BOOL g_OverCurr[PH_TH] = {FALSE,FALSE,FALSE};

/*------------------------------------------------------------------------
* 结构体名 : tagKGProtectStateStruct
* 功    能 : 保护判断过程状态数据
------------------------------------------------------------------------*/
typedef struct tagKGProtectStateStruct
{
    BOOL HappenDelayEnable;     // 是否启动延时
    BOOL RtnDelayEnable;        // 初次保护返回,用于启动延时
    BOOL HappenSure;            // 确定保护发生
    BOOL SwitchSure;            // 确定切除
    u32 DelayTickWhen;          // 延时时间
} KGProtectStateStruct;



enum emADProtect   //
{
    ADJUSTVOL_A_CUR = 0, // 过流保护

    ADJUSTVOL_B_CUR = 1, // 过流保护

    ADJUSTVOL_C_CUR = 2, // 过流保护

    ADJUSTVOL_MAX = 3,

};

// 保护状态变量
KGProtectStateStruct g_KGProtectState[ADJUSTVOL_MAX];

// ---------------------- definitions ------------------------------

#define RATIO_ADJUST_I	10000 // 电流保护数值倍率

// 取消判断
void KG_CancelJudge(KGProtectStateStruct *pKGProtectState)
{
    pKGProtectState->HappenDelayEnable = FALSE;
    pKGProtectState->RtnDelayEnable = FALSE;
    pKGProtectState->HappenSure = FALSE;
    pKGProtectState->SwitchSure = FALSE;
    pKGProtectState->DelayTickWhen = 0;
}

// 判断
void KG_ProtectHappen(KGProtectStateStruct *pKGProtectState, u32 wActTime)
{
    if (pKGProtectState->HappenDelayEnable == FALSE)
    {
        pKGProtectState->HappenDelayEnable = TRUE;
        pKGProtectState->RtnDelayEnable = FALSE;
        pKGProtectState->SwitchSure = FALSE;
        pKGProtectState->DelayTickWhen = PT_TimerGet();
    }
    else
    {
        if (PT_TimerDiff(pKGProtectState->DelayTickWhen) > wActTime)
        {
            pKGProtectState->HappenDelayEnable = FALSE;
            pKGProtectState->RtnDelayEnable = FALSE;
            pKGProtectState->HappenSure = TRUE;
            pKGProtectState->SwitchSure = TRUE;
            pKGProtectState->DelayTickWhen = 0;
        }
    }
}

void KG_ProtectRtn(KGProtectStateStruct *pKGProtectState, u32 RtnTime)
{
    if (pKGProtectState->RtnDelayEnable == FALSE)   // 没有确定
    {
        pKGProtectState->HappenDelayEnable = FALSE;
        pKGProtectState->RtnDelayEnable = TRUE;
        pKGProtectState->SwitchSure = FALSE;
        pKGProtectState->DelayTickWhen = PT_TimerGet();
    }
    else
    {
        if (PT_TimerDiff(pKGProtectState->DelayTickWhen) > RtnTime)
        {
            KG_CancelJudge(pKGProtectState);
        }
    }
}

BOOL KG_JudgeOverHappen(u32 OverProteVal, u32 BackLashVal, u32 RealVal, enum emADProtect AdjustType, u32 HappenDlyTime, u32 CancelDlyTime)
{
    if (OverProteVal == 0)
    {
        KG_CancelJudge(&g_KGProtectState[AdjustType]);
        return FALSE;
    }
    if (OverProteVal <= RealVal)
    {
        KG_ProtectHappen(&g_KGProtectState[AdjustType], HappenDlyTime);
    }
    else
    {
        if ((OverProteVal - BackLashVal) >= RealVal)   // 可以取消保护
        {
            // 保护已经确定发生
            if (g_KGProtectState[AdjustType].HappenSure == TRUE)
            {
                KG_ProtectRtn(&g_KGProtectState[AdjustType], CancelDlyTime);
            }
            else
            {
                KG_CancelJudge(&g_KGProtectState[AdjustType]);
            }
        }
        else     // 处于发生和返回之间,保护继续延时动作
        {
            // 保护已经确定发生
            if (g_KGProtectState[AdjustType].HappenSure == TRUE)
            {
                KG_ProtectHappen(&g_KGProtectState[AdjustType], HappenDlyTime);
            }
        }
    }
    return g_KGProtectState[AdjustType].HappenSure;
}


void Protect_AutoProc(void)
{


    u32 BackCur = 0;
    u32 OverCur_Value1 = 0;
    //static union AdjustVolState_Reg s_OldAdjustVolState[PH_TH];
    float fCalcCur;

    // 判断过流1
    fCalcCur = (float)g_tParam.CtrlParam.Alarm / 100* 5 * g_tParam.CtrlParam.CTNum; // 计算二次侧电流 120
    fCalcCur *= RATIO_ADJUST_I; // 转换成mA
    OverCur_Value1 = (float)fCalcCur; // 整形表示
    BackCur = (fCalcCur * 0.1); // 返回值，门限值的0.9倍返回  120A

    KG_JudgeOverHappen(OverCur_Value1, BackCur, vg_Power_Val.Real2_Val[PH_A].I, ADJUSTVOL_A_CUR, 10000, 10000);

    KG_JudgeOverHappen(OverCur_Value1, BackCur, vg_Power_Val.Real2_Val[PH_B].I, ADJUSTVOL_B_CUR, 10000, 10000); //60000 60S

    KG_JudgeOverHappen(OverCur_Value1, BackCur, vg_Power_Val.Real2_Val[PH_C].I, ADJUSTVOL_C_CUR, 10000, 10000);

    // 测量不准确，非正常状态，返回
    if (vg_Power_Val.Real2_Val[PH_A].U < 1000 || vg_Power_Val.Real2_Val[PH_B].U < 1000 || vg_Power_Val.Real2_Val[PH_C].U < 1000)
    {
        return;
    }

    // 过流
    if (g_KGProtectState[ADJUSTVOL_A_CUR].SwitchSure)
    {
        g_OverCurr[PH_A] = TRUE;
    }
    if (g_KGProtectState[ADJUSTVOL_B_CUR].SwitchSure)
    {
        g_OverCurr[PH_B] = TRUE;
    }
    if (g_KGProtectState[ADJUSTVOL_C_CUR].SwitchSure)
    {
        g_OverCurr[PH_C] = TRUE;
    }
    // 过流
    if (!g_KGProtectState[ADJUSTVOL_A_CUR].HappenSure)
    {
        g_OverCurr[PH_A] = FALSE;
    }
    if (!g_KGProtectState[ADJUSTVOL_B_CUR].HappenSure)
    {
        g_OverCurr[PH_B] = FALSE;
    }
    if (!g_KGProtectState[ADJUSTVOL_C_CUR].HappenSure)
    {
        g_OverCurr[PH_C] = FALSE;
    }


/***********************************************************************************************************/
/***********************************************************************************************************/
}

void Task_Protect_Function(void)
{
    Protect_AutoProc();
}

