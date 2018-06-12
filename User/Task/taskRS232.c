/*! @file
********************************************************************************
<PRE>
模块名       : 485通讯处理任务模块
文件名       : taskRS232.c
相关文件     :
文件实现功能 : 通讯处理
作者         : < 、>
版本         : 1.0
--------------------------------------------------------------------------------
备注         :
--------------------------------------------------------------------------------
修改记录 :
日 期        版本   修改人         修改内容
2017/03/22   1.0    < 、>           创建
</PRE>
********************************************************************************

  * 版权所有(c) YYYY, <xxx>, 保留所有权利

*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "Include.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
static const unsigned char aucCRCHi[] = {

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

    0x00, 0xC1, 0x81, 0x40

};

static const unsigned char aucCRCLo[] = {

    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,

	0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,

    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,

    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,

    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,

    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,

    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,

    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,

    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,

    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,

    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,

    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,

    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,

    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,

    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,

    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,

    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,

    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,

    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,

    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,

    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,

    0x41, 0x81, 0x80, 0x40

};

unsigned short usMBCRC16( unsigned char * pucFrame, unsigned short usLen )

{

    unsigned char ucCRCHi = 0xFF;

    unsigned char ucCRCLo = 0xFF;

    int iIndex;

    while( usLen-- )

    {

        iIndex = ucCRCLo ^ *( pucFrame++ );

        ucCRCLo = ( unsigned char )( ucCRCHi ^ aucCRCHi[iIndex] );

        ucCRCHi = aucCRCLo[iIndex];

    }

    return ( unsigned short )( ucCRCLo << 8 | ucCRCHi );

}



/* Private macro -------------------------------------------------------------*/

#define PTL_START1AAREA_ADDR	0	// 帧起始域首地址
#define PTL_START2AAREA_ADDR	3	// 帧起始域首地址
#define PTL_CTRLCODEAREA_ADDR	4	// 控制码首地址
#define PTL_LENGTHAREA_ADDR		5	// 长度域首地址
#define PTL_CMDAAREA_ADDR		7	// 命令域首地址
#define PTL_DATAAREA_ADDR		9	// 数据域首地址

enum enMSPTLSTATE  // 校表
{
	MSPTL_HEAD = 0,
	MSPTL_ADDR = 1,
	MSPTL_LENGTH = 2,
	MSPTL_TAIL = 4,
};

enum enRTUSTATE  // rtu
{
	RTU_ADDR = 0,
	RTU_FUNC = 1,
	RTU_DSADDR = 2,
	RTU_DSNUM = 3,
	RTU_CRC = 4,
};

enum enMSPTLCTRLTYPE  // 校表
{
	CTRLTYPE_QUERY = 0x01,          // 数据查询
	CTRLTYPE_SET = 0x02,            // 设置
	CTRLTYPE_ADJUSTQUERY = 0x08,    // 校准查询 、电力数据查询
	CTRLTYPE_ADJUSTSET = 0x09,      // 校准设置
};

enum enRTUCTRLTYPE  // rtu
{
    RTUCTRLTYPE_ReadRelay = 0X01,
    RTUCTRLTYPE_ReadSwitch = 0X02,
    RTUCTRLTYPE_ReadPrimaryData = 0X03,       // 读1次侧
    RTUCTRLTYPE_ReadsecondaryData = 0X04,     // 读2次侧
    RTUCTRLTYPE_RemSingleRelay = 0X05,
    RTUCTRLTYPE_RemMultiRalay = 0X0F,
    RTUCTRLTYPE_WriteCtrlData = 0X10,
};

enum enMSPTLCMDTYPE_QUERY
{
	CMDTYPE_PARAM					= 0x01, // 参数
	CMDTYPE_RUNDATA					= 0x02, // 通信值
	CMDTYPE_STATUS					= 0x03, // 状态值
	CMDTYPE_ACT						= 0x04, // 动作
	CMDTYPE_INDICATOR				= 0x05, // 状态值
	CMDTYPE_JH						= 0x06, // 状态值
	//CMDTYPE_ADJUST				    = 0x07, // 校准值
	CMDTYPE_POWER					= 0x07, // 采样值
	CMDTYPE_SIMPOWER				= 0x09, // 模拟采样值
	CMDTYPE_JUMP					= 0x0A, // 程序跳转
};


/* Private variables ---------------------------------------------------------*/
static u8 sg_MSPTLFlag;
static u8 sg_RtuFlag;

MSSendPackStruct	sg_TxPack;
MSRecvPackStruct	sg_RxPack;

RtuSendPackStruct	rtu_TxPack;
RtuRecvPackStruct	rtu_RxPack;

/* Private function prototypes -----------------------------------------------*/
BOOL vMSRS232_RxProtocol(void);
void vMSRS232_SetFrameData(void);
void Task_RS232_Function(void);

/* Private functions ---------------------------------------------------------*/

void Task_RS232_init(void)
{
	sg_MSPTLFlag = MSPTL_HEAD;
	sg_RtuFlag = RTU_ADDR;
	memset((u8 *)&sg_RxPack, INIT_ZERO_VAL, sizeof(MSRecvPackStruct));
	memset((u8 *)&rtu_RxPack, INIT_ZERO_VAL, sizeof(RtuRecvPackStruct));
}

/****************************************************************************************
** 函数名称: Task_Ptl_Function
** 功能描述:
** 参    数: None
** 返 回 值: None
** 作　  者:
** 日  　期: 2010年10月05日
**---------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**--------------------------------------------------------------------------------------
****************************************************************************************/
void Task_RS232_Function(void)
{
	vMSRS232_RxProtocol();
}

// CRC校准
u8 MSRS232_CRC(u8 *pbyData, u16 wLen)
{
	u16 wCrc = 0;
	u16 Index = 0;
	u8 byCrc = 0;

	for (Index = 0; Index < wLen; Index++)
	{
		wCrc += *(pbyData + Index);
	}
	byCrc = (u8)wCrc;
	return byCrc;
}

// 组帧 发送数据
void vMSRS232_SetFrameData(void)
{
	u8 Crc = 0;

	sg_TxPack.Len += 1;
	sg_TxPack.Data[0] = 0x68;
	sg_TxPack.Data[1] = sg_TxPack.Addr1;
	sg_TxPack.Data[2] = sg_TxPack.Addr2;
	sg_TxPack.Data[3] = 0x68;
	sg_TxPack.Data[4] = sg_TxPack.CtrlCode;
	sg_TxPack.Data[5] = WORD_LO(sg_TxPack.Len);
	sg_TxPack.Data[6] = WORD_HI(sg_TxPack.Len);
	sg_TxPack.Data[7] = sg_TxPack.Cmd;

	Crc = MSRS232_CRC(sg_TxPack.Data, 7+sg_TxPack.Len);  // 0x68起 数据多6(<6+1)
	sg_TxPack.Data[7+sg_TxPack.Len] = Crc;
	sg_TxPack.Data[8+sg_TxPack.Len] = 0x16;
    USART_DMAReConfig(sg_TxPack.Data, 9+sg_TxPack.Len);
}

