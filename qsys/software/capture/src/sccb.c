
#if CPU_ID < 2
 
#include "sccb.h"

#include <stdio.h>
#include <alt_types.h>
#include <unistd.h>
#include <altera_avalon_pio_regs.h>
#include <io.h>

#include "CameraOV7670RegisterDefinitions.h"

#define SCCB_CLK 0x08042020
#define SCCB_DATA 0x08042030

#define SDA_DIR_IN() IOWR_ALTERA_AVALON_PIO_DIRECTION(SCCB_DATA, 0)
#define SDA_DIR_OUT() IOWR_ALTERA_AVALON_PIO_DIRECTION(SCCB_DATA, 1)
#define SDA_WRITE(data) IOWR_ALTERA_AVALON_PIO_DATA(SCCB_DATA, data)
#define SDA_READ() IORD_ALTERA_AVALON_PIO_DATA(SCCB_DATA)
#define SCL_WRITE(data) IOWR_ALTERA_AVALON_PIO_DATA(SCCB_CLK, data)

#define SCCB_ID 0x42
#define DELAY() usleep(100)

void SCCB_Init() {
	SDA_DIR_OUT();
	SCL_WRITE(0);
	SDA_WRITE(1);
}

//SCCB起始信号
//当时钟为高的时候,数据线的高到低,为SCCB起始信号
//在激活状态下,SDA和SCL均为低电平
void SCCB_Start(void) {
	SDA_WRITE(1);     //数据线高电平
	DELAY();
	
	SCL_WRITE(1);        //在时钟线高的时候数据线由高至低
	DELAY();
	
	SDA_WRITE(0);
	DELAY();
	
	SCL_WRITE(0);        //数据线恢复低电平，单操作函数必要
	DELAY();
}

//SCCB停止信号
//当时钟为高的时候,数据线的低到高,为SCCB停止信号
//空闲状况下,SDA,SCL均为高电平
void SCCB_Stop(void) {
	SDA_WRITE(0);
	DELAY();
	
	SCL_WRITE(1);
	DELAY();
	
	SDA_WRITE(1);
	DELAY();
}

//产生NA信号
void SCCB_No_Ack(void) {
	DELAY();
	SDA_WRITE(1);
	SCL_WRITE(1);
	DELAY();
	SCL_WRITE(0);
	DELAY();
	SDA_WRITE(0);
	DELAY();
}

//SCCB,写入一个字节
//返回值:0,成功;1,失败.
alt_u8 SCCB_WR_Byte(alt_u8 dat) {
	alt_u8 j, res;
	for (j = 0; j < 8; j++) //循环8次发送数据
	{
		if (dat & 0x80) {
			SDA_WRITE(1);
		} else {
			SDA_WRITE(0);
		}
		DELAY();
		dat <<= 1;
		SCL_WRITE(1);
		DELAY();
		SCL_WRITE(0);
		DELAY();
	}
	
	SDA_DIR_IN();        //设置SDA为输入
	DELAY();
	
	SCL_WRITE(1);            //接收第九位,以判断是否发送成功
	DELAY();
	
	if (SDA_READ()) {    //SDA=1发送失败，返回1
		res = 1;
	} else {    //SDA=0发送成功，返回0
		res = 0;
	}
	DELAY();
	
	SCL_WRITE(0);
	DELAY();
	SDA_DIR_OUT();        //设置SDA为输出
	DELAY();
	//printf("write %x %s.\n",dat,res?"failed":"success");
	return res;
}

//SCCB 读取一个字节
//在SCL的上升沿,数据锁存
//返回值:读到的数据
alt_u8 SCCB_RD_Byte(void) {
	alt_u8 temp = 0, j;
	SDA_DIR_IN();    //设置SDA为输入
	DELAY();
	for (j = 8; j > 0; j--)    //循环8次接收数据
	{
		SCL_WRITE(1);
		DELAY();
		temp = temp << 1;
		if (SDA_READ()) {
			temp++;
		}
		DELAY();
		SCL_WRITE(0);
		DELAY();
	}
	SDA_DIR_OUT();        //设置SDA为输出
	DELAY();
	return temp;
}

