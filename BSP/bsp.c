/* Includes ------------------------------------------------------------------*/
#include "Include.h"

DMA_InitTypeDef  USART_DMA_InitStructure;

void RCC_Configuration(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA
						| RCC_AHBPeriph_GPIOB
						| RCC_AHBPeriph_GPIOC
						| RCC_AHBPeriph_GPIOD
						| RCC_AHBPeriph_GPIOF
						| RCC_AHBPeriph_DMA1,
						ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM14,
							ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG
							| RCC_APB2Periph_USART1| RCC_APB2Periph_TIM17 |RCC_APB2Periph_TIM16,
							ENABLE);
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
	// USART1
	GPIO_PinAFConfig(PORT_USARTA, PINSCR_USARTA_RX, GPIO_AF_1);
	GPIO_PinAFConfig(PORT_USARTA, PINSCR_USARTA_TX, GPIO_AF_1);

	GPIO_InitStructure.GPIO_Pin = PIN_USARTA_RX | PIN_USARTA_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PORT_USARTA, &GPIO_InitStructure);

    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
	// Key
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_InitStructure.GPIO_Pin = PIN_KEY_ONE;      //PB6/5/4/3
	GPIO_Init(PORT_KEY_ONE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_KEY_TWO;
	GPIO_Init(PORT_KEY_TWO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_KEY_THREE;
	GPIO_Init(PORT_KEY_THREE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(PORT_KEY_FOUR, &GPIO_InitStructure);

#ifdef _GUILED_
    //-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	// 显示1640
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

    GPIO_InitStructure.GPIO_Pin = PIN_DIN;
	GPIO_Init(PORT_DIN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_CLK;
	GPIO_Init(PORT_CLK, &GPIO_InitStructure);
#endif
    //-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//RN8302
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_Pin = PIN_SPI_MOSI;
	GPIO_Init(PORT_SPI_MOSI, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_SPI_SCK;
	GPIO_Init(PORT_SPI_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_SPI_CSN;
	GPIO_Init(PORT_SPI_CSN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_SPI_RST;
	GPIO_Init(PORT_SPI_RST, &GPIO_InitStructure);

	RN_SPI_CSN_H;
	RN_SPI_RST_H;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	GPIO_InitStructure.GPIO_Pin = PIN_SPI_INTN;
	GPIO_Init(PORT_SPI_INTN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RN_PIN_MISO;
	GPIO_Init(RN_PORT_MISO, &GPIO_InitStructure);
	
    //-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//PWM1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
    GPIO_InitStructure.GPIO_Pin =  PIN_PWM1;
    GPIO_Init(PORT_PWM1, &GPIO_InitStructure);

    //PWM2
    GPIO_InitStructure.GPIO_Pin =  PIN_PWM2;
    GPIO_Init(PORT_PWM2, &GPIO_InitStructure);

    PWM1_H;
    PWM2_H;
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    //DOUT
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	
    GPIO_InitStructure.GPIO_Pin =  PIN_CTRL1;
    GPIO_Init(PORT_CTRL1, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  PIN_CTRL2;
    GPIO_Init(PORT_CTRL2, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  PIN_CTRL3;
    GPIO_Init(PORT_CTRL3, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  PIN_CTRL4;
    GPIO_Init(PORT_CTRL4, &GPIO_InitStructure);

    CTRL1_DisEnble;
    CTRL2_DisEnble;
    CTRL3_DisEnble;
    CTRL4_DisEnble;
    
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    //DIN
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_InitStructure.GPIO_Pin = PIN_DIN_STATUS1;
	GPIO_Init(PORT_DIN_STATUS1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DIN_STATUS2;
	GPIO_Init(PORT_DIN_STATUS2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DIN_STATUS3;
	GPIO_Init(PORT_DIN_STATUS3, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DIN_STATUS4;
	GPIO_Init(PORT_DIN_STATUS4, &GPIO_InitStructure);

	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	// LCD显示
#ifdef _GUILCD_
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_BG;
	GPIO_Init(PORT_LCD_BG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_CS;
	GPIO_Init(PORT_LCD_CS, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_WR;
	GPIO_Init(PORT_LCD_WR, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_DATA;
	GPIO_Init(PORT_LCD_DATA, &GPIO_InitStructure);
#endif
}

void USART_DMAReConfig(u8* pDataBuf, u16 BufSize) //串口1发送数据
{
    DMA_DeInit(USARTx_TX_DMA_CHANNEL);
    USART_DMA_InitStructure.DMA_PeripheralBaseAddr = USARTx_TDR_ADDRESS;
    USART_DMA_InitStructure.DMA_BufferSize = (u32)BufSize;
    USART_DMA_InitStructure.DMA_MemoryBaseAddr = (u32)pDataBuf;
    USART_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    USART_DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_Init(USARTx_TX_DMA_CHANNEL, &USART_DMA_InitStructure);

	/* Clear the TC bit in the SR register by writing 0 to it */
	USART_ClearFlag(USART1, USART_FLAG_TC);
	/* Enable DMA1 USART Tx Channel */
	DMA_Cmd(USARTx_TX_DMA_CHANNEL, ENABLE);
}

void USART_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;
    // 串口1配置参数
    switch(g_tParam.CtrlParam.Baud485)
    {
        case Baud12:
            USART_InitStructure.USART_BaudRate = 1200;
            break;
        case Baud24:
            USART_InitStructure.USART_BaudRate = 2400;
            break;
        case Baud48:
            USART_InitStructure.USART_BaudRate = 4800;
            break;
        case Baud96:
            USART_InitStructure.USART_BaudRate = 9600;
            break;
        case Baud192:
            USART_InitStructure.USART_BaudRate = 19200;
            break;
        default:
            break;
    }

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	if(g_tParam.CtrlParam.DataFormat == n81)
	{
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
	}
	else if(g_tParam.CtrlParam.DataFormat == e81)
	{
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
	}
	else if(g_tParam.CtrlParam.DataFormat == o81)
	{
        USART_InitStructure.USART_Parity = USART_Parity_Odd;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
	}
	else if(g_tParam.CtrlParam.DataFormat == n82)
	{
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_StopBits = USART_StopBits_2;
	}
	else
	{
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
	}

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	USART_DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	USART_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	USART_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	USART_DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	USART_DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

}



/*******************************************************************************
* Function Name  : TIM17_Configuration
* Description    : Configures the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

u32 TimerPeriod = 0;
u32 Channel1Pulse = 0;
u32 Channel2Pulse = 0;

void TIM_PWM_Configuration_AO1(void)  //电流配置
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin =    PIN_PWM1;
    GPIO_Init(PORT_PWM1, &GPIO_InitStructure);
    GPIO_PinAFConfig(PORT_PWM1,GPIO_PinSource9,GPIO_AF_2);

    TimerPeriod = (SystemCoreClock / 1000 ) - 1;
    Channel1Pulse = (u32) (500 * (TimerPeriod - 1) / 10000);

    /*Time 定时基础设置*/
    TIM_TimeBaseStructure.TIM_Prescaler = 11;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*Time 定时设置为上升沿计算模式*/
    TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);

    /*PWM模式设置*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;//使能
    TIM_OC1Init(TIM17, &TIM_OCInitStructure);

    /*TIME17 计算器使能*/
    TIM_Cmd(TIM17, ENABLE);

    /*TIM17 主输出使能*/
    TIM_CtrlPWMOutputs(TIM17,ENABLE);
}

void TIM_PWM_Configuration_AO2(void)  //电流配置
{
    //PWM2
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin =    PIN_PWM2;
    GPIO_Init(PORT_PWM2, &GPIO_InitStructure);
    GPIO_PinAFConfig(PORT_PWM2,GPIO_PinSource8,GPIO_AF_2);
    TimerPeriod = (SystemCoreClock / 1000 ) - 1;
    Channel2Pulse = (u32) (3500 * (TimerPeriod - 1) / 10000);

    /*Time 定时基础设置*/
    TIM_TimeBaseStructure.TIM_Prescaler = 11;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*Time 定时设置为上升沿计算模式*/
    TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);

    /*PWM模式设置*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;//使能
    TIM_OC1Init(TIM16, &TIM_OCInitStructure);

    /*TIME16 计算器使能*/
    TIM_Cmd(TIM16, ENABLE);

    /*TIM16 主输出使能*/
    TIM_CtrlPWMOutputs(TIM16,ENABLE);
}

void NVIC_Configuration(void) //只用到串口1
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : EXTI_Configuration
* Description    : 外部中断
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI_Configuration(void)
{

}

void SysTick_Configuration(void)
{
  if (SysTick_Config(SystemCoreClock / 1000))//1oá??
  {
    /* Capture error */
    while (1);
  }
}


void IWDG_Configuration(void)
{
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
    {
        RCC_ClearFlag();
    }
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_32);
    IWDG_SetReload(3000);
    IWDG_ReloadCounter();
    IWDG_Enable();
}


void Bsp_Init(void)
{
	SystemInit();
    //SystemCoreClockUpdate();
	RCC_Configuration();

	GPIO_Configuration();

	//USART_Configuration();

	EXTI_Configuration();

	NVIC_Configuration();

	SysTick_Configuration();
#ifndef  IWDG_DEBUG
	IWDG_Configuration();
#endif
}


/******************* (C) EEPW.com.cn*****END OF FILE****/
