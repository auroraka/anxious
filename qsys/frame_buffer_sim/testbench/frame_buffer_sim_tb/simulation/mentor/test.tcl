source msim_setup.tcl
dev_com
com
alias sim_prep {
	vlog -sv frame_buffer_sim_tb.sv
	set TOP_LEVEL_NAME frame_buffer_sim_tb
	elab
	do wave.do
	run 200us
}
sim_prep