//写寄存器
//返回值:0,成功;1,失败.
alt_u8 SCCB_WR_Reg(alt_u8 reg, alt_u8 data) {
	SCCB_Start();                    //启动SCCB传输
	if (SCCB_WR_Byte(SCCB_ID)) {//写器件ID
		SCCB_Stop();
		printf("write device id failed\n");
		return 1;
	}
	//printf("write device id success\n");
	DELAY();
	
	if (SCCB_WR_Byte(reg)) {//写寄存器地址
		SCCB_Stop();
		printf("write addr failed\n");
		return 1;
	}
	//printf("write addr success\n");
	DELAY();
	
	if (SCCB_WR_Byte(data)) {//写数据
		SCCB_Stop();
		printf("write data failed\n");
		return 1;
	}
	//printf("write data success");
	DELAY();
	
	SCCB_Stop();
	
	printf("write %x %x\n", reg, data);
	usleep(2000);
	return 0;
}

//读寄存器
//返回值:读到的寄存器值
alt_u8 SCCB_RD_Reg(alt_u8 reg) {
	alt_u8 val = 0;
	SCCB_Start();                //启动SCCB传输
	if (SCCB_WR_Byte(SCCB_ID)) {//写器件ID
		SCCB_Stop();
		printf("write device id failed\n");
		return 1;
	}
	//printf("write device id success\n");
	DELAY();
	
	if (SCCB_WR_Byte(reg)) {//写寄存器地址
		SCCB_Stop();
		printf("write addr failed\n");
		return 1;
	}
	//printf("write addr success\n");
	DELAY();
	
	SCCB_Stop();
	DELAY();
	
	//设置寄存器地址后，才是读
	SCCB_Start();
	DELAY();
	if (SCCB_WR_Byte(SCCB_ID | 0X01)) {    //发送读命令
		SCCB_Stop();
		printf("send read addr failed.\n");
	}
	//printf("send read addr success.\n");
	DELAY();
	val = SCCB_RD_Byte();            //读取数据
	DELAY();
	SCCB_No_Ack();
	DELAY();
	SCCB_Stop();
	printf("read: %x %x\n", reg, val);
	
	usleep(1000);
	return val;
}

alt_u8 OV7670_Init(void) {
	alt_u8 temp;
	alt_u16 i = 0;
	SCCB_Init();                //初始化SCCB 的IO口
	if (SCCB_WR_Reg(0x12, 0x80)) {//复位SCCB
		return 1;
	}
	DELAY();
	//读取产品型号
	temp = SCCB_RD_Reg(0x0b);
	printf("%x\n", temp);
	//if(temp!=0x73)return 2;
	temp = SCCB_RD_Reg(0x0a);
	printf("%x\n", temp);
	//if(temp!=0x76)return 2;
//	//初始化序列
//	for(i=0;i<sizeof(ov7670_init_reg_tbl)/sizeof(ov7670_init_reg_tbl[0]);i++)
//	{
//	   	SCCB_WR_Reg(ov7670_init_reg_tbl[i][0],ov7670_init_reg_tbl[i][1]);
//		DELAY()_ms(2);
// 	}
	return 0x00;    //ok
}

