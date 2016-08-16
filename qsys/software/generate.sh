#!/bin/sh
#
# AUTHOR:       soreilly
# DATA:         05/12/2011
# DESCRIPTION:  A script that contains a bunch of app and bsp switches that will generate the makefiles,
#		compile the code, download the code, open a terminal.
#
#		The first parameter is the philosopher number cpu subsystem to target.
#               Pass in a jtag cable number as the second parameter if you have multiple jtag programming cables.
#
# 		Usage:      ./philosopher.sh <philosopher number> <optional cable number>
#               Example:    ./philosopher.sh 2
#               Tip: Type 'jtagconfig' to list all available jtag cables .

# Check the number of arguments passed into the script
SHOW_HELP="0"
if [ $# -lt 2 ]
then
	SHOW_HELP="1"
else
	COMMAND=$1
	CPU_ID=$2
	CABLE_NUMBER="1"
	if [[ $COMMAND != "all" && $COMMAND != "generate-bsp" && $COMMAND != "generate-makefile" && $COMMAND != "make" && $COMMAND != "download" && $COMMAND != "open-terminal" ]]
	then
		SHOW_HELP="1"
	fi
	if [ $# -eq 3 ]
	then
		CABLE_NUMBER=$3
	else
		if [ $# -gt 3 ]
		then
  			SHOW_HELP="1"
  		fi
	fi
fi


if [ "$SHOW_HELP" = "1" ]
then
	echo ""
	echo "Usage:    ./generate.sh <command> <CPU number> [<cable number>]"
	echo "Example:  ./generate.sh all 2"
	echo ""
	echo "Description:  A script that contains a bunch of app and bsp switches that will generate"
	echo "the makefiles, compile the code, download the code, open a terminal."
	echo ""
	echo "Command can be one of 'all', 'generate-bsp', 'generate-makefile', 'make', 'download', or 'open-terminal'."
	echo ""
	echo "Pass in a jtag cable number if you have multiple jtag programming cables."
	echo ""
	echo "Tip: Type 'jtagconfig -n' to see all available jtag cables."
	exit 1
fi

if [ $CPU_ID -gt 5 ]
then
	echo "Please specify a CPU number between 0 and 4."
	exit 2
fi

# Name of elf file
ELF_NAME=test_code$CPU_ID.elf

# JDI file name
JDI_FILE=../../anxious_st.jdi

# Derive Nios II CPU and jtag uart Qsys component names for 1st
# parameter specified philosopher subsystem number
# PHILOSOPHER_DOWNLOAD_CPU_NAME is a special case.  nios2-download
# requires a philosopher cpu name specified without the sub-system
# name pre-pended.  For this reason, each Nios II CPU component
# in your Qsys design must have a unique cpu name.

PHILOSOPHER_CPU_NAME="nios2_gen2_"$CPU_ID
PHILOSOPHER_DOWNLOAD_CPU_NAME=$PHILOSOPHER_CPU_NAME
PHILOSOPHER_JTAG_UART_NAME="jtag_uart_"$CPU_ID

# cpu and jtag uart instance numbers
CPU_INSTANCE_NUMBER=$(($CPU_ID))
JTAG_UART_INSTANCE_NUMBER=$(($CPU_ID))

# These are the two folders where all the files are dumped
APP_DIR="render_"$CPU_ID
BSP_DIR="render_bsp_"$CPU_ID

# Source dir
SRC_DIR="./src"

echo "Philosopher number is " $CPU_ID
echo "Philosopher cpu name is " $PHILOSOPHER_CPU_NAME
echo "Philosopher jtag uart name is " $PHILOSOPHER_JTAG_UART_NAME
echo "CPU instance number is " $CPU_INSTANCE_NUMBER
echo "Jtag_Uart instance number is " $JTAG_UART_INSTANCE_NUMBER
echo "Application directory is " $APP_DIR
echo "BSP directory is " $BSP_DIR
echo "ELF name is " $ELF_NAME

# BSP generation configuration

SOPC_DIR=../anxious_qsys.sopcinfo
OPTIMIZATION_LEVEL="-O2"
MEMORY_NAME=onchip_memory2_0
SIMULATION_OPTIMIZED_SUPPORT="false"
BSP_TYPE=hal
BSP_FLAGS="--set hal.max_file_descriptors 4 \
--cpu-name $PHILOSOPHER_CPU_NAME \
--set hal.stdin $PHILOSOPHER_JTAG_UART_NAME \
--set hal.stdout $PHILOSOPHER_JTAG_UART_NAME \
--set hal.stderr $PHILOSOPHER_JTAG_UART_NAME \
--set hal.sys_clk_timer none \
--set hal.timestamp_timer none \
--set hal.enable_exit true \
--set hal.enable_c_plus_plus false \
--set hal.enable_clean_exit true \
--set hal.enable_reduced_device_drivers true \
--set hal.enable_lightweight_device_driver_api true \
--set hal.enable_small_c_library true \
--set hal.enable_sim_optimize $SIMULATION_OPTIMIZED_SUPPORT \
--set hal.make.bsp_cflags_optimization $OPTIMIZATION_LEVEL \
--default_sections_mapping $MEMORY_NAME"


# ******** DON'T NEED TO MODIFY ANYTHING BELOW THIS LINE **************

if [[ "$COMMAND" = "all" || "$COMMAND" = "generate-bsp" ]]
then
	# make sure the application and bsp directories are blown away first before attempting to regenerate new projects
	rm -rf $APP_DIR
	rm -rf $BSP_DIR

	mkdir $BSP_DIR

	# generate the BSP in the $BSP_DIR
	generate_bsp_cmd="nios2-bsp $BSP_TYPE $BSP_DIR $SOPC_DIR $BSP_FLAGS"
	$generate_bsp_cmd || {
	  echo "nios2-bsp failed"
	  exit 1
	}
fi


if [[ "$COMMAND" = "all" || "$COMMAND" = "generate-makefile" ]]
then
	# generate the application in the $APP_DIR
	generate_makefile_cmd="nios2-app-generate-makefile --app-dir $APP_DIR --bsp-dir $BSP_DIR --elf-name $ELF_NAME --src-rdir $SRC_DIR --set APP_CFLAGS_OPTIMIZATION $OPTIMIZATION_LEVEL --set CFLAGS \"-DCPU_ID=$CPU_ID\""
	$generate_makefile_cmd || {
		echo "nios2-app-generate-makefile failed"
		exit 1
	}
fi


if [[ "$COMMAND" = "all" || "$COMMAND" = "make" ]]
then
	# Running make (for application and the bsp due to dependencies)
	make_cmd="make --directory=$APP_DIR"
	echo "Executing" $cmd
	$make_cmd || {
	    echo "make failed"
	    exit 1
	}
fi

# Downloading the code
#
# PHILOSOPHER_DOWNLOAD_CPU_NAME is a special case.  nios2-download --cpu_name
# switch requires a philosopher cpu name specified without the sub-system
# name pre-pended.  For this reason, each Nios II CPU component
# in your Qsys design must have a unique cpu name. For example, For cpu
# name philosopher_zero_cpu_zero, the name to use for nios2-download
# is cpu_zero.
#
# Alternatively, --instance can be used instead of --cpu-name switch;
# however, use of --cpu-name does not require Quartus II derived
# instance number assignments.  --instance does use the Quartus II
# derived instance numbers.  --jdi switch is required when using
# --cpu-name switch.  Note that nios2-terminal requires --instance,
# and does not support --cpu-name switch.  The following cmd
# assignment shows nios2-download with --instance instead of --cpu-name.
#
# cmd="nios2-download -g -r --instance $CPU_INSTANCE_NUMBER \
#      --cable $CABLE_NUMBER $APP_DIR/$ELF_NAME"
#
# In the multiprocessor_tutorial HW design, Quartus II and Qsys happen to assign
# Philosopher cpu instance and philosopher jtag_uart instances set
# to 1 higher than the philosopher number.

if [[ "$COMMAND" = "all" || "$COMMAND" = "download" ]]
then
	#cmd="nios2-download -g -r --jdi $SOPC_DIR/$JDI_FILE --cpu_name $PHILOSOPHER_DOWNLOAD_CPU_NAME --cable $CABLE_NUMBER $APP_DIR/$ELF_NAME"
	download_cmd="nios2-download -g -r --instance $CPU_INSTANCE_NUMBER --cable $CABLE_NUMBER $APP_DIR/$ELF_NAME"
	retrys=5
	for ((i = 0; i < 5; ++i))
	do
		echo "Executing" $open_terminal_cmd
		$download_cmd || {
			echo "failed to download the software .elf file"
		    continue
		}
		exit 1
	done
fi


if [[ "$COMMAND" = "all" || "$COMMAND" = "open-terminal" ]]
then
	# Opening terminal connection
	open_terminal_cmd="nios2-terminal --cable=$CABLE_NUMBER --instance=$JTAG_UART_INSTANCE_NUMBER"
	retrys=5
	for ((i = 0; i < 5; ++i))
	do
		echo "Executing" $open_terminal_cmd
		$open_terminal_cmd || {
		    echo "failed to open the Nios II terminal"
		    continue
		}
		exit 1
	done
fi

#*******************************************************************************
#*                                                                             *
#* License Agreement                                                           *
#*                                                                             *
#* Copyright (c) 2011 Altera Corporation, San Jose, California, USA.           *
#* All rights reserved.                                                        *
#*                                                                             *
#* Permission is hereby granted, free of charge, to any person obtaining a     *
#* copy of this software and associated documentation files (the "Software"),  *
#* to deal in the Software without restriction, including without limitation   *
#* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
#* and/or sell copies of the Software, and to permit persons to whom the       *
#* Software is furnished to do so, subject to the following conditions:        *
#*                                                                             *
#* The above copyright notice and this permission notice shall be included in  *
#* all copies or substantial portions of the Software.                         *
#*                                                                             *
#* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
#* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
#* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
#* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
#* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
#* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
#* DEALINGS IN THE SOFTWARE.                                                   *
#*                                                                             *
#* This agreement shall be governed in all respects by the laws of the State   *
#* of California and by the laws of the United States of America.              *
#* Altera does not recommend, suggest or require that this reference design    *
#* file be used in conjunction or combination with any other product.          *
#*******************************************************************************

