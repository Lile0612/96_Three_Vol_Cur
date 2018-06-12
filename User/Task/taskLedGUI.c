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
#ifdef _GUILED_
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
	Select_Light = 0x0A,
	Select_Reset = 0x0B,
}SELECT_CATE;

// 主菜单索引
enum emMENU_MAIN_INDEX
{
#if(FUNCTION_CURRENT == 1)
	MENU_INDEX_I	= 0x00, // 电流
	MENU_INDEX_MIN	= MENU_INDEX_I,
	MENU_INDEX_F	= 0x01, // 频率
	MENU_INDEX_DI	= 0x02,
	MENU_INDEX_DO	= 0x03,
	MENU_INDEX_MAX	= MENU_INDEX_DO,
#endif

#if(FUNCTION_VOLTAGE == 1)
    MENU_INDEX_U    = 0x00, // 电压
	MENU_INDEX_MIN	= MENU_INDEX_U,
	MENU_INDEX_F	= 0x01, // 频率
	MENU_INDEX_DI	= 0x02,
	MENU_INDEX_DO	= 0x03,
	MENU_INDEX_MAX	= MENU_INDEX_DO,
#endif
};


enum emMENU_SUBMAIN_INDEX
{
	MENUSUB_INDEX_READ	= 0x00, // 查看
	MENUSUB_INDEX_MIN	= MENUSUB_INDEX_READ,
	MENUSUB_INDEX_PROG	= 0x01, // 设置
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
	MENU1_INDEX_DO4     = 0x06, // 输出4
	MENU1_INDEX_AO1     = 0x07, // 模拟输出1
	MENU1_INDEX_AO2     = 0x08, // 模拟输出2
    MENU1_INDEX_VER     = 0x09, // 版本
	MENU1_INDEX_MAX	    = MENU1_INDEX_AO2,
	MENU1_INDEX_RMAX    = MENU1_INDEX_VER,
};

// MENU2_1菜单索引
enum emMENU_SETMENU2_1_INDEX
{
#if(FUNCTION_CURRENT == 1)
	MENU2_1_INDEX_CT    = 0x00, // CT
	MENU2_1_INDEX_MIN   = MENU2_1_INDEX_CT,
	MENU2_1_INDEX_MAX	= MENU2_1_INDEX_CT,
#endif

#if(FUNCTION_VOLTAGE == 1)
	MENU2_1_INDEX_PT	= 0x00, // PT
	MENU2_1_INDEX_MIN   = MENU2_1_INDEX_PT,
	MENU2_1_INDEX_MAX	= MENU2_1_INDEX_PT,
#endif
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
    MENU2_3_INDEX_LIGH	= 0x00, // 亮度等级
	MENU2_3_INDEX_MIN	= MENU2_3_INDEX_LIGH,
	MENU2_3_INDEX_CODE	= 0x01, // 编程密码
	MENU2_3_INDEX_RESET	= 0x02,
	MENU2_3_INDEX_MAX	= MENU2_3_INDEX_RESET,
	MENU2_3_INDEX_RMAX  = MENU2_3_INDEX_MIN,
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
void DispMenu_MenuAdjust(void);  // 校准界面


// 与变量Key_Tab行对应
#define KEYFUNC_MAIN        0 // 显示主菜单
#define KEYFUNC_MENU1	    1 // 选择模式
#define KEYFUNC_MENU2	    2 // 设置菜单二级
#define KEYFUNC_MENU3		3 // 设置菜单三级
#define KEYFUNC_MENUVALUE	4 // 设置菜单值
#define KEYFUNC_MENUCODE	5 // 密码输入界面
#define KEYFUNC_MENUCODEERR	6 // 密码输入错误界面
#define KEYFUNC_MENUSAVE	7 // 退出保存界面
#define KEYFUNC_ADJUST      8 // 校准界面


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
    { 8, 8, 8, 8, 8,(*DispMenu_MenuAdjust)}, // 校准界面

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
static u8 sg_DispSetMenu2_3_Index = MENU2_3_INDEX_MIN;		// 要显示的2-3菜单索引号
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
static BOOL sg_b2sFlashEnable = TRUE;  // 闪烁使能
static BOOL g_bInitDefault = FALSE;		// 上电后显示默认
static BOOL g_bUpdataDefault = FALSE;	// 显示默认
static BOOL g_bBlackLightDefault = FALSE;  // 背光亮
BOOL g_fftEnable = FALSE;        //
BOOL g_AoOutEnable = FALSE;        //

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
static BOOL sg_AoOutDely = TRUE;        // 模拟量输出启动延时

static u32 s_SysStartTick = 0;
static BOOL s_bSysStartEnable = FALSE;
static u16 g_fftTick = 0;
static u32 g_BlackTick = 0;
static u16 g_CycleTick = 0;
static u32 g_AoOutDelyTick = 0;

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
/**
  * @brief  GUI初始化.
  * @note
  * @param  None
  * @retval None
  */
void Task_GUI_init(void)
{
    for(u8 i = 0;i < 13;i++)
    {
        vg_Display_Value[i] = 0xff;
    }
    Update_screen();
    PT_TimerDelay(2000);
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
	static u32 s_2sFlashNum = 0;
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
		if (g_fftTick++ >= TIMER_DELAY_1S)
		{
			g_fftTick = 0;
			g_fftEnable = FALSE; 
		}
	}
	else
	{
		g_fftTick = 0;
	}

