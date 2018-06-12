/*! @file
********************************************************************************
<PRE>
模块名       : 界面显示任务模块
文件名       : taskGUI.c
相关文件     :
文件实现功能 : 人机交互,LED控制,键盘扫描(数码管显示)
作者         : < 、>
版本         : 1.0
--------------------------------------------------------------------------------
备注         : 数显表(电流表)
--------------------------------------------------------------------------------
修改记录 :
日 期        版本   修改人         修改内容
2017/03/22   1.0    < 、>           创建
</PRE>
********************************************************************************

  * 版权所有(c) YYYY, <xxx>, 保留所有权利

*******************************************************************************/
#ifdef _GUILCD_
/* Includes ------------------------------------------------------------------*/
#include "Include.h"
/* Public variate ------------------------------------------------------------*/



/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define KEY_NUM					0x04 // 按键数量


#define KEY_UP_BUTTON			0x00
#define KEY_DOWN_BUTTON			0x01
#define KEY_MENU_BUTTON			0x02
#define KEY_OK_BUTTON			0x03

#define KEY_PRESSED				0x01
#define KEY_UNPRESSED			0x02
#define KEY_LONG_PRESSED		0x04

u8 g_Key_Status[KEY_NUM];


u16 Leve_1_DispNUM; // 单位标记符

#define  NEG_NUM           '-' // 负号ASCII
#define  POS_NUM           '+' // 正号ASCII

/* Private variables ---------------------------------------------------------*/
#define TIMER_IDLE	SYSTICK

#define TIMER_DELAY_30M	    (1800000/TIMER_IDLE)// 延时30*60s
#define TIMER_DELAY_300S	(300000/TIMER_IDLE) // 延时300s
#define TIMER_DELAY_120S	(120000/TIMER_IDLE) // 延时120s
#define TIMER_DELAY_60S		(60000/TIMER_IDLE)	// 延时60s
#define TIMER_DELAY_50S		(50000/TIMER_IDLE)	// 延时50s
#define TIMER_DELAY_30S		(30000/TIMER_IDLE)	// 延时30s
#define TIMER_DELAY_15S		(15000/TIMER_IDLE)	// 延时15s
#define TIMER_DELAY_7S		(9000/TIMER_IDLE)	// 延时7s
#define TIMER_DELAY_5S		(5000/TIMER_IDLE)	// 延时5s
#define TIMER_DELAY_2S		(2000/TIMER_IDLE)	// 延时2s
#define TIMER_DELAY_1S		(1000/TIMER_IDLE)	// 延时1s
#define TIMER_DELAY_10ms    (9/TIMER_IDLE)	    // 延时10ms
#define TIMER_DELAY_100ms	(100/TIMER_IDLE)	// 延时100ms
#define TIMER_DELAY_200ms	(200/TIMER_IDLE)	// 延时200ms
#define TIMER_DELAY_300ms	(300/TIMER_IDLE)	// 延时300ms
#define TIMER_DELAY_500ms	(500/TIMER_IDLE)	// 延时500ms
#define TIMER_DELAY_750ms	(750/TIMER_IDLE)	// 延时750ms

#define TIMER_DELAY_LONGSET (3000/TIMER_IDLE)   //按键长按延时3s

typedef enum
{
    UPDATA_ADD = 0,
    UPDATA_SUB = 1,

} UPDATA_DIR;

//筛选类别
typedef enum
{
	Select_Net = 0x00,
	Select_Baud = 0x01,
	Select_Data = 0x02,
	Select_Coms = 0x03,
	Select_Save = 0x04,
	Select_Disp_Default = 0x05,
	Select_Do_Mode = 0x06,
	Select_Ao_Mode = 0x07,
	Select_Do_item = 0x08,
	Select_Ao_item = 0x09,

}SELECT_CATE;

// 主菜单索引
#ifdef _LEDHarm_
enum emMENU_MAIN_INDEX
{
    MENU_INDEX_U     = 0x00, // 电压
	MENU_INDEX_MIN	 = MENU_INDEX_U,
	MENU_INDEX_LineU = 0x01, // 线电压
	MENU_INDEX_I	 = 0x02, // 电流
	MENU_INDEX_P	 = 0x03, // 有功功率
	MENU_INDEX_Q     = 0x04, // 无功功率
	MENU_INDEX_S     = 0x05, // 视在功率
	MENU_INDEX_PF    = 0x06, // 分相功因
	MENU_INDEX_F     = 0x07, // 功因频率
	MENU_INDEX_PosEPT    = 0x08, // +有功电能
	MENU_INDEX_NegEPT    = 0x09, // -有功电能
	MENU_INDEX_PosEQT    = 0x0A, // +无功电能
	MENU_INDEX_NegEQT    = 0x0B, // -无功电能
	MENU_INDEX_UAngle    = 0x0C,
    MENU_INDEX_IAngle    = 0x0D,
    MENU_INDEX_Uimb      = 0x0E,
    MENU_INDEX_HarmU     = 0x0F,
    MENU_INDEX_HarmI     = 0x10,
    MENU_INDEX_HarmU3    = 0x11,
    MENU_INDEX_HarmU5    = 0x12,
    MENU_INDEX_HarmU7    = 0x13,
    MENU_INDEX_HarmU9    = 0x14,
    MENU_INDEX_HarmU11   = 0x15,
    MENU_INDEX_HarmU13   = 0x16,
    MENU_INDEX_HarmU15   = 0x17,
    MENU_INDEX_HarmU17   = 0x18,
    MENU_INDEX_HarmU19   = 0x19,
    MENU_INDEX_HarmU21   = 0x1A,
    MENU_INDEX_HarmU23   = 0x1B,
    MENU_INDEX_HarmU25   = 0x1C,
    MENU_INDEX_HarmU27   = 0x1D,
    MENU_INDEX_HarmU29   = 0x1E,
    MENU_INDEX_HarmU31   = 0x1F,
    MENU_INDEX_HarmI3    = 0x20,
    MENU_INDEX_HarmI5    = 0x21,
    MENU_INDEX_HarmI7    = 0x22,
    MENU_INDEX_HarmI9    = 0x23,
    MENU_INDEX_HarmI11   = 0x24,
    MENU_INDEX_HarmI13   = 0x25,
    MENU_INDEX_HarmI15   = 0x26,
    MENU_INDEX_HarmI17   = 0x27,
    MENU_INDEX_HarmI19   = 0x28,
    MENU_INDEX_HarmI21   = 0x29,
    MENU_INDEX_HarmI23   = 0x2A,
    MENU_INDEX_HarmI25   = 0x2B,
    MENU_INDEX_HarmI27   = 0x2C,
    MENU_INDEX_HarmI29   = 0x2D,
    MENU_INDEX_HarmI31   = 0x2E,
	MENU_INDEX_MAX	 = MENU_INDEX_HarmI31,
};
#else
enum emMENU_MAIN_INDEX
{
    MENU_INDEX_U     = 0x00, // 电压
	MENU_INDEX_MIN	 = MENU_INDEX_U,
	MENU_INDEX_LineU = 0x01, // 线电压
	MENU_INDEX_I	 = 0x02, // 电流
	MENU_INDEX_P	 = 0x03, // 有功功率
	MENU_INDEX_Q     = 0x04, // 无功功率
	MENU_INDEX_S     = 0x05, // 视在功率
	MENU_INDEX_PF    = 0x06, // 分相功因
	MENU_INDEX_F     = 0x07, // 功因频率
	MENU_INDEX_PosEPT    = 0x08, // +有功电能
	MENU_INDEX_NegEPT    = 0x09, // -有功电能
	MENU_INDEX_PosEQT    = 0x0A, // +无功电能
	MENU_INDEX_NegEQT    = 0x0B, // -无功电能
	MENU_INDEX_UAngle    = 0x0C,
    MENU_INDEX_IAngle    = 0x0D,
    MENU_INDEX_Uimb      = 0x0E,
    MENU_INDEX_HarmU     = 0x0F,
    MENU_INDEX_HarmI     = 0x10,
    MENU_INDEX_HarmU3    = 0x11,
    MENU_INDEX_HarmU5    = 0x12,
    MENU_INDEX_HarmU7    = 0x13,
    MENU_INDEX_HarmU9    = 0x14,
    MENU_INDEX_HarmU11   = 0x15,
    MENU_INDEX_HarmU13   = 0x16,
    MENU_INDEX_HarmU15   = 0x17,
    MENU_INDEX_HarmU17   = 0x18,
    MENU_INDEX_HarmU19   = 0x19,
    MENU_INDEX_HarmU21   = 0x1A,
    MENU_INDEX_HarmU23   = 0x1B,
    MENU_INDEX_HarmU25   = 0x1C,
    MENU_INDEX_HarmU27   = 0x1D,
    MENU_INDEX_HarmU29   = 0x1E,
    MENU_INDEX_HarmU31   = 0x1F,
    MENU_INDEX_HarmI3    = 0x20,
    MENU_INDEX_HarmI5    = 0x21,
    MENU_INDEX_HarmI7    = 0x22,
    MENU_INDEX_HarmI9    = 0x23,
    MENU_INDEX_HarmI11   = 0x24,
    MENU_INDEX_HarmI13   = 0x25,
    MENU_INDEX_HarmI15   = 0x26,
    MENU_INDEX_HarmI17   = 0x27,
    MENU_INDEX_HarmI19   = 0x28,
    MENU_INDEX_HarmI21   = 0x29,
    MENU_INDEX_HarmI23   = 0x2A,
    MENU_INDEX_HarmI25   = 0x2B,
    MENU_INDEX_HarmI27   = 0x2C,
    MENU_INDEX_HarmI29   = 0x2D,
    MENU_INDEX_HarmI31   = 0x2E,
	MENU_INDEX_MAX	 = MENU_INDEX_IAngle,
};
#endif

enum emMENU_SUBMAIN_INDEX
{
	MENUSUB_INDEX_READ	= 0x00, // 电流
	MENUSUB_INDEX_MIN	= MENUSUB_INDEX_READ,
	MENUSUB_INDEX_PROG	= 0x01, // 频率
	MENUSUB_INDEX_MAX	= MENUSUB_INDEX_PROG,

};

// MENU1菜单索引
enum emMENU_SETMENU1_INDEX
{
	MENU1_INDEX_INPT	= 0x00, // 输入设置
	MENU1_INDEX_MIN	    = MENU1_INDEX_INPT,
	MENU1_INDEX_COM1	= 0x01, // 通讯
	MENU1_INDEX_SYS     = 0x02, // 系统
	MENU1_INDEX_DO1     = 0x03, // 输出1
	MENU1_INDEX_DO2     = 0x04, // 输出2
	MENU1_INDEX_DO3     = 0x05, // 输出3
	MENU1_INDEX_DO4     = 0x06, // 输出3
	MENU1_INDEX_AO1     = 0x07, // 模拟输出1
	MENU1_INDEX_AO2     = 0x08, // 模拟输出1
    MENU1_INDEX_VER     = 0x09, // 版本
	MENU1_INDEX_MAX	    = MENU1_INDEX_AO2,
	MENU1_INDEX_RMAX    = MENU1_INDEX_VER,
};

// MENU2_1菜单索引
enum emMENU_SETMENU2_1_INDEX
{
    MENU2_1_INDEX_NET   = 0x00, // 接线方式
    MENU2_1_INDEX_MIN	= MENU2_1_INDEX_NET,
    MENU2_1_INDEX_PT	= 0x01, // PT
    MENU2_1_INDEX_CT    = 0x02, // CT
	MENU2_1_INDEX_MAX	= MENU2_1_INDEX_CT,
};

// MENU2_2菜单索引
enum emMENU_SETMENU2_2_INDEX
{
	MENU2_2_INDEX_ADD	= 0x00, // 通讯地址
	MENU2_2_INDEX_MIN	= MENU2_2_INDEX_ADD,
	MENU2_2_INDEX_BAUD	= 0x01, // 波特率
	MENU2_2_INDEX_DATA  = 0x02, // 数据格式
	MENU2_2_INDEX_COMS  = 0x03, // 数据格式
	MENU2_2_INDEX_MAX	= MENU2_2_INDEX_COMS,
};

// MENU2_3菜单索引
enum emMENU_SETMENU2_3_INDEX
{
	MENU2_3_INDEX_CYC	= 0x00, // 显示切换时间
	MENU2_3_INDEX_MIN	= MENU2_3_INDEX_CYC,
	MENU2_3_INDEX_LIGH	= 0x01, // 亮度等级
	MENU2_3_INDEX_DISP	= 0x02, // 默认显示
	MENU2_3_INDEX_ALR	= 0x03, // 闪烁报警
	MENU2_3_INDEX_CLRE	= 0x04, // 电能清零
	MENU2_3_INDEX_CODE	= 0x05, // 编程密码
	MENU2_3_INDEX_MAX	= MENU2_3_INDEX_CODE,
	MENU2_3_INDEX_RMAX  = MENU2_3_INDEX_ALR,
};

// MENU2_4菜单索引  D0-1
enum emMENU_SETMENU2_4_INDEX
{
	MENU2_4_INDEX_MODE	= 0x00, // 工作模式
	MENU2_4_INDEX_MIN	= MENU2_4_INDEX_MODE,
	MENU2_4_INDEX_TIME	= 0x01, // 脉冲输出宽度
	MENU2_4_INDEX_ITEM  = 0x02, // 报警项目
	MENU2_4_INDEX_UAL   = 0x03, // 报警值
	MENU2_4_INDEX_HYS   = 0x04, // 回滞值
	MENU2_4_INDEX_DELAY = 0x05, // 延时
	MENU2_4_INDEX_MAX	= MENU2_4_INDEX_DELAY,
};

// MENU2_5菜单索引  D0-2
enum emMENU_SETMENU2_5_INDEX
{
	MENU2_5_INDEX_MODE	= 0x00, // 工作模式
	MENU2_5_INDEX_MIN	= MENU2_5_INDEX_MODE,
	MENU2_5_INDEX_TIME	= 0x01, // 脉冲输出宽度
	MENU2_5_INDEX_ITEM  = 0x02, // 报警项目
	MENU2_5_INDEX_UAL   = 0x03, // 报警值
	MENU2_5_INDEX_HYS   = 0x04, // 回滞值
	MENU2_5_INDEX_DELAY = 0x05, // 延时
	MENU2_5_INDEX_MAX	= MENU2_5_INDEX_DELAY,
};

// MENU2_6菜单索引  DO-3
enum emMENU_SETMENU2_6_INDEX
{
	MENU2_6_INDEX_MODE	= 0x00, // 工作模式
	MENU2_6_INDEX_MIN	= MENU2_6_INDEX_MODE,
	MENU2_6_INDEX_TIME	= 0x01, // 脉冲输出宽度
	MENU2_6_INDEX_ITEM  = 0x02, // 报警项目
	MENU2_6_INDEX_UAL   = 0x03, // 报警值
	MENU2_6_INDEX_HYS   = 0x04, // 回滞值
	MENU2_6_INDEX_DELAY = 0x05, // 延时
	MENU2_6_INDEX_MAX	= MENU2_6_INDEX_DELAY,
};

// MENU2_7菜单索引  DO-4
enum emMENU_SETMENU2_7_INDEX
{
	MENU2_7_INDEX_MODE	= 0x00, // 工作模式
	MENU2_7_INDEX_MIN	= MENU2_7_INDEX_MODE,
	MENU2_7_INDEX_TIME	= 0x01, // 脉冲输出宽度
	MENU2_7_INDEX_ITEM  = 0x02, // 报警项目
	MENU2_7_INDEX_UAL   = 0x03, // 报警值
	MENU2_7_INDEX_HYS   = 0x04, // 回滞值
	MENU2_7_INDEX_DELAY = 0x05, // 延时
	MENU2_7_INDEX_MAX	= MENU2_7_INDEX_DELAY,
};

// MENU2_8菜单索引  AO-1
enum emMENU_SETMENU2_8_INDEX
{
	MENU2_8_INDEX_MODE	= 0x00, // 工作模式
	MENU2_8_INDEX_MIN	= MENU2_8_INDEX_MODE,
	MENU2_8_INDEX_ITEM  = 0x01, // 报警项目
	MENU2_8_INDEX_DS    = 0x02, // 报警值
	MENU2_8_INDEX_FS    = 0x03, // 报警值
	MENU2_8_INDEX_MAX	= MENU2_8_INDEX_FS,
};

// MENU2_8菜单索引  AO-2
enum emMENU_SETMENU2_9_INDEX
{
	MENU2_9_INDEX_MODE	= 0x00, // 工作模式
	MENU2_9_INDEX_MIN	= MENU2_9_INDEX_MODE,
	MENU2_9_INDEX_ITEM  = 0x01, // 报警项目
	MENU2_9_INDEX_DS    = 0x02, // 报警值
	MENU2_9_INDEX_FS    = 0x03, // 报警值
	MENU2_9_INDEX_MAX	= MENU2_9_INDEX_FS,
};

// 焦点显示第几位
#define CURRENTFOCUS_NONE		0x00
#define CURRENTFOCUS_0			0x01
#define CURRENTFOCUS_1			0x02
#define CURRENTFOCUS_2			0x04
#define CURRENTFOCUS_3			0x08
#define CHARMODE_NC             0x00

// 按键状态
enum emKEYSTATUS
{
	KEYSTATUS_NONE = 0,
	KEYSTATUS_UP = 1,
	KEYSTATUS_DOWN = 2,
	KEYSTATUS_MENU = 3,
	KEYSTATUS_OK = 4,
	KEYSTATUS_MENULONG = 5,
};

void Disp_IO_Status(u8 InStatus,u8 OutSatus);
void DispMenu_Main(void);   // 主界面
void DispMenu_Menu1(void); // 模式选择(阅读/编程)
void DispMenu_Menu2(void); // 设置主选项菜单
void DispMenu_Menu3(void); // 设置子菜单
void DispMenu_MenuValue(void);     // 设置数值
void DispMenu_MenuCode(void);     // 密码显示
void DispMenu_MenuCodeErr(void);  // 密码错误显示
void DispMenu_MenuSave(void);      // 模式选择界面




// 与变量Key_Tab行对应
#define KEYFUNC_MAIN        0 // 显示主菜单
#define KEYFUNC_MENU1	    1 // 选择模式
#define KEYFUNC_MENU2	    2 // 设置菜单二级
#define KEYFUNC_MENU3		3 // 设置菜单三级
#define KEYFUNC_MENUVALUE	4 // 设置菜单值
#define KEYFUNC_MENUCODE	5 // 密码输入界面
#define KEYFUNC_MENUCODEERR	6 // 密码输入错误界面
#define KEYFUNC_MENUSAVE	7 // 退出保存界面

void (*KeyFuncPtr)();			// 按键功能指针

typedef struct{
	u8 KeyStateIndex;			// 当前状态索引号
	u8 KeyLeftState;			// 按下"向左"键时转向的状态索引号
	u8 KeyRightState;			// 按下"向右"键时转向的状态索引号
    u8 KeyUpState;              // 按下"向上"键时转向的状态索引号
    u8 KeyDnState;              // 按下"向下"键时转向的状态索引号
	void (*CurrentOperate)();	// 当前状态应该执行的功能操作
} KbdTabStruct;

const KbdTabStruct Key_Tab[]={
//    |-----------> Index
//    |  Up
//    |  |  Down
//    |  |  |  Menu
//    |  |  |  |  Ok   --->功能函数
//    |  |  |  |  |       |
    { 0, 0, 0, 0, 0,(*DispMenu_Main)},  // 待机画面 显示主菜单画面
    { 1, 1, 1, 0, 2,(*DispMenu_Menu1)}, // 显示MENU键长按设置值
    { 2, 2, 2, 0, 3,(*DispMenu_Menu2)}, // 显示OK设置值
    { 3, 3, 3, 2, 4,(*DispMenu_Menu3)}, // 显示OK设置值
    { 4, 4, 4, 3, 4,(*DispMenu_MenuValue)}, // 显示OK设置值
    { 5, 5, 5, 1, 5,(*DispMenu_MenuCode)},  // 显示密码输入界面
    { 6, 6, 6, 6, 6,(*DispMenu_MenuCodeErr)}, // 显示密码输入界面
    { 7, 7, 7, 7, 7,(*DispMenu_MenuSave)}, // 保存界面

};

static u8 DispValueUpDataFlag = FALSE;
u8 DispMode = ReadMode;
u8 LEDLEVEL = 0;

static u8 KeyFuncIndex = KEYFUNC_MAIN;
static u8 sg_DispMainMenuIndex = MENU_INDEX_MIN;              // 要显示的主菜单索引号
static u8 sg_DispSubMainMenuIndex = MENUSUB_INDEX_READ;     // 要显示的主菜单索引号
static u8 sg_DispSetMenu1Index = MENU1_INDEX_INPT;		    // 要显示的设置一级索引号
static u8 sg_DispSetMenu2_1_Index = MENU2_1_INDEX_MIN;		// 要显示的2-1菜单索引号
static u8 sg_DispSetMenu2_2_Index = MENU2_2_INDEX_ADD;		// 要显示的2-2菜单索引号
static u8 sg_DispSetMenu2_3_Index = MENU2_3_INDEX_CYC;		// 要显示的2-3菜单索引号
static u8 sg_DispSetMenu2_4_Index = MENU2_4_INDEX_MODE;		// 要显示的2-4菜单索引号
static u8 sg_DispSetMenu2_5_Index = MENU2_5_INDEX_MODE;		// 要显示的2-5菜单索引号
static u8 sg_DispSetMenu2_6_Index = MENU2_6_INDEX_MODE;		// 要显示的2-6菜单索引号
static u8 sg_DispSetMenu2_7_Index = MENU2_7_INDEX_MODE;		// 要显示的2-7菜单索引号
static u8 sg_DispSetMenu2_8_Index = MENU2_8_INDEX_MODE;		// 要显示的2-8菜单索引号
static u8 sg_DispSetMenu2_9_Index = MENU2_9_INDEX_MODE;		// 要显示的2-9菜单索引号

typedef struct
{
	u8 CurrentFocus;	// 当前的焦点
	u8 DispWidth;		// 显示位数
}Disp_FocusStruct;
Disp_FocusStruct Disp_Focus;

static u8 sg_CurrentDispChar[13];			// 要显示的字符
static s16 sg_CurrentDispNumber;			// 要显示的数据
static s32 sg_DispSetValue = 0;				// 要显示的设置数据

static u8 key_scan[KEY_NUM];				// 按键扫描状态
static u8 key_scan_bak[KEY_NUM];			// 上次键扫描状态
static u8 key_status[KEY_NUM];				// 当前按键状态
static u8 key_Surestatus[KEY_NUM];		    // 当前按键状态
static u16 key_LongPress_Tick[KEY_NUM];	    // 键长按计数值

u8 KeyDiff(void); // 按键状态判断
void DispChar(u8 num,u8 ch1, u8 ch2, u8 ch3, u8 ch4);
u16 CharToNumber(void);	// 在显示设置菜单中的字符数据转换为十进制数据
void GetSetOldValue(void);
void UpdataSetValue(UPDATA_DIR Updata_dir);
void UpdataOrSetValue(SELECT_CATE Cate,UPDATA_DIR Updata_Dir);  //更新数值

volatile u8 vg_Display_Value[16];   // 显示保存缓存

//static u8 UpDataFlagNum = 0;
static BOOL sg_bSetFlashEnable = TRUE;  // 闪烁使能
static BOOL g_bInitDefault = FALSE;		// 上电后显示默认
static BOOL g_bUpdataDefault = FALSE;	// 显示默认
static BOOL g_bBlackLightDefault = FALSE;  // 背光亮
BOOL g_fftEnable = FALSE;        //

static BOOL g_bCycleDefault = FALSE;  // 背光亮

static BOOL g_bRestoreDlyEnable = TRUE;
static BOOL g_b1DlyEnable = TRUE;
static BOOL g_b5DlyEnable = FALSE;
static BOOL g_bBlackLightEnble = FALSE;  // 背光亮
BOOL g_LEDEnable = TRUE;

static u16 sg_ReFlashCount = 0;         // 没有按键后的延时值
static u32 sg_RestoreDlyTick = 0;
static u16 sg_ComFlashNum = 0;
static u8 sg_ComFlag = FALSE;           // 通信标志

static u32 s_SysStartTick = 0;
static BOOL s_bSysStartEnable = FALSE;
static u16 g_fftTick = 0;
static u32 g_BlackTick = 0;
static u16 g_CycleTick = 0;

/*--------------------------输出时间 设置------------------------*/
static u16 g_Out_1_tick = 0;
static u16 g_Out_2_tick = 0;
static u16 g_Out_3_tick = 0;
static u16 g_Out_4_tick = 0;

static u16 g_Out_01_Time10ms = 0;
static u16 g_Out_02_Time10ms = 0;
static u16 g_Out_03_Time10ms = 0;
static u16 g_Out_04_Time10ms = 0;

static u16 g_Out_11_Time10ms = 0;
static u16 g_Out_12_Time10ms = 0;
static u16 g_Out_13_Time10ms = 0;
static u16 g_Out_14_Time10ms = 0;


static BOOL g_b1OutOverTurn = FALSE;
static BOOL g_b2OutOverTurn = FALSE;
static BOOL g_b3OutOverTurn = FALSE;
static BOOL g_b4OutOverTurn = FALSE;


/*---------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/*void Task_Timer_init(void)
{

}*/


/**
  * @brief  GUI初始化.
  * @note
  * @param  None
  * @retval None
  */
void Task_GUI_init(void)
{
    HT_Init();
    LCD_Update_screen_T(TRUE);
    PT_TimerDelay(2000);
    Reset_Screen();
	KeyDiff();
}


/**
  * @brief  得到某个按键的状态.
  * @note
  * @param  KeyNo:按键编号
  * @retval None
  */
void OneKeyStaus(u8 KeyNo)
{
    if(g_Key_Status[KeyNo])
    {
        key_scan[KeyNo] <<= 1;
        key_scan[KeyNo] += 1;
        if((key_scan[KeyNo] == 0xff)  && (key_scan_bak[KeyNo] != 0xff))
        {
            key_status[KeyNo] |= KEY_UNPRESSED;
            if (key_LongPress_Tick[KeyNo] != 0)
            {
                key_Surestatus[KeyNo] |= KEY_PRESSED;
            }
            key_LongPress_Tick[KeyNo] = 0;
        }
    }
    else
    {
        key_scan[KeyNo] <<= 1;
        if((key_scan[KeyNo] == 0x00) && (key_scan_bak[KeyNo] != 0x00))
        {
            key_status[KeyNo] |= KEY_PRESSED;
            key_LongPress_Tick[KeyNo] = TIMER_DELAY_LONGSET ;
        }
    }
    key_scan_bak[KeyNo] = key_scan[KeyNo];
    if(key_LongPress_Tick[KeyNo])
    {
        if(!--key_LongPress_Tick[KeyNo])
        {
            key_status[KeyNo] |= KEY_LONG_PRESSED;//0x04
        }
    }
}


/**
  * @brief  通过IO口得到4个按键的状态,然后进行转换得到具体的按键状态.
  * @note
  * @param  None
  * @retval None
  */
void KeyStatus(void)
{
	g_Key_Status[0] = KEY_UP_STATUS;
	g_Key_Status[1] = KEY_DOWN_STATUS;
	g_Key_Status[2] = KEY_MENU_STATUS;
	g_Key_Status[3] = KEY_OK_STATUS;


	OneKeyStaus(KEY_UP_BUTTON);
	OneKeyStaus(KEY_DOWN_BUTTON);
	OneKeyStaus(KEY_MENU_BUTTON);
	OneKeyStaus(KEY_OK_BUTTON);
}




/**
  * @brief	对各种按键组合进行判断
  * @param	None
  * @retval 键类型
  */
u8 KeyDiff(void)
{
    u8 Key = KEYSTATUS_NONE;
    /*********************************************************/
    if(key_Surestatus[KEY_UP_BUTTON] & KEY_PRESSED)
    {
        key_Surestatus[KEY_UP_BUTTON] &= ~KEY_PRESSED;
        Key = KEYSTATUS_UP;
    }
    if(key_Surestatus[KEY_DOWN_BUTTON] & KEY_PRESSED)
    {
        key_Surestatus[KEY_DOWN_BUTTON] &= ~KEY_PRESSED;
        Key = KEYSTATUS_DOWN;
    }
    if(key_Surestatus[KEY_MENU_BUTTON] & KEY_PRESSED)
    {
        key_Surestatus[KEY_MENU_BUTTON] &= ~KEY_PRESSED;
        Key = KEYSTATUS_MENU;
    }
    if(key_Surestatus[KEY_OK_BUTTON] & KEY_PRESSED)
    {
        key_Surestatus[KEY_OK_BUTTON] &= ~KEY_PRESSED;
        Key = KEYSTATUS_OK;
    }
    if(key_status[KEY_MENU_BUTTON] & KEY_LONG_PRESSED)
    {
        key_status[KEY_MENU_BUTTON] &= ~KEY_LONG_PRESSED;
        Key = KEYSTATUS_MENULONG;           //直接退到主界面
    }
    return Key;
}


/*
void DinHandler(void)
{
    u8 i = 0;
    for(i = 0; i < DIN_NUM;i++)
    {
        if(g_Din_Status[2] == 0)
        {
            CTRL1_Enble;
        }
        else
        {
            CTRL1_DisEnble;
        }
        if(g_Din_Status[3] == 0)
        {
            CTRL2_Enble;
        }
        else
        {
            CTRL2_DisEnble;
        }
        if(g_Din_Status[4] == 0)
        {
            CTRL3_Enble;
        }
        else
        {
            CTRL3_DisEnble;
        }
    }
}*/

/**
  * @brief	通信任务接收到正确帧后设置标志,在GUI任务判断该标志进行闪烁显示处理
  * @param	None
  * @retval None
  */
void GUI_SetComFlag(void)
{
	sg_ComFlag = TRUE;
	sg_ComFlashNum = 0;
}


void GUI_Timer_On(void)
{
	static u32 s_Dly1sTick = 0;
	static u32 s_Dly5sTick = 0;
	static u16 s_FlashNum = 0;
	u32 TimerDlyNum = 0;

	sg_ReFlashCount++;
	if (!s_bSysStartEnable)
	{
		s_SysStartTick++;
		if (s_SysStartTick++ > TIMER_DELAY_7S)
		{
			s_bSysStartEnable = TRUE;
			s_SysStartTick = 0;
		}
	}
    /**************************OutPut*****************************/
    if(g_Out_Status[Dout_ONE])
    {
        if(g_tParam.CtrlParam.DO1_Time != 0x00)
        {
            if(g_Out_1_tick++ >= TIMER_DELAY_10ms)
            {
                g_Out_1_tick= 0;
                if(g_b1OutOverTurn == FALSE)
                {
                    if(g_Out_01_Time10ms++ >= g_tParam.CtrlParam.DO1_Time)
                    {
                        g_Out_01_Time10ms = 0;
                        g_b1OutOverTurn = TRUE;
                        CTRL1_DisEnble;
                    }
                }
                else
                {
                    if(g_Out_11_Time10ms++ >= (10000 - g_tParam.CtrlParam.DO1_Time))
                    {
                        g_Out_11_Time10ms = 0;
                        g_b1OutOverTurn = FALSE;
                        CTRL1_Enble;
                    }
                }
            }
        }
        else
        {
             CTRL1_Enble;
        }
    }
    else
    {
        CTRL1_DisEnble;
    }
    if(g_Out_Status[Dout_TWO])
    {
        if(g_tParam.CtrlParam.DO2_Time != 0x00)
        {
            if(g_Out_2_tick++ >= TIMER_DELAY_10ms)
            {
                g_Out_2_tick= 0;
                if(g_b2OutOverTurn == FALSE)
                {
                    if(g_Out_02_Time10ms++ >= g_tParam.CtrlParam.DO2_Time/100)
                    {
                        g_Out_02_Time10ms = 0;
                        g_b2OutOverTurn = TRUE;
                        CTRL2_DisEnble;
                    }
                }
                else
                {
                    if(g_Out_12_Time10ms++ >= (10000 - g_tParam.CtrlParam.DO2_Time))
                    {
                        g_Out_12_Time10ms = 0;
                        g_b2OutOverTurn = FALSE;
                        CTRL2_Enble;
                    }
                }
            }
        }
        else
        {
             CTRL2_Enble;
        }
    }
    else
    {
        CTRL2_DisEnble;
    }
    if(g_Out_Status[Dout_THREE])
    {
        if(g_tParam.CtrlParam.DO3_Time != 0x00)
        {
            if(g_Out_3_tick++ >= TIMER_DELAY_10ms)
            {
                g_Out_3_tick= 0;
                if(g_b3OutOverTurn == FALSE)
                {
                    if(g_Out_03_Time10ms++ >= g_tParam.CtrlParam.DO3_Time)
                    {
                        g_Out_03_Time10ms = 0;
                        g_b3OutOverTurn = TRUE;
                        CTRL3_DisEnble;
                    }
                }
                else
                {
                    if(g_Out_13_Time10ms++ >= (10000 - g_tParam.CtrlParam.DO3_Time))
                    {
                        g_Out_13_Time10ms = 0;
                        g_b3OutOverTurn = FALSE;
                        CTRL3_Enble;
                    }
                }
            }
        }
        else
        {
             CTRL3_Enble;
        }
    }
    else
    {
        CTRL3_DisEnble;
    }
    if(g_Out_Status[Dout_FOUR])
    {
        if(g_tParam.CtrlParam.DO4_Time != 0x00)
        {
            if(g_Out_4_tick++ >= TIMER_DELAY_10ms)
            {
                g_Out_4_tick= 0;
                if(g_b4OutOverTurn == FALSE)
                {
                    if(g_Out_04_Time10ms++ >= g_tParam.CtrlParam.DO4_Time)
                    {
                        g_Out_04_Time10ms = 0;
                        g_b4OutOverTurn = TRUE;
                        CTRL4_DisEnble;
                    }
                }
                else
                {
                    if(g_Out_14_Time10ms++ >= (10000 - g_tParam.CtrlParam.DO4_Time))
                    {
                        g_Out_14_Time10ms = 0;
                        g_b4OutOverTurn = FALSE;
                        CTRL4_Enble;
                    }
                }
            }
        }
        else
        {
             CTRL4_Enble;
        }
    }
    else
    {
        CTRL4_DisEnble;
    }
	/**************************未动作300S后关闭显示*****************
	if (g_bOffDispDlyEnable)
	{
		if (sg_OffDispDlyTick++ >= TIMER_DELAY_300S)// 300s后关闭显示
		{
			sg_OffDispDlyTick = 0;
			g_bOffDispDlyEnable = FALSE;  // 关闭显示时间变量
		}
	}
	else
	{
		sg_OffDispDlyTick = 0;
	}*/

    /**************************循环显示*******************************/
    if((g_tParam.CtrlParam.Cycle == CYCYES) && g_bCycleDefault)
    {
        if( g_CycleTick++ >= TIMER_DELAY_5S)
        {
            g_CycleTick = 0;
            g_bCycleDefault = FALSE;
            sg_DispMainMenuIndex++;
			if(sg_DispMainMenuIndex > MENU_INDEX_MAX)
			{
			    sg_DispMainMenuIndex = MENU_INDEX_MIN;
			}
        }
    }
    else
    {
        g_CycleTick = 0;
        g_bCycleDefault = TRUE;
    }

    /******************************背光*******************************/
    if ( g_bBlackLightDefault )
    {
        if(g_BlackTick ++ >= TIMER_DELAY_1S * g_tParam.CtrlParam.BlackTime)
        {
            g_BlackTick = 0;
            //g_bBlackLightDefault = FALSE;
            g_bBlackLightEnble = FALSE;
        }
    }
    else
    {
        g_BlackTick = 0;
        g_bBlackLightEnble = TRUE;
    }

    /**************************FFT计算初始化及时**********************/
    if (g_fftEnable)
	{
		if (g_fftTick++ >= TIMER_DELAY_1S)// 300s后关闭显示
		{
			g_fftTick = 0;
			g_fftEnable = FALSE;  // 关闭显示时间变量
		}
	}
	else
	{
		g_fftTick = 0;
	}

    /**************************其他界面120S后显示默认界面************/
	if (g_bRestoreDlyEnable && (g_tParam.CtrlParam.Cycle != CYCYES))
	{
		//TimerDlyNum = TIMER_DELAY_120S;         // 延时120秒
		TimerDlyNum = TIMER_DELAY_30M;
		if (sg_RestoreDlyTick++ > TimerDlyNum)  // 显示默认界面
		{
			sg_RestoreDlyTick = 0;
			g_bRestoreDlyEnable = FALSE;
			g_bUpdataDefault = TRUE;
		}
	}
	else
	{
		sg_RestoreDlyTick = 0;
	}
    /**************************开机1S延时****************************/
	if (g_b1DlyEnable)
	{
		if (s_Dly1sTick++ >= TIMER_DELAY_1S)
		{
			s_Dly1sTick = 0;
			g_bInitDefault = TRUE;
			g_b1DlyEnable = FALSE;
		}
	}
	/**************************密码错误界面返回**********************/
	if (g_b5DlyEnable)
	{
		if (s_Dly5sTick++ >= TIMER_DELAY_5S)
		{
			s_Dly5sTick = 0;
			g_b5DlyEnable = FALSE;
			sg_DispSetValue = 0;
			KeyFuncIndex = KEYFUNC_MENUCODE;
			Disp_Focus.CurrentFocus = CURRENTFOCUS_3;
			Reset_Screen();
			KeyFuncPtr=Key_Tab[KeyFuncIndex].CurrentOperate;
		}
	}
	/**************************闪烁标识******************************/
	s_FlashNum++;
	if (s_FlashNum >= TIMER_DELAY_750ms)
	{
		s_FlashNum = 0;
		sg_bSetFlashEnable = !sg_bSetFlashEnable;
	}
    /**************************通讯*****无****************************/
	if (sg_ComFlag)
	{
		sg_ComFlashNum++;
		if (sg_ComFlashNum >= TIMER_DELAY_500ms)
		{
			sg_ComFlashNum = 0;
			sg_ComFlag = FALSE;
		}
	}
	else
	{
		sg_ComFlashNum = 0;
	}
}

void Task_Timer_Function(void)
{
	GUI_Timer_On();
	KeyStatus();


}

