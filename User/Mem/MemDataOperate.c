/* Includes ------------------------------------------------------------------*/
#include "Include.h"

/* Public variables ---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

volatile BOOL vg_sysInitFlag = FALSE; // 系统启动标志

/* Private functions ---------------------------------------------------------*/

BOOL MemData_Write_To_Flash(u32 Addr, u8 *pInData, u16 Count)  //Flash 写入
{
    u16 Counter = 0;
    u32 dwRegRealAddress = Addr;
    BOOL MemoryProgramStatus = TRUE;

    if (Count <= FLASH_PARAM_SIZE)
    {
        FLASH_Unlock();

        FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

        FLASH_ErasePage(dwRegRealAddress);

        for (Counter = 0; Counter < FLASH_PARAM_SIZE / 4; Counter++)
        {
            if (FLASH_ProgramWord(dwRegRealAddress, *(vu32*) (pInData+Counter*4)) == FLASH_COMPLETE) // 写完成
            {
                if((*(vu32*) dwRegRealAddress) != *(vu32*)(pInData+Counter*4)) // 内容检查
                {
                    MemoryProgramStatus = FALSE;
                    break;
                }
                dwRegRealAddress += 4;
            }
            else
            {
                MemoryProgramStatus = FALSE;
                break;
            }
        }
        FLASH_Lock();

        return MemoryProgramStatus;
    }
    else
    {
        return FALSE;
    }
}
void MemData_WriteCalib(u8 *pScrData, u16 Count)
{
	MemData_Write_To_Flash(CALIB_STARTADDR, pScrData, Count);
}

void MemData_ReadCalib(u8 *pDstData, u16 Count)
{
	memcpy(pDstData, (u8 *)CALIB_STARTADDR, Count);
}

bool MemData_CmpParamType(PARAM_T *pSettingParam)
{
    if( pSettingParam->CtrlParam.NetMode > MAX_NetMode)
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.Addr485 > MAX_Addr )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DataFormat > MAX_DataFormat )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DataProtocol > MAX_DataProt || pSettingParam->CtrlParam.DataProtocol < MIN_DataProt )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.Cycle > MAX_Cycle || pSettingParam->CtrlParam.Cycle < MIN_Cycle )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.BlackTime > MAX_BlackTime  || pSettingParam->CtrlParam.BlackTime  < MIN_BlackTime  )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DefaultDisp > MAX_Disp )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.PTNum > MAX_PT || pSettingParam->CtrlParam.PTNum < MIN_PT )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.CTNum > MAX_CT || pSettingParam->CtrlParam.CTNum < MIN_CT )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.Baud485 > MAX_Baud )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.Alarm > MAX_Alarm)
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.Code > MAX_Code || pSettingParam->CtrlParam.Code < MIN_Code )
    {
        return FALSE;
    }

    /*********************************DO-1*******************************************************************/
    if( pSettingParam->CtrlParam.DO1_Mode > MAX_DO_Mode )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO1_Item > MAX_DO_Item )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO1_Time > MAX_DO_Time )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO1_UAL > MAX_DO_Ual )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO1_HYS > MAX_DO_Hys )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO1_Delay > MAX_DO_Dely )
    {
        return FALSE;
    }

    /*********************************DO-2*******************************************************************/
    if( pSettingParam->CtrlParam.DO2_Mode > MAX_DO_Mode )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO2_Item > MAX_DO_Item )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO2_Time > MAX_DO_Time )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO2_UAL > MAX_DO_Ual )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO2_HYS > MAX_DO_Hys )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO2_Delay > MAX_DO_Dely )
    {
        return FALSE;
    }

    /*********************************DO-3*******************************************************************/
    if( pSettingParam->CtrlParam.DO3_Mode > MAX_DO_Mode )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO3_Item > MAX_DO_Item )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO3_Time > MAX_DO_Time )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO3_UAL > MAX_DO_Ual )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO3_HYS > MAX_DO_Hys )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO3_Delay > MAX_DO_Dely )
    {
        return FALSE;
    }

    /*********************************DO-4*******************************************************************/
    if( pSettingParam->CtrlParam.DO4_Mode > MAX_DO_Mode )//|| pSettingParam->CtrlParam.DO3_Mode < MIN_DO_Mode )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO4_Item > MAX_DO_Item )//|| pSettingParam->CtrlParam.DO3_Item < MIN_DO_Item )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO4_Time > MAX_DO_Time )//|| pSettingParam->CtrlParam.DO3_Time < MIN_DO_Time )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO4_UAL > MAX_DO_Ual )//|| pSettingParam->CtrlParam.DO3_UAL < MIN_DO_Ual )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO4_HYS > MAX_DO_Hys )//|| pSettingParam->CtrlParam.DO3_HYS < MIN_DO_Hys )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.DO4_Delay > MAX_DO_Dely )//|| pSettingParam->CtrlParam.DO3_Delay < MIN_DO_Dely )
    {
        return FALSE;
    }

    /*********************************AO-1*******************************************************************/
    if( pSettingParam->CtrlParam.AO1_Mode > MAX_AO_Mode )//|| pSettingParam->CtrlParam.AO1_Mode < MIN_AO_Mode )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.AO1_Item > MAX_DO_Item )//|| pSettingParam->CtrlParam.DO1_Item < MIN_DO_Item )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.AO1_DS > MAX_AO_DS )//|| pSettingParam->CtrlParam.AO1_DS< MIN_AO_DS)
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.AO1_FS > MAX_AO_FS )//|| pSettingParam->CtrlParam.AO1_FS < MIN_AO_FS)
    {
        return FALSE;
    }

    /*********************************AO-2*******************************************************************/
    if( pSettingParam->CtrlParam.AO2_Mode > MAX_AO_Mode )//|| pSettingParam->CtrlParam.AO1_Mode < MIN_AO_Mode )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.AO2_Item > MAX_DO_Item )//|| pSettingParam->CtrlParam.DO1_Item < MIN_DO_Item )
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.AO2_DS > MAX_AO_DS )//|| pSettingParam->CtrlParam.AO1_DS< MIN_AO_DS)
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.AO2_FS > MAX_AO_FS )//|| pSettingParam->CtrlParam.AO1_FS < MIN_AO_FS)
    {
        return FALSE;
    }
    /*********************************参数初始化标记位********************************************************************/
    if( pSettingParam->CtrlParam.AnaCalibFlag > 0x01 )  // 系统第一次初始化后
    {
        return FALSE;
    }
    if( pSettingParam->CtrlParam.INIT != 0x55 )  // 系统第一次初始化后
    {
        return FALSE;
    }
    return TRUE;
}

