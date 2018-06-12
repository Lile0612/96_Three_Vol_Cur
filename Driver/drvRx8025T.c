
/* Includes--------------------------------------------------------------*/
#include "Include.h"

#define SCL_1         PORT_EE_SCL->BSRR = PIN_EE_SCL
#define SCL_0         PORT_EE_SCL->BRR  = PIN_EE_SCL

#define SDA_1         PORT_EE_SDA->BSRR = PIN_EE_SDA
#define SDA_0         PORT_EE_SDA->BRR  = PIN_EE_SDA

#define SCL_read      PORT_EE_SDA->IDR & PIN_EE_SDA
#define SDA_read      PORT_EE_SDA->IDR & PIN_EE_SDA

void MyI2C_GPIO_OUT_Config(void);
void MyI2C_GPIO_IN_Config(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
u8   MyI2C_SendByte(u8 ByteData);
u8   MyI2C_ReceiveByte(u8 last_char);

//for 72mhz clk means t=12=>1us
void  delay_us(long t)
{
    long i;

    for(i=0;i<t;i++);
}

//配置SDA(PB11)推挽输出
void MyI2C_GPIO_Out_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin   =  PIN_EE_SDA;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(PORT_EE_SDA, &GPIO_InitStructure);
}

//配置SDA(PB11)上拉输入
void  MyI2C_GPIO_IN_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin   =  PIN_EE_SDA;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(PORT_EE_SDA, &GPIO_InitStructure);
}

//模拟起始信号
void MyI2C_Start(void)
{
  MyI2C_GPIO_Out_Config();
  SDA_1;
  SCL_1;
  delay_us(50);
  SDA_0;
  delay_us(50);
  SCL_0;
}

//模拟停止信号
void MyI2C_Stop(void)
{
  MyI2C_GPIO_Out_Config();
  SDA_0;                                       //I2CStop的条件是在SCL是低电平的情况下
  SCL_1;
  delay_us(50);
  SDA_1;
}

//I2C发送字节
u8 MyI2C_SendByte(u8 ByteData)              //I2C的基本传送方式是字节发送
{
  u8 ack;
  u8 i;
  MyI2C_GPIO_Out_Config();
  for (i = 0;  i < 8;  i++)                   //主器件通过SCL发出8个时钟脉冲将要发送的
  {                                           //数据通过SDA发出
    SCL_0;
    delay_us(50);
    if (ByteData&0x80)
      SDA_1;
    else
      SDA_0;
    delay_us(30);
    ByteData <<= 1;                           //数据建立时间200us
    SCL_1;
    delay_us(50);                            //数据保持时间没有要求
  }

  SCL_0;                                      //在第9个时钟时读取从件（ATC1024）发送的
                                              //ack应答信号，此时SDA应为输入状态，SD//为输入状态时，SDA是高电平
  delay_us(50);

  MyI2C_GPIO_IN_Config();                       //注意在设置SDA为输入时，一定要先使SCL

  SCL_1;                                      //是低电平，千万不能使SCL为高电平，因为在
  delay_us(50);                              //SCL是高电平的情况下，有可能出现SDA从低

  ack = SDA_read ;                            //到高的跳变（I2CStop状况）发送失败，
  SCL_0;

  return (ack);                               //返回ack信号，低电平表示有应答，高则没有
}


//I2C接收字节
u8 MyI2C_ReceiveByte(u8 last_char)
{
  u8 data=0, i;
  MyI2C_GPIO_IN_Config();                       //主器件通过8个时钟信号，接收从件发出的数据
  for (i = 0;  i < 8;  i++)
  {
    SCL_0;
    delay_us(50);
    SCL_1;
    delay_us(50);
    data <<=1;
    if (SDA_read ) data++;
  }
  SCL_0;
  delay_us(50);

  MyI2C_GPIO_Out_Config();
  if (last_char)                              //如果接收是最后的一个字节时，主器件在第9个周期
    SDA_1;                                    //给从件一个高电平(NACK)，否则给一个低电平(ACK)
  else                                       //表示继续接收
    SDA_0;

  delay_us(30);
  SCL_1;
  delay_us(50);
  SCL_0;
  return data;
}

