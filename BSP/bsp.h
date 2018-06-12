/*! @file
********************************************************************************
<PRE>
模块名       : 硬件驱动
文件名       : stm32f10x_target.h
相关文件     :
文件实现功能 : 硬件初始化
作者         : <xxx>
版本         : 1.0
--------------------------------------------------------------------------------
备注         :
--------------------------------------------------------------------------------
修改记录 :
日 期        版本   修改人         修改内容
2009/09/15   1.0    <xxx>         创建
</PRE>
********************************************************************************

  * 版权所有(c) YYYY, <xxx>, 保留所有权利

*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __bsp_H
#define __bsp_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

void Bsp_Init (void);
//void TIM3_Configuration(void);
void EXTI_Configuration(void);
void RCC_Configuration(void);
void USART_Configuration(void);
void ADC_Configuration(void);
void SysTick_Configuration(void);
void NVIC_Configuration(void);
void USART_DMAReConfig(u8* pDataBuf, u16 BufSize);
void TIM14_Configuration(FunctionalState State);
void TIM_PWM_Configuration_AO1(void);
void TIM_PWM_Configuration_AO2(void);


#ifdef __cplusplus
}
#endif

#endif /* __bsp */


//===========================================================================
// No more.
//===========================================================================


