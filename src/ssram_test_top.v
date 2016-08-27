module ssram_test_top
`include "./common.v"

ssram_test u0 (
	.clk_clk                                        (CLOCK_50),                                        //                    clk.clk
	.reset_reset_n                                  (KEY[0]),                                  //                  reset.reset_n

	.ssram_mm_0_ssram_wires_tcm_chipselect_n_out    (),                // ssram_mm_0_ssram_wires.tcm_chipselect_n_out
	.ssram_mm_0_ssram_wires_tcm_byteenable_n_out    (SSRAM_BE),        //                       .tcm_byteenable_n_out
	.ssram_mm_0_ssram_wires_tcm_outputenable_n_out  (SSRAM_OE_N),      //                       .tcm_outputenable_n_out
	.ssram_mm_0_ssram_wires_tcm_write_n_out         (SSRAM_WE_N),      //                       .tcm_write_n_out
	.ssram_mm_0_ssram_wires_tcm_data_out            (FS_DQ),           //                       .tcm_data_out
	.ssram_mm_0_ssram_wires_tcm_address_out         (FS_ADDR[21:2]),   //                       .tcm_address_out
	.ssram_mm_0_ssram_wires_tcm_reset_n_out         (),                //                       .tcm_reset_n_out
	.ssram_mm_0_ssram_wires_tcm_begintransfer_n_out (SSRAM_ADSC_N)     //                       .tcm_begintransfer_n_out
);

endmodule