// 时间寄存器定义
#define	RTC8025_Second        0  //秒寄存器
#define	RTC8025_Minute        1  //分寄存器
#define	RTC8025_Hour          2  //时寄存器
#define	RTC8025_Week          3  //星期寄存器
#define	RTC8025_Day           4  //日寄存器
#define	RTC8025_Month         5  //月寄存器
#define	RTC8025_Year          6  //年寄存器

// 控制寄存器定义(时钟芯片型号不相同，相应的配置也是不相同的)
#define	RTC8025T_Control1     (0x0D)  //控制1 寄存器 （R8025T）
#define	RTC8025_Control1      (0x0E)  //控制1 寄存器  (R8025AC)

#define	RTC8025_PON           (0x10)  // RTC电源失效标志位
#define	RTC8025_XST           (0x20)  // RTC内部晶振失效标志位

// 工作模式定义
#define	RTC8025_Standard_Read (0x00)  //标准读模式
#define	RTC8025_Simple_Read   (0x04)  //简单读模式

STDATETIME      stDateTime;
SPECIALFLAG     specialFlag;
DEVSTATE        devState;
u8              pubRam[32];

void Get8025( u8 addr, u8 *data, u8 counter);
void Set8025( u8 addr, u8 *data, u8 counter);
void Init8025(void);
void RtcSetDateTime(STDATETIME *pTime);
void RtcSetLocalTime(void);
void UpdateDateTime(void);


//BCD码转二位十进制
u8 BCD2DEC(u8 temp)
{
  temp = (temp >> 4) * 10 + (temp & 0x0f);
  return temp;
}

//二位十进制转BCD码
u8 DEC2BCD(u8 temp)
{
  temp = (temp / 10) * 16 + (temp % 10);
  return temp;
}

void Get8025( u8 addr, u8 *data,u8 counter)
{
    u8 i;
    MyI2C_Start();
    MyI2C_SendByte(0x64);
    MyI2C_SendByte(addr);
    MyI2C_Start();
    MyI2C_SendByte(0x65);
    for (i = 0;  i < counter - 1 ;  i++)
      *data++ = MyI2C_ReceiveByte(0);
    *data++ = MyI2C_ReceiveByte(1);
    MyI2C_Stop();
}

void Set8025( u8 addr, u8 *data,u8 counter)
{
   u8 i;
   MyI2C_Start();
   MyI2C_SendByte(0x64);
   MyI2C_SendByte(addr);
   for(i = 0; i <counter; i++)
     MyI2C_SendByte(*data++);
   MyI2C_Stop();
}

void Init8025(void)
{
    u8 da[3];
    da[0]=0x00;
    da[1]=0x00;         // 24小时模式设置,1Hz  频率输出
    da[2]=0x60;
    Set8025(RTC8025T_Control1,da,3);
    memset(pubRam,0,3);
    Get8025(RTC8025T_Control1,pubRam,3);

    if(pubRam[2] != da[2])
    {
      specialFlag.I2C8025F = 1;
    }
    else
    {
      specialFlag.I2C8025F = 0;
    }
}

void TimerDataHandle(u8* pDate)
{
    stDateTime.second = BCD2DEC(pDate[0]);
    stDateTime.minute = BCD2DEC(pDate[1]);

    if(pDate[2]==0x24)
        pDate[2] = 0;
    stDateTime.hour = BCD2DEC(pDate[2]);

    if(pDate[3] == 0x01)
        stDateTime.week = 0;
    else if(pDate[3] == 0x02)
        stDateTime.week = 1;
    else if(pDate[3] == 0x04)
        stDateTime.week = 2;
    else if(pDate[3] == 0x08)
        stDateTime.week = 3;
    else if(pDate[3] == 0x10)
        stDateTime.week = 4;
    else if(pDate[3] == 0x20)
        stDateTime.week = 5;
    else if(pDate[3] == 0x40)
        stDateTime.week = 6;

    stDateTime.date  = BCD2DEC(pDate[4]);
    stDateTime.month = BCD2DEC(pDate[5]);
    stDateTime.year  = BCD2DEC(pDate[6]);
}

