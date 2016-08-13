source msim_setup.tcl
dev_com
com
alias sim_prep {
	vlog -sv anxious_qsys_tb.sv
	set TOP_LEVEL_NAME anxious_qsys_tb
	elab
	do wave.do
	run 200us
}
sim_prep