void GUI_Key_Menu(void)     //菜单键操作
{
	switch (KeyFuncIndex)
	{
    	case KEYFUNC_MAIN:
    	    g_bUpdataDefault = TRUE;  // 恢复到默认界面
    		break;
    	case KEYFUNC_MENU1:
    	    g_bUpdataDefault = TRUE;  // 恢复到默认界面
            break;
    	case KEYFUNC_MENU2:
    	    if(DispMode == ProgMode)
    	    {
                KeyFuncIndex = KEYFUNC_MENUSAVE;        //在设置二级菜单退回到主界面
    	    }
            else
            {
                KeyFuncIndex = KEYFUNC_MAIN;
                DispMode = ReadMode;
            }
            sg_DispSubMainMenuIndex = MENUSUB_INDEX_READ;
    		sg_DispSetMenu1Index = MENU1_INDEX_INPT;
    		sg_DispSetMenu2_1_Index = MENU2_1_INDEX_NET;
    		sg_DispSetMenu2_2_Index = MENU2_2_INDEX_ADD;
    		sg_DispSetMenu2_3_Index = MENU2_3_INDEX_CYC;
    		sg_DispSetMenu2_4_Index = MENU2_4_INDEX_MODE;
            sg_DispSetMenu2_5_Index = MENU2_5_INDEX_MODE;
    		sg_DispSetMenu2_6_Index = MENU2_6_INDEX_MODE;
    		sg_DispSetMenu2_7_Index = MENU2_7_INDEX_MODE;
    		sg_DispSetMenu2_8_Index = MENU2_8_INDEX_MODE;
    		sg_DispSetMenu2_9_Index = MENU2_9_INDEX_MODE;
    		sg_DispSetValue = SaveNO;
    		break;
    	case KEYFUNC_MENU3:
	  	    KeyFuncIndex = KEYFUNC_MENU2;
    		sg_DispSetMenu2_1_Index = MENU2_1_INDEX_NET;
    		sg_DispSetMenu2_2_Index = MENU2_2_INDEX_ADD;
    		sg_DispSetMenu2_3_Index = MENU2_3_INDEX_CYC;
    		sg_DispSetMenu2_4_Index = MENU2_4_INDEX_MODE;
            sg_DispSetMenu2_5_Index = MENU2_5_INDEX_MODE;
    		sg_DispSetMenu2_6_Index = MENU2_6_INDEX_MODE;
    		sg_DispSetMenu2_7_Index = MENU2_7_INDEX_MODE;
    		sg_DispSetMenu2_8_Index = MENU2_8_INDEX_MODE;
    		sg_DispSetMenu2_9_Index = MENU2_9_INDEX_MODE;
    		break;
    	case KEYFUNC_MENUVALUE:
    	  	KeyFuncIndex = KEYFUNC_MENU3;
    		break;
		case KEYFUNC_MENUCODE:
			KeyFuncIndex = KEYFUNC_MENU1;
			sg_DispSubMainMenuIndex = MENUSUB_INDEX_PROG;
			sg_DispSetMenu1Index = MENU1_INDEX_INPT;
    		sg_DispSetMenu2_1_Index = MENU2_1_INDEX_NET;
    		sg_DispSetMenu2_2_Index = MENU2_2_INDEX_ADD;
    		sg_DispSetMenu2_3_Index = MENU2_3_INDEX_CYC;
    		sg_DispSetMenu2_4_Index = MENU2_4_INDEX_MODE;
            sg_DispSetMenu2_5_Index = MENU2_5_INDEX_MODE;
    		sg_DispSetMenu2_6_Index = MENU2_6_INDEX_MODE;
    		sg_DispSetMenu2_7_Index = MENU2_7_INDEX_MODE;
    		sg_DispSetMenu2_8_Index = MENU2_8_INDEX_MODE;
    		sg_DispSetMenu2_9_Index = MENU2_9_INDEX_MODE;
    		break;
    	case KEYFUNC_MENUCODEERR:
    	    KeyFuncIndex = KEYFUNC_MENUCODE;
    		break;
    	case KEYFUNC_MENUSAVE:
    	    KeyFuncIndex = KEYFUNC_MENU2;
			sg_DispSetMenu1Index = MENU1_INDEX_INPT;  // 密码正确 进入设置菜单
    	    break;
        default:
            g_bUpdataDefault = TRUE;
            break;
	}
	Reset_Screen();
    KeyFuncPtr=Key_Tab[KeyFuncIndex].CurrentOperate;
}


void GUI_Key_Ok(void)
{
	switch (KeyFuncIndex)
	{
		case KEYFUNC_MAIN:
			break;
		case KEYFUNC_MENU1:
		    DispCtrlParam = g_tParam.CtrlParam;
		    if( sg_DispSubMainMenuIndex == MENUSUB_INDEX_PROG)
		    {
                DispMode = ProgMode;  // 显示模式标志位
                KeyFuncIndex = KEYFUNC_MENUCODE;
                Disp_Focus.CurrentFocus = CURRENTFOCUS_3;
			    sg_DispSetValue = 0x00;
		    }
		    else if( sg_DispSubMainMenuIndex == MENUSUB_INDEX_READ)
		    {
		        DispMode = ReadMode;  // 显示模式标志位
                KeyFuncIndex = KEYFUNC_MENU2;
		    }
			break;
		case KEYFUNC_MENU2:
			if(DispMode == ReadMode)
		    {
                GetSetOldValue();
		    }
            KeyFuncIndex = KEYFUNC_MENU3;
			break;
		case KEYFUNC_MENU3:
		    if(DispMode == ProgMode)
		    {
		        GetSetOldValue();
    			KeyFuncIndex = KEYFUNC_MENUVALUE;
    			if(sg_DispSetMenu1Index == MENU1_INDEX_VER)
    				KeyFuncIndex = KEYFUNC_MENU3;
    	    }
			break;
		case KEYFUNC_MENUVALUE:
		    if(DispMode == ProgMode)
		    {
		        if((sg_DispSetValue == SaveYes) && (sg_DispSetMenu2_3_Index == MENU2_3_INDEX_CLRE))
		        {
                    ElectricEnergy.PosEPT = 0;
                    ElectricEnergy.PosEQT = 0;
                    ElectricEnergy.NegEPT = 0;
                    ElectricEnergy.NegEQT = 0;
                    KeyFuncIndex = KEYFUNC_MENU3;
		        }
		        else
		        {
    		        KeyFuncIndex = KEYFUNC_MENU3;
    		        GetSetNewValue();
		        }
		    }
			break;
		case KEYFUNC_MENUCODE:
			if(sg_DispSetValue == g_tParam.CtrlParam.Code)
			{
				KeyFuncIndex = KEYFUNC_MENU2;
				sg_DispSetMenu1Index = MENU1_INDEX_INPT;  //密码正确 进入设置菜单
			}
			else
			{
				KeyFuncIndex = KEYFUNC_MENUCODEERR;
				g_b5DlyEnable = TRUE;
			}
			break;
		case KEYFUNC_MENUCODEERR:
		    break;
		case KEYFUNC_MENUSAVE: //数据保存更新
            if((sg_DispSetValue == SaveYes)&&(DispValueUpDataFlag == TRUE))
            {
                if((g_tParam.CtrlParam.Baud485 != DispCtrlParam.Baud485) || (g_tParam.CtrlParam.DataFormat != DispCtrlParam.DataFormat))
                {
                    g_tParam.CtrlParam.Baud485 = DispCtrlParam.Baud485;
                    g_tParam.CtrlParam.DataFormat = DispCtrlParam.DataFormat;
                    USART_Configuration();
                    PT_TimerDelay(10);
                }
                if(g_tParam.CtrlParam.NetMode != DispCtrlParam.NetMode)
                {
                    g_tParam.CtrlParam = DispCtrlParam;
                    SaveParam();
                    DispValueUpDataFlag = FALSE;
                    g_bUpdataDefault = TRUE;
                    DispMode = ReadMode;
                    PT_TimerDelay(1000);
                    NVIC_SystemReset();
                }
                g_tParam.CtrlParam = DispCtrlParam;
                SaveParam();
                DispValueUpDataFlag = FALSE;
            }
            g_bUpdataDefault = TRUE;
            DispMode = ReadMode;
		    break;
		default:
            g_bUpdataDefault = TRUE;
		    break;
	}
    Reset_Screen();
	KeyFuncPtr=Key_Tab[KeyFuncIndex].CurrentOperate;
}


void GUI_Key_MenuLong(void)  //菜单键长按 只有在主菜单模式下有用 进入模式选择
{
	if (KeyFuncIndex == KEYFUNC_MAIN)
	{
			KeyFuncIndex = KEYFUNC_MENU1;
	}
	Reset_Screen();
	KeyFuncPtr=Key_Tab[KeyFuncIndex].CurrentOperate;
}


void GUI_Key_Up(void)       //左按键 实质:减去
{
    switch (KeyFuncIndex)
	{
		case KEYFUNC_MAIN:      //在主菜单
            if(g_tParam.CtrlParam.NetMode == N34)
			{
                sg_DispMainMenuIndex--;
			}
			else
			{
                if(sg_DispMainMenuIndex == MENU_INDEX_LineU)
                {
                    sg_DispMainMenuIndex = MENU_INDEX_MAX;
                }
                else if(sg_DispMainMenuIndex == MENU_INDEX_F)
                {
                    sg_DispMainMenuIndex = MENU_INDEX_P;
                }
                else
                {
                    sg_DispMainMenuIndex--;
                }
			}
			if(sg_DispMainMenuIndex > MENU_INDEX_MAX)
			{
			    sg_DispMainMenuIndex = MENU_INDEX_MAX;
			}
			break;
		case KEYFUNC_MENU1:      //子菜单  查看/编程
			sg_DispSubMainMenuIndex--;
			if(sg_DispSubMainMenuIndex > MENUSUB_INDEX_MAX)
			{
			    sg_DispSubMainMenuIndex = MENUSUB_INDEX_MAX;
            }
			break;
		case KEYFUNC_MENU2:
            sg_DispSetMenu1Index--;
			if(DispMode == ReadMode)
			{
                if(sg_DispSetMenu1Index > MENU1_INDEX_RMAX)
			    {
			        sg_DispSetMenu1Index = MENU1_INDEX_RMAX;
                }
			}
			else
			{
                if(sg_DispSetMenu1Index > MENU1_INDEX_MAX)
			    {
			        sg_DispSetMenu1Index = MENU1_INDEX_MAX;
                }
			}
			break;
		case KEYFUNC_MENU3:
            if(sg_DispSetMenu1Index == MENU1_INDEX_INPT)
			{
				sg_DispSetMenu2_1_Index--;
				if(sg_DispSetMenu2_1_Index > MENU2_1_INDEX_MAX)
				{
				    sg_DispSetMenu2_1_Index = MENU2_1_INDEX_MAX;
                }
			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_COM1)
			{
				sg_DispSetMenu2_2_Index--;
				if(sg_DispSetMenu2_2_Index > MENU2_2_INDEX_MAX)
				{
				    sg_DispSetMenu2_2_Index = MENU2_2_INDEX_MAX;
				}
			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_SYS)
			{
                sg_DispSetMenu2_3_Index--;
                if(DispMode == ReadMode)
                {
                    if(sg_DispSetMenu2_3_Index > MENU2_3_INDEX_RMAX)
                    {
                        sg_DispSetMenu2_3_Index = MENU2_3_INDEX_RMAX;
                    }
                }
                else
                {
                    if(sg_DispSetMenu2_3_Index > MENU2_3_INDEX_MAX)
                    {
                        sg_DispSetMenu2_3_Index = MENU2_3_INDEX_MAX;
                    }
                }
			}
            else if(sg_DispSetMenu1Index == MENU1_INDEX_DO1)
			{
                if(DispCtrlParam.DO1_Mode == DO_MODE_OFF)
                {
                    sg_DispSetMenu2_4_Index = MENU2_4_INDEX_MODE;
                }
                else if(DispCtrlParam.DO1_Mode == DO_MODE_REM)
                {
                    sg_DispSetMenu2_4_Index--;
    				if(sg_DispSetMenu2_4_Index > MENU2_4_INDEX_TIME)
    				{
    				    sg_DispSetMenu2_4_Index = MENU2_4_INDEX_TIME;
                    }
                }
                else
                {
                    if((DispCtrlParam.DO1_Item >= DO_ITEM_DI1_H) && (DispCtrlParam.DO1_Item <= DO_ITEM_DI4_L))
                    {
                        sg_DispSetMenu2_4_Index--;
        				if(sg_DispSetMenu2_4_Index > MENU2_4_INDEX_ITEM)
        				{
        				    sg_DispSetMenu2_4_Index = MENU2_4_INDEX_ITEM;
                        }
                    }
                    else
                    {
                        sg_DispSetMenu2_4_Index--;
        				if(sg_DispSetMenu2_4_Index > MENU2_4_INDEX_MAX)
        				{
        				    sg_DispSetMenu2_4_Index = MENU2_4_INDEX_MAX;
                        }
                    }
                }
			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_DO2)
			{
				if(DispCtrlParam.DO2_Mode == DO_MODE_OFF)
                {
                    sg_DispSetMenu2_5_Index = MENU2_5_INDEX_MODE;
                }
                else if(DispCtrlParam.DO2_Mode == DO_MODE_REM)
                {
                    sg_DispSetMenu2_5_Index--;
    				if(sg_DispSetMenu2_5_Index > MENU2_5_INDEX_TIME)
    				{
    				    sg_DispSetMenu2_5_Index = MENU2_5_INDEX_TIME;
                    }
                }
                else
                {
                    if((DispCtrlParam.DO2_Item >= DO_ITEM_DI1_H) && (DispCtrlParam.DO2_Item <= DO_ITEM_DI4_L))
                    {
                        sg_DispSetMenu2_5_Index--;
        				if(sg_DispSetMenu2_5_Index > MENU2_5_INDEX_ITEM)
        				{
        				    sg_DispSetMenu2_5_Index = MENU2_5_INDEX_ITEM;
                        }
                    }
                    else
                    {
                        sg_DispSetMenu2_5_Index--;
        				if(sg_DispSetMenu2_5_Index > MENU2_5_INDEX_MAX)
        				{
        				    sg_DispSetMenu2_5_Index = MENU2_5_INDEX_MAX;
                        }
                    }
                }
			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_DO3)
			{
				if(DispCtrlParam.DO3_Mode == DO_MODE_OFF)
                {
                    sg_DispSetMenu2_6_Index = MENU2_6_INDEX_MODE;
                }
                else if(DispCtrlParam.DO3_Mode == DO_MODE_REM)
                {
                    sg_DispSetMenu2_6_Index--;
    				if(sg_DispSetMenu2_6_Index > MENU2_6_INDEX_TIME)
    				{
    				    sg_DispSetMenu2_6_Index = MENU2_6_INDEX_TIME;
                    }
                }
                else
                {
                    if((DispCtrlParam.DO3_Item >= DO_ITEM_DI1_H) && (DispCtrlParam.DO3_Item <= DO_ITEM_DI4_L))
                    {
                        sg_DispSetMenu2_6_Index--;
        				if(sg_DispSetMenu2_6_Index > MENU2_6_INDEX_ITEM)
        				{
        				    sg_DispSetMenu2_6_Index = MENU2_6_INDEX_ITEM;
                        }
                    }
                    else
                    {
                        sg_DispSetMenu2_6_Index--;
        				if(sg_DispSetMenu2_6_Index > MENU2_6_INDEX_MAX)
        				{
        				    sg_DispSetMenu2_6_Index = MENU2_6_INDEX_MAX;
                        }
                    }
                }
			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_DO4)
			{
				if(DispCtrlParam.DO4_Mode == DO_MODE_OFF)
                {
                    sg_DispSetMenu2_7_Index = MENU2_7_INDEX_MODE;
                }
                else if(DispCtrlParam.DO4_Mode == DO_MODE_REM)
                {
                    sg_DispSetMenu2_7_Index--;
    				if(sg_DispSetMenu2_7_Index > MENU2_7_INDEX_TIME)
    				{
    				    sg_DispSetMenu2_7_Index = MENU2_7_INDEX_TIME;
                    }
                }
                else
                {
                    if((DispCtrlParam.DO4_Item >= DO_ITEM_DI1_H) && (DispCtrlParam.DO4_Item <= DO_ITEM_DI4_L))
                    {
                        sg_DispSetMenu2_7_Index--;
        				if(sg_DispSetMenu2_7_Index > MENU2_7_INDEX_ITEM)
        				{
        				    sg_DispSetMenu2_7_Index = MENU2_7_INDEX_ITEM;
                        }
                    }
                    else
                    {
                        sg_DispSetMenu2_7_Index--;
        				if(sg_DispSetMenu2_7_Index > MENU2_7_INDEX_MAX)
        				{
        				    sg_DispSetMenu2_7_Index = MENU2_7_INDEX_MAX;
                        }
                    }
                }
			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_AO1)
			{
			    if(DispCtrlParam.AO1_Mode == AO_MODE_OFF)
			    {
                    sg_DispSetMenu2_8_Index = AO_MODE_OFF;
			    }
			    else
			    {
                    sg_DispSetMenu2_8_Index--;
    				if(sg_DispSetMenu2_8_Index > MENU2_8_INDEX_MAX)
    				{
    				    sg_DispSetMenu2_8_Index = MENU2_8_INDEX_MAX;
                    }
			    }
			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_AO2)
			{
			    if(DispCtrlParam.AO2_Mode == AO_MODE_OFF)
			    {
                    sg_DispSetMenu2_9_Index = AO_MODE_OFF;
			    }
			    else
			    {
                    sg_DispSetMenu2_9_Index--;
    				if(sg_DispSetMenu2_9_Index > MENU2_9_INDEX_MAX)
    				{
    				    sg_DispSetMenu2_9_Index = MENU2_9_INDEX_MAX;
                    }
			    }
			}
			if(DispMode == ReadMode)
			{
                GetSetOldValue();
			}
			break;
		case KEYFUNC_MENUVALUE://设置值
		    if(DispMode == ProgMode)
		    {
		        switch(sg_DispSetMenu1Index)
		        {
                    case MENU1_INDEX_INPT:
                        if(sg_DispSetMenu2_1_Index == MENU2_1_INDEX_NET)
        		        {
                            UpdataOrSetValue(Select_Net,UPDATA_ADD);
        		        }
                        else
                        {
                            if(Disp_Focus.CurrentFocus == CURRENTFOCUS_0)
                            {
                                Disp_Focus.CurrentFocus = CURRENTFOCUS_3;
                            }
                    		else
                            {
                                Disp_Focus.CurrentFocus = Disp_Focus.CurrentFocus >> 1;
                            }
                        }
                        break;
                    case MENU1_INDEX_COM1:
                        if(sg_DispSetMenu2_2_Index == MENU2_2_INDEX_BAUD)
                        {
                            UpdataOrSetValue(Select_Baud,UPDATA_SUB);
                        }
                        else if(sg_DispSetMenu2_2_Index == MENU2_2_INDEX_DATA)
                        {
                            UpdataOrSetValue(Select_Data,UPDATA_SUB);
                        }
                        else if(sg_DispSetMenu2_2_Index == MENU2_2_INDEX_COMS)
                        {
                            UpdataOrSetValue(Select_Coms,UPDATA_SUB);
                        }
                        else
                        {
                            if(Disp_Focus.CurrentFocus == CURRENTFOCUS_0)
                            {
                                Disp_Focus.CurrentFocus = CURRENTFOCUS_3;
                            }
                    		else
                            {
                                Disp_Focus.CurrentFocus = Disp_Focus.CurrentFocus >> 1;
                            }
                        }
                        break;

                    case MENU1_INDEX_SYS:
        		        switch(sg_DispSetMenu2_3_Index)
        		        {
                            case MENU2_3_INDEX_CYC:
                                UpdataOrSetValue(Select_Save,UPDATA_SUB);
                                break;
                            case MENU2_3_INDEX_DISP:
                                UpdataOrSetValue(Select_Disp_Default,UPDATA_SUB);
                                break;
                            case MENU2_3_INDEX_CLRE:
                                UpdataOrSetValue(Select_Save,UPDATA_SUB);
                                break;
                            default:
                                if(Disp_Focus.CurrentFocus == CURRENTFOCUS_0)
                                {
                                    Disp_Focus.CurrentFocus = CURRENTFOCUS_3;
                                }
                        		else
                                {
                                    Disp_Focus.CurrentFocus = Disp_Focus.CurrentFocus >> 1;
                                }
                                break;
        		        }
        		        break;
        		    case MENU1_INDEX_DO1:
        		        switch(sg_DispSetMenu2_4_Index)
        		        {
                            case MENU2_4_INDEX_MODE:
                                UpdataOrSetValue(Select_Do_Mode,UPDATA_SUB);
                                break;
                            case MENU2_4_INDEX_ITEM:
                                UpdataOrSetValue(Select_Do_item,UPDATA_SUB);
                                break;
                            default:
                                if(Disp_Focus.CurrentFocus == CURRENTFOCUS_0)
                                {
                                    Disp_Focus.CurrentFocus = CURRENTFOCUS_3;
                                }
                        		else
                                {
                                    Disp_Focus.CurrentFocus = Disp_Focus.CurrentFocus >> 1;
                                }
                                break;
        		        }
        		        break;
        		    case MENU1_INDEX_DO2:
        		        switch(sg_DispSetMenu2_5_Index)
        		        {
                            case MENU2_5_INDEX_MODE:
                                UpdataOrSetValue(Select_Do_Mode,UPDATA_SUB);
                                break;
                            case MENU2_5_INDEX_ITEM:
                                UpdataOrSetValue(Select_Do_item,UPDATA_SUB);
                                break;
                            default:
                                if(Disp_Focus.CurrentFocus == CURRENTFOCUS_0)
                                {
                                    Disp_Focus.CurrentFocus = CURRENTFOCUS_3;
                                }
                        		else
                                {
                                    Disp_Focus.CurrentFocus = Disp_Focus.CurrentFocus >> 1;
                                }
                                break;
        		        }
        		        break;
        		    case MENU1_INDEX_DO3:
        		        switch(sg_DispSetMenu2_6_Index)
        		        {
                            case MENU2_6_INDEX_MODE:
                                UpdataOrSetValue(Select_Do_Mode,UPDATA_SUB);
                                break;
                            case MENU2_6_INDEX_ITEM:
                                UpdataOrSetValue(Select_Do_item,UPDATA_SUB);
                                break;
                            default:
                                if(Disp_Focus.CurrentFocus == CURRENTFOCUS_0)
                                {
                                    Disp_Focus.CurrentFocus = CURRENTFOCUS_3;
                                }
                        		else
                                {
                                    Disp_Focus.CurrentFocus = Disp_Focus.CurrentFocus >> 1;
                                }
                                break;
        		        }
        		        break;
        		    case MENU1_INDEX_DO4:
        		        switch(sg_DispSetMenu2_7_Index)
        		        {
                            case MENU2_7_INDEX_MODE:
                                UpdataOrSetValue(Select_Do_Mode,UPDATA_SUB);
                                break;
                            case MENU2_7_INDEX_ITEM:
                                UpdataOrSetValue(Select_Do_item,UPDATA_SUB);
                                break;
                            default:
                                if(Disp_Focus.CurrentFocus == CURRENTFOCUS_0)
                                {
                                    Disp_Focus.CurrentFocus = CURRENTFOCUS_3;
                                }
                        		else
                                {
                                    Disp_Focus.CurrentFocus = Disp_Focus.CurrentFocus >> 1;
                                }
                                break;
        		        }
        		        break;
        		    case MENU1_INDEX_AO1:
        		        switch(sg_DispSetMenu2_8_Index)
        		        {
                            case MENU2_8_INDEX_MODE:
                                UpdataOrSetValue(Select_Ao_Mode,UPDATA_SUB);
                                break;
                            case MENU2_8_INDEX_ITEM:
                                UpdataOrSetValue(Select_Ao_item,UPDATA_SUB);
                                break;
                            default:
                                if(Disp_Focus.CurrentFocus == CURRENTFOCUS_0)
                                {
                                    Disp_Focus.CurrentFocus = CURRENTFOCUS_3;
                                }
                        		else
                                {
                                    Disp_Focus.CurrentFocus = Disp_Focus.CurrentFocus >> 1;
                                }
                                break;
        		        }
        		        break;
        		    case MENU1_INDEX_AO2:
        		        switch(sg_DispSetMenu2_9_Index)
        		        {
                            case MENU2_9_INDEX_MODE:
                                UpdataOrSetValue(Select_Ao_Mode,UPDATA_SUB);
                                break;
                            case MENU2_9_INDEX_ITEM:
                                UpdataOrSetValue(Select_Ao_item,UPDATA_SUB);
                                break;
                            default:
                                if(Disp_Focus.CurrentFocus == CURRENTFOCUS_0)
                                {
                                    Disp_Focus.CurrentFocus = CURRENTFOCUS_3;
                                }
                        		else
                                {
                                    Disp_Focus.CurrentFocus = Disp_Focus.CurrentFocus >> 1;
                                }
                                break;
        		        }
        		    default:
        		        break;
                }
    		}
            break;
		case KEYFUNC_MENUCODE:
			if(Disp_Focus.CurrentFocus == CURRENTFOCUS_0)
            {
                Disp_Focus.CurrentFocus = CURRENTFOCUS_3;
            }
    		else
            {
                Disp_Focus.CurrentFocus = Disp_Focus.CurrentFocus >> 1;
            }
			break;
		case KEYFUNC_MENUCODEERR:
		    break;
    	case KEYFUNC_MENUSAVE:
    	    UpdataOrSetValue(Select_Save,UPDATA_ADD);
    	    break;
	    default: //按键错误的处理
            g_bUpdataDefault = TRUE;
    		break;
	}
	Reset_Screen();
	KeyFuncPtr=Key_Tab[KeyFuncIndex].CurrentOperate;
}

void GUI_Key_Down(void)  //右按键
{
    switch (KeyFuncIndex)
	{
		case KEYFUNC_MAIN:      //在主菜单 电流/频率
			if(g_tParam.CtrlParam.NetMode == N34)
			{
			    sg_DispMainMenuIndex++;
            }
			else
			{
                if(sg_DispMainMenuIndex == MENU_INDEX_MAX)
                {
                    sg_DispMainMenuIndex = MENU_INDEX_LineU;
                }
                else if(sg_DispMainMenuIndex == MENU_INDEX_P)
                {
                    sg_DispMainMenuIndex = MENU_INDEX_F;
                }
                else
                {
                    sg_DispMainMenuIndex++;
                }
			}
			if(sg_DispMainMenuIndex > MENU_INDEX_MAX)
			{
			    sg_DispMainMenuIndex = MENU_INDEX_MIN;
			}
			break;
	    case KEYFUNC_MENU1:
			sg_DispSubMainMenuIndex++;
			if(sg_DispSubMainMenuIndex > MENUSUB_INDEX_MAX)
			{
			    sg_DispSubMainMenuIndex = MENUSUB_INDEX_MIN;
			}
			break;
		case KEYFUNC_MENU2:
			sg_DispSetMenu1Index++;
			if(DispMode == ReadMode)
			{
                if(sg_DispSetMenu1Index > MENU1_INDEX_RMAX)
			    {
			        sg_DispSetMenu1Index = MENU1_INDEX_MIN;
                }
			}
			else
			{
                if(sg_DispSetMenu1Index > MENU1_INDEX_MAX)
			    {
			        sg_DispSetMenu1Index = MENU1_INDEX_MIN;
                }
			}
			break;
		case KEYFUNC_MENU3:
			if(sg_DispSetMenu1Index == MENU1_INDEX_INPT)
			{
				sg_DispSetMenu2_1_Index++;
				if(sg_DispSetMenu2_1_Index > MENU2_1_INDEX_MAX)
				{
				    sg_DispSetMenu2_1_Index = MENU2_1_INDEX_MIN;
                }
			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_COM1)
			{
				sg_DispSetMenu2_2_Index++;
				if(sg_DispSetMenu2_2_Index > MENU2_2_INDEX_MAX)
				{
				    sg_DispSetMenu2_2_Index = MENU2_2_INDEX_MIN;
				}
			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_SYS)
			{
                sg_DispSetMenu2_3_Index++;
                if(DispMode == ReadMode)
                {
                    if(sg_DispSetMenu2_3_Index > MENU2_3_INDEX_RMAX)
                    {
                        sg_DispSetMenu2_3_Index = MENU2_3_INDEX_MIN;
                    }
                }
                else
                {
                    if(sg_DispSetMenu2_3_Index > MENU2_3_INDEX_MAX)
                    {
                        sg_DispSetMenu2_3_Index = MENU2_3_INDEX_MIN;
                    }
                }

			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_DO1)
			{
                if(DispCtrlParam.DO1_Mode == DO_MODE_OFF)
                {
                    sg_DispSetMenu2_4_Index = MENU2_4_INDEX_MODE;
                }
                else if(DispCtrlParam.DO1_Mode == DO_MODE_REM)
                {
                    sg_DispSetMenu2_4_Index++;
    				if(sg_DispSetMenu2_4_Index > MENU2_4_INDEX_TIME)
    				{
    				    sg_DispSetMenu2_4_Index = MENU2_4_INDEX_MIN;
                    }
                }
                else
                {
                    if((DispCtrlParam.DO1_Item >= DO_ITEM_DI1_H) && (DispCtrlParam.DO1_Item <= DO_ITEM_DI4_L))
                    {
                        sg_DispSetMenu2_4_Index++;
        				if(sg_DispSetMenu2_4_Index > MENU2_4_INDEX_ITEM)
        				{
        				    sg_DispSetMenu2_4_Index = MENU2_4_INDEX_MIN;
        				}
                    }
                    else
                    {
                        sg_DispSetMenu2_4_Index++;
        				if(sg_DispSetMenu2_4_Index > MENU2_4_INDEX_MAX)
        				{
        				    sg_DispSetMenu2_4_Index = MENU2_4_INDEX_MIN;
        				}
                    }
                }
			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_DO2)
			{
				if(DispCtrlParam.DO2_Mode == DO_MODE_OFF)
                {
                    sg_DispSetMenu2_5_Index = MENU2_5_INDEX_MODE;
                }
                else if(DispCtrlParam.DO2_Mode == DO_MODE_REM)
                {
                    sg_DispSetMenu2_5_Index++;
    				if(sg_DispSetMenu2_5_Index > MENU2_5_INDEX_TIME)
    				{
    				    sg_DispSetMenu2_5_Index = MENU2_5_INDEX_MIN;
                    }
                }
                else
                {
                    if((DispCtrlParam.DO2_Item >= DO_ITEM_DI1_H) && (DispCtrlParam.DO2_Item <= DO_ITEM_DI4_L))
                    {
                        sg_DispSetMenu2_5_Index++;
        				if(sg_DispSetMenu2_5_Index > MENU2_5_INDEX_ITEM)
        				{
        				    sg_DispSetMenu2_5_Index = MENU2_5_INDEX_MIN;
        				}
                    }
                    else
                    {
                        sg_DispSetMenu2_5_Index++;
        				if(sg_DispSetMenu2_5_Index > MENU2_5_INDEX_MAX)
        				{
        				    sg_DispSetMenu2_5_Index = MENU2_5_INDEX_MIN;
        				}
                    }
                }
			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_DO3)
			{
				if(DispCtrlParam.DO3_Mode == DO_MODE_OFF)
                {
                    sg_DispSetMenu2_6_Index = MENU2_6_INDEX_MODE;
                }
                else if(DispCtrlParam.DO3_Mode == DO_MODE_REM)
                {
                    sg_DispSetMenu2_6_Index++;
    				if(sg_DispSetMenu2_6_Index > MENU2_6_INDEX_TIME)
    				{
    				    sg_DispSetMenu2_6_Index = MENU2_6_INDEX_MIN;
                    }
                }
                else
                {
                    if((DispCtrlParam.DO3_Item >= DO_ITEM_DI1_H) && (DispCtrlParam.DO3_Item <= DO_ITEM_DI4_L))
                    {
                        sg_DispSetMenu2_6_Index++;
        				if(sg_DispSetMenu2_6_Index > MENU2_6_INDEX_ITEM)
        				{
        				    sg_DispSetMenu2_6_Index = MENU2_6_INDEX_MIN;
        				}
                    }
                    else
                    {
                        sg_DispSetMenu2_6_Index++;
        				if(sg_DispSetMenu2_6_Index > MENU2_6_INDEX_MAX)
        				{
        				    sg_DispSetMenu2_6_Index = MENU2_6_INDEX_MIN;
        				}
                    }
                }
			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_DO4)
			{
				if(DispCtrlParam.DO4_Mode == DO_MODE_OFF)
                {
                    sg_DispSetMenu2_7_Index = MENU2_7_INDEX_MODE;
                }
                else if(DispCtrlParam.DO4_Mode == DO_MODE_REM)
                {
                    sg_DispSetMenu2_7_Index++;
    				if(sg_DispSetMenu2_7_Index > MENU2_7_INDEX_TIME)
    				{
    				    sg_DispSetMenu2_7_Index = MENU2_7_INDEX_MIN;
                    }
                }
                else
                {
                    if((DispCtrlParam.DO4_Item >= DO_ITEM_DI1_H) && (DispCtrlParam.DO4_Item <= DO_ITEM_DI4_L))
                    {
                        sg_DispSetMenu2_7_Index++;
        				if(sg_DispSetMenu2_7_Index > MENU2_7_INDEX_ITEM)
        				{
        				    sg_DispSetMenu2_7_Index = MENU2_7_INDEX_MIN;
        				}
                    }
                    else
                    {
                        sg_DispSetMenu2_7_Index++;
        				if(sg_DispSetMenu2_7_Index > MENU2_7_INDEX_MAX)
        				{
        				    sg_DispSetMenu2_7_Index = MENU2_7_INDEX_MIN;
        				}
                    }
                }
			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_AO1)
			{
			    if(DispCtrlParam.AO1_Mode == AO_MODE_OFF)
			    {
                    sg_DispSetMenu2_8_Index = AO_MODE_OFF;
			    }
			    else
			    {
                    sg_DispSetMenu2_8_Index++;
    				if(sg_DispSetMenu2_8_Index > MENU2_8_INDEX_MAX)
    				{
    				    sg_DispSetMenu2_8_Index = MENU2_8_INDEX_MIN;
    				}
			    }
			}
			else if(sg_DispSetMenu1Index == MENU1_INDEX_AO2)
			{
				if(DispCtrlParam.AO2_Mode == AO_MODE_OFF)
			    {
                    sg_DispSetMenu2_9_Index = AO_MODE_OFF;
			    }
			    else
			    {
                    sg_DispSetMenu2_9_Index++;
    				if(sg_DispSetMenu2_9_Index > MENU2_9_INDEX_MAX)
    				{
    				    sg_DispSetMenu2_9_Index = MENU2_9_INDEX_MIN;
    				}
			    }
			}
			if(DispMode == ReadMode)
			{
                GetSetOldValue();
			}
			break;
		case KEYFUNC_MENUVALUE://设置值
		    if(DispMode == ProgMode)
		    {
		        switch(sg_DispSetMenu1Index)
		        {
                    case MENU1_INDEX_INPT:
                        switch(sg_DispSetMenu2_1_Index)
                        {
                            case MENU2_1_INDEX_NET:
                                UpdataOrSetValue(Select_Net,UPDATA_ADD);
                                break;
                            case MENU2_1_INDEX_PT:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_1_INDEX_CT:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            default:
                        		break;
                        }
                        break;
                    case MENU1_INDEX_COM1:
                        switch(sg_DispSetMenu2_2_Index)
                        {
                            case MENU2_2_INDEX_ADD:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_2_INDEX_BAUD:
                                UpdataOrSetValue(Select_Baud,UPDATA_ADD);
                                break;
                            case MENU2_2_INDEX_DATA:
                                UpdataOrSetValue(Select_Data,UPDATA_ADD);
                                break;
                            case MENU2_2_INDEX_COMS:
                                UpdataOrSetValue(Select_Coms,UPDATA_ADD);
                                break;
                            default:
                        		break;
                        }
                        break;
                    case MENU1_INDEX_SYS:
                        switch(sg_DispSetMenu2_3_Index)
                        {
                            case MENU2_3_INDEX_CYC:
                                UpdataOrSetValue(Select_Save,UPDATA_ADD);
                                break;
                            case MENU2_3_INDEX_LIGH:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_3_INDEX_DISP:
                                UpdataOrSetValue(Select_Disp_Default,UPDATA_ADD);
                                break;
                            case MENU2_3_INDEX_ALR:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_3_INDEX_CLRE:
                                UpdataOrSetValue(Select_Save,UPDATA_ADD);
                                break;
                            case MENU2_3_INDEX_CODE:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            default:
                        		break;
                        }
                        break;
                    case MENU1_INDEX_DO1:
                        switch(sg_DispSetMenu2_4_Index)
                        {
                            case MENU2_4_INDEX_MODE:
                                UpdataOrSetValue(Select_Do_Mode,UPDATA_ADD);
                                break;
                            case MENU2_4_INDEX_TIME:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_4_INDEX_ITEM:
                                UpdataOrSetValue(Select_Do_item,UPDATA_ADD);
                                break;
                            case MENU2_4_INDEX_UAL:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_4_INDEX_HYS:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_4_INDEX_DELAY:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            default:
                                break;
                        }
                        break;
                    case MENU1_INDEX_DO2:
                        switch(sg_DispSetMenu2_5_Index)
                        {
                            case MENU2_5_INDEX_MODE:
                                UpdataOrSetValue(Select_Do_Mode,UPDATA_ADD);
                                break;
                            case MENU2_5_INDEX_TIME:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_5_INDEX_ITEM:
                                UpdataOrSetValue(Select_Do_item,UPDATA_ADD);
                                break;
                            case MENU2_5_INDEX_UAL:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_5_INDEX_HYS:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_5_INDEX_DELAY:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            default:
                                break;
                        }
                        break;
                    case MENU1_INDEX_DO3:
                        switch(sg_DispSetMenu2_6_Index)
                        {
                            case MENU2_6_INDEX_MODE:
                                UpdataOrSetValue(Select_Do_Mode,UPDATA_ADD);
                                break;
                            case MENU2_6_INDEX_TIME:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_6_INDEX_ITEM:
                                UpdataOrSetValue(Select_Do_item,UPDATA_ADD);
                                break;
                            case MENU2_6_INDEX_UAL:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_6_INDEX_HYS:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_6_INDEX_DELAY:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            default:
                                break;
                        }
                        break;
                    case MENU1_INDEX_DO4:
                        switch(sg_DispSetMenu2_7_Index)
                        {
                            case MENU2_7_INDEX_MODE:
                                UpdataOrSetValue(Select_Do_Mode,UPDATA_ADD);
                                break;
                            case MENU2_7_INDEX_TIME:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_7_INDEX_ITEM:
                                UpdataOrSetValue(Select_Do_item,UPDATA_ADD);
                                break;
                            case MENU2_7_INDEX_UAL:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_7_INDEX_HYS:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_7_INDEX_DELAY:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            default:
                                break;
                        }
                        break;
                    case MENU1_INDEX_AO1:
                        switch(sg_DispSetMenu2_8_Index)
                        {
                            case MENU2_8_INDEX_MODE:
                                UpdataOrSetValue(Select_Ao_Mode,UPDATA_ADD);
                                break;
                            case MENU2_8_INDEX_ITEM:
                                UpdataOrSetValue(Select_Ao_item,UPDATA_ADD);
                                break;
                            case MENU2_8_INDEX_DS:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_8_INDEX_FS:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            default:
                                break;
                        }
                        break;
                    case MENU1_INDEX_AO2:
                        switch(sg_DispSetMenu2_9_Index)
                        {
                            case MENU2_9_INDEX_MODE:
                                UpdataOrSetValue(Select_Ao_Mode,UPDATA_ADD);
                                break;
                            case MENU2_9_INDEX_ITEM:
                                UpdataOrSetValue(Select_Ao_item,UPDATA_ADD);
                                break;
                            case MENU2_9_INDEX_DS:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            case MENU2_9_INDEX_FS:
                                UpdataSetValue(UPDATA_ADD);
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
		        }
    		}
            break;
		case KEYFUNC_MENUCODE:
			UpdataSetValue(UPDATA_ADD);
			break;
		case KEYFUNC_MENUCODEERR:
		    break;
    	case KEYFUNC_MENUSAVE:
    	    UpdataOrSetValue(Select_Save,UPDATA_SUB);
            break;
        default: //按键错误的处理
            g_bUpdataDefault = TRUE;
    		break;
	}
    Reset_Screen();
	KeyFuncPtr=Key_Tab[KeyFuncIndex].CurrentOperate;
}


void GUI_Key_Default(u8 Key)
{
    // 默认电流界面
	if ((Key == KEYSTATUS_NONE && g_bInitDefault) || g_bUpdataDefault)
	{
		g_bInitDefault = FALSE;
        KeyFuncIndex = KEYFUNC_MAIN;
        switch(g_tParam.CtrlParam.DefaultDisp)
        {
            case DISP_UINT_U:
                if(g_tParam.CtrlParam.NetMode == N34)
                {
                    sg_DispMainMenuIndex = MENU_INDEX_U;
                }
                else if(g_tParam.CtrlParam.NetMode == N33)
                {
                    sg_DispMainMenuIndex = MENU_INDEX_LineU;
                }
                break;
            case DISP_UINT_I:
                sg_DispMainMenuIndex = MENU_INDEX_I;
                break;
            case DISP_UINT_P:
                sg_DispMainMenuIndex = MENU_INDEX_P;
                break;
            case DISP_UINT_PF:
                sg_DispMainMenuIndex = MENU_INDEX_PF;
                break;
            case DISP_UINT_EP:
                sg_DispMainMenuIndex = MENU_INDEX_PosEPT;
                break;
            case DISP_UINT_THD:
                sg_DispMainMenuIndex = MENU_INDEX_HarmU;
                break;
            //case DISP_UINT_DEM:
                //sg_DispMainMenuIndex = MENU_INDEX_;
                //break;
            default:
                sg_DispMainMenuIndex = MENU_INDEX_U;
                break;
        }
		sg_DispSetMenu1Index = MENU1_INDEX_INPT;
		sg_DispSetMenu2_1_Index = MENU2_1_INDEX_MIN;
		sg_DispSetMenu2_2_Index = MENU2_2_INDEX_ADD;
		sg_DispSetMenu2_3_Index = MENU2_3_INDEX_CYC;
		sg_DispSetMenu2_4_Index = MENU2_4_INDEX_MODE;
		sg_DispSetMenu2_5_Index = MENU2_5_INDEX_MODE;
		sg_DispSetMenu2_6_Index = MENU2_6_INDEX_MODE;
		sg_DispSetMenu2_7_Index = MENU2_7_INDEX_MODE;
		Reset_Screen();
		KeyFuncPtr=Key_Tab[KeyFuncIndex].CurrentOperate;
	}

}


void GUI_Main(void)
{
    u8 Key = KeyDiff();

    switch(Key)
	{
    	case KEYSTATUS_UP:      // 向上键
    	{
    	    g_bRestoreDlyEnable = FALSE;
    	    g_bUpdataDefault = FALSE;
    	    g_bBlackLightDefault = FALSE;
    	    GUI_Key_Up();
    		break;
    	}
    	case KEYSTATUS_DOWN:    // 向下键
    	{
    	    g_bRestoreDlyEnable = FALSE;
    	    g_bUpdataDefault = FALSE;
    	    g_bBlackLightDefault = FALSE;
    	    GUI_Key_Down();
    		break;
    	}
    	case KEYSTATUS_MENU:    // 向左键 MENU
    	{
    	    g_bRestoreDlyEnable = FALSE;
    	    g_bUpdataDefault = FALSE;
    	    g_bBlackLightDefault = FALSE;

    	    GUI_Key_Menu();
    		break;
        }
    	case KEYSTATUS_OK:      // 向右键 确定键
    	{
    	    g_bRestoreDlyEnable = FALSE;
    	    g_bUpdataDefault = FALSE;
    	    g_bBlackLightDefault = FALSE;
    	    GUI_Key_Ok();
    		break;
        }
    	case KEYSTATUS_MENULONG: //MENU 长按键
    	{
    	    g_bRestoreDlyEnable = FALSE;
            g_bUpdataDefault = FALSE;
            g_bBlackLightDefault = FALSE;
    	    GUI_Key_MenuLong();
    		break;
        }
    	//---------------------------------------------------------------------------
    	//---------------------------------------------------------------------------
    	default: //按键错误的处理
    	    g_bRestoreDlyEnable = TRUE;
    	    g_bBlackLightDefault = TRUE;
    		break;
    }
    GUI_Key_Default(Key);

	if (KeyFuncPtr != NULL)
	{
		(*KeyFuncPtr)();//执行当前按键的操作
	}

}

void Task_GUI_Function(void)
{
	GUI_Main();
	HT_Write_Four_Quadrant(TRUE);
	Disp_IO_Status(DinStatus_Disp,OutStatus_Disp);
    Lcd_Update_screen();
    HT_LedStatus(g_bBlackLightEnble);
	//DinHandler();
}


void Disp_Set_FU(u8 Index, u8 *pFuData)
{

}



/**
  * @brief	显示电压值
  * @param	s16 DispNum-要显示的数字
			u8 Decimal-小数点位数
			u8 DispWidth-显示宽度(几位数)
			BOOL bFlash-是否闪烁
			u8 FlashDec-闪烁的位数
			BOOL bEnableDisp-强制显示全部位
  * @retval None
  * 增加分类显示
  */

void DispValueVoltage(u8 num,s32 DispNum, u8 Decimal, u8 DispWidth, BOOL bFlash, u8 FlashDec, BOOL bEnableDisp, u8 NumLeve)
{
    if(DispNum < 10000)
    {
        if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 2;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 3;
        }
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 1;
        }
        else if(NumLeve & 0x04)
        {
            DispNum = DispNum * 0.01;
            Decimal = 2;
        }
#if(FUNCTION_VOLTAGE == 1)
        else if(NumLeve & 0x02)
        {
            DispNum = DispNum * 0.1;
            Decimal = 0;
        }
#else
        else if(NumLeve & 0x02)
        {
            DispNum = DispNum * 0.1;
            Decimal = 3;
        }
#endif
        else if( NumLeve & 0x01 )
        {
            DispNum = DispNum;
            Decimal = 1;
        }
    }
    else if((DispNum >= 10000) && (DispNum < 100000))
    {
        if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 2;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 3;
        }
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 1;
        }
        else if(NumLeve & 0x04)
        {
            DispNum = DispNum * 0.01;
            Decimal = 2;
        }
