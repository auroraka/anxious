//=======================================================
//	This code is generated by Terasic System Builder
//=======================================================

module anxious_capture_top (

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

//////////// GPIO, GPIO connect to GPIO Default //////////
inout			[0:35]		GPIO;

//////////// Fan Control //////////
inout						FAN_CTRL;


//=======================================================
//	REG/WIRE declarations
//=======================================================




//=======================================================
//	Structural coding
//=======================================================



	//////////// FAN Control //////////
assign FAN_CTRL = SW[0]; // turn off FAN

wire clk_camera;
wire clk_qsys;
wire clk_vga;

cam_pll U_cam_pll (
	.inclk0 (CLOCK_50),
	.c0     (clk_camera),
	.c1     (clk_qsys),
	.c2     (DRAM_CLK),
	.c3     (clk_vga)
);


`define CAM_SIOC_0 GPIO[20]
`define CAM_SIOD_0 GPIO[21]
`define CAM_VSYNC_0 GPIO[22]
`define CAM_HREF_0 GPIO[23]
`define CAM_PCLK_0 GPIO[24]
`define CAM_XCLK_0 GPIO[25]
`define CAM_DIN_0 GPIO[26:33] //D7:D0
`define CAM_RESET_0 GPIO[34]
`define CAM_PWDN_0 GPIO[35]

`define CAM_SIOC_1 GPIO[2]
`define CAM_SIOD_1 GPIO[3]
`define CAM_VSYNC_1 GPIO[4]
`define CAM_HREF_1 GPIO[5]
`define CAM_PCLK_1 GPIO[6]
`define CAM_XCLK_1 GPIO[7]
`define CAM_DIN_1 GPIO[8:15]
`define CAM_RESET_1 GPIO[16]
`define CAM_PWDN_1 GPIO[17]


reg [1:0] recog_buffer_port_0, recog_buffer_port_1;
reg recog_buffer_vsync_0, recog_buffer_vsync_1;

`define RENDER_CORES 1
reg [1:0] render_buffer_port;
reg [`RENDER_CORES-1:0] render_vsync;
reg render_vsync_all, render_vsync_any;
assign render_vsync_all = render_vsync[0]; // and
assign render_vsync_any = render_vsync[0]; // or
reg render_start, render_start_r, render_start_rin;

always @ (posedge clk_qsys) begin
	render_start_r <= render_start_rin;
end

