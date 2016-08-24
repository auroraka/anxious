# TCL File Generated by Component Editor 15.0
# Wed Aug 24 17:17:11 CST 2016
# DO NOT MODIFY


# 
# vga_mm "vga_mm" v1.0
#  2016.08.24.17:17:11
# 
# 

# 
# request TCL package from ACDS 15.0
# 
package require -exact qsys 15.0


# 
# module vga_mm
# 
set_module_property DESCRIPTION ""
set_module_property NAME vga_mm
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME vga_mm
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL vga_mm
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file vga_controller.vhd VHDL PATH src/vga_controller.vhd
add_fileset_file ip_fifo.vhd VHDL PATH src/ip_fifo.vhd
add_fileset_file mm_read_buffer.vhd VHDL PATH src/mm_read_buffer.vhd
add_fileset_file NEEE.vhd VHDL PATH src/NEEE.vhd
add_fileset_file vga_mm.vhd VHDL PATH src/vga_mm.vhd TOP_LEVEL_FILE

add_fileset SIM_VERILOG SIM_VERILOG "" ""
set_fileset_property SIM_VERILOG TOP_LEVEL vga_mm
set_fileset_property SIM_VERILOG ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VERILOG ENABLE_FILE_OVERWRITE_MODE true
add_fileset_file vga_controller.vhd VHDL PATH src/vga_controller.vhd
add_fileset_file ip_fifo.vhd VHDL PATH src/ip_fifo.vhd
add_fileset_file mm_read_buffer.vhd VHDL PATH src/mm_read_buffer.vhd
add_fileset_file NEEE.vhd VHDL PATH src/NEEE.vhd
add_fileset_file vga_mm.vhd VHDL PATH src/vga_mm.vhd

add_fileset SIM_VHDL SIM_VHDL "" ""
set_fileset_property SIM_VHDL TOP_LEVEL vga_mm
set_fileset_property SIM_VHDL ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VHDL ENABLE_FILE_OVERWRITE_MODE true
add_fileset_file vga_controller.vhd VHDL PATH src/vga_controller.vhd
add_fileset_file ip_fifo.vhd VHDL PATH src/ip_fifo.vhd
add_fileset_file mm_read_buffer.vhd VHDL PATH src/mm_read_buffer.vhd
add_fileset_file NEEE.vhd VHDL PATH src/NEEE.vhd
add_fileset_file vga_mm.vhd VHDL PATH src/vga_mm.vhd


# 
# parameters
# 
add_parameter DATA_WIDTH POSITIVE 24
set_parameter_property DATA_WIDTH DEFAULT_VALUE 24
set_parameter_property DATA_WIDTH DISPLAY_NAME DATA_WIDTH
set_parameter_property DATA_WIDTH TYPE POSITIVE
set_parameter_property DATA_WIDTH UNITS None
set_parameter_property DATA_WIDTH ALLOWED_RANGES 1:2147483647
set_parameter_property DATA_WIDTH HDL_PARAMETER true
add_parameter FIFO_LENGTH_LOG_2 POSITIVE 6
set_parameter_property FIFO_LENGTH_LOG_2 DEFAULT_VALUE 6
set_parameter_property FIFO_LENGTH_LOG_2 DISPLAY_NAME FIFO_LENGTH_LOG_2
set_parameter_property FIFO_LENGTH_LOG_2 TYPE POSITIVE
set_parameter_property FIFO_LENGTH_LOG_2 UNITS None
set_parameter_property FIFO_LENGTH_LOG_2 ALLOWED_RANGES 1:2147483647
set_parameter_property FIFO_LENGTH_LOG_2 HDL_PARAMETER true
add_parameter ADDR_WIDTH POSITIVE 27
set_parameter_property ADDR_WIDTH DEFAULT_VALUE 27
set_parameter_property ADDR_WIDTH DISPLAY_NAME ADDR_WIDTH
set_parameter_property ADDR_WIDTH TYPE POSITIVE
set_parameter_property ADDR_WIDTH UNITS None
set_parameter_property ADDR_WIDTH ALLOWED_RANGES 1:2147483647
set_parameter_property ADDR_WIDTH HDL_PARAMETER true
add_parameter BANK STD_LOGIC_VECTOR 0
set_parameter_property BANK DEFAULT_VALUE 0
set_parameter_property BANK DISPLAY_NAME BANK
set_parameter_property BANK TYPE STD_LOGIC_VECTOR
set_parameter_property BANK UNITS None
set_parameter_property BANK ALLOWED_RANGES 0:3
set_parameter_property BANK HDL_PARAMETER true
add_parameter FRAME_PIXELS POSITIVE 307200
set_parameter_property FRAME_PIXELS DEFAULT_VALUE 307200
set_parameter_property FRAME_PIXELS DISPLAY_NAME FRAME_PIXELS
set_parameter_property FRAME_PIXELS TYPE POSITIVE
set_parameter_property FRAME_PIXELS UNITS None
set_parameter_property FRAME_PIXELS ALLOWED_RANGES 1:2147483647
set_parameter_property FRAME_PIXELS HDL_PARAMETER true