#if(FUNCTION_VOLTAGE == 1)
        else if(NumLeve & 0x02)
        {
            DispNum = DispNum * 0.1;
            Decimal = 0;
        }
#else
        else if(NumLeve & 0x02)
        {
            DispNum = DispNum * 0.1;
            Decimal = 3;
        }
#endif
    }
    else if((DispNum >= 100000) && (DispNum < 1000000))  //超电压范围
    {
        if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 2;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 3;
        }
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 1;
        }
        else if(NumLeve & 0x04)
        {
            DispNum = DispNum * 0.01;
            Decimal = 2;
        }
    }
    else if((DispNum >= 1000000) && (DispNum < 10000000))
    {
        if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 2;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 3;
        }
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 1;
        }
    }
    else if((DispNum >= 10000000) && (DispNum < 100000000))
    {
        if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 2;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 3;
        }
    }
    else if((DispNum >= 100000000) && (DispNum < 1000000000))
    {
        if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 2;
        }
    }
    DispValue(num,DispNum,Decimal, 4, FALSE,0, TRUE, FALSE);
}



/**
  * @brief	显示电流值
  * @param	s16 DispNum-要显示的数字
			u8 Decimal-小数点位数
			u8 DispWidth-显示宽度(几位数)
			BOOL bFlash-是否闪烁
			u8 FlashDec-闪烁的位数
			BOOL bEnableDisp-强制显示全部位
  * @retval None
  * 增加分类显示
  */

void DispValueCurrent(u8 num,s32 DispNum, u8 Decimal, u8 DispWidth, BOOL bFlash, u8 FlashDec, BOOL bEnableDisp, u8 NumLeve)
{
    if(DispNum < 10000)
    {
        if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 1;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 2;
        }
#if(FUNCTION_CURRENT == 1)
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 0;
        }
#else
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 3;
        }
#endif
        else if(NumLeve & 0x04)
        {
            DispNum = DispNum * 0.01;
            Decimal = 1;
        }
        else if(NumLeve & 0x02)
        {
            DispNum = DispNum * 0.1;
            Decimal = 2;
        }
        else if( NumLeve & 0x01 )
        {
            DispNum = DispNum;
            Decimal = 3;
        }
    }
    else if((DispNum >= 10000) && (DispNum < 100000))
    {
        if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 1;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 2;
        }
#if(FUNCTION_CURRENT == 1)
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 0;
        }
#else
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 3;
        }
#endif
        else if(NumLeve & 0x04)
        {
            DispNum = DispNum * 0.01;
            Decimal = 1;
        }
        else if(NumLeve & 0x02)
        {
            DispNum = DispNum * 0.1;
            Decimal = 2;
        }
    }
    else if((DispNum >= 100000) && (DispNum < 1000000))
    {
        if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 1;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 2;
        }
#if(FUNCTION_CURRENT == 1)
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 0;
        }
#else
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 3;
        }
#endif
        else if(NumLeve & 0x04)
        {
            DispNum = DispNum * 0.01;
            Decimal = 1;
        }
    }
    else if((DispNum >= 1000000) && (DispNum < 10000000))
    {
        if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 1;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 2;
        }
#if(FUNCTION_CURRENT == 1)
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 0;
        }
#else
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 3;
        }
#endif
    }
    else if((DispNum >= 10000000) && (DispNum < 100000000))
    {
        if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 1;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 2;
        }
    }
    else if((DispNum >= 100000000) && (DispNum < 1000000000))
    {
        if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 1;
        }
    }
    DispValue(num,DispNum,Decimal, 4, FALSE,0, TRUE, FALSE);
}


/**
  * @brief	显示功率值
  * @param	s16 DispNum-要显示的数字
			u8 Decimal-小数点位数
			u8 DispWidth-显示宽度(几位数)
			BOOL bFlash-是否闪烁
			u8 FlashDec-闪烁的位数
			BOOL bEnableDisp-强制显示全部位
  * @retval None
  * 增加分类显示
  */

void DispValuePower(u8 num,s64 DispNum, u8 Decimal, u8 DispWidth, BOOL bFlash, u8 FlashDec, BOOL bEnableDisp, u16 NumLeve)
{
    if(DispNum < 10000)  // 123.4w
    {
        if(NumLeve & 0x0200)
        {
            DispNum = DispNum * 0.000000001;
            Decimal = 1;
        }
        else if( NumLeve & 0x0100)
        {
            DispNum = DispNum * 0.00000001;
            Decimal = 2;
        }
        else if( NumLeve & 0x80)
        {
            DispNum = DispNum * 0.0000001;
            Decimal = 0;
        }
        else if(NumLeve & 0x40 )
        {
            DispNum = DispNum * 0.000001;
            Decimal = 1;
        }
        else if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 2;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 3;
        }
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 1;
        }
        else if(NumLeve & 0x04)
        {
            DispNum = DispNum * 0.01;
            Decimal = 2;
        }
        else if(NumLeve & 0x02)
        {
            DispNum = DispNum * 0.1;
            Decimal = 3;
        }
        else if( NumLeve & 0x01 )
        {
            DispNum = (DispNum * 0.1);
            Decimal = 0;
        }

    }
    else if((DispNum >= 10000) && (DispNum < 100000)) // 1.234 KW
    {
        if(NumLeve & 0x0200)
        {
            DispNum = DispNum * 0.000000001;
            Decimal = 1;
        }
        else if( NumLeve & 0x0100)
        {
            DispNum = DispNum * 0.00000001;
            Decimal = 2;
        }
        else if( NumLeve & 0x80)
        {
            DispNum = DispNum * 0.0000001;
            Decimal = 0;
        }
        else if(NumLeve & 0x40 )
        {
            DispNum = DispNum * 0.000001;
            Decimal = 1;
        }
        else if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 2;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 3;
        }
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 1;
        }
        else if(NumLeve & 0x04)
        {
            DispNum = DispNum * 0.01;
            Decimal = 2;
        }
        else if(NumLeve & 0x02)
        {
            DispNum = DispNum * 0.1;
            Decimal = 3;
        }
    }
    else if((DispNum >= 100000) && (DispNum < 1000000)) // 12.34 KW
    {
        if(NumLeve & 0x0200)
        {
            DispNum = DispNum * 0.000000001;
            Decimal = 1;
        }
        else if( NumLeve & 0x0100)
        {
            DispNum = DispNum * 0.00000001;
            Decimal = 2;
        }
        else if( NumLeve & 0x80)
        {
            DispNum = DispNum * 0.0000001;
            Decimal = 0;
        }
        else if(NumLeve & 0x40 )
        {
            DispNum = DispNum * 0.000001;
            Decimal = 1;
        }
        else if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 2;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 3;
        }
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 1;
        }
        else if(NumLeve & 0x04)
        {
            DispNum = DispNum * 0.01;
            Decimal = 2;
        }

    }
    else if((DispNum >= 1000000) && (DispNum < 10000000))   // 123.4 KW
    {
        if(NumLeve & 0x0200)
        {
            DispNum = DispNum * 0.000000001;
            Decimal = 1;
        }
        else if( NumLeve & 0x0100)
        {
            DispNum = DispNum * 0.00000001;
            Decimal = 2;
        }
        else if( NumLeve & 0x80)
        {
            DispNum = DispNum * 0.0000001;
            Decimal = 0;
        }
        else if(NumLeve & 0x40 )
        {
            DispNum = DispNum * 0.000001;
            Decimal = 1;
        }
        else if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 2;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 3;
        }
        else if(NumLeve & 0x08 )
        {
            DispNum = DispNum * 0.001;
            Decimal = 1;
        }
    }
    else if((DispNum >= 10000000) && (DispNum < 100000000))  //1234 KW = 1.234MW
    {
        if(NumLeve & 0x0200)
        {
            DispNum = DispNum * 0.000000001;
            Decimal = 1;
        }
        else if( NumLeve & 0x0100)
        {
            DispNum = DispNum * 0.00000001;
            Decimal = 2;
        }
        else if( NumLeve & 0x80)
        {
            DispNum = DispNum * 0.0000001;
            Decimal = 0;
        }
        else if(NumLeve & 0x40 )
        {
            DispNum = DispNum * 0.000001;
            Decimal = 1;
        }
        else if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 2;
        }
        else if(NumLeve & 0x10 )
        {
            DispNum = DispNum * 0.0001;
            Decimal = 3;
        }
    }
    else if((DispNum >= 100000000) && (DispNum < 1000000000))  // 12.34 MW
    {
        if(NumLeve & 0x0200)
        {
            DispNum = DispNum * 0.000000001;
            Decimal = 1;
        }
        else if( NumLeve & 0x0100)
        {
            DispNum = DispNum * 0.00000001;
            Decimal = 2;
        }
        else if( NumLeve & 0x80)
        {
            DispNum = DispNum * 0.0000001;
            Decimal = 0;
        }
        else if(NumLeve & 0x40 )
        {
            DispNum = DispNum * 0.000001;
            Decimal = 1;
        }
        else if(NumLeve & 0x20 )
        {
            DispNum = DispNum * 0.00001;
            Decimal = 2;
        }
    }
    else if((DispNum >= 1000000000) && (DispNum < 10000000000))  // 123.4 MW
    {
        if(NumLeve & 0x0200)
        {
            DispNum = DispNum * 0.000000001;
            Decimal = 1;
        }
        else if( NumLeve & 0x0100)
        {
            DispNum = DispNum * 0.00000001;
            Decimal = 2;
        }
        else if( NumLeve & 0x80)
        {
            DispNum = DispNum * 0.0000001;
            Decimal = 0;
        }
        else if(NumLeve & 0x40 )
        {
            DispNum = DispNum * 0.000001;
            Decimal = 1;
        }
    }
    else if((DispNum >= 10000000000) && (DispNum < 100000000000))  // 1234 MW
    {
        if(NumLeve & 0x0200)
        {
            DispNum = DispNum * 0.000000001;
            Decimal = 1;
        }
        else if( NumLeve & 0x0100)
        {
            DispNum = DispNum * 0.00000001;
            Decimal = 2;
        }
        else if(NumLeve & 0x80 )
        {
            DispNum = DispNum * 0.0000001;
            Decimal = 0;
        }
    }
    else if((DispNum >= 100000000000) && (DispNum < 1000000000000))  // 12.34 KMW
    {
        if(NumLeve & 0x0200)
        {
            DispNum = DispNum * 0.000000001;
            Decimal = 1;
        }
        else if(NumLeve & 0x0100)
        {
            DispNum = DispNum * 0.00000001;
            Decimal = 2;
        }
    }
    else if((DispNum >= 1000000000000) && (DispNum < 10000000000000))  // 123.4 KMW
    {
        if(NumLeve & 0x0200)
        {
            DispNum = DispNum * 0.000000001;
            Decimal = 1;
        }
    }

    DispValue(num,(s32)DispNum,Decimal, 4, FALSE,0, TRUE, FALSE);
}

/**
  * @brief	显示数字
  * @param	s16 DispNum-要显示的数字
			u8 Decimal-小数点位数
			u8 DispWidth-显示宽度(几位数)
			BOOL bFlash-是否闪烁
			u8 FlashDec-闪烁的位数
			BOOL bEnableDisp-强制显示全部位
  * @retval None
  */
void DispValue(u8 Row,s32 DispNum, u8 Decimal, u8 DispWidth, BOOL bFlash, u8 FlashDec, BOOL bEnableDisp, BOOL bSet)
{
	u8 temp8[4] = {0, 0, 0, 0};
	u8 BitChar[4] = {0, 0, 0, 0};
	u8 i = 0;

    if(DispNum >= 10000)
    {
        DispNum = DispNum * 0.1;
    }

	BitChar[0] = kabs(DispNum)/1000;
	BitChar[1] = (kabs(DispNum)%1000)/100;
	BitChar[2] = (kabs(DispNum)%100)/10;
	BitChar[3] = kabs(DispNum)%10;
	// 保存要显示的字符
	for (i = 0 ; i < 4; i++)
	{
    	if (sg_CurrentDispChar[i] != POS_NUM && sg_CurrentDispChar[i] != NEG_NUM)
    	{
    		sg_CurrentDispChar[i] = BitChar[i];
    	}
    	temp8[i] = BitChar[i];
    	HT_WriteDot(Row, i, CHAR_NONE);  //清空小数点
	}

	if (Decimal == 3)  //小数点
	{
        HT_WriteDot(Row,0,TRUE);
		if (DispNum < 0)
		{
		    HT_WriteNegative(Row,TRUE);
		}
	}
	else if (Decimal == 2)
	{
		if (DispNum < 0)
		{
		    HT_WriteNegative(Row,TRUE);
		}
		else
		{
			if (BitChar[0] == 0)
			{
				temp8[0] = CHARMODE_NC;
			}
		}
        HT_WriteDot(Row,1,TRUE);
	}
	else if (Decimal == 1)
	{

		if (DispNum < 0)
		{
			if (BitChar[0] == 0)
			{
				temp8[0] = CHARMODE_NC;
				if (BitChar[1] == 0)
				{
					temp8[1] = CHARMODE_NC;
				}
				HT_WriteNegative(Row,TRUE);
			}
		}
		else
		{
			if (BitChar[0] == 0)
			{
				temp8[0] = CHARMODE_NC;
				if (BitChar[1] == 0)
				{
					temp8[1] = CHARMODE_NC;
				}
			}
			HT_WriteDot(Row,2,TRUE);
		}

	}
	else if (Decimal == 0)
	{
		if (DispNum < 0)
		{
		    HT_WriteNegative(Row,TRUE);
		}
	}
	else
	{
		return;
	}
	switch (DispWidth)
	{
    	case 1:
    		temp8[0] = CHAR_NONE;
    		temp8[1] = CHAR_NONE;
    		temp8[2] = CHAR_NONE;
    	    break;
    	case 2:
    		temp8[0] = CHAR_NONE;
    		temp8[1] = CHAR_NONE;
    	    break;
    	case 3:
    		temp8[0] = CHAR_NONE;
    	    break;
    	default:
    	    break;
	}

	if (bEnableDisp)
	{
		if (temp8[0] == CHAR_NONE)
		{
			temp8[0] = CHAR_0;
		}
		if (temp8[1] == CHAR_NONE)
		{
			temp8[1] = CHAR_0;
		}
		if (temp8[2] == CHAR_NONE)
		{
			temp8[2] = CHAR_0;
		}
		if (temp8[3] == CHAR_NONE)
		{
			temp8[3] = CHAR_0;
		}
	}

	if (bSet) // 进入设置状态0
	{
		/*for (i = 0; i < 4; i++)
		{
			if (sg_CurrentDispChar[i] == POS_NUM)
			{
				temp8[i] = CHARMODE_ZHENG;
			}
			else if (sg_CurrentDispChar[i] == NEG_NUM)
			{
				temp8[i] = CHARMODE_FU;
			}
		}*/
	}
	if (bFlash)
	{
		for (i = 0; i < 4; i++)
		{
    		if (Get_Bit(FlashDec, i) == 1)
    		{
    			temp8[i] = CHAR_NONE;
    		}
		}
	}
    HT_WriteValue(Row, 0, temp8[0]);
	HT_WriteValue(Row, 1, temp8[1]);
	HT_WriteValue(Row, 2, temp8[2]);
	HT_WriteValue(Row, 3, temp8[3]);


}

/**
  * @brief	显示字符
  * @param	u8 ch1-第1位字符 ：emInderMode
			u8 ch2-第2位字符 ：emInderMode
			u8 ch3-第3位字符 ：emInderMode
			u8 ch4-第4位字符 ：emInderMode
  * @retval None
  */
void DispHarmonicContent(u8 Select_VA,u8 Index)
{
    u32 DispValTmp[3];
    u8 Indextmp1,Indextmp2;
    Indextmp1 = Index / 10;
    Indextmp2 = Index % 10;
    /***********************************标签页*******************************************/
    HT_WriteValue(Line_R5,0,DISP1_T);
    HT_WriteValue(Line_R5,1,DISP1_H);
    HT_WriteValue(Line_R5,2,DISP1_d);

    /***********************************上三屏*******************************************/
    if(Select_VA == TRUE)
    {
        if(g_tParam.CtrlParam.NetMode == N34)
        {
            DispValTmp[0] = (u32)(HarmonicData.HarmonicpercentU[0][Index-1]/100);
            DispValTmp[1] = (u32)(HarmonicData.HarmonicpercentU[1][Index-1]/100);
            DispValTmp[2] = (u32)(HarmonicData.HarmonicpercentU[2][Index-1]/100);
            DispValue(0,DispValTmp[0], 2, 4, FALSE,0, FALSE, FALSE);
            DispValue(1,DispValTmp[1], 2, 4, FALSE,0, FALSE, FALSE);
            DispValue(2,DispValTmp[2], 2, 4, FALSE,0, FALSE, FALSE);
            HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_PerV);
            HT_WritePhase(TRUE,PH_TH);
        }
        else
        {
            DispValTmp[0] = (u32)(HarmonicData.HarmonicpercentU[0][Index-1]/100);
            DispValTmp[2] = (u32)(HarmonicData.HarmonicpercentU[2][Index-1]/100);
            DispValue(0,DispValTmp[0], 2, 4, FALSE,0, FALSE, FALSE);
            DispValue(1,DispValTmp[2], 2, 4, FALSE,0, FALSE, FALSE);
            HT_Write4Value(2,DISP_FU,DISP_FU,DISP_FU,DISP_FU);
            HT_WriteUnit(TRUE,PH_A,DISP_UNIT_PerV);
            HT_WriteUnit(TRUE,PH_B,DISP_UNIT_PerV);
            HT_WritePhaseDouble(TRUE,PH_A);
            HT_WritePhaseDouble(TRUE,PH_B);
        }

    }
    else
    {
        DispValTmp[0] = (u32)(HarmonicData.HarmonicpercentI[0][Index-1]/100);
        DispValTmp[1] = (u32)(HarmonicData.HarmonicpercentI[1][Index-1]/100);
        DispValTmp[2] = (u32)(HarmonicData.HarmonicpercentI[2][Index-1]/100);
        DispValue(0,DispValTmp[0], 2, 4, FALSE,0, FALSE, FALSE);
        DispValue(1,DispValTmp[1], 2, 4, FALSE,0, FALSE, FALSE);
        DispValue(2,DispValTmp[2], 2, 4, FALSE,0, FALSE, FALSE);
        HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_PerA);
        HT_WritePhase(TRUE,PH_TH);
    }
    HT_WriteValue(4,R4_L0,CHAR_NONE);
    HT_WriteValue(4,R4_L1,CHAR_NONE);
    if(Indextmp1 == 0)
        HT_WriteValue(4,R4_L2,CHAR_NONE);
    else
        HT_WriteValue(4,R4_L2,Indextmp1);
    HT_WriteValue(4,R4_L3,Indextmp2);
}

/**
  * @brief	显示字符
  * @param	u8 ch1-第1位字符 ：emInderMode
			u8 ch2-第2位字符 ：emInderMode
			u8 ch3-第3位字符 ：emInderMode
			u8 ch4-第4位字符 ：emInderMode
  * @retval None
  */

void DispChar(u8 num,u8 ch1, u8 ch2, u8 ch3, u8 ch4)    //主显示程序
{
	switch(num)
	{
		case 0:
            vg_Display_Value[0] = ch1;
            vg_Display_Value[1] = ch2;
            vg_Display_Value[2] = ch3;
            vg_Display_Value[3] = ch4;
		    break;
		case 1:
            vg_Display_Value[4] = ch1;
            vg_Display_Value[5] = ch2;
            vg_Display_Value[6] = ch3;
            vg_Display_Value[7] = ch4;
		    break;
		case 2:
            vg_Display_Value[8] = ch1;
            vg_Display_Value[9] = ch2;
            vg_Display_Value[10] = ch3;
            vg_Display_Value[11] = ch4;
            break;
		default:
		    break;
	}
}

/**
  * @brief	显示开关量
  * @param	u8 Index-第1-4位  ：
			u8 ch-字符  ：emInderMode  1 1 1 1 1 1 1   1 1 1 1 0 0 0 0
  * @retval None
  */
void Disp_IO_Status(u8 InStatus,u8 OutStatus)
{
    u8 i,j;
    HT_Write_IO_Frame(TRUE);
    for(i = 0;i<DIN_NUM;i++)
    {
        if(InStatus & 0x01)
        {
            HT_Write_IO_Status(TRUE,0,i);
        }
        else
        {
            HT_Write_IO_Status(FALSE,0,i);
        }
        InStatus >>= 1;
    }
    for(j = 0;j<OUT_NUM;j++)
    {
        if(OutStatus& 0x01)
        {
            HT_Write_IO_Status(TRUE,1,j);
        }
        else
        {
            HT_Write_IO_Status(FALSE,1,j);
        }
        OutStatus >>= 1;
    }
}


/**
  * @brief	显示字符
  * @param	u8 Index-第1-4位  ：DIG1 DIG2 DIG3 DIG4
			u8 ch-字符  ：emInderMode
  * @retval None
  */
void DispBitChar(u8 Index, u8 ch)
{
    if (Index < 4) // 共4位
    {
        vg_Display_Value[Index] = ch;
    }
}


/*u8 Menu_Main[][4] = {
				{CHARMODE_NC,CHARMODE_P,CHARMODE_F,CHARMODE_A},
				{CHARMODE_NC,CHARMODE_P,CHARMODE_F,CHARMODE_B},
				{CHARMODE_NC,CHARMODE_P,CHARMODE_F,CHARMODE_C},
				{CHARMODE_NC,CHARMODE_U,CHARMODE_FU,CHARMODE_A},
				{CHARMODE_NC,CHARMODE_U,CHARMODE_FU,CHARMODE_B},
				{CHARMODE_NC,CHARMODE_U,CHARMODE_FU,CHARMODE_C},
				{CHARMODE_NC,CHARMODE_I,CHARMODE_FU,CHARMODE_A},
				{CHARMODE_NC,CHARMODE_I,CHARMODE_FU,CHARMODE_B},
				{CHARMODE_NC,CHARMODE_I,CHARMODE_FU,CHARMODE_C},
				{CHARMODE_NC,CHARMODE_P,CHARMODE_FU,CHARMODE_A},
				{CHARMODE_NC,CHARMODE_P,CHARMODE_FU,CHARMODE_B},
				{CHARMODE_NC,CHARMODE_P,CHARMODE_FU,CHARMODE_C},
				{CHARMODE_NC,CHARMODE_Q,CHARMODE_FU,CHARMODE_A},
				{CHARMODE_NC,CHARMODE_Q,CHARMODE_FU,CHARMODE_B},
				{CHARMODE_NC,CHARMODE_Q,CHARMODE_FU,CHARMODE_C},
				{CHARMODE_NC,CHARMODE_S,CHARMODE_FU,CHARMODE_A},
				{CHARMODE_NC,CHARMODE_S,CHARMODE_FU,CHARMODE_B},
				{CHARMODE_NC,CHARMODE_S,CHARMODE_FU,CHARMODE_C},
			};*/


/**
  * @brief	显示数字
  * @param	s16 DispNum-要显示的数字
			u8 Decimal-小数点位数
			u8 DispWidth-显示宽度(几位数)
			BOOL bFlash-是否闪烁
			u8 FlashDec-闪烁的位数
			BOOL bEnableDisp-强制显示全部位
  * @retval None
  */
