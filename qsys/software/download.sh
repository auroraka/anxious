if [ $# -eq 2 ]
then
	CPU_NUMBER=$1
	CABLE_NUMBER=$2
else
	if [ $# -eq 1 ]
	then
		CPU_NUMBER=$1
		CABLE_NUMBER="1"
	else
		echo ""
		echo "Usage:    ./download.sh <CPU number> [<cable number>]"
		echo "Example:  ./download.sh 2"
		echo ""
		echo "The first parameter is the CPU number cpu subsystem to target."
		echo ""
		echo "Pass in a jtag cable number as the second parameter if you have multiple jtag programming cables."
		echo ""
		echo "Tip: Type 'jtagconfig -n' to see all available jtag cables."
		exit 1
	fi
fi

cmd="nios2-download -g -r --instance $CPU_INSTANCE_NUMBER --cable $CABLE_NUMBER $APP_DIR/$ELF_NAME"
echo "Executing" $cmd
$cmd || {
	echo "failed to download the software .elf file"
    exit 1
}