void MemData_CmpParam(PARAM_T *pSettingParam)
{
    bool aa = TRUE;
    aa = MemData_CmpParamType(pSettingParam);
    //if (!MemData_CmpParamType(pSettingParam))
    if(!aa)
    {
        InitCtrlParam();                     /* 初始化控制参数 */
        InitRNParam();                       /* 初始化控制参数 */
        MemData_WriteParam();
    }
}
// 设置默认参数
// 恢复默认设置值 除了PT CT 地址
void MemData_SetDefaultParam(PARAM_T *pSettingParam, BOOL bInit)
{
        //恢复出厂设置
}

u16 MemData_Crc(u8 *pData)
{
	u16 Crc = 0;
	u16 Counter = 0;
	for (Counter = 0; Counter < FLASH_PARAM_SIZE-2; Counter++)
	{
		Crc += *(pData+Counter);
	}
	return Crc;
}

// 计算参数
void MemData_CalcParam(void)
{
    //更新参数
}

/*------------------------------------------------------------------------
 * 函 数 名 : MemData_WriteParam
 * 函数功能 : 保存参数到FLASH
 * 时    间 : 2012年2月20日
 * 返 回 值 : BOOL --- TRUE 无错误
                        FALSE 无回应
 * 参数说明 : u8 Addr---芯片内部数据存储地址
              u8 *pOutData---写入数据存放区域
              u8 Count---要写入的数据长度 为0 不用填写
 * 修改历史 :
------------------------------------------------------------------------*/
void MemData_WriteParam(void)
{
	u8 FlashData[FLASH_PARAM_SIZE];
	u16 Size = 0;
	u16 Crc = 0;
	u8 *pData;

	memset((u8 *)&FlashData, 0xFF, FLASH_PARAM_SIZE);
	Size = sizeof(PARAM_T);;
	memcpy((u8 *)&FlashData, (u8 *)&g_tParam, Size);

	pData = (u8 *)&FlashData[0];
	Crc = MemData_Crc(pData);
	pData += (FLASH_PARAM_SIZE-2);
	memcpy(pData, (u8 *)&Crc, 2);
	MemData_Write_To_Flash(CONFIG_STARTADDR, FlashData, FLASH_PARAM_SIZE);

	//MemData_CalcParam(); // 每写一次就计算 数据更新
	__nop();

}

/*------------------------------------------------------------------------
 * 函 数 名 : MemData_ReadParam
 * 函数功能 : 从FLASH读参数
 * 时    间 : 2006年6月23日
 * 时    间 : 2012年2月20日
 * 返 回 值 : BOOL --- TRUE 无错误
                        FALSE 无回应
 * 参数说明 : u8 Addr---芯片内部数据存储地址
              u8 *pInData---读取数据存放区域
              u8 Count---要读取的数据长度 为0 不用填写
 * 修改历史 :
------------------------------------------------------------------------*/
void MemData_ReadParam(void)
{
	u8 FlashData[FLASH_PARAM_SIZE];
	u16 Size = 0;
	u16 Crc = 0;

	memcpy(FlashData, (u8 *)CONFIG_STARTADDR, FLASH_PARAM_SIZE);
	Crc = MemData_Crc(FlashData);
	if (Crc == FLIPW(&FlashData[FLASH_PARAM_SIZE-2]))  //CRC验证 读取是否出错
	{
		Size = sizeof(PARAM_T);;
		memcpy((u8 *)&g_tParam, (u8 *)&FlashData[0], Size);
		MemData_CmpParam(&g_tParam);
	}
	else // 失败，把当前数据写入EEPROM
	{
		MemData_WriteParam();
	}
}

// 数据初始化
void MemData_InitSettingParam(void)
{
    MemData_SetDefaultParam(&g_tParam, TRUE);
    MemData_ReadParam();
}


void MemData_InitPowerVal(void) //电能量初始化
{
    u8 ph = PH_A;
    for (ph = PH_A; ph < PH_TH; ph++)
    {
        vg_Power_Val.Real2_Val[ph].U = 22000;//二次侧电压 AC
        vg_Power_Val.Real2_Val[ph].I= 50000;//二次侧电流 B
        vg_Power_Val.Real2_Val[ph].P = 31350;//二次侧有功功率
        vg_Power_Val.Real2_Val[ph].Q = 10304;//二次侧无功功率
        vg_Power_Val.Real2_Val[ph].S = 33000;//二次侧视在功率
        vg_Power_Val.Real2_Val[ph].PF = 950;//功率因数值
    }
    vg_Power_Val.Freq = 5000;
}


// 数据初始化
void MemData_Init(void)
{
    MemData_InitSettingParam();

    MemData_InitPowerVal();

	MemData_CalcParam(); // 初始化计算

    MemData_WriteParam();

}