always begin
	render_start = render_start_r;
	if (render_vsync_all == 1'b1) begin
		render_start = 1'b1;
	end else if (render_vsync_any == 1'b0) begin
		render_start = 1'b0;
	end
	render_start_rin <= render_start;
end

wire render_buffer_vsync;
assign render_buffer_vsync = render_vsync_all;

anxious_capture u0 (
    .cam_buffer_switcher_enable_enable                  (SW[4]),
    .camera_mm_0_conduit_clk_camera                     (clk_camera),
    .camera_mm_0_conduit_enable_n                       (SW[3]),
    .camera_mm_0_conduit_cam_din                        (`CAM_DIN_0),//(SW[7] ? `CAM_DIN_1 : `CAM_DIN_0),
    .camera_mm_0_conduit_cam_href                       (`CAM_HREF_0),//(SW[7] ? `CAM_HREF_1 : `CAM_HREF_0),
    .camera_mm_0_conduit_cam_pclk                       (`CAM_PCLK_0),//(SW[7] ? `CAM_PCLK_1 : `CAM_PCLK_0),
    // .camera_mm_0_conduit_cam_pwdn                       (`CAM_PWDN_0),
    // .camera_mm_0_conduit_cam_reset                      (`CAM_RESET_0),
    .camera_mm_0_conduit_cam_vsync                      (`CAM_VSYNC_0),//(SW[7] ? `CAM_VSYNC_1 : `CAM_VSYNC_0),
    // .camera_mm_0_conduit_cam_xclk                       (`CAM_XCLK_0),

    .camera_sioc_0_external_connection_export           (`CAM_SIOC_0),
    .camera_sioc_1_external_connection_export           (`CAM_SIOC_1),
    .camera_siod_0_external_connection_export           (`CAM_SIOD_0),
    .camera_siod_1_external_connection_export           (`CAM_SIOD_1),

    .camera_st_0_conduit_cam_din                        (`CAM_DIN_0),
    .camera_st_0_conduit_cam_href                       (`CAM_HREF_0),
    .camera_st_0_conduit_cam_pclk                       (`CAM_PCLK_0),
    .camera_st_0_conduit_cam_pwdn                       (`CAM_PWDN_0),
    .camera_st_0_conduit_cam_reset                      (`CAM_RESET_0),
    .camera_st_0_conduit_cam_vsync                      (`CAM_VSYNC_0),
    .camera_st_0_conduit_cam_xclk                       (`CAM_XCLK_0),
    .camera_st_0_conduit_clk_camera                     (clk_camera),
    .camera_st_0_conduit_enable_n                       (SW[1]),

    .camera_st_1_conduit_cam_din                        (`CAM_DIN_1),
    .camera_st_1_conduit_cam_href                       (`CAM_HREF_1),
    .camera_st_1_conduit_cam_pclk                       (`CAM_PCLK_1),
    .camera_st_1_conduit_cam_pwdn                       (`CAM_PWDN_1),
    .camera_st_1_conduit_cam_reset                      (`CAM_RESET_1),
    .camera_st_1_conduit_cam_vsync                      (`CAM_VSYNC_1),
    .camera_st_1_conduit_cam_xclk                       (`CAM_XCLK_1),
    .camera_st_1_conduit_clk_camera                     (clk_camera),
    .camera_st_1_conduit_enable_n                       (SW[2]),

    .clk_clk                                            (clk_qsys),

    .ir_receiver_0_ir_clock_clk                         (CLOCK_50),
    .ir_receiver_0_irda_IRDA                            (IRDA_RXD),

    .key_pio_external_connection_export                 (KEY[1]),

    .new_sdram_controller_0_wire_addr                   (DRAM_ADDR),
    .new_sdram_controller_0_wire_ba                     (DRAM_BA),
    .new_sdram_controller_0_wire_cas_n                  (DRAM_CAS_N),
    .new_sdram_controller_0_wire_cke                    (DRAM_CKE),
    .new_sdram_controller_0_wire_cs_n                   (DRAM_CS_N),
    .new_sdram_controller_0_wire_dq                     (DRAM_DQ),
    .new_sdram_controller_0_wire_dqm                    (DRAM_DQM),
    .new_sdram_controller_0_wire_ras_n                  (DRAM_RAS_N),
    .new_sdram_controller_0_wire_we_n                   (DRAM_WE_N),

    .recog_buffer_port_pio_0_external_connection_export (recog_buffer_port_0),
    .recog_buffer_port_pio_1_external_connection_export (recog_buffer_port_1),
    .recog_buffer_switcher_0_enable_enable              (SW[5]),
    .recog_buffer_switcher_0_read_buffer_buffer_port    (recog_buffer_port_0),
    .recog_buffer_switcher_0_read_buffer_buffer_vsync   (recog_buffer_vsync_0),
    .recog_buffer_switcher_1_enable_enable              (SW[5]),
    .recog_buffer_switcher_1_read_buffer_buffer_port    (recog_buffer_port_1),
    .recog_buffer_switcher_1_read_buffer_buffer_vsync   (recog_buffer_vsync_1),
    .recog_vsync_pio_0_external_connection_export       (recog_buffer_vsync_0),
    .recog_vsync_pio_1_external_connection_export       (recog_buffer_vsync_1),
    .render_buffer_switcher_enable_enable               (SW[6]),
    .render_buffer_switcher_write_buffer_buffer_port    (render_buffer_port),
    .render_buffer_switcher_write_buffer_buffer_vsync   (render_buffer_vsync),
    .render_port_pio_external_connection_export         (render_buffer_port),
    .render_start_pio_external_connection_export        (render_start_r),
    .render_vsync_pio_0_external_connection_export      (render_vsync[0]),

    .reset_reset_n                                      (KEY[0]),

    .vga_composer_0_conduit_vga_b               (VGA_B),
    .vga_composer_0_conduit_vga_blank_n         (VGA_BLANK_N),
    .vga_composer_0_conduit_vga_clk             (VGA_CLK),
    .vga_composer_0_conduit_vga_g               (VGA_G),
    .vga_composer_0_conduit_vga_hs              (VGA_HS),
    .vga_composer_0_conduit_vga_r               (VGA_R),
    .vga_composer_0_conduit_vga_sync_n          (VGA_SYNC_N),
    .vga_composer_0_conduit_vga_vs              (VGA_VS),
    .vga_composer_0_conduit_clk_vga             (clk_vga),

    // .vga_composer_overlay_0_conduit_vga_b               (VGA_B),
    // .vga_composer_overlay_0_conduit_vga_blank_n         (VGA_BLANK_N),
    // .vga_composer_overlay_0_conduit_vga_clk             (VGA_CLK),
    // .vga_composer_overlay_0_conduit_vga_g               (VGA_G),
    // .vga_composer_overlay_0_conduit_vga_hs              (VGA_HS),
    // .vga_composer_overlay_0_conduit_vga_r               (VGA_R),
    // .vga_composer_overlay_0_conduit_vga_sync_n          (VGA_SYNC_N),
    // .vga_composer_overlay_0_conduit_vga_vs              (VGA_VS),
    // .vga_composer_overlay_0_conduit_clk_vga             (clk_vga),
);

endmodule