// ModeBus-RTU 组帧 发送数据
void vModeBus_SetFrameData(void)
{
	u16 Crc = 0;
    switch(rtu_TxPack.RTU_Func)
    {
        case 0x01:
            rtu_TxPack.Data[0] = rtu_TxPack.RTU_Addr;
            rtu_TxPack.Data[1] = rtu_TxPack.RTU_Func;
            rtu_TxPack.Data[2] = rtu_TxPack.RTU_DataNum;
            rtu_TxPack.Data[3] = rtu_TxPack.RTU_Data;
            Crc = usMBCRC16(rtu_TxPack.Data,4);
            rtu_TxPack.Data[4] = (Crc>>8)&0xff;
            rtu_TxPack.Data[5] = Crc & 0xff;
            USART_DMAReConfig(rtu_TxPack.Data, 6);
            break;
        case 0x02:
            rtu_TxPack.Data[0] = rtu_TxPack.RTU_Addr;
            rtu_TxPack.Data[1] = rtu_TxPack.RTU_Func;
            rtu_TxPack.Data[2] = rtu_TxPack.RTU_DataNum;
            rtu_TxPack.Data[3] = rtu_TxPack.RTU_Data;
            Crc = usMBCRC16(rtu_TxPack.Data,4);
            rtu_TxPack.Data[4] = (Crc>>8)&0xff;
            rtu_TxPack.Data[5] = Crc & 0xff;
            USART_DMAReConfig(rtu_TxPack.Data, 6);
            break;
        case 0x03:
            rtu_TxPack.Data[0] = rtu_TxPack.RTU_Addr;
            rtu_TxPack.Data[1] = rtu_TxPack.RTU_Func;
            rtu_TxPack.Data[2] = rtu_TxPack.RTU_DataNum;
            Crc = usMBCRC16(rtu_TxPack.Data,rtu_TxPack.RTU_DataNum + 3);
            rtu_TxPack.Data[rtu_TxPack.RTU_DataNum + 3] = (Crc>>8)&0xff;
            rtu_TxPack.Data[rtu_TxPack.RTU_DataNum + 4] = Crc & 0xff;
            USART_DMAReConfig(rtu_TxPack.Data, rtu_TxPack.RTU_DataNum + 5);
            break;
        case 0x04:
            rtu_TxPack.Data[0] = rtu_TxPack.RTU_Addr;
            rtu_TxPack.Data[1] = rtu_TxPack.RTU_Func;
            rtu_TxPack.Data[2] = rtu_TxPack.RTU_DataNum;
            Crc = usMBCRC16(rtu_TxPack.Data,rtu_TxPack.RTU_DataNum + 3);
            rtu_TxPack.Data[rtu_TxPack.RTU_DataNum + 3] = (Crc>>8)&0xff;
            rtu_TxPack.Data[rtu_TxPack.RTU_DataNum + 4] = Crc & 0xff;
            USART_DMAReConfig(rtu_TxPack.Data, rtu_TxPack.RTU_DataNum + 5);
            break;
        case 0x05:
            rtu_TxPack.Data[0] = rtu_TxPack.RTU_Addr;
            rtu_TxPack.Data[1] = rtu_TxPack.RTU_Func;
            rtu_TxPack.Data[2] = WORD_HI(rtu_TxPack.RTU_DsAddr);
            rtu_TxPack.Data[3] = WORD_LO(rtu_TxPack.RTU_DsAddr);
            rtu_TxPack.Data[4] = WORD_HI(rtu_TxPack.RTU_DsData);
            rtu_TxPack.Data[5] = WORD_LO(rtu_TxPack.RTU_DsData);
            Crc = usMBCRC16(rtu_TxPack.Data,rtu_TxPack.RTU_DataNum);
            rtu_TxPack.Data[rtu_TxPack.RTU_DataNum] = (Crc>>8)&0xff;
            rtu_TxPack.Data[rtu_TxPack.RTU_DataNum + 1] = Crc & 0xff;
            USART_DMAReConfig(rtu_TxPack.Data, rtu_TxPack.RTU_DataNum + 2);
            break;
        case 0x0F:
            rtu_TxPack.Data[0] = rtu_TxPack.RTU_Addr;
            rtu_TxPack.Data[1] = rtu_TxPack.RTU_Func;
            rtu_TxPack.Data[2] = WORD_HI(rtu_TxPack.RTU_DsAddr);
            rtu_TxPack.Data[3] = WORD_LO(rtu_TxPack.RTU_DsAddr);
            rtu_TxPack.Data[4] = WORD_HI(rtu_TxPack.RTU_DsNum);
            rtu_TxPack.Data[5] = WORD_LO(rtu_TxPack.RTU_DsNum);
            Crc = usMBCRC16(rtu_TxPack.Data,rtu_TxPack.RTU_DataNum);
            rtu_TxPack.Data[rtu_TxPack.RTU_DataNum] = (Crc>>8)&0xff;
            rtu_TxPack.Data[rtu_TxPack.RTU_DataNum + 1] = Crc & 0xff;
            USART_DMAReConfig(rtu_TxPack.Data, rtu_TxPack.RTU_DataNum + 2);
            break;
        case 0x10:
            rtu_TxPack.Data[0] = rtu_TxPack.RTU_Addr;
            rtu_TxPack.Data[1] = rtu_TxPack.RTU_Func;
            rtu_TxPack.Data[2] = WORD_HI(rtu_TxPack.RTU_DsAddr);
            rtu_TxPack.Data[3] = WORD_LO(rtu_TxPack.RTU_DsAddr);
            rtu_TxPack.Data[4] = WORD_HI(rtu_TxPack.RTU_DsNum);
            rtu_TxPack.Data[5] = WORD_LO(rtu_TxPack.RTU_DsNum);
            Crc = usMBCRC16(rtu_TxPack.Data,rtu_TxPack.RTU_DataNum);
            rtu_TxPack.Data[rtu_TxPack.RTU_DataNum] = (Crc>>8)&0xff;
            rtu_TxPack.Data[rtu_TxPack.RTU_DataNum + 1] = Crc & 0xff;
            USART_DMAReConfig(rtu_TxPack.Data, rtu_TxPack.RTU_DataNum + 2);
            break;
        default:
            break;
    }
}
void vMSRS232_Query_Param(void) // 查询控制参数
{
    sg_TxPack.Len = sizeof(SX_PARAM);
    memcpy((u8 *)&sg_TxPack.Data[PTL_DATAAREA_ADDR], (u8 *)&g_tParam.CtrlParam, sg_TxPack.Len);
}

void vMSRS232_Query_Status(void)
{
    // 状态查询
}