    /**************************其他界面120S后显示默认界面************/
	if (g_bRestoreDlyEnable && (g_tParam.CtrlParam.Cycle != CYCYES))
	{
		TimerDlyNum = TIMER_DELAY_300S;         // 延时120秒
		//TimerDlyNum = TIMER_DELAY_30M;
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
	/**************************开机5S_Ao延时****************************/
	if (sg_AoOutDely)
	{
		if (g_AoOutDelyTick ++ >= TIMER_DELAY_5S)
		{
			g_AoOutDelyTick = 0;
			g_AoOutEnable = TRUE;
			sg_AoOutDely = FALSE;
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
	/**************************闪烁标识******************************/
	s_2sFlashNum++;
	if (s_2sFlashNum >= TIMER_DELAY_2S)
	{
		s_2sFlashNum = 0;
		sg_b2sFlashEnable = !sg_b2sFlashEnable;
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
    		sg_DispSetMenu2_1_Index = MENU2_1_INDEX_MIN;
    		sg_DispSetMenu2_2_Index = MENU2_2_INDEX_ADD;
    		sg_DispSetMenu2_3_Index = MENU2_3_INDEX_MIN;
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
    		sg_DispSetMenu2_1_Index = MENU2_1_INDEX_MIN;
    		sg_DispSetMenu2_2_Index = MENU2_2_INDEX_ADD;
    		sg_DispSetMenu2_3_Index = MENU2_3_INDEX_MIN;
    		sg_DispSetMenu2_4_Index = MENU2_4_INDEX_MODE;
            sg_DispSetMenu2_5_Index = MENU2_5_INDEX_MODE;
    		sg_DispSetMenu2_6_Index = MENU2_6_INDEX_MODE;
    		sg_DispSetMenu2_7_Index = MENU2_7_INDEX_MODE;
    		sg_DispSetMenu2_8_Index = MENU2_8_INDEX_MODE;
    		sg_DispSetMenu2_9_Index = MENU2_9_INDEX_MODE;
    		break;
    	case KEYFUNC_MENUVALUE:
    	  	KeyFuncIndex = KEYFUNC_MENU3;
    	  	LEDLEVEL = DispCtrlParam.BlackTime;
    		break;
		case KEYFUNC_MENUCODE:
			KeyFuncIndex = KEYFUNC_MENU1;
			sg_DispSubMainMenuIndex = MENUSUB_INDEX_PROG;
			sg_DispSetMenu1Index = MENU1_INDEX_INPT;
    		sg_DispSetMenu2_1_Index = MENU2_1_INDEX_MIN;
    		sg_DispSetMenu2_2_Index = MENU2_2_INDEX_ADD;
    		sg_DispSetMenu2_3_Index = MENU2_3_INDEX_MIN;
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
                LEDLEVEL = DispCtrlParam.BlackTime;
			    sg_DispSetValue = 0x00;
		    }
		    else if( sg_DispSubMainMenuIndex == MENUSUB_INDEX_READ)
		    {
		        DispMode = ReadMode;  // 显示模式标志位
                KeyFuncIndex = KEYFUNC_MENU2;
		    }
			break;
		case KEYFUNC_MENU2:
		    if((sg_DispSetMenu1Index == MENU1_INDEX_VER )&& (DispMode == ReadMode))
		    {
                break;
		    }
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
		        if((sg_DispSetValue == SaveYes) && (sg_DispSetMenu2_3_Index ==  MENU2_3_INDEX_RESET))
		        {
                    g_tParam.CtrlParam.AdjustFinishFlag = FALSE;
                    InitRNParam();
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
                    NVIC_SystemReset();
                }
                g_tParam.CtrlParam = DispCtrlParam;
                SaveParam();
                DispValueUpDataFlag = FALSE;
            }
            g_bUpdataDefault = TRUE;
            DispMode = ReadMode;
		    break;
		case KEYFUNC_ADJUST:
		    SaveParam();
		    g_bUpdataDefault = TRUE;
            DispMode = ReadMode;
            break;
		default:
            g_bUpdataDefault = TRUE;
		    break;
		}
	KeyFuncPtr=Key_Tab[KeyFuncIndex].CurrentOperate;
}


void GUI_Key_MenuLong(void)  //菜单键长按 只有在主菜单模式下有用 进入模式选择
{
	if (KeyFuncIndex == KEYFUNC_MAIN)
	{
			KeyFuncIndex = KEYFUNC_MENU1;
	}
	else if(KeyFuncIndex == KEYFUNC_MENU1)
	{
        KeyFuncIndex = KEYFUNC_ADJUST;
	}
	KeyFuncPtr=Key_Tab[KeyFuncIndex].CurrentOperate;
}


void GUI_Key_Up(void)       //左按键 实质:减去
{
    switch (KeyFuncIndex)
	{
		case KEYFUNC_MAIN:      //在主菜单
            sg_DispMainMenuIndex--;
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
						if(Disp_Focus.CurrentFocus == CURRENTFOCUS_0)
						{
							Disp_Focus.CurrentFocus = CURRENTFOCUS_3;
						}
						else
						{
							Disp_Focus.CurrentFocus = Disp_Focus.CurrentFocus >> 1;
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
                            case MENU2_3_INDEX_LIGH:
                                UpdataOrSetValue(Select_Light,UPDATA_SUB);
                                break;
                            case MENU2_3_INDEX_RESET:
                                UpdataOrSetValue(Select_Reset,UPDATA_SUB);
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
	KeyFuncPtr=Key_Tab[KeyFuncIndex].CurrentOperate;
}

void GUI_Key_Down(void)  //右按键
{
    switch (KeyFuncIndex)
	{
		case KEYFUNC_MAIN:      //在主菜单 电流/频率
			sg_DispMainMenuIndex++;
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
#if(FUNCTION_VOLTAGE == 1)
                            case MENU2_1_INDEX_PT:
                                UpdataSetValue(UPDATA_ADD);
                                break;
#endif
#if(FUNCTION_CURRENT == 1)
                            case MENU2_1_INDEX_CT:
                                UpdataSetValue(UPDATA_ADD);
                                break;
#endif
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
                            case MENU2_3_INDEX_LIGH:
                                UpdataOrSetValue(Select_Light,UPDATA_ADD);
                                break;
                            case MENU2_3_INDEX_RESET:
                                UpdataOrSetValue(Select_Reset,UPDATA_ADD);
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
	KeyFuncPtr=Key_Tab[KeyFuncIndex].CurrentOperate;
}


void GUI_Key_Default(u8 Key)
{
    // 默认电流界面
	if ((Key == KEYSTATUS_NONE && g_bInitDefault) || g_bUpdataDefault)
	{
		g_bInitDefault = FALSE;
        KeyFuncIndex = KEYFUNC_MAIN;
#if(FUNCTION_VOLTAGE == 1)
        sg_DispMainMenuIndex = MENU_INDEX_U;
#endif
#if(FUNCTION_CURRENT == 1)
        sg_DispMainMenuIndex = MENU_INDEX_I;
#endif
		sg_DispSetMenu1Index = MENU1_INDEX_INPT;
		sg_DispSetMenu2_1_Index = MENU2_1_INDEX_MIN;
		sg_DispSetMenu2_2_Index = MENU2_2_INDEX_ADD;
		sg_DispSetMenu2_3_Index = MENU2_3_INDEX_MIN;
		sg_DispSetMenu2_4_Index = MENU2_4_INDEX_MODE;
		sg_DispSetMenu2_5_Index = MENU2_5_INDEX_MODE;
		sg_DispSetMenu2_6_Index = MENU2_6_INDEX_MODE;
		sg_DispSetMenu2_7_Index = MENU2_7_INDEX_MODE;
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
    Update_screen();
}

void Disp_Set_FU(u8 Index, u8 *pFuData)
{
    *pFuData = CHARMODE_FU;
    sg_CurrentDispChar[Index] = NEG_NUM;
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
            Decimal = 3;
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
            Decimal = 3;
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
            Decimal = 3;
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
            Decimal = 3;
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
            Decimal = 3;
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
            Decimal = 3;
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
  * @brief	显示数字
  * @param	s16 DispNum-要显示的数字
			u8 Decimal-小数点位数
			u8 DispWidth-显示宽度(几位数)
			BOOL bFlash-是否闪烁
			u8 FlashDec-闪烁的位数
			BOOL bEnableDisp-强制显示全部位
  * @retval None
  */
void DispValue(u8 num,s32 DispNum, u8 Decimal, u8 DispWidth, BOOL bFlash, u8 FlashDec, BOOL bEnableDisp, BOOL bSet)
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
    	temp8[i] =HT_Data_Index( BitChar[i]);
	}

	if (Decimal == 3)  //小数点
	{

		if (DispNum < 0)
		{
		    Disp_Set_FU(0, &temp8[0]);
		}
		temp8[0] |= CHARMODE_DOT;
	}
	else if (Decimal == 2)
	{
		if (DispNum < 0)
		{
		    Disp_Set_FU(0, &temp8[0]);
		}
		else
		{
			if (BitChar[0] == 0)
			{
				temp8[0] = CHARMODE_NC;
			}
		}
		temp8[1] |= CHARMODE_DOT;
	}
	else if (Decimal == 1)
	{

		if (DispNum < 0)
		{
			if (BitChar[0] == 0)
			{
				if (BitChar[1] > 0)
				{
		            Disp_Set_FU(0, &temp8[0]);
				}
				else
				{
		            Disp_Set_FU(1, &temp8[1]);
                    temp8[0] = CHARMODE_NC;
				}
				temp8[2] |= CHARMODE_DOT;
			}
			else
			{
                temp8[3] = temp8[2];
			    temp8[2] = temp8[1];
			    temp8[1] = temp8[0];
				temp8[0] = CHARMODE_FU;
				//temp8[2] |= CHARMODE_DOT;
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
			temp8[2] |= CHARMODE_DOT;
		}

	}
	else if (Decimal == 0)
	{
        if (DispNum < 0 && DispNum > -10)
 		{
		    Disp_Set_FU(2, &temp8[2]);
 		}
 		else if (DispNum <= -10 && DispNum > -100)
 		{
		    Disp_Set_FU(1, &temp8[1]);
 		}
 		else if (DispNum <= -100 && DispNum > -1000)
 		{
		    Disp_Set_FU(0, &temp8[0]);
 		}
	}
	else
	{
		return;
	}
	switch (DispWidth)
	{
    	case 1:
    		temp8[0] = CHARMODE_NC;
    		temp8[1] = CHARMODE_NC;
    		temp8[2] = CHARMODE_NC;
    	    break;
    	case 2:
    		temp8[0] = CHARMODE_NC;
    		temp8[1] = CHARMODE_NC;
    	    break;
    	case 3:
    		temp8[0] = CHARMODE_NC;
    	    break;
	}

	if (bEnableDisp)
	{
		if (temp8[0] == CHARMODE_NC)
		{
			temp8[0] = CHARMODE_0;
		}
		if (temp8[1] == CHARMODE_NC)
		{
			temp8[1] = CHARMODE_0;
		}
		if (temp8[2] == CHARMODE_NC)
		{
			temp8[2] = CHARMODE_0;
		}
		if (temp8[3] == CHARMODE_NC)
		{
			temp8[3] = CHARMODE_0;
		}
	}

	if (bSet) // 进入设置状态0
	{
		for (i = 0; i < 4; i++)
		{
			if (sg_CurrentDispChar[i] == POS_NUM)
			{
				temp8[i] = CHARMODE_ZHENG;
			}
			else if (sg_CurrentDispChar[i] == NEG_NUM)
			{
				temp8[i] = CHARMODE_FU;
			}
		}
	}
	if (bFlash)
	{
		for (i = 0; i < 4; i++)
		{
    		if (Get_Bit(FlashDec, i) == 1)
    		{
    			temp8[i] = CHARMODE_NC;
    		}
		}
	}
	DispChar(num,temp8[0], temp8[1], temp8[2], temp8[3]);

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
        case 3:
            vg_Display_Value[12] = ch2;    //单位
            break;
		default:
		    break;
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
    u8 tmpInCharmode[4];
    u8 tmpOutCharmode[4];
    u32 DispValTmp[3];
    Leve_1_DispNUM = 0x00;
    switch (sg_DispMainMenuIndex)
    {
#if(FUNCTION_VOLTAGE == 1)
		
        case MENU_INDEX_U: // 单位V 1位小数点 三相电压 
		    for(u8 i = 0; i<3; i++)
    		    {
    		        DispValTmp[i] = vg_Power_Val.Real2_Val[i].U;
    		        if(DispValTmp[i] < 100000)     //123.4
                    {
                        Leve_1_DispNUM |= 0x01;
                    }                                           //6                                   //7
                    if((DispValTmp[i] >= 100000) && (DispValTmp[i] < 1000000)) //1.234 K
                    {
                        Leve_1_DispNUM |= 0x02;
                    }                                           //7                                    //8
                    else if((DispValTmp[i] >= 1000000) && (DispValTmp[i] < 10000000))//12.34 K
                    {
                        Leve_1_DispNUM |= 0x04;
                    }                                           //8                                    //9
                    else if((DispValTmp[i] >= 10000000) && (DispValTmp[i] < 100000000))//123.4 K
                    {
                        Leve_1_DispNUM |= 0x08;
                    }                                           //9                                    //10
                    else if((DispValTmp[i] >= 100000000) && (DispValTmp[i] < 1000000000))//1.234 M
                    {
                        Leve_1_DispNUM |= 0x10;
                    }
    		    }
    		    for(u8 j = 0; j<3; j++)
    		    {
                    DispValueVoltage(j,DispValTmp[j]*0.1, 0, 4, FALSE,0, TRUE, Leve_1_DispNUM);
    		    }
    		    if((Leve_1_DispNUM > 0x01) && Leve_1_DispNUM < 0x10)
    		    {
                    UintFlag = 0;
                    UintFlag |= CHARMODE_DK;
    		        DispChar(3,CHARMODE_NC,UintFlag,CHARMODE_NC,CHARMODE_NC);
    		    }
    			break;
#endif
#if(FUNCTION_CURRENT == 1)
		case MENU_INDEX_I://单位A 3位小数点
            for(u8 i = 0; i<3; i++)
            {
                DispValTmp[i] = vg_Power_Val.Real2_Val[i].I;
                if(DispValTmp[i] < 100000)     //1.234
                {
                    Leve_1_DispNUM |= 0x01;
                }                                           //6                                   //7
                if((DispValTmp[i] >= 100000) && (DispValTmp[i] < 1000000)) //12.34
                {
                    Leve_1_DispNUM |= 0x02;
                }                                           //7                                    //8
                else if((DispValTmp[i] >= 1000000) && (DispValTmp[i] < 10000000))//123.4
                {
                    Leve_1_DispNUM |= 0x04;
                }                                           //8                                    //9
                else if((DispValTmp[i] >= 10000000) && (DispValTmp[i] < 100000000))//1.234 K
                {
                    Leve_1_DispNUM |= 0x08;
                }                                           //9                                    //10
                else if((DispValTmp[i] >= 100000000) && (DispValTmp[i] < 1000000000))//12.34 K
                {
                    Leve_1_DispNUM |= 0x10;
                }
            }
            for(u8 j = 0; j<3; j++)
            {
                DispValueCurrent(j,DispValTmp[j]*0.1, 0, 4, FALSE,0, TRUE, Leve_1_DispNUM);
            }
            if( Leve_1_DispNUM & 0x20 || Leve_1_DispNUM & 0x10 || Leve_1_DispNUM & 0x08)
            {
                DispChar(3,CHARMODE_NC,CHARMODE_DK,CHARMODE_NC,CHARMODE_NC);
            }
            break;
#endif
        case MENU_INDEX_F:
            DispChar(0,CHARMODE_F,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
            if(vg_Power_Val.Freq == 0)
            {
                DispChar(1,CHARMODE_FU,CHARMODE_FU,CHARMODE_FU,CHARMODE_FU);
            }
            else
            {
                DispValue(1,vg_Power_Val.Freq, 2, 4, FALSE, 0, FALSE, FALSE);
            }
            DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_H,CHARMODE_2);
            break;
        case MENU_INDEX_DI:
            for(u8 j = 0;j<DIN_NUM;j++)
            {
                if(g_Din_Status[j])
                    tmpInCharmode[j] = CHARMODE_0;
                else
                    tmpInCharmode[j] = CHARMODE_1;
            }
            DispChar(0,CHARMODE_D,CHARMODE_I,CHARMODE_NC,CHARMODE_NC);
            DispChar(1,tmpInCharmode[3],tmpInCharmode[2],tmpInCharmode[1],tmpInCharmode[0]);
            break;
        case MENU_INDEX_DO:
            for(u8 k = 0;k<OUT_NUM;k++)
            {
                if(g_Out_Status[k])
                    tmpOutCharmode[k] = CHARMODE_1;
                else
                    tmpOutCharmode[k] = CHARMODE_0;
            }
            DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_NC,CHARMODE_NC);
            DispChar(1,tmpOutCharmode[3],tmpOutCharmode[2],tmpOutCharmode[1],tmpOutCharmode[0]);
            break;
        /*=================================================================================================*/
        /*=================================================================================================*/
		default :
			break;

	}
}


void DispMenu_Menu1(void) //一级界面(备用)
{
    switch (sg_DispSubMainMenuIndex)
	{
		case MENUSUB_INDEX_READ:
			DispChar(0,CHARMODE_R,CHARMODE_E,CHARMODE_A,CHARMODE_D);
			break;
		case MENUSUB_INDEX_PROG:
		    DispChar(0,CHARMODE_P,CHARMODE_R,CHARMODE_O,CHARMODE_G);
            break;
		default :
			break;
	}
}

void DispMenu_Menu2(void) //二级界面 设置选项(4)
{
	switch(sg_DispSetMenu1Index)
	{
		case MENU1_INDEX_INPT:
			DispChar(0,CHARMODE_I, CHARMODE_N,CHARMODE_P,CHARMODE_T);
		    break;
		case MENU1_INDEX_COM1:
			DispChar(0,CHARMODE_NC,CHARMODE_C,CHARMODE_O,CHARMODE_M);
		    break;
		case MENU1_INDEX_SYS:
			DispChar(0,CHARMODE_NC,CHARMODE_S,CHARMODE_Y,CHARMODE_S);
		    break;
		case MENU1_INDEX_DO1:
			DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_1);
		    break;
		case MENU1_INDEX_DO2:
			DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_2);
		    break;
		case MENU1_INDEX_DO3:
			DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_3);
		    break;
		case MENU1_INDEX_DO4:
			DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_4);
		    break;
		case MENU1_INDEX_AO1:
			DispChar(0,CHARMODE_A,CHARMODE_O,CHARMODE_FU,CHARMODE_1);
		    break;
		case MENU1_INDEX_AO2:
			DispChar(0,CHARMODE_A,CHARMODE_O,CHARMODE_FU,CHARMODE_2);
		    break;
		case MENU1_INDEX_VER:
			DispChar(0,CHARMODE_NC,CHARMODE_U,CHARMODE_E,CHARMODE_R);
			DispChar(2,CHARMODE_1,CHARMODE_7,CHARMODE_1,CHARMODE_2);
		    break;
		default:
		    break;
	}
}


