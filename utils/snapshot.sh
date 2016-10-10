#!/usr/bin/env sh

if [ -z "$1" ]; then
	of="output.jpg"
else
	of=$1
fi

system-console --rc-script=tcl/jtag_cmds_sc.tcl --script=tcl/snapshot.tcl
dump2img/dump2img camera.dump $of
