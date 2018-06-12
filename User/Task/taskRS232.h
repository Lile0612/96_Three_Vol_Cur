/*! @file
********************************************************************************
<PRE>
模块名       : 485通讯处理任务模块
文件名       : taskRS232.h
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


#ifndef _TASKRS232_H
#define _TASKRS232_H


/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
#define ADDR_ALL                0xEE
#define ADDR_PC                 0xEF
#define RTU_ADDR_ALL            0xFA
#define RTU_ADDR_PC             0xFE
#define RX_SUBCOM_LEN           64
#define TX_SUBCOM_LEN           128

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/*------ 校表 ------*/
typedef struct
{
    u8 Start1;               // 帧起始符
    u8 Addr1;                // 发送方地址
    u8 Addr2;                // 接收方地址
    u8 Start2;               // 帧起始符
    u8 CtrlCode;             // 控制码
    u16 Len;                 // 数据长度
    u8 Cmd;                  // 命令码
    u8 Data[TX_SUBCOM_LEN];  // 数据域
    u8 CS;                   // 校验
}MSSendPackStruct;

// 接收数据结构
typedef struct
{
    u8 Start1;               // 帧起始符
    u8 Addr1;                // 发送方地址
    u8 Addr2;                // 接收方地址
    u8 Start2;               // 帧起始符
    u8 CtrlCode;             // 控制码
    u16 Len;                 // 数据长度
    u8 Cmd;                  // 命令码
    u8 Data[RX_SUBCOM_LEN];  // 数据地址
    u8 byCS;                 // 校验
}MSRecvPackStruct;

/*------ rtu ------*/
typedef struct
{
    u8 RTU_Addr;                 // 仪表地址
    u8 RTU_Func;                 // 功能码
    u16 RTU_DsAddr;              // 起始地址
    u16 RTU_DsData;               // 字节长度 动作值
    u16 RTU_DsNum;
    u8 RTU_DataNum;              //
    u8 RTU_DataLen;              //
    u8 RTU_Data;
    u8 Data[TX_SUBCOM_LEN];     // 数据域
    u16 RtuCRC;                 // 校验
}RtuSendPackStruct;

// 接收数据结构
typedef struct
{
    u8 RR_Addr;                 // 仪表地址
    u8 RR_Func;                 // 功能码
    u16 RR_DsAddr;              // 起始地址
    u16 RR_DsNum;               // 字节长度 动作值
    u8 RR_DataNum;              //
    u8 RR_DataLen;              //
    u8 RR_Data;
    u8 Data[RX_SUBCOM_LEN];  // 数据地址
    u16 RR_CRC;                 // 校验
}RtuRecvPackStruct;

typedef struct
{
    u8 Cmd;
    u8 Phase;
    u8 Data[64];
}AdjustMsgStruct;


// 告警编码
__packed struct  MSPTLALARM_BITS { // bit  description
	u8 OUA:1;			// 0	过压告警,0:正常,1:越限
	u8 LIP:1;			// 1	欠流越限,0:正常,1:越限
	u8 OUP:1;			// 2	过压越限
	u8 LUP:1;			// 3    欠压越限

	u8 THDIP:1;			// 4    电流谐波越限
	u8 TEMP:1;			// 5    温度越限
	u8 NUMP:1;			// 6	投切次数告警
	u8 LP:1;			// 7	缺相

	u8 AKGFAULT:1;		// 8    A组开关告警
	u8 BKGFAULT:1;		// 8    B组开关告警
	u8 CKGFAULT:1;		// 8    C组开关告警
	u8 DKGFAULT:1;		// 8    D组开关告警

	u8 rsvd:4;			// 15 - 12  保留
};


__packed union MSPTLALARM_REG
{
	u16					all;
	struct MSPTLALARM_BITS	bit;
};

// 从机参数(07H)
__packed typedef struct {
// 	u8 UpperPF;								// 功率因数上限	1字节	1.0-0.98C
	u8 LowerPF;								// 功率因数下限	1字节	0.80-0.99L
	u8 TempVal;								// 温度限值	1字节	20~70
	u8 THDIVal;								// 谐波限值	1字节	20~60
	u16 WarnShield;							// 告警屏蔽	2字节	0~FFFH
	u16 OUAVal;								// 过压告警限值	2字节	共补：4000~4200对应电压400~420(V)默认406V
											//						分补：2310~2410对应电压231~241(V)默认236V
	u16 OUPVal;								// 严重过压限值	2字节	共补：4200~4600对应电压420~460(V) 默认437V
											//						分补：2420~2530对应电压231~241(V)默认247V
	u16 LIPVal;								// 欠电流闭锁限值	2字节	20~100对应电流0.2~1(A)
}MSPTL_SubParamUpStruct;

__packed typedef struct {
// 	u8 UpperPF;								// 功率因数上限	1字节	1.0-0.98C
	u8 LowerPF;								// 功率因数下限	1字节	0.80-0.99L
	u8 TempVal;								// 温度限值	1字节	20~70
	u8 THDIVal;								// 谐波限值	1字节	20~60
	u16 OUAVal;								// 过压告警限值	2字节	共补：4000~4200对应电压400~420(V)默认406V
											//						分补：2310~2410对应电压231~241(V)默认236V
	u16 OUPVal;								// 严重过压限值	2字节	共补：4200~4600对应电压420~460(V) 默认437V
											//						分补：2420~2530对应电压231~241(V)默认247V
	u16 LIPVal;								// 欠电流闭锁限值	2字节	20~100对应电流0.2~1(A)
	u16 WarnShield;							// 告警屏蔽	2字节	0~FFFH
}MSPTL_SubParamDownStruct;

// 采样信息（08H）
__packed typedef struct {
	u16 U2;									// 采样电压	2字节	/10之后为实际电压值(V)
	u32 rsvd1;								//
	u16 I2;									// 采样电流	2字节	/100之后为实际电流值(A)
	u32 rsvd2;								//
	u16 P2;									// 采样有功	2字节	实际有功值(W)
	u32 rsvd3;								//
	u16 Q2;									// 采样无功	2字节	实际无功值(Var)
	u32 rsvd4;								//
	u16 THDI;								// 采样谐波	2字节	谐波值畸变率
	u32 rsvd5;								//
}MSPTL_SampleInfoStruct;

// 从机序号列表（获取用）（09H）

// 程序版本号（10H）


/* Exported functions ------------------------------------------------------- */
void Task_RS232_init(void);
void Task_RS232_Function(void);


#endif /* _TASKRS232_H */

//===========================================================================
// No more.
//===========================================================================