void DispMenu_Menu3(void) //三级界面
{
    if(DispMode == ProgMode)
    {
    	switch(sg_DispSetMenu1Index)
    	{
    		case MENU1_INDEX_INPT:
    			DispChar(0,CHARMODE_I,CHARMODE_N,CHARMODE_P,CHARMODE_T);
                switch(sg_DispSetMenu2_1_Index)
                {
#if(FUNCTION_CURRENT == 1)
                    case MENU2_1_INDEX_CT:
                        DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_C,CHARMODE_T);
                        break;
#endif
#if(FUNCTION_VOLTAGE == 1)
                    case MENU2_1_INDEX_PT:
                        DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_P,CHARMODE_T);
                        break;
#endif
                    default:
                        break;
                }
                break;
            case MENU1_INDEX_COM1:
                DispChar(0,CHARMODE_NC,CHARMODE_C,CHARMODE_O,CHARMODE_M);
    			switch(sg_DispSetMenu2_2_Index)
    			{
    				case MENU2_2_INDEX_ADD:
                        DispChar(1,CHARMODE_NC,CHARMODE_A,CHARMODE_D,CHARMODE_D);
    				    break;
    				case MENU2_2_INDEX_BAUD:
    					DispChar(1,CHARMODE_B,CHARMODE_A,CHARMODE_U,CHARMODE_D);
    			    	break;
    				case MENU2_2_INDEX_DATA:
    					DispChar(1,CHARMODE_D,CHARMODE_A,CHARMODE_T,CHARMODE_A);
    				    break;
    				case MENU2_2_INDEX_COMS:
    					DispChar(1,CHARMODE_C,CHARMODE_O,CHARMODE_M,CHARMODE_S);
    				    break;
    				default:
    				    break;
    			}
    		    break;
            case MENU1_INDEX_SYS:
                DispChar(0,CHARMODE_NC,CHARMODE_S,CHARMODE_Y,CHARMODE_S);
    			switch(sg_DispSetMenu2_3_Index)
    			{
    				case MENU2_3_INDEX_LIGH:
    					DispChar(1,CHARMODE_L,CHARMODE_I,CHARMODE_G,CHARMODE_H);
    				    break;
    				case MENU2_3_INDEX_CODE:
    					DispChar(1,CHARMODE_C,CHARMODE_O,CHARMODE_D,CHARMODE_E);
    				    break;
    				case MENU2_3_INDEX_RESET:
    				    DispChar(1,CHARMODE_C,CHARMODE_L,CHARMODE_E,CHARMODE_A);
    				default:
    				    break;
    			}
    		    break;
    		case MENU1_INDEX_DO1:
                DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_1);
    			switch(sg_DispSetMenu2_4_Index)
    			{
    				case MENU2_4_INDEX_MODE:
                        DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
    				    break;
    				case MENU2_4_INDEX_TIME:
    					DispChar(1,CHARMODE_T,CHARMODE_I,CHARMODE_M,CHARMODE_E);
    			    	break;
    				case MENU2_4_INDEX_ITEM:
    					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);
    			    	break;
    				case MENU2_4_INDEX_UAL:
    					DispChar(1,CHARMODE_NC,CHARMODE_U,CHARMODE_A,CHARMODE_L);
    				    break;
    				case MENU2_4_INDEX_HYS:
    					DispChar(1,CHARMODE_NC,CHARMODE_H,CHARMODE_Y,CHARMODE_S);
    				    break;
    				case MENU2_4_INDEX_DELAY:
    					DispChar(1,CHARMODE_D,CHARMODE_E,CHARMODE_L,CHARMODE_Y);
    				    break;
    				default:
    				    break;
    			}
    		    break;
    		case MENU1_INDEX_DO2:
                DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_2);
    			switch(sg_DispSetMenu2_5_Index)
    			{
    				case MENU2_5_INDEX_MODE:
                        DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
    				    break;
    				case MENU2_5_INDEX_TIME:
    					DispChar(1,CHARMODE_T,CHARMODE_I,CHARMODE_M,CHARMODE_E);
    			    	break;
    				case MENU2_5_INDEX_ITEM:
    					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);
    			    	break;
    				case MENU2_5_INDEX_UAL:
    					DispChar(1,CHARMODE_NC,CHARMODE_U,CHARMODE_A,CHARMODE_L);
    				    break;
    				case MENU2_5_INDEX_HYS:
    					DispChar(1,CHARMODE_NC,CHARMODE_H,CHARMODE_Y,CHARMODE_S);
    				    break;
    				case MENU2_5_INDEX_DELAY:
    					DispChar(1,CHARMODE_D,CHARMODE_E,CHARMODE_L,CHARMODE_Y);
    				    break;
    				default:
    				    break;
    			}
    		    break;
    		case MENU1_INDEX_DO3:
                DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_3);
    			switch(sg_DispSetMenu2_6_Index)
    			{
    				case MENU2_6_INDEX_MODE:
                        DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
    				    break;
    				case MENU2_6_INDEX_TIME:
    					DispChar(1,CHARMODE_T,CHARMODE_I,CHARMODE_M,CHARMODE_E);
    			    	break;
    				case MENU2_6_INDEX_ITEM:
    					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);
    			    	break;
    				case MENU2_6_INDEX_UAL:
    					DispChar(1,CHARMODE_NC,CHARMODE_U,CHARMODE_A,CHARMODE_L);
    				    break;
    				case MENU2_6_INDEX_HYS:
    					DispChar(1,CHARMODE_NC,CHARMODE_H,CHARMODE_Y,CHARMODE_S);
    				    break;
    				case MENU2_6_INDEX_DELAY:
    					DispChar(1,CHARMODE_D,CHARMODE_E,CHARMODE_L,CHARMODE_Y);
    				    break;
    				default:
    				    break;
    			}
    		    break;
    		case MENU1_INDEX_DO4:
                DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_4);
    			switch(sg_DispSetMenu2_7_Index)
    			{
    				case MENU2_7_INDEX_MODE:
                        DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
    				    break;
    				case MENU2_7_INDEX_TIME:
    					DispChar(1,CHARMODE_T,CHARMODE_I,CHARMODE_M,CHARMODE_E);
    			    	break;
    				case MENU2_7_INDEX_ITEM:
    					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);
    			    	break;
    				case MENU2_7_INDEX_UAL:
    					DispChar(1,CHARMODE_NC,CHARMODE_U,CHARMODE_A,CHARMODE_L);
    				    break;
    				case MENU2_7_INDEX_HYS:
    					DispChar(1,CHARMODE_NC,CHARMODE_H,CHARMODE_Y,CHARMODE_S);
    				    break;
    				case MENU2_7_INDEX_DELAY:
    					DispChar(1,CHARMODE_D,CHARMODE_E,CHARMODE_L,CHARMODE_Y);
    				    break;
    				default:
    				    break;
    			}
    		    break;
    		case MENU1_INDEX_AO1:
                DispChar(0,CHARMODE_A,CHARMODE_O,CHARMODE_FU,CHARMODE_1);
    			switch(sg_DispSetMenu2_8_Index)
    			{
    				case MENU2_8_INDEX_MODE:
                        DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
    				    break;
    				case MENU2_8_INDEX_ITEM:
    					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);
    			    	break;
    				case MENU2_8_INDEX_DS:
    					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_D,CHARMODE_S);
    				    break;
    				case MENU2_8_INDEX_FS:
    					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_F,CHARMODE_S);
    				    break;
    				default:
    				    break;
    			}
    		    break;
    		case MENU1_INDEX_AO2:
                DispChar(0,CHARMODE_A,CHARMODE_O,CHARMODE_FU,CHARMODE_2);
    			switch(sg_DispSetMenu2_9_Index)
    			{
    				case MENU2_9_INDEX_MODE:
                        DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
    				    break;
    				case MENU2_9_INDEX_ITEM:
    					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);
    			    	break;
    				case MENU2_9_INDEX_DS:
    					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_D,CHARMODE_S);
    				    break;
    				case MENU2_9_INDEX_FS:
    					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_F,CHARMODE_S);
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
        sg_bSetFlashEnable = FALSE;
        switch(sg_DispSetMenu1Index)
    	{
    		case MENU1_INDEX_INPT:
			DispChar(0,CHARMODE_I,CHARMODE_N,CHARMODE_P,CHARMODE_T);
			switch(sg_DispSetMenu2_1_Index)
			{
#if(FUNCTION_VOLTAGE == 1)
				case MENU2_1_INDEX_PT:
					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_P,CHARMODE_T);

					Decimal =  0;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal, Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
#endif
#if(FUNCTION_CURRENT == 1)
				case MENU2_1_INDEX_CT:
					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_C,CHARMODE_T);

					Decimal = 0;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
