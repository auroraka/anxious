module ssram_test_top
`include "./common.v"

wire clk_qsys;

cam_pll U_cam_pll (
	.inclk0 (CLOCK_50),
	.c0     (),
	.c1     (clk_qsys),
	.c2     (SSRAM_CLK),
	.c3     ()
);

// ssram_test u0 (
	// .clk_clk                                        (CLOCK_50),                                        //                    clk.clk
	// .reset_reset_n                                  (KEY[0]),                                  //                  reset.reset_n
//
	// .ssram_mm_0_ssram_wires_tcm_chipselect_n_out    (),                // ssram_mm_0_ssram_wires.tcm_chipselect_n_out
	// .ssram_mm_0_ssram_wires_tcm_byteenable_n_out    (SSRAM_BE),        //                       .tcm_byteenable_n_out
	// .ssram_mm_0_ssram_wires_tcm_outputenable_n_out  (SSRAM_OE_N),      //                       .tcm_outputenable_n_out
	// .ssram_mm_0_ssram_wires_tcm_write_n_out         (SSRAM_WE_N),      //                       .tcm_write_n_out
	// .ssram_mm_0_ssram_wires_tcm_data_out            (FS_DQ),           //                       .tcm_data_out
	// .ssram_mm_0_ssram_wires_tcm_address_out         (FS_ADDR[21:2]),   //                       .tcm_address_out
	// .ssram_mm_0_ssram_wires_tcm_reset_n_out         (),                //                       .tcm_reset_n_out
	// .ssram_mm_0_ssram_wires_tcm_begintransfer_n_out (SSRAM_ADSC_N)     //                       .tcm_begintransfer_n_out
// );


// My_First_Nios_DE2i_150_QSYS u0 (
	// .clk_clk                              (CLOCK_50),                              //                        clk.clk
	// .flash_tri_state_bridge_out_fs_data   (FS_DQ[15:0]),   // flash_tri_state_bridge_out.fs_data
	// .flash_tri_state_bridge_out_fl_we_n   (FL_WE_N),   //                           .fl_we_n
	// .flash_tri_state_bridge_out_fl_read_n (FL_OE_N), //                           .fl_read_n
	// .flash_tri_state_bridge_out_fs_addr   (FS_ADDR),   //                           .fs_addr
	// .flash_tri_state_bridge_out_fl_cs_n   (FL_CE_N),   //                           .fl_cs_n
	// .led_export                           (LED),                           //                        led.export
	// .reset_reset_n                        (1'b1)                         //                      reset.reset_n
// );


// ssram_test u0 (
	// .clk_clk          (CLOCK_50),    //      clk.clk
	// .reset_reset_n    (KEY[0]),      //    reset.reset_n
	// .fl_wires_fs_data (FS_DQ[15:0]), // fl_wires.fs_data
	// .fl_wires_fl_we_n (FL_WE_N),     //         .fl_we_n
	// .fl_wires_fl_oe_n (FL_OE_N),     //         .fl_oe_n
	// .fl_wires_fs_addr (FS_ADDR),     //         .fs_addr
	// .fl_wires_fl_ce_n (FL_CE_N)      //         .fl_ce_n
// );

// ssram_test u0 (
	// .clk_clk                       (CLOCK_50),     //      clk.clk
	// .fl_wires_sram_we_n            (SSRAM_WE_N),   // fl_wires.sram_we_n
	// .fl_wires_fl_we_n              (FL_WE_N),      //         .fl_we_n
	// .fl_wires_sram_oe_n            (SSRAM_OE_N),   //         .sram_oe_n
	// .fl_wires_sram_begintransfer_n (SSRAM_ADSC_N), //         .sram_begintransfer_n
	// .fl_wires_fl_oe_n              (FL_OE_N),      //         .fl_oe_n
	// .fl_wires_sram_ce_n            (),             //         .sram_ce_n
	// .fl_wires_fl_ce_n              (FL_CE_N),      //         .fl_ce_n
	// .fl_wires_sram_be_n            (SSRAM_BE),     //         .sram_be_n
	// .fl_wires_fs_data              (FS_DQ),        //         .fs_data
	// .fl_wires_sram_reset_n         (),             //         .sram_reset_n
	// .fl_wires_fs_addr              (FS_ADDR),      //         .fs_addr
	// .reset_reset_n                 (KEY[0])        //    reset.reset_n
// );

fs_mm_test u0 (
	.clk_clk                       (clk_qsys),     //      clk.clk
	.reset_reset_n                 (KEY[0]),       //    reset.reset_n
	.fs_wires_sram_we_n            (SSRAM_WE_N),   // fs_wires.sram_we_n
	.fs_wires_fl_we_n              (FL_WE_N),      //         .fl_we_n
	.fs_wires_sram_oe_n            (SSRAM_OE_N),   //         .sram_oe_n
	.fs_wires_sram_begintransfer_n (SSRAM_ADSC_N), //         .sram_begintransfer_n
	.fs_wires_fl_oe_n              (FL_OE_N),      //         .fl_oe_n
	.fs_wires_sram_ce_n            (),             //         .sram_ce_n
	.fs_wires_fl_ce_n              (FL_CE_N),      //         .fl_ce_n
	.fs_wires_sram_be_n            (SSRAM_BE),     //         .sram_be_n
	.fs_wires_fs_data              (FS_DQ),        //         .fs_data
	.fs_wires_sram_reset_n         (),             //         .sram_reset_n
	.fs_wires_fs_addr              (FS_ADDR)       //         .fs_addr
);


endmodule