void vMSRS232_Query_Adjust(void) // 查询校准参数
{
    sg_TxPack.Len = sizeof(RN_COEFF) + 1;   // 校准数据长度
    sg_TxPack.Data[PTL_CMDAAREA_ADDR+1] = 0xFC;
    memcpy((u8 *)&sg_TxPack.Data[PTL_DATAAREA_ADDR], (u8 *)&g_tParam.RN_Coefficients, sg_TxPack.Len);
}

void vMSRS232_Query_Power(void) // 交流采样值
{
    sg_TxPack.Len = sizeof(vg_Read_Val) + 1;
    sg_TxPack.Data[PTL_CMDAAREA_ADDR+1] = 0x01;
    memcpy((u8 *)&sg_TxPack.Data[PTL_DATAAREA_ADDR], (u8 *)&vg_Read_Val, sg_TxPack.Len);
}

void vMSRS232_Query_Ver(void)    // 放在控制参数内
{
//	sg_TxPack.Data[8] = CMDTYPE_VER;
//	sg_TxPack.Data[9] = 0x01;
//	sg_TxPack.Data[10] = 0x01;
//	sg_TxPack.Len = 2;
}

/*------------------------ Modebus-RTU 处理 --------------------- */
void vModeBus_Query_Primary(void)
{
    u8 i,j;
    u8 charArray[36][4];
    u8 * pdata;
    for(i = 0; i<36;i++)
    {
        pdata = (u8*)&(RtuPrimaryData[i]);
        for(j=0;j<4;j++)
        {
            charArray[i][3-j] = *pdata++;
        }
    }
    void   *pf;
    u8* px;
    for(i = 0; i<36;i++)
    {
        pf = &(RtuPrimaryData[i]);
        px = &(charArray[i][0]);
        for(j=0;j<4;j++)
        {
            *((unsigned char*)pf+j)=*(px+j);
        }
    }
}
void vModeBus_Query_Secondary(void)
{
    u8 j,Hj,Lo;
    for(j = 0;j<192;j++)
    {
        Lo = WORD_LO(RtuSecondaryData[j]);
        Hj = WORD_HI(RtuSecondaryData[j]);
        RtuSecondaryData[j] = Lo<<8 | Hj;
    }
}
void vModeBus_Query_THD(void)
{
    int index,th;
    u8 i,Hi,Lo;
#ifdef _LEDHarm_
    for(th = 0;th < 3;th++)
    {
        RtuTHDData[Index_4_THD_Ua + th] = vg_Power2_Val.Real22_Val[th].THDU;
        RtuTHDData[Index_4_THD_Ia + th] = vg_Power2_Val.Real22_Val[th].THDI;
        RtuTHDData[Index_4_H_Ua + th] = vg_Power2_Val.Real22_Val[th].UH;
        RtuTHDData[Index_4_H_Ia + th] = vg_Power2_Val.Real22_Val[th].IH;
    }
    for(index = 0;index < 15;index++)
    {
        RtuTHDData[Index_4_THD_3Ua + index*6] = HarmonicData.HarmonicpercentU[0][index*2 + 2]/100;
        RtuTHDData[Index_4_THD_3Ub + index*6] = HarmonicData.HarmonicpercentU[1][index*2 + 2]/100;
        RtuTHDData[Index_4_THD_3Uc + index*6] = HarmonicData.HarmonicpercentU[2][index*2 + 2]/100;

        RtuTHDData[Index_4_THD_3Ia + index*6] = HarmonicData.HarmonicpercentI[0][index*2 + 2]/100;
        RtuTHDData[Index_4_THD_3Ib + index*6] = HarmonicData.HarmonicpercentI[1][index*2 + 2]/100;
        RtuTHDData[Index_4_THD_3Ic + index*6] = HarmonicData.HarmonicpercentI[2][index*2 + 2]/100;
    }
    for(i = 0;i<102;i++)
    {
        Lo = WORD_LO(RtuTHDData[i]);
        Hi = WORD_HI(RtuTHDData[i]);
        RtuTHDData[i] = Lo<<8 | Hi;
    }
#else
    for(i = 0;i<102;i++)
    {
        RtuTHDData[i] = 0xffff;
    }
#endif
}

void vModeBus_Query_CTRL(void)
{
    u8 i,Hi,Lo;
    RtuCTRLData[Index_8_Net] = g_tParam.CtrlParam.NetMode;
    RtuCTRLData[Index_8_Pt]  = g_tParam.CtrlParam.PTNum;
    RtuCTRLData[Index_8_Ct]  = g_tParam.CtrlParam.CTNum;

    RtuCTRLData[Index_8_Addr] = g_tParam.CtrlParam.Addr485;
    RtuCTRLData[Index_8_Baud] = g_tParam.CtrlParam.Baud485;
    RtuCTRLData[Index_8_Data] = g_tParam.CtrlParam.DataFormat;

    RtuCTRLData[Index_8_Cyc]  = g_tParam.CtrlParam.Cycle;
    RtuCTRLData[Index_8_Ligh] = g_tParam.CtrlParam.BlackTime;

    RtuCTRLData[Index_8_AO1_Mode] = g_tParam.CtrlParam.AO1_Mode;
    RtuCTRLData[Index_8_AO1_Item] = g_tParam.CtrlParam.AO1_Item;
    RtuCTRLData[Index_8_AO1_Ds] = g_tParam.CtrlParam.AO1_DS;
    RtuCTRLData[Index_8_AO1_Fs] = g_tParam.CtrlParam.AO1_FS;

    RtuCTRLData[Index_8_AO2_Mode] = g_tParam.CtrlParam.AO2_Mode;
    RtuCTRLData[Index_8_AO2_Item] = g_tParam.CtrlParam.AO2_Item;
    RtuCTRLData[Index_8_AO2_Ds] = g_tParam.CtrlParam.AO2_DS;
    RtuCTRLData[Index_8_AO2_Fs] = g_tParam.CtrlParam.AO2_FS;

    RtuCTRLData[Index_8_DO1_Mode] = g_tParam.CtrlParam.DO1_Mode;
    RtuCTRLData[Index_8_DO1_Time] = g_tParam.CtrlParam.DO1_Time;
    RtuCTRLData[Index_8_DO1_Item] = g_tParam.CtrlParam.DO1_Item;
    RtuCTRLData[Index_8_DO1_Dely] = g_tParam.CtrlParam.DO1_Delay;
    RtuCTRLData[Index_8_DO1_Ual] = g_tParam.CtrlParam.DO1_UAL;
    RtuCTRLData[Index_8_DO1_Hys] = g_tParam.CtrlParam.DO1_HYS;

    RtuCTRLData[Index_8_DO2_Mode] = g_tParam.CtrlParam.DO2_Mode;
    RtuCTRLData[Index_8_DO2_Time] = g_tParam.CtrlParam.DO2_Time;
    RtuCTRLData[Index_8_DO2_Item] = g_tParam.CtrlParam.DO2_Item;
    RtuCTRLData[Index_8_DO2_Dely] = g_tParam.CtrlParam.DO2_Delay;
    RtuCTRLData[Index_8_DO2_Ual] = g_tParam.CtrlParam.DO2_UAL;
    RtuCTRLData[Index_8_DO2_Hys] = g_tParam.CtrlParam.DO2_HYS;

    RtuCTRLData[Index_8_DO3_Mode] = g_tParam.CtrlParam.DO3_Mode;
    RtuCTRLData[Index_8_DO3_Time] = g_tParam.CtrlParam.DO3_Time;
    RtuCTRLData[Index_8_DO3_Item] = g_tParam.CtrlParam.DO3_Item;
    RtuCTRLData[Index_8_DO3_Dely] = g_tParam.CtrlParam.DO3_Delay;
    RtuCTRLData[Index_8_DO3_Ual] = g_tParam.CtrlParam.DO3_UAL;
    RtuCTRLData[Index_8_DO3_Hys] = g_tParam.CtrlParam.DO3_HYS;

    RtuCTRLData[Index_8_DO4_Mode] = g_tParam.CtrlParam.DO4_Mode;
    RtuCTRLData[Index_8_DO4_Time] = g_tParam.CtrlParam.DO4_Time;
    RtuCTRLData[Index_8_DO4_Item] = g_tParam.CtrlParam.DO4_Item;
    RtuCTRLData[Index_8_DO4_Dely] = g_tParam.CtrlParam.DO4_Delay;
    RtuCTRLData[Index_8_DO4_Ual] = g_tParam.CtrlParam.DO4_UAL;
    RtuCTRLData[Index_8_DO4_Hys] = g_tParam.CtrlParam.DO4_HYS;

    for(i = 0;i<64;i++)
    {
        Lo = WORD_LO(RtuCTRLData[i]);
        Hi = WORD_HI(RtuCTRLData[i]);
        RtuCTRLData[i] = Lo<<8 | Hi;
    }
}