void DispMenu_Main(void)
{
    u8 UintFlag;
    s16 TmpPf;
    u32 DispValTmp[3];
    s64 DispValTmpPQS[4];
    Leve_1_DispNUM = 0x00;
    if(g_OverCurr[PH_A] || g_OverCurr[PH_B] || g_OverCurr[PH_C])
    {
        sg_DispMainMenuIndex = MENU_INDEX_I;
    }
    switch (sg_DispMainMenuIndex)
    {
        case MENU_INDEX_U://单位V 1位小数点
		    /***********************************标签页*******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_E);
			HT_WriteValue(Line_R5,1,DISP1_L);
		    HT_WriteValue(Line_R5,2,CHAR_NONE);

		    /***********************************上三屏*******************************************/
		    for(u8 i = 0; i<3; i++)
		    {
		        DispValTmp[i] = vg_Power_Val.Real2_Val[i].U;
		        if(vg_Power_Val.Real2_Val[i].U < 100000)     //123.4
                {
                    Leve_1_DispNUM |= 0x01;
                }                                           //6                                   //7
                if((vg_Power_Val.Real2_Val[i].U >= 100000) && (vg_Power_Val.Real2_Val[i].U < 1000000)) //1.234 K
                {
                    Leve_1_DispNUM |= 0x02;
                }                                           //7                                    //8
                else if((vg_Power_Val.Real2_Val[i].U >= 1000000) && (vg_Power_Val.Real2_Val[i].U < 10000000))//12.34 K
                {
                    Leve_1_DispNUM |= 0x04;
                }                                           //8                                    //9
                else if((vg_Power_Val.Real2_Val[i].U >= 10000000) && (vg_Power_Val.Real2_Val[i].U < 100000000))//123.4 K
                {
                    Leve_1_DispNUM |= 0x08;
                }                                           //9                                    //10
                else if((vg_Power_Val.Real2_Val[i].U >= 100000000) && (vg_Power_Val.Real2_Val[i].U < 1000000000))//1.234 M
                {
                    Leve_1_DispNUM |= 0x10;
                }
		    }
		    for(u8 j = 0; j<3; j++)
		    {
                DispValueVoltage(j,DispValTmp[j]*0.1, 0, 4, FALSE,0, TRUE, Leve_1_DispNUM);
		    }
            if(Leve_1_DispNUM == 0x01)
            {
                HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_V);
            }
		    if((Leve_1_DispNUM > 0x01) && Leve_1_DispNUM < 0x10)
		    {
                HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_kV);
		    }
            else if(Leve_1_DispNUM & 0x10)
		    {
                HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_mV);
		    }
		    HT_WritePhase(TRUE,PH_TH);
            /***********************************下一屏*******************************************/
            /*HT_WriteValue(3,R4_L0,CHAR_P);
              HT_WriteValue(3,R4_L1,CHAR_t);
              DispValue(4,g_tParam.CtrlParam.PTNum,0,4,FALSE,0,FALSE,FALSE);*/
            DispValueVoltage(4,vg_Power_Val.Uavr*0.1, 0, 4, FALSE,0, TRUE, Leve_1_DispNUM);
			break;
	    case MENU_INDEX_LineU:// 单位V 1位小数点
	        /***********************************标签页*******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_E);
            HT_WriteValue(Line_R5,1,DISP1_L);
		    HT_WriteValue(Line_R5,2,CHAR_NONE);

		    /***********************************上三屏*******************************************/
		    for(u8 i = 0; i<3; i++)
		    {
		        DispValTmp[i] = vg_Power_Val.Real2_Val[i].LineU;
		        if(vg_Power_Val.Real2_Val[i].LineU < 100000)     //123.4
                {
                    Leve_1_DispNUM |= 0x01;
                }                                           //6                                   //7
                if((vg_Power_Val.Real2_Val[i].LineU >= 100000) && (vg_Power_Val.Real2_Val[i].LineU < 1000000)) //1.234 K
                {
                    Leve_1_DispNUM |= 0x02;
                }                                           //7                                    //8
                else if((vg_Power_Val.Real2_Val[i].LineU >= 1000000) && (vg_Power_Val.Real2_Val[i].LineU < 10000000))//12.34 K
                {
                    Leve_1_DispNUM |= 0x04;
                }                                           //8                                    //9
                else if((vg_Power_Val.Real2_Val[i].LineU >= 10000000) && (vg_Power_Val.Real2_Val[i].LineU < 100000000))//123.4 K
                {
                    Leve_1_DispNUM |= 0x08;
                }                                           //9                                    //10
                else if((vg_Power_Val.Real2_Val[i].LineU >= 100000000) && (vg_Power_Val.Real2_Val[i].LineU < 1000000000))//1.234 M
                {
                    Leve_1_DispNUM |= 0x10;
                }
		    }
		    for(u8 j = 0; j<3; j++)
		    {
                DispValueVoltage(j,DispValTmp[j]*0.1, 0, 4, FALSE,0, TRUE, Leve_1_DispNUM);
		    }
            if(Leve_1_DispNUM == 0x01)
            {
                HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_V);
            }
		    if((Leve_1_DispNUM > 0x01) && Leve_1_DispNUM < 0x10)
		    {
                HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_kV);
		    }
            else if(Leve_1_DispNUM & 0x10)
		    {
                HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_mV);
		    }
		    HT_WritePhaseDouble(TRUE,PH_TH);
		    /***********************************下一屏*****************************************/
            /*HT_WriteValue(3,R4_L0,CHAR_P);
              HT_WriteValue(3,R4_L1,CHAR_t);
              DispValue(4,g_tParam.CtrlParam.PTNum,0,4,FALSE,0,FALSE,FALSE);*/
            DispValueVoltage(4,vg_Power_Val.ULavr*0.1, 0, 4, FALSE,0, TRUE, Leve_1_DispNUM);
			break;
		case MENU_INDEX_I://单位A 3位小数点
            /***********************************标签页******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_E);
            HT_WriteValue(Line_R5,1,DISP1_L);
		    HT_WriteValue(Line_R5,2,CHAR_NONE);

		    /***********************************上三屏*******************************************/
		    u8 i ;
            for(i = 0; i<3; i++)
		    {
		        DispValTmp[i] = vg_Power_Val.Real2_Val[i].I;
		        if(vg_Power_Val.Real2_Val[i].I < 100000)     // 1.234
                {
                    Leve_1_DispNUM |= 0x01;
                }                                           //6                                   //7
                if((vg_Power_Val.Real2_Val[i].I >= 100000) && (vg_Power_Val.Real2_Val[i].I < 1000000)) // 12.34
                {
                    Leve_1_DispNUM |= 0x02;
                }                                           //7                                    //8
                else if((vg_Power_Val.Real2_Val[i].I >= 1000000) && (vg_Power_Val.Real2_Val[i].I < 10000000))// 123.4
                {
                    Leve_1_DispNUM |= 0x04;
                }                                           //8                                    //9
                else if((vg_Power_Val.Real2_Val[i].I >= 10000000) && (vg_Power_Val.Real2_Val[i].I < 100000000))// 1.234 K
                {
                    Leve_1_DispNUM |= 0x08;
                }                                           //9                                    //10
                else if((vg_Power_Val.Real2_Val[i].I >= 100000000) && (vg_Power_Val.Real2_Val[i].I < 1000000000))// 12.34 K
                {
                    Leve_1_DispNUM |= 0x10;
                }
		    }

            /**************************过载报警*******************************************/
		    for(u8 j = 0; j<3; j++)
		    {
                DispValueCurrent(j,DispValTmp[j]*0.1, 0, 4, FALSE,0, TRUE, Leve_1_DispNUM);
		    }
		    if(g_OverCurr[PH_A] && (g_tParam.CtrlParam.Alarm !=0))
		    {
                if(sg_bSetFlashEnable)
                {
                    HT_Write4Value(PH_A,DISP1_NC,DISP1_NC,DISP1_NC,DISP1_NC);
                    for (i = 0 ; i < 4; i++)
                	{
                    	HT_WriteDot(PH_A, i, CHAR_NONE);  // 清空小数点
                	}
                }
		    }
		    if(g_OverCurr[PH_B] && (g_tParam.CtrlParam.Alarm !=0))
		    {
                if(sg_bSetFlashEnable)
                {
                    HT_Write4Value(PH_B,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
                    for (i = 0 ; i < 4; i++)
                	{
                    	HT_WriteDot(PH_B, i, CHAR_NONE);  // 清空小数点
                	}
                }
		    }
		    if(g_OverCurr[PH_C] && (g_tParam.CtrlParam.Alarm !=0))
		    {
                if(sg_bSetFlashEnable)
                {
                    HT_Write4Value(PH_C,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
                    for (i = 0 ; i < 4; i++)
                	{
                    	HT_WriteDot(PH_C, i, CHAR_NONE);  // 清空小数点
                	}
                 }
		    }

		    if( Leve_1_DispNUM & 0x20 || Leve_1_DispNUM & 0x10 || Leve_1_DispNUM & 0x08)
		    {
		        HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_kA);
		    }
		    else
		    {
                HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_A);
		    }
		    HT_WritePhase(TRUE,PH_TH);
		    /***********************************下一屏*******************************************/
            /*HT_WriteValue(3,R4_L0,CHAR_C);
              HT_WriteValue(3,R4_L1,CHAR_t);
              DispValue(4,g_tParam.CtrlParam.CTNum,0,4,FALSE,0,FALSE,FALSE);*/
            HT_WriteValue(3,R4_L0,CHAR_NONE);
            DispValueCurrent(4,vg_Power_Val.Iavr*0.1, 0, 4, FALSE,0, TRUE, Leve_1_DispNUM);
			break;
       	case MENU_INDEX_P:// 有功
       	    /***********************************标签页*******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_E);
            HT_WriteValue(Line_R5,1,DISP1_L);
		    HT_WriteValue(Line_R5,2,CHAR_NONE);

		    /***********************************上三屏*******************************************/
			for(u8 i = 0; i<4; i++)
		    {
		        if(g_tParam.CtrlParam.NetMode == N34)
		        {
                    if(i < 3)
                    {
                        DispValTmpPQS[i] = kabs(vg_Power_Val.Real2_Val[i].P);
                    }
                    else
                    {
                        DispValTmpPQS[i] = kabs(vg_Power_Val.Pt);
                    }
                }
                else
                {
                    if(i == 0)
                    {
                        DispValTmpPQS[i] = kabs(vg_Power_Val.Pt);
                    }
                    else if(i == 1)
                    {
                        DispValTmpPQS[i] = kabs(vg_Power_Val.Qt);
                    }
                    else if(i == 2)
                    {
                        DispValTmpPQS[i] = vg_Power_Val.St;
                    }
					else if(i == 3)
                    {
                        DispValTmpPQS[i] = 0x00;
                    }
                }

		        if(DispValTmpPQS[i] < 100000)
                {
                    Leve_1_DispNUM |= 0x01;
                }                                                                             //7
                if((DispValTmpPQS[i] >= 100000) && (DispValTmpPQS[i]< 1000000))
                {
                    Leve_1_DispNUM |= 0x02;
                }                                           //7                                        //8
                else if((DispValTmpPQS[i] >= 1000000) && (DispValTmpPQS[i] < 10000000))
                {
                    Leve_1_DispNUM |= 0x04;
                }                                           //8                                        //9
                else if((DispValTmpPQS[i] >= 10000000) && (DispValTmpPQS[i] < 100000000))
                {
                    Leve_1_DispNUM |= 0x08;
                }                                           //9                                        //10
                else if((DispValTmpPQS[i] >= 100000000) && (DispValTmpPQS[i] < 1000000000))
                {
                    Leve_1_DispNUM |= 0x10;
                }                                           //10                                       //11
                else if((DispValTmpPQS[i] >= 1000000000) && (DispValTmpPQS[i] < 10000000000))
                {
                    Leve_1_DispNUM |= 0x20;
                }                                           //11                                       //12
                else if((DispValTmpPQS[i] >= 10000000000) && (DispValTmpPQS[i] < 100000000000))
                {
                    Leve_1_DispNUM |= 0x40;
                }                                           //12                                       //13
                else if((DispValTmpPQS[i] >= 100000000000) && (DispValTmpPQS[i] < 1000000000000))
                {
                    Leve_1_DispNUM |= 0x80;
                }                                          //13                                        //14
                else if((DispValTmpPQS[i] >= 1000000000000) && (DispValTmpPQS[i] < 10000000000000))
                {
                    Leve_1_DispNUM |= 0x0100;
                }                                          //14                                        //15
                else if((DispValTmpPQS[i] >= 10000000000000) && (DispValTmpPQS[i] < 100000000000000))
                {
                    Leve_1_DispNUM |= 0x0200;
                }

		    }
		    for(u8 j = 0; j<3; j++)
		    {
                DispValuePower(j,DispValTmpPQS[j]*0.1, 0, 4, FALSE,0, TRUE, Leve_1_DispNUM);
		    }
            if(g_tParam.CtrlParam.NetMode == N34)
            {
                DispValuePower(4,DispValTmpPQS[3]*0.1, 0, 4, FALSE,0, TRUE, Leve_1_DispNUM);
                if( Leve_1_DispNUM == 0x01)
                {
                    HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_w);
                }
    		    else if( Leve_1_DispNUM < 0x10 )
    		    {
                    HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_kw);
    		    }
    		    else if( Leve_1_DispNUM <0x0100)
    		    {
    		        HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_mw);
    		    }
    		    else if( Leve_1_DispNUM & 0x0100 || Leve_1_DispNUM & 0x0200)
    		    {
                    HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_kmw);
    		    }
    		    for(u8 k=PH_A; k<PH_TH; k++)
    		    {
                    if(vg_Power_Val.Real2_Val[k].P < 0)
    		        {
                        HT_WriteNegative(k,TRUE);
    		        }
    		        else
    		        {
                        HT_WriteNegative(k,CHAR_NONE);
    		        }
    		    }

    		    HT_WritePhase(TRUE,PH_TH);
    		    HT_WriteSum(PH_TH,TRUE);
    		    HT_WriteValue(3,R4_L0,CHAR_P);
                HT_WriteValue(3,R4_L1,CHAR_NONE);
                HT_WriteValue(3,R4_L2,CHAR_NONE);
                HT_WriteValue(3,R4_L3,CHAR_NONE);

                if(vg_Power_Val.Pt < 0)
    		        HT_WriteNegative(PH_TH,TRUE);
            }
            else
            {
                if( Leve_1_DispNUM == 0x01)
                {
                    HT_WriteUnit(TRUE,PH_A,DISP_UNIT_w);
                    HT_WriteUnit(TRUE,PH_B,DISP_UNIT_var);
                    HT_WriteUnit(TRUE,PH_C,DISP_UNIT_VA);
                }
    		    else if( Leve_1_DispNUM < 0x10 )
    		    {
                    HT_WriteUnit(TRUE,PH_A,DISP_UNIT_kw);
                    HT_WriteUnit(TRUE,PH_B,DISP_UNIT_kvar);
                    HT_WriteUnit(TRUE,PH_C,DISP_UNIT_kVA);
    		    }
    		    else if( Leve_1_DispNUM <0x0100)
    		    {
    		        HT_WriteUnit(TRUE,PH_A,DISP_UNIT_mw);
                    HT_WriteUnit(TRUE,PH_B,DISP_UNIT_mvar);
                    HT_WriteUnit(TRUE,PH_C,DISP_UNIT_mVA);
    		    }
    		    else if( Leve_1_DispNUM & 0x0100 || Leve_1_DispNUM & 0x0200)
    		    {
                    HT_WriteUnit(TRUE,PH_A,DISP_UNIT_kmw);
                    HT_WriteUnit(TRUE,PH_B,DISP_UNIT_kmvar);
                    HT_WriteUnit(TRUE,PH_C,DISP_UNIT_kmVA);
    		    }
    		    HT_WriteSum(PH_A,TRUE);
    		    HT_WriteSum(PH_B,TRUE);
    		    HT_WriteSum(PH_C,TRUE);
                if(vg_Power_Val.Pt < 0)
    		        HT_WriteNegative(PH_A,TRUE);
    		    else
    		        HT_WriteNegative(PH_A,CHAR_NONE);
    		    if(vg_Power_Val.Qt < 0)
    		        HT_WriteNegative(PH_B,TRUE);
    		    else
    		        HT_WriteNegative(PH_B,CHAR_NONE);
            }
			break;
	    case MENU_INDEX_Q:// 无功
	        /***********************************标签页*******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_E);
            HT_WriteValue(Line_R5,1,DISP1_L);
		    HT_WriteValue(Line_R5,2,CHAR_NONE);

		    /***********************************上三屏*******************************************/
			for(u8 i = 0; i<4; i++)
		    {
                if(i < 3)
                {
                    DispValTmpPQS[i] = kabs(vg_Power_Val.Real2_Val[i].Q);
                }
                if(i == 3)
                {
                    DispValTmpPQS[i] = kabs(vg_Power_Val.Qt);
                }

		        if(DispValTmpPQS[i] < 100000)
                {
                    Leve_1_DispNUM |= 0x01;
                }                                                                             //7
                if((DispValTmpPQS[i] >= 100000) && (DispValTmpPQS[i]< 1000000))
                {
                    Leve_1_DispNUM |= 0x02;
                }                                           //7                                        //8
                else if((DispValTmpPQS[i] >= 1000000) && (DispValTmpPQS[i] < 10000000))
                {
                    Leve_1_DispNUM |= 0x04;
                }                                           //8                                        //9
                else if((DispValTmpPQS[i] >= 10000000) && (DispValTmpPQS[i] < 100000000))
                {
                    Leve_1_DispNUM |= 0x08;
                }                                           //9                                        //10
                else if((DispValTmpPQS[i] >= 100000000) && (DispValTmpPQS[i] < 1000000000))
                {
                    Leve_1_DispNUM |= 0x10;
                }                                           //10                                       //11
                else if((DispValTmpPQS[i] >= 1000000000) && (DispValTmpPQS[i] < 10000000000))
                {
                    Leve_1_DispNUM |= 0x20;
                }                                           //11                                       //12
                else if((DispValTmpPQS[i] >= 10000000000) && (DispValTmpPQS[i] < 100000000000))
                {
                    Leve_1_DispNUM |= 0x40;
                }                                           //12                                       //13
                else if((DispValTmpPQS[i] >= 100000000000) && (DispValTmpPQS[i] < 1000000000000))
                {
                    Leve_1_DispNUM |= 0x80;
                }                                           //13                                       //14
                else if((DispValTmpPQS[i] >= 1000000000000) && (DispValTmpPQS[i] < 10000000000000))
                {
                    Leve_1_DispNUM |= 0x0100;
                }                                           //14                                       //15
                else if((DispValTmpPQS[i] >= 10000000000000) && (DispValTmpPQS[i] < 100000000000000))
                {
                    Leve_1_DispNUM |= 0x0200;
                }

		    }
		    for(u8 j = 0; j<3; j++)
		    {
                DispValuePower(j,DispValTmpPQS[j]*0.1, 0, 4, FALSE,0, TRUE, Leve_1_DispNUM);
		    }
		    DispValuePower(4,DispValTmpPQS[3]*0.1, 0, 4, FALSE,0, TRUE, Leve_1_DispNUM);

            if( Leve_1_DispNUM == 0x01)
            {
                HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_var);
            }
		    else if( Leve_1_DispNUM < 0x10 )
		    {
                HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_kvar);
		    }
		    else if( Leve_1_DispNUM <0x0100)
		    {
		        HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_mvar);
		    }
		    else if( Leve_1_DispNUM & 0x0100 || Leve_1_DispNUM & 0x0200)
		    {
                HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_kmvar);
		    }
		    for(u8 k=PH_A; k<PH_TH; k++)
		    {
                if(vg_Power_Val.Real2_Val[k].Q < 0)
		        {
                    HT_WriteNegative(k,TRUE);
		        }
		        else
		        {
                    HT_WriteNegative(k,CHAR_NONE);
		        }
		    }


		    HT_WritePhase(TRUE,PH_TH);
		    HT_WriteSum(PH_TH,TRUE);
		    HT_WriteValue(3,R4_L0,CHAR_q);
            HT_WriteValue(3,R4_L1,CHAR_NONE);
            HT_WriteValue(3,R4_L2,CHAR_NONE);
            HT_WriteValue(3,R4_L3,CHAR_NONE);

            if(vg_Power_Val.Qt < 0)
		        HT_WriteNegative(PH_TH,TRUE);
			break;
	    case MENU_INDEX_S://有功
	        /***********************************标签页*******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_E);
            HT_WriteValue(Line_R5,1,DISP1_L);
		    HT_WriteValue(Line_R5,2,CHAR_NONE);

		    /***********************************上三屏*******************************************/
			for(u8 i = 0; i<4; i++)
		    {
                if(i < 3)
                {
                    DispValTmpPQS[i] = vg_Power_Val.Real2_Val[i].S;
                }
                if(i == 3)
                {
                    DispValTmpPQS[i] = vg_Power_Val.St;
                }

		        if(DispValTmpPQS[i] < 100000)
                {
                    Leve_1_DispNUM |= 0x01;
                }                                                                             //7
                if((DispValTmpPQS[i] >= 100000) && (DispValTmpPQS[i]< 1000000))
                {
                    Leve_1_DispNUM |= 0x02;
                }                                           //7                                        //8
                else if((DispValTmpPQS[i] >= 1000000) && (DispValTmpPQS[i] < 10000000))
                {
                    Leve_1_DispNUM |= 0x04;
                }                                           //8                                        //9
                else if((DispValTmpPQS[i] >= 10000000) && (DispValTmpPQS[i] < 100000000))
                {
                    Leve_1_DispNUM |= 0x08;
                }                                           //9                                        //10
                else if((DispValTmpPQS[i] >= 100000000) && (DispValTmpPQS[i] < 1000000000))
                {
                    Leve_1_DispNUM |= 0x10;
                }                                           //10                                       //11
                else if((DispValTmpPQS[i] >= 1000000000) && (DispValTmpPQS[i] < 10000000000))
                {
                    Leve_1_DispNUM |= 0x20;
                }                                           //11                                       //12
                else if((DispValTmpPQS[i] >= 10000000000) && (DispValTmpPQS[i] < 100000000000))
                {
                    Leve_1_DispNUM |= 0x40;
                }                                           //12                                       //13
                else if((DispValTmpPQS[i] >= 100000000000) && (DispValTmpPQS[i] < 1000000000000))
                {
                    Leve_1_DispNUM |= 0x80;
                }                                           //13                                       //14
                else if((DispValTmpPQS[i] >= 1000000000000) && (DispValTmpPQS[i] < 10000000000000))
                {
                    Leve_1_DispNUM |= 0x0100;
                }                                           //14                                       //15
                else if((DispValTmpPQS[i] >= 10000000000000) && (DispValTmpPQS[i] < 100000000000000))
                {
                    Leve_1_DispNUM |= 0x0200;
                }

		    }
		    for(u8 j = 0; j<3; j++)
		    {
                DispValuePower(j,DispValTmpPQS[j]*0.1, 0, 4, FALSE,0, TRUE, Leve_1_DispNUM);
		    }
		    DispValuePower(4,DispValTmpPQS[3]*0.1, 0, 4, FALSE,0, TRUE, Leve_1_DispNUM);

            if( Leve_1_DispNUM == 0x01)
            {
                HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_VA);
            }
		    else if( Leve_1_DispNUM < 0x10 )
		    {
                HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_kVA);
		    }
		    else if( Leve_1_DispNUM <0x0100)
		    {
		        HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_mVA);
		    }
		    else if( Leve_1_DispNUM & 0x0100 || Leve_1_DispNUM & 0x0200)
		    {
                HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_kmVA);
		    }

		    HT_WritePhase(TRUE,PH_TH);
		    HT_WriteSum(PH_TH,TRUE);
		    HT_WriteValue(3,R4_L0,CHAR_S);
            HT_WriteValue(3,R4_L1,CHAR_NONE);
            HT_WriteValue(3,R4_L2,CHAR_NONE);
            HT_WriteValue(3,R4_L3,CHAR_NONE);
			break;
		case MENU_INDEX_PF:
		    /***********************************标签页*******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_E);
            HT_WriteValue(Line_R5,1,DISP1_L);
		    HT_WriteValue(Line_R5,2,CHAR_NONE);

		    /***********************************上三屏*******************************************/
		    for(u8 i = PH_A; i<PH_TH; i++)
            {
                DispValTmpPQS[i] = kabs(vg_Power_Val.Real2_Val[i].PF);
                DispValue(i,DispValTmpPQS[i],3,4,FALSE,0,FALSE,FALSE);
            }
            for(u8 k=PH_A; k<PH_TH; k++)
		    {
                if(vg_Power_Val.Real2_Val[k].PF < 0)
		        {
                    HT_WriteNegative(k,TRUE);
		        }
		        else
		        {
                    HT_WriteNegative(k,CHAR_NONE);
		        }
		    }
		    HT_WritePhase(TRUE,PH_TH);
            HT_WriteValue(4,R4_L2,CHAR_P);
            HT_WriteValue(4,R4_L3,CHAR_F);
		    break;
        case MENU_INDEX_F:
		    /***********************************标签页*******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_E);
            HT_WriteValue(Line_R5,1,DISP1_L);
		    HT_WriteValue(Line_R5,2,CHAR_NONE);

		    /***********************************上三屏*******************************************/
		    HT_WriteValue(0,1,DISP1_p);
		    HT_WriteValue(0,2,DISP1_F);
		    if(vg_Power_Val.PFTA == 0)
		    {
                HT_WriteValue(1,0,DISP_FU);
                HT_WriteValue(1,1,DISP_FU);
                HT_WriteValue(1,2,DISP_FU);
                HT_WriteValue(1,3,DISP_FU);
		    }
		    else
		    {
                if(vg_Power_Val.PFTA < 0)
                    TmpPf = - vg_Power_Val.PFTA;
                else
                    TmpPf = vg_Power_Val.PFTA;
                DispValue(1,TmpPf,3,4,FALSE,0,FALSE,FALSE);
		    }
		    if(vg_Power_Val.PFTA < 0)
		    {
                HT_WriteNegative(PH_B,TRUE);
		    }
		    else
		    {
                HT_WriteNegative(PH_B,CHAR_NONE);
		    }
		    if(vg_Power_Val.Freq == 0)
		    {
                HT_WriteValue(4,0,CHAR_FU);
                HT_WriteValue(4,1,CHAR_FU);
                HT_WriteValue(4,2,CHAR_FU);
                HT_WriteValue(4,3,CHAR_FU);
		    }
		    else
		    {
                DispValue(4,vg_Power_Val.Freq,2,4,FALSE,0,FALSE,FALSE);
		    }
		    HT_WriteValue(3,R4_L0,CHAR_F);
            HT_WriteValue(3,R4_L1,CHAR_NONE);
		    break;
        case MENU_INDEX_PosEPT://单位k
            /***********************************标签页*******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_E);
            HT_WriteValue(Line_R5,1,DISP1_n);
		    HT_WriteValue(Line_R5,2,CHAR_NONE);

		    /***********************************上三屏*******************************************/

            HT_WriteValue(0,0,DISP1_E);
            HT_WriteValue(0,1,DISP1_p);
		    HT_WriteValue(0,2,CHAR_NONE);
		    HT_WriteValue(0,3,DISP1_ZHENG);

            if(ElectricEnergy.PosEPT > 999999)
            {
                DispValTmpPQS[0] = (s32)(ElectricEnergy.PosEPT * 10);
                DispValTmpPQS[1] = DispValTmpPQS[0]/10000;
                DispValTmpPQS[2] = DispValTmpPQS[0]%10000;
                DispValue(2,DispValTmpPQS[2],1,4, FALSE, 0, TRUE, FALSE);
            }
            else
            {
                DispValTmpPQS[0] = (s32)(ElectricEnergy.PosEPT * 100);
                DispValTmpPQS[1] = DispValTmpPQS[0]/10000;
                DispValTmpPQS[2] = DispValTmpPQS[0]%10000;
                DispValue(2,DispValTmpPQS[2],2,4, FALSE, 0, TRUE, FALSE);
            }
            DispValue(1,DispValTmpPQS[1],0,4, FALSE, 0, TRUE, FALSE);
            HT_WriteUnit(TRUE,PH_ALL,0);
			break;
	    case MENU_INDEX_NegEPT://单位k
	        /***********************************标签页*******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_E);
            HT_WriteValue(Line_R5,1,DISP1_n);
		    HT_WriteValue(Line_R5,2,CHAR_NONE);

		    /***********************************上三屏*******************************************/

	        HT_ClearLeftSign(TRUE);
		    HT_ClearRightSign(TRUE);
            HT_WriteValue(0,0,DISP1_E);
            HT_WriteValue(0,1,DISP1_p);
		    HT_WriteValue(0,2,CHAR_NONE);
		    HT_WriteValue(0,3,DISP1_FU);

	        if(ElectricEnergy.NegEPT > 999999)
            {
                DispValTmpPQS[0] = (s32)(ElectricEnergy.NegEPT * 10);
                DispValTmpPQS[1] = DispValTmpPQS[0]/10000;
                DispValTmpPQS[2] = DispValTmpPQS[0]%10000;
                DispValue(2,DispValTmpPQS[2],1,4, FALSE, 0, TRUE, FALSE);
            }
            else
            {
                DispValTmpPQS[0] = (s32)(ElectricEnergy.NegEPT * 100);
                DispValTmpPQS[1] = DispValTmpPQS[0]/10000;
                DispValTmpPQS[2] = DispValTmpPQS[0]%10000;
                DispValue(2,DispValTmpPQS[2],2,4, FALSE, 0, TRUE, FALSE);
            }
            DispValue(1,DispValTmpPQS[1],0,4, FALSE, 0, TRUE, FALSE);
            HT_WriteUnit(TRUE,PH_ALL,0);
			break;
	    case MENU_INDEX_PosEQT://单位k
            /***********************************标签页*******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_E);
            HT_WriteValue(Line_R5,1,DISP1_n);
		    HT_WriteValue(Line_R5,2,CHAR_NONE);

		    /***********************************上三屏*******************************************/

            HT_WriteValue(0,0,DISP1_E);
            HT_WriteValue(0,1,DISP1_q);
		    HT_WriteValue(0,2,CHAR_NONE);
		    HT_WriteValue(0,3,DISP1_ZHENG);

	        if(ElectricEnergy.PosEQT > 999999)
	        {
                DispValTmpPQS[0] = (s32)(ElectricEnergy.PosEQT * 10);
		        DispValTmpPQS[1] = DispValTmpPQS[0]/10000;
		        DispValTmpPQS[2] = DispValTmpPQS[0]%10000;
		        DispValue(2,DispValTmpPQS[2],1,4, FALSE, 0, TRUE, FALSE);
	        }
	        else
	        {
                DispValTmpPQS[0] = (s32)(ElectricEnergy.PosEQT * 100);
		        DispValTmpPQS[1] = DispValTmpPQS[0]/10000;
		        DispValTmpPQS[2] = DispValTmpPQS[0]%10000;
		        DispValue(2,DispValTmpPQS[2],2,4, FALSE, 0, TRUE, FALSE);
	        }
            DispValue(1,DispValTmpPQS[1],0,4, FALSE, 0, TRUE, FALSE);
            HT_WriteUnit(TRUE,PH_ALL,1);
			break;
	    case MENU_INDEX_NegEQT://单位k
	        /***********************************标签页*******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_E);
            HT_WriteValue(Line_R5,1,DISP1_n);
		    HT_WriteValue(Line_R5,2,CHAR_NONE);

		    /***********************************上三屏*******************************************/
            HT_WriteValue(0,0,DISP1_E);
            HT_WriteValue(0,1,DISP1_q);
		    HT_WriteValue(0,2,CHAR_NONE);
		    HT_WriteValue(0,3,DISP1_FU);

	        if(ElectricEnergy.NegEQT > 999999)
	        {
                DispValTmpPQS[0] = (s32)(ElectricEnergy.NegEQT * 10);
		        DispValTmpPQS[1] = DispValTmpPQS[0]/10000;
		        DispValTmpPQS[2] = DispValTmpPQS[0]%10000;
		        DispValue(2,DispValTmpPQS[2],1,4, FALSE, 0, TRUE, FALSE);
	        }
	        else
	        {
                DispValTmpPQS[0] = (s32)(ElectricEnergy.NegEQT * 100);
		        DispValTmpPQS[1] = DispValTmpPQS[0]/10000;
		        DispValTmpPQS[2] = DispValTmpPQS[0]%10000;
		        DispValue(2,DispValTmpPQS[2],2,4, FALSE, 0, TRUE, FALSE);
	        }
            DispValue(1,DispValTmpPQS[1],0,4, FALSE, 0, TRUE, FALSE);
            HT_WriteUnit(TRUE,PH_ALL,1);
			break;
	    case MENU_INDEX_Uimb:
            /***********************************标签页*******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_I);
            HT_WriteValue(Line_R5,1,DISP1_M);
		    HT_WriteValue(Line_R5,2,DISP1_b);

		    /***********************************上三屏*******************************************/
		    DispValue(0,vg_Power2_Val.U_Imb, 2, 4, FALSE,0, FALSE, FALSE);
            HT_WriteUnit(TRUE,PH_A,DISP_UNIT_PerV);
            DispValue(2,vg_Power2_Val.I_Imb, 2, 4, FALSE,0, FALSE, FALSE);
            HT_WriteUnit(TRUE,PH_C,DISP_UNIT_PerA);
            break;
	    case MENU_INDEX_HarmU: // 电压总谐波
	        /***********************************标签页*******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_T);
            HT_WriteValue(Line_R5,1,DISP1_H);
		    HT_WriteValue(Line_R5,2,DISP1_d);

		    /***********************************上三屏*******************************************/
	        if(g_tParam.CtrlParam.NetMode == N34)
	        {
                DispValue(0,vg_Power_Val.Real2_Val[0].THDU, 2, 4, FALSE,0, FALSE, FALSE);
    			DispValue(1,vg_Power_Val.Real2_Val[1].THDU, 2, 4, FALSE,0, FALSE, FALSE);
    			DispValue(2,vg_Power_Val.Real2_Val[2].THDU, 2, 4, FALSE,0, FALSE, FALSE);
    			HT_WritePhase(TRUE,PH_TH);
    			HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_PerV);
	        }
	        else
	        {
                DispValue(0,vg_Power_Val.Real2_Val[0].THDU, 2, 4, FALSE,0, FALSE, FALSE);
    			DispValue(1,vg_Power_Val.Real2_Val[2].THDU, 2, 4, FALSE,0, FALSE, FALSE);
    			HT_Write4Value(2,DISP_FU,DISP_FU,DISP_FU,DISP_FU);
    			HT_WritePhaseDouble(TRUE,PH_A);
    			HT_WritePhaseDouble(TRUE,PH_B);
    			HT_WriteUnit(TRUE,PH_A,DISP_UNIT_PerV);
    			HT_WriteUnit(TRUE,PH_B,DISP_UNIT_PerV);
	        }
			HT_WriteValue(4,R4_L1,CHAR_t);
            HT_WriteValue(4,R4_L2,CHAR_H);
            HT_WriteValue(4,R4_L3,CHAR_d);
			break;
       	case MENU_INDEX_HarmI:  // 电流总谐波
            /***********************************标签页*******************************************/
		    HT_WriteValue(Line_R5,0,DISP1_T);
            HT_WriteValue(Line_R5,1,DISP1_H);
		    HT_WriteValue(Line_R5,2,DISP1_d);

		    /***********************************上三屏*******************************************/
			DispValue(0,vg_Power_Val.Real2_Val[0].THDI, 2, 4, FALSE,0, FALSE, FALSE);
			DispValue(1,vg_Power_Val.Real2_Val[1].THDI, 2, 4, FALSE,0, FALSE, FALSE);
			DispValue(2,vg_Power_Val.Real2_Val[2].THDI, 2, 4, FALSE,0, FALSE, FALSE);
			HT_WritePhase(TRUE,PH_TH);
			HT_WriteUnit(TRUE,PH_TH,DISP_UNIT_PerA);
			HT_WriteValue(4,R4_L1,CHAR_t);
            HT_WriteValue(4,R4_L2,CHAR_H);
            HT_WriteValue(4,R4_L3,CHAR_d);
			break;
	    case MENU_INDEX_HarmU3:
	        DispHarmonicContent(TRUE,3);
            break;
        case MENU_INDEX_HarmU5:
	        DispHarmonicContent(TRUE,5);
            break;
        case MENU_INDEX_HarmU7:
	        DispHarmonicContent(TRUE,7);
            break;
        case MENU_INDEX_HarmU9:
	        DispHarmonicContent(TRUE,9);
            break;
        case MENU_INDEX_HarmU11:
	        DispHarmonicContent(TRUE,11);
            break;
        case MENU_INDEX_HarmU13:
	        DispHarmonicContent(TRUE,13);
            break;
        case MENU_INDEX_HarmU15:
	        DispHarmonicContent(TRUE,15);
            break;
        case MENU_INDEX_HarmU17:
	        DispHarmonicContent(TRUE,17);
            break;
        case MENU_INDEX_HarmU19:
	        DispHarmonicContent(TRUE,19);
            break;
        case MENU_INDEX_HarmU21:
	        DispHarmonicContent(TRUE,21);
            break;
        case MENU_INDEX_HarmU23:
	        DispHarmonicContent(TRUE,23);
            break;
        case MENU_INDEX_HarmU25:
	        DispHarmonicContent(TRUE,25);
            break;
        case MENU_INDEX_HarmU27:
	        DispHarmonicContent(TRUE,27);
            break;
        case MENU_INDEX_HarmU29:
	        DispHarmonicContent(TRUE,29);
            break;
        case MENU_INDEX_HarmU31:
	        DispHarmonicContent(TRUE,31);
            break;
        case MENU_INDEX_HarmI3:
	        DispHarmonicContent(FALSE,3);
            break;
        case MENU_INDEX_HarmI5:
	        DispHarmonicContent(FALSE,5);
            break;
        case MENU_INDEX_HarmI7:
	        DispHarmonicContent(FALSE,7);
            break;
        case MENU_INDEX_HarmI9:
	        DispHarmonicContent(FALSE,9);
            break;
        case MENU_INDEX_HarmI11:
	        DispHarmonicContent(FALSE,11);
            break;
        case MENU_INDEX_HarmI13:
	        DispHarmonicContent(FALSE,13);
            break;
        case MENU_INDEX_HarmI15:
	        DispHarmonicContent(FALSE,15);
            break;
        case MENU_INDEX_HarmI17:
	        DispHarmonicContent(FALSE,17);
            break;
        case MENU_INDEX_HarmI19:
	        DispHarmonicContent(FALSE,19);
            break;
        case MENU_INDEX_HarmI21:
	        DispHarmonicContent(FALSE,21);
            break;
        case MENU_INDEX_HarmI23:
	        DispHarmonicContent(FALSE,23);
            break;
        case MENU_INDEX_HarmI25:
	        DispHarmonicContent(FALSE,25);
            break;
        case MENU_INDEX_HarmI27:
	        DispHarmonicContent(FALSE,27);
            break;
        case MENU_INDEX_HarmI29:
	        DispHarmonicContent(FALSE,29);
            break;
        case MENU_INDEX_HarmI31:
            HT_ClearLeftSign(TRUE);
            /**************标签页********************************************************/
		    HT_WriteValue(Line_R5,0,DISP1_T);
            HT_WriteValue(Line_R5,1,DISP1_H);
		    HT_WriteValue(Line_R5,2,DISP1_d);
		    /*****************************************************************************/
	        DispHarmonicContent(FALSE,31);

	        HT_WriteValue(Line_R41,0,CHAR_NONE);
	        HT_WriteValue(Line_R41,1,CHAR_NONE);
	        HT_WriteValue(Line_R41,2,CHAR_NONE);
            break;
        case MENU_INDEX_UAngle:
            Reset_Screen();
            /**************标签页********************************************************/
		    HT_WriteValue(Line_R5,0,DISP1_d);
            HT_WriteValue(Line_R5,1,DISP1_E);
		    HT_WriteValue(Line_R5,2,DISP1_G);
		    /*****************************************************************************/
            if(g_tParam.CtrlParam.NetMode == N34)
            {
                DispValue(0,Phase_U[0], 1, 4, FALSE,0, FALSE, FALSE);
			    DispValue(1,Phase_U[1], 1, 4, FALSE,0, FALSE, FALSE);
			    DispValue(2,Phase_U[2], 1, 4, FALSE,0, FALSE, FALSE);
			    HT_WritePhase(TRUE,PH_TH);
            }
            else
            {
                DispValue(0,Phase_U[0], 1, 4, FALSE,0, FALSE, FALSE);
			    DispValue(1,Phase_U[2], 1, 4, FALSE,0, FALSE, FALSE);
			    HT_WriteValue(2,0,DISP_FU);
                HT_WriteValue(2,1,DISP_FU);
                HT_WriteValue(2,2,DISP_FU);
                HT_WriteValue(2,3,DISP_FU);
			    HT_WritePhaseDouble(TRUE,PH_A);
			    HT_WritePhaseDouble(TRUE,PH_B);
            }
            HT_WriteValue(4,R4_L1,CHAR_V);
            HT_WriteValue(4,R4_L2,CHAR_O);
            HT_WriteValue(4,R4_L3,CHAR_L);
            break;
        case MENU_INDEX_IAngle:
            Reset_Screen();
            /**************标签页********************************************************/
		    HT_WriteValue(Line_R5,0,DISP1_d);
            HT_WriteValue(Line_R5,1,DISP1_E);
		    HT_WriteValue(Line_R5,2,DISP1_G);
		    /*****************************************************************************/
            DispValue(0,Phase_I[0], 1, 4, FALSE,0, FALSE, FALSE);
			DispValue(1,Phase_I[1], 1, 4, FALSE,0, FALSE, FALSE);
			DispValue(2,Phase_I[2], 1, 4, FALSE,0, FALSE, FALSE);
			HT_WritePhase(TRUE,PH_TH);
            HT_WriteValue(4,R4_L1,CHAR_C);
            HT_WriteValue(4,R4_L2,CHAR_U);
            HT_WriteValue(4,R4_L3,CHAR_R);
            break;
		default :
			break;
	}
}


void DispMenu_Menu1(void) //一级界面(备用)
{
    Reset_Screen();
    switch (sg_DispSubMainMenuIndex)
	{
		case MENUSUB_INDEX_READ:
		    /**************标签页********************************************************/
	        HT_WriteValue(Line_R5,0,DISP1_r);
            HT_WriteValue(Line_R5,1,DISP1_d);
	        HT_WriteValue(Line_R5,2,CHAR_NONE);
	        /*****************************************************************************/
			HT_Write4Value(0,DISP1_r,DISP1_E,DISP1_A,DISP1_d);
			break;
		case MENUSUB_INDEX_PROG:
		    /**************标签页********************************************************/
	        HT_WriteValue(Line_R5,0,DISP1_p);
            HT_WriteValue(Line_R5,1,DISP1_r);
	        HT_WriteValue(Line_R5,2,CHAR_NONE);
	        /*****************************************************************************/
		    HT_Write4Value(0,DISP1_p,DISP1_r,DISP1_o,DISP1_G);
            break;
		default :
			break;
	}
}

void DispMenu_Menu2(void) //二级界面 设置选项(4)
{
    Reset_Screen();
    if(DispMode == ReadMode)
    {
        /**************标签页********************************************************/
        HT_WriteValue(Line_R5,0,DISP1_r);
        HT_WriteValue(Line_R5,1,DISP1_d);
        HT_WriteValue(Line_R5,2,CHAR_NONE);
        /*****************************************************************************/
    }
    else if(DispMode == ProgMode)
    {
        /**************标签页********************************************************/
        HT_WriteValue(Line_R5,0,DISP1_p);
        HT_WriteValue(Line_R5,1,DISP1_r);
        HT_WriteValue(Line_R5,2,CHAR_NONE);
        /*****************************************************************************/
    }
	switch(sg_DispSetMenu1Index)
	{
		case MENU1_INDEX_INPT:
			HT_Write4Value(0,DISP1_I,DISP1_n,DISP1_p,DISP1_T);
		    break;
		case MENU1_INDEX_COM1:
			HT_Write4Value(0,DISP1_C,DISP1_o,DISP1_M,DISP1_1);
		    break;
		case MENU1_INDEX_SYS:
			HT_Write4Value(0,DISP1_NC,DISP1_s,DISP1_Y,DISP1_s);
		    break;
		case MENU1_INDEX_DO1:
			HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_1);
		    break;
		case MENU1_INDEX_DO2:
			HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_2);
		    break;
		case MENU1_INDEX_DO3:
			HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_3);
		    break;
		case MENU1_INDEX_DO4:
			HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_4);
		    break;
		case MENU1_INDEX_AO1:
			HT_Write4Value(0,DISP1_A,DISP1_o,DISP1_FU,DISP1_1);
		    break;
		case MENU1_INDEX_AO2:
			HT_Write4Value(0,DISP1_A,DISP1_o,DISP1_FU,DISP1_2);
		    break;
		case MENU1_INDEX_VER:
			HT_Write4Value(0,DISP1_NC,DISP1_U,DISP1_E,DISP1_r);
			HT_Write4Value(2,DISP_1,DISP_8,DISP_0,DISP_1);
		    break;
		default:
		    break;
	}
}


