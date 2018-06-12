/* Includes ------------------------------------------------------------------*/

#include "Include.h"

//RN8302

static u8 Rn8302_RX_Buf[10];
static u8 Rn8302_TX_Buf[10];
static ErrorStatus RN8302_TX_Frame(u8 num);
static ErrorStatus RN8302_RX_Frame(u8 num);
static u8 RN8302WR(RegStructure *dat);
static u8 RN8302RD(RegStructure *dat);
static void RN8302Init(void);
u32 CheckSum;
POWER2_ValStruct vg_Power_Val;
POWER22_ValStruct vg_Power2_Val;
POWER2_Read_ValStruct vg_Read_Val;

//struct RN_AnalogData g_AnalogData;
/******************************************************************************

                         RN8209全波计量参数定义及初始化

  ****************************************************************************/

All_REG_Data AllReg_data =
{
    /*校准参数和计量控制寄存器*/
    .SYSCON =   {0x00, 2, RW, 0},
    .EMUCON =   {0x01, 2, RW, 0},
    .HFConst =  {0x02, 2, RW, 0},
    .PStart =   {0x03, 2, RW, 0},
    .DStart =   {0x04, 2, RW, 0},
    .GPQA =     {0x05, 2, RW, 0},
    .GPQB =     {0x06, 2, RW, 0},
    .PhsA =     {0x07, 1, RW, 0},
    .PhsB =     {0x08, 1, RW, 0},
    .QPhsCal =  {0x09, 2, RW, 0},
    .APOSA =    {0x0A, 2, RW, 0},
    .APOSB =    {0x0B, 2, RW, 0},
    .RPOSA =    {0x0C, 2, RW, 0},
    .RPOSB =    {0x0D, 2, RW, 0},
    .IARMSOS =  {0x0E, 2, RW, 0},
    .IBRMSOS =  {0x0F, 2, RW, 0},
    .IBGain =   {0x10, 2, RW, 0},
    .D2FPL =    {0x11, 2, RW, 0},
    .D2FPH =    {0x12, 2, RW, 0},
    .DCIAH =    {0x13, 2, RW, 0},
    .DCIBH =    {0x14, 2, RW, 0},
    .DCUH =     {0x15, 2, RW, 0},
    .DCL =      {0x16, 2, RW, 0},
    .EMUCON2 =  {0x17, 2, RW, 0},
    /*计量参数和状态寄存器*/
    .PFCnt =    {0x20, 2, RW, 0},
    .DFcnt =    {0x21, 2, RW, 0},
    .IARMS =    {0x22, 3, RO, 0},
    .IBRMS =    {0x23, 3, RO, 0},
    .URMS =     {0x24, 3, RO, 0},
    .UFreq =    {0x25, 2, RO, 0},
    .PowerPA =  {0x26, 4, RO, 0},
    .PowerPB =  {0x27, 4, RO, 0},
    .PowerQ =   {0x28, 4, RO, 0},
    .EnergyP =  {0x29, 3, RO, 0},
    .EnergyP2 = {0x2A, 3, RO, 0},
    .EnergyD =  {0x2B, 3, RO, 0},
    .EnergyD2 = {0x2C, 3, RO, 0},
    .EMUStatus= {0x2D, 3, RO, 0},
    .SPL_IA =   {0x30, 3, RO, 0},
    .SPL_IB =   {0x31, 3, RO, 0},
    .SPL_U =    {0x32, 3, RO, 0},
    .UFreq2 =   {0x35, 3, RO, 0},
    /*中断寄存器*/
    .IE =       {0x40, 1, RO, 0},
    .IF =       {0x41, 1, RO, 0},
    .RIF =      {0x42, 1, RO, 0},
    /*系统状态寄存器*/
    .SysStatus= {0x43, 1, RO, 0},
    .RData =    {0x44, 4, RO, 0},
    .WData =    {0x45, 2, RO, 0},
    .DeviceID = {0x7F, 3, RO, 0},
    /*特殊命令*/
    .WREN =     {0xEA, 1, RW, 0},
    .WPRO =     {0xEA, 1, RW, 0},
    .IAChoice = {0xEA, 1, RW, 0},
    .IBChoice = {0xEA, 1, RW, 0},
    .SOFT_RST = {0xEA, 1, RW, 0},
};

