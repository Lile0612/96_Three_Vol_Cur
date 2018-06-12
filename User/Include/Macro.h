#ifndef _MACRO_H
#define _MACRO_H

//#include "stm32f10x.h"
#if defined __ICCARM__
#define __packed
#endif


typedef signed long long  s64;
typedef signed long  s32;
typedef signed short s16;
typedef signed char  s8;

typedef signed long  const sc32;  /* Read Only */
typedef signed short const sc16;  /* Read Only */
typedef signed char  const sc8;   /* Read Only */

typedef volatile signed long  vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

typedef volatile signed long  const vsc32;  /* Read Only */
typedef volatile signed short const vsc16;  /* Read Only */
typedef volatile signed char  const vsc8;   /* Read Only */

typedef unsigned long long  u64;
typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef unsigned long  const uc32;  /* Read Only */
typedef unsigned short const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */

typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned long  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */

typedef enum {FALSE = 0, TRUE = !FALSE} bool;
//#define FALSE   0
//#define TRUE   1

//typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

//typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
//#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

//typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

#define U8_MAX     ((u8)255)
#define S8_MAX     ((s8)127)
#define S8_MIN     ((s8)-128)
#define U16_MAX    ((u16)65535u)
#define S16_MAX    ((s16)32767)
#define S16_MIN    ((s16)-32768)
#define U32_MAX    ((u32)4294967295uL)
#define S32_MAX    ((s32)2147483647)
#define S32_MIN    ((s32)-2147483648)
//////////////////////////////////////////////////////////////////////////////////////////
typedef unsigned char  BOOL;
typedef unsigned char  BYTE;                    /* Unsigned  8 bit quantity       */
typedef signed   char  sBYTE;                   /* Signed    8 bit quantity       */
typedef unsigned short WORD;                    /* Unsigned 16 bit quantity       */
typedef signed   short sWORD;                   /* Signed   16 bit quantity       */
typedef unsigned int   DWORD;                   /* Unsigned 32 bit quantity       */
typedef signed   int   sDWORD;                  /* Signed   32 bit quantity       */
typedef float          FP32;                   /* Single precision floating point*/
typedef double         FP64;                   /* Double precision floating point*/

#ifndef NULL
#define NULL ((void *)0)
#endif


#define GET_BITFIELD(addr)         (*((volatile bitfield *)(addr)))

#define __GET_BIT(addr,b)        GET_BITFIELD(addr).bit##b
#define GET_BIT(addr,b) __GET_BIT(addr,b)

#define GET_Bitfield(addr)         (*(( bitfield *)(addr)))
#define __GET_Bit(addr,b)       GET_Bitfield(addr).bit##b
#define GET_Bit(addr,b)       __GET_Bit(addr,b)

#define GET_IO_BITFIELD(addr) GET_BITFIELD(addr)
#define GET_IO_BIT(addr,b) GET_BIT(addr,b)


//---------------------------------------------------------------------------------

// = 按照HSB格式把两个字节转化为一个Word
#define FHIPW( ray ) ( (((WORD) (ray)[0]) << 8) + (ray)[1] )

// = 按照LSB格式把两个字节转化为一个Word
#define FLIPW( ray ) ( (((WORD) (ray)[1]) << 8) + (ray)[0] )
// = 按照LSB格式把四个字节转化为一个DWord
#define FLIPDW( ray ) ((((((DWORD) (ray)[3]) << 8) + (ray)[2])) * 65536 + ((ray)[1] << 8) + (ray)[0])
// = 得到一个字的高位和低位字节
#define WORD_LO(xxx) ((BYTE) ((WORD)(xxx) & 0xff))
#define WORD_HI(xxx) ((BYTE) ((WORD)(xxx) >> 8))
// = 得到一个双字的高位和低位字
#define DWORD_LO(xxx) ((WORD) ((DWORD)(xxx) & 0xffff))
#define DWORD_HI(xxx) ((WORD) ((DWORD)(xxx) / 65536))
// =设置某位
#define Set_Bit(val, bitn)	(val |= (1 << (bitn)))
// =清除某位
#define Clr_Bit(val, bitn)	(val &= ~(1 << (bitn)))
// =得到某位
#define Get_Bit(val, bitn)	((val >> bitn) & 1)


#define MODIFY_BIT(val, bitdata)	(val &= ~bitdata)

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

//---------------------------------------------------------------------------------
#define INIT_ZERO_VAL	0			// 初始化值:0
#define INIT_FFFF_VAL	0xffffffff	// 初始化值:ff
#define DATA_INVALID	0xff		// 无效的数据
//---------------------------------------------------------------------------------
#define TIME_CTRL_RATIO		1000	// 控制流程时间倍率
#define PROTECT_TIME_RATIO	1000	// 保护发生时间倍率,1秒=1000
//---------------------------------------------------------------------------------
#define RATED_V_VAL	22000			// 单位:V,额定电压,保存2位小数
#define RATED_I_VAL	5000			// 单位:A,额定电流,保存3位小数
#define RATED_P_VAL	11000			// 单位:W,额定有功,保存1位小数,无功保存位数一样
//---------------------------------------------------------------------------------
#define PI 3.14159265358979323846
#define SQRT3 1.7320508075688772935274463415059	// SQRT(3)
#define SQRT3_1 0.57735026918962576450914878050196	// 1/SQRT(3)
#define SQRT2 1.4142135623730950488016887242097	// SQRT(2)
#define SQRT2_1 0.70710678118654752440084436210485	// 1/SQRT(2)
#define SQRT3_Integer 1732	// SQRT(3)
#define SQRT3_1_Integer 577	// 1/SQRT(3)
//---------------------------------------------------------------------------------
//取绝对值
#define kabs(f)  ((f) < 0 ? (-(f)) : (f))