const alt_u8 ov7670_init_reg_tbl[][2] =
		{
				/*以下为OV7670 QVGA RGB565参数  */
				//	{0x3a, 0x04},
//	{0x40, 0x10},
//	{0x12, 0x14},//QVGA,RGB输出

//	//输出窗口设置
//	{0x32, 0x80},
//	{0x17, 0x16},
//	{0x18, 0x04},//5
//	{0x19, 0x02},
//	{0x1a, 0x7a},//0x7a,
// 	{0x03, 0x0a},//0x0a,
//
//
//	{0x0c, 0x0c},
//	{0x15, 0x00},
//	{0x3e, 0x00},//10
//	{0x70, 0x00},
//	{0x71, 0x01},
//	{0x72, 0x11},
//	{0x73, 0x09},//
//
//	{0xa2, 0x02},//15
//	{0x11, 0x00},//时钟分频设置,0,不分频.
//	{0x7a, 0x20},
//	{0x7b, 0x1c},
//	{0x7c, 0x28},
//
//	{0x7d, 0x3c},//20
//	{0x7e, 0x55},
//	{0x7f, 0x68},
//	{0x80, 0x76},
//	{0x81, 0x80},
//
//	{0x82, 0x88},
//	{0x83, 0x8f},
//	{0x84, 0x96},
//	{0x85, 0xa3},
//	{0x86, 0xaf},
//
//	{0x87, 0xc4},//30
//	{0x88, 0xd7},
//	{0x89, 0xe8},
//	{0x13, 0xe0},
//	{0x00, 0x00},//AGC
//
//	{0x10, 0x00},
//	{0x0d, 0x00},
//	{0x14, 0x20},//0x38, limit the max gain
//	{0xa5, 0x05},
//	{0xab, 0x07},
//
//	{0x24, 0x75},//40
//	{0x25, 0x63},
//	{0x26, 0xA5},
//	{0x9f, 0x78},
//	{0xa0, 0x68},
//
//	{0xa1, 0x03},//0x0b,
//	{0xa6, 0xdf},//0xd8,
//	{0xa7, 0xdf},//0xd8,
//	{0xa8, 0xf0},
//	{0xa9, 0x90},
//
//	{0xaa, 0x94},//50
//	{0x13, 0xe5},
//	{0x0e, 0x61},
//	{0x0f, 0x4b},
//	{0x16, 0x02},
//
//	{0x1e, 0x27},//图像输出镜像控制.0x07,
//	{0x21, 0x02},
//	{0x22, 0x91},
//	{0x29, 0x07},
//	{0x33, 0x0b},
//
//	{0x35, 0x0b},//60
//	{0x37, 0x1d},
//	{0x38, 0x71},
//	{0x39, 0x2a},
//	{0x3c, 0x78},
//
//	{0x4d, 0x40},
//	{0x4e, 0x20},
//	{0x69, 0x5d},
//	{0x6b, 0x40},//PLL*4=48Mhz
//	{0x74, 0x19},
//	{0x8d, 0x4f},
//
//	{0x8e, 0x00},//70
//	{0x8f, 0x00},
//	{0x90, 0x00},
//	{0x91, 0x00},
//	{0x92, 0x00},//0x19,//0x66
//
//	{0x96, 0x00},
//	{0x9a, 0x80},
//	{0xb0, 0x84},
//	{0xb1, 0x0c},
//	{0xb2, 0x0e},
//
//	{0xb3, 0x82},//80
//	{0xb8, 0x0a},
//	{0x43, 0x14},
//	{0x44, 0xf0},
//	{0x45, 0x34},
//
//	{0x46, 0x58},
//	{0x47, 0x28},
//	{0x48, 0x3a},
//	{0x59, 0x88},
//	{0x5a, 0x88},
//
//	{0x5b, 0x44},//90
//	{0x5c, 0x67},
//	{0x5d, 0x49},
//	{0x5e, 0x0e},
//	{0x64, 0x04},
//	{0x65, 0x20},
//
//	{0x66, 0x05},
//	{0x94, 0x04},
//	{0x95, 0x08},
//	{0x6c, 0x0a},
//	{0x6d, 0x55},
//
//
//	{0x4f, 0x80},
//	{0x50, 0x80},
//	{0x51, 0x00},
//	{0x52, 0x22},
//	{0x53, 0x5e},
//	{0x54, 0x80},
//
//	//{0x54, 0x40},//110
//
//
//	{0x09, 0x03},//驱动能力最大
				
				//{0x6e, 0x11},//100
				//{0x6f, 0x9f},//0x9e for advance AWB
				//{0x55, 0x00},//亮度
				//{0x56, 0x40},//对比度
				//{0x57, 0x80},//0x40,  change according to Jim's request
		};

// =================================================

#include <stdio.h>
#include <math.h>

//const unsigned kXjbRegList[][2] = {
//		{REG_TSLB,  0x14},
//		{REG_COM13, 0x88},
//		{REG_COM7,  0x00},
//		{REG_COM5,  0x61},
//		{REG_COM6,  0x4b},
//		{0x16,      0x02},
//		{REG_MVFP,  0x07},
//		{0x21,      0x02},
//		{0x22,      0x91},
//		{0x29,      0x07},
//		{0x33,      0x0b},
//		{0x35,      0x0b},
//		{0x37,      0x1d},
//		{0x38,      0x71},
//		{0x39,      0x2a},
//		{REG_COM12, 0x78},
//		{0x4d,      0x40},
//		{0x4e,      0x20},
//		{REG_GFIX,  0},
//		{0x6b,      0x4a},
//		{0x74,      0x10},
//		{0x8d,      0x4f},
//		{0x8e,      0},
//		{0x8f,      0},
//		{0x90,      0},
//		{0x91,      0},
//		{0x96,      0},
//		{0x9a,      0},
//		{0xb0,      0x84},
//		{0xb1,      0x0c},
//		{0xb2,      0x0e},
//		{0xb3,      0x82},
//		{0xb8,      0x0a},
//		{0xFF,      0xFF} // End marker
//};

