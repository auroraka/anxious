source msim_setup.tcl
dev_com
com
alias sim_prep {
	vlog -sv mm_lock_sim_tb.sv
	set TOP_LEVEL_NAME mm_lock_sim_tb
	elab
	do wave.do
	run 200us
}
sim_prep