void DispMenu_Menu3(void) //三级界面
{
    Reset_Screen();
    if(DispMode == ReadMode)
    {
        /**************标签页********************************************************/
        HT_WriteValue(Line_R5,0,DISP1_r);
        HT_WriteValue(Line_R5,1,DISP1_d);
        HT_WriteValue(Line_R5,2,CHAR_NONE);
        /*****************************************************************************/
    }
    else if(DispMode == ProgMode)
    {
        /**************标签页********************************************************/
        HT_WriteValue(Line_R5,0,DISP1_p);
        HT_WriteValue(Line_R5,1,DISP1_r);
        HT_WriteValue(Line_R5,2,CHAR_NONE);
        /*****************************************************************************/
    }
    if(DispMode == ProgMode)
    {
        switch(sg_DispSetMenu1Index)
    	{

    		case MENU1_INDEX_INPT:
    			HT_Write4Value(0,DISP1_I,DISP1_n,DISP1_p,DISP1_T);
    			switch(sg_DispSetMenu2_1_Index)
    			{
    			    case MENU2_1_INDEX_NET:
                        HT_Write4Value(1,DISP_NC,DISP_n,DISP_E,DISP_t);
                        break;
    				case MENU2_1_INDEX_PT:
    					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_p,DISP_t);
    				    break;
    				case MENU2_1_INDEX_CT:
    					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_C,DISP_t);
    				    break;
    				default:
    				    break;
    			}
    		    break;
            case MENU1_INDEX_COM1:
                HT_Write4Value(0,DISP1_C,DISP1_o,DISP1_M,DISP1_1);
    			switch(sg_DispSetMenu2_2_Index)
    			{
    				case MENU2_2_INDEX_ADD:
                        HT_Write4Value(1,DISP_a,DISP_d,DISP_d,DISP_r);
    				    break;
    				case MENU2_2_INDEX_BAUD:
    					HT_Write4Value(1,DISP_b,DISP_a,DISP_U,DISP_d);
    			    	break;
    				case MENU2_2_INDEX_DATA:
    					HT_Write4Value(1,DISP_d,DISP_a,DISP_t,DISP_a);
    				    break;
    				case MENU2_2_INDEX_COMS:
    					HT_Write4Value(1,DISP_C,DISP_o,DISP_m,DISP_s);
    				    break;
    				default:
    				    break;
    			}
    		    break;
            case MENU1_INDEX_SYS:
                HT_Write4Value(0,DISP1_NC,DISP1_s,DISP1_Y,DISP1_s);
    			switch(sg_DispSetMenu2_3_Index)
    			{
    				case MENU2_3_INDEX_CYC:
    					HT_Write4Value(1,DISP_NC,DISP_C,DISP_Y,DISP_C);
    				    break;
    				case MENU2_3_INDEX_LIGH:
    					HT_Write4Value(1,DISP_L,DISP_I,DISP_G,DISP_H);
    				    break;
    				case MENU2_3_INDEX_DISP:
    					HT_Write4Value(1,DISP_d,DISP_I,DISP_s,DISP_p);
    				    break;
    				case MENU2_3_INDEX_ALR:
    					HT_Write4Value(1,DISP_NC,DISP_a,DISP_L,DISP_r);
    				    break;
    				case MENU2_3_INDEX_CLRE:
    					HT_Write4Value(1,DISP_C,DISP_L,DISP_r,DISP_E);
    					HT_WriteDot(1,2,TRUE);
    				    break;
    				case MENU2_3_INDEX_CODE:
    					HT_Write4Value(1,DISP_C,DISP_o,DISP_d,DISP_E);
    				    break;
    				default:
    				    break;
    			}
    		    break;
    		case MENU1_INDEX_DO1:
                HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_1);
    			switch(sg_DispSetMenu2_4_Index)
    			{
    				case MENU2_4_INDEX_MODE:
                        HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
    				    break;
    				case MENU2_4_INDEX_TIME:
    					HT_Write4Value(1,DISP_t,DISP_I,DISP_m,DISP_E);
    			    	break;
    				case MENU2_4_INDEX_ITEM:
    					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);
    			    	break;
    				case MENU2_4_INDEX_UAL:
    					HT_Write4Value(1,DISP_NC,DISP_U,DISP_a,DISP_L);
    				    break;
    				case MENU2_4_INDEX_HYS:
    					HT_Write4Value(1,DISP_NC,DISP_H,DISP_Y,DISP_s);
    				    break;
    				case MENU2_4_INDEX_DELAY:
    					HT_Write4Value(1,DISP_d,DISP_E,DISP_L,DISP_Y);
    				    break;
    				default:
    				    break;
    			}
    		    break;
    		case MENU1_INDEX_DO2:
                HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_2);
    			switch(sg_DispSetMenu2_5_Index)
    			{
    				case MENU2_5_INDEX_MODE:
                        HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
    				    break;
    				case MENU2_5_INDEX_TIME:
    					HT_Write4Value(1,DISP_t,DISP_I,DISP_m,DISP_E);
    			    	break;
    				case MENU2_5_INDEX_ITEM:
    					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);
    			    	break;
    				case MENU2_5_INDEX_UAL:
    					HT_Write4Value(1,DISP_NC,DISP_U,DISP_a,DISP_L);
    				    break;
    				case MENU2_5_INDEX_HYS:
    					HT_Write4Value(1,DISP_NC,DISP_H,DISP_Y,DISP_s);
    				    break;
    				case MENU2_5_INDEX_DELAY:
    					HT_Write4Value(1,DISP_d,DISP_E,DISP_L,DISP_Y);
    				    break;
    				default:
    				    break;
    			}
    		    break;
    		case MENU1_INDEX_DO3:
                HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_3);
    			switch(sg_DispSetMenu2_6_Index)
    			{
    				case MENU2_6_INDEX_MODE:
                        HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
    				    break;
    				case MENU2_6_INDEX_TIME:
    					HT_Write4Value(1,DISP_t,DISP_I,DISP_m,DISP_E);
    			    	break;
    				case MENU2_6_INDEX_ITEM:
    					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);
    			    	break;
    				case MENU2_6_INDEX_UAL:
    					HT_Write4Value(1,DISP_NC,DISP_U,DISP_a,DISP_L);
    				    break;
    				case MENU2_6_INDEX_HYS:
    					HT_Write4Value(1,DISP_NC,DISP_H,DISP_Y,DISP_s);
    				    break;
    				case MENU2_6_INDEX_DELAY:
    					HT_Write4Value(1,DISP_d,DISP_E,DISP_L,DISP_Y);
    				    break;
    				default:
    				    break;
    			}
    		    break;
    		case MENU1_INDEX_DO4:
                HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_4);
    			switch(sg_DispSetMenu2_7_Index)
    			{
    				case MENU2_7_INDEX_MODE:
                        HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
    				    break;
    				case MENU2_7_INDEX_TIME:
    					HT_Write4Value(1,DISP_t,DISP_I,DISP_m,DISP_E);
    			    	break;
    				case MENU2_7_INDEX_ITEM:
    					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);
    			    	break;
    				case MENU2_7_INDEX_UAL:
    					HT_Write4Value(1,DISP_NC,DISP_U,DISP_a,DISP_L);
    				    break;
    				case MENU2_7_INDEX_HYS:
    					HT_Write4Value(1,DISP_NC,DISP_H,DISP_Y,DISP_s);
    				    break;
    				case MENU2_7_INDEX_DELAY:
    					HT_Write4Value(1,DISP_d,DISP_E,DISP_L,DISP_Y);
    				    break;
    				default:
    				    break;
    			}
    		    break;
    		case MENU1_INDEX_AO1:
                HT_Write4Value(0,DISP1_A,DISP1_o,DISP1_FU,DISP1_1);
    			switch(sg_DispSetMenu2_8_Index)
    			{
    				case MENU2_8_INDEX_MODE:
                        HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
    				    break;
    				case MENU2_8_INDEX_ITEM:
    					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);
    			    	break;
    				case MENU2_8_INDEX_DS:
    					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_d,DISP_s);
    				    break;
    				case MENU2_8_INDEX_FS:
    					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_F,DISP_s);
    				    break;
    				default:
    				    break;
    			}
    		    break;
    		case MENU1_INDEX_AO2:
                HT_Write4Value(0,DISP1_A,DISP1_o,DISP1_FU,DISP1_2);
    			switch(sg_DispSetMenu2_9_Index)
    			{
    				case MENU2_9_INDEX_MODE:
                        HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
    				    break;
    				case MENU2_9_INDEX_ITEM:
    					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);
    			    	break;
    				case MENU2_9_INDEX_DS:
    					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_d,DISP_s);
    				    break;
    				case MENU2_9_INDEX_FS:
    					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_F,DISP_s);
    				    break;
    				default:
    				    break;
    			}
    		    break;
    		default:
    		    break;
    	}
    }
    else if(DispMode == ReadMode)
    {
        u8 Decimal = 0;
        sg_bSetFlashEnable = FALSE; //查看选项，不闪烁
        switch(sg_DispSetMenu1Index)
    	{
    		case MENU1_INDEX_INPT:
    			HT_Write4Value(0,DISP1_I,DISP1_n,DISP1_p,DISP1_T);
    			switch(sg_DispSetMenu2_1_Index)
    			{
                    case MENU2_1_INDEX_NET:
                        HT_Write4Value(1,DISP_NC,DISP_n,DISP_E,DISP_t);
                        if( sg_DispSetValue == N34)
                        {
                            if(!sg_bSetFlashEnable)
                    			HT_Write4Value(2,DISP_NC,DISP_n,DISP_3,DISP_4);
                    		else
                    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
                        }
                        else if( sg_DispSetValue == N33)
                        {
                            if(!sg_bSetFlashEnable)
                    			HT_Write4Value(2,DISP_NC,DISP_n,DISP_3,DISP_3);
                    		else
                    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
                        }
                        break;
    				case MENU2_1_INDEX_PT:
    					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_p,DISP_t);

    					Decimal =  0;
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal, Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
    					CharToNumber();
    				    break;
    				case MENU2_1_INDEX_CT:
    					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_C,DISP_t);

    					Decimal = 0;
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
    					CharToNumber();
    				    break;
    				default:
    				    break;
    			}
    		    break;
    	    case MENU1_INDEX_COM1:
    	        HT_Write4Value(0,DISP1_C,DISP1_o,DISP1_M,DISP1_1);
    			switch(sg_DispSetMenu2_2_Index)
    			{
    				case MENU2_2_INDEX_ADD:
    					HT_Write4Value(1,DISP_a,DISP_d,DISP_d,DISP_r);
    					Decimal =  0;
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				case MENU2_2_INDEX_BAUD:
    					HT_Write4Value(1,DISP_b,DISP_a,DISP_U,DISP_d);
    					if(sg_bSetFlashEnable)
    					{
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    					else
    					{
                            switch(sg_DispSetValue)
        					{
                                case Baud12:
                                    HT_Write4Value(2,DISP_1,DISP_2,DISP_0,DISP_0);
                                    //HT_WriteDot(2,2,FALSE);
                                    break;
                                case Baud24:
                                    HT_Write4Value(2,DISP_2,DISP_4,DISP_0,DISP_0);
                                    break;
                                case Baud48:
                                    HT_Write4Value(2,DISP_4,DISP_8,DISP_0,DISP_0);
                                    break;
                                case Baud96:
                                    HT_Write4Value(2,DISP_9,DISP_6,DISP_0,DISP_0);
                                    //HT_WriteDot(2,2,FALSE);
                                    break;
                                case Baud192:
                                    HT_Write4Value(2,DISP_1,DISP_9,DISP_2,DISP_0);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                default:
                                    break;
        					}
    					}
    			    	break;
    				case MENU2_2_INDEX_DATA:
    					HT_Write4Value(1,DISP_d,DISP_a,DISP_t,DISP_a);
    					if(sg_DispSetValue == n81)
    					{
                            if(!sg_bSetFlashEnable)
                    			HT_Write4Value(2,DISP_NC,DISP_n,DISP_8,DISP_1);
                    		else
                    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    					else if( sg_DispSetValue == o81)
    					{
                            if(!sg_bSetFlashEnable)
                    			HT_Write4Value(2,DISP_NC,DISP_o,DISP_8,DISP_1);
                    		else
                    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    					else if( sg_DispSetValue == e81)
    					{
                            if(!sg_bSetFlashEnable)
                    			HT_Write4Value(2,DISP_NC,DISP_E,DISP_8,DISP_1);
                    		else
                    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    					else if( sg_DispSetValue == n82)
    					{
                            if(!sg_bSetFlashEnable)
                    			HT_Write4Value(2,DISP_NC,DISP_n,DISP_8,DISP_2);
                    		else
                    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    				    break;
    				case MENU2_2_INDEX_COMS:
    					HT_Write4Value(1,DISP_C,DISP_o,DISP_m,DISP_s);
    					if(sg_DispSetValue == rtu485)
    					{
                            if(!sg_bSetFlashEnable)
                    			HT_Write4Value(2,DISP_NC,DISP_r,DISP_t,DISP_U);
                    		else
                    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    					else if( sg_DispSetValue == calib)
    					{
                            if(!sg_bSetFlashEnable)
                    			HT_Write4Value(2,DISP_NC,DISP_C,DISP_a,DISP_L);
                    		else
                    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    				    break;
    				default:
    				    break;
    			}
    		    break;
            case MENU1_INDEX_SYS:
                HT_Write4Value(0,DISP1_NC,DISP1_s,DISP1_Y,DISP1_s);
                switch(sg_DispSetMenu2_3_Index)
    			{
                    case MENU2_3_INDEX_CYC:
    					HT_Write4Value(1,DISP_NC,DISP_C,DISP_Y,DISP_C);

    					if(sg_DispSetValue == SaveYes)
    					{
                            if(!sg_bSetFlashEnable)
                    			HT_Write4Value(2,DISP_NC,DISP_Y,DISP_E,DISP_s);
                    		else
                    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    					else if( sg_DispSetValue == SaveNO)
    					{
                            if(!sg_bSetFlashEnable)
                    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_n,DISP_o);
                    		else
                    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    				    break;
    				case MENU2_3_INDEX_LIGH:        // 背光时间
    					HT_Write4Value(1,DISP_L,DISP_I,DISP_G,DISP_H);

    					Decimal =  0;
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
    					CharToNumber();
    				    break;
    				case MENU2_3_INDEX_DISP:
    					HT_Write4Value(1,DISP_d,DISP_I,DISP_s,DISP_p);

    					if(!sg_bSetFlashEnable)
    					{
                            if(sg_DispSetValue == DISP_UINT_U)
                                HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_U);
                            else if(sg_DispSetValue == DISP_UINT_I)
                                HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_I);
                            else if(sg_DispSetValue == DISP_UINT_P)
                                HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_p);
                            else if(sg_DispSetValue == DISP_UINT_PF)
                                HT_Write4Value(2,DISP_NC,DISP_NC,DISP_p,DISP_F);
                            else if(sg_DispSetValue == DISP_UINT_EP)
                                HT_Write4Value(2,DISP_NC,DISP_NC,DISP_E,DISP_p);
                            else if(sg_DispSetValue == DISP_UINT_THD)
                                HT_Write4Value(2,DISP_NC,DISP_t,DISP_H,DISP_d);
                            else if(sg_DispSetValue == DISP_UINT_DEM)
                                HT_Write4Value(2,DISP_NC,DISP_d,DISP_E,DISP_m);
    					}
    					else
    					{
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    					break;
    			    case MENU2_3_INDEX_ALR:
    					HT_Write4Value(1,DISP_NC,DISP_a,DISP_L,DISP_r);
    					Decimal =  0;
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
    					CharToNumber();
    				    break;
    				case MENU2_3_INDEX_CLRE:
    					HT_Write4Value(1,DISP_C,DISP_L,DISP_r,DISP_E);
    					HT_WriteDot(1,2,TRUE);

                        if(sg_DispSetValue == SaveYes)
    					{
                            if(!sg_bSetFlashEnable)
                    			HT_Write4Value(2,DISP_NC,DISP_Y,DISP_E,DISP_s);
                    		else
                    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    					else if( sg_DispSetValue == SaveNO)
    					{
                            if(!sg_bSetFlashEnable)
                    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_n,DISP_o);
                    		else
                    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    				    break;
    				case MENU2_3_INDEX_CODE:
    					HT_Write4Value(1,DISP_C,DISP_o,DISP_d,DISP_E);

    					Decimal =  0;
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				default:
    				    break;
                }
    		    break;
    		case MENU1_INDEX_DO1:
                HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_1);
                switch(sg_DispSetMenu2_4_Index)
    			{
                    case MENU2_4_INDEX_MODE:
                        HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
                        if(!sg_bSetFlashEnable)
    					{
                            if(sg_DispSetValue == DO_MODE_OFF)
                                HT_Write4Value(2,DISP_NC,DISP_o,DISP_F,DISP_F);
                            else if(sg_DispSetValue == DO_MODE_ALR)
                                HT_Write4Value(2,DISP_NC,DISP_a,DISP_L,DISP_r);
                            else if(sg_DispSetValue == DO_MODE_REM)
                                HT_Write4Value(2,DISP_NC,DISP_r,DISP_E,DISP_m);
    					}
    					else
    					{
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    				    break;
    				case MENU2_4_INDEX_TIME:
    					HT_Write4Value(1,DISP_t,DISP_I,DISP_m,DISP_E);

                        Decimal =  2;
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    			    	break;
    				case MENU2_4_INDEX_ITEM:
    					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);

    					if(!sg_bSetFlashEnable)
    					{
    					    switch(sg_DispSetValue)
    					    {
    							case DO_ITEM_Ua_H:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ua_L:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ub_H:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ub_L:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Uc_H:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Uc_L:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Uavg_H:
                                    HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_Uavg_L:
                                    HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*-------------------------------------------------------*/
    							case DO_ITEM_Uab_H:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Uab_L:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Ubc_H:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Ubc_L:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Uca_H:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Uca_L:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ULavg_H:
                                    HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_ULavg_L:
                                    HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                /*-------------------------------------------------------*/
                                case DO_ITEM_Ia_H:
                                    HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ia_L:
                                    HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ib_H:
                                    HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ib_L:
                                    HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ic_H:
                                    HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ic_L:
                                    HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Iavg_H:
                                    HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_Iavg_L:
                                    HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*------------------------------------------------------*/
                                case DO_ITEM_Pa_H:
                                    HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pa_L:
                                    HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pb_H:
                                    HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pb_L:
                                    HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pc_H:
                                    HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pc_L:
                                    HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_P_H:
                                    HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_P_L:
                                    HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_Qa_H:
                                    HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qa_L:
                                    HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qb_H:
                                    HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qb_L:
                                    HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qc_H:
                                    HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qc_L:
                                    HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Q_H:
                                    HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_Q_L:
                                    HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*------------------------------------------------------*/
                                case DO_ITEM_Sa_H:
                                    HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sa_L:
                                    HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sb_H:
                                    HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sb_L:
                                    HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sc_H:
                                    HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sc_L:
                                    HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_S_H:
                                    HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_S_L:
                                    HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*------------------------------------------------------*/
                                case DO_ITEM_PFa_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFa_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFb_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFb_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFc_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFc_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PF_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_PF_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_F_H:
                                    HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_F_L:
                                    HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_UTHa_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_a);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_UTHb_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_b);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_UTHc_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_C);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_UTH_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_NC);
                                    //HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_ITHa_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_a);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ITHb_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_b);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ITHc_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_C);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ITH_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_NC);
                                    //HT_WriteDot(2,1,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_IMBU_H:
                                    HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_U);
                                    //HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_IMBI_H:
                                    HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_I);
                                    //HT_WriteDot(2,2,TRUE);
                                    break;
                                /*case DO_ITEM_Io_H:
                                    HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                    case DO_ITEM_Io_L:
                                    HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;*/
                                /*-----------------------------------------------------*/

                                case DO_ITEM_DI1_H:
                                    HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI1_L:
                                    HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_0);
                                    break;
                                case DO_ITEM_DI2_H:
                                    HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI2_L:
                                    HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_0);
                                    break;
                                case DO_ITEM_DI3_H:
                                    HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI3_L:
                                    HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_0);
                                    break;
                                case DO_ITEM_DI4_H:
                                    HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI4_L:
                                    HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_0);
                                    break;
                                default:
                                    break;
    					    }
    					}
    					else
    					{
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    			    	break;
    				case MENU2_4_INDEX_UAL:
    					HT_Write4Value(1,DISP_NC,DISP_U,DISP_a,DISP_L);

                        switch(DispCtrlParam.DO1_Item)
    				    {
    						case DO_ITEM_Ua_H:
                            case DO_ITEM_Ua_L:
                            case DO_ITEM_Ub_H:
                            case DO_ITEM_Ub_L:
                            case DO_ITEM_Uc_H:
                            case DO_ITEM_Uc_L:
                            case DO_ITEM_Uavg_H:
                            case DO_ITEM_Uavg_L:
                            /*-------------------------------------------------------*/
    						case DO_ITEM_Uab_H:
                            case DO_ITEM_Uab_L:
                            case DO_ITEM_Ubc_H:
                            case DO_ITEM_Ubc_L:
                            case DO_ITEM_Uca_H:
                            case DO_ITEM_Uca_L:
                            case DO_ITEM_ULavg_H:
                            case DO_ITEM_ULavg_L:
                                Decimal = 1;
                                break;
                            /*-------------------------------------------------------*/
                            case DO_ITEM_Ia_H:
                            case DO_ITEM_Ia_L:
                            case DO_ITEM_Ib_H:
                            case DO_ITEM_Ib_L:
                            case DO_ITEM_Ic_H:
                            case DO_ITEM_Ic_L:
                            case DO_ITEM_Iavg_H:
                            case DO_ITEM_Iavg_L:
                                Decimal = 3;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Pa_H:
                            case DO_ITEM_Pa_L:
                            case DO_ITEM_Pb_H:
                            case DO_ITEM_Pb_L:
                            case DO_ITEM_Pc_H:
                            case DO_ITEM_Pc_L:
                            case DO_ITEM_P_H:
                            case DO_ITEM_P_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_Qa_H:
                            case DO_ITEM_Qa_L:
                            case DO_ITEM_Qb_H:
                            case DO_ITEM_Qb_L:
                            case DO_ITEM_Qc_H:
                            case DO_ITEM_Qc_L:
                            case DO_ITEM_Q_H:
                            case DO_ITEM_Q_L:
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Sa_H:
                            case DO_ITEM_Sa_L:
                            case DO_ITEM_Sb_H:
                            case DO_ITEM_Sb_L:
                            case DO_ITEM_Sc_H:
                            case DO_ITEM_Sc_L:
                            case DO_ITEM_S_H:
                            case DO_ITEM_S_L:
                                Decimal = 0;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_PFa_H:
                            case DO_ITEM_PFa_L:
                            case DO_ITEM_PFb_H:
                            case DO_ITEM_PFb_L:
                            case DO_ITEM_PFc_H:
                            case DO_ITEM_PFc_L:
                            case DO_ITEM_PF_H:
                            case DO_ITEM_PF_L:
                                Decimal = 3;
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                            case DO_ITEM_F_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_UTHa_H:
                            case DO_ITEM_UTHb_H:
                            case DO_ITEM_UTHc_H:
                            case DO_ITEM_UTH_H:
                            case DO_ITEM_ITHa_H:
                            case DO_ITEM_ITHb_H:
                            case DO_ITEM_ITHc_H:
                            case DO_ITEM_ITH_H:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_IMBU_H:
                            case DO_ITEM_IMBI_H:
                                Decimal = 2;
                                break;
                            /*case DO_ITEM_Io_H:
                                case DO_ITEM_Io_L:
                                Decimal = 3;
                                break;*/
                            /*-----------------------------------------------------*/
                            default:
                                break;
    				    }
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				case MENU2_4_INDEX_HYS:
    					HT_Write4Value(1,DISP_NC,DISP_H,DISP_Y,DISP_s);
    					switch(DispCtrlParam.DO1_Item)
    				    {
    						case DO_ITEM_Ua_H:
                            case DO_ITEM_Ua_L:
                            case DO_ITEM_Ub_H:
                            case DO_ITEM_Ub_L:
                            case DO_ITEM_Uc_H:
                            case DO_ITEM_Uc_L:
                            case DO_ITEM_Uavg_H:
                            case DO_ITEM_Uavg_L:
                            /*-------------------------------------------------------*/
    						case DO_ITEM_Uab_H:
                            case DO_ITEM_Uab_L:
                            case DO_ITEM_Ubc_H:
                            case DO_ITEM_Ubc_L:
                            case DO_ITEM_Uca_H:
                            case DO_ITEM_Uca_L:
                            case DO_ITEM_ULavg_H:
                            case DO_ITEM_ULavg_L:
                                Decimal = 1;
                                break;
                            /*-------------------------------------------------------*/
                            case DO_ITEM_Ia_H:
                            case DO_ITEM_Ia_L:
                            case DO_ITEM_Ib_H:
                            case DO_ITEM_Ib_L:
                            case DO_ITEM_Ic_H:
                            case DO_ITEM_Ic_L:
                            case DO_ITEM_Iavg_H:
                            case DO_ITEM_Iavg_L:
                                Decimal = 3;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Pa_H:
                            case DO_ITEM_Pa_L:
                            case DO_ITEM_Pb_H:
                            case DO_ITEM_Pb_L:
                            case DO_ITEM_Pc_H:
                            case DO_ITEM_Pc_L:
                            case DO_ITEM_P_H:
                            case DO_ITEM_P_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_Qa_H:
                            case DO_ITEM_Qa_L:
                            case DO_ITEM_Qb_H:
                            case DO_ITEM_Qb_L:
                            case DO_ITEM_Qc_H:
                            case DO_ITEM_Qc_L:
                            case DO_ITEM_Q_H:
                            case DO_ITEM_Q_L:
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Sa_H:
                            case DO_ITEM_Sa_L:
                            case DO_ITEM_Sb_H:
                            case DO_ITEM_Sb_L:
                            case DO_ITEM_Sc_H:
                            case DO_ITEM_Sc_L:
                            case DO_ITEM_S_H:
                            case DO_ITEM_S_L:
                                Decimal = 0;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_PFa_H:
                            case DO_ITEM_PFa_L:
                            case DO_ITEM_PFb_H:
                            case DO_ITEM_PFb_L:
                            case DO_ITEM_PFc_H:
                            case DO_ITEM_PFc_L:
                            case DO_ITEM_PF_H:
                            case DO_ITEM_PF_L:
                                Decimal = 3;
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                            case DO_ITEM_F_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_UTHa_H:
                            case DO_ITEM_UTHb_H:
                            case DO_ITEM_UTHc_H:
                            case DO_ITEM_UTH_H:
                            case DO_ITEM_ITHa_H:
                            case DO_ITEM_ITHb_H:
                            case DO_ITEM_ITHc_H:
                            case DO_ITEM_ITH_H:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_IMBU_H:
                            case DO_ITEM_IMBI_H:
                                Decimal = 2;
                                break;
                            /*case DO_ITEM_Io_H:
                                case DO_ITEM_Io_L:
                                Decimal = 3;
                                break;*/
                            /*-----------------------------------------------------*/
                            default:
                                break;
    				    }
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				case MENU2_4_INDEX_DELAY:
    					HT_Write4Value(1,DISP_d,DISP_E,DISP_L,DISP_Y);
                        Decimal = 2;
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				default:
    				    break;
                }
                break;
            case MENU1_INDEX_DO2:
                HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_2);
                switch(sg_DispSetMenu2_5_Index)
    			{
                    case MENU2_5_INDEX_MODE:
                        HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
                        if(!sg_bSetFlashEnable)
    					{
                            if(sg_DispSetValue == DO_MODE_OFF)
                                HT_Write4Value(2,DISP_NC,DISP_o,DISP_F,DISP_F);
                            else if(sg_DispSetValue == DO_MODE_ALR)
                                HT_Write4Value(2,DISP_NC,DISP_a,DISP_L,DISP_r);
                            else if(sg_DispSetValue == DO_MODE_REM)
                                HT_Write4Value(2,DISP_NC,DISP_r,DISP_E,DISP_m);
    					}
    					else
    					{
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    				    break;
    				case MENU2_5_INDEX_TIME:
    					HT_Write4Value(1,DISP_t,DISP_I,DISP_m,DISP_E);

                        Decimal = 2;
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    			    	break;
    				case MENU2_5_INDEX_ITEM:
    					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);

    					if(!sg_bSetFlashEnable)
    					{
    					    switch(sg_DispSetValue)
    					    {
    							case DO_ITEM_Ua_H:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ua_L:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ub_H:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ub_L:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Uc_H:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Uc_L:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Uavg_H:
                                    HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_Uavg_L:
                                    HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*-------------------------------------------------------*/
    							case DO_ITEM_Uab_H:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Uab_L:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Ubc_H:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Ubc_L:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Uca_H:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Uca_L:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ULavg_H:
                                    HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_ULavg_L:
                                    HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                /*-------------------------------------------------------*/
                                case DO_ITEM_Ia_H:
                                    HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ia_L:
                                    HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ib_H:
                                    HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ib_L:
                                    HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ic_H:
                                    HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ic_L:
                                    HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Iavg_H:
                                    HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_Iavg_L:
                                    HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*------------------------------------------------------*/
                                case DO_ITEM_Pa_H:
                                    HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pa_L:
                                    HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pb_H:
                                    HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pb_L:
                                    HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pc_H:
                                    HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pc_L:
                                    HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_P_H:
                                    HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_P_L:
                                    HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_Qa_H:
                                    HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qa_L:
                                    HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qb_H:
                                    HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qb_L:
                                    HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qc_H:
                                    HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qc_L:
                                    HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Q_H:
                                    HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_Q_L:
                                    HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*------------------------------------------------------*/
                                case DO_ITEM_Sa_H:
                                    HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sa_L:
                                    HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sb_H:
                                    HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sb_L:
                                    HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sc_H:
                                    HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sc_L:
                                    HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_S_H:
                                    HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_S_L:
                                    HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*------------------------------------------------------*/
                                case DO_ITEM_PFa_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFa_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFb_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFb_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFc_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFc_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PF_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_PF_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_F_H:
                                    HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_F_L:
                                    HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_UTHa_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_a);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_UTHb_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_b);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_UTHc_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_C);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_UTH_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_NC);
                                    //HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_ITHa_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_a);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ITHb_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_b);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ITHc_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_C);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ITH_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_NC);
                                    //HT_WriteDot(2,1,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_IMBU_H:
                                    HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_U);
                                    //HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_IMBI_H:
                                    HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_I);
                                    //HT_WriteDot(2,2,TRUE);
                                    break;
                                /*case DO_ITEM_Io_H:
                                    HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                    case DO_ITEM_Io_L:
                                    HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;*/
                                /*-----------------------------------------------------*/

                                case DO_ITEM_DI1_H:
                                    HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI1_L:
                                    HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_0);
                                    break;
                                case DO_ITEM_DI2_H:
                                    HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI2_L:
                                    HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_0);
                                    break;
                                case DO_ITEM_DI3_H:
                                    HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI3_L:
                                    HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_0);
                                    break;
                                case DO_ITEM_DI4_H:
                                    HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI4_L:
                                    HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_0);
                                    break;
                                default:
                                    break;
    					    }
    					}
    					else
    					{
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    					break;
    			    case MENU2_5_INDEX_UAL:
    					HT_Write4Value(1,DISP_NC,DISP_U,DISP_a,DISP_L);

                        switch(DispCtrlParam.DO2_Item)
    				    {
    						case DO_ITEM_Ua_H:
                            case DO_ITEM_Ua_L:
                            case DO_ITEM_Ub_H:
                            case DO_ITEM_Ub_L:
                            case DO_ITEM_Uc_H:
                            case DO_ITEM_Uc_L:
                            case DO_ITEM_Uavg_H:
                            case DO_ITEM_Uavg_L:
                            /*-------------------------------------------------------*/
    						case DO_ITEM_Uab_H:
                            case DO_ITEM_Uab_L:
                            case DO_ITEM_Ubc_H:
                            case DO_ITEM_Ubc_L:
                            case DO_ITEM_Uca_H:
                            case DO_ITEM_Uca_L:
                            case DO_ITEM_ULavg_H:
                            case DO_ITEM_ULavg_L:
                                Decimal = 1;
                                break;
                            /*-------------------------------------------------------*/
                            case DO_ITEM_Ia_H:
                            case DO_ITEM_Ia_L:
                            case DO_ITEM_Ib_H:
                            case DO_ITEM_Ib_L:
                            case DO_ITEM_Ic_H:
                            case DO_ITEM_Ic_L:
                            case DO_ITEM_Iavg_H:
                            case DO_ITEM_Iavg_L:
                                Decimal = 3;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Pa_H:
                            case DO_ITEM_Pa_L:
                            case DO_ITEM_Pb_H:
                            case DO_ITEM_Pb_L:
                            case DO_ITEM_Pc_H:
                            case DO_ITEM_Pc_L:
                            case DO_ITEM_P_H:
                            case DO_ITEM_P_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_Qa_H:
                            case DO_ITEM_Qa_L:
                            case DO_ITEM_Qb_H:
                            case DO_ITEM_Qb_L:
                            case DO_ITEM_Qc_H:
                            case DO_ITEM_Qc_L:
                            case DO_ITEM_Q_H:
                            case DO_ITEM_Q_L:
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Sa_H:
                            case DO_ITEM_Sa_L:
                            case DO_ITEM_Sb_H:
                            case DO_ITEM_Sb_L:
                            case DO_ITEM_Sc_H:
                            case DO_ITEM_Sc_L:
                            case DO_ITEM_S_H:
                            case DO_ITEM_S_L:
                                Decimal = 0;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_PFa_H:
                            case DO_ITEM_PFa_L:
                            case DO_ITEM_PFb_H:
                            case DO_ITEM_PFb_L:
                            case DO_ITEM_PFc_H:
                            case DO_ITEM_PFc_L:
                            case DO_ITEM_PF_H:
                            case DO_ITEM_PF_L:
                                Decimal = 3;
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                            case DO_ITEM_F_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_UTHa_H:
                            case DO_ITEM_UTHb_H:
                            case DO_ITEM_UTHc_H:
                            case DO_ITEM_UTH_H:
                            case DO_ITEM_ITHa_H:
                            case DO_ITEM_ITHb_H:
                            case DO_ITEM_ITHc_H:
                            case DO_ITEM_ITH_H:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_IMBU_H:
                            case DO_ITEM_IMBI_H:
                                Decimal = 2;
                                break;
                            /*case DO_ITEM_Io_H:
                                case DO_ITEM_Io_L:
                                Decimal = 3;
                                break;*/
                            /*-----------------------------------------------------*/
                            default:
                                break;
    				    }
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				case MENU2_5_INDEX_HYS:
    					HT_Write4Value(1,DISP_NC,DISP_H,DISP_Y,DISP_s);
    					switch(DispCtrlParam.DO2_Item)
    				    {
    						case DO_ITEM_Ua_H:
                            case DO_ITEM_Ua_L:
                            case DO_ITEM_Ub_H:
                            case DO_ITEM_Ub_L:
                            case DO_ITEM_Uc_H:
                            case DO_ITEM_Uc_L:
                            case DO_ITEM_Uavg_H:
                            case DO_ITEM_Uavg_L:
                            /*-------------------------------------------------------*/
    						case DO_ITEM_Uab_H:
                            case DO_ITEM_Uab_L:
                            case DO_ITEM_Ubc_H:
                            case DO_ITEM_Ubc_L:
                            case DO_ITEM_Uca_H:
                            case DO_ITEM_Uca_L:
                            case DO_ITEM_ULavg_H:
                            case DO_ITEM_ULavg_L:
                                Decimal = 1;
                                break;
                            /*-------------------------------------------------------*/
                            case DO_ITEM_Ia_H:
                            case DO_ITEM_Ia_L:
                            case DO_ITEM_Ib_H:
                            case DO_ITEM_Ib_L:
                            case DO_ITEM_Ic_H:
                            case DO_ITEM_Ic_L:
                            case DO_ITEM_Iavg_H:
                            case DO_ITEM_Iavg_L:
                                Decimal = 3;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Pa_H:
                            case DO_ITEM_Pa_L:
                            case DO_ITEM_Pb_H:
                            case DO_ITEM_Pb_L:
                            case DO_ITEM_Pc_H:
                            case DO_ITEM_Pc_L:
                            case DO_ITEM_P_H:
                            case DO_ITEM_P_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_Qa_H:
                            case DO_ITEM_Qa_L:
                            case DO_ITEM_Qb_H:
                            case DO_ITEM_Qb_L:
                            case DO_ITEM_Qc_H:
                            case DO_ITEM_Qc_L:
                            case DO_ITEM_Q_H:
                            case DO_ITEM_Q_L:
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Sa_H:
                            case DO_ITEM_Sa_L:
                            case DO_ITEM_Sb_H:
                            case DO_ITEM_Sb_L:
                            case DO_ITEM_Sc_H:
                            case DO_ITEM_Sc_L:
                            case DO_ITEM_S_H:
                            case DO_ITEM_S_L:
                                Decimal = 0;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_PFa_H:
                            case DO_ITEM_PFa_L:
                            case DO_ITEM_PFb_H:
                            case DO_ITEM_PFb_L:
                            case DO_ITEM_PFc_H:
                            case DO_ITEM_PFc_L:
                            case DO_ITEM_PF_H:
                            case DO_ITEM_PF_L:
                                Decimal = 3;
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                            case DO_ITEM_F_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_UTHa_H:
                            case DO_ITEM_UTHb_H:
                            case DO_ITEM_UTHc_H:
                            case DO_ITEM_UTH_H:
                            case DO_ITEM_ITHa_H:
                            case DO_ITEM_ITHb_H:
                            case DO_ITEM_ITHc_H:
                            case DO_ITEM_ITH_H:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_IMBU_H:
                            case DO_ITEM_IMBI_H:
                                Decimal = 2;
                                break;
                            /*case DO_ITEM_Io_H:
                                case DO_ITEM_Io_L:
                                Decimal = 3;
                                break;*/
                            /*-----------------------------------------------------*/
                            default:
                                break;
    				    }
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				case MENU2_5_INDEX_DELAY:
    					HT_Write4Value(1,DISP_d,DISP_E,DISP_L,DISP_Y);
                        Decimal = 2;
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				default:
    				    break;
    			}
    			break;
            case MENU1_INDEX_DO3:
                HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_3);
                switch(sg_DispSetMenu2_6_Index)
    			{
                    case MENU2_6_INDEX_MODE:
                        HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
                        if(!sg_bSetFlashEnable)
    					{
                            if(sg_DispSetValue == DO_MODE_OFF)
                                HT_Write4Value(2,DISP_NC,DISP_o,DISP_F,DISP_F);
                            else if(sg_DispSetValue == DO_MODE_ALR)
                                HT_Write4Value(2,DISP_NC,DISP_a,DISP_L,DISP_r);
                            else if(sg_DispSetValue == DO_MODE_REM)
                                HT_Write4Value(2,DISP_NC,DISP_r,DISP_E,DISP_m);
    					}
    					else
    					{
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    				    break;
    				case MENU2_6_INDEX_TIME:
    					HT_Write4Value(1,DISP_t,DISP_I,DISP_m,DISP_E);

                        Decimal = 2;
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    			    	break;
    				case MENU2_6_INDEX_ITEM:
    					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);

    					if(!sg_bSetFlashEnable)
    					{
    					    switch(sg_DispSetValue)
    					    {
    							case DO_ITEM_Ua_H:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ua_L:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ub_H:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ub_L:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Uc_H:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Uc_L:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Uavg_H:
                                    HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_Uavg_L:
                                    HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*-------------------------------------------------------*/
    							case DO_ITEM_Uab_H:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Uab_L:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Ubc_H:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Ubc_L:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Uca_H:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Uca_L:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ULavg_H:
                                    HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_ULavg_L:
                                    HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                /*-------------------------------------------------------*/
                                case DO_ITEM_Ia_H:
                                    HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ia_L:
                                    HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ib_H:
                                    HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ib_L:
                                    HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ic_H:
                                    HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ic_L:
                                    HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Iavg_H:
                                    HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_Iavg_L:
                                    HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*------------------------------------------------------*/
                                case DO_ITEM_Pa_H:
                                    HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pa_L:
                                    HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pb_H:
                                    HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pb_L:
                                    HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pc_H:
                                    HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pc_L:
                                    HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_P_H:
                                    HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_P_L:
                                    HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_Qa_H:
                                    HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qa_L:
                                    HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qb_H:
                                    HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qb_L:
                                    HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qc_H:
                                    HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qc_L:
                                    HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Q_H:
                                    HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_Q_L:
                                    HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*------------------------------------------------------*/
                                case DO_ITEM_Sa_H:
                                    HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sa_L:
                                    HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sb_H:
                                    HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sb_L:
                                    HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sc_H:
                                    HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sc_L:
                                    HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_S_H:
                                    HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_S_L:
                                    HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*------------------------------------------------------*/
                                case DO_ITEM_PFa_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFa_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFb_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFb_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFc_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFc_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PF_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_PF_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_F_H:
                                    HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_F_L:
                                    HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_UTHa_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_a);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_UTHb_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_b);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_UTHc_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_C);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_UTH_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_NC);
                                    //HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_ITHa_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_a);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ITHb_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_b);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ITHc_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_C);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ITH_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_NC);
                                    //HT_WriteDot(2,1,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_IMBU_H:
                                    HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_U);
                                    //HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_IMBI_H:
                                    HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_I);
                                    //HT_WriteDot(2,2,TRUE);
                                    break;
                                /*case DO_ITEM_Io_H:
                                    HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                    case DO_ITEM_Io_L:
                                    HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;*/
                                /*-----------------------------------------------------*/

                                case DO_ITEM_DI1_H:
                                    HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI1_L:
                                    HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_0);
                                    break;
                                case DO_ITEM_DI2_H:
                                    HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI2_L:
                                    HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_0);
                                    break;
                                case DO_ITEM_DI3_H:
                                    HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI3_L:
                                    HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_0);
                                    break;
                                case DO_ITEM_DI4_H:
                                    HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI4_L:
                                    HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_0);
                                    break;
                                default:
                                    break;
    					    }
    					}
    					else
    					{
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    			    	break;
    				case MENU2_6_INDEX_UAL:
    					HT_Write4Value(1,DISP_NC,DISP_U,DISP_a,DISP_L);

                        switch(DispCtrlParam.DO3_Item)
    				    {
    						case DO_ITEM_Ua_H:
                            case DO_ITEM_Ua_L:
                            case DO_ITEM_Ub_H:
                            case DO_ITEM_Ub_L:
                            case DO_ITEM_Uc_H:
                            case DO_ITEM_Uc_L:
                            case DO_ITEM_Uavg_H:
                            case DO_ITEM_Uavg_L:
                            /*-------------------------------------------------------*/
    						case DO_ITEM_Uab_H:
                            case DO_ITEM_Uab_L:
                            case DO_ITEM_Ubc_H:
                            case DO_ITEM_Ubc_L:
                            case DO_ITEM_Uca_H:
                            case DO_ITEM_Uca_L:
                            case DO_ITEM_ULavg_H:
                            case DO_ITEM_ULavg_L:
                                Decimal = 1;
                                break;
                            /*-------------------------------------------------------*/
                            case DO_ITEM_Ia_H:
                            case DO_ITEM_Ia_L:
                            case DO_ITEM_Ib_H:
                            case DO_ITEM_Ib_L:
                            case DO_ITEM_Ic_H:
                            case DO_ITEM_Ic_L:
                            case DO_ITEM_Iavg_H:
                            case DO_ITEM_Iavg_L:
                                Decimal = 3;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Pa_H:
                            case DO_ITEM_Pa_L:
                            case DO_ITEM_Pb_H:
                            case DO_ITEM_Pb_L:
                            case DO_ITEM_Pc_H:
                            case DO_ITEM_Pc_L:
                            case DO_ITEM_P_H:
                            case DO_ITEM_P_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_Qa_H:
                            case DO_ITEM_Qa_L:
                            case DO_ITEM_Qb_H:
                            case DO_ITEM_Qb_L:
                            case DO_ITEM_Qc_H:
                            case DO_ITEM_Qc_L:
                            case DO_ITEM_Q_H:
                            case DO_ITEM_Q_L:
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Sa_H:
                            case DO_ITEM_Sa_L:
                            case DO_ITEM_Sb_H:
                            case DO_ITEM_Sb_L:
                            case DO_ITEM_Sc_H:
                            case DO_ITEM_Sc_L:
                            case DO_ITEM_S_H:
                            case DO_ITEM_S_L:
                                Decimal = 0;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_PFa_H:
                            case DO_ITEM_PFa_L:
                            case DO_ITEM_PFb_H:
                            case DO_ITEM_PFb_L:
                            case DO_ITEM_PFc_H:
                            case DO_ITEM_PFc_L:
                            case DO_ITEM_PF_H:
                            case DO_ITEM_PF_L:
                                Decimal = 3;
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                            case DO_ITEM_F_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_UTHa_H:
                            case DO_ITEM_UTHb_H:
                            case DO_ITEM_UTHc_H:
                            case DO_ITEM_UTH_H:
                            case DO_ITEM_ITHa_H:
                            case DO_ITEM_ITHb_H:
                            case DO_ITEM_ITHc_H:
                            case DO_ITEM_ITH_H:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_IMBU_H:
                            case DO_ITEM_IMBI_H:
                                Decimal = 2;
                                break;
                            /*case DO_ITEM_Io_H:
                                case DO_ITEM_Io_L:
                                Decimal = 3;
                                break;*/
                            /*-----------------------------------------------------*/
                            default:
                                break;
    				    }
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				case MENU2_6_INDEX_HYS:
    					HT_Write4Value(1,DISP_NC,DISP_H,DISP_Y,DISP_s);
    					switch(DispCtrlParam.DO3_Item)
    				    {
    						case DO_ITEM_Ua_H:
                            case DO_ITEM_Ua_L:
                            case DO_ITEM_Ub_H:
                            case DO_ITEM_Ub_L:
                            case DO_ITEM_Uc_H:
                            case DO_ITEM_Uc_L:
                            case DO_ITEM_Uavg_H:
                            case DO_ITEM_Uavg_L:
                            /*-------------------------------------------------------*/
    						case DO_ITEM_Uab_H:
                            case DO_ITEM_Uab_L:
                            case DO_ITEM_Ubc_H:
                            case DO_ITEM_Ubc_L:
                            case DO_ITEM_Uca_H:
                            case DO_ITEM_Uca_L:
                            case DO_ITEM_ULavg_H:
                            case DO_ITEM_ULavg_L:
                                Decimal = 1;
                                break;
                            /*-------------------------------------------------------*/
                            case DO_ITEM_Ia_H:
                            case DO_ITEM_Ia_L:
                            case DO_ITEM_Ib_H:
                            case DO_ITEM_Ib_L:
                            case DO_ITEM_Ic_H:
                            case DO_ITEM_Ic_L:
                            case DO_ITEM_Iavg_H:
                            case DO_ITEM_Iavg_L:
                                Decimal = 3;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Pa_H:
                            case DO_ITEM_Pa_L:
                            case DO_ITEM_Pb_H:
                            case DO_ITEM_Pb_L:
                            case DO_ITEM_Pc_H:
                            case DO_ITEM_Pc_L:
                            case DO_ITEM_P_H:
                            case DO_ITEM_P_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_Qa_H:
                            case DO_ITEM_Qa_L:
                            case DO_ITEM_Qb_H:
                            case DO_ITEM_Qb_L:
                            case DO_ITEM_Qc_H:
                            case DO_ITEM_Qc_L:
                            case DO_ITEM_Q_H:
                            case DO_ITEM_Q_L:
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Sa_H:
                            case DO_ITEM_Sa_L:
                            case DO_ITEM_Sb_H:
                            case DO_ITEM_Sb_L:
                            case DO_ITEM_Sc_H:
                            case DO_ITEM_Sc_L:
                            case DO_ITEM_S_H:
                            case DO_ITEM_S_L:
                                Decimal = 0;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_PFa_H:
                            case DO_ITEM_PFa_L:
                            case DO_ITEM_PFb_H:
                            case DO_ITEM_PFb_L:
                            case DO_ITEM_PFc_H:
                            case DO_ITEM_PFc_L:
                            case DO_ITEM_PF_H:
                            case DO_ITEM_PF_L:
                                Decimal = 3;
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                            case DO_ITEM_F_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_UTHa_H:
                            case DO_ITEM_UTHb_H:
                            case DO_ITEM_UTHc_H:
                            case DO_ITEM_UTH_H:
                            case DO_ITEM_ITHa_H:
                            case DO_ITEM_ITHb_H:
                            case DO_ITEM_ITHc_H:
                            case DO_ITEM_ITH_H:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_IMBU_H:
                            case DO_ITEM_IMBI_H:
                                Decimal = 2;
                                break;
                            /*case DO_ITEM_Io_H:
                                case DO_ITEM_Io_L:
                                Decimal = 3;
                                break;*/
                            /*-----------------------------------------------------*/
                            default:
                                break;
    				    }
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				case MENU2_6_INDEX_DELAY:
    					HT_Write4Value(1,DISP_d,DISP_E,DISP_L,DISP_Y);
                        Decimal = 2;
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				default:
    				    break;
    		    }
    			break;
            case MENU1_INDEX_DO4:
                HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_4);
                switch(sg_DispSetMenu2_7_Index)
    			{
                    case MENU2_7_INDEX_MODE:
                        HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
                        if(!sg_bSetFlashEnable)
    					{
                            if(sg_DispSetValue == DO_MODE_OFF)
                                HT_Write4Value(2,DISP_NC,DISP_o,DISP_F,DISP_F);
                            else if(sg_DispSetValue == DO_MODE_ALR)
                                HT_Write4Value(2,DISP_NC,DISP_a,DISP_L,DISP_r);
                            else if(sg_DispSetValue == DO_MODE_REM)
                                HT_Write4Value(2,DISP_NC,DISP_r,DISP_E,DISP_m);
    					}
    					else
    					{
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    				    break;
    				case MENU2_7_INDEX_TIME:
    					HT_Write4Value(1,DISP_t,DISP_I,DISP_m,DISP_E);

                        Decimal =  2;
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    			    	break;
    				case MENU2_7_INDEX_ITEM:
    					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);

    					if(!sg_bSetFlashEnable)
    					{
    					    switch(sg_DispSetValue)
    					    {
    							case DO_ITEM_Ua_H:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ua_L:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ub_H:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ub_L:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Uc_H:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Uc_L:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Uavg_H:
                                    HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_Uavg_L:
                                    HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*-------------------------------------------------------*/
    							case DO_ITEM_Uab_H:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Uab_L:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Ubc_H:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Ubc_L:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Uca_H:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_Uca_L:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ULavg_H:
                                    HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_ULavg_L:
                                    HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                /*-------------------------------------------------------*/
                                case DO_ITEM_Ia_H:
                                    HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ia_L:
                                    HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ib_H:
                                    HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ib_L:
                                    HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ic_H:
                                    HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Ic_L:
                                    HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Iavg_H:
                                    HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_Iavg_L:
                                    HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*------------------------------------------------------*/
                                case DO_ITEM_Pa_H:
                                    HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pa_L:
                                    HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pb_H:
                                    HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pb_L:
                                    HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pc_H:
                                    HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Pc_L:
                                    HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_P_H:
                                    HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_P_L:
                                    HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_Qa_H:
                                    HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qa_L:
                                    HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qb_H:
                                    HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qb_L:
                                    HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qc_H:
                                    HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Qc_L:
                                    HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Q_H:
                                    HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_Q_L:
                                    HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*------------------------------------------------------*/
                                case DO_ITEM_Sa_H:
                                    HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sa_L:
                                    HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sb_H:
                                    HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sb_L:
                                    HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sc_H:
                                    HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_Sc_L:
                                    HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_S_H:
                                    HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_S_L:
                                    HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*------------------------------------------------------*/
                                case DO_ITEM_PFa_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFa_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFb_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFb_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFc_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_H);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PFc_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_L);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_PF_H:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_PF_L:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_F_H:
                                    HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_H);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                case DO_ITEM_F_L:
                                    HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_L);
                                    HT_WriteDot(2,0,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_UTHa_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_a);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_UTHb_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_b);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_UTHc_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_C);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_UTH_H:
                                    HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_NC);
                                    //HT_WriteDot(2,1,TRUE);
                                    break;
                                case DO_ITEM_ITHa_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_a);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ITHb_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_b);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ITHc_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_C);
                                    HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_ITH_H:
                                    HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_NC);
                                    //HT_WriteDot(2,1,TRUE);
                                    break;
                                /*-----------------------------------------------------*/
                                case DO_ITEM_IMBU_H:
                                    HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_U);
                                    //HT_WriteDot(2,2,TRUE);
                                    break;
                                case DO_ITEM_IMBI_H:
                                    HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_I);
                                    //HT_WriteDot(2,2,TRUE);
                                    break;
                                /*case DO_ITEM_Io_H:
                                    HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_H);
                                    HT_WriteDot(2,1,TRUE);
                                    break;
                                    case DO_ITEM_Io_L:
                                    HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_L);
                                    HT_WriteDot(2,1,TRUE);
                                    break;*/
                                /*-----------------------------------------------------*/

                                case DO_ITEM_DI1_H:
                                    HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI1_L:
                                    HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_0);
                                    break;
                                case DO_ITEM_DI2_H:
                                    HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI2_L:
                                    HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_0);
                                    break;
                                case DO_ITEM_DI3_H:
                                    HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI3_L:
                                    HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_0);
                                    break;
                                case DO_ITEM_DI4_H:
                                    HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_1);
                                    break;
                                case DO_ITEM_DI4_L:
                                    HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_0);
                                    break;
                                default:
                                    break;
    					    }
    					}
    					else
    					{
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    			    	break;
    				case MENU2_7_INDEX_UAL:
    					HT_Write4Value(1,DISP_NC,DISP_U,DISP_a,DISP_L);

                        switch(DispCtrlParam.DO4_Item)
    				    {
    						case DO_ITEM_Ua_H:
                            case DO_ITEM_Ua_L:
                            case DO_ITEM_Ub_H:
                            case DO_ITEM_Ub_L:
                            case DO_ITEM_Uc_H:
                            case DO_ITEM_Uc_L:
                            case DO_ITEM_Uavg_H:
                            case DO_ITEM_Uavg_L:
                            /*-------------------------------------------------------*/
    						case DO_ITEM_Uab_H:
                            case DO_ITEM_Uab_L:
                            case DO_ITEM_Ubc_H:
                            case DO_ITEM_Ubc_L:
                            case DO_ITEM_Uca_H:
                            case DO_ITEM_Uca_L:
                            case DO_ITEM_ULavg_H:
                            case DO_ITEM_ULavg_L:
                                Decimal = 1;
                                break;
                            /*-------------------------------------------------------*/
                            case DO_ITEM_Ia_H:
                            case DO_ITEM_Ia_L:
                            case DO_ITEM_Ib_H:
                            case DO_ITEM_Ib_L:
                            case DO_ITEM_Ic_H:
                            case DO_ITEM_Ic_L:
                            case DO_ITEM_Iavg_H:
                            case DO_ITEM_Iavg_L:
                                Decimal = 3;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Pa_H:
                            case DO_ITEM_Pa_L:
                            case DO_ITEM_Pb_H:
                            case DO_ITEM_Pb_L:
                            case DO_ITEM_Pc_H:
                            case DO_ITEM_Pc_L:
                            case DO_ITEM_P_H:
                            case DO_ITEM_P_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_Qa_H:
                            case DO_ITEM_Qa_L:
                            case DO_ITEM_Qb_H:
                            case DO_ITEM_Qb_L:
                            case DO_ITEM_Qc_H:
                            case DO_ITEM_Qc_L:
                            case DO_ITEM_Q_H:
                            case DO_ITEM_Q_L:
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Sa_H:
                            case DO_ITEM_Sa_L:
                            case DO_ITEM_Sb_H:
                            case DO_ITEM_Sb_L:
                            case DO_ITEM_Sc_H:
                            case DO_ITEM_Sc_L:
                            case DO_ITEM_S_H:
                            case DO_ITEM_S_L:
                                Decimal = 0;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_PFa_H:
                            case DO_ITEM_PFa_L:
                            case DO_ITEM_PFb_H:
                            case DO_ITEM_PFb_L:
                            case DO_ITEM_PFc_H:
                            case DO_ITEM_PFc_L:
                            case DO_ITEM_PF_H:
                            case DO_ITEM_PF_L:
                                Decimal = 3;
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                            case DO_ITEM_F_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_UTHa_H:
                            case DO_ITEM_UTHb_H:
                            case DO_ITEM_UTHc_H:
                            case DO_ITEM_UTH_H:
                            case DO_ITEM_ITHa_H:
                            case DO_ITEM_ITHb_H:
                            case DO_ITEM_ITHc_H:
                            case DO_ITEM_ITH_H:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_IMBU_H:
                            case DO_ITEM_IMBI_H:
                                Decimal = 2;
                                break;
                            /*case DO_ITEM_Io_H:
                                case DO_ITEM_Io_L:
                                Decimal = 3;
                                break;*/
                            /*-----------------------------------------------------*/
                            default:
                                break;
    				    }
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				case MENU2_7_INDEX_HYS:
    					HT_Write4Value(1,DISP_NC,DISP_H,DISP_Y,DISP_s);
    					switch(DispCtrlParam.DO4_Item)
    				    {
    						case DO_ITEM_Ua_H:
                            case DO_ITEM_Ua_L:
                            case DO_ITEM_Ub_H:
                            case DO_ITEM_Ub_L:
                            case DO_ITEM_Uc_H:
                            case DO_ITEM_Uc_L:
                            case DO_ITEM_Uavg_H:
                            case DO_ITEM_Uavg_L:
                            /*-------------------------------------------------------*/
    						case DO_ITEM_Uab_H:
                            case DO_ITEM_Uab_L:
                            case DO_ITEM_Ubc_H:
                            case DO_ITEM_Ubc_L:
                            case DO_ITEM_Uca_H:
                            case DO_ITEM_Uca_L:
                            case DO_ITEM_ULavg_H:
                            case DO_ITEM_ULavg_L:
                                Decimal = 1;
                                break;
                            /*-------------------------------------------------------*/
                            case DO_ITEM_Ia_H:
                            case DO_ITEM_Ia_L:
                            case DO_ITEM_Ib_H:
                            case DO_ITEM_Ib_L:
                            case DO_ITEM_Ic_H:
                            case DO_ITEM_Ic_L:
                            case DO_ITEM_Iavg_H:
                            case DO_ITEM_Iavg_L:
                                Decimal = 3;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Pa_H:
                            case DO_ITEM_Pa_L:
                            case DO_ITEM_Pb_H:
                            case DO_ITEM_Pb_L:
                            case DO_ITEM_Pc_H:
                            case DO_ITEM_Pc_L:
                            case DO_ITEM_P_H:
                            case DO_ITEM_P_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_Qa_H:
                            case DO_ITEM_Qa_L:
                            case DO_ITEM_Qb_H:
                            case DO_ITEM_Qb_L:
                            case DO_ITEM_Qc_H:
                            case DO_ITEM_Qc_L:
                            case DO_ITEM_Q_H:
                            case DO_ITEM_Q_L:
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Sa_H:
                            case DO_ITEM_Sa_L:
                            case DO_ITEM_Sb_H:
                            case DO_ITEM_Sb_L:
                            case DO_ITEM_Sc_H:
                            case DO_ITEM_Sc_L:
                            case DO_ITEM_S_H:
                            case DO_ITEM_S_L:
                                Decimal = 0;
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_PFa_H:
                            case DO_ITEM_PFa_L:
                            case DO_ITEM_PFb_H:
                            case DO_ITEM_PFb_L:
                            case DO_ITEM_PFc_H:
                            case DO_ITEM_PFc_L:
                            case DO_ITEM_PF_H:
                            case DO_ITEM_PF_L:
                                Decimal = 3;
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                            case DO_ITEM_F_L:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_UTHa_H:
                            case DO_ITEM_UTHb_H:
                            case DO_ITEM_UTHc_H:
                            case DO_ITEM_UTH_H:
                            case DO_ITEM_ITHa_H:
                            case DO_ITEM_ITHb_H:
                            case DO_ITEM_ITHc_H:
                            case DO_ITEM_ITH_H:
                            /*-----------------------------------------------------*/
                            case DO_ITEM_IMBU_H:
                            case DO_ITEM_IMBI_H:
                                Decimal = 2;
                                break;
                            /*case DO_ITEM_Io_H:
                                case DO_ITEM_Io_L:
                                Decimal = 3;
                                break;*/
                            /*-----------------------------------------------------*/
                            default:
                                break;
    				    }
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				case MENU2_7_INDEX_DELAY:
    					HT_Write4Value(1,DISP_d,DISP_E,DISP_L,DISP_Y);
                        Decimal = 2;
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				default:
    				    break;
    		    }
    			break;
            case MENU1_INDEX_AO1:
                HT_Write4Value(0,DISP1_A,DISP1_o,DISP1_FU,DISP1_1);
                switch(sg_DispSetMenu2_8_Index)
    			{
                    case MENU2_8_INDEX_MODE:
                        HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
                        if(!sg_bSetFlashEnable)
    					{
    					    if(sg_DispSetValue == AO_MODE_OFF)
    					        HT_Write4Value(2,DISP_NC,DISP_o,DISP_F,DISP_F);
                            else if(sg_DispSetValue == AO_MODE_420)
                                HT_Write4Value(2,DISP_4,DISP_FU,DISP_2,DISP_0);
                            else if(sg_DispSetValue == AO_MODE_020)
                                HT_Write4Value(2,DISP_0,DISP_FU,DISP_2,DISP_0);
                            else if(sg_DispSetValue == AO_MODE_1220)
                            {
                                HT_Write4Value(2,DISP_1,DISP_2,DISP_2,DISP_0);
                                HT_WriteDot(2,1,TRUE);
                            }
    					}
    					else
    					{
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    				    break;
    				case MENU2_8_INDEX_ITEM:
    					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);
    					if(!sg_bSetFlashEnable)
    					{
    					    switch(sg_DispSetValue)
    					    {
                                case AO_ITEM_UA:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_UB:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_UC:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_UAB:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_NC);
                                    break;
                                case AO_ITEM_UBC:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_NC);
                                    break;
                                case AO_ITEM_UCA:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_NC);
                                    break;
                                case AO_ITEM_IA:
                                    HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_IB:
                                    HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_IC:
                                    HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_PA:
                                    HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_PB:
                                    HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_PC:
                                    HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_P:
                                    HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_QA:
                                    HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_QB:
                                    HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_QC:
                                    HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_Q:
                                    HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_SA:
                                    HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_SB:
                                    HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_SC:
                                    HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_S:
                                    HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_PFA:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_NC);
                                    break;
                                case AO_ITEM_PFB:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_NC);
                                    break;
                                case AO_ITEM_PFC:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_NC);
                                    break;
                                case AO_ITEM_PF:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_F:
                                    HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_1);
                                    break;
                                default:
                                    break;
    					    }
    					}
    					else
    					{
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    			    	break;
    				case MENU2_8_INDEX_DS:
    					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_d,DISP_s);
    					switch(DispCtrlParam.AO1_Item)
    				    {
                            case AO_ITEM_UA:
                            case AO_ITEM_UB:
                            case AO_ITEM_UC:
                            case AO_ITEM_UAB:
                            case AO_ITEM_UBC:
                            case AO_ITEM_UCA:
                                Decimal = 1;
                                break;
                            case AO_ITEM_IA:
                            case AO_ITEM_IB:
                            case AO_ITEM_IC:
                                Decimal = 3;
                                break;
                            case AO_ITEM_PA:
                            case AO_ITEM_PB:
                            case AO_ITEM_PC:
                            case AO_ITEM_P:
                            case AO_ITEM_QA:
                            case AO_ITEM_QB:
                            case AO_ITEM_QC:
                            case AO_ITEM_Q:
                            case AO_ITEM_SA:
                            case AO_ITEM_SB:
                            case AO_ITEM_SC:
                            case AO_ITEM_S:
                                Decimal = 0;
                                break;
                            case AO_ITEM_PFA:
                            case AO_ITEM_PFB:
                            case AO_ITEM_PFC:
                            case AO_ITEM_PF:
                                Decimal = 3;
                                break;
                            case AO_ITEM_F:
                                Decimal = 2;
                                break;
                            default:
                                break;
    				    }
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				case MENU2_8_INDEX_FS:
    					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_F,DISP_s);
    					switch(DispCtrlParam.AO1_Item)
    				    {
                            case AO_ITEM_UA:
                            case AO_ITEM_UB:
                            case AO_ITEM_UC:
                            case AO_ITEM_UAB:
                            case AO_ITEM_UBC:
                            case AO_ITEM_UCA:
                                Decimal = 1;
                                break;
                            case AO_ITEM_IA:
                            case AO_ITEM_IB:
                            case AO_ITEM_IC:
                                Decimal = 3;
                                break;
                            case AO_ITEM_PA:
                            case AO_ITEM_PB:
                            case AO_ITEM_PC:
                            case AO_ITEM_P:
                            case AO_ITEM_QA:
                            case AO_ITEM_QB:
                            case AO_ITEM_QC:
                            case AO_ITEM_Q:
                            case AO_ITEM_SA:
                            case AO_ITEM_SB:
                            case AO_ITEM_SC:
                            case AO_ITEM_S:
                                Decimal = 0;
                                break;
                            case AO_ITEM_PFA:
                            case AO_ITEM_PFB:
                            case AO_ITEM_PFC:
                            case AO_ITEM_PF:
                                Decimal = 3;
                                break;
                            case AO_ITEM_F:
                                Decimal = 2;
                                break;
                            default:
                                break;
    				    }
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				default:
    				    break;
                }
                break;
            case MENU1_INDEX_AO2:
                HT_Write4Value(0,DISP1_A,DISP1_o,DISP1_FU,DISP1_2);
                switch(sg_DispSetMenu2_9_Index)
    			{
                    case MENU2_9_INDEX_MODE:
                        HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
                        if(!sg_bSetFlashEnable)
    					{
    					    if(sg_DispSetValue == AO_MODE_OFF)
    					        HT_Write4Value(2,DISP_NC,DISP_o,DISP_F,DISP_F);
                            else if(sg_DispSetValue == AO_MODE_420)
                                HT_Write4Value(2,DISP_4,DISP_FU,DISP_2,DISP_0);
                            else if(sg_DispSetValue == AO_MODE_020)
                                HT_Write4Value(2,DISP_0,DISP_FU,DISP_2,DISP_0);
                            else if(sg_DispSetValue == AO_MODE_1220)
                            {
                                HT_Write4Value(2,DISP_1,DISP_2,DISP_2,DISP_0);
                                HT_WriteDot(2,1,TRUE);
                            }
    					}
    					else
    					{
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    				    break;
    				case MENU2_9_INDEX_ITEM:
    					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);

    					if(!sg_bSetFlashEnable)
    					{
    					    switch(sg_DispSetValue)
    					    {
                                case AO_ITEM_UA:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_UB:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_UC:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_UAB:
                                    HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_NC);
                                    break;
                                case AO_ITEM_UBC:
                                    HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_NC);
                                    break;
                                case AO_ITEM_UCA:
                                    HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_NC);
                                    break;
                                case AO_ITEM_IA:
                                    HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_IB:
                                    HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_IC:
                                    HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_PA:
                                    HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_PB:
                                    HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_PC:
                                    HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_P:
                                    HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_QA:
                                    HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_QB:
                                    HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_QC:
                                    HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_Q:
                                    HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_SA:
                                    HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_SB:
                                    HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_SC:
                                    HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_S:
                                    HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_PFA:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_NC);
                                    break;
                                case AO_ITEM_PFB:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_NC);
                                    break;
                                case AO_ITEM_PFC:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_NC);
                                    break;
                                case AO_ITEM_PF:
                                    HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_NC);
                                    break;
                                case AO_ITEM_F:
                                    HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_1);
                                    break;
                                default:
                                    break;

    					    }
    					}
    					else
    					{
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
    					}
    			    	break;
    				case MENU2_9_INDEX_DS:
    					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_d,DISP_s);
    					switch(DispCtrlParam.AO2_Item)
    				    {
                            case AO_ITEM_UA:
                            case AO_ITEM_UB:
                            case AO_ITEM_UC:
                            case AO_ITEM_UAB:
                            case AO_ITEM_UBC:
                            case AO_ITEM_UCA:
                                Decimal = 1;
                                break;
                            case AO_ITEM_IA:
                            case AO_ITEM_IB:
                            case AO_ITEM_IC:
                                Decimal = 3;
                                break;
                            case AO_ITEM_PA:
                            case AO_ITEM_PB:
                            case AO_ITEM_PC:
                            case AO_ITEM_P:
                            case AO_ITEM_QA:
                            case AO_ITEM_QB:
                            case AO_ITEM_QC:
                            case AO_ITEM_Q:
                            case AO_ITEM_SA:
                            case AO_ITEM_SB:
                            case AO_ITEM_SC:
                            case AO_ITEM_S:
                                Decimal = 0;
                                break;
                            case AO_ITEM_PFA:
                            case AO_ITEM_PFB:
                            case AO_ITEM_PFC:
                            case AO_ITEM_PF:
                                Decimal = 3;
                                break;
                            case AO_ITEM_F:
                                Decimal = 2;
                                break;
                            default:
                                break;
    				    }
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				case MENU2_9_INDEX_FS:
    					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_F,DISP_s);
    					switch(DispCtrlParam.AO2_Item)
    				    {
                            case AO_ITEM_UA:
                            case AO_ITEM_UB:
                            case AO_ITEM_UC:
                            case AO_ITEM_UAB:
                            case AO_ITEM_UBC:
                            case AO_ITEM_UCA:
                                Decimal = 1;
                                break;
                            case AO_ITEM_IA:
                            case AO_ITEM_IB:
                            case AO_ITEM_IC:
                                Decimal = 3;
                                break;
                            case AO_ITEM_PA:
                            case AO_ITEM_PB:
                            case AO_ITEM_PC:
                            case AO_ITEM_P:
                            case AO_ITEM_QA:
                            case AO_ITEM_QB:
                            case AO_ITEM_QC:
                            case AO_ITEM_Q:
                            case AO_ITEM_SA:
                            case AO_ITEM_SB:
                            case AO_ITEM_SC:
                            case AO_ITEM_S:
                                Decimal = 0;
                                break;
                            case AO_ITEM_PFA:
                            case AO_ITEM_PFB:
                            case AO_ITEM_PFC:
                            case AO_ITEM_PF:
                                Decimal = 3;
                                break;
                            case AO_ITEM_F:
                                Decimal = 2;
                                break;
                            default:
                                break;
    				    }
    					Disp_Focus.DispWidth = 4;
    					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
    					CharToNumber();
    				    break;
    				default:
    				    break;
                }
                break;
    		default:
    		    break;
    	}
    }
}


