(
	//////////// CLOCK //////////
	CLOCK_50,	// BANK 4
	CLOCK2_50,	// BANK 7
	CLOCK3_50,	// BANK 3A

	//////////// LED (High Active) //////////
	LEDG,
	LEDR,

	//////////// KEY (Active Low) //////////
	KEY,

	//////////// SW //////////
	SW,

	//////////// SEG7 (Low Active) //////////
	HEX0,
	HEX1,
	HEX2,
	HEX3,
	HEX4,
	HEX5,
	HEX6,
	HEX7,

	//////////// LCD //////////
	LCD_DATA,
	LCD_EN,
	LCD_ON,
	LCD_RS,
	LCD_RW,

	//////////// SDCARD //////////
	SD_CLK,
	SD_CMD,
	SD_DAT,
	SD_WP_N,

	//////////// VGA //////////
	VGA_B,
	VGA_BLANK_N,
	VGA_CLK,
	VGA_G,
	VGA_HS,
	VGA_R,
	VGA_SYNC_N,
	VGA_VS,

	//////////// IR Receiver //////////
	IRDA_RXD,

	//////////// SDRAM //////////
	DRAM_ADDR,
	DRAM_BA,
	DRAM_CAS_N,
	DRAM_CKE,
	DRAM_CLK,
	DRAM_CS_N,
	DRAM_DQ,
	DRAM_DQM,
	DRAM_RAS_N,
	DRAM_WE_N,

	//////////// SSRAM //////////
	SSRAM_ADSC_N,
	SSRAM_ADSP_N,
	SSRAM_ADV_N,
	SSRAM_BE,
	SSRAM_CLK,
	SSRAM_GW_N,
	SSRAM_OE_N,
	SSRAM_WE_N,
	SSRAM0_CE_N,
	SSRAM1_CE_N,

	//////////// Data and Address bus shared by Flash & SSRAM //////////
	FS_ADDR,
	FS_DQ,

	//////////// GPIO, GPIO connect to GPIO Default //////////
	GPIO,

	//////////// Fan Control //////////
	FAN_CTRL
);

//=======================================================
//	PARAMETER declarations
//=======================================================


//=======================================================
//	PORT declarations
//=======================================================

//////////// CLOCK //////////
input						CLOCK_50;
input						CLOCK2_50;
input						CLOCK3_50;

//////////// LED (High Active) //////////
output			 [8:0]		LEDG;
output			[17:0]		LEDR;

//////////// KEY (Active Low) //////////
input			 [3:0]		KEY;

//////////// SW //////////
input			[17:0]		SW;

//////////// SEG7 (Low Active) //////////
output			 [6:0]		HEX0;
output			 [6:0]		HEX1;
output			 [6:0]		HEX2;
output			 [6:0]		HEX3;
output			 [6:0]		HEX4;
output			 [6:0]		HEX5;
output			 [6:0]		HEX6;
output			 [6:0]		HEX7;

//////////// LCD //////////
inout			 [7:0]		LCD_DATA;
output						LCD_EN;
output						LCD_ON;
output						LCD_RS;
output						LCD_RW;

//////////// SDCARD //////////
output						SD_CLK;
inout						SD_CMD;
inout			 [3:0]		SD_DAT;
input						SD_WP_N;

//////////// VGA //////////
output			 [7:0]		VGA_B;
output						VGA_BLANK_N;
output						VGA_CLK;
output			 [7:0]		VGA_G;
output						VGA_HS;
output			 [7:0]		VGA_R;
output						VGA_SYNC_N;
output						VGA_VS;

//////////// IR Receiver //////////
input						IRDA_RXD;

//////////// SDRAM //////////
output			[12:0]		DRAM_ADDR;
output			 [1:0]		DRAM_BA;
output						DRAM_CAS_N;
output						DRAM_CKE;
output						DRAM_CLK;
output						DRAM_CS_N;
inout			[31:0]		DRAM_DQ;
output			 [3:0]		DRAM_DQM;
output						DRAM_RAS_N;
output						DRAM_WE_N;

//////////// SSRAM //////////
output		          		SSRAM_ADSC_N;
output		          		SSRAM_ADSP_N;
output		          		SSRAM_ADV_N;
output		     [3:0]		SSRAM_BE;
output		          		SSRAM_CLK;
output		          		SSRAM_GW_N;
output		          		SSRAM_OE_N;
output		          		SSRAM_WE_N;
output		          		SSRAM0_CE_N;
output		          		SSRAM1_CE_N;

//////////// Data and Address bus shared by Flash & SSRAM //////////
output		    [26:1]		FS_ADDR;
inout 		    [31:0]		FS_DQ;

//////////// GPIO, GPIO connect to GPIO Default //////////
inout			[0:35]		GPIO;

//////////// Fan Control //////////
inout						FAN_CTRL;

// SSRAM Fixed Signals
assign SSRAM_CLK = CLOCK_50;
assign SSRAM0_CE_N = 1;
assign SSRAM1_CE_N = 0;
assign SSRAM_ADV_N = 1;
assign SSRAM_ADSP_N = 1;
assign SSRAM_GW_N = 1;
