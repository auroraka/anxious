source msim_setup.tcl
dev_com
com
alias sim_prep {
	vlog -sv anxious_up_tb.sv
	set TOP_LEVEL_NAME anxious_up_tb
	elab
	do wave.do
	run 200us
}
sim_prep