void DispMenu_MenuValue(void) //4级菜单
{
	u8 Decimal = 0;
	Reset_Screen();
    if(DispMode == ReadMode)
    {
        /**************标签页********************************************************/
        HT_WriteValue(Line_R5,0,DISP1_r);
        HT_WriteValue(Line_R5,1,DISP1_d);
        HT_WriteValue(Line_R5,2,CHAR_NONE);
        /*****************************************************************************/
    }
    else if(DispMode == ProgMode)
    {
        /**************标签页********************************************************/
        HT_WriteValue(Line_R5,0,DISP1_p);
        HT_WriteValue(Line_R5,1,DISP1_r);
        HT_WriteValue(Line_R5,2,CHAR_NONE);
        /*****************************************************************************/
    }
	if(DispMode == ReadMode)
	{
        sg_bSetFlashEnable = FALSE; //查看选项，不闪烁
	}
	switch(sg_DispSetMenu1Index)
	{
		case MENU1_INDEX_INPT:
			HT_Write4Value(0,DISP1_I,DISP1_n,DISP1_p,DISP1_T);
			switch(sg_DispSetMenu2_1_Index)
			{
                case MENU2_1_INDEX_NET:
                    HT_Write4Value(1,DISP_NC,DISP_n,DISP_E,DISP_t);
                    if( sg_DispSetValue == N34)
                    {
                        if(!sg_bSetFlashEnable)
                			HT_Write4Value(2,DISP_NC,DISP_n,DISP_3,DISP_4);
                		else
                			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
                    }
                    else if( sg_DispSetValue == N33)
                    {
                        if(!sg_bSetFlashEnable)
                			HT_Write4Value(2,DISP_NC,DISP_n,DISP_3,DISP_3);
                		else
                			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
                    }
                    break;
				case MENU2_1_INDEX_PT:
					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_p,DISP_t);

					Decimal =  0;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal, Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_1_INDEX_CT:
					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_C,DISP_t);

					Decimal = 0;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
			}
		    break;
	    case MENU1_INDEX_COM1:
	        HT_Write4Value(0,DISP1_C,DISP1_o,DISP1_M,DISP1_1);
			switch(sg_DispSetMenu2_2_Index)
			{
				case MENU2_2_INDEX_ADD:
					HT_Write4Value(1,DISP_a,DISP_d,DISP_d,DISP_r);
					Decimal =  0;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				case MENU2_2_INDEX_BAUD:
					HT_Write4Value(1,DISP_b,DISP_a,DISP_U,DISP_d);
					if(sg_bSetFlashEnable)
					{
                        HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
					else
					{
                        switch(sg_DispSetValue)
    					{
                            case Baud12:
                                HT_Write4Value(2,DISP_1,DISP_2,DISP_0,DISP_0);
                                //HT_WriteDot(2,2,FALSE);
                                break;
                            case Baud24:
                                HT_Write4Value(2,DISP_2,DISP_4,DISP_0,DISP_0);
                                break;
                            case Baud48:
                                HT_Write4Value(2,DISP_4,DISP_8,DISP_0,DISP_0);
                                break;
                            case Baud96:
                                HT_Write4Value(2,DISP_9,DISP_6,DISP_0,DISP_0);
                                //HT_WriteDot(2,2,FALSE);
                                break;
                            case Baud192:
                                HT_Write4Value(2,DISP_1,DISP_9,DISP_2,DISP_0);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            default:
                                break;
    					}
					}
			    	break;
				case MENU2_2_INDEX_DATA:
					HT_Write4Value(1,DISP_d,DISP_a,DISP_t,DISP_a);
					if(sg_DispSetValue == n81)
					{
                        if(!sg_bSetFlashEnable)
                			HT_Write4Value(2,DISP_NC,DISP_n,DISP_8,DISP_1);
                		else
                			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
					else if( sg_DispSetValue == o81)
					{
                        if(!sg_bSetFlashEnable)
                			HT_Write4Value(2,DISP_NC,DISP_o,DISP_8,DISP_1);
                		else
                			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
					else if( sg_DispSetValue == e81)
					{
                        if(!sg_bSetFlashEnable)
                			HT_Write4Value(2,DISP_NC,DISP_E,DISP_8,DISP_1);
                		else
                			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
					else if( sg_DispSetValue == n82)
					{
                        if(!sg_bSetFlashEnable)
                			HT_Write4Value(2,DISP_NC,DISP_n,DISP_8,DISP_2);
                		else
                			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
				    break;
				case MENU2_2_INDEX_COMS:
					HT_Write4Value(1,DISP_C,DISP_o,DISP_m,DISP_s);
					if(sg_DispSetValue == rtu485)
					{
                        if(!sg_bSetFlashEnable)
                			HT_Write4Value(2,DISP_NC,DISP_r,DISP_t,DISP_U);
                		else
                			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
					else if( sg_DispSetValue == calib)
					{
                        if(!sg_bSetFlashEnable)
                			HT_Write4Value(2,DISP_NC,DISP_C,DISP_a,DISP_L);
                		else
                			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
				    break;
				default:
				    break;
			}
		    break;
        case MENU1_INDEX_SYS:
            HT_Write4Value(0,DISP1_NC,DISP1_s,DISP1_Y,DISP1_s);
            switch(sg_DispSetMenu2_3_Index)
			{
                case MENU2_3_INDEX_CYC:
					HT_Write4Value(1,DISP_NC,DISP_C,DISP_Y,DISP_C);

					if(sg_DispSetValue == SaveYes)
					{
                        if(!sg_bSetFlashEnable)
                			HT_Write4Value(2,DISP_NC,DISP_Y,DISP_E,DISP_s);
                		else
                			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
					else if( sg_DispSetValue == SaveNO)
					{
                        if(!sg_bSetFlashEnable)
                			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_n,DISP_o);
                		else
                			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
				    break;
				case MENU2_3_INDEX_LIGH:        // 背光时间
					HT_Write4Value(1,DISP_L,DISP_I,DISP_G,DISP_H);

					Decimal =  0;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_3_INDEX_DISP:
					HT_Write4Value(1,DISP_d,DISP_I,DISP_s,DISP_p);

					if(!sg_bSetFlashEnable)
					{
                        if(sg_DispSetValue == DISP_UINT_U)
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_U);
                        else if(sg_DispSetValue == DISP_UINT_I)
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_I);
                        else if(sg_DispSetValue == DISP_UINT_P)
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_p);
                        else if(sg_DispSetValue == DISP_UINT_PF)
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_p,DISP_F);
                        else if(sg_DispSetValue == DISP_UINT_EP)
                            HT_Write4Value(2,DISP_NC,DISP_NC,DISP_E,DISP_p);
                        else if(sg_DispSetValue == DISP_UINT_THD)
                            HT_Write4Value(2,DISP_NC,DISP_t,DISP_H,DISP_d);
                        else if(sg_DispSetValue == DISP_UINT_DEM)
                            HT_Write4Value(2,DISP_NC,DISP_d,DISP_E,DISP_m);
					}
					else
					{
                        HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
					break;
			    case MENU2_3_INDEX_ALR:
					HT_Write4Value(1,DISP_NC,DISP_a,DISP_L,DISP_r);
					Decimal =  0;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_3_INDEX_CLRE:
					HT_Write4Value(1,DISP_C,DISP_L,DISP_r,DISP_E);
					HT_WriteDot(1,2,TRUE);

                    if(sg_DispSetValue == SaveYes)
					{
                        if(!sg_bSetFlashEnable)
                			HT_Write4Value(2,DISP_NC,DISP_Y,DISP_E,DISP_s);
                		else
                			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
					else if( sg_DispSetValue == SaveNO)
					{
                        if(!sg_bSetFlashEnable)
                			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_n,DISP_o);
                		else
                			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
				    break;
				case MENU2_3_INDEX_CODE:
					HT_Write4Value(1,DISP_C,DISP_o,DISP_d,DISP_E);

					Decimal =  0;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
            }
		    break;
		case MENU1_INDEX_DO1:
            HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_1);
            switch(sg_DispSetMenu2_4_Index)
			{
                case MENU2_4_INDEX_MODE:
                    HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
                    if(!sg_bSetFlashEnable)
					{
                        if(sg_DispSetValue == DO_MODE_OFF)
                            HT_Write4Value(2,DISP_NC,DISP_o,DISP_F,DISP_F);
                        else if(sg_DispSetValue == DO_MODE_ALR)
                            HT_Write4Value(2,DISP_NC,DISP_a,DISP_L,DISP_r);
                        else if(sg_DispSetValue == DO_MODE_REM)
                            HT_Write4Value(2,DISP_NC,DISP_r,DISP_E,DISP_m);
					}
					else
					{
                        HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
				    break;
				case MENU2_4_INDEX_TIME:
					HT_Write4Value(1,DISP_t,DISP_I,DISP_m,DISP_E);

                    Decimal =  2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
			    	break;
				case MENU2_4_INDEX_ITEM:
					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);

					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
							case DO_ITEM_Ua_H:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ua_L:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ub_H:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ub_L:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Uc_H:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Uc_L:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Uavg_H:
                                HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_Uavg_L:
                                HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*-------------------------------------------------------*/
							case DO_ITEM_Uab_H:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Uab_L:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Ubc_H:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Ubc_L:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Uca_H:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Uca_L:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ULavg_H:
                                HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_ULavg_L:
                                HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            /*-------------------------------------------------------*/
                            case DO_ITEM_Ia_H:
                                HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ia_L:
                                HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ib_H:
                                HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ib_L:
                                HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ic_H:
                                HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ic_L:
                                HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Iavg_H:
                                HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_Iavg_L:
                                HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Pa_H:
                                HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pa_L:
                                HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pb_H:
                                HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pb_L:
                                HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pc_H:
                                HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pc_L:
                                HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_P_H:
                                HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_P_L:
                                HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_Qa_H:
                                HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qa_L:
                                HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qb_H:
                                HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qb_L:
                                HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qc_H:
                                HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qc_L:
                                HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Q_H:
                                HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_Q_L:
                                HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Sa_H:
                                HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sa_L:
                                HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sb_H:
                                HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sb_L:
                                HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sc_H:
                                HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sc_L:
                                HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_S_H:
                                HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_S_L:
                                HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_PFa_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFa_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFb_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFb_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFc_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFc_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PF_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_PF_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                                HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_F_L:
                                HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_UTHa_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_a);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_UTHb_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_b);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_UTHc_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_C);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_UTH_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_NC);
                                //HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_ITHa_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_a);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ITHb_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_b);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ITHc_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_C);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ITH_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_NC);
                                //HT_WriteDot(2,1,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_IMBU_H:
                                HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_U);
                                //HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_IMBI_H:
                                HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_I);
                                //HT_WriteDot(2,2,TRUE);
                                break;
                            /*case DO_ITEM_Io_H:
                                HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                                case DO_ITEM_Io_L:
                                HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;*/
                            /*-----------------------------------------------------*/

                            case DO_ITEM_DI1_H:
                                HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI1_L:
                                HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_0);
                                break;
                            case DO_ITEM_DI2_H:
                                HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI2_L:
                                HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_0);
                                break;
                            case DO_ITEM_DI3_H:
                                HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI3_L:
                                HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_0);
                                break;
                            case DO_ITEM_DI4_H:
                                HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI4_L:
                                HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_0);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
			    	break;
				case MENU2_4_INDEX_UAL:
					HT_Write4Value(1,DISP_NC,DISP_U,DISP_a,DISP_L);

                    switch(DispCtrlParam.DO1_Item)
				    {
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                        case DO_ITEM_Uavg_H:
                        case DO_ITEM_Uavg_L:
                        /*-------------------------------------------------------*/
						case DO_ITEM_Uab_H:
                        case DO_ITEM_Uab_L:
                        case DO_ITEM_Ubc_H:
                        case DO_ITEM_Ubc_L:
                        case DO_ITEM_Uca_H:
                        case DO_ITEM_Uca_L:
                        case DO_ITEM_ULavg_H:
                        case DO_ITEM_ULavg_L:
                            Decimal = 1;
                            break;
                        /*-------------------------------------------------------*/
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                        case DO_ITEM_Iavg_H:
                        case DO_ITEM_Iavg_L:
                            Decimal = 3;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Pa_H:
                        case DO_ITEM_Pa_L:
                        case DO_ITEM_Pb_H:
                        case DO_ITEM_Pb_L:
                        case DO_ITEM_Pc_H:
                        case DO_ITEM_Pc_L:
                        case DO_ITEM_P_H:
                        case DO_ITEM_P_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_Qa_H:
                        case DO_ITEM_Qa_L:
                        case DO_ITEM_Qb_H:
                        case DO_ITEM_Qb_L:
                        case DO_ITEM_Qc_H:
                        case DO_ITEM_Qc_L:
                        case DO_ITEM_Q_H:
                        case DO_ITEM_Q_L:
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Sa_H:
                        case DO_ITEM_Sa_L:
                        case DO_ITEM_Sb_H:
                        case DO_ITEM_Sb_L:
                        case DO_ITEM_Sc_H:
                        case DO_ITEM_Sc_L:
                        case DO_ITEM_S_H:
                        case DO_ITEM_S_L:
                            Decimal = 0;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_PFa_H:
                        case DO_ITEM_PFa_L:
                        case DO_ITEM_PFb_H:
                        case DO_ITEM_PFb_L:
                        case DO_ITEM_PFc_H:
                        case DO_ITEM_PFc_L:
                        case DO_ITEM_PF_H:
                        case DO_ITEM_PF_L:
                            Decimal = 3;
                            break;
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_UTHa_H:
                        case DO_ITEM_UTHb_H:
                        case DO_ITEM_UTHc_H:
                        case DO_ITEM_UTH_H:
                        case DO_ITEM_ITHa_H:
                        case DO_ITEM_ITHb_H:
                        case DO_ITEM_ITHc_H:
                        case DO_ITEM_ITH_H:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_IMBU_H:
                        case DO_ITEM_IMBI_H:
                            Decimal = 2;
                            break;
                        /*case DO_ITEM_Io_H:
                            case DO_ITEM_Io_L:
                            Decimal = 3;
                            break;*/
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				case MENU2_4_INDEX_HYS:
					HT_Write4Value(1,DISP_NC,DISP_H,DISP_Y,DISP_s);
					switch(DispCtrlParam.DO1_Item)
				    {
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                        case DO_ITEM_Uavg_H:
                        case DO_ITEM_Uavg_L:
                        /*-------------------------------------------------------*/
						case DO_ITEM_Uab_H:
                        case DO_ITEM_Uab_L:
                        case DO_ITEM_Ubc_H:
                        case DO_ITEM_Ubc_L:
                        case DO_ITEM_Uca_H:
                        case DO_ITEM_Uca_L:
                        case DO_ITEM_ULavg_H:
                        case DO_ITEM_ULavg_L:
                            Decimal = 1;
                            break;
                        /*-------------------------------------------------------*/
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                        case DO_ITEM_Iavg_H:
                        case DO_ITEM_Iavg_L:
                            Decimal = 3;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Pa_H:
                        case DO_ITEM_Pa_L:
                        case DO_ITEM_Pb_H:
                        case DO_ITEM_Pb_L:
                        case DO_ITEM_Pc_H:
                        case DO_ITEM_Pc_L:
                        case DO_ITEM_P_H:
                        case DO_ITEM_P_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_Qa_H:
                        case DO_ITEM_Qa_L:
                        case DO_ITEM_Qb_H:
                        case DO_ITEM_Qb_L:
                        case DO_ITEM_Qc_H:
                        case DO_ITEM_Qc_L:
                        case DO_ITEM_Q_H:
                        case DO_ITEM_Q_L:
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Sa_H:
                        case DO_ITEM_Sa_L:
                        case DO_ITEM_Sb_H:
                        case DO_ITEM_Sb_L:
                        case DO_ITEM_Sc_H:
                        case DO_ITEM_Sc_L:
                        case DO_ITEM_S_H:
                        case DO_ITEM_S_L:
                            Decimal = 0;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_PFa_H:
                        case DO_ITEM_PFa_L:
                        case DO_ITEM_PFb_H:
                        case DO_ITEM_PFb_L:
                        case DO_ITEM_PFc_H:
                        case DO_ITEM_PFc_L:
                        case DO_ITEM_PF_H:
                        case DO_ITEM_PF_L:
                            Decimal = 3;
                            break;
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_UTHa_H:
                        case DO_ITEM_UTHb_H:
                        case DO_ITEM_UTHc_H:
                        case DO_ITEM_UTH_H:
                        case DO_ITEM_ITHa_H:
                        case DO_ITEM_ITHb_H:
                        case DO_ITEM_ITHc_H:
                        case DO_ITEM_ITH_H:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_IMBU_H:
                        case DO_ITEM_IMBI_H:
                            Decimal = 2;
                            break;
                        /*case DO_ITEM_Io_H:
                            case DO_ITEM_Io_L:
                            Decimal = 3;
                            break;*/
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				case MENU2_4_INDEX_DELAY:
					HT_Write4Value(1,DISP_d,DISP_E,DISP_L,DISP_Y);
                    Decimal = 2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
            }
            break;
        case MENU1_INDEX_DO2:
            HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_2);
            switch(sg_DispSetMenu2_5_Index)
			{
                case MENU2_5_INDEX_MODE:
                    HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
                    if(!sg_bSetFlashEnable)
					{
                        if(sg_DispSetValue == DO_MODE_OFF)
                            HT_Write4Value(2,DISP_NC,DISP_o,DISP_F,DISP_F);
                        else if(sg_DispSetValue == DO_MODE_ALR)
                            HT_Write4Value(2,DISP_NC,DISP_a,DISP_L,DISP_r);
                        else if(sg_DispSetValue == DO_MODE_REM)
                            HT_Write4Value(2,DISP_NC,DISP_r,DISP_E,DISP_m);
					}
					else
					{
                        HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
				    break;
				case MENU2_5_INDEX_TIME:
					HT_Write4Value(1,DISP_t,DISP_I,DISP_m,DISP_E);

                    Decimal = 2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
			    	break;
				case MENU2_5_INDEX_ITEM:
					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);

					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
							case DO_ITEM_Ua_H:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ua_L:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ub_H:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ub_L:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Uc_H:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Uc_L:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Uavg_H:
                                HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_Uavg_L:
                                HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*-------------------------------------------------------*/
							case DO_ITEM_Uab_H:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Uab_L:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Ubc_H:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Ubc_L:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Uca_H:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Uca_L:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ULavg_H:
                                HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_ULavg_L:
                                HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            /*-------------------------------------------------------*/
                            case DO_ITEM_Ia_H:
                                HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ia_L:
                                HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ib_H:
                                HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ib_L:
                                HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ic_H:
                                HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ic_L:
                                HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Iavg_H:
                                HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_Iavg_L:
                                HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Pa_H:
                                HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pa_L:
                                HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pb_H:
                                HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pb_L:
                                HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pc_H:
                                HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pc_L:
                                HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_P_H:
                                HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_P_L:
                                HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_Qa_H:
                                HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qa_L:
                                HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qb_H:
                                HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qb_L:
                                HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qc_H:
                                HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qc_L:
                                HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Q_H:
                                HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_Q_L:
                                HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Sa_H:
                                HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sa_L:
                                HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sb_H:
                                HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sb_L:
                                HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sc_H:
                                HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sc_L:
                                HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_S_H:
                                HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_S_L:
                                HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_PFa_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFa_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFb_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFb_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFc_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFc_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PF_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_PF_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                                HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_F_L:
                                HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_UTHa_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_a);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_UTHb_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_b);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_UTHc_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_C);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_UTH_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_NC);
                                //HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_ITHa_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_a);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ITHb_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_b);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ITHc_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_C);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ITH_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_NC);
                                //HT_WriteDot(2,1,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_IMBU_H:
                                HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_U);
                                //HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_IMBI_H:
                                HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_I);
                                //HT_WriteDot(2,2,TRUE);
                                break;
                            /*case DO_ITEM_Io_H:
                                HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                                case DO_ITEM_Io_L:
                                HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;*/
                            /*-----------------------------------------------------*/

                            case DO_ITEM_DI1_H:
                                HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI1_L:
                                HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_0);
                                break;
                            case DO_ITEM_DI2_H:
                                HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI2_L:
                                HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_0);
                                break;
                            case DO_ITEM_DI3_H:
                                HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI3_L:
                                HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_0);
                                break;
                            case DO_ITEM_DI4_H:
                                HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI4_L:
                                HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_0);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
					break;
			    case MENU2_5_INDEX_UAL:
					HT_Write4Value(1,DISP_NC,DISP_U,DISP_a,DISP_L);

                    switch(DispCtrlParam.DO2_Item)
				    {
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                        case DO_ITEM_Uavg_H:
                        case DO_ITEM_Uavg_L:
                        /*-------------------------------------------------------*/
						case DO_ITEM_Uab_H:
                        case DO_ITEM_Uab_L:
                        case DO_ITEM_Ubc_H:
                        case DO_ITEM_Ubc_L:
                        case DO_ITEM_Uca_H:
                        case DO_ITEM_Uca_L:
                        case DO_ITEM_ULavg_H:
                        case DO_ITEM_ULavg_L:
                            Decimal = 1;
                            break;
                        /*-------------------------------------------------------*/
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                        case DO_ITEM_Iavg_H:
                        case DO_ITEM_Iavg_L:
                            Decimal = 3;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Pa_H:
                        case DO_ITEM_Pa_L:
                        case DO_ITEM_Pb_H:
                        case DO_ITEM_Pb_L:
                        case DO_ITEM_Pc_H:
                        case DO_ITEM_Pc_L:
                        case DO_ITEM_P_H:
                        case DO_ITEM_P_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_Qa_H:
                        case DO_ITEM_Qa_L:
                        case DO_ITEM_Qb_H:
                        case DO_ITEM_Qb_L:
                        case DO_ITEM_Qc_H:
                        case DO_ITEM_Qc_L:
                        case DO_ITEM_Q_H:
                        case DO_ITEM_Q_L:
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Sa_H:
                        case DO_ITEM_Sa_L:
                        case DO_ITEM_Sb_H:
                        case DO_ITEM_Sb_L:
                        case DO_ITEM_Sc_H:
                        case DO_ITEM_Sc_L:
                        case DO_ITEM_S_H:
                        case DO_ITEM_S_L:
                            Decimal = 0;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_PFa_H:
                        case DO_ITEM_PFa_L:
                        case DO_ITEM_PFb_H:
                        case DO_ITEM_PFb_L:
                        case DO_ITEM_PFc_H:
                        case DO_ITEM_PFc_L:
                        case DO_ITEM_PF_H:
                        case DO_ITEM_PF_L:
                            Decimal = 3;
                            break;
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_UTHa_H:
                        case DO_ITEM_UTHb_H:
                        case DO_ITEM_UTHc_H:
                        case DO_ITEM_UTH_H:
                        case DO_ITEM_ITHa_H:
                        case DO_ITEM_ITHb_H:
                        case DO_ITEM_ITHc_H:
                        case DO_ITEM_ITH_H:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_IMBU_H:
                        case DO_ITEM_IMBI_H:
                            Decimal = 2;
                            break;
                        /*case DO_ITEM_Io_H:
                            case DO_ITEM_Io_L:
                            Decimal = 3;
                            break;*/
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				case MENU2_5_INDEX_HYS:
					HT_Write4Value(1,DISP_NC,DISP_H,DISP_Y,DISP_s);
					switch(DispCtrlParam.DO2_Item)
				    {
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                        case DO_ITEM_Uavg_H:
                        case DO_ITEM_Uavg_L:
                        /*-------------------------------------------------------*/
						case DO_ITEM_Uab_H:
                        case DO_ITEM_Uab_L:
                        case DO_ITEM_Ubc_H:
                        case DO_ITEM_Ubc_L:
                        case DO_ITEM_Uca_H:
                        case DO_ITEM_Uca_L:
                        case DO_ITEM_ULavg_H:
                        case DO_ITEM_ULavg_L:
                            Decimal = 1;
                            break;
                        /*-------------------------------------------------------*/
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                        case DO_ITEM_Iavg_H:
                        case DO_ITEM_Iavg_L:
                            Decimal = 3;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Pa_H:
                        case DO_ITEM_Pa_L:
                        case DO_ITEM_Pb_H:
                        case DO_ITEM_Pb_L:
                        case DO_ITEM_Pc_H:
                        case DO_ITEM_Pc_L:
                        case DO_ITEM_P_H:
                        case DO_ITEM_P_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_Qa_H:
                        case DO_ITEM_Qa_L:
                        case DO_ITEM_Qb_H:
                        case DO_ITEM_Qb_L:
                        case DO_ITEM_Qc_H:
                        case DO_ITEM_Qc_L:
                        case DO_ITEM_Q_H:
                        case DO_ITEM_Q_L:
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Sa_H:
                        case DO_ITEM_Sa_L:
                        case DO_ITEM_Sb_H:
                        case DO_ITEM_Sb_L:
                        case DO_ITEM_Sc_H:
                        case DO_ITEM_Sc_L:
                        case DO_ITEM_S_H:
                        case DO_ITEM_S_L:
                            Decimal = 0;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_PFa_H:
                        case DO_ITEM_PFa_L:
                        case DO_ITEM_PFb_H:
                        case DO_ITEM_PFb_L:
                        case DO_ITEM_PFc_H:
                        case DO_ITEM_PFc_L:
                        case DO_ITEM_PF_H:
                        case DO_ITEM_PF_L:
                            Decimal = 3;
                            break;
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_UTHa_H:
                        case DO_ITEM_UTHb_H:
                        case DO_ITEM_UTHc_H:
                        case DO_ITEM_UTH_H:
                        case DO_ITEM_ITHa_H:
                        case DO_ITEM_ITHb_H:
                        case DO_ITEM_ITHc_H:
                        case DO_ITEM_ITH_H:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_IMBU_H:
                        case DO_ITEM_IMBI_H:
                            Decimal = 2;
                            break;
                        /*case DO_ITEM_Io_H:
                            case DO_ITEM_Io_L:
                            Decimal = 3;
                            break;*/
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				case MENU2_5_INDEX_DELAY:
					HT_Write4Value(1,DISP_d,DISP_E,DISP_L,DISP_Y);
                    Decimal = 2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
			}
			break;
        case MENU1_INDEX_DO3:
            HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_3);
            switch(sg_DispSetMenu2_6_Index)
			{
                case MENU2_6_INDEX_MODE:
                    HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
                    if(!sg_bSetFlashEnable)
					{
                        if(sg_DispSetValue == DO_MODE_OFF)
                            HT_Write4Value(2,DISP_NC,DISP_o,DISP_F,DISP_F);
                        else if(sg_DispSetValue == DO_MODE_ALR)
                            HT_Write4Value(2,DISP_NC,DISP_a,DISP_L,DISP_r);
                        else if(sg_DispSetValue == DO_MODE_REM)
                            HT_Write4Value(2,DISP_NC,DISP_r,DISP_E,DISP_m);
					}
					else
					{
                        HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
				    break;
				case MENU2_6_INDEX_TIME:
					HT_Write4Value(1,DISP_t,DISP_I,DISP_m,DISP_E);

                    Decimal = 2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
			    	break;
				case MENU2_6_INDEX_ITEM:
					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);

					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
							case DO_ITEM_Ua_H:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ua_L:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ub_H:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ub_L:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Uc_H:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Uc_L:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Uavg_H:
                                HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_Uavg_L:
                                HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*-------------------------------------------------------*/
							case DO_ITEM_Uab_H:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Uab_L:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Ubc_H:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Ubc_L:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Uca_H:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Uca_L:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ULavg_H:
                                HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_ULavg_L:
                                HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            /*-------------------------------------------------------*/
                            case DO_ITEM_Ia_H:
                                HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ia_L:
                                HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ib_H:
                                HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ib_L:
                                HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ic_H:
                                HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ic_L:
                                HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Iavg_H:
                                HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_Iavg_L:
                                HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Pa_H:
                                HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pa_L:
                                HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pb_H:
                                HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pb_L:
                                HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pc_H:
                                HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pc_L:
                                HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_P_H:
                                HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_P_L:
                                HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_Qa_H:
                                HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qa_L:
                                HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qb_H:
                                HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qb_L:
                                HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qc_H:
                                HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qc_L:
                                HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Q_H:
                                HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_Q_L:
                                HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Sa_H:
                                HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sa_L:
                                HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sb_H:
                                HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sb_L:
                                HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sc_H:
                                HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sc_L:
                                HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_S_H:
                                HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_S_L:
                                HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_PFa_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFa_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFb_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFb_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFc_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFc_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PF_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_PF_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                                HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_F_L:
                                HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_UTHa_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_a);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_UTHb_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_b);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_UTHc_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_C);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_UTH_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_NC);
                                //HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_ITHa_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_a);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ITHb_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_b);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ITHc_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_C);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ITH_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_NC);
                                //HT_WriteDot(2,1,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_IMBU_H:
                                HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_U);
                                //HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_IMBI_H:
                                HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_I);
                                //HT_WriteDot(2,2,TRUE);
                                break;
                            /*case DO_ITEM_Io_H:
                                HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                                case DO_ITEM_Io_L:
                                HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;*/
                            /*-----------------------------------------------------*/

                            case DO_ITEM_DI1_H:
                                HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI1_L:
                                HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_0);
                                break;
                            case DO_ITEM_DI2_H:
                                HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI2_L:
                                HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_0);
                                break;
                            case DO_ITEM_DI3_H:
                                HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI3_L:
                                HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_0);
                                break;
                            case DO_ITEM_DI4_H:
                                HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI4_L:
                                HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_0);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
			    	break;
				case MENU2_6_INDEX_UAL:
					HT_Write4Value(1,DISP_NC,DISP_U,DISP_a,DISP_L);

                    switch(DispCtrlParam.DO3_Item)
				    {
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                        case DO_ITEM_Uavg_H:
                        case DO_ITEM_Uavg_L:
                        /*-------------------------------------------------------*/
						case DO_ITEM_Uab_H:
                        case DO_ITEM_Uab_L:
                        case DO_ITEM_Ubc_H:
                        case DO_ITEM_Ubc_L:
                        case DO_ITEM_Uca_H:
                        case DO_ITEM_Uca_L:
                        case DO_ITEM_ULavg_H:
                        case DO_ITEM_ULavg_L:
                            Decimal = 1;
                            break;
                        /*-------------------------------------------------------*/
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                        case DO_ITEM_Iavg_H:
                        case DO_ITEM_Iavg_L:
                            Decimal = 3;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Pa_H:
                        case DO_ITEM_Pa_L:
                        case DO_ITEM_Pb_H:
                        case DO_ITEM_Pb_L:
                        case DO_ITEM_Pc_H:
                        case DO_ITEM_Pc_L:
                        case DO_ITEM_P_H:
                        case DO_ITEM_P_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_Qa_H:
                        case DO_ITEM_Qa_L:
                        case DO_ITEM_Qb_H:
                        case DO_ITEM_Qb_L:
                        case DO_ITEM_Qc_H:
                        case DO_ITEM_Qc_L:
                        case DO_ITEM_Q_H:
                        case DO_ITEM_Q_L:
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Sa_H:
                        case DO_ITEM_Sa_L:
                        case DO_ITEM_Sb_H:
                        case DO_ITEM_Sb_L:
                        case DO_ITEM_Sc_H:
                        case DO_ITEM_Sc_L:
                        case DO_ITEM_S_H:
                        case DO_ITEM_S_L:
                            Decimal = 0;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_PFa_H:
                        case DO_ITEM_PFa_L:
                        case DO_ITEM_PFb_H:
                        case DO_ITEM_PFb_L:
                        case DO_ITEM_PFc_H:
                        case DO_ITEM_PFc_L:
                        case DO_ITEM_PF_H:
                        case DO_ITEM_PF_L:
                            Decimal = 3;
                            break;
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_UTHa_H:
                        case DO_ITEM_UTHb_H:
                        case DO_ITEM_UTHc_H:
                        case DO_ITEM_UTH_H:
                        case DO_ITEM_ITHa_H:
                        case DO_ITEM_ITHb_H:
                        case DO_ITEM_ITHc_H:
                        case DO_ITEM_ITH_H:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_IMBU_H:
                        case DO_ITEM_IMBI_H:
                            Decimal = 2;
                            break;
                        /*case DO_ITEM_Io_H:
                            case DO_ITEM_Io_L:
                            Decimal = 3;
                            break;*/
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				case MENU2_6_INDEX_HYS:
					HT_Write4Value(1,DISP_NC,DISP_H,DISP_Y,DISP_s);
					switch(DispCtrlParam.DO3_Item)
				    {
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                        case DO_ITEM_Uavg_H:
                        case DO_ITEM_Uavg_L:
                        /*-------------------------------------------------------*/
						case DO_ITEM_Uab_H:
                        case DO_ITEM_Uab_L:
                        case DO_ITEM_Ubc_H:
                        case DO_ITEM_Ubc_L:
                        case DO_ITEM_Uca_H:
                        case DO_ITEM_Uca_L:
                        case DO_ITEM_ULavg_H:
                        case DO_ITEM_ULavg_L:
                            Decimal = 1;
                            break;
                        /*-------------------------------------------------------*/
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                        case DO_ITEM_Iavg_H:
                        case DO_ITEM_Iavg_L:
                            Decimal = 3;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Pa_H:
                        case DO_ITEM_Pa_L:
                        case DO_ITEM_Pb_H:
                        case DO_ITEM_Pb_L:
                        case DO_ITEM_Pc_H:
                        case DO_ITEM_Pc_L:
                        case DO_ITEM_P_H:
                        case DO_ITEM_P_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_Qa_H:
                        case DO_ITEM_Qa_L:
                        case DO_ITEM_Qb_H:
                        case DO_ITEM_Qb_L:
                        case DO_ITEM_Qc_H:
                        case DO_ITEM_Qc_L:
                        case DO_ITEM_Q_H:
                        case DO_ITEM_Q_L:
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Sa_H:
                        case DO_ITEM_Sa_L:
                        case DO_ITEM_Sb_H:
                        case DO_ITEM_Sb_L:
                        case DO_ITEM_Sc_H:
                        case DO_ITEM_Sc_L:
                        case DO_ITEM_S_H:
                        case DO_ITEM_S_L:
                            Decimal = 0;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_PFa_H:
                        case DO_ITEM_PFa_L:
                        case DO_ITEM_PFb_H:
                        case DO_ITEM_PFb_L:
                        case DO_ITEM_PFc_H:
                        case DO_ITEM_PFc_L:
                        case DO_ITEM_PF_H:
                        case DO_ITEM_PF_L:
                            Decimal = 3;
                            break;
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_UTHa_H:
                        case DO_ITEM_UTHb_H:
                        case DO_ITEM_UTHc_H:
                        case DO_ITEM_UTH_H:
                        case DO_ITEM_ITHa_H:
                        case DO_ITEM_ITHb_H:
                        case DO_ITEM_ITHc_H:
                        case DO_ITEM_ITH_H:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_IMBU_H:
                        case DO_ITEM_IMBI_H:
                            Decimal = 2;
                            break;
                        /*case DO_ITEM_Io_H:
                            case DO_ITEM_Io_L:
                            Decimal = 3;
                            break;*/
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				case MENU2_6_INDEX_DELAY:
					HT_Write4Value(1,DISP_d,DISP_E,DISP_L,DISP_Y);
                    Decimal = 2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
		    }
			break;
        case MENU1_INDEX_DO4:
            HT_Write4Value(0,DISP1_d,DISP1_o,DISP1_FU,DISP1_4);
            switch(sg_DispSetMenu2_7_Index)
			{
                case MENU2_7_INDEX_MODE:
                    HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
                    if(!sg_bSetFlashEnable)
					{
                        if(sg_DispSetValue == DO_MODE_OFF)
                            HT_Write4Value(2,DISP_NC,DISP_o,DISP_F,DISP_F);
                        else if(sg_DispSetValue == DO_MODE_ALR)
                            HT_Write4Value(2,DISP_NC,DISP_a,DISP_L,DISP_r);
                        else if(sg_DispSetValue == DO_MODE_REM)
                            HT_Write4Value(2,DISP_NC,DISP_r,DISP_E,DISP_m);
					}
					else
					{
                        HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
				    break;
				case MENU2_7_INDEX_TIME:
					HT_Write4Value(1,DISP_t,DISP_I,DISP_m,DISP_E);

                    Decimal =  2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
			    	break;
				case MENU2_7_INDEX_ITEM:
					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);

					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
							case DO_ITEM_Ua_H:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ua_L:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ub_H:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ub_L:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Uc_H:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Uc_L:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Uavg_H:
                                HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_Uavg_L:
                                HT_Write4Value(2,DISP_U,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*-------------------------------------------------------*/
							case DO_ITEM_Uab_H:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Uab_L:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Ubc_H:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Ubc_L:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Uca_H:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_Uca_L:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ULavg_H:
                                HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_ULavg_L:
                                HT_Write4Value(2,DISP_U,DISP_L,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            /*-------------------------------------------------------*/
                            case DO_ITEM_Ia_H:
                                HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ia_L:
                                HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ib_H:
                                HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ib_L:
                                HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ic_H:
                                HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Ic_L:
                                HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Iavg_H:
                                HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_Iavg_L:
                                HT_Write4Value(2,DISP_I,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Pa_H:
                                HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pa_L:
                                HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pb_H:
                                HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pb_L:
                                HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pc_H:
                                HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Pc_L:
                                HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_P_H:
                                HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_P_L:
                                HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_Qa_H:
                                HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qa_L:
                                HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qb_H:
                                HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qb_L:
                                HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qc_H:
                                HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Qc_L:
                                HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Q_H:
                                HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_Q_L:
                                HT_Write4Value(2,DISP_q,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_Sa_H:
                                HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sa_L:
                                HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sb_H:
                                HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sb_L:
                                HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sc_H:
                                HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_Sc_L:
                                HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_S_H:
                                HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_S_L:
                                HT_Write4Value(2,DISP_s,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*------------------------------------------------------*/
                            case DO_ITEM_PFa_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFa_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFb_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFb_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFc_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_H);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PFc_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_L);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_PF_H:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_PF_L:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                                HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_H);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            case DO_ITEM_F_L:
                                HT_Write4Value(2,DISP_F,DISP_NC,DISP_NC,DISP_L);
                                HT_WriteDot(2,0,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_UTHa_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_a);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_UTHb_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_b);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_UTHc_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_C);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_UTH_H:
                                HT_Write4Value(2,DISP_U,DISP_t,DISP_H,DISP_NC);
                                //HT_WriteDot(2,1,TRUE);
                                break;
                            case DO_ITEM_ITHa_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_a);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ITHb_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_b);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ITHc_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_C);
                                HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_ITH_H:
                                HT_Write4Value(2,DISP_I,DISP_t,DISP_H,DISP_NC);
                                //HT_WriteDot(2,1,TRUE);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_IMBU_H:
                                HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_U);
                                //HT_WriteDot(2,2,TRUE);
                                break;
                            case DO_ITEM_IMBI_H:
                                HT_Write4Value(2,DISP_I,DISP_m,DISP_b,DISP_I);
                                //HT_WriteDot(2,2,TRUE);
                                break;
                            /*case DO_ITEM_Io_H:
                                HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_H);
                                HT_WriteDot(2,1,TRUE);
                                break;
                                case DO_ITEM_Io_L:
                                HT_Write4Value(2,DISP_I,DISP_o,DISP_NC,DISP_L);
                                HT_WriteDot(2,1,TRUE);
                                break;*/
                            /*-----------------------------------------------------*/

                            case DO_ITEM_DI1_H:
                                HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI1_L:
                                HT_Write4Value(2,DISP_d,DISP_1,DISP_FU,DISP_0);
                                break;
                            case DO_ITEM_DI2_H:
                                HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI2_L:
                                HT_Write4Value(2,DISP_d,DISP_2,DISP_FU,DISP_0);
                                break;
                            case DO_ITEM_DI3_H:
                                HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI3_L:
                                HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_0);
                                break;
                            case DO_ITEM_DI4_H:
                                HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_1);
                                break;
                            case DO_ITEM_DI4_L:
                                HT_Write4Value(2,DISP_d,DISP_4,DISP_FU,DISP_0);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
			    	break;
				case MENU2_7_INDEX_UAL:
					HT_Write4Value(1,DISP_NC,DISP_U,DISP_a,DISP_L);

                    switch(DispCtrlParam.DO4_Item)
				    {
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                        case DO_ITEM_Uavg_H:
                        case DO_ITEM_Uavg_L:
                        /*-------------------------------------------------------*/
						case DO_ITEM_Uab_H:
                        case DO_ITEM_Uab_L:
                        case DO_ITEM_Ubc_H:
                        case DO_ITEM_Ubc_L:
                        case DO_ITEM_Uca_H:
                        case DO_ITEM_Uca_L:
                        case DO_ITEM_ULavg_H:
                        case DO_ITEM_ULavg_L:
                            Decimal = 1;
                            break;
                        /*-------------------------------------------------------*/
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                        case DO_ITEM_Iavg_H:
                        case DO_ITEM_Iavg_L:
                            Decimal = 3;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Pa_H:
                        case DO_ITEM_Pa_L:
                        case DO_ITEM_Pb_H:
                        case DO_ITEM_Pb_L:
                        case DO_ITEM_Pc_H:
                        case DO_ITEM_Pc_L:
                        case DO_ITEM_P_H:
                        case DO_ITEM_P_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_Qa_H:
                        case DO_ITEM_Qa_L:
                        case DO_ITEM_Qb_H:
                        case DO_ITEM_Qb_L:
                        case DO_ITEM_Qc_H:
                        case DO_ITEM_Qc_L:
                        case DO_ITEM_Q_H:
                        case DO_ITEM_Q_L:
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Sa_H:
                        case DO_ITEM_Sa_L:
                        case DO_ITEM_Sb_H:
                        case DO_ITEM_Sb_L:
                        case DO_ITEM_Sc_H:
                        case DO_ITEM_Sc_L:
                        case DO_ITEM_S_H:
                        case DO_ITEM_S_L:
                            Decimal = 0;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_PFa_H:
                        case DO_ITEM_PFa_L:
                        case DO_ITEM_PFb_H:
                        case DO_ITEM_PFb_L:
                        case DO_ITEM_PFc_H:
                        case DO_ITEM_PFc_L:
                        case DO_ITEM_PF_H:
                        case DO_ITEM_PF_L:
                            Decimal = 3;
                            break;
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_UTHa_H:
                        case DO_ITEM_UTHb_H:
                        case DO_ITEM_UTHc_H:
                        case DO_ITEM_UTH_H:
                        case DO_ITEM_ITHa_H:
                        case DO_ITEM_ITHb_H:
                        case DO_ITEM_ITHc_H:
                        case DO_ITEM_ITH_H:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_IMBU_H:
                        case DO_ITEM_IMBI_H:
                            Decimal = 2;
                            break;
                        /*case DO_ITEM_Io_H:
                            case DO_ITEM_Io_L:
                            Decimal = 3;
                            break;*/
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				case MENU2_7_INDEX_HYS:
					HT_Write4Value(1,DISP_NC,DISP_H,DISP_Y,DISP_s);
					switch(DispCtrlParam.DO4_Item)
				    {
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                        case DO_ITEM_Uavg_H:
                        case DO_ITEM_Uavg_L:
                        /*-------------------------------------------------------*/
						case DO_ITEM_Uab_H:
                        case DO_ITEM_Uab_L:
                        case DO_ITEM_Ubc_H:
                        case DO_ITEM_Ubc_L:
                        case DO_ITEM_Uca_H:
                        case DO_ITEM_Uca_L:
                        case DO_ITEM_ULavg_H:
                        case DO_ITEM_ULavg_L:
                            Decimal = 1;
                            break;
                        /*-------------------------------------------------------*/
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                        case DO_ITEM_Iavg_H:
                        case DO_ITEM_Iavg_L:
                            Decimal = 3;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Pa_H:
                        case DO_ITEM_Pa_L:
                        case DO_ITEM_Pb_H:
                        case DO_ITEM_Pb_L:
                        case DO_ITEM_Pc_H:
                        case DO_ITEM_Pc_L:
                        case DO_ITEM_P_H:
                        case DO_ITEM_P_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_Qa_H:
                        case DO_ITEM_Qa_L:
                        case DO_ITEM_Qb_H:
                        case DO_ITEM_Qb_L:
                        case DO_ITEM_Qc_H:
                        case DO_ITEM_Qc_L:
                        case DO_ITEM_Q_H:
                        case DO_ITEM_Q_L:
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_Sa_H:
                        case DO_ITEM_Sa_L:
                        case DO_ITEM_Sb_H:
                        case DO_ITEM_Sb_L:
                        case DO_ITEM_Sc_H:
                        case DO_ITEM_Sc_L:
                        case DO_ITEM_S_H:
                        case DO_ITEM_S_L:
                            Decimal = 0;
                            break;
                        /*------------------------------------------------------*/
                        case DO_ITEM_PFa_H:
                        case DO_ITEM_PFa_L:
                        case DO_ITEM_PFb_H:
                        case DO_ITEM_PFb_L:
                        case DO_ITEM_PFc_H:
                        case DO_ITEM_PFc_L:
                        case DO_ITEM_PF_H:
                        case DO_ITEM_PF_L:
                            Decimal = 3;
                            break;
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_UTHa_H:
                        case DO_ITEM_UTHb_H:
                        case DO_ITEM_UTHc_H:
                        case DO_ITEM_UTH_H:
                        case DO_ITEM_ITHa_H:
                        case DO_ITEM_ITHb_H:
                        case DO_ITEM_ITHc_H:
                        case DO_ITEM_ITH_H:
                        /*-----------------------------------------------------*/
                        case DO_ITEM_IMBU_H:
                        case DO_ITEM_IMBI_H:
                            Decimal = 2;
                            break;
                        /*case DO_ITEM_Io_H:
                            case DO_ITEM_Io_L:
                            Decimal = 3;
                            break;*/
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				case MENU2_7_INDEX_DELAY:
					HT_Write4Value(1,DISP_d,DISP_E,DISP_L,DISP_Y);
                    Decimal = 2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
		    }
			break;
        case MENU1_INDEX_AO1:
            HT_Write4Value(0,DISP1_A,DISP1_o,DISP1_FU,DISP1_1);
            switch(sg_DispSetMenu2_8_Index)
			{
                case MENU2_8_INDEX_MODE:
                    HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
                    if(!sg_bSetFlashEnable)
					{
					    if(sg_DispSetValue == AO_MODE_OFF)
					        HT_Write4Value(2,DISP_NC,DISP_o,DISP_F,DISP_F);
                        else if(sg_DispSetValue == AO_MODE_420)
                            HT_Write4Value(2,DISP_4,DISP_FU,DISP_2,DISP_0);
                        else if(sg_DispSetValue == AO_MODE_020)
                            HT_Write4Value(2,DISP_0,DISP_FU,DISP_2,DISP_0);
                        else if(sg_DispSetValue == AO_MODE_1220)
                        {
                            HT_Write4Value(2,DISP_1,DISP_2,DISP_2,DISP_0);
                            HT_WriteDot(2,1,TRUE);
                        }
					}
					else
					{
                        HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
				    break;
				case MENU2_8_INDEX_ITEM:
					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);
					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
                            case AO_ITEM_UA:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_UB:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_UC:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_UAB:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_NC);
                                break;
                            case AO_ITEM_UBC:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_NC);
                                break;
                            case AO_ITEM_UCA:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_NC);
                                break;
                            case AO_ITEM_IA:
                                HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_IB:
                                HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_IC:
                                HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_PA:
                                HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_PB:
                                HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_PC:
                                HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_P:
                                HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_QA:
                                HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_QB:
                                HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_QC:
                                HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_Q:
                                HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_SA:
                                HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_SB:
                                HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_SC:
                                HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_S:
                                HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_PFA:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_NC);
                                break;
                            case AO_ITEM_PFB:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_NC);
                                break;
                            case AO_ITEM_PFC:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_NC);
                                break;
                            case AO_ITEM_PF:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_F:
                                HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_1);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
			    	break;
				case MENU2_8_INDEX_DS:
					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_d,DISP_s);
					switch(DispCtrlParam.AO1_Item)
				    {
                        case AO_ITEM_UA:
                        case AO_ITEM_UB:
                        case AO_ITEM_UC:
                        case AO_ITEM_UAB:
                        case AO_ITEM_UBC:
                        case AO_ITEM_UCA:
                            Decimal = 1;
                            break;
                        case AO_ITEM_IA:
                        case AO_ITEM_IB:
                        case AO_ITEM_IC:
                            Decimal = 3;
                            break;
                        case AO_ITEM_PA:
                        case AO_ITEM_PB:
                        case AO_ITEM_PC:
                        case AO_ITEM_P:
                        case AO_ITEM_QA:
                        case AO_ITEM_QB:
                        case AO_ITEM_QC:
                        case AO_ITEM_Q:
                        case AO_ITEM_SA:
                        case AO_ITEM_SB:
                        case AO_ITEM_SC:
                        case AO_ITEM_S:
                            Decimal = 0;
                            break;
                        case AO_ITEM_PFA:
                        case AO_ITEM_PFB:
                        case AO_ITEM_PFC:
                        case AO_ITEM_PF:
                            Decimal = 3;
                            break;
                        case AO_ITEM_F:
                            Decimal = 2;
                            break;
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				case MENU2_8_INDEX_FS:
					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_F,DISP_s);
					switch(DispCtrlParam.AO1_Item)
				    {
                        case AO_ITEM_UA:
                        case AO_ITEM_UB:
                        case AO_ITEM_UC:
                        case AO_ITEM_UAB:
                        case AO_ITEM_UBC:
                        case AO_ITEM_UCA:
                            Decimal = 1;
                            break;
                        case AO_ITEM_IA:
                        case AO_ITEM_IB:
                        case AO_ITEM_IC:
                            Decimal = 3;
                            break;
                        case AO_ITEM_PA:
                        case AO_ITEM_PB:
                        case AO_ITEM_PC:
                        case AO_ITEM_P:
                        case AO_ITEM_QA:
                        case AO_ITEM_QB:
                        case AO_ITEM_QC:
                        case AO_ITEM_Q:
                        case AO_ITEM_SA:
                        case AO_ITEM_SB:
                        case AO_ITEM_SC:
                        case AO_ITEM_S:
                            Decimal = 0;
                            break;
                        case AO_ITEM_PFA:
                        case AO_ITEM_PFB:
                        case AO_ITEM_PFC:
                        case AO_ITEM_PF:
                            Decimal = 3;
                            break;
                        case AO_ITEM_F:
                            Decimal = 2;
                            break;
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
            }
            break;
        case MENU1_INDEX_AO2:
            HT_Write4Value(0,DISP1_A,DISP1_o,DISP1_FU,DISP1_2);
            switch(sg_DispSetMenu2_9_Index)
			{
                case MENU2_9_INDEX_MODE:
                    HT_Write4Value(1,DISP_m,DISP_o,DISP_d,DISP_E);
                    if(!sg_bSetFlashEnable)
					{
					    if(sg_DispSetValue == AO_MODE_OFF)
					        HT_Write4Value(2,DISP_NC,DISP_o,DISP_F,DISP_F);
                        else if(sg_DispSetValue == AO_MODE_420)
                            HT_Write4Value(2,DISP_4,DISP_FU,DISP_2,DISP_0);
                        else if(sg_DispSetValue == AO_MODE_020)
                            HT_Write4Value(2,DISP_0,DISP_FU,DISP_2,DISP_0);
                        else if(sg_DispSetValue == AO_MODE_1220)
                        {
                            HT_Write4Value(2,DISP_1,DISP_2,DISP_2,DISP_0);
                            HT_WriteDot(2,1,TRUE);
                        }
					}
					else
					{
                        HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
				    break;
				case MENU2_9_INDEX_ITEM:
					HT_Write4Value(1,DISP_I,DISP_t,DISP_E,DISP_m);

					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
                            case AO_ITEM_UA:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_UB:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_UC:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_UAB:
                                HT_Write4Value(2,DISP_U,DISP_a,DISP_b,DISP_NC);
                                break;
                            case AO_ITEM_UBC:
                                HT_Write4Value(2,DISP_U,DISP_b,DISP_C,DISP_NC);
                                break;
                            case AO_ITEM_UCA:
                                HT_Write4Value(2,DISP_U,DISP_C,DISP_a,DISP_NC);
                                break;
                            case AO_ITEM_IA:
                                HT_Write4Value(2,DISP_I,DISP_a,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_IB:
                                HT_Write4Value(2,DISP_I,DISP_b,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_IC:
                                HT_Write4Value(2,DISP_I,DISP_C,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_PA:
                                HT_Write4Value(2,DISP_p,DISP_a,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_PB:
                                HT_Write4Value(2,DISP_p,DISP_b,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_PC:
                                HT_Write4Value(2,DISP_p,DISP_C,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_P:
                                HT_Write4Value(2,DISP_p,DISP_NC,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_QA:
                                HT_Write4Value(2,DISP_q,DISP_a,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_QB:
                                HT_Write4Value(2,DISP_q,DISP_b,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_QC:
                                HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_Q:
                                HT_Write4Value(2,DISP_q,DISP_C,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_SA:
                                HT_Write4Value(2,DISP_s,DISP_a,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_SB:
                                HT_Write4Value(2,DISP_s,DISP_b,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_SC:
                                HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_S:
                                HT_Write4Value(2,DISP_s,DISP_C,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_PFA:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_a,DISP_NC);
                                break;
                            case AO_ITEM_PFB:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_b,DISP_NC);
                                break;
                            case AO_ITEM_PFC:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_C,DISP_NC);
                                break;
                            case AO_ITEM_PF:
                                HT_Write4Value(2,DISP_p,DISP_F,DISP_NC,DISP_NC);
                                break;
                            case AO_ITEM_F:
                                HT_Write4Value(2,DISP_d,DISP_3,DISP_FU,DISP_1);
                                break;
                            default:
                                break;

					    }
					}
					else
					{
                        HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
					}
			    	break;
				case MENU2_9_INDEX_DS:
					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_d,DISP_s);
					switch(DispCtrlParam.AO2_Item)
				    {
                        case AO_ITEM_UA:
                        case AO_ITEM_UB:
                        case AO_ITEM_UC:
                        case AO_ITEM_UAB:
                        case AO_ITEM_UBC:
                        case AO_ITEM_UCA:
                            Decimal = 1;
                            break;
                        case AO_ITEM_IA:
                        case AO_ITEM_IB:
                        case AO_ITEM_IC:
                            Decimal = 3;
                            break;
                        case AO_ITEM_PA:
                        case AO_ITEM_PB:
                        case AO_ITEM_PC:
                        case AO_ITEM_P:
                        case AO_ITEM_QA:
                        case AO_ITEM_QB:
                        case AO_ITEM_QC:
                        case AO_ITEM_Q:
                        case AO_ITEM_SA:
                        case AO_ITEM_SB:
                        case AO_ITEM_SC:
                        case AO_ITEM_S:
                            Decimal = 0;
                            break;
                        case AO_ITEM_PFA:
                        case AO_ITEM_PFB:
                        case AO_ITEM_PFC:
                        case AO_ITEM_PF:
                            Decimal = 3;
                            break;
                        case AO_ITEM_F:
                            Decimal = 2;
                            break;
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				case MENU2_9_INDEX_FS:
					HT_Write4Value(1,DISP_NC,DISP_NC,DISP_F,DISP_s);
					switch(DispCtrlParam.AO2_Item)
				    {
                        case AO_ITEM_UA:
                        case AO_ITEM_UB:
                        case AO_ITEM_UC:
                        case AO_ITEM_UAB:
                        case AO_ITEM_UBC:
                        case AO_ITEM_UCA:
                            Decimal = 1;
                            break;
                        case AO_ITEM_IA:
                        case AO_ITEM_IB:
                        case AO_ITEM_IC:
                            Decimal = 3;
                            break;
                        case AO_ITEM_PA:
                        case AO_ITEM_PB:
                        case AO_ITEM_PC:
                        case AO_ITEM_P:
                        case AO_ITEM_QA:
                        case AO_ITEM_QB:
                        case AO_ITEM_QC:
                        case AO_ITEM_Q:
                        case AO_ITEM_SA:
                        case AO_ITEM_SB:
                        case AO_ITEM_SC:
                        case AO_ITEM_S:
                            Decimal = 0;
                            break;
                        case AO_ITEM_PFA:
                        case AO_ITEM_PFB:
                        case AO_ITEM_PFC:
                        case AO_ITEM_PF:
                            Decimal = 3;
                            break;
                        case AO_ITEM_F:
                            Decimal = 2;
                            break;
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
            }
            break;
		default:
		    break;
	}
}


void DispMenu_MenuCode(void)    //显示密码界面
{
        /**************标签页********************************************************/
        HT_WriteValue(Line_R5,0,DISP1_p);
        HT_WriteValue(Line_R5,1,DISP1_r);
        HT_WriteValue(Line_R5,2,CHAR_NONE);
        /*****************************************************************************/
		u8 Decimal = 0;
		HT_Write4Value(0,DISP1_p,DISP1_r,DISP1_o,DISP1_G);
		HT_Write4Value(1,DISP_C,DISP_o,DISP_d,DISP_E);
		Decimal =  0;
		DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
		CharToNumber();
}

void DispMenu_MenuCodeErr(void)
{
		HT_Write4Value(0,DISP1_p,DISP1_r,DISP1_o,DISP1_G);
		HT_Write4Value(1,DISP_C,DISP_o,DISP_d,DISP_E);
		if(sg_bSetFlashEnable)
			HT_Write4Value(2,DISP_NC,DISP_E,DISP_r,DISP_r);
		else
			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
}


void DispMenu_MenuSave(void)
{
		HT_Write4Value(0,DISP1_s,DISP1_A,DISP1_U,DISP1_E);
		HT_Write4Value(1,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
        if( sg_DispSetValue == SaveYes)
		{
            if(sg_bSetFlashEnable)
    			HT_Write4Value(2,DISP_NC,DISP_Y,DISP_E,DISP_s);
    		else
    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
		}
		else if( sg_DispSetValue == SaveNO)
		{
            if(sg_bSetFlashEnable)
    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_n,DISP_o);
    		else
    			HT_Write4Value(2,DISP_NC,DISP_NC,DISP_NC,DISP_NC);
		}
}


const u16 cg_CharToNum[4] = {1000, 100, 10, 1};

/****************************************************************************************
** 函数名称: CharToNumber
** 功能描述: 在显示设置菜单中的字符数据转换为十进制数据
** 参    数: void
** 返 回 值: void
** 作　  者:
** 日  　期: 2011年11月13日
**---------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**--------------------------------------------------------------------------------------
****************************************************************************************/
u16 CharToNumber(void)
{
	u8 i;
	u16 TotalData = 0;

	for (i = 0; i < 4; i++)
	{
		if (sg_CurrentDispChar[i] != NEG_NUM && sg_CurrentDispChar[i] != POS_NUM)
		{
			TotalData += sg_CurrentDispChar[i] * cg_CharToNum[i];
		}
	}
	sg_CurrentDispNumber = TotalData;

	return TotalData;
}


/****************************************************************************************
** 函数名称: GetSetOldValue
** 功能描述: 显示设置菜单中要显示的数据
** 参    数: BOOL bUpdata-是否要闪烁焦点
** 返 回 值: void
** 作　  者:
** 日  　期: 2011年11月13日
**---------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**--------------------------------------------------------------------------------------
****************************************************************************************/
void GetSetOldValue(void)
{
	u8 i = 0;
	for (i = 0; i < 4; i++)
	{
		sg_CurrentDispChar[i] = 0x00;
	}
	Disp_Focus.DispWidth = 4;
	Disp_Focus.CurrentFocus = CURRENTFOCUS_3; //退出后更新聚点
	switch(sg_DispSetMenu1Index)
	{
		case MENU1_INDEX_INPT:
			switch(sg_DispSetMenu2_1_Index)
			{
                case MENU2_1_INDEX_NET:
                    sg_DispSetValue = DispCtrlParam.NetMode;
                    break;
				case MENU2_1_INDEX_PT:
					sg_DispSetValue = DispCtrlParam.PTNum;
				    break;
				case MENU2_1_INDEX_CT:
					sg_DispSetValue = DispCtrlParam.CTNum;
				    break;
				default:
				    break;
			}
		    break;
        case MENU1_INDEX_COM1:
			switch(sg_DispSetMenu2_2_Index)
			{
				case MENU2_2_INDEX_ADD:
					sg_DispSetValue = DispCtrlParam.Addr485;
				    break;
				case MENU2_2_INDEX_BAUD:
					sg_DispSetValue = DispCtrlParam.Baud485;
			    	break;
				case MENU2_2_INDEX_DATA:
					sg_DispSetValue = DispCtrlParam.DataFormat;
				    break;
				case MENU2_2_INDEX_COMS:
					sg_DispSetValue = DispCtrlParam.DataProtocol;
				    break;
				default:
				    break;
			}
		    break;
		case MENU1_INDEX_SYS:
			switch(sg_DispSetMenu2_3_Index)
			{
				case MENU2_3_INDEX_CYC:
					sg_DispSetValue = DispCtrlParam.Cycle;
				    break;
				case MENU2_3_INDEX_LIGH:
					sg_DispSetValue = DispCtrlParam.BlackTime;
				    break;
				case MENU2_3_INDEX_DISP:
					sg_DispSetValue = DispCtrlParam.DefaultDisp;
				    break;
				case MENU2_3_INDEX_ALR:
					sg_DispSetValue = DispCtrlParam.Alarm;
				    break;
				case MENU2_3_INDEX_CODE:
					sg_DispSetValue = DispCtrlParam.Code;
				    break;
				case MENU2_3_INDEX_CLRE:
					sg_DispSetValue = SaveNO;
				    break;
				default:
				    break;
			}
		    break;
		case MENU1_INDEX_DO1:
			switch(sg_DispSetMenu2_4_Index)
			{
				case MENU2_4_INDEX_MODE:
					sg_DispSetValue = DispCtrlParam.DO1_Mode;
				    break;
				case MENU2_4_INDEX_TIME:
					sg_DispSetValue = DispCtrlParam.DO1_Time;
				    break;
				case MENU2_4_INDEX_ITEM:
					if((g_tParam.CtrlParam.NetMode == N33) && (DispCtrlParam.DO1_Item < DO_ITEM_Uab_H))
				    {
                        sg_DispSetValue = DO_ITEM_Uab_H;
				    }
				    else
				    {
                        sg_DispSetValue = DispCtrlParam.DO1_Item;
				    }
				    break;
				case MENU2_4_INDEX_UAL:
					sg_DispSetValue = DispCtrlParam.DO1_UAL;
				    break;
				case MENU2_4_INDEX_HYS:
					sg_DispSetValue = DispCtrlParam.DO1_HYS;
				    break;
				case MENU2_4_INDEX_DELAY:
					sg_DispSetValue = DispCtrlParam.DO1_Delay;
				    break;
				default:
				    break;
			}
		    break;
		case MENU1_INDEX_DO2:
			switch(sg_DispSetMenu2_5_Index)
			{
				case MENU2_5_INDEX_MODE:
					sg_DispSetValue = DispCtrlParam.DO2_Mode;
				    break;
				case MENU2_5_INDEX_TIME:
					sg_DispSetValue = DispCtrlParam.DO2_Time;
				    break;
				case MENU2_5_INDEX_ITEM:
					if((g_tParam.CtrlParam.NetMode == N33) && (DispCtrlParam.DO2_Item < DO_ITEM_Uab_H))
				    {
                        sg_DispSetValue = DO_ITEM_Uab_H;
				    }
				    else
				    {
                        sg_DispSetValue = DispCtrlParam.DO2_Item;
				    }
				    break;
				case MENU2_5_INDEX_UAL:
					sg_DispSetValue = DispCtrlParam.DO2_UAL;
				    break;
				case MENU2_5_INDEX_HYS:
					sg_DispSetValue = DispCtrlParam.DO2_HYS;
				    break;
				case MENU2_5_INDEX_DELAY:
					sg_DispSetValue = DispCtrlParam.DO2_Delay;
				    break;
				default:
				    break;
			}
		    break;
		case MENU1_INDEX_DO3:
			switch(sg_DispSetMenu2_6_Index)
			{
				case MENU2_6_INDEX_MODE:
					sg_DispSetValue = DispCtrlParam.DO3_Mode;
				    break;
				case MENU2_6_INDEX_TIME:
					sg_DispSetValue = DispCtrlParam.DO3_Time;
				    break;
				case MENU2_6_INDEX_ITEM:
					if((g_tParam.CtrlParam.NetMode == N33) && (DispCtrlParam.DO3_Item < DO_ITEM_Uab_H))
				    {
                        sg_DispSetValue = DO_ITEM_Uab_H;
				    }
				    else
				    {
                        sg_DispSetValue = DispCtrlParam.DO3_Item;
				    }
				    break;
				case MENU2_6_INDEX_UAL:
					sg_DispSetValue = DispCtrlParam.DO3_UAL;
				    break;
				case MENU2_6_INDEX_HYS:
					sg_DispSetValue = DispCtrlParam.DO3_HYS;
				    break;
				case MENU2_6_INDEX_DELAY:
					sg_DispSetValue = DispCtrlParam.DO3_Delay;
				    break;
				default:
				    break;
			}
		    break;
		case MENU1_INDEX_DO4:
			switch(sg_DispSetMenu2_7_Index)
			{
				case MENU2_7_INDEX_MODE:
					sg_DispSetValue = DispCtrlParam.DO4_Mode;
				    break;
				case MENU2_7_INDEX_TIME:
					sg_DispSetValue = DispCtrlParam.DO4_Time;
				    break;
				case MENU2_7_INDEX_ITEM:
					if((g_tParam.CtrlParam.NetMode == N33) && (DispCtrlParam.DO4_Item < DO_ITEM_Uab_H))
				    {
                        sg_DispSetValue = DO_ITEM_Uab_H;
				    }
				    else
				    {
                        sg_DispSetValue = DispCtrlParam.DO4_Item;
				    }
				    break;
				case MENU2_7_INDEX_UAL:
					sg_DispSetValue = DispCtrlParam.DO4_UAL;
				    break;
				case MENU2_7_INDEX_HYS:
					sg_DispSetValue = DispCtrlParam.DO4_HYS;
				    break;
				case MENU2_7_INDEX_DELAY:
					sg_DispSetValue = DispCtrlParam.DO4_Delay;
				    break;
				default:
				    break;
			}
		    break;
		case MENU1_INDEX_AO1:
			switch(sg_DispSetMenu2_8_Index)
			{
				case MENU2_8_INDEX_MODE:
					sg_DispSetValue = DispCtrlParam.AO1_Mode;
				    break;
				case MENU2_8_INDEX_ITEM:
					if((g_tParam.CtrlParam.NetMode == N33) && ( DispCtrlParam.AO1_Item < AO_ITEM_UAB))
				    {
                        sg_DispSetValue = AO_ITEM_UAB;
				    }
				    else
				    {
                        sg_DispSetValue = DispCtrlParam.AO1_Item;
				    }
				    break;
				case MENU2_8_INDEX_DS:
					sg_DispSetValue = DispCtrlParam.AO1_DS;
				    break;
				case MENU2_8_INDEX_FS:
					sg_DispSetValue = DispCtrlParam.AO1_FS;
				    break;
				default:
				    break;
			}
		    break;
		case MENU1_INDEX_AO2:
			switch(sg_DispSetMenu2_9_Index)
			{
				case MENU2_9_INDEX_MODE:
					sg_DispSetValue = DispCtrlParam.AO2_Mode;
				    break;
				case MENU2_9_INDEX_ITEM:
					if((g_tParam.CtrlParam.NetMode == N33) && ( DispCtrlParam.AO2_Item < AO_ITEM_UAB))
				    {
                        sg_DispSetValue = AO_ITEM_UAB;
				    }
				    else
				    {
                        sg_DispSetValue = DispCtrlParam.AO2_Item;
				    }
				    break;
				case MENU2_9_INDEX_DS:
					sg_DispSetValue = DispCtrlParam.AO2_DS;
				    break;
				case MENU2_9_INDEX_FS:
					sg_DispSetValue = DispCtrlParam.AO2_FS;
				    break;
				default:
				    break;
			}
		    break;
		default:
		    break;
	}
}


/*-----------------------------------------------------------------------------------------------------------------*/
s32 JudgeSetNewValue(BOOL *pUpdataFlag, s32 OldValue, s32 *pNewValue, s32 MinValue, s32 MaxValue)
{
	if (*pNewValue >= MinValue && *pNewValue <= MaxValue)
	{
		if (OldValue != *pNewValue)
		{
		    *pUpdataFlag = TRUE;
			return *pNewValue;
		}
	}
	*pUpdataFlag = FALSE;
	*pNewValue = OldValue;
		return OldValue;
}

/*----------------------------------------------------------------------------------------------------------------*/
BOOL JudgeNewParam(BOOL *pUpdataFlag, s32 *pNewValue,s32 MinValue, s32 MaxValue)
{
    if (*pNewValue >= MinValue && *pNewValue <= MaxValue)
	{
		*pUpdataFlag = TRUE;
	}
	else
	{
        *pUpdataFlag = FALSE;
	}
	return *pUpdataFlag;
}
/*-----------------------------------------------------------------------------------------------------------------*/
/****************************************************************************************
** 函数名称: GetSetNewValue
** 功能描述: 保存设置好的菜单中数据,只有数据不一样时才更新保存到FLASH中
** 参    数: void
** 返 回 值: void
** 作　  者:
** 日  　期: 2011年11月11日
**---------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**--------------------------------------------------------------------------------------
****************************************************************************************/

void GetSetNewValue(void)
{
    BOOL UpData = FALSE;
    switch(sg_DispSetMenu1Index)
	{
		case MENU1_INDEX_INPT:
			switch(sg_DispSetMenu2_1_Index)
			{
                case MENU2_1_INDEX_NET:
                    DispCtrlParam.NetMode =
                        (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.NetMode,(s32*)&sg_DispSetValue,MIN_NetMode,MAX_NetMode);
                    break;
				case MENU2_1_INDEX_PT:
					DispCtrlParam.PTNum =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.PTNum,(s32*)&sg_DispSetValue,MIN_PT,MAX_PT);
				    break;
				case MENU2_1_INDEX_CT:
					DispCtrlParam.CTNum =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.CTNum,(s32*)&sg_DispSetValue,MIN_CT,MAX_CT);
				    break;
				default:
				    break;
			}
		    break;
        case MENU1_INDEX_COM1:
			switch(sg_DispSetMenu2_2_Index)
			{
				case MENU2_2_INDEX_ADD:
					DispCtrlParam.Addr485 =
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.Addr485,(s32*)&sg_DispSetValue,MIN_Addr,MAX_Addr);
				    break;
				case MENU2_2_INDEX_BAUD:
                    DispCtrlParam.Baud485=
                        (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.Baud485,(s32*)&sg_DispSetValue,MIN_Baud,MAX_Baud);
			    	break;
				case MENU2_2_INDEX_DATA:
					DispCtrlParam.DataFormat=
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DataFormat,(s32*)&sg_DispSetValue,MIN_DataFormat,MAX_DataFormat);
                    break;
                case MENU2_2_INDEX_COMS:
					DispCtrlParam.DataProtocol=
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DataProtocol,(s32*)&sg_DispSetValue,MIN_DataProt,MAX_DataProt);
                    break;
				default:
				    break;
			}
		    break;
		case MENU1_INDEX_SYS:
			switch(sg_DispSetMenu2_3_Index)
			{
				case MENU2_3_INDEX_CYC:
					DispCtrlParam.Cycle =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.Cycle,(s32*)&sg_DispSetValue,MIN_Cycle,MAX_Cycle);
				    break;
				case MENU2_3_INDEX_LIGH:
					DispCtrlParam.BlackTime =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.BlackTime,(s32*)&sg_DispSetValue,MIN_BlackTime,MAX_BlackTime);
				    break;
				case MENU2_3_INDEX_DISP:
					DispCtrlParam.DefaultDisp=
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DefaultDisp,(s32*)&sg_DispSetValue,MIN_Disp,MAX_Disp);
				    break;
				case MENU2_3_INDEX_ALR:
					DispCtrlParam.Alarm =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.Alarm,(s32*)&sg_DispSetValue,MIN_Alarm,MAX_Alarm);
				    break;
				case MENU2_3_INDEX_CODE:
					DispCtrlParam.Code =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.Code,(s32*)&sg_DispSetValue,MIN_Code,MAX_Code);
				    break;
				default:
				    break;
			}
		    break;

		case MENU1_INDEX_DO1:
			switch(sg_DispSetMenu2_4_Index)
			{
				case MENU2_4_INDEX_MODE:
					DispCtrlParam.DO1_Mode =
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO1_Mode,(s32*)&sg_DispSetValue,MIN_DO_Mode,MAX_DO_Mode);
				    break;
				case MENU2_4_INDEX_TIME:
					DispCtrlParam.DO1_Time =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO1_Time,(s32*)&sg_DispSetValue,MIN_DO_Time,MAX_DO_Time);
				    break;
				case MENU2_4_INDEX_ITEM:
					DispCtrlParam.DO1_Item =
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO1_Item,(s32*)&sg_DispSetValue,MIN_DO_Item,MAX_DO_Item);
				    break;
				case MENU2_4_INDEX_UAL:
					DispCtrlParam.DO1_UAL =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO1_UAL,(s32*)&sg_DispSetValue,MIN_DO_Ual,MAX_DO_Ual);
				    break;
				case MENU2_4_INDEX_HYS:
					DispCtrlParam.DO1_HYS =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO1_HYS,(s32*)&sg_DispSetValue,MIN_DO_Hys,MAX_DO_Hys);
				    break;
				case MENU2_4_INDEX_DELAY:
					DispCtrlParam.DO1_Delay=
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO1_Delay,(s32*)&sg_DispSetValue,MIN_DO_Dely,MAX_DO_Dely);
				    break;
				default:
				    break;
			}
		    break;

		case MENU1_INDEX_DO2:
			switch(sg_DispSetMenu2_5_Index)
			{
				case MENU2_5_INDEX_MODE:
					DispCtrlParam.DO2_Mode =
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO2_Mode,(s32*)&sg_DispSetValue,MIN_DO_Mode,MAX_DO_Mode);
				    break;
				case MENU2_5_INDEX_TIME:
					DispCtrlParam.DO2_Time =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO2_Time,(s32*)&sg_DispSetValue,MIN_DO_Time,MAX_DO_Time);
				    break;
				case MENU2_5_INDEX_ITEM:
					DispCtrlParam.DO2_Item =
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO2_Item,(s32*)&sg_DispSetValue,MIN_DO_Item,MAX_DO_Item);
				    break;
				case MENU2_5_INDEX_UAL:
					DispCtrlParam.DO2_UAL =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO2_UAL,(s32*)&sg_DispSetValue,MIN_DO_Ual,MAX_DO_Ual);
				    break;
				case MENU2_5_INDEX_HYS:
					DispCtrlParam.DO2_HYS =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO2_HYS,(s32*)&sg_DispSetValue,MIN_DO_Hys,MAX_DO_Hys);
				    break;
				case MENU2_5_INDEX_DELAY:
					DispCtrlParam.DO2_Delay=
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO2_Delay,(s32*)&sg_DispSetValue,MIN_DO_Dely,MAX_DO_Dely);
				    break;
				default:
				    break;
			}
		    break;

		case MENU1_INDEX_DO3:
			switch(sg_DispSetMenu2_6_Index)
			{
				case MENU2_6_INDEX_MODE:
					DispCtrlParam.DO3_Mode =
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO3_Mode,(s32*)&sg_DispSetValue,MIN_DO_Mode,MAX_DO_Mode);
				    break;
				case MENU2_6_INDEX_TIME:
					DispCtrlParam.DO3_Time =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO3_Time,(s32*)&sg_DispSetValue,MIN_DO_Time,MAX_DO_Time);
				    break;
				case MENU2_6_INDEX_ITEM:
					DispCtrlParam.DO3_Item =
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO3_Item,(s32*)&sg_DispSetValue,MIN_DO_Item,MAX_DO_Item);
				    break;
				case MENU2_6_INDEX_UAL:
					DispCtrlParam.DO3_UAL =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO3_UAL,(s32*)&sg_DispSetValue,MIN_DO_Ual,MAX_DO_Ual);
				    break;
				case MENU2_6_INDEX_HYS:
					DispCtrlParam.DO3_HYS =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO3_HYS,(s32*)&sg_DispSetValue,MIN_DO_Hys,MAX_DO_Hys);
				    break;
				case MENU2_6_INDEX_DELAY:
					DispCtrlParam.DO3_Delay=
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO3_Delay,(s32*)&sg_DispSetValue,MIN_DO_Dely,MAX_DO_Dely);
				    break;
				default:
				    break;
			}
		    break;

		case MENU1_INDEX_DO4:
			switch(sg_DispSetMenu2_7_Index)
			{
				case MENU2_7_INDEX_MODE:
					DispCtrlParam.DO4_Mode =
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO4_Mode,(s32*)&sg_DispSetValue,MIN_DO_Mode,MAX_DO_Mode);
				    break;
				case MENU2_7_INDEX_TIME:
					DispCtrlParam.DO4_Time =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO4_Time,(s32*)&sg_DispSetValue,MIN_DO_Time,MAX_DO_Time);
				    break;
				case MENU2_7_INDEX_ITEM:
					DispCtrlParam.DO4_Item =
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO4_Item,(s32*)&sg_DispSetValue,MIN_DO_Item,MAX_DO_Item);
				    break;
				case MENU2_7_INDEX_UAL:
					DispCtrlParam.DO4_UAL =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO4_UAL,(s32*)&sg_DispSetValue,MIN_DO_Ual,MAX_DO_Ual);
				    break;
				case MENU2_7_INDEX_HYS:
					DispCtrlParam.DO4_HYS =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO4_HYS,(s32*)&sg_DispSetValue,MIN_DO_Hys,MAX_DO_Hys);
				    break;
				case MENU2_7_INDEX_DELAY:
					DispCtrlParam.DO4_Delay=
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.DO4_Delay,(s32*)&sg_DispSetValue,MIN_DO_Dely,MAX_DO_Dely);
				    break;
				default:
				    break;
			}
		    break;

        case MENU1_INDEX_AO1:
			switch(sg_DispSetMenu2_8_Index)
			{
				case MENU2_8_INDEX_MODE:
					DispCtrlParam.AO1_Mode =
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.AO1_Mode,(s32*)&sg_DispSetValue,MIN_AO_Mode,MAX_AO_Mode);
				    break;
				case MENU2_8_INDEX_ITEM:
					DispCtrlParam.AO1_Item =
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.AO1_Item,(s32*)&sg_DispSetValue,MIN_AO_Item,MAX_AO_Item);
				    break;
				case MENU2_8_INDEX_DS:
					DispCtrlParam.AO1_DS =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.AO1_DS,(s32*)&sg_DispSetValue,MIN_AO_DS,MAX_AO_DS);
				    break;
				case MENU2_8_INDEX_FS:
					DispCtrlParam.AO1_FS =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.AO1_FS,(s32*)&sg_DispSetValue,MIN_AO_FS,MAX_AO_FS);
				    break;
				default:
				    break;
			}
		    break;

		case MENU1_INDEX_AO2:
			switch(sg_DispSetMenu2_9_Index)
			{
				case MENU2_9_INDEX_MODE:
					DispCtrlParam.AO2_Mode =
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.AO2_Mode,(s32*)&sg_DispSetValue,MIN_AO_Mode,MAX_AO_Mode);
				    break;
				case MENU2_9_INDEX_ITEM:
					DispCtrlParam.AO2_Item =
					    (u8)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.AO2_Item,(s32*)&sg_DispSetValue,MIN_AO_Item,MAX_AO_Item);
				    break;
				case MENU2_9_INDEX_DS:
					DispCtrlParam.AO2_DS =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.AO2_DS,(s32*)&sg_DispSetValue,MIN_AO_DS,MAX_AO_DS);
				    break;
				case MENU2_9_INDEX_FS:
					DispCtrlParam.AO2_FS =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.AO2_FS,(s32*)&sg_DispSetValue,MIN_AO_FS,MAX_AO_FS);
				    break;
				default:
				    break;
			}
		    break;
		case MENU1_INDEX_VER: //只读
		    break;
		default:
		    break;
	}
	if(UpData)   //判断控制参数是否保存到flash
	{
        DispValueUpDataFlag = TRUE;
	}
	else if(DispValueUpDataFlag != TRUE)
	{
        DispValueUpDataFlag = FALSE;
	}
}

