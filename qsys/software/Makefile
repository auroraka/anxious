ifndef CONFIG
  $(error set variable CONFIG to your config file. See example.config.)
endif
ifeq ($(wildcard $(CONFIG)),)
  $(error $(CONFIG) not found.)
endif
include $(CONFIG)

# Name of elf file
ELF_NAME ?= $(PROJECT)_$(CPU_ID).elf

# Name of flash file
FLASH_NAME ?= $(PROJECT)_$(CPU_ID).flash

# JDI file name
JDI_FILE ?= $(QUARTUS_PROJECT_ROOT)/$(QUARTUS_PROJECT_NAME).jdi

# Source dir
SRC_DIR ?= $(PROJECT)/src

# Derive Nios II CPU and jtag uart Qsys component names for 1st
# parameter specified philosopher subsystem number
# PHILOSOPHER_DOWNLOAD_CPU_NAME is a special case.  nios2-download
# requires a philosopher cpu name specified without the sub-system
# name pre-pended.  For this reason, each Nios II CPU component
# in your Qsys design must have a unique cpu name.

PHILOSOPHER_CPU_NAME := nios2_gen2_$(CPU_ID)
PHILOSOPHER_DOWNLOAD_CPU_NAME := $(PHILOSOPHER_CPU_NAME)
PHILOSOPHER_JTAG_UART_NAME := jtag_uart_$(CPU_ID)

# cpu and jtag uart instance numbers
CPU_INSTANCE_NUMBER ?= $(CPU_ID)
JTAG_UART_INSTANCE_NUMBER ?= $(CPU_ID)

# These are the two folders where all the files are dumped
APP_DIR := $(PROJECT)/generated_app_$(CPU_ID)
BSP_DIR := $(PROJECT)/generated_bsp_$(CPU_ID)

# BSP generation configuration

OPTIMIZATION_LEVEL ?= -O2
MEMORY_NAME ?= onchip_memory2_0
SIMULATION_OPTIMIZED_SUPPORT := false
BSP_TYPE := hal
BSP_FLAGS := --set hal.max_file_descriptors 4 \
	--cpu-name $(PHILOSOPHER_CPU_NAME) \
	--set hal.stdin $(PHILOSOPHER_JTAG_UART_NAME) \
	--set hal.stdout $(PHILOSOPHER_JTAG_UART_NAME) \
	--set hal.stderr $(PHILOSOPHER_JTAG_UART_NAME) \
	--set hal.sys_clk_timer none \
	--set hal.timestamp_timer none \
	--set hal.enable_exit true \
	--set hal.enable_c_plus_plus false \
	--set hal.enable_clean_exit true \
	--set hal.enable_reduced_device_drivers true \
	--set hal.enable_lightweight_device_driver_api true \
	--set hal.enable_small_c_library true \
	--set hal.enable_sim_optimize $(SIMULATION_OPTIMIZED_SUPPORT) \
	--set hal.make.bsp_cflags_optimization $(OPTIMIZATION_LEVEL) \
	--default_sections_mapping $(MEMORY_NAME)

# ******** DON'T NEED TO MODIFY ANYTHING BELOW THIS LINE **************

.PHONY: all
all: | generate-bsp generate-makefile make download open-terminal

.PHONY: run
run: | download open-terminal

.PHONY: generate-bsp
generate-bsp:
	# make sure the application and bsp directories are blown away first before attempting to regenerate new projects
	-rm -rf $(APP_DIR)
	-rm -rf $(BSP_DIR)

	mkdir -pv $(BSP_DIR)

	# generate the BSP in the $BSP_DIR
	nios2-bsp $(BSP_TYPE) $(BSP_DIR) $(SOPC_DIR) $(BSP_FLAGS)

.PHONY: generate-makefile
generate-makefile:
	# generate the application in the $APP_DIR
	nios2-app-generate-makefile --app-dir $(APP_DIR) --bsp-dir $(BSP_DIR) --elf-name $(ELF_NAME) --src-rdir $(SRC_DIR) --set APP_CFLAGS_OPTIMIZATION $(OPTIMIZATION_LEVEL) --set APP_CFLAGS_USER_FLAGS $(CFLAGS_USER_FLAGS) --set CFLAGS \"-DCPU_ID=$(CPU_ID)\"

.PHONY: make
make:
	# Running make (for application and the bsp due to dependencies)
	$(MAKE) --directory=$(APP_DIR)

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
# instance number assignments.	--instance does use the Quartus II
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

.PHONY: configure
configure:
	-nios2-configure-sof $(QUARTUS_PROJECT_ROOT)/$(QUARTUS_PROJECT_NAME).sof

.PHONY: download
download:
	nios2-download -g -r --instance $(CPU_INSTANCE_NUMBER) --cable $(CABLE_NUMBER) $(APP_DIR)/$(ELF_NAME)

.PHONY: open-terminal
open-terminal:
	nios2-terminal --cable=$(CABLE_NUMBER) --instance=$(JTAG_UART_INSTANCE_NUMBER)

.PHONY: redownload
redownload: | generate-makefile make download open-terminal

.PHONY: flash
flash:
	python flash.py

.PHONY: do_flash
do_flash: | make
	elf2flash --input="$(APP_DIR)/$(ELF_NAME)" --output="$(APP_DIR)/$(FLASH_NAME)" --boot="$(SOPC_KIT_NIOS2)/components/altera_nios2/boot_loader_cfi.srec" --base=$(FLASH_BASE) --end=$(FLASH_END) --reset=$(FLASH_RESET) --verbose
	nios2-flash-programmer "$(APP_DIR)/$(FLASH_NAME)" --base=$(FLASH_BASE) --program --verbose

.PHONY: help
help:
	@echo ""
	@echo "Usage:    ./generate.sh <command> <CPU number> [<cable number>]"
	@echo "Example:  ./generate.sh all 2"
	@echo ""
	@echo "Description:	A script that contains a bunch of app and bsp switches that will generate"
	@echo "the makefiles, compile the code, download the code, open a terminal."
	@echo ""
	@echo "Command can be one of 'all', 'generate-bsp', 'generate-makefile', 'make', 'download', or 'open-terminal'."
	@echo ""
	@echo "Pass in a jtag cable number if you have multiple jtag programming cables."
	@echo ""
	@echo "Tip: Type 'jtagconfig -n' to see all available jtag cables."

print-%  : ; @echo $* = $($*)