/******************************************************************************
  * *   函数名称: sqrt_64fixed(u64 a)

  * *   函数说明: 求算数平方根

  * *   输入参数: a 数值

  * *   输出参数: 无

  * *   返回值:  无
  ****************************************************************************/
u32 sqrt_64fixed(u64 a)
{
    u64 i, c;
    u64 b = 0;
    for(i = 0x4000000000000000; i != 0; i >>= 2)
    {
        c = i + b;
        b >>= 1;
        if(c <= a)
        {
            a -= c;
            b += i;
        }
    }
    return (u32)b;
}

void bsp_InitRN8023(void)
{
    RN8302Init();
}

/******************************************************************************
  * *   函数名称: RN8302WR(RegStructure* dat)

  * *   函数说明: 向RN8302指定的寄存器中写数据

  * *   输入参数: RegStructure* dat，指定的寄存器结构指针

  * *   输出参数: 无

  * *   返回值:  无
  ****************************************************************************/
static u8 RN8302WR(RegStructure *dat)
{
    u8 data1, data2, data3;
    u8 check;

    if(dat->rw == RO)return ERROR;
    switch(dat->bytenum)
    {
    case 1:
        data1 = dat->data & 0xff;
        Rn8302_TX_Buf[0] = 0x80 | dat->addr;
        Rn8302_TX_Buf[1] = data1;
        check = RN8302_TX_Frame(2);
        break;
    case 2:
        data1 = (dat->data >> 8) & 0xff;
        data2 = dat->data & 0xff;
        Rn8302_TX_Buf[0] = 0x80 | dat->addr;
        Rn8302_TX_Buf[1] = data1;
        Rn8302_TX_Buf[2] = data2;
        check = RN8302_TX_Frame(3);
        break;
    case 3:
        data1 = (dat->data >> 16) & 0xff;
        data2 = (dat->data >> 8) & 0xff;
        data3 = dat->data & 0xff;
        Rn8302_TX_Buf[0] = 0x80 | dat->addr;
        Rn8302_TX_Buf[1] = data1;
        Rn8302_TX_Buf[2] = data2;
        Rn8302_TX_Buf[3] = data3;
        check = RN8302_TX_Frame(4);
        break;
    default:
        break;
    }
    return check;
}

/******************************************************************************

  * *   函数名称: RN8302RD(RegStructure* dat)

  * *   函数说明: 从RN8302指定的寄存器中读取数据

  * *   输入参数: RegStructure* dat，指定的寄存器结构指针

  * *   输出参数: 无

  * *   返回值:   校验结果，正确返回1，错误返回0

  ****************************************************************************/
static u8 RN8302RD(RegStructure *dat)
{
    u8 check = 0;
    if(dat->rw != RO && dat->rw != RW)return 0;
    switch(dat->bytenum)
    {
    case 1:
        Rn8302_TX_Buf[0] = dat->addr;
        check = RN8302_RX_Frame(1);
        if(check) dat->data = Rn8302_RX_Buf[0];
        break;
    case 2:
        Rn8302_TX_Buf[0] = dat->addr;
        check = RN8302_RX_Frame(2);
        if(check) dat->data = Rn8302_RX_Buf[0] << 8 | Rn8302_RX_Buf[1];
        break;
    case 3:
        Rn8302_TX_Buf[0] = dat->addr;
        check = RN8302_RX_Frame(3);
        if(check) dat->data = Rn8302_RX_Buf[0] << 16 | Rn8302_RX_Buf[1] << 8 | Rn8302_RX_Buf[2];
        break;
    case 4:
        Rn8302_TX_Buf[0] = dat->addr;
        check = RN8302_RX_Frame(4);
        if(check) dat->data = Rn8302_RX_Buf[0] << 24 | Rn8302_RX_Buf[1] << 16 | Rn8302_RX_Buf[2] << 8 | Rn8302_RX_Buf[3];
        break;
    default:
        break;
    }
    return check;
}



void spi_nop(void)
{
    u8 i = 80; //这里可以优化速度
    while(i)
    {
        i--;
    }
}