void UpdataSetValue(UPDATA_DIR Updata_Dir)  //更新数值
{
	u16 tmpData[4] = {0};
	u8 bit = 0;

	if (Disp_Focus.CurrentFocus == CURRENTFOCUS_NONE)
	{
		return;
	}
	else if (Disp_Focus.CurrentFocus == CURRENTFOCUS_0)
	{
		bit = 0;
	}
	else if (Disp_Focus.CurrentFocus == CURRENTFOCUS_1)
	{
		bit = 1;
	}
	else if (Disp_Focus.CurrentFocus == CURRENTFOCUS_2)
	{
		bit = 2;
	}
	else if (Disp_Focus.CurrentFocus == CURRENTFOCUS_3)
	{
		bit = 3;
	}

	tmpData[0] = sg_CurrentDispNumber/1000;
	tmpData[1] = sg_CurrentDispNumber%1000/100;
	tmpData[2] = sg_CurrentDispNumber%100/10;
	tmpData[3] = sg_CurrentDispNumber%10;

	if (sg_CurrentDispChar[bit] == NEG_NUM)
	{
		sg_CurrentDispChar[bit] = POS_NUM;
	}
	else if (sg_CurrentDispChar[bit] == POS_NUM)
	{
		sg_CurrentDispChar[bit] = NEG_NUM;
	}
	else
	{
    	if (Updata_Dir == UPDATA_ADD)
    	{
    		tmpData[bit] += 1;
    		if (tmpData[bit] == 10)
    		{
    			tmpData[bit] = 0;
    		}
    	}
    	else
    	{
    		tmpData[bit] -= 1;
    		if (tmpData[bit] > 9)
    		{
    			tmpData[bit] = 9;
    		}
    	}
	}
	sg_DispSetValue = (tmpData[0]*1000 + tmpData[1]*100 + tmpData[2]*10 + tmpData[3]);
	//有-号就认为是负数
	/*if (sg_CurrentDispChar[0] == NEG_NUM || sg_CurrentDispChar[1] == NEG_NUM
	|| sg_CurrentDispChar[2] == NEG_NUM || sg_CurrentDispChar[3] == NEG_NUM)
	{
		sg_DispSetValue = -sg_DispSetValue;
	}*/
}


