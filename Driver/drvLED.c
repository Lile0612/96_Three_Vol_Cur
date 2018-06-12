/*! @file
********************************************************************************
<PRE>
模块名       : 驱动模块
文件名       : drvLED.c
相关文件     :
文件实现功能 : LED驱动程序
作者         : <--->
版本         : 1.0
--------------------------------------------------------------------------------
备注         :
--------------------------------------------------------------------------------
修改记录 :
日 期        版本   修改人         修改内容
2012/11/12   1.0    <xxxx>         创建
</PRE>
********************************************************************************

  * 版权所有(c) YYYY, <xxx>, 保留所有权利

*******************************************************************************/
#ifdef _GUILED_
/* Includes ------------------------------------------------------------------*/
#include "Include.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/

const u8 cg_DispCharacter[] = {	CHARMODE_0,/*0*/
								CHARMODE_1,/*1*/
								CHARMODE_2,/*2*/
								CHARMODE_3,/*3*/
								CHARMODE_4,/*4*/
								CHARMODE_5,/*5*/
								CHARMODE_6,/*6*/
								CHARMODE_7,/*7*/
								CHARMODE_8,/*8*/
								CHARMODE_9 /*9*/
					           };
/****************************************************************************************
** 函数名称: TM1640_delay
** 功能描述: 延时
** 参    数: None
** 返 回 值: None
** 作　  者:
** 日  　期: 2009年10月14日
**---------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**--------------------------------------------------------------------------------------
****************************************************************************************/
void TM1640_delay(void)
{
   u8 i = 30; //这里可以优化速度
   while(i)
   {
     i--;
   }
}


u8 HT_Data_Index(unsigned char data)
{
    return cg_DispCharacter[data];
}
// TM1640开始
void TM1640_Start(void)
{
	TM_DIN_H;
	TM_CLK_H;
	TM1640_delay();
	TM_DIN_L;
	TM1640_delay();
	TM_CLK_L;
	TM1640_delay();
}
// TM1640结束
void TM1640_End(void)
{
	TM_DIN_L;
	TM_CLK_L;
	TM1640_delay();
	TM_CLK_H;
	TM1640_delay();
	TM_DIN_H;
	TM1640_delay();
}
// 写一个字节数据
void TM1640_I2C_WrByte(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
        TM_CLK_L;
	    TM1640_delay();
		if(dat&0x01)
		    TM_DIN_H;
		else
		    TM_DIN_L;
		TM1640_delay();
		TM_CLK_H;
		dat>>=1;//右移一位，低位先进，串口通讯模式
		TM1640_delay();
	}
}

/***********************************
往指定地址写入一个字节的数据
先发命令，再发数据
************************************/
void TM1640_Write(unsigned char dat)
{

	TM1640_I2C_WrByte(dat);
}

void Update_screen(void)
{
		u8 i;
		TM1640_Start();
		TM1640_I2C_WrByte(0x40); // 设置自动地址
		TM1640_End();

		TM1640_Start();
		TM1640_I2C_WrByte(0xc0); // 设置起始地址

		for(i=0;i<16;i++)
		{
			TM1640_Write(vg_Display_Value[i]);     // LED灯 LED    pDisplay_Data[i]
		}

		TM1640_End();
		TM1640_Start();
        if(DispMode == ProgMode)
        {
            switch(LEDLEVEL)
            {
                case 0x01:
                    TM1640_I2C_WrByte(0X89);
                    break;
                case 0x02:
                    TM1640_I2C_WrByte(0X8A);
                    break;
                case 0x03:
                    TM1640_I2C_WrByte(0X8B);
                    break;
                default:
                    TM1640_I2C_WrByte(0X8A);
                    break;
            }
        }
        else //if(DispMode == ReadMode)
        {
            switch(g_tParam.CtrlParam.BlackTime)
            {
                case 0x01:
                    TM1640_I2C_WrByte(0X89);
                    break;
                case 0x02:
                    TM1640_I2C_WrByte(0X8A);
                    break;
                case 0x03:
                    TM1640_I2C_WrByte(0X8B);
                    break;
                default:
                    TM1640_I2C_WrByte(0X8A);
                    break;
            }
        }

		TM1640_End();

		for(i=0;i<16;i++)
		{
			vg_Display_Value[i] = 0;     // LED灯 LED    pDisplay_Data[i]
		}
}

#endif // #ifdef _GUILED_

