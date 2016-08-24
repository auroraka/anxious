module anxious_fb_top

`include "./common.v"

`include "./camera_gpio_defs.v"

assign FAN_CTRL = SW[0];

wire clk_camera, clk_qsys, clk_vga;

cam_pll U_cam_pll (
	.inclk0 (CLOCK_50),
	.c0     (clk_camera),
	.c1     (clk_qsys),
	.c2     (DRAM_CLK),
	.c3     (clk_vga)
);

anxious_fb u0 (
	.camera_sioc_external_connection_export (`CAM_SIOC_0),  // camera_sioc_external_connection.export
	.camera_siod_external_connection_export (`CAM_SIOD_0),  // camera_siod_external_connection.export

	.camera_st_0_conduit_cam_din            (`CAM_DIN_0),   //             camera_st_0_conduit.cam_din
	.camera_st_0_conduit_cam_href           (`CAM_HREF_0),  //                                .cam_href
	.camera_st_0_conduit_cam_pclk           (`CAM_PCLK_0),  //                                .cam_pclk
	.camera_st_0_conduit_cam_pwdn           (`CAM_PWDN_0),  //                                .cam_pwdn
	.camera_st_0_conduit_cam_reset          (`CAM_RESET_0), //                                .cam_reset
	.camera_st_0_conduit_cam_vsync          (`CAM_VSYNC_0), //                                .cam_vsync
	.camera_st_0_conduit_cam_xclk           (`CAM_XCLK_0),  //                                .cam_xclk
	.camera_st_0_conduit_clk_camera         (clk_camera),   //                                .clk_camera
	.camera_st_0_conduit_enable_n           (SW[1]),        //                                .enable_n

	.clk_clk                                (clk_qsys),     //                             clk.clk

	.frame_buffer_switcher_0_enable_enable  (SW[2]),        //  frame_buffer_switcher_0_enable.enable

	.new_sdram_controller_0_wire_addr       (DRAM_ADDR),    //     new_sdram_controller_0_wire.addr
	.new_sdram_controller_0_wire_ba         (DRAM_BA),      //                                .ba
	.new_sdram_controller_0_wire_cas_n      (DRAM_CAS_N),   //                                .cas_n
	.new_sdram_controller_0_wire_cke        (DRAM_CKE),     //                                .cke
	.new_sdram_controller_0_wire_cs_n       (DRAM_CS_N),    //                                .cs_n
	.new_sdram_controller_0_wire_dq         (DRAM_DQ),      //                                .dq
	.new_sdram_controller_0_wire_dqm        (DRAM_DQM),     //                                .dqm
	.new_sdram_controller_0_wire_ras_n      (DRAM_RAS_N),   //                                .ras_n
	.new_sdram_controller_0_wire_we_n       (DRAM_WE_N),    //                                .we_n

	.reset_reset_n                          (SW[0]),        //                           reset.reset_n

	.vga_mm_0_conduit_vga_b                 (VGA_B),        //                vga_mm_0_conduit.vga_b
	.vga_mm_0_conduit_vga_blank_n           (VGA_BLANK_N),  //                                .vga_blank_n
	.vga_mm_0_conduit_vga_clk               (VGA_CLK),      //                                .vga_clk
	.vga_mm_0_conduit_vga_g                 (VGA_G),        //                                .vga_g
	.vga_mm_0_conduit_vga_hs                (VGA_HS),       //                                .vga_hs
	.vga_mm_0_conduit_vga_r                 (VGA_R),        //                                .vga_r
	.vga_mm_0_conduit_vga_sync_n            (VGA_SYNC_N),   //                                .vga_sync_n
	.vga_mm_0_conduit_vga_vs                (VGA_VS),       //                                .vga_vs
	.vga_mm_0_conduit_clk_vga               (clk_vga),      //                                .clk_vga
);

endmodule