void UpdataOrSetValue(SELECT_CATE Cate,UPDATA_DIR Updata_Dir)//更新数值 or
{
    switch(Cate)
    {
        case Select_Net:
            if(sg_DispSetValue == N34)
            {
                sg_DispSetValue = N33;
            }
            else
            {
                sg_DispSetValue = N34;
            }
            break;
        case Select_Baud:
            if(Updata_Dir == UPDATA_ADD)
            {
                sg_DispSetValue++;
                if(sg_DispSetValue > Baud192)
                {
                    sg_DispSetValue = Baud12;
                }
            }
            else
            {
                sg_DispSetValue--;
                if(sg_DispSetValue < Baud12)
                {
                    sg_DispSetValue = Baud192;
                }
            }
            break;
        case Select_Data:
            if(Updata_Dir == UPDATA_ADD)
            {
                sg_DispSetValue++;
                if(sg_DispSetValue > n82)
                {
                    sg_DispSetValue = n81;
                }
            }
            else
            {
                sg_DispSetValue--;
                if(sg_DispSetValue < n81)
                {
                    sg_DispSetValue = n82;
                }
            }
            break;
        case Select_Coms:
            if(sg_DispSetValue == rtu485)
            {
                sg_DispSetValue = calib;
            }
            else
            {
                sg_DispSetValue = rtu485;
            }
            break;
        case Select_Save:
            if(sg_DispSetValue == SaveYes)
            {
                sg_DispSetValue = SaveNO;
            }
            else
            {
                sg_DispSetValue = SaveYes;
            }
            break;
        case Select_Disp_Default:
#ifdef _LEDHarm_
            if(Updata_Dir == UPDATA_ADD)
            {
                sg_DispSetValue++;
                if(sg_DispSetValue > DISP_UINT_THD)
                {
                    sg_DispSetValue = DISP_UINT_U;
                }
            }
            else
            {
                sg_DispSetValue--;
                if(sg_DispSetValue < DISP_UINT_U)
                {
                    sg_DispSetValue = DISP_UINT_THD;
                }
            }
#else
            if(Updata_Dir == UPDATA_ADD)
            {
                sg_DispSetValue++;
                if(sg_DispSetValue > DISP_UINT_EP)
                {
                    sg_DispSetValue = DISP_UINT_U;
                }
            }
            else
            {
                sg_DispSetValue--;
                if(sg_DispSetValue < DISP_UINT_U)
                {
                    sg_DispSetValue = DISP_UINT_EP;
                }
            }
#endif
            break;
        case Select_Do_Mode:
            if(Updata_Dir == UPDATA_ADD)
            {
                sg_DispSetValue++;
                if(sg_DispSetValue > DO_MODE_REM)
                {
                    sg_DispSetValue = DO_MODE_OFF;
                }
            }
            else
            {
                sg_DispSetValue--;
                if(sg_DispSetValue < DO_MODE_OFF)
                {
                    sg_DispSetValue = DO_MODE_REM;
                }
            }
            break;
        case Select_Do_item:
            if(g_tParam.CtrlParam.NetMode == N33)
            {
                if(Updata_Dir == UPDATA_ADD)
                {
                    sg_DispSetValue++;
                    if(sg_DispSetValue > DO_ITEM_DI4_L)
                    {
                        sg_DispSetValue = DO_ITEM_Uab_H;
                    }
                }
                else
                {
                    sg_DispSetValue--;
                    if(sg_DispSetValue < DO_ITEM_Uab_H)
                    {
                        sg_DispSetValue = DO_ITEM_DI4_L;
                    }
                }
            }
            else
            {
                if(Updata_Dir == UPDATA_ADD)
                {
                    sg_DispSetValue++;
                    if(sg_DispSetValue > DO_ITEM_DI4_L)
                    {
                        sg_DispSetValue = DO_ITEM_Ua_H;
                    }
                }
                else
                {
                    sg_DispSetValue--;
                    if(sg_DispSetValue < DO_ITEM_Ua_H)
                    {
                        sg_DispSetValue = DO_ITEM_DI4_L;
                    }
                }
            }
            break;
        case Select_Ao_Mode:
            if(Updata_Dir == UPDATA_ADD)
            {
                sg_DispSetValue++;
                if(sg_DispSetValue > AO_MODE_1220)
                {
                    sg_DispSetValue = AO_MODE_OFF;
                }
            }
            else
            {
                sg_DispSetValue--;
                if(sg_DispSetValue < AO_MODE_OFF)
                {
                    sg_DispSetValue = AO_MODE_1220;
                }
            }
            break;
        case Select_Ao_item:
            if(g_tParam.CtrlParam.NetMode == N33)
            {
                if(Updata_Dir == UPDATA_ADD)
                {
                    sg_DispSetValue++;
                    if(sg_DispSetValue > AO_ITEM_F)
                    {
                        sg_DispSetValue = AO_ITEM_UAB;
                    }
                }
                else
                {
                    sg_DispSetValue--;
                    if(sg_DispSetValue < AO_ITEM_UAB)
                    {
                        sg_DispSetValue = AO_ITEM_F;
                    }
                }
            }
            else
            {
                if(Updata_Dir == UPDATA_ADD)
                {
                    sg_DispSetValue++;
                    if(sg_DispSetValue > AO_ITEM_F)
                    {
                        sg_DispSetValue = AO_ITEM_UA;
                    }
                }
                else
                {
                    sg_DispSetValue--;
                    if(sg_DispSetValue < AO_ITEM_UA)
                    {
                        sg_DispSetValue = AO_ITEM_F;
                    }
                }
            }
            break;
        default:
            break;
    }
}

#endif // #ifdef _GUI_LCD_

//===========================================================================
//  End.
//===========================================================================