//---------------------------------------------------------------------------------
// 相位表示
// 三相表示
typedef enum
{
	PH_A = 0,
	PH_B = 1,
	PH_C = 2,
	PH_TH = 3,
	PH_ALL = 4,
	PH_INVALID = DATA_INVALID,
	PH_TOTAL = 0x11,   // 校准所有相位 只用于校准
}emPHASETH;

// 三线表示
typedef enum
{
	PH_AB = 0,
	PH_BC = 1,
	PH_CA = 2,
	PH_THALL = 3,
}emPHASETHALL;

// 行表示
typedef enum
{
	Line_R1 = 0,
	Line_R2 = 1,
	Line_R3 = 2,
	Line_R41 = 3,
	Line_R42 = 4,
 	Line_R5 = 5,

}tpyLine5;

// 第4行 列表示
typedef enum
{
	R4_L0 = 0,
	R4_L1 = 1,
	R4_L2 = 2,
	R4_L3 = 3,
	R4_L4 = 4,
	R4_L5 = 5,
	R4_L6 = 6,
	R4_L7 = 7,
}tpyRow4;

__packed typedef struct
{
	unsigned bit0:1;
	unsigned bit1:1;
	unsigned bit2:1;
	unsigned bit3:1;
	unsigned bit4:1;
	unsigned bit5:1;
	unsigned bit6:1;
	unsigned bit7:1;
}bitfield;


__packed typedef struct
{
	unsigned bit0:1;
	unsigned bit1:1;
	unsigned bit2:1;
	unsigned bit3:1;
	unsigned bit4:1;
	unsigned bit5:1;
	unsigned bit6:1;
	unsigned bit7:1;
	unsigned bit8:1;
	unsigned bit9:1;
	unsigned bit10:1;
	unsigned bit11:1;
	unsigned bit12:1;
	unsigned bit13:1;
	unsigned bit14:1;
	unsigned bit15:1;
} bitfield16;

__packed typedef struct
{
	unsigned bit0:1;
	unsigned bit1:1;
	unsigned bit2:1;
	unsigned bit3:1;
	unsigned bit4:1;
	unsigned bit5:1;
	unsigned bit6:1;
	unsigned bit7:1;
	unsigned bit8:1;
	unsigned bit9:1;
	unsigned bit10:1;
	unsigned bit11:1;
	unsigned bit12:1;
	unsigned bit13:1;
	unsigned bit14:1;
	unsigned bit15:1;
	unsigned bit16:1;
	unsigned bit17:1;
	unsigned bit18:1;
	unsigned bit19:1;
	unsigned bit20:1;
	unsigned bit21:1;
	unsigned bit22:1;
	unsigned bit23:1;
	unsigned bit24:1;
	unsigned bit25:1;
	unsigned bit26:1;
	unsigned bit27:1;
	unsigned bit28:1;
	unsigned bit29:1;
	unsigned bit30:1;
	unsigned bit31:1;
} bitfield32;

// 一个WORD的位声明
__packed struct  WORD_BITS { // bit  description
	u16 bit0:1;	// 0
	u16 bit1:1;	// 1
	u16 bit2:1;	// 2
	u16 bit3:1;	// 3
	u16 bit4:1;	// 4
	u16 bit5:1;	// 5
	u16 bit6:1;	// 6
	u16 bit7:1;	// 7
	u16 bit8:1;	// 8
	u16 bit9:1;	// 9
	u16 bit10:1;// 10
	u16 bit11:1;// 11
	u16 bit12:1;// 12
	u16 bit13:1;// 13
	u16 bit14:1;// 14
	u16 bit15:1;// 15
};

__packed union WORD_REG
{
	u16				  all;
	struct WORD_BITS  bit;
};

// 一个BYTE的位声明
__packed struct  BYTE_BITS {	// bit  description
	u8 bit0:1;
	u8 bit1:1;
	u8 bit2:1;
	u8 bit3:1;
	u8 bit4:1;
	u8 bit5:1;
	u8 bit6:1;
	u8 bit7:1;
};
__packed union BYTE_REG
{
	u8					all;
	struct  BYTE_BITS	bit;
};

#define countof(a)   (sizeof(a) / sizeof(*(a)))

#define PHASE_Add(phase)    (phase = ((phase+1) > (PH_C)) ? PH_A : (phase + 1))
#define absk(f)  ((f) < 0 ? (-(f)) : (f))

#endif	// _MACRO_H