void vModeBus_Set_Relay(void)
{
    if(rtu_TxPack.RTU_DsData == 0xFF00)
    {
        RemOutCtrl |= g_Din_BIT[rtu_TxPack.RTU_DsAddr];
    }
    else if(rtu_TxPack.RTU_DsData == 0x0000)
    {
        RemOutCtrl &= ~g_Din_BIT[rtu_TxPack.RTU_DsAddr];
    }
}

void vModeBus_Set_Multi_Relay(void)
{
    u8 i,TempData;
    TempData = rtu_RxPack.RR_Data;
    for(i =0 ;i<rtu_RxPack.RR_DsNum;i++)
    {
        if(TempData & 0x01)
        {
            RemOutCtrl |= g_Din_BIT[rtu_TxPack.RTU_DsAddr + i];
        }
        else
        {
            RemOutCtrl &= ~g_Din_BIT[rtu_TxPack.RTU_DsAddr + i];
        }
        TempData >>= 1;
    }
}

void vModeBus_Set_Ctrl_Parm(void)
{
    u8 Len,i;
    u16 DsAddr;
    BOOL UpDataRTU = FALSE;
    Len = rtu_RxPack.RR_DataLen/2;
    for(i = 0; i<Len; i++)
    {
        DsAddr = rtu_RxPack.RR_DsAddr - 0x0800 + i;
        RtuCTRLDataTemp[DsAddr] = FHIPW(&rtu_RxPack.Data[7+i*2]);
        switch(rtu_RxPack.RR_DsAddr - 0x0800 + i)
        {
            case Index_8_Net:
                g_tParam.CtrlParam.NetMode=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.NetMode,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_NetMode,MAX_NetMode);
				break;
		    case Index_8_Pt:
                g_tParam.CtrlParam.PTNum=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.PTNum,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_PT,MAX_PT);
				break;
		    case Index_8_Ct:
                g_tParam.CtrlParam.CTNum=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.CTNum,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_CT,MAX_CT);
				break;
		    case Index_8_Addr:
                g_tParam.CtrlParam.Addr485=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.Addr485,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_Addr,MAX_Addr);
				break;
			case Index_8_Baud:
                g_tParam.CtrlParam.Baud485=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.Baud485,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_Baud,MAX_Baud);
				break;
			case Index_8_Data:
                g_tParam.CtrlParam.DataFormat=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DataFormat,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DataFormat,MAX_DataFormat);
				break;
			case Index_8_Cyc:
                g_tParam.CtrlParam.Cycle=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.Cycle,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_Cycle,MAX_Cycle);
				break;
			case Index_8_Ligh:
                g_tParam.CtrlParam.BlackTime=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.BlackTime,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_BlackTime,MAX_BlackTime);
				break;
			case Index_8_AO1_Mode:
                g_tParam.CtrlParam.AO1_Mode=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.AO1_Mode,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_AO_Mode,MAX_AO_Mode);
				break;
			case Index_8_AO1_Item:
                g_tParam.CtrlParam.AO1_Item=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.AO1_Item,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_AO_Item,MAX_AO_Item);
				break;
			case Index_8_AO1_Fs:
                g_tParam.CtrlParam.AO1_FS=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.AO1_FS,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_AO_FS,MAX_AO_FS);
				break;
			case Index_8_AO1_Ds:
                g_tParam.CtrlParam.AO1_DS=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.AO1_DS,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_AO_DS,MAX_AO_DS);
				break;
			case Index_8_AO2_Mode:
                g_tParam.CtrlParam.AO2_Mode=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.AO2_Mode,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_AO_Mode,MAX_AO_Mode);
				break;
			case Index_8_AO2_Item:
                g_tParam.CtrlParam.AO2_Item=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.AO2_Item,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_AO_Item,MAX_AO_Item);
				break;
			case Index_8_AO2_Fs:
                g_tParam.CtrlParam.AO2_FS=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.AO2_FS,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_AO_FS,MAX_AO_FS);
				break;
			case Index_8_AO2_Ds:
                g_tParam.CtrlParam.AO2_DS=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.AO2_DS,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_AO_DS,MAX_AO_DS);
				break;
			/*-------------------------------------------------------------------------------------------------------------------------------------*/
			case Index_8_DO1_Mode:
                g_tParam.CtrlParam.DO1_Mode=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO1_Mode,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Mode,MAX_DO_Mode);
				break;
			case Index_8_DO1_Time:
                g_tParam.CtrlParam.DO1_Time=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO1_Time,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Time,MAX_DO_Time);
				break;
			case Index_8_DO1_Item:
                g_tParam.CtrlParam.DO1_Item=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO1_Item,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Item,MAX_DO_Item);
				break;
			case Index_8_DO1_Ual:
                g_tParam.CtrlParam.DO1_UAL=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO1_UAL,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Ual,MAX_DO_Ual);
				break;
			case Index_8_DO1_Hys:
                g_tParam.CtrlParam.DO1_HYS=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO1_HYS,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Hys,MAX_DO_Hys);
				break;
			case Index_8_DO1_Dely:
                g_tParam.CtrlParam.DO1_Delay=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO1_Delay,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Dely,MAX_DO_Dely);
				break;
			/*-------------------------------------------------------------------------------------------------------------------------------------*/
			case Index_8_DO2_Mode:
                g_tParam.CtrlParam.DO2_Mode=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO2_Mode,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Mode,MAX_DO_Mode);
				break;
			case Index_8_DO2_Time:
                g_tParam.CtrlParam.DO2_Time=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO2_Time,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Time,MAX_DO_Time);
				break;
			case Index_8_DO2_Item:
                g_tParam.CtrlParam.DO2_Item=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO2_Item,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Item,MAX_DO_Item);
				break;
			case Index_8_DO2_Ual:
                g_tParam.CtrlParam.DO2_UAL=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO2_UAL,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Ual,MAX_DO_Ual);
				break;
			case Index_8_DO2_Hys:
                g_tParam.CtrlParam.DO2_HYS=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO2_HYS,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Hys,MAX_DO_Hys);
				break;
			case Index_8_DO2_Dely:
                g_tParam.CtrlParam.DO2_Delay=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO2_Delay,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Dely,MAX_DO_Dely);
				break;
			/*-------------------------------------------------------------------------------------------------------------------------------------*/
			case Index_8_DO3_Mode:
                g_tParam.CtrlParam.DO3_Mode=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO3_Mode,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Mode,MAX_DO_Mode);
				break;
			case Index_8_DO3_Time:
                g_tParam.CtrlParam.DO3_Time=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO3_Time,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Time,MAX_DO_Time);
				break;
			case Index_8_DO3_Item:
                g_tParam.CtrlParam.DO3_Item=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO3_Item,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Item,MAX_DO_Item);
				break;
			case Index_8_DO3_Ual:
                g_tParam.CtrlParam.DO3_UAL=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO3_UAL,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Ual,MAX_DO_Ual);
				break;
			case Index_8_DO3_Hys:
                g_tParam.CtrlParam.DO3_HYS=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO3_HYS,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Hys,MAX_DO_Hys);
				break;
			case Index_8_DO3_Dely:
                g_tParam.CtrlParam.DO3_Delay=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO3_Delay,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Dely,MAX_DO_Dely);
				break;
			/*-------------------------------------------------------------------------------------------------------------------------------------*/
			case Index_8_DO4_Mode:
                g_tParam.CtrlParam.DO4_Mode=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO4_Mode,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Mode,MAX_DO_Mode);
				break;
			case Index_8_DO4_Time:
                g_tParam.CtrlParam.DO4_Time=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO4_Time,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Time,MAX_DO_Time);
				break;
			case Index_8_DO4_Item:
                g_tParam.CtrlParam.DO4_Item=
			    (u8)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO4_Item,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Item,MAX_DO_Item);
				break;
			case Index_8_DO4_Ual:
                g_tParam.CtrlParam.DO4_UAL=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO4_UAL,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Ual,MAX_DO_Ual);
				break;
			case Index_8_DO4_Hys:
                g_tParam.CtrlParam.DO4_HYS=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO4_HYS,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Hys,MAX_DO_Hys);
				break;
			case Index_8_DO4_Dely:
                g_tParam.CtrlParam.DO4_Delay=
			    (u16)JudgeSetNewValue(&UpDataRTU,(s32)g_tParam.CtrlParam.DO4_Delay,(s32*)&RtuCTRLDataTemp[DsAddr],MIN_DO_Dely,MAX_DO_Dely);
				break;
		    default:
		        break;
        }
    }
    if(UpDataRTU)
    {
        SaveParam();
    }

}