/****************************************************************************************
** 函数名称: Spi_RN8302_SendByte
** 功能描述: 向RN8302B发送一个字节
** 参    数: u8 byData---发送字节
** 返 回 值: TRUE---成功, FALSE---失败
** 作　  者:
** 日  　期: 2010年10月8日
**---------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**--------------------------------------------------------------------------------------
****************************************************************************************/
u8 Spi_RN8302_SendByte(u8 byData)
{
    u8 i;
    for (i = 0; i < 8; i++)     // Setup byte circulation bits
    {
        if (byData & 0x80)              // Put DATA_BUF.7 on data line
            RN_SPI_MOSI_H;
        else
            RN_SPI_MOSI_L;
        RN_SPI_SCK_H;                   // Set clock line high
        byData <<= 1;                       // Shift DATA_BUF
        spi_nop();
        RN_SPI_SCK_L;                   // Set clock line low
        spi_nop();
    }
    return TRUE;
}

/****************************************************************************************
** 函数名称: Spi_RN8302_ReadByte
** 功能描述: 从RN8302B接收一个字节
** 参    数:
** 返 回 值: 读取的一个字节值, 0xff为无效值
** 作　  者:
** 日  　期: 2010年10月8日
**---------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**--------------------------------------------------------------------------------------
****************************************************************************************/
u8 Spi_RN8302_ReadByte(void)
{
    u8 i;
    u8 byte;
    byte = 0;
    for (i = 0; i < 8; i++)     // Setup byte circulation bits
    {
        byte <<= 1; // Right shift DATA_BUF
        RN_SPI_SCK_H;                   // Set clock line high
        if (RN_SPI_MISO_IN)
            byte |= 0x01;           // Read data
        spi_nop();

        RN_SPI_SCK_L;                   // Set clock line low
        spi_nop();
    }
    return byte;            // Return function parameter
}

/******************************************************************************

  * *   函数名称: RN8302_TX_Frame(uchar num)

  * *   函数说明: 向8302中发送一帧数据，Rn8302_TX_Buf发送缓冲区

  * *   输入参数: num 帧字节数

  * *   输出参数: 无

  * *   返回值:   无

  ****************************************************************************/
static ErrorStatus RN8302_TX_Frame(u8 num)
{
    u8 i, Repeat, temp;
    //u8 checksum = 0;
    u8 *p;
    ErrorStatus  err = SUCCESS;

    RN_SPI_CSN_L;
    for(Repeat = 3; Repeat > 0; Repeat--)
    {
        p = Rn8302_TX_Buf;
        for(i = 0; i < num; i++)
        {
            Spi_RN8302_SendByte(*p);
            p++;
        }
        __nop();
        //读WData寄存器检查-----------------------
        Spi_RN8302_SendByte(0x45);
        for(i = 2 ; i > 0 ; i--)
        {
            temp = Spi_RN8302_ReadByte();
            if((num - 1) >= i)
            {
                if(temp != Rn8302_TX_Buf[num - i])
                {
                    err = ERROR;
                    break;
                }
            }
        }
        if(err == SUCCESS)
        {
            if(num - 1 == 3)AllReg_data.WData.data =  Rn8302_TX_Buf[1] << 16 | Rn8302_TX_Buf[2] << 8 | Rn8302_TX_Buf[3
];
            else if(num - 1 == 2)AllReg_data.WData.data =  Rn8302_TX_Buf[1] << 8 | Rn8302_TX_Buf[2];
            else if(num - 1 == 1)AllReg_data.WData.data =  Rn8302_TX_Buf[1];
            break;
        }
    }
    RN_SPI_CSN_H;
    return(err);
}


/******************************************************************************

  * *   函数名称: RN8302_RX_Frame(uchar num)

  * *   函数说明: 从8302中读取一帧数据，Rn8302_RX_Buf接收缓冲区

  * *   输入参数: num 帧字节数

  * *   输出参数: 无

  * *   返回值:   校验结果，正确返回1，错误返回0

  ****************************************************************************/