void RtcSetDateTime(STDATETIME *pTime)
{
   u8 Timebuf[7];

   Timebuf[0] = DEC2BCD(pTime->second);
   Timebuf[1] = DEC2BCD(pTime->minute);
   Timebuf[2] = DEC2BCD(pTime->hour);
   Timebuf[3] = (0x01)<<(pTime->week);
   Timebuf[4] = DEC2BCD(pTime->date);
   Timebuf[5] = DEC2BCD(pTime->month);
   Timebuf[6] = DEC2BCD(pTime->year);

   Set8025(0,Timebuf,7);   //Timebuf中数据为BCD码
   TimerDataHandle(Timebuf);
}
void Write8025TDateTime(SysTimeStruct WriteTime)
{
   u8 Timebuf[7];

   Timebuf[0] = WriteTime.Second;
   Timebuf[1] = WriteTime.Minute;
   Timebuf[2] = WriteTime.Hour;
   Timebuf[3] = (0x01)<<(WriteTime.Date.Week);
   Timebuf[4] = WriteTime.Date.Day;
   Timebuf[5] = WriteTime.Date.Month;
   Timebuf[6] = WriteTime.Date.Year;

   Set8025(0,Timebuf,7);   //Timebuf中数据为BCD码
}

void RtcSetLocalTime()
{
//  struct    tm *now_ptm;
//  time_t     timep;
//  STDATETIME set_time;                      //年月日时分秒都是BCD码

//  timep = time(NULL);                       //获取当前RTC时间戳
//  timep += 8 * 3600;                        //RTC时间戳转化成北京时间的时间戳
//  now_ptm = gmtime(&timep);                 //指针指向结构体中所存为十进制
//  set_time.second  = now_ptm->tm_sec;       //取值区间为[0,59]
//  set_time.minute  = now_ptm->tm_min;       //取值区间为[0,59]
//  set_time.hour    = now_ptm->tm_hour;      //取值区间为[0,23]
//  set_time.week    = now_ptm->tm_wday;      //取值区间为[0,6]，0为星期天
//  set_time.date    = now_ptm->tm_mday;      //取值区间为[1,31]
//  set_time.month   = now_ptm->tm_mon + 1;   //取值区间为[0,11] ，0为1月
//  set_time.year    = now_ptm->tm_year - 100;//tm的年从1900开始计算
//  set_time.reserve = 0;

//  RtcSetDateTime(&set_time);
}

void UpdateDateTime()
{
    u8 Timebuf[7];

    Get8025(RTC8025_Second, Timebuf, 7);   //Timebuf中数据为BCD码
    TimerDataHandle(Timebuf);
}

void Read8025TDateTime(SysTimeStruct *pSysTime)
{
    u8 Timebuf[7];
    Get8025(RTC8025_Second, Timebuf, 7);   //Timebuf中数据为BCD码
    //TimerDataHandle(Timebuf);

    pSysTime->Date.Year = Timebuf[6];
    pSysTime->Date.Week = Timebuf[3];
    pSysTime->Date.Month = Timebuf[5];
    pSysTime->Date.Day = Timebuf[4];
    pSysTime->Hour = Timebuf[2];
    pSysTime->Minute = Timebuf[1];
    pSysTime->Second = Timebuf[0];
//    Get8025(RTC8025_Second, (u8 *)&pSysTime, 7);   //Timebuf中数据为BCD码
}


