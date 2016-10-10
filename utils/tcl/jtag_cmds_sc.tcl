# -----------------------------------------------------------------
# jtag_cmds_sc.tcl
#
# 2/20/2012 D. W. Hawkins (dwh@ovro.caltech.edu)
#
# JTAG-to-Avalon-MM tutorial SystemConsole commands for the DE2.
#
#  Address    Device
# ---------  --------
#  0x0000     8-bit LEDs (8-bits of both the green and red LEDs)
#  0x0010     8-bits of switches
#  0x1000     4kB of on-chip SRAM
#
# -----------------------------------------------------------------

# =================================================================
# Master access
# =================================================================
#
# -----------------------------------------------------------------
# Open the JTAG master service
# -----------------------------------------------------------------

# Open the first Avalon-MM master service
proc jtag_open {} {
	global jtag

	# Close any open service
	if {[info exists jtag(master)]} {
		jtag_close
	}

	set master_paths [get_service_paths master]
	if {[llength $master_paths] == 0} {
		puts "Sorry, no master nodes found"
		return
	}

	# Select the first master service
	set jtag(master) [lindex $master_paths 0]

	open_service master $jtag(master)
	return
}

# -----------------------------------------------------------------
# Close the JTAG master service
# -----------------------------------------------------------------
#
proc jtag_close {} {
	global jtag

	if {[info exists jtag(master)]} {
		close_service master $jtag(master)
		unset jtag(master)
	}
	return
}