static ErrorStatus RN8302_RX_Frame(u8 num)
{
    u8 i, Repeat, temp;
//  u8 checksum = 0;
    u8 *p;
    ErrorStatus err = SUCCESS;

    RN_SPI_CSN_L;
    for(Repeat = 3; Repeat > 0; Repeat--)
    {
        p = Rn8302_RX_Buf;
        Spi_RN8302_SendByte(Rn8302_TX_Buf[0]);
        for(i = 0; i < num; i++)
        {
            *p = Spi_RN8302_ReadByte();
            p++;
        }
        //---读RData寄存器检查------------------------------------
        Spi_RN8302_SendByte(0x44);
        for(i = 4 ; i > 0 ; i--)
        {
            temp = Spi_RN8302_ReadByte();
            if(num >= i)
            {
                if(temp != Rn8302_RX_Buf[num - i])
                {
                    err = ERROR;
                    break;
                }
            }
        }
        if(err == SUCCESS)
        {
            if(num == 4)AllReg_data.RData.data =  Rn8302_RX_Buf[0] << 24 | Rn8302_RX_Buf[1] << 16 | Rn8302_RX_Buf[2] << 8 | Rn8302_RX_Buf[3];
            else if(num == 3)AllReg_data.RData.data =  Rn8302_RX_Buf[0] << 16 | Rn8302_RX_Buf[1] << 8 | Rn8302_RX_Buf[2];
            else if(num == 2)AllReg_data.RData.data =  Rn8302_RX_Buf[0] << 8 | Rn8302_RX_Buf[1];
            else if(num == 1)AllReg_data.RData.data =  Rn8302_RX_Buf[0];
            break;
        }

    }
    RN_SPI_CSN_H;
    return(err);
}


/******************************************************************************

  * *   函数名称: RN8302Init(void)

  * *   函数说明: RN8302初始化函数，恢复增益、相位校正系数，表常数等

  * *   输入参数: 无

  * *   输出参数: 无

  * *   返回值:   无

  ****************************************************************************/
void RN8302Init(void)
{
    int i;
    // 读芯片ID
    RN8302RD(&AllReg_data.DeviceID);
    if (AllReg_data.DeviceID.data != 0x820900)
    {
        return;
    }

    // 写使能
    AllReg_data.WREN.data = WRENABLE;
    RN8302WR(&AllReg_data.WREN);

    // 软件复位，会清除写使能
    AllReg_data.SOFT_RST.data = SOFTRST;
    RN8302WR(&AllReg_data.SOFT_RST);

    for (i = 0; i < 50000; i++)
    {
        __ASM volatile ("nop");
    }

    // 写使能
    AllReg_data.WREN.data = WRENABLE;
    RN8302WR(&AllReg_data.WREN);

    // 系数恢复

    // ADC通道增益设置---------------------------Ib-------------U-------------Ia---
    AllReg_data.SYSCON.data = 0x01 << 6 | GAIN_1 << 4 | GAIN_1 << 2 | GAIN_1;
    RN8302WR(&AllReg_data.SYSCON);

    AllReg_data.EMUCON.data = 0x00;
    RN8302WR(&AllReg_data.EMUCON);

    //AllReg_data.HFConst.data = g_tParam.RN_Coefficients.HFConst;3592
    AllReg_data.HFConst.data = 3592;
    RN8302WR(&AllReg_data.HFConst);

    AllReg_data.PStart.data = 0x0060;
    RN8302WR(&AllReg_data.PStart);

    AllReg_data.DStart.data = 0x0120;
    RN8302WR(&AllReg_data.DStart);

    AllReg_data.GPQA.data = 0x0000;
    RN8302WR(&AllReg_data.GPQA);

    AllReg_data.GPQB.data = 0x0000;
    RN8302WR(&AllReg_data.GPQB);

    AllReg_data.PhsA.data = 0x0000;
    RN8302WR(&AllReg_data.PhsA);

    AllReg_data.PhsB.data = 0x0000;
    RN8302WR(&AllReg_data.PhsB);

    AllReg_data.QPhsCal.data = 0x0000;
    RN8302WR(&AllReg_data.QPhsCal);

    AllReg_data.APOSA.data = 0x0000;
    RN8302WR(&AllReg_data.APOSA);

    AllReg_data.APOSB.data = 0x0000;
    RN8302WR(&AllReg_data.APOSB);   // 有功offset校正

    AllReg_data.RPOSA.data = 0x0000;
    RN8302WR(&AllReg_data.RPOSA);

    AllReg_data.RPOSB.data = 0x0000;
    RN8302WR(&AllReg_data.RPOSB);

    AllReg_data.IARMSOS.data = 0x0000;
    RN8302WR(&AllReg_data.IARMSOS);

    AllReg_data.IBRMSOS.data = 0x0000;
    RN8302WR(&AllReg_data.IBRMSOS);     //有效值offset校正  小电流

    AllReg_data.IBGain.data = 0x0000;
    RN8302WR(&AllReg_data.IBGain);      //电流IB增益

    // 校验和寄存器1
    RN8302RD(&AllReg_data.EMUStatus);

    //写保护
    AllReg_data.WREN.data = WRDISABLE;
    RN8302WR(&AllReg_data.WREN);
}