/*--------------------------------------------------------------- */

void vMSRS232_Set_Param(u8 *pData)  // 参数设置(验证正确性)
{
    BOOL bUpdata = FALSE;
    SX_PARAM tmp_CtrlParam;
    tmp_CtrlParam.NetMode = *pData;        // Buf[8] 开始
    tmp_CtrlParam.Addr485 = *(pData+1);
    tmp_CtrlParam.DataFormat = *(pData+2);
    tmp_CtrlParam.DataProtocol = *(pData+3);
    tmp_CtrlParam.Cycle = *(pData+4);
    tmp_CtrlParam.BlackTime = *(pData+5);
    tmp_CtrlParam.DefaultDisp = *(pData+6);
    tmp_CtrlParam.tmpAlign = *(pData+7);
    tmp_CtrlParam.PTNum = FLIPW(pData+8);
    tmp_CtrlParam.CTNum = FLIPW(pData+10);
    tmp_CtrlParam.Baud485 =FLIPW(pData+12);
    tmp_CtrlParam.Alarm =FLIPW(pData+14);
    tmp_CtrlParam.Code = FLIPW(pData+16);

    tmp_CtrlParam.DO1_Mode = *(pData+18);
    tmp_CtrlParam.DO1_Item = *(pData+19);
    tmp_CtrlParam.DO1_Time = *(pData+20);
    tmp_CtrlParam.DO1_UAL = *(pData+22);
    tmp_CtrlParam.DO1_HYS = *(pData+24);
    tmp_CtrlParam.DO1_Delay = *(pData+26);

    tmp_CtrlParam.DO2_Mode = *(pData+28);
    tmp_CtrlParam.DO2_Item = *(pData+29);
    tmp_CtrlParam.DO2_Time = *(pData+30);
    tmp_CtrlParam.DO2_UAL = *(pData+32);
    tmp_CtrlParam.DO2_HYS = *(pData+34);
    tmp_CtrlParam.DO2_Delay = *(pData+36);

    tmp_CtrlParam.DO3_Mode = *(pData+38);
    tmp_CtrlParam.DO3_Item = *(pData+39);
    tmp_CtrlParam.DO3_Time = *(pData+40);
    tmp_CtrlParam.DO3_UAL = *(pData+42);
    tmp_CtrlParam.DO3_HYS = *(pData+44);
    tmp_CtrlParam.DO3_Delay = *(pData+46);

    tmp_CtrlParam.AO1_Mode = *(pData+48);
    tmp_CtrlParam.AO1_Item = *(pData+49);
    tmp_CtrlParam.AO1_DS = *(pData+50);
    tmp_CtrlParam.AO1_FS = *(pData+52);



    g_tParam.CtrlParam.Baud485=
        (u16)JudgeSetNewValue(&bUpdata,(s32)g_tParam.CtrlParam.Baud485,(s32*)&tmp_CtrlParam.Baud485,MIN_Baud,MAX_Baud);
    g_tParam.CtrlParam.DataFormat=
        (u8)JudgeSetNewValue(&bUpdata,(s32)g_tParam.CtrlParam.DataFormat,(s32*)&tmp_CtrlParam.DataFormat,MIN_DataFormat,MAX_DataFormat);

    if(bUpdata)
    {
        USART_Configuration();  //串口通信参数发生改变 重新配置
        PT_TimerDelay(10);
    }

    g_tParam.CtrlParam.NetMode =
        (u8)JudgeSetNewValue(&bUpdata,(s32)g_tParam.CtrlParam.NetMode,(s32*)&tmp_CtrlParam.NetMode,MIN_NetMode,MAX_NetMode);
    g_tParam.CtrlParam.Addr485=
        (u8)JudgeSetNewValue(&bUpdata,(s32)g_tParam.CtrlParam.Addr485,(s32*)&tmp_CtrlParam.Addr485,MIN_Addr,MAX_Addr);
    g_tParam.CtrlParam.BlackTime=
        (u8)JudgeSetNewValue(&bUpdata,(s32)g_tParam.CtrlParam.BlackTime,(s32*)&tmp_CtrlParam.BlackTime,MIN_BlackTime,MAX_BlackTime);
    g_tParam.CtrlParam.Code=
        (u16)JudgeSetNewValue(&bUpdata,(s32)g_tParam.CtrlParam.Code,(s32*)&tmp_CtrlParam.Code,MIN_Code,MAX_Code);
    g_tParam.CtrlParam.CTNum=
        (u16)JudgeSetNewValue(&bUpdata,(s32)g_tParam.CtrlParam.CTNum,(s32*)&tmp_CtrlParam.CTNum,MIN_CT,MAX_CT);
    g_tParam.CtrlParam.PTNum=
        (u16)JudgeSetNewValue(&bUpdata,(s32)g_tParam.CtrlParam.PTNum,(s32*)&tmp_CtrlParam.PTNum,MIN_PT,MAX_PT);
    g_tParam.CtrlParam.Alarm=
        (u16)JudgeSetNewValue(&bUpdata,(s32)g_tParam.CtrlParam.Alarm,(s32*)&tmp_CtrlParam.Alarm,MIN_Alarm,MAX_Alarm);
    g_tParam.CtrlParam.Cycle=
        (u16)JudgeSetNewValue(&bUpdata,(s32)g_tParam.CtrlParam.Cycle,(s32*)&tmp_CtrlParam.Cycle,MIN_Cycle,MAX_Cycle);

    if(bUpdata)
    {
        SaveParam();
    }
}