# 
# display items
# 


# 
# connection point clock
# 
add_interface clock clock end
set_interface_property clock clockRate 0
set_interface_property clock ENABLED true
set_interface_property clock EXPORT_OF ""
set_interface_property clock PORT_NAME_MAP ""
set_interface_property clock CMSIS_SVD_VARIABLES ""
set_interface_property clock SVD_ADDRESS_GROUP ""

add_interface_port clock clk clk Input 1


# 
# connection point reset
# 
add_interface reset reset end
set_interface_property reset associatedClock clock
set_interface_property reset synchronousEdges DEASSERT
set_interface_property reset ENABLED true
set_interface_property reset EXPORT_OF ""
set_interface_property reset PORT_NAME_MAP ""
set_interface_property reset CMSIS_SVD_VARIABLES ""
set_interface_property reset SVD_ADDRESS_GROUP ""

add_interface_port reset reset_n reset_n Input 1


# 
# connection point conduit
# 
add_interface conduit conduit end
set_interface_property conduit associatedClock clock
set_interface_property conduit associatedReset ""
set_interface_property conduit ENABLED true
set_interface_property conduit EXPORT_OF ""
set_interface_property conduit PORT_NAME_MAP ""
set_interface_property conduit CMSIS_SVD_VARIABLES ""
set_interface_property conduit SVD_ADDRESS_GROUP ""

add_interface_port conduit VGA_B vga_b Output 8
add_interface_port conduit VGA_BLANK_N vga_blank_n Output 1
add_interface_port conduit VGA_CLK vga_clk Output 1
add_interface_port conduit VGA_G vga_g Output 8
add_interface_port conduit VGA_HS vga_hs Output 1
add_interface_port conduit VGA_R vga_r Output 8
add_interface_port conduit VGA_SYNC_N vga_sync_n Output 1
add_interface_port conduit VGA_VS vga_vs Output 1
add_interface_port conduit clk_vga clk_vga Input 1


# 
# connection point avalon_master
# 
add_interface avalon_master avalon start
set_interface_property avalon_master addressUnits SYMBOLS
set_interface_property avalon_master associatedClock clock
set_interface_property avalon_master associatedReset reset
set_interface_property avalon_master bitsPerSymbol 8
set_interface_property avalon_master burstOnBurstBoundariesOnly false
set_interface_property avalon_master burstcountUnits WORDS
set_interface_property avalon_master doStreamReads false
set_interface_property avalon_master doStreamWrites false
set_interface_property avalon_master holdTime 0
set_interface_property avalon_master linewrapBursts false
set_interface_property avalon_master maximumPendingReadTransactions 0
set_interface_property avalon_master maximumPendingWriteTransactions 0
set_interface_property avalon_master readLatency 0
set_interface_property avalon_master readWaitTime 1
set_interface_property avalon_master setupTime 0
set_interface_property avalon_master timingUnits Cycles
set_interface_property avalon_master writeWaitTime 0
set_interface_property avalon_master ENABLED true
set_interface_property avalon_master EXPORT_OF ""
set_interface_property avalon_master PORT_NAME_MAP ""
set_interface_property avalon_master CMSIS_SVD_VARIABLES ""
set_interface_property avalon_master SVD_ADDRESS_GROUP ""

add_interface_port avalon_master address address Output addr_width
add_interface_port avalon_master read read Output 1
add_interface_port avalon_master readdata readdata Input data_width
add_interface_port avalon_master waitrequest waitrequest Input 1
add_interface_port avalon_master readdatavalid readdatavalid Input 1
add_interface_port avalon_master burstcount burstcount Output "(fifo_length_log_2) - (0) + 1"


# 
# connection point conduit_buffer
# 
add_interface conduit_buffer conduit end
set_interface_property conduit_buffer associatedClock clock
set_interface_property conduit_buffer associatedReset ""
set_interface_property conduit_buffer ENABLED true
set_interface_property conduit_buffer EXPORT_OF ""
set_interface_property conduit_buffer PORT_NAME_MAP ""
set_interface_property conduit_buffer CMSIS_SVD_VARIABLES ""
set_interface_property conduit_buffer SVD_ADDRESS_GROUP ""

add_interface_port conduit_buffer buffer_port buffer_port Input 2
add_interface_port conduit_buffer vsync_out buffer_vsync Output 1