#define RATIED_VAL_U  22000     /* 额定电压 0.01*/
#define RATIED_VAL_I  50000     /* 额定电流 0.0001*/
#define RATIED_VAL_P  (220*5*100)     /* 1.0时额定有功 0.01*/
#define RATIED_VAL_HALFP  (0.5*220*5*10)     /* 0.5L时额定有功 0.1*/
//#define STANDARD_VAL_U  20000000     /* 额定电压标准寄存器值 */
#define STANDARD_VAL_U  500000     /* 额定电压标准寄存器值 */
//#define STANDARD_VAL_I  70000000     /* 额定电流标准寄存器值 */
#define STANDARD_VAL_I  800000     /* 额定电流标准寄存器值 */
//#define STANDARD_VAL_P  166893005     /* 1.0L时额定有功标准寄存器值 20000000*70000000 /2^23*/
#define STANDARD_VAL_P  143051    /* 1.0L时额定有功标准寄存器值 20000000*70000000 /2^23*/
#define STANDARD_VAL_HALFP  (STANDARD_VAL_P/2)     /* 0.5L时额定有功标准寄存器值 */
#define POWER15 32768 /* 2^15 */
#define POWER16 65536 /* 2^16 */
#define POWER23 8388608 /* 2^23 */
#define POWER24 16777216 /* 2^24 */

/******************************************************************************

  * *   函数名称: Refresh_AnalogData()

  * *   函数说明: 读取RN8302中的数据，刷新数据

  * *   输入参数: 无

  * *   输出参数: 无

  * *   返回值:   无

  ****************************************************************************/