void vMSRS232_Set_Adjust(u8 *pData)  // 设置校验系数
{
    u16 U[PH_TH];
    u16 I[PH_TH];
    u16 Ao[4];
    AdjustMsgStruct SendAdjustMsg;
    u16 Size;
    u16 AoNum;

    // 校准或直接写寄存器的值  校准模式
    switch (*pData)   // Buf[8] 开始  子命令 相位 校准数据A相
    {
        case CALIB_AnoPwm:
            Ao[0] = FLIPW(pData+2);
            Ao[1] = FLIPW(pData+4);
            Ao[2] = FLIPW(pData+6);
            Ao[3] = FLIPW(pData+8);
            Size = sizeof(Ao);
            SendAdjustMsg.Cmd = *pData;
            SendAdjustMsg.Phase = *(pData+1);
            memcpy((u8 *)&SendAdjustMsg.Data[0], (u8 *)&Ao[0], Size);
            RN8032_Adjust(SendAdjustMsg.Cmd,SendAdjustMsg.Phase,(u8 *) &SendAdjustMsg.Data[0]);
            break;
        case CALIB_SetPwm:
            SendAdjustMsg.Cmd = *pData;
            SendAdjustMsg.Phase = *(pData+1);
            if(SendAdjustMsg.Phase == 0x00)
            {
                AoNum = (u16)(500 * (47999 - 1) / 10000);
            }
            else
            {
                AoNum = (u16)(3500 * (47999 - 1) / 10000);
            }
            TIM_SetCompare1(TIM16,AoNum);
            TIM_SetCompare1(TIM17,AoNum);
            break;
        default:
            break;
    }
}

/**
* @Description  解析协议
* @Input        None
* @Output       None
* @Return       None
* @Others       None
*/
void vMSRS232_Analysis(void)
{
	BOOL bAnswer = TRUE;
	u8 CtrlType = sg_RxPack.CtrlCode;    // 读取 or 写入
	u8 CmdType = sg_RxPack.Cmd;          // 数据种类 系数 数据 参数
	u8 *pData = &sg_RxPack.Data[8];

    if( sg_RxPack.Addr2 == ADDR_ALL || sg_RxPack.Addr2 == g_tParam.CtrlParam.Addr485\
      || sg_RxPack.Addr1 == ADDR_PC )    // 仪表地址 判断地址
    {
    	sg_TxPack.Addr1 = sg_RxPack.Addr2;
    	sg_TxPack.Addr2 = sg_RxPack.Addr1;
    	sg_TxPack.CtrlCode = sg_RxPack.CtrlCode + 0x80;
    	sg_TxPack.Cmd = sg_RxPack.Cmd;
    	if (CtrlType == CTRLTYPE_ADJUSTQUERY)  // 读取
    	{
    		switch (CmdType)
    		{
        		case CMDTYPE_PARAM:
        			vMSRS232_Query_Param();   // 参数
        			break;
        		case CMDTYPE_RUNDATA:
                    //vMSRS232_Query_RunData();   //  通信
        			break;
        		case CMDTYPE_STATUS:           // 状态
        			break;
        		case CMDTYPE_POWER:
        		    if(sg_RxPack.Data[8] == 0x01)
        		    {
                        vMSRS232_Query_Power();   // 读取交流采样值
        		    }
        			else if(sg_RxPack.Data[8] == 0xFC)
        			{
                        vMSRS232_Query_Adjust();  // 读取校准值
        			}
        			break;
    		}
    	}
    	else if (CtrlType == CTRLTYPE_ADJUSTSET)  // 设置
    	{
    		switch (CmdType)
    		{
        		case CMDTYPE_PARAM:
        			vMSRS232_Set_Param(pData);    // 参数
        			break;
        		case CMDTYPE_POWER:
        			vMSRS232_Set_Adjust(pData);   // 校准
        			vMSRS232_Query_Adjust();      // 校准后返回校准值
        			break;
    		}
    	}
    	else
    	{
    		return;
    	}
    	if (bAnswer)
    	{
    		vMSRS232_SetFrameData();  //调用DMA发送数据
    	}
    }
    else
    {
        return;
    }
}

/**
* @Description  ModeBus-RTU 解析协议
* @Input        None
* @Output       None
* @Return       None
* @Others       None
*/
const u16 g_16_BIT[16] = {0x0001,0x0002,0x0004,0x0008,
                         0x0010,0x0020,0x0040,0x0080,
                         0x0100,0x0200,0x0400,0x0800,
                         0x1000,0x2000,0x4000,0x8000,};