#endif
				default:
				    break;
			}
		    break;
	    case MENU1_INDEX_COM1:
	        DispChar(0,CHARMODE_NC,CHARMODE_C,CHARMODE_O,CHARMODE_M);
			switch(sg_DispSetMenu2_2_Index)
			{
				case MENU2_2_INDEX_ADD:
					DispChar(1,CHARMODE_NC,CHARMODE_A,CHARMODE_D,CHARMODE_D);
					Decimal =  0;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				case MENU2_2_INDEX_BAUD:
					DispChar(1,CHARMODE_B,CHARMODE_A,CHARMODE_U,CHARMODE_D);
					if(sg_bSetFlashEnable)
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
					else
					{
                        switch(sg_DispSetValue)
    					{
                            case Baud12:
                                DispChar(2,CHARMODE_1,CHARMODE_2,CHARMODE_0,CHARMODE_0);
                                break;
                            case Baud24:
                                DispChar(2,CHARMODE_2,CHARMODE_4,CHARMODE_0,CHARMODE_0);
                                break;
                            case Baud48:
                                DispChar(2,CHARMODE_4,CHARMODE_8,CHARMODE_0,CHARMODE_0);
                                break;
                            case Baud96:
                                DispChar(2,CHARMODE_9,CHARMODE_6,CHARMODE_0,CHARMODE_0);
                                break;
                            case Baud192:
                                DispChar(2,CHARMODE_1,CHARMODE_9+CHARMODE_DOT,CHARMODE_2,CHARMODE_0);
                                break;
                            default:
                                break;
    					}
					}
			    	break;
				case MENU2_2_INDEX_DATA:
					DispChar(1,CHARMODE_D,CHARMODE_A,CHARMODE_T,CHARMODE_A);
				    if(sg_bSetFlashEnable)
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
					else
					{
                        switch(sg_DispSetValue)
    					{
                            case n81:
                                DispChar(2,CHARMODE_NC,CHARMODE_N,CHARMODE_8,CHARMODE_1);
                                break;
                            case o81:
                                DispChar(2,CHARMODE_NC,CHARMODE_O,CHARMODE_8,CHARMODE_1);
                                break;
                            case e81:
                                DispChar(2,CHARMODE_NC,CHARMODE_E,CHARMODE_8,CHARMODE_1);
                                break;
                            case n82:
                                DispChar(2,CHARMODE_NC,CHARMODE_N,CHARMODE_8,CHARMODE_2);
                                break;
                            default:
                                break;
    					}
					}
			    	break;
				case MENU2_2_INDEX_COMS:
					DispChar(1,CHARMODE_C,CHARMODE_O,CHARMODE_M,CHARMODE_S);
					if(sg_DispSetValue == rtu485)
					{
                        if(!sg_bSetFlashEnable)
                			DispChar(2,CHARMODE_NC,CHARMODE_R,CHARMODE_T,CHARMODE_U);
                		else
                			DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
					else if( sg_DispSetValue == calib)
					{
                        if(!sg_bSetFlashEnable)
                			DispChar(2,CHARMODE_NC,CHARMODE_C,CHARMODE_A,CHARMODE_L);
                		else
                			DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				default:
				    break;
			}
		    break;
        case MENU1_INDEX_SYS:
            DispChar(0,CHARMODE_NC,CHARMODE_S,CHARMODE_Y,CHARMODE_S);
            switch(sg_DispSetMenu2_3_Index)
			{
				case MENU2_3_INDEX_LIGH:        // 亮度等级
					DispChar(1,CHARMODE_L,CHARMODE_I,CHARMODE_G,CHARMODE_H);
					if(!sg_bSetFlashEnable)
					{
                        switch(sg_DispSetValue)
                        {
                            case BrightLevel_1:
                                DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_L,CHARMODE_1);
                                break;
                            case BrightLevel_2:
                                DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_L,CHARMODE_2);
                                break;
                            case BrightLevel_3:
                                DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_L,CHARMODE_3);
                                break;
                            default:
                                break;
                        }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				default:
				    break;
            }
		    break;
		case MENU1_INDEX_DO1:
            DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_1);
            switch(sg_DispSetMenu2_4_Index)
			{
                case MENU2_4_INDEX_MODE:
                    DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
                    if(!sg_bSetFlashEnable)
					{
                        if(sg_DispSetValue == DO_MODE_OFF)
                            DispChar(2,CHARMODE_NC,CHARMODE_O,CHARMODE_F,CHARMODE_F);
                        else if(sg_DispSetValue == DO_MODE_ALR)
                            DispChar(2,CHARMODE_NC,CHARMODE_A,CHARMODE_L,CHARMODE_R);
                        else if(sg_DispSetValue == DO_MODE_REM)
                            DispChar(2,CHARMODE_NC,CHARMODE_R,CHARMODE_E,CHARMODE_M);
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				case MENU2_4_INDEX_TIME:
					DispChar(1,CHARMODE_T,CHARMODE_I,CHARMODE_M,CHARMODE_E);

                    Decimal =  2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
			    	break;
				case MENU2_4_INDEX_ITEM:
					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);

					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
#if(FUNCTION_VOLTAGE == 1)
							case DO_ITEM_Ua_H:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ua_L:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ub_H:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ub_L:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Uc_H:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Uc_L:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
#endif
#if(FUNCTION_CURRENT == 1)
                            case DO_ITEM_Ia_H:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ia_L:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ib_H:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ib_L:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ic_H:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ic_L:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
#endif
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_F_L:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_DI1_H:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI1_L:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_L);
                                break;
                            case DO_ITEM_DI2_H:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI2_L:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_L);
                                break;
                            case DO_ITEM_DI3_H:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI3_L:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_L);
                                break;
                            case DO_ITEM_DI4_H:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI4_L:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_L);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
			    	break;
				case MENU2_4_INDEX_UAL:
					DispChar(1,CHARMODE_NC,CHARMODE_U,CHARMODE_A,CHARMODE_L);

                    switch(DispCtrlParam.DO1_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_4_INDEX_HYS:
					DispChar(1,CHARMODE_NC,CHARMODE_H,CHARMODE_Y,CHARMODE_S);
					switch(DispCtrlParam.DO1_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_4_INDEX_DELAY:
					DispChar(1,CHARMODE_D,CHARMODE_E,CHARMODE_L,CHARMODE_Y);
                    Decimal = 2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
            }
            break;
        case MENU1_INDEX_DO2:
            DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_2);
            switch(sg_DispSetMenu2_5_Index)
			{
                case MENU2_5_INDEX_MODE:
                    DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
                    if(!sg_bSetFlashEnable)
					{
                        if(sg_DispSetValue == DO_MODE_OFF)
                            DispChar(2,CHARMODE_NC,CHARMODE_O,CHARMODE_F,CHARMODE_F);
                        else if(sg_DispSetValue == DO_MODE_ALR)
                            DispChar(2,CHARMODE_NC,CHARMODE_A,CHARMODE_L,CHARMODE_R);
                        else if(sg_DispSetValue == DO_MODE_REM)
                            DispChar(2,CHARMODE_NC,CHARMODE_R,CHARMODE_E,CHARMODE_M);
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				case MENU2_5_INDEX_TIME:
					DispChar(1,CHARMODE_T,CHARMODE_I,CHARMODE_M,CHARMODE_E);

                    Decimal =  2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
			    	break;
				case MENU2_5_INDEX_ITEM:
					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);
					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
#if(FUNCTION_VOLTAGE == 1)
							case DO_ITEM_Ua_H:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ua_L:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ub_H:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ub_L:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Uc_H:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Uc_L:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
#endif
#if(FUNCTION_CURRENT == 1)
                            case DO_ITEM_Ia_H:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ia_L:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ib_H:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ib_L:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ic_H:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ic_L:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
#endif
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_F_L:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_DI1_H:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI1_L:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_L);
                                break;
                            case DO_ITEM_DI2_H:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI2_L:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_L);
                                break;
                            case DO_ITEM_DI3_H:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI3_L:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_L);
                                break;
                            case DO_ITEM_DI4_H:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI4_L:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_L);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
			    	break;
			    case MENU2_5_INDEX_UAL:
					DispChar(1,CHARMODE_NC,CHARMODE_U,CHARMODE_A,CHARMODE_L);

                    switch(DispCtrlParam.DO2_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_5_INDEX_HYS:
					DispChar(1,CHARMODE_NC,CHARMODE_H,CHARMODE_Y,CHARMODE_S);
					switch(DispCtrlParam.DO2_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                                break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_5_INDEX_DELAY:
					DispChar(1,CHARMODE_D,CHARMODE_E,CHARMODE_L,CHARMODE_Y);
                    Decimal = 2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
			}
			break;
        case MENU1_INDEX_DO3:
            DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_3);
            switch(sg_DispSetMenu2_6_Index)
			{
                case MENU2_6_INDEX_MODE:
                    DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
                    if(!sg_bSetFlashEnable)
					{
                        if(sg_DispSetValue == DO_MODE_OFF)
                            DispChar(2,CHARMODE_NC,CHARMODE_O,CHARMODE_F,CHARMODE_F);
                        else if(sg_DispSetValue == DO_MODE_ALR)
                            DispChar(2,CHARMODE_NC,CHARMODE_A,CHARMODE_L,CHARMODE_R);
                        else if(sg_DispSetValue == DO_MODE_REM)
                            DispChar(2,CHARMODE_NC,CHARMODE_R,CHARMODE_E,CHARMODE_M);
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				case MENU2_6_INDEX_TIME:
					DispChar(1,CHARMODE_T,CHARMODE_I,CHARMODE_M,CHARMODE_E);

                    Decimal =  2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
			    	break;
				case MENU2_6_INDEX_ITEM:
					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);

					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
#if(FUNCTION_VOLTAGE == 1)
							case DO_ITEM_Ua_H:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ua_L:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ub_H:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ub_L:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Uc_H:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Uc_L:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
#endif
#if(FUNCTION_CURRENT == 1)
                            case DO_ITEM_Ia_H:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ia_L:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ib_H:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ib_L:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ic_H:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ic_L:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
#endif
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_F_L:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_DI1_H:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI1_L:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_L);
                                break;
                            case DO_ITEM_DI2_H:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI2_L:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_L);
                                break;
                            case DO_ITEM_DI3_H:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI3_L:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_L);
                                break;
                            case DO_ITEM_DI4_H:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI4_L:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_L);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
			    	break;
			    case MENU2_6_INDEX_UAL:
					DispChar(1,CHARMODE_NC,CHARMODE_U,CHARMODE_A,CHARMODE_L);

                    switch(DispCtrlParam.DO3_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                                break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_6_INDEX_HYS:
					DispChar(1,CHARMODE_NC,CHARMODE_H,CHARMODE_Y,CHARMODE_S);
					switch(DispCtrlParam.DO3_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                                break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_6_INDEX_DELAY:
					DispChar(1,CHARMODE_D,CHARMODE_E,CHARMODE_L,CHARMODE_Y);
                    Decimal = 2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
			}
			break;
        case MENU1_INDEX_DO4:
            DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_4);
            switch(sg_DispSetMenu2_7_Index)
			{
                case MENU2_7_INDEX_MODE:
                    DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
                    if(!sg_bSetFlashEnable)
					{
                        if(sg_DispSetValue == DO_MODE_OFF)
                            DispChar(2,CHARMODE_NC,CHARMODE_O,CHARMODE_F,CHARMODE_F);
                        else if(sg_DispSetValue == DO_MODE_ALR)
                            DispChar(2,CHARMODE_NC,CHARMODE_A,CHARMODE_L,CHARMODE_R);
                        else if(sg_DispSetValue == DO_MODE_REM)
                            DispChar(2,CHARMODE_NC,CHARMODE_R,CHARMODE_E,CHARMODE_M);
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				case MENU2_7_INDEX_TIME:
					DispChar(1,CHARMODE_T,CHARMODE_I,CHARMODE_M,CHARMODE_E);

                    Decimal =  2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
			    	break;
				case MENU2_7_INDEX_ITEM:
					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);

					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
#if(FUNCTION_VOLTAGE == 1)
							case DO_ITEM_Ua_H:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ua_L:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ub_H:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ub_L:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Uc_H:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Uc_L:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
#endif
#if(FUNCTION_CURRENT == 1)
                            case DO_ITEM_Ia_H:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ia_L:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ib_H:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ib_L:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ic_H:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ic_L:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
#endif
                            /*-----------------------------------------------------*/
                            case DO_ITEM_F_H:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_F_L:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_DI1_H:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI1_L:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_L);
                                break;
                            case DO_ITEM_DI2_H:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI2_L:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_L);
                                break;
                            case DO_ITEM_DI3_H:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI3_L:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_L);
                                break;
                            case DO_ITEM_DI4_H:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_H);
                                break;
                            case DO_ITEM_DI4_L:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_L);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
			    	break;
			    case MENU2_7_INDEX_UAL:
					DispChar(1,CHARMODE_NC,CHARMODE_U,CHARMODE_A,CHARMODE_L);

                    switch(DispCtrlParam.DO4_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                                break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_7_INDEX_HYS:
					DispChar(1,CHARMODE_NC,CHARMODE_H,CHARMODE_Y,CHARMODE_S);
					switch(DispCtrlParam.DO4_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                                break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_7_INDEX_DELAY:
					DispChar(1,CHARMODE_D,CHARMODE_E,CHARMODE_L,CHARMODE_Y);
                    Decimal = 2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
			}
			break;
        case MENU1_INDEX_AO1:
            DispChar(0,CHARMODE_A,CHARMODE_O,CHARMODE_FU,CHARMODE_1);
            switch(sg_DispSetMenu2_8_Index)
			{
                case MENU2_8_INDEX_MODE:
                    DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
                    if(!sg_bSetFlashEnable)
					{
					    if(sg_DispSetValue == AO_MODE_OFF)
					        DispChar(2,CHARMODE_NC,CHARMODE_O,CHARMODE_F,CHARMODE_F);
                        else if(sg_DispSetValue == AO_MODE_420)
                            DispChar(2,CHARMODE_NC,CHARMODE_4,CHARMODE_2,CHARMODE_0);
                        else if(sg_DispSetValue == AO_MODE_020)
                            DispChar(2,CHARMODE_NC,CHARMODE_0,CHARMODE_2,CHARMODE_0);
                        else if(sg_DispSetValue == AO_MODE_1220)
                        {
                            DispChar(2,CHARMODE_1,CHARMODE_2,CHARMODE_2,CHARMODE_0);
                        }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				case MENU2_8_INDEX_ITEM:
					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);
					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
#if(FUNCTION_VOLTAGE == 1)
                            case AO_ITEM_UA:
                                DispChar(2,CHARMODE_U,CHARMODE_A,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_UB:
                                DispChar(2,CHARMODE_U,CHARMODE_B,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_UC:
                                DispChar(2,CHARMODE_U,CHARMODE_C,CHARMODE_NC,CHARMODE_NC);
                                break;
#endif
#if(FUNCTION_CURRENT == 1)
                            case AO_ITEM_IA:
                                DispChar(2,CHARMODE_I,CHARMODE_A,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_IB:
                                DispChar(2,CHARMODE_I,CHARMODE_B,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_IC:
                                DispChar(2,CHARMODE_I,CHARMODE_C,CHARMODE_NC,CHARMODE_NC);
                                break;
#endif
                            case AO_ITEM_F:
                                DispChar(2,CHARMODE_F,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
			    	break;
				case MENU2_8_INDEX_DS:
					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_D,CHARMODE_S);
					switch(DispCtrlParam.AO1_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
                        case AO_ITEM_UA:
                        case AO_ITEM_UB:
                        case AO_ITEM_UC:
                            Decimal = 1;
                            break;
#endif
#if(FUNCTION_CURRENT == 1)
                        case AO_ITEM_IA:
                        case AO_ITEM_IB:
                        case AO_ITEM_IC:
                            Decimal = 3;
                            break;
#endif
                        case AO_ITEM_F:
                            Decimal = 2;
                            break;
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_8_INDEX_FS:
					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_F,CHARMODE_S);
					switch(DispCtrlParam.AO1_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
                        case AO_ITEM_UA:
                        case AO_ITEM_UB:
                        case AO_ITEM_UC:
                            Decimal = 1;
                            break;
#endif
#if(FUNCTION_CURRENT == 1)
                        case AO_ITEM_IA:
                        case AO_ITEM_IB:
                        case AO_ITEM_IC:
                            Decimal = 3;
                            break;
#endif
                        case AO_ITEM_F:
                            Decimal = 2;
                            break;
                        default:
                            break;

				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
            }
            break;
        case MENU1_INDEX_AO2:
            DispChar(0,CHARMODE_A,CHARMODE_O,CHARMODE_FU,CHARMODE_2);
            switch(sg_DispSetMenu2_9_Index)
			{
                case MENU2_9_INDEX_MODE:
                    DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
                    if(!sg_bSetFlashEnable)
					{
					    if(sg_DispSetValue == AO_MODE_OFF)
					        DispChar(2,CHARMODE_NC,CHARMODE_O,CHARMODE_F,CHARMODE_F);
                        else if(sg_DispSetValue == AO_MODE_420)
                            DispChar(2,CHARMODE_NC,CHARMODE_4,CHARMODE_2,CHARMODE_0);
                        else if(sg_DispSetValue == AO_MODE_020)
                            DispChar(2,CHARMODE_NC,CHARMODE_0,CHARMODE_2,CHARMODE_0);
                        else if(sg_DispSetValue == AO_MODE_1220)
                        {
                            DispChar(2,CHARMODE_1,CHARMODE_2,CHARMODE_2,CHARMODE_0);
                        }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				case MENU2_9_INDEX_ITEM:
					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);
					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
#if(FUNCTION_VOLTAGE == 1)
                            case AO_ITEM_UA:
                                DispChar(2,CHARMODE_U,CHARMODE_A,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_UB:
                                DispChar(2,CHARMODE_U,CHARMODE_B,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_UC:
                                DispChar(2,CHARMODE_U,CHARMODE_C,CHARMODE_NC,CHARMODE_NC);
                                break;
#endif
#if(FUNCTION_CURRENT == 1)
                            case AO_ITEM_IA:
                                DispChar(2,CHARMODE_I,CHARMODE_A,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_IB:
                                DispChar(2,CHARMODE_I,CHARMODE_B,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_IC:
                                DispChar(2,CHARMODE_I,CHARMODE_C,CHARMODE_NC,CHARMODE_NC);
                                break;
#endif
                            case AO_ITEM_F:
                                DispChar(2,CHARMODE_F,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
			    	break;
				case MENU2_9_INDEX_DS:
					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_D,CHARMODE_S);
					switch(DispCtrlParam.AO2_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
                        case AO_ITEM_UA:
                        case AO_ITEM_UB:
                        case AO_ITEM_UC:
                            Decimal = 1;
                            break;
#endif
#if(FUNCTION_CURRENT == 1)
                        case AO_ITEM_IA:
                        case AO_ITEM_IB:
                        case AO_ITEM_IC:
                            Decimal = 3;
                            break;
#endif
                        case AO_ITEM_F:
                            Decimal = 2;
                            break;
                        default:
                            break;

				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_9_INDEX_FS:
					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_F,CHARMODE_S);
					switch(DispCtrlParam.AO2_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
                        case AO_ITEM_UA:
                        case AO_ITEM_UB:
                        case AO_ITEM_UC:
                            Decimal = 1;
                            break;
#endif
#if(FUNCTION_CURRENT == 1)
                        case AO_ITEM_IA:
                        case AO_ITEM_IB:
                        case AO_ITEM_IC:
                            Decimal = 3;
                            break;
#endif
                        case AO_ITEM_F:
                            Decimal = 2;
                            break;
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
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
	if(DispMode == ReadMode)
	{
        sg_bSetFlashEnable = FALSE; //查看选项，不闪烁
	}
	switch(sg_DispSetMenu1Index)
	{
		case MENU1_INDEX_INPT:
			DispChar(0,CHARMODE_I,CHARMODE_N,CHARMODE_P,CHARMODE_T);
			switch(sg_DispSetMenu2_1_Index)
			{
#if(FUNCTION_VOLTAGE == 1)
				case MENU2_1_INDEX_PT:
					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_P,CHARMODE_T);

					Decimal =  0;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal, Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
#endif
#if(FUNCTION_CURRENT == 1)
				case MENU2_1_INDEX_CT:
					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_C,CHARMODE_T);

					Decimal = 0;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
#endif
				default:
				    break;
			}
		    break;
	    case MENU1_INDEX_COM1:
	        DispChar(0,CHARMODE_NC,CHARMODE_C,CHARMODE_O,CHARMODE_M);
			switch(sg_DispSetMenu2_2_Index)
			{
				case MENU2_2_INDEX_ADD:
					DispChar(1,CHARMODE_NC,CHARMODE_A,CHARMODE_D,CHARMODE_D);
					Decimal =  0;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
				case MENU2_2_INDEX_BAUD:
					DispChar(1,CHARMODE_B,CHARMODE_A,CHARMODE_U,CHARMODE_D);
					if(sg_bSetFlashEnable)
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
					else
					{
                        switch(sg_DispSetValue)
    					{
                            case Baud12:
                                DispChar(2,CHARMODE_1,CHARMODE_2,CHARMODE_0,CHARMODE_0);
                                break;
                            case Baud24:
                                DispChar(2,CHARMODE_2,CHARMODE_4,CHARMODE_0,CHARMODE_0);
                                break;
                            case Baud48:
                                DispChar(2,CHARMODE_4,CHARMODE_8,CHARMODE_0,CHARMODE_0);
                                break;
                            case Baud96:
                                DispChar(2,CHARMODE_9,CHARMODE_6,CHARMODE_0,CHARMODE_0);
                                break;
                            case Baud192:
                                DispChar(2,CHARMODE_1,CHARMODE_9+CHARMODE_DOT,CHARMODE_2,CHARMODE_0);
                                break;
                            default:
                                break;
    					}
					}
			    	break;
				case MENU2_2_INDEX_DATA:
					DispChar(1,CHARMODE_D,CHARMODE_A,CHARMODE_T,CHARMODE_A);
				    if(sg_bSetFlashEnable)
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
					else
					{
                        switch(sg_DispSetValue)
    					{
                            case n81:
                                DispChar(2,CHARMODE_NC,CHARMODE_N,CHARMODE_8,CHARMODE_1);
                                break;
                            case o81:
                                DispChar(2,CHARMODE_NC,CHARMODE_O,CHARMODE_8,CHARMODE_1);
                                break;
                            case e81:
                                DispChar(2,CHARMODE_NC,CHARMODE_E,CHARMODE_8,CHARMODE_1);
                                break;
                            case n82:
                                DispChar(2,CHARMODE_NC,CHARMODE_N,CHARMODE_8,CHARMODE_2);
                                break;
                            default:
                                break;
    					}
					}
			    	break;
				case MENU2_2_INDEX_COMS:
					DispChar(1,CHARMODE_C,CHARMODE_O,CHARMODE_M,CHARMODE_S);
					if(sg_DispSetValue == rtu485)
					{
                        if(!sg_bSetFlashEnable)
                			DispChar(2,CHARMODE_NC,CHARMODE_R,CHARMODE_T,CHARMODE_U);
                		else
                			DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
					else if( sg_DispSetValue == calib)
					{
                        if(!sg_bSetFlashEnable)
                			DispChar(2,CHARMODE_NC,CHARMODE_C,CHARMODE_A,CHARMODE_L);
                		else
                			DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				default:
				    break;
			}
		    break;
        case MENU1_INDEX_SYS:
            DispChar(0,CHARMODE_NC,CHARMODE_S,CHARMODE_Y,CHARMODE_S);
            switch(sg_DispSetMenu2_3_Index)
			{
				case MENU2_3_INDEX_LIGH:        // 亮度等级
					DispChar(1,CHARMODE_L,CHARMODE_I,CHARMODE_G,CHARMODE_H);
					if(!sg_bSetFlashEnable)
					{
                        switch(sg_DispSetValue)
                        {
                            case BrightLevel_1:
                                DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_L,CHARMODE_1);
                                break;
                            case BrightLevel_2:
                                DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_L,CHARMODE_2);
                                break;
                            case BrightLevel_3:
                                DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_L,CHARMODE_3);
                                break;
                            default:
                                break;
                        }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				case MENU2_3_INDEX_CODE:
					DispChar(1,CHARMODE_C,CHARMODE_O,CHARMODE_D,CHARMODE_E);

					Decimal =  0;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal, Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, FALSE, TRUE);
					CharToNumber();
				    break;
			    case MENU2_3_INDEX_RESET:
			        DispChar(1,CHARMODE_C,CHARMODE_L,CHARMODE_E,CHARMODE_A);
					if( sg_DispSetValue == SaveYes)
            		{
                        if(sg_bSetFlashEnable)
                			DispChar(2,CHARMODE_NC,CHARMODE_Y,CHARMODE_E,CHARMODE_S);
                		else
                			DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
            		}
            		else if( sg_DispSetValue == SaveNO)
            		{
                        if(sg_bSetFlashEnable)
                			DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_N,CHARMODE_O);
                		else
                			DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
            		}
				    break;
				default:
				    break;
            }
		    break;
		case MENU1_INDEX_DO1:
            DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_1);
            switch(sg_DispSetMenu2_4_Index)
			{
                case MENU2_4_INDEX_MODE:
                    DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
                    if(!sg_bSetFlashEnable)
					{
                        if(sg_DispSetValue == DO_MODE_OFF)
                            DispChar(2,CHARMODE_NC,CHARMODE_O,CHARMODE_F,CHARMODE_F);
                        else if(sg_DispSetValue == DO_MODE_ALR)
                            DispChar(2,CHARMODE_NC,CHARMODE_A,CHARMODE_L,CHARMODE_R);
                        else if(sg_DispSetValue == DO_MODE_REM)
                            DispChar(2,CHARMODE_NC,CHARMODE_R,CHARMODE_E,CHARMODE_M);
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				case MENU2_4_INDEX_TIME:
					DispChar(1,CHARMODE_T,CHARMODE_I,CHARMODE_M,CHARMODE_E);

                    Decimal =  2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
			    	break;
				case MENU2_4_INDEX_ITEM:
					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);

					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
#if(FUNCTION_VOLTAGE == 1)
							case DO_ITEM_Ua_H:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ua_L:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ub_H:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ub_L:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Uc_H:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Uc_L:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*-------------------------------------------------------*/
#endif
#if(FUNCTION_CURRENT == 1)
                            case DO_ITEM_Ia_H:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ia_L:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ib_H:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ib_L:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ic_H:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ic_L:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*------------------------------------------------------*/
#endif
                            case DO_ITEM_F_H:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_F_L:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_DI1_H:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI1_L:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_0);
                                break;
                            case DO_ITEM_DI2_H:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI2_L:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_0);
                                break;
                            case DO_ITEM_DI3_H:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI3_L:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_0);
                                break;
                            case DO_ITEM_DI4_H:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI4_L:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_0);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
			    	break;
				case MENU2_4_INDEX_UAL:
					DispChar(1,CHARMODE_NC,CHARMODE_U,CHARMODE_A,CHARMODE_L);

                    switch(DispCtrlParam.DO1_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_4_INDEX_HYS:
					DispChar(1,CHARMODE_NC,CHARMODE_H,CHARMODE_Y,CHARMODE_S);
					switch(DispCtrlParam.DO1_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_4_INDEX_DELAY:
					DispChar(1,CHARMODE_D,CHARMODE_E,CHARMODE_L,CHARMODE_Y);
                    Decimal = 2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
            }
            break;
        case MENU1_INDEX_DO2:
            DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_2);
            switch(sg_DispSetMenu2_5_Index)
			{
                case MENU2_5_INDEX_MODE:
                    DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
                    if(!sg_bSetFlashEnable)
					{
                        if(sg_DispSetValue == DO_MODE_OFF)
                            DispChar(2,CHARMODE_NC,CHARMODE_O,CHARMODE_F,CHARMODE_F);
                        else if(sg_DispSetValue == DO_MODE_ALR)
                            DispChar(2,CHARMODE_NC,CHARMODE_A,CHARMODE_L,CHARMODE_R);
                        else if(sg_DispSetValue == DO_MODE_REM)
                            DispChar(2,CHARMODE_NC,CHARMODE_R,CHARMODE_E,CHARMODE_M);
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				case MENU2_5_INDEX_TIME:
					DispChar(1,CHARMODE_T,CHARMODE_I,CHARMODE_M,CHARMODE_E);

                    Decimal =  2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
			    	break;
				case MENU2_5_INDEX_ITEM:
					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);

					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
#if(FUNCTION_VOLTAGE == 1)
							case DO_ITEM_Ua_H:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ua_L:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ub_H:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ub_L:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Uc_H:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Uc_L:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*-------------------------------------------------------*/
#endif
#if(FUNCTION_CURRENT == 1)
                            case DO_ITEM_Ia_H:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ia_L:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ib_H:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ib_L:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ic_H:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ic_L:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*------------------------------------------------------*/
#endif
                            case DO_ITEM_F_H:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_F_L:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_DI1_H:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI1_L:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_0);
                                break;
                            case DO_ITEM_DI2_H:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI2_L:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_0);
                                break;
                            case DO_ITEM_DI3_H:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI3_L:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_0);
                                break;
                            case DO_ITEM_DI4_H:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI4_L:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_0);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
			    	break;
			    case MENU2_5_INDEX_UAL:
					DispChar(1,CHARMODE_NC,CHARMODE_U,CHARMODE_A,CHARMODE_L);

                    switch(DispCtrlParam.DO2_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_5_INDEX_HYS:
					DispChar(1,CHARMODE_NC,CHARMODE_H,CHARMODE_Y,CHARMODE_S);
					switch(DispCtrlParam.DO2_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_5_INDEX_DELAY:
					DispChar(1,CHARMODE_D,CHARMODE_E,CHARMODE_L,CHARMODE_Y);
                    Decimal = 2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
			}
			break;
        case MENU1_INDEX_DO3:
            DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_3);
            switch(sg_DispSetMenu2_6_Index)
			{
                case MENU2_6_INDEX_MODE:
                    DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
                    if(!sg_bSetFlashEnable)
					{
                        if(sg_DispSetValue == DO_MODE_OFF)
                            DispChar(2,CHARMODE_NC,CHARMODE_O,CHARMODE_F,CHARMODE_F);
                        else if(sg_DispSetValue == DO_MODE_ALR)
                            DispChar(2,CHARMODE_NC,CHARMODE_A,CHARMODE_L,CHARMODE_R);
                        else if(sg_DispSetValue == DO_MODE_REM)
                            DispChar(2,CHARMODE_NC,CHARMODE_R,CHARMODE_E,CHARMODE_M);
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				case MENU2_6_INDEX_TIME:
					DispChar(1,CHARMODE_T,CHARMODE_I,CHARMODE_M,CHARMODE_E);

                    Decimal =  2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
			    	break;
				case MENU2_6_INDEX_ITEM:
					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);

					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
#if(FUNCTION_VOLTAGE == 1)
							case DO_ITEM_Ua_H:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ua_L:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ub_H:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ub_L:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Uc_H:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Uc_L:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*-------------------------------------------------------*/
#endif
#if(FUNCTION_CURRENT == 1)
                            case DO_ITEM_Ia_H:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ia_L:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ib_H:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ib_L:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ic_H:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ic_L:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*------------------------------------------------------*/
#endif
                            case DO_ITEM_F_H:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_F_L:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_DI1_H:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI1_L:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_0);
                                break;
                            case DO_ITEM_DI2_H:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI2_L:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_0);
                                break;
                            case DO_ITEM_DI3_H:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI3_L:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_0);
                                break;
                            case DO_ITEM_DI4_H:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI4_L:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_0);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
			    	break;
			    case MENU2_6_INDEX_UAL:
					DispChar(1,CHARMODE_NC,CHARMODE_U,CHARMODE_A,CHARMODE_L);

                    switch(DispCtrlParam.DO3_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_6_INDEX_HYS:
					DispChar(1,CHARMODE_NC,CHARMODE_H,CHARMODE_Y,CHARMODE_S);
					switch(DispCtrlParam.DO3_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_6_INDEX_DELAY:
					DispChar(1,CHARMODE_D,CHARMODE_E,CHARMODE_L,CHARMODE_Y);
                    Decimal = 2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
			}
			break;
        case MENU1_INDEX_DO4:
            DispChar(0,CHARMODE_D,CHARMODE_O,CHARMODE_FU,CHARMODE_4);
            switch(sg_DispSetMenu2_7_Index)
			{
                case MENU2_7_INDEX_MODE:
                    DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
                    if(!sg_bSetFlashEnable)
					{
                        if(sg_DispSetValue == DO_MODE_OFF)
                            DispChar(2,CHARMODE_NC,CHARMODE_O,CHARMODE_F,CHARMODE_F);
                        else if(sg_DispSetValue == DO_MODE_ALR)
                            DispChar(2,CHARMODE_NC,CHARMODE_A,CHARMODE_L,CHARMODE_R);
                        else if(sg_DispSetValue == DO_MODE_REM)
                            DispChar(2,CHARMODE_NC,CHARMODE_R,CHARMODE_E,CHARMODE_M);
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				case MENU2_7_INDEX_TIME:
					DispChar(1,CHARMODE_T,CHARMODE_I,CHARMODE_M,CHARMODE_E);

                    Decimal =  2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
			    	break;
				case MENU2_7_INDEX_ITEM:
					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);

					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
#if(FUNCTION_VOLTAGE == 1)
							case DO_ITEM_Ua_H:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ua_L:
                                DispChar(2,CHARMODE_U,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ub_H:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ub_L:
                                DispChar(2,CHARMODE_U,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Uc_H:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Uc_L:
                                DispChar(2,CHARMODE_U,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*-------------------------------------------------------*/
#endif
#if(FUNCTION_CURRENT == 1)
                            case DO_ITEM_Ia_H:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ia_L:
                                DispChar(2,CHARMODE_I,CHARMODE_A + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ib_H:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ib_L:
                                DispChar(2,CHARMODE_I,CHARMODE_B + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            case DO_ITEM_Ic_H:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_Ic_L:
                                DispChar(2,CHARMODE_I,CHARMODE_C + CHARMODE_DOT,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*------------------------------------------------------*/
#endif
                            case DO_ITEM_F_H:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_H);
                                break;
                            case DO_ITEM_F_L:
                                DispChar(2,CHARMODE_F + CHARMODE_DOT,CHARMODE_NC,CHARMODE_NC,CHARMODE_L);
                                break;
                            /*-----------------------------------------------------*/
                            case DO_ITEM_DI1_H:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI1_L:
                                DispChar(2,CHARMODE_D,CHARMODE_1,CHARMODE_FU,CHARMODE_0);
                                break;
                            case DO_ITEM_DI2_H:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI2_L:
                                DispChar(2,CHARMODE_D,CHARMODE_2,CHARMODE_FU,CHARMODE_0);
                                break;
                            case DO_ITEM_DI3_H:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI3_L:
                                DispChar(2,CHARMODE_D,CHARMODE_3,CHARMODE_FU,CHARMODE_0);
                                break;
                            case DO_ITEM_DI4_H:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_1);
                                break;
                            case DO_ITEM_DI4_L:
                                DispChar(2,CHARMODE_D,CHARMODE_4,CHARMODE_FU,CHARMODE_0);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
			    	break;
			    case MENU2_7_INDEX_UAL:
					DispChar(1,CHARMODE_NC,CHARMODE_U,CHARMODE_A,CHARMODE_L);

                    switch(DispCtrlParam.DO4_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_7_INDEX_HYS:
					DispChar(1,CHARMODE_NC,CHARMODE_H,CHARMODE_Y,CHARMODE_S);
					switch(DispCtrlParam.DO4_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
						case DO_ITEM_Ua_H:
                        case DO_ITEM_Ua_L:
                        case DO_ITEM_Ub_H:
                        case DO_ITEM_Ub_L:
                        case DO_ITEM_Uc_H:
                        case DO_ITEM_Uc_L:
                            Decimal = 1;
                            break;
#endif
                        /*-------------------------------------------------------*/
#if(FUNCTION_CURRENT == 1)
                        case DO_ITEM_Ia_H:
                        case DO_ITEM_Ia_L:
                        case DO_ITEM_Ib_H:
                        case DO_ITEM_Ib_L:
                        case DO_ITEM_Ic_H:
                        case DO_ITEM_Ic_L:
                            Decimal = 3;
                            break;
#endif
                        /*-----------------------------------------------------*/
                        case DO_ITEM_F_H:
                        case DO_ITEM_F_L:
                        /*-----------------------------------------------------*/
                            Decimal = 2;
                            break;
                        /*-----------------------------------------------------*/
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_7_INDEX_DELAY:
					DispChar(1,CHARMODE_D,CHARMODE_E,CHARMODE_L,CHARMODE_Y);
                    Decimal = 2;
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
			}
			break;
        case MENU1_INDEX_AO1:
            DispChar(0,CHARMODE_A,CHARMODE_O,CHARMODE_FU,CHARMODE_1);
            switch(sg_DispSetMenu2_8_Index)
			{
                case MENU2_8_INDEX_MODE:
                    DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
                    if(!sg_bSetFlashEnable)
					{
					    if(sg_DispSetValue == AO_MODE_OFF)
					        DispChar(2,CHARMODE_NC,CHARMODE_O,CHARMODE_F,CHARMODE_F);
                        else if(sg_DispSetValue == AO_MODE_420)
                            DispChar(2,CHARMODE_NC,CHARMODE_4,CHARMODE_2,CHARMODE_0);
                        else if(sg_DispSetValue == AO_MODE_020)
                            DispChar(2,CHARMODE_NC,CHARMODE_0,CHARMODE_2,CHARMODE_0);
                        else if(sg_DispSetValue == AO_MODE_1220)
                        {
                            DispChar(2,CHARMODE_1,CHARMODE_2,CHARMODE_2,CHARMODE_0);
                        }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				case MENU2_8_INDEX_ITEM:
					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);
					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
#if(FUNCTION_VOLTAGE == 1)
                            case AO_ITEM_UA:
                                DispChar(2,CHARMODE_U,CHARMODE_A,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_UB:
                                DispChar(2,CHARMODE_U,CHARMODE_B,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_UC:
                                DispChar(2,CHARMODE_U,CHARMODE_C,CHARMODE_NC,CHARMODE_NC);
                                break;
#endif
#if(FUNCTION_CURRENT == 1)
                            case AO_ITEM_IA:
                                DispChar(2,CHARMODE_I,CHARMODE_A,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_IB:
                                DispChar(2,CHARMODE_I,CHARMODE_B,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_IC:
                                DispChar(2,CHARMODE_I,CHARMODE_C,CHARMODE_NC,CHARMODE_NC);
                                break;
#endif
                            case AO_ITEM_F:
                                DispChar(2,CHARMODE_F,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
			    	break;
				case MENU2_8_INDEX_DS:
					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_D,CHARMODE_S);
					switch(DispCtrlParam.AO1_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
                        case AO_ITEM_UA:
                        case AO_ITEM_UB:
                        case AO_ITEM_UC:
                            Decimal = 1;
                            break;
#endif
#if(FUNCTION_CURRENT == 1)
                        case AO_ITEM_IA:
                        case AO_ITEM_IB:
                        case AO_ITEM_IC:
                            Decimal = 3;
                            break;
#endif
                        case AO_ITEM_F:
                            Decimal = 2;
                            break;
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_8_INDEX_FS:
					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_F,CHARMODE_S);
					switch(DispCtrlParam.AO1_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
                        case AO_ITEM_UA:
                        case AO_ITEM_UB:
                        case AO_ITEM_UC:
                            Decimal = 1;
                            break;
#endif
#if(FUNCTION_CURRENT == 1)
                        case AO_ITEM_IA:
                        case AO_ITEM_IB:
                        case AO_ITEM_IC:
                            Decimal = 3;
                            break;
#endif
                        case AO_ITEM_F:
                            Decimal = 2;
                            break;
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				default:
				    break;
            }
            break;
        case MENU1_INDEX_AO2:
            DispChar(0,CHARMODE_A,CHARMODE_O,CHARMODE_FU,CHARMODE_2);
            switch(sg_DispSetMenu2_9_Index)
			{
                case MENU2_9_INDEX_MODE:
                    DispChar(1,CHARMODE_M,CHARMODE_O,CHARMODE_D,CHARMODE_E);
                    if(!sg_bSetFlashEnable)
					{
					    if(sg_DispSetValue == AO_MODE_OFF)
					        DispChar(2,CHARMODE_NC,CHARMODE_O,CHARMODE_F,CHARMODE_F);
                        else if(sg_DispSetValue == AO_MODE_420)
                            DispChar(2,CHARMODE_NC,CHARMODE_4,CHARMODE_2,CHARMODE_0);
                        else if(sg_DispSetValue == AO_MODE_020)
                            DispChar(2,CHARMODE_NC,CHARMODE_0,CHARMODE_2,CHARMODE_0);
                        else if(sg_DispSetValue == AO_MODE_1220)
                        {
                            DispChar(2,CHARMODE_1,CHARMODE_2,CHARMODE_2,CHARMODE_0);
                        }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
				    break;
				case MENU2_9_INDEX_ITEM:
					DispChar(1,CHARMODE_I,CHARMODE_T,CHARMODE_E,CHARMODE_M);
					if(!sg_bSetFlashEnable)
					{
					    switch(sg_DispSetValue)
					    {
#if(FUNCTION_VOLTAGE == 1)
                            case AO_ITEM_UA:
                                DispChar(2,CHARMODE_U,CHARMODE_A,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_UB:
                                DispChar(2,CHARMODE_U,CHARMODE_B,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_UC:
                                DispChar(2,CHARMODE_U,CHARMODE_C,CHARMODE_NC,CHARMODE_NC);
                                break;
#endif
#if(FUNCTION_CURRENT == 1)
                            case AO_ITEM_IA:
                                DispChar(2,CHARMODE_I,CHARMODE_A,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_IB:
                                DispChar(2,CHARMODE_I,CHARMODE_B,CHARMODE_NC,CHARMODE_NC);
                                break;
                            case AO_ITEM_IC:
                                DispChar(2,CHARMODE_I,CHARMODE_C,CHARMODE_NC,CHARMODE_NC);
                                break;
#endif
                            case AO_ITEM_F:
                                DispChar(2,CHARMODE_F,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
                                break;
                            default:
                                break;
					    }
					}
					else
					{
                        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
					}
			    	break;
				case MENU2_9_INDEX_DS:
					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_D,CHARMODE_S);
					switch(DispCtrlParam.AO2_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
                        case AO_ITEM_UA:
                        case AO_ITEM_UB:
                        case AO_ITEM_UC:
                            Decimal = 1;
                            break;
#endif
#if(FUNCTION_CURRENT == 1)
                        case AO_ITEM_IA:
                        case AO_ITEM_IB:
                        case AO_ITEM_IC:
                            Decimal = 3;
                            break;
#endif
                        case AO_ITEM_F:
                            Decimal = 2;
                            break;
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
					CharToNumber();
				    break;
				case MENU2_9_INDEX_FS:
					DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_F,CHARMODE_S);
					switch(DispCtrlParam.AO2_Item)
				    {
#if(FUNCTION_VOLTAGE == 1)
                        case AO_ITEM_UA:
                        case AO_ITEM_UB:
                        case AO_ITEM_UC:
                            Decimal = 1;
                            break;
#endif
#if(FUNCTION_CURRENT == 1)
                        case AO_ITEM_IA:
                        case AO_ITEM_IB:
                        case AO_ITEM_IC:
                            Decimal = 3;
                            break;
#endif
                        case AO_ITEM_F:
                            Decimal = 2;
                            break;
                        default:
                            break;
				    }
					Disp_Focus.DispWidth = 4;
					DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
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
	u8 Decimal = 0;
	DispChar(0,CHARMODE_P,CHARMODE_R,CHARMODE_O,CHARMODE_G);
	DispChar(1,CHARMODE_C,CHARMODE_O,CHARMODE_D,CHARMODE_E);
	Decimal =  0;
	DispValue(2,sg_DispSetValue, Decimal,  Disp_Focus.DispWidth, sg_bSetFlashEnable, Disp_Focus.CurrentFocus, TRUE, TRUE);
	CharToNumber();
}

void DispMenu_MenuCodeErr(void)
{
	DispChar(0,CHARMODE_P,CHARMODE_R,CHARMODE_O,CHARMODE_G);
	DispChar(1,CHARMODE_C,CHARMODE_O,CHARMODE_D,CHARMODE_E);
	if(sg_bSetFlashEnable)
		DispChar(2,CHARMODE_NC,CHARMODE_E,CHARMODE_R,CHARMODE_R);
	else
		DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
}

void DispMenu_MenuSave(void)
{
		DispChar(0,CHARMODE_S,CHARMODE_A,CHARMODE_U,CHARMODE_E);
	    DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
        if( sg_DispSetValue == SaveYes)
		{
            if(sg_bSetFlashEnable)
    			DispChar(2,CHARMODE_NC,CHARMODE_Y,CHARMODE_E,CHARMODE_S);
    		else
    			DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
		}
		else if( sg_DispSetValue == SaveNO)
		{
            if(sg_bSetFlashEnable)
    			DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_N,CHARMODE_O);
    		else
    			DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
		}
}

void DispMenu_MenuAdjust(void)
{
    if(sg_bSetFlashEnable)
    {
        DispMenu_Main();
    }
    else
    {
        DispChar(0,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
        DispChar(1,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
        DispChar(2,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC,CHARMODE_NC);
    }
#if(FUNCTION_VOLTAGE == 1)
    if(g_tParam.CtrlParam.AdjustFinishFlag == FALSE)
    {
        g_tParam.RN_Coefficients.Kram[0] = 22000.0/vg_Power_Val.Real2_Val[PH_A].U;
        g_tParam.RN_Coefficients.Kram[1] = 22000.0/vg_Power_Val.Real2_Val[PH_B].U;
        g_tParam.RN_Coefficients.Kram[2] = 22000.0/vg_Power_Val.Real2_Val[PH_C].U;
        g_tParam.CtrlParam.AdjustFinishFlag = TRUE;
    }
#endif
#if(FUNCTION_CURRENT == 1)
    if(g_tParam.CtrlParam.AdjustFinishFlag == FALSE)
    {
        g_tParam.RN_Coefficients.Kram[0] = 50000.0/vg_Power_Val.Real2_Val[PH_A].I;
        g_tParam.RN_Coefficients.Kram[1] = 50000.0/vg_Power_Val.Real2_Val[PH_B].I;
        g_tParam.RN_Coefficients.Kram[2] = 50000.0/vg_Power_Val.Real2_Val[PH_C].I;
        g_tParam.CtrlParam.AdjustFinishFlag = TRUE;
    }
#endif
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
#if(FUNCTION_VOLTAGE == 1)
				case MENU2_1_INDEX_PT:
					sg_DispSetValue = DispCtrlParam.PTNum;
				    break;
#endif
#if(FUNCTION_CURRENT == 1)
				case MENU2_1_INDEX_CT:
					sg_DispSetValue = DispCtrlParam.CTNum;
				    break;
#endif
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
				case MENU2_3_INDEX_LIGH:
					sg_DispSetValue = DispCtrlParam.BlackTime;
					LEDLEVEL = DispCtrlParam.BlackTime;
				    break;
				case MENU2_3_INDEX_CODE:
					sg_DispSetValue = DispCtrlParam.Code;
				    break;
				case MENU2_3_INDEX_RESET:
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
                    sg_DispSetValue = DispCtrlParam.DO1_Item;
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
                    sg_DispSetValue = DispCtrlParam.DO2_Item;
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
                    sg_DispSetValue = DispCtrlParam.DO3_Item;
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
                    sg_DispSetValue = DispCtrlParam.DO4_Item;
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
                    sg_DispSetValue = DispCtrlParam.AO1_Item;
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
                    sg_DispSetValue = DispCtrlParam.AO2_Item;
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
#if(FUNCTION_VOLTAGE == 1)
				case MENU2_1_INDEX_PT:
					DispCtrlParam.PTNum =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.PTNum,(s32*)&sg_DispSetValue,MIN_PT,MAX_PT);
				    break;
#endif
#if(FUNCTION_CURRENT == 1)
				case MENU2_1_INDEX_CT:
					DispCtrlParam.CTNum =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.CTNum,(s32*)&sg_DispSetValue,MIN_CT,MAX_CT);
				    break;
#endif
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
				case MENU2_3_INDEX_LIGH:
					DispCtrlParam.BlackTime =
					    (u16)JudgeSetNewValue(&UpData,(s32)DispCtrlParam.BlackTime,(s32*)&sg_DispSetValue,MIN_BlackTime,MAX_BlackTime);
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
#if(FUNCTION_VOLTAGE == 1)
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
#endif
#if(FUNCTION_CURRENT == 1)
            if(Updata_Dir == UPDATA_ADD)
			{
				sg_DispSetValue++;
				if(sg_DispSetValue > DO_ITEM_DI4_L)
				{
					sg_DispSetValue = DO_ITEM_Ia_H;
				}
			}
			else
			{
				sg_DispSetValue--;
				if(sg_DispSetValue < DO_ITEM_Ia_H)
				{
					sg_DispSetValue = DO_ITEM_DI4_L;
				}
			}
#endif
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
#if(FUNCTION_VOLTAGE == 1)
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
#endif
#if(FUNCTION_CURRENT == 1)
                if(Updata_Dir == UPDATA_ADD)
                {
                    sg_DispSetValue++;
                    if(sg_DispSetValue > AO_ITEM_F)
                    {
                        sg_DispSetValue = AO_ITEM_IA;
                    }
                }
                else
                {
                    sg_DispSetValue--;
                    if(sg_DispSetValue < AO_ITEM_IA)
                    {
                        sg_DispSetValue = AO_ITEM_F;
                    }
                }
#endif
            break;
        case Select_Light:
            if(Updata_Dir == UPDATA_ADD)
            {
                sg_DispSetValue++;
                if(sg_DispSetValue > BrightLevel_3)
                {
                    sg_DispSetValue = BrightLevel_1;
                }
                LEDLEVEL= sg_DispSetValue;
            }
            else
            {
                sg_DispSetValue--;
                if(sg_DispSetValue == BrightLevel_Off)
                {
                    sg_DispSetValue = BrightLevel_3;
                }
                LEDLEVEL = sg_DispSetValue;
            }
            break;
        case Select_Reset:
            if(sg_DispSetValue == SaveYes)
            {
                sg_DispSetValue = SaveNO;
            }
            else
            {
                sg_DispSetValue = SaveYes;
            }
            break;
        default:
            break;
    }
}

#endif // #ifdef _GUILED_

//===========================================================================
//  End.
//===========================================================================


