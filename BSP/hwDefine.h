#ifndef __HWDEFINE_H__
#define __HWDEFINE_H__
#include "global.h"

#ifdef _GUILCD_
//========================================================================================
// °´¼ü
#define PORT_KEY_ONE		GPIOC
#define PIN_KEY_ONE        	GPIO_Pin_3
#define PORT_KEY_TWO     	GPIOB
#define PIN_KEY_TWO         GPIO_Pin_5
#define PORT_KEY_THREE     	GPIOB
#define PIN_KEY_THREE       GPIO_Pin_4
#define PORT_KEY_FOUR		GPIOB
#define PIN_KEY_FOUR       	GPIO_Pin_3

#define KEY_UP_STATUS       GPIO_ReadInputDataBit(PORT_KEY_ONE, PIN_KEY_ONE)
#define KEY_DOWN_STATUS     GPIO_ReadInputDataBit(PORT_KEY_TWO, PIN_KEY_TWO)
#define KEY_MENU_STATUS     GPIO_ReadInputDataBit(PORT_KEY_THREE, PIN_KEY_THREE)
#define KEY_OK_STATUS       GPIO_ReadInputDataBit(PORT_KEY_FOUR, PIN_KEY_FOUR)

//========================================================================================
// Dout
#define PORT_CTRL1		GPIOB
#define PIN_CTRL1       GPIO_Pin_15
#define CTRL1_DisEnble  GPIO_SetBits(PORT_CTRL1, PIN_CTRL1)     // 1
#define CTRL1_Enble     GPIO_ResetBits(PORT_CTRL1, PIN_CTRL1)

#define PORT_CTRL2		GPIOC
#define PIN_CTRL2       GPIO_Pin_6
#define CTRL2_DisEnble  GPIO_SetBits(PORT_CTRL2, PIN_CTRL2)     // 2
#define CTRL2_Enble     GPIO_ResetBits(PORT_CTRL2, PIN_CTRL2)

#define PORT_CTRL3		GPIOC
#define PIN_CTRL3       GPIO_Pin_7
#define CTRL3_DisEnble  GPIO_SetBits(PORT_CTRL3, PIN_CTRL3)
#define CTRL3_Enble     GPIO_ResetBits(PORT_CTRL3, PIN_CTRL3)   // 3

#define PORT_CTRL4		GPIOC
#define PIN_CTRL4       GPIO_Pin_8
#define CTRL4_DisEnble  GPIO_SetBits(PORT_CTRL4, PIN_CTRL4)     // 4
#define CTRL4_Enble     GPIO_ResetBits(PORT_CTRL4, PIN_CTRL4)

//========================================================================================
// Din
#define PORT_DIN_STATUS1    GPIOA
#define PIN_DIN_STATUS1     GPIO_Pin_1
#define KEY_DIN_STATUS1     GPIO_ReadInputDataBit(PORT_DIN_STATUS1, PIN_DIN_STATUS1)

#define PORT_DIN_STATUS2    GPIOA
#define PIN_DIN_STATUS2     GPIO_Pin_0
#define KEY_DIN_STATUS2     GPIO_ReadInputDataBit(PORT_DIN_STATUS2, PIN_DIN_STATUS2)

#define PORT_DIN_STATUS3    GPIOB
#define PIN_DIN_STATUS3     GPIO_Pin_12
#define KEY_DIN_STATUS3     GPIO_ReadInputDataBit(PORT_DIN_STATUS3, PIN_DIN_STATUS3)

#define PORT_DIN_STATUS4    GPIOB
#define PIN_DIN_STATUS4     GPIO_Pin_13
#define KEY_DIN_STATUS4     GPIO_ReadInputDataBit(PORT_DIN_STATUS4, PIN_DIN_STATUS4)

//========================================================================================
// LCD
#define PORT_LCD_BG         GPIOB
#define PIN_LCD_BG          GPIO_Pin_0
#define PORT_LCD_CS         GPIOB
#define PIN_LCD_CS          GPIO_Pin_1
#define PORT_LCD_WR         GPIOB
#define PIN_LCD_WR          GPIO_Pin_2
#define PORT_LCD_DATA       GPIOB
#define PIN_LCD_DATA        GPIO_Pin_10