const unsigned regsDefault[][2] = { //from the linux driver
		/* {REG_COM7, COM7_RESET}, */
		{REG_TSLB,                  0x04},    /* OV */
		{REG_COM7,                  0},        /* VGA */
		/*
		 * Set the hardware window.  These values from OV don't entirely
		 * make sense - hstop is less than hstart.  But they work...
		 */
		{REG_HSTART,                0x13},
		{REG_HSTOP,                 0x01},
		{REG_HREF,                  0xb6},
		{REG_VSTART,                0x02},
		{REG_VSTOP,                 0x7a},
		{REG_VREF,                  0x0a},
		
		{REG_COM3,                  0},
		{REG_COM14,                 0},
		
		/* Mystery scaling numbers */
		{0x70,                      0x3a},
		{0x71,                      0x35},
		{0x72,                      0x11},
		{0x73,                      0xf0},
		{0xa2,/* 0x02 changed to 1*/1},
		
		/* {REG_COM10, COM10_VS_NEG}, */
		/* Gamma curve values */
		{0x7a,                      0x20},
		{0x7b,                      0x10},
		{0x7c,                      0x1e},
		{0x7d,                      0x35},
		{0x7e,                      0x5a},
		{0x7f,                      0x69},
		{0x80,                      0x76},
		{0x81,                      0x80},
		{0x82,                      0x88},
		{0x83,                      0x8f},
		{0x84,                      0x96},
		{0x85,                      0xa3},
		{0x86,                      0xaf},
		{0x87,                      0xc4},
		{0x88,                      0xd7},
		{0x89,                      0xe8},
		/* AGC and AEC parameters.  Note we start by disabling those features,
		   then turn them only after tweaking the values. */
		{REG_COM8,  COM8_FASTAEC | COM8_AECSTEP},
		{REG_GAIN,                  0},
		{REG_AECH,                  0},
		{REG_COM4,                  0x40}, /* magic reserved bit */
		{REG_COM9,                  0x18}, /* 4x gain + magic rsvd bit */
		{REG_BD50MAX,               0x05},
		{REG_BD60MAX,               0x07},
		{REG_AEW,                   0x95},
		{REG_AEB,                   0x33},
		{REG_VPT,                   0xe3},
		{REG_HAECC1,                0x78},
		{REG_HAECC2,                0x68},
		{0xa1,                      0x03}, /* magic */
		{REG_HAECC3,                0xd8},
		{REG_HAECC4,                0xd8},
		{REG_HAECC5,                0xf0},
		{REG_HAECC6,                0x90},
		{REG_HAECC7,                0x94},
		{REG_COM8,  COM8_FASTAEC | COM8_AECSTEP | COM8_AGC | COM8_AEC},
		{0x30,                      0},
		{0x31,                      0},//disable some delays
		/* Almost all of these are magic "reserved" values.  */
		{REG_COM5,                  0x61},
		{REG_COM6,                  0x4b},
		{0x16,                      0x02},
		{REG_MVFP,                  0x07},
		{0x21,                      0x02},
		{0x22,                      0x91},
		{0x29,                      0x07},
		{0x33,                      0x0b},
		{0x35,                      0x0b},
		{0x37,                      0x1d},
		{0x38,                      0x71},
		{0x39,                      0x2a},
		{REG_COM12,                 0x78},
		{0x4d,                      0x40},
		{0x4e,                      0x20},
		{REG_GFIX,                  0},
		/*{0x6b, 0x4a},*/
		{0x74,                      0x10},
		{0x8d,                      0x4f},
		{0x8e,                      0},
		{0x8f,                      0},
		{0x90,                      0},
		{0x91,                      0},
		{0x96,                      0},
		{0x9a,                      0},
		{0xb0,                      0x84},
		{0xb1,                      0x0c},
		{0xb2,                      0x0e},
		{0xb3,                      0x82},
		{0xb8,                      0x0a},
		
		/* More reserved magic, some of which tweaks white balance */
		{0x43,                      0x0a},
		{0x44,                      0xf0},
		{0x45,                      0x34},
		{0x46,                      0x58},
		{0x47,                      0x28},
		{0x48,                      0x3a},
		{0x59,                      0x88},
		{0x5a,                      0x88},
		{0x5b,                      0x44},
		{0x5c,                      0x67},
		{0x5d,                      0x49},
		{0x5e,                      0x0e},
		{0x6c,                      0x0a},
		{0x6d,                      0x55},
		{0x6e,                      0x11},
		{0x6f,                      0x9e}, /* it was 0x9F "9e for advance AWB" */
		{0x6a,                      0x40},
		{REG_BLUE,                  0x40},
		{REG_RED,                   0x60},
		{REG_COM8,  COM8_FASTAEC | COM8_AECSTEP | COM8_AGC | COM8_AEC | COM8_AWB},
		
		/* Matrix coefficients */
		{0x4f,                      0x80},
		{0x50,                      0x80},
		{0x51,                      0},
		{0x52,                      0x22},
		{0x53,                      0x5e},
		{0x54,                      0x80},
		{0x58,                      0x9e},
		
		{REG_COM16, COM16_AWBGAIN},
		{REG_EDGE,                  0},
		{0x75,                      0x05},
		{REG_REG76,                 0xe1},
		{0x4c,                      0},
		{0x77,                      0x01},
		{REG_COM13, /*0xc3*/        0x48},
		{0x4b,                      0x09},
		{0xc9,                      0x60},        /*{REG_COM16, 0x38},*/
		{0x56,                      0x40},
		
		{0x34,                      0x11},
		{REG_COM11, COM11_EXP | COM11_HZAUTO},
		{0xa4,                      0x82/*Was 0x88*/},
		{0x96,                      0},
		{0x97,                      0x30},
		{0x98,                      0x20},
		{0x99,                      0x30},
		{0x9a,                      0x84},
		{0x9b,                      0x29},
		{0x9c,                      0x03},
		{0x9d,                      0x4c},
		{0x9e,                      0x3f},
		{0x78,                      0x04},
		
		/* Extra-weird stuff.  Some sort of multiplexor register */
		{0x79,                      0x01},
		{0xc8,                      0xf0},
		{0x79,                      0x0f},
		{0xc8,                      0x00},
		{0x79,                      0x10},
		{0xc8,                      0x7e},
		{0x79,                      0x0a},
		{0xc8,                      0x80},
		{0x79,                      0x0b},
		{0xc8,                      0x01},
		{0x79,                      0x0c},
		{0xc8,                      0x0f},
		{0x79,                      0x0d},
		{0xc8,                      0x20},
		{0x79,                      0x09},
		{0xc8,                      0x80},
		{0x79,                      0x02},
		{0xc8,                      0xc0},
		{0x79,                      0x03},
		{0xc8,                      0x40},
		{0x79,                      0x05},
		{0xc8,                      0x30},
		{0x79,                      0x26},
		
		{REG_MVFP,                  0x27},  /* flip horizontally */
		
		{0xff,                      0xff},    /* END MARKER */
};