void vModeBusRtu_Analysis(void)
{
	BOOL bAnswer = TRUE;
	u8 CtrlType = rtu_RxPack.RR_Func;    // 读取 or 写入
	u8 i;
//	u8 CmdType = sg_RxPack.Cmd;              // 数据种类 系数 数据 参数
//	u8 *pData = &sg_RxPack.Data[8];

    if( rtu_RxPack.RR_Addr == ADDR_ALL || rtu_RxPack.RR_Addr == g_tParam.CtrlParam.Addr485\
      || rtu_RxPack.RR_Addr == ADDR_PC )    // 仪表地址 判断地址
    {
    	rtu_TxPack.RTU_Addr = rtu_RxPack.RR_Addr;
    	rtu_TxPack.RTU_Func = rtu_RxPack.RR_Func;
        switch(CtrlType)
        {
            case 0x01:
                rtu_TxPack.RTU_DataNum = 0x01;
                rtu_TxPack.RTU_Data = OutStatus_Disp;
                for(i = 0;i<4;i++)  //增加数量选择
                {
                    if(i<rtu_RxPack.RR_DsNum)
                    {
                        rtu_TxPack.RTU_Data |= g_16_BIT[i];
                    }
                    else
                    {
                        rtu_TxPack.RTU_Data &=~g_16_BIT[i];
                    }
                }
                //vModeBus_Query_Relay();
                break;
            case 0x02:
                rtu_TxPack.RTU_DataNum = 0x01;
                rtu_TxPack.RTU_Data = DinStatus_Disp;
                for(i = 0;i<12;i++)  //增加数量选择
                {
                    if(i<rtu_RxPack.RR_DsNum)
                    {
                        rtu_TxPack.RTU_Data |= g_16_BIT[i];
                    }
                    else
                    {
                        rtu_TxPack.RTU_Data &=~g_16_BIT[i];
                    }
                }
                break;
            case 0x03:
                vModeBus_Query_Primary();
                rtu_TxPack.RTU_DataNum = rtu_RxPack.RR_DsNum * 4;
                memcpy((u8 *)&rtu_TxPack.Data[0x03], (u8 *)&RtuPrimaryData[rtu_RxPack.RR_DsAddr], rtu_TxPack.RTU_DataNum);
                break;
            case 0x04:
                if((rtu_RxPack.RR_DsAddr > 0x0100) && (rtu_RxPack.RR_DsAddr < 0x0200))
                {
                    vModeBus_Query_Secondary();
                    if((rtu_RxPack.RR_DsAddr > 0x0121) && (rtu_RxPack.RR_DsAddr < 0x0130))
                    {
                        rtu_TxPack.RTU_DataNum = rtu_RxPack.RR_DsNum * 4;
                        memcpy((u8 *)&rtu_TxPack.Data[0x03], (u8 *)&RtuSecondaryData[rtu_RxPack.RR_DsAddr - 0x0100], rtu_TxPack.RTU_DataNum);
                    }
                    else
                    {
                        rtu_TxPack.RTU_DataNum = rtu_RxPack.RR_DsNum * 2;
                        memcpy((u8 *)&rtu_TxPack.Data[0x03], (u8 *)&RtuSecondaryData[rtu_RxPack.RR_DsAddr - 0x0100], rtu_TxPack.RTU_DataNum);
                    }
                }
                else if((rtu_RxPack.RR_DsAddr >= 0x0400) && (rtu_RxPack.RR_DsAddr < 0x04A0))  // 谐波数据
                {
                    rtu_TxPack.RTU_DataNum = rtu_RxPack.RR_DsNum * 2;
                    vModeBus_Query_THD();  // 谐波数据刷新
                    memcpy((u8 *)&rtu_TxPack.Data[0x03], (u8 *)&RtuTHDData[rtu_RxPack.RR_DsAddr - 0x0400], rtu_TxPack.RTU_DataNum);
                }
                else if((rtu_RxPack.RR_DsAddr >= 0x0800) && (rtu_RxPack.RR_DsAddr < 0x0840))
                {
                    rtu_TxPack.RTU_DataNum = rtu_RxPack.RR_DsNum * 2;
                    vModeBus_Query_CTRL();
                    memcpy((u8 *)&rtu_TxPack.Data[0x03], (u8 *)&RtuCTRLData[rtu_RxPack.RR_DsAddr - 0x0800], rtu_TxPack.RTU_DataNum);
                }
                else
                {
                    bAnswer = FALSE;
                }
                break;
            case 0x05:
                rtu_TxPack.RTU_DsAddr = rtu_RxPack.RR_DsAddr;
                rtu_TxPack.RTU_DsData = rtu_RxPack.RR_DsNum;
                rtu_TxPack.RTU_DataNum = 6;
                vModeBus_Set_Relay();
                break;
            case 0x0F:
                rtu_TxPack.RTU_DsAddr = rtu_RxPack.RR_DsAddr;
                rtu_TxPack.RTU_DsNum = rtu_RxPack.RR_DsNum;
                rtu_TxPack.RTU_DataNum = 6;
                vModeBus_Set_Multi_Relay();
                break;
            case 0x10:
                rtu_TxPack.RTU_DsAddr = rtu_RxPack.RR_DsAddr;
                rtu_TxPack.RTU_DsNum = rtu_RxPack.RR_DsNum;
                rtu_TxPack.RTU_DataNum = 6;
                vModeBus_Set_Ctrl_Parm();
                break;
            default:
                break;
        }
    	if (bAnswer)
    	{
    		vModeBus_SetFrameData();  //调用DMA发送数据
    	}
    }
    else
    {
        return;
    }
}