void RN8032_Updata(void)
{
    float ftemp,ftemp2;
#if(FUNCTION_CURRENT == 1)
    if(RN8302RD(&AllReg_data.IARMS))
    {
        ftemp = AllReg_data.IARMS.data * g_tParam.RN_Coefficients.Kram[0];
        ftemp2 =  ftemp * RATIED_VAL_I / STANDARD_VAL_I;       // ABC相电压有效值
        vg_Power_Val.Real2_Val[PH_A].I = ftemp2;
        vg_Power2_Val.Real22_Val[PH_A].I = (int)(ftemp2/10);
        vg_Read_Val.Real_Read_Val[PH_A].I = (u32)ftemp2;
        if (vg_Power_Val.Real2_Val[PH_A].I < 100)  
        {
            vg_Power_Val.Real2_Val[PH_A].I = 0;
            vg_Power2_Val.Real22_Val[PH_A].I = 0;
            vg_Read_Val.Real_Read_Val[PH_A].I = 0;
        }
        vg_Power_Val.Real2_Val[PH_A].I = vg_Power_Val.Real2_Val[PH_A].I * g_tParam.CtrlParam.CTNum;

        RtuPrimaryData[Index_1_Ia + PH_A] = vg_Power_Val.Real2_Val[PH_A].I/10000;
        RtuSecondaryData[Index_2_Ia + PH_A] = vg_Power2_Val.Real22_Val[PH_A].I;
    }
    if(RN8302RD(&AllReg_data.IBRMS))
    {
        ftemp = AllReg_data.IBRMS.data * g_tParam.RN_Coefficients.Kram[1];
        ftemp2 = ftemp * RATIED_VAL_I / STANDARD_VAL_I;         // ABC相电流有效值
        vg_Power_Val.Real2_Val[PH_B].I = ftemp2;
        vg_Power2_Val.Real22_Val[PH_B].I = (int)(ftemp2/10);
        vg_Read_Val.Real_Read_Val[PH_B].I = (u32)ftemp2;
        if (vg_Power_Val.Real2_Val[PH_B].I < 100)  
        {
            vg_Power_Val.Real2_Val[PH_B].I = 0;
            vg_Power2_Val.Real22_Val[PH_B].I = 0;
            vg_Read_Val.Real_Read_Val[PH_B].I = 0;
        }
        vg_Power_Val.Real2_Val[PH_B].I = vg_Power_Val.Real2_Val[PH_B].I * g_tParam.CtrlParam.CTNum;

        RtuPrimaryData[Index_1_Ia + PH_B] = vg_Power_Val.Real2_Val[PH_B].I/10000;
        RtuSecondaryData[Index_2_Ia + PH_B] = vg_Power2_Val.Real22_Val[PH_B].I;
    }
    if(RN8302RD(&AllReg_data.URMS))
    {
        ftemp = AllReg_data.URMS.data * g_tParam.RN_Coefficients.Kram[2];
        ftemp2 = ftemp * RATIED_VAL_I / STANDARD_VAL_I;         // ABC相电流有效值
        vg_Power_Val.Real2_Val[PH_C].I = ftemp2;
        vg_Power2_Val.Real22_Val[PH_C].I = (int)(ftemp2/10);
        vg_Read_Val.Real_Read_Val[PH_C].I = (u32)ftemp2;
        if (vg_Power_Val.Real2_Val[PH_C].I < 100)  
        {
            vg_Power_Val.Real2_Val[PH_C].I = 0;
            vg_Power2_Val.Real22_Val[PH_C].I = 0;
            vg_Read_Val.Real_Read_Val[PH_C].I = 0;
        }
        vg_Power_Val.Real2_Val[PH_C].I = vg_Power_Val.Real2_Val[PH_C].I * g_tParam.CtrlParam.CTNum;

        RtuPrimaryData[Index_1_Ia + PH_C] = vg_Power_Val.Real2_Val[PH_C].I/10000;
        RtuSecondaryData[Index_2_Ia + PH_C] = vg_Power2_Val.Real22_Val[PH_C].I;
    }
#endif
#if(FUNCTION_VOLTAGE == 1)
    if(RN8302RD(&AllReg_data.URMS))
    {
        ftemp = AllReg_data.URMS.data * g_tParam.RN_Coefficients.Kram[0];
        ftemp2 = ftemp * RATIED_VAL_U / STANDARD_VAL_U;       // ABC相电压有效值
        vg_Power_Val.Real2_Val[PH_A].U = ftemp2;
        vg_Power2_Val.Real22_Val[PH_A].U = (int)(ftemp2/10);
        vg_Read_Val.Real_Read_Val[PH_A].U = (int)ftemp2;
        if (vg_Power_Val.Real2_Val[PH_A].U < 100) // 1V    22000 = 220V
        {
            vg_Power_Val.Real2_Val[PH_A].U = 0;
            vg_Power2_Val.Real22_Val[PH_A].U = 0;
            vg_Read_Val.Real_Read_Val[PH_A].U = 0;
        }
        vg_Power_Val.Real2_Val[PH_A].U = vg_Power_Val.Real2_Val[PH_A].U * g_tParam.CtrlParam.PTNum;

        RtuPrimaryData[Index_1_Ua + PH_A] = vg_Power_Val.Real2_Val[PH_A].U/100;
        RtuSecondaryData[Index_2_Ua + PH_A] = vg_Power2_Val.Real22_Val[PH_A].U;
    }

    if(RN8302RD(&AllReg_data.IBRMS))
    {
        ftemp = AllReg_data.IBRMS.data * g_tParam.RN_Coefficients.Kram[1];
        ftemp2 = ftemp * RATIED_VAL_U / STANDARD_VAL_U;       // ABC相电压有效值
        vg_Power_Val.Real2_Val[PH_B].U = ftemp2;
        vg_Power2_Val.Real22_Val[PH_B].U = (int)(ftemp2/10);
        vg_Read_Val.Real_Read_Val[PH_B].U = (int)ftemp2;
        if (vg_Power_Val.Real2_Val[PH_B].U < 100) // 1V    22000 = 220V
        {
            vg_Power_Val.Real2_Val[PH_B].U = 0;
            vg_Power2_Val.Real22_Val[PH_B].U = 0;
            vg_Read_Val.Real_Read_Val[PH_B].U = 0;
        }
        vg_Power_Val.Real2_Val[PH_B].U = vg_Power_Val.Real2_Val[PH_B].U * g_tParam.CtrlParam.PTNum;

        RtuPrimaryData[Index_1_Ua + PH_B] = vg_Power_Val.Real2_Val[PH_B].U/100;
        RtuSecondaryData[Index_2_Ua + PH_B] = vg_Power2_Val.Real22_Val[PH_B].U;
    }

    if(RN8302RD(&AllReg_data.IARMS))
    {
        ftemp = AllReg_data.IARMS.data * g_tParam.RN_Coefficients.Kram[2];
        ftemp2 = ftemp * RATIED_VAL_U / STANDARD_VAL_U;       // ABC相电压有效值
        vg_Power_Val.Real2_Val[PH_C].U = ftemp2;
        vg_Power2_Val.Real22_Val[PH_C].U = (int)(ftemp2/10);
        vg_Read_Val.Real_Read_Val[PH_C].U = (int)ftemp2;
        if (vg_Power_Val.Real2_Val[PH_C].U < 100) // 1V    22000 = 220V
        {
            vg_Power_Val.Real2_Val[PH_C].U = 0;
            vg_Power2_Val.Real22_Val[PH_C].U = 0;
            vg_Read_Val.Real_Read_Val[PH_C].U = 0;
        }
        vg_Power_Val.Real2_Val[PH_C].U = vg_Power_Val.Real2_Val[PH_C].U * g_tParam.CtrlParam.PTNum;

        RtuPrimaryData[Index_1_Ua + PH_C] = vg_Power_Val.Real2_Val[PH_C].U/100;
        RtuSecondaryData[Index_2_Ua + PH_C] = vg_Power2_Val.Real22_Val[PH_C].U;
    }
#endif

    if(RN8302RD(&AllReg_data.UFreq)) // fosc*8/reg,保留两位小数,(8.192*10^6)*8
    {
        vg_Power_Val.Freq = 357954500/8/ AllReg_data.UFreq.data;  // 电压线频率
        if((vg_Power_Val.Real2_Val[PH_A].U == 0) && (vg_Power_Val.Real2_Val[PH_A].I == 0) && (vg_Power_Val.Real2_Val[PH_B].I == 0))
        {
            vg_Power_Val.Freq = 0;
            vg_Power2_Val.Freq = 0;
        }
    }
    RtuPrimaryData[Index_1_Hz] = vg_Power_Val.Freq/100;
    RtuSecondaryData[Index_2_Hz] = vg_Power2_Val.Freq;   
}

