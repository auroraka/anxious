module anxious_overlay_top
`include "./common.v"
`include "./camera_gpio_defs.v"

assign FAN_CTRL = SW[0];

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

assign SSRAM_CLK = DRAM_CLK;

wire [1:0] overlay_buffer_port;
wire overlay_buffer_vsync;

anxious_overlay u0 (
	.bank0_0_vga_vsync_s_vsync                      (VGA_VS),        //         bank0_0_vga_vsync.s_vsync

	.clk_clk                                        (clk_qsys),      //                       clk.clk
	.clk_vga_clk                                    (clk_vga),       //                   clk_vga.clk

	.reset_reset_n                                  (KEY[0]),        //                     reset.reset_n

	.vga_st_composer_0_conduit_vga_b                (VGA_B),         // vga_st_composer_0_conduit.vga_b
	.vga_st_composer_0_conduit_vga_blank_n          (VGA_BLANK_N),   //                          .vga_blank_n
	.vga_st_composer_0_conduit_vga_clk              (VGA_CLK),       //                          .vga_clk
	.vga_st_composer_0_conduit_vga_g                (VGA_G),         //                          .vga_g
	.vga_st_composer_0_conduit_vga_hs               (VGA_HS),        //                          .vga_hs
	.vga_st_composer_0_conduit_vga_r                (VGA_R),         //                          .vga_r
	.vga_st_composer_0_conduit_vga_sync_n           (VGA_SYNC_N),    //                          .vga_sync_n
	.vga_st_composer_0_conduit_vga_vs               (VGA_VS),        //                          .vga_vs

	.vid_read_buffer_0_vsync_s_vsync                (VGA_VS),        //   vid_read_buffer_0_vsync.s_vsync

	.sdram_controller_0_wire_addr                   (DRAM_ADDR),     //   sdram_controller_0_wire.addr
	.sdram_controller_0_wire_ba                     (DRAM_BA),       //                          .ba
	.sdram_controller_0_wire_cas_n                  (DRAM_CAS_N),    //                          .cas_n
	.sdram_controller_0_wire_cke                    (DRAM_CKE),      //                          .cke
	.sdram_controller_0_wire_cs_n                   (DRAM_CS_N),     //                          .cs_n
	.sdram_controller_0_wire_dq                     (DRAM_DQ),       //                          .dq
	.sdram_controller_0_wire_dqm                    (DRAM_DQM),      //                          .dqm
	.sdram_controller_0_wire_ras_n                  (DRAM_RAS_N),    //                          .ras_n
	.sdram_controller_0_wire_we_n                   (DRAM_WE_N),     //                          .we_n

	.ssram_mm_0_ssram_wires_tcm_chipselect_n_out    (),              //    ssram_mm_0_ssram_wires.tcm_chipselect_n_out
	.ssram_mm_0_ssram_wires_tcm_byteenable_n_out    (SSRAM_BE),      //                          .tcm_byteenable_n_out
	.ssram_mm_0_ssram_wires_tcm_outputenable_n_out  (SSRAM_OE_N),    //                          .tcm_outputenable_n_out
	.ssram_mm_0_ssram_wires_tcm_write_n_out         (SSRAM_WE_N),    //                          .tcm_write_n_out
	.ssram_mm_0_ssram_wires_tcm_data_out            (FS_DQ),         //                          .tcm_data_out
	.ssram_mm_0_ssram_wires_tcm_address_out         (FS_ADDR[21:2]), //                          .tcm_address_out
	.ssram_mm_0_ssram_wires_tcm_reset_n_out         (),              //                          .tcm_reset_n_out
	.ssram_mm_0_ssram_wires_tcm_begintransfer_n_out (SSRAM_ADSC_N),  //                          .tcm_begintransfer_n_out

	.overlay_buffer_switcher_write_buffer_buffer_port  (overlay_buffer_port),  //     overlay_buffer_switcher_write_buffer.buffer_port
	.overlay_buffer_switcher_write_buffer_buffer_vsync (overlay_buffer_vsync), //                                         .buffer_vsync
	.overlay_buffer_switcher_enable_enable             (SW[1]),             //           overlay_buffer_switcher_enable.enable
	.overlay_buffer_port_external_connection_export    (overlay_buffer_port),    //  overlay_buffer_port_external_connection.export
	.overlay_buffer_vsync_external_connection_export   (overlay_buffer_vsync)    // overlay_buffer_vsync_external_connection.export
);

endmodule