/****************************************************************************************
** 函数名称: vMSRxProtocol
** 功能描述: 串口通信解析
** 参    数: None
** 返 回 值: BOOL
** 作　  者:
** 日  　期: 2012年08月06日
**---------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**--------------------------------------------------------------------------------------
****************************************************************************************/
BOOL vMSRS232_RxProtocol(void)
{
	static u32  s_TimeWhen;
	BOOL bReturn = FALSE;
    if(g_tParam.CtrlParam.DataProtocol == calib)
    {
    	switch (sg_MSPTLFlag)
    	{
        	case MSPTL_HEAD:
                if (USART_GetRxString((u8 *)&sg_RxPack.Data[0], 1))
        		{
        			if (sg_RxPack.Data[0] == 0x68)
        			{
        				sg_MSPTLFlag = MSPTL_ADDR;
                        /* 获取当前的系统时间 */
                        s_TimeWhen = PT_TimerGet();
        			}
        		}
        		break;
        	case MSPTL_ADDR:
                if (USART_GetRxString((u8 *)&sg_RxPack.Data[1], 3))
        		{
        			if (sg_RxPack.Data[3] == 0x68)
        			{
        				sg_RxPack.Addr1 = sg_RxPack.Data[1];
        				sg_RxPack.Addr2 = sg_RxPack.Data[2];
        				sg_MSPTLFlag = MSPTL_LENGTH;
        				s_TimeWhen = PT_TimerGet();
        			}
        			else
        			{
        				sg_MSPTLFlag = MSPTL_HEAD;
        			}
        		}
        		break;
        	case MSPTL_LENGTH:
                if (USART_GetRxString((u8 *)&sg_RxPack.Data[4], 3))
        		{
        			sg_RxPack.CtrlCode = sg_RxPack.Data[4];
        			sg_RxPack.Len = FLIPW(&sg_RxPack.Data[5]);  // Len
                    s_TimeWhen = PT_TimerGet();
        			sg_MSPTLFlag = MSPTL_TAIL;
        		}
        		break;
        	case MSPTL_TAIL:
        		if (USART_GetRxString((u8 *)&sg_RxPack.Data[7], sg_RxPack.Len + 2))
        		{
        			if (sg_RxPack.Data[sg_RxPack.Len + 8] == 0x16)  // 除数据外另有8位
        			{
        				sg_RxPack.Cmd = sg_RxPack.Data[7];
        				if (MSRS232_CRC(&sg_RxPack.Data[0], sg_RxPack.Len + 7) == sg_RxPack.Data[sg_RxPack.Len + 7])
        				{
        					vMSRS232_Analysis();
        				}
        			}
        			Task_RS232_init();
        			sg_MSPTLFlag = MSPTL_HEAD;
        		}
        		break;
        	default:
        		sg_MSPTLFlag = MSPTL_HEAD;
        		break;
    	}
    }
    /*------ ModBus-RTU ------*/
    else if(g_tParam.CtrlParam.DataProtocol == rtu485)
    {
    	switch (sg_RtuFlag)
    	{
        	case RTU_ADDR:
                if (USART_GetRxString((u8 *)&rtu_RxPack.Data[0], 1))
        		{
        			if (rtu_RxPack.Data[0] <= 0xff)
        			{
        				rtu_RxPack.RR_Addr = rtu_RxPack.Data[0];
        				sg_RtuFlag = RTU_FUNC;
                        /* 获取当前的系统时间 */
                        s_TimeWhen = PT_TimerGet();
        			}
        		}
        		break;
        	case RTU_FUNC:
                if (USART_GetRxString((u8 *)&rtu_RxPack.Data[1], 1))
        		{
        		    if (rtu_RxPack.Data[1] <= 0x10)
        			{
                        rtu_RxPack.RR_Func = rtu_RxPack.Data[1];
        				sg_RtuFlag = RTU_DSADDR;
        				s_TimeWhen = PT_TimerGet();
        		    }
        		}
        		break;
        	case RTU_DSADDR:
                if (USART_GetRxString((u8 *)&rtu_RxPack.Data[2], 2))
        		{
        			rtu_RxPack.RR_DsAddr = FHIPW(&rtu_RxPack.Data[2]);  // 起始地址
        			sg_RtuFlag = RTU_CRC;
        			s_TimeWhen = PT_TimerGet();
        		}
        		break;
        	case RTU_CRC:
        	    switch(rtu_RxPack.RR_Func)
    		    {
                    case 0x01:
        		    case 0x02:
        			case 0x03:
        		    case 0x04:
        		    case 0x05:
                		rtu_RxPack.RR_DataLen = 2;
                        if (USART_GetRxString((u8 *)&rtu_RxPack.Data[4], rtu_RxPack.RR_DataLen + 2))
                		{
                            rtu_RxPack.RR_CRC = FHIPW(&rtu_RxPack.Data[6]);
            				if (usMBCRC16(&rtu_RxPack.Data[0], rtu_RxPack.RR_DataLen + 4) == rtu_RxPack.RR_CRC)
            				{
            				    rtu_RxPack.RR_DsNum = FHIPW(&rtu_RxPack.Data[4]);
            					vModeBusRtu_Analysis();
            				}
                			Task_RS232_init();
                			sg_RtuFlag = RTU_ADDR;
                		}
        				break;
        		    case 0x0F:
                        rtu_RxPack.RR_DataLen = 4;
                        if (USART_GetRxString((u8 *)&rtu_RxPack.Data[4], rtu_RxPack.RR_DataLen + 2))
                		{
                            rtu_RxPack.RR_CRC = FHIPW(&rtu_RxPack.Data[8]);
            				if (usMBCRC16(&rtu_RxPack.Data[0], rtu_RxPack.RR_DataLen + 4) == rtu_RxPack.RR_CRC)
            				{
            				    rtu_RxPack.RR_DsNum = FHIPW(&rtu_RxPack.Data[4]);
            				    rtu_RxPack.RR_DataNum = rtu_RxPack.Data[6];
            				    rtu_RxPack.RR_Data = rtu_RxPack.Data[7];
            					vModeBusRtu_Analysis();
            				}
                			Task_RS232_init();
                			sg_RtuFlag = RTU_ADDR;
                		}
        				break;
        		    case 0x10:
                        if (USART_GetRxString((u8 *)&rtu_RxPack.Data[4],3))
                		{
                		    rtu_RxPack.RR_DsNum = FHIPW(&rtu_RxPack.Data[4]);
                            rtu_RxPack.RR_DataNum = rtu_RxPack.Data[6];
                            rtu_RxPack.RR_DataLen = rtu_RxPack.RR_DsNum * rtu_RxPack.RR_DataNum;
                            if (USART_GetRxString((u8 *)&rtu_RxPack.Data[7], rtu_RxPack.RR_DataLen + 2))
                    		{
                                rtu_RxPack.RR_CRC = FHIPW(&rtu_RxPack.Data[rtu_RxPack.RR_DataLen + 7]);
                				if (usMBCRC16(&rtu_RxPack.Data[0], rtu_RxPack.RR_DataLen + 7) == rtu_RxPack.RR_CRC)
                				{
                					vModeBusRtu_Analysis();
                				}
                    			Task_RS232_init();
                    			sg_RtuFlag = RTU_ADDR;
                    		}
                		}
        				break;
        		    default:
        		        sg_RtuFlag = RTU_ADDR;
    		    }

        		break;
        	default:
        		sg_RtuFlag = RTU_ADDR;
        		break;
    	}
    }
    if (sg_RtuFlag != RTU_ADDR) // 已有一段报文正确
	{
		if (PT_TimerDiff(s_TimeWhen) > 800) // 超过0.8s没有响应就认为上次发的数据无效
        {
			Task_RS232_init();
			sg_RtuFlag = RTU_ADDR;
			bReturn = FALSE;
		}
	}

	return bReturn;
}

//===========================================================================
//  End..
//===========================================================================