void Task_Power_Function(void)
{
    RN8032_Updata();
}

void InitRNParam(void)
{
    g_tParam.RN_Coefficients.Kram[0] = 1;
    g_tParam.RN_Coefficients.Kram[1] = 1;
    g_tParam.RN_Coefficients.Kram[2] = 1;
}
void RN8032_Adjust(u8 Cmd, u8 Phase, u8 *pAdjustData) // 校准函数
{
    u16 AoData[4];
    u8 Size;

    switch (Cmd)
    {
        case CALIB_AnoPwm:
            Size = sizeof(AoData);
            memcpy((u8 *)&AoData[0], pAdjustData, Size);
            g_tParam.RN_Coefficients.Ana_Pwm1_5h = (float)AoData[0]/10000;
            g_tParam.RN_Coefficients.Ana_Pwm1_2k = (float)AoData[1]/1000;
            g_tParam.RN_Coefficients.Ana_Pwm1_ki = (g_tParam.RN_Coefficients.Ana_Pwm1_2k - g_tParam.RN_Coefficients.Ana_Pwm1_5h)/3000;

            g_tParam.RN_Coefficients.Ana_Pwm2_5h = (float)AoData[2]/10000;
            g_tParam.RN_Coefficients.Ana_Pwm2_2k = (float)AoData[3]/1000;
            g_tParam.RN_Coefficients.Ana_Pwm2_ki = (g_tParam.RN_Coefficients.Ana_Pwm2_2k - g_tParam.RN_Coefficients.Ana_Pwm2_5h)/3000;
            g_tParam.CtrlParam.AnaCalibFlag = TRUE;
            break;
        default:
            break;
    }
    PT_TimerDelay(2000);
    SaveParam();
}
//===========================================================================
//===========================================================================



