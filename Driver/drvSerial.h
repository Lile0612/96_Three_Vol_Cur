/*! @file
********************************************************************************
<PRE>
模块名       : 硬件驱动模块
文件名       : drvSerial.h
相关文件     :
文件实现功能 : 串口驱动
作者         : <---->
版本         : 1.0
--------------------------------------------------------------------------------
备注         :
--------------------------------------------------------------------------------
修改记录 :
日 期        版本   修改人         修改内容
2010/10/15   1.0    <---->         创建
</PRE>
********************************************************************************

  * 版权所有(c) YYYY, <xxx>, 保留所有权利

*******************************************************************************/

#ifndef _DRVSERIAL_H
#define _DRVSERIAL_H

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} STATUS;
typedef enum {COM_STATUS_LENERR = 0, COM_STATUS_OK}COM_STATUS;
/* Private define ------------------------------------------------------------*/
// 串口数量
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#define USART_RX_BUF_MAX	128	/*必须是2的幂*/
#define USART_RX_BUF_MARK	(USART_RX_BUF_MAX-1)

#define USART_TX_BUF_MAX	128	/*必须是2的幂*/
#define USART_TX_BUF_MARK	(USART_TX_BUF_MAX-1)

extern volatile u8 vu8_sending;
extern unsigned char uart1_delay_ok;
extern unsigned char uart1_delay_number;
/* Public variables ---------------------------------------------------------*/

bool USART_IsDataInRxBuf(void);
u32 USART_GetRxBufLen(void);

COM_STATUS USART_GetRxString(u8 *pData, u16 len);
COM_STATUS USART_PutTxString(u8 *pData, u16 len);
void SERIAL_IRQHandler(void);

#endif/* _DRVSERIAL_H */

//===========================================================================
// No more.
//===========================================================================