const unsigned regsRGB565[][2] = {
		{REG_COM7,                  COM7_RGB}, /* Selects RGB mode */
		{REG_RGB444, 0},      /* No RGB444 please */
		{REG_COM1,   0x0},
		{REG_COM15,                 COM15_RGB565 | COM15_R00FF},
		{REG_COM9,   0x6A},     /* 128x gain ceiling; 0x8 is reserved bit */
		{0x4f,       0xb3},         /* "matrix coefficient 1" */
		{0x50,       0xb3},         /* "matrix coefficient 2" */
		{0x51,       0},         /* vb */
		{0x52,       0x3d},         /* "matrix coefficient 4" */
		{0x53,       0xa7},         /* "matrix coefficient 5" */
		{0x54,       0xe4},         /* "matrix coefficient 6" */
		{REG_COM13, /*COM13_GAMMA|*/COM13_UVSAT},
		{0xff,       0xff}    /* END MARKER */
};

//const unsigned regsYUV422[][2] = {
//		{REG_COM7,   0x0},    /* Selects YUV mode */
//		{REG_RGB444, 0},    /* No RGB444 please */
//		{REG_COM1,   0},
//		{REG_COM15,                COM15_R00FF},
//		{REG_COM9,   0x6A},    /* 128x gain ceiling; 0x8 is reserved bit */
//		{0x4f,       0x80},        /* "matrix coefficient 1" */
//		{0x50,       0x80},        /* "matrix coefficient 2" */
//		{0x51,       0},        /* vb */
//		{0x52,       0x22},        /* "matrix coefficient 4" */
//		{0x53,       0x5e},        /* "matrix coefficient 5" */
//		{0x54,       0x80},        /* "matrix coefficient 6" */
//		{REG_COM13,/*COM13_GAMMA|*/COM13_UVSAT},
//		{0xff,       0xff},
//};