#define LCD_BG_H             GPIO_SetBits(PORT_LCD_BG , PIN_LCD_BG)
#define LCD_BG_L             GPIO_ResetBits(PORT_LCD_BG , PIN_LCD_BG)
#define LCD_CS_H             GPIO_SetBits(PORT_LCD_CS , PIN_LCD_CS)
#define LCD_CS_L             GPIO_ResetBits(PORT_LCD_CS , PIN_LCD_CS)
#define LCD_SCL_H            GPIO_SetBits(PORT_LCD_WR , PIN_LCD_WR)
#define LCD_SCL_L            GPIO_ResetBits(PORT_LCD_WR , PIN_LCD_WR)
#define LCD_SDA_H            GPIO_SetBits(PORT_LCD_DATA , PIN_LCD_DATA)
#define LCD_SDA_L            GPIO_ResetBits(PORT_LCD_DATA , PIN_LCD_DATA)
#endif // #ifdef _GUILCD_

#ifdef _GUILED_
    //========================================================================================
    // °´¼ü
    #define PORT_KEY_ONE		GPIOB
    #define PIN_KEY_ONE        	GPIO_Pin_6
    #define PORT_KEY_TWO     	GPIOB
    #define PIN_KEY_TWO         GPIO_Pin_5
    #define PORT_KEY_THREE     	GPIOB
    #define PIN_KEY_THREE       GPIO_Pin_4
    #define PORT_KEY_FOUR		GPIOB
    #define PIN_KEY_FOUR       	GPIO_Pin_3

    #define KEY_UP_STATUS       GPIO_ReadInputDataBit(PORT_KEY_ONE, PIN_KEY_ONE)
    #define KEY_DOWN_STATUS     GPIO_ReadInputDataBit(PORT_KEY_TWO, PIN_KEY_TWO)
    #define KEY_MENU_STATUS     GPIO_ReadInputDataBit(PORT_KEY_THREE, PIN_KEY_THREE)
    #define KEY_OK_STATUS       GPIO_ReadInputDataBit(PORT_KEY_FOUR, PIN_KEY_FOUR)

    //========================================================================================
    // TM1640
    #define PORT_DIN        GPIOB
    #define PIN_DIN         GPIO_Pin_11
    #define TM_DIN_H		GPIO_SetBits(PORT_DIN, PIN_DIN)
    #define TM_DIN_L		GPIO_ResetBits(PORT_DIN, PIN_DIN)

    #define PORT_CLK        GPIOB
    #define PIN_CLK         GPIO_Pin_10
    #define TM_CLK_H	    GPIO_SetBits(PORT_CLK, PIN_CLK)
    #define TM_CLK_L	    GPIO_ResetBits(PORT_CLK, PIN_CLK)
    
    //========================================================================================
    // Dout
    #define PORT_CTRL1		GPIOB
    #define PIN_CTRL1       GPIO_Pin_15
    #define CTRL1_DisEnble  GPIO_SetBits(PORT_CTRL1, PIN_CTRL1)     // 1
    #define CTRL1_Enble     GPIO_ResetBits(PORT_CTRL1, PIN_CTRL1)

    #define PORT_CTRL2		GPIOC
    #define PIN_CTRL2       GPIO_Pin_6
    #define CTRL2_DisEnble  GPIO_SetBits(PORT_CTRL2, PIN_CTRL2)     // 2
    #define CTRL2_Enble     GPIO_ResetBits(PORT_CTRL2, PIN_CTRL2)

    #define PORT_CTRL3		GPIOC
    #define PIN_CTRL3       GPIO_Pin_7
    #define CTRL3_DisEnble  GPIO_SetBits(PORT_CTRL3, PIN_CTRL3)
    #define CTRL3_Enble     GPIO_ResetBits(PORT_CTRL3, PIN_CTRL3)   // 3

    #define PORT_CTRL4		GPIOC
    #define PIN_CTRL4       GPIO_Pin_8
    #define CTRL4_DisEnble  GPIO_SetBits(PORT_CTRL4, PIN_CTRL4)     // 4
    #define CTRL4_Enble     GPIO_ResetBits(PORT_CTRL4, PIN_CTRL4)

    //========================================================================================
    // Din
    #define PORT_DIN_STATUS1    GPIOA
    #define PIN_DIN_STATUS1     GPIO_Pin_1
    #define KEY_DIN_STATUS1     GPIO_ReadInputDataBit(PORT_DIN_STATUS1, PIN_DIN_STATUS1)

    #define PORT_DIN_STATUS2    GPIOA
    #define PIN_DIN_STATUS2     GPIO_Pin_0
    #define KEY_DIN_STATUS2     GPIO_ReadInputDataBit(PORT_DIN_STATUS2, PIN_DIN_STATUS2)

    #define PORT_DIN_STATUS3    GPIOB
    #define PIN_DIN_STATUS3     GPIO_Pin_12
    #define KEY_DIN_STATUS3     GPIO_ReadInputDataBit(PORT_DIN_STATUS3, PIN_DIN_STATUS3)

    #define PORT_DIN_STATUS4    GPIOB
    #define PIN_DIN_STATUS4     GPIO_Pin_13
    #define KEY_DIN_STATUS4     GPIO_ReadInputDataBit(PORT_DIN_STATUS4, PIN_DIN_STATUS4)
#endif

//========================================================================================
// PWM
#define PORT_PWM1        GPIOB
#define PIN_PWM1         GPIO_Pin_9
#define PWM1_H			 GPIO_SetBits(PORT_PWM1, PIN_PWM1)
#define PWM1_L			 GPIO_ResetBits(PORT_PWM1, PIN_PWM1)

#define PORT_PWM2        GPIOB
#define PIN_PWM2         GPIO_Pin_8
#define PWM2_H			 GPIO_SetBits(PORT_PWM2, PIN_PWM2)
#define PWM2_L			 GPIO_ResetBits(PORT_PWM2, PIN_PWM2)

//========================================================================================
// ´®¿Ú1
#define PORT_USARTA             GPIOA
#define PIN_USARTA_TX           GPIO_Pin_9 // USART1
#define PIN_USARTA_RX           GPIO_Pin_10 // USART1
#define PINSCR_USARTA_TX        GPIO_PinSource9
#define PINSCR_USARTA_RX        GPIO_PinSource10

#define USARTx_TDR_ADDRESS               0x40013828
#define USARTx_RDR_ADDRESS               0x40013824

#define USARTx_TX_DMA_CHANNEL            DMA1_Channel2
#define USARTx_TX_DMA_FLAG_TC            DMA1_FLAG_TC2
#define USARTx_TX_DMA_FLAG_GL            DMA1_FLAG_GL2
#define USARTx_RX_DMA_CHANNEL            DMA1_Channel3
#define USARTx_RX_DMA_FLAG_TC            DMA1_FLAG_TC3
#define USARTx_RX_DMA_FLAG_GL            DMA1_FLAG_GL3

//========================================================================================
// RN8302
#define PORT_SPI_CSN			GPIOA
#define PIN_SPI_CSN         	GPIO_Pin_4
#define RN_SPI_CSN_H			GPIO_SetBits(PORT_SPI_CSN, PIN_SPI_CSN)
#define RN_SPI_CSN_L			GPIO_ResetBits(PORT_SPI_CSN, PIN_SPI_CSN)

#define PORT_SPI_SCK			GPIOA
#define PIN_SPI_SCK         	GPIO_Pin_5
#define RN_SPI_SCK_H			GPIO_SetBits(PORT_SPI_SCK, PIN_SPI_SCK)
#define RN_SPI_SCK_L			GPIO_ResetBits(PORT_SPI_SCK, PIN_SPI_SCK)

#define PORT_SPI_MOSI			GPIOA
#define PIN_SPI_MOSI         	GPIO_Pin_7
#define RN_SPI_MOSI_H			GPIO_SetBits(PORT_SPI_MOSI, PIN_SPI_MOSI)
#define RN_SPI_MOSI_L			GPIO_ResetBits(PORT_SPI_MOSI, PIN_SPI_MOSI)

#define PORT_SPI_RST			GPIOC
#define PIN_SPI_RST         	GPIO_Pin_5
#define RN_SPI_RST_H			GPIO_SetBits(PORT_SPI_RST, PIN_SPI_RST)
#define RN_SPI_RST_L			GPIO_ResetBits(PORT_SPI_RST, PIN_SPI_RST)

#define RN_PORT_MISO			GPIOA
#define RN_PIN_MISO        		GPIO_Pin_6
#define RN_SPI_MISO_IN		    GPIO_ReadInputDataBit(RN_PORT_MISO, RN_PIN_MISO)

#define PORT_SPI_INTN           GPIOC
#define PIN_SPI_INTN            GPIO_Pin_4

#endif