const unsigned regsVGA[][2] = {
		{REG_HREF, 0xF6},    // was B6
		{0x17,     0x13},        // HSTART
		{0x18,     0x01},        // HSTOP
		{0x19,     0x02},        // VSTART
		{0x1a,     0x7a},        // VSTOP
		{REG_VREF, 0x0a},    // VREF
		{0xff,     0xff},        /* END MARKER */
};

//const unsigned regsQVGA[][2] = {
//		{REG_COM3, COM3_DCWEN}, // enable downsamp/crop/window
//
//		{REG_COM14,  0x19},
//		{0x72,       0x11},
//		{0x73,       0xf1},
//		{REG_HSTART, 0x16},
//		{REG_HSTOP,  0x04},
//		{REG_HREF,   0x24},
//		{REG_VSTART, 0x02},
//		{REG_VSTOP,  0x7a},
//		{REG_VREF,   0x0a},
//		{0xff,       0xff},    /* END MARKER */
//};
//
//const unsigned regsMartin[][2] = {
//		//QQVGA RGB444
//		{REG_CLKRC,  0x80},
//		{REG_COM11,  0x0A},
//		{REG_TSLB,   0x04},
//		{REG_COM7,   0x04},
//
//		//{REG_RGB444, 0x02},
//		//{REG_COM15, 0xd0},
//		{REG_RGB444, 0x00},     // Disable RGB 444?
//		{REG_COM15,  0xD0},      // Set RGB 565?
//
//		{REG_HSTART, 0x16},
//		{REG_HSTOP,  0x04},
//		{REG_HREF,   0x24},
//		{REG_VSTART, 0x02},
//		{REG_VSTOP,  0x7a},
//		{REG_VREF,   0x0a},
//		{REG_COM10,  0x02},
//		{REG_COM3,   0x04},
//		{REG_COM14,  0x1a},
//		{REG_MVFP,   0x27},
//		{0x72,       0x22},
//		{0x73,       0xf2},
//
//		// COLOR SETTING
//		{0x4f,       0x80},
//		{0x50,       0x80},
//		{0x51,       0x00},
//		{0x52,       0x22},
//		{0x53,       0x5e},
//		{0x54,       0x80},
//		{0x56,       0x40},
//		{0x58,       0x9e},
//		{0x59,       0x88},
//		{0x5a,       0x88},
//		{0x5b,       0x44},
//		{0x5c,       0x67},
//		{0x5d,       0x49},
//		{0x5e,       0x0e},
//		{0x69,       0x00},
//		{0x6a,       0x40},
//		{0x6b,       0x0a},
//		{0x6c,       0x0a},
//		{0x6d,       0x55},
//		{0x6e,       0x11},
//		{0x6f,       0x9f},
//
//		{0xb0,       0x84},
//		{0xFF,       0xFF}, // End Marker
//};

void sccb_write_reg_list(const unsigned (*regs)[2]) {
	int i;
	for (i = 0;; ++i) {
		const unsigned *reg_val = regs[i];
		unsigned reg = reg_val[0];
		unsigned val = reg_val[1];
		if (reg == 0xFF && val == 0xFF) {
			break;
		} else {
			SCCB_WR_Reg(reg, val);
		}
	}
	printf("%s: %d regs written.\n", __FUNCTION__, i);
}

void configure_sccb() {
	printf("Configuring camera through SCCB...\n");
	
	SCCB_WR_Reg(REG_COM7, 0x80); // set COM7[7] => Reset
	usleep(1000 * 1000 * 3);
	
	sccb_write_reg_list(regsDefault);
	/* sccb_write_reg_list(regsYUV422); */
	/* sccb_write_reg_list(kXjbRegList); */
	sccb_write_reg_list(regsRGB565);
	/* sccb_write_reg_list(regsQVGA); */
	sccb_write_reg_list(regsVGA);
	/* sccb_write_reg_list(regsMartin); */
}

#endif