# TCL File Generated by Component Editor 15.0
# Fri Aug 26 16:51:00 CST 2016
# DO NOT MODIFY


# 
# vid_write_buffer "Simple ST to MM Write Buffer" v2.0
#  2016.08.26.16:51:00
# 
# 

# 
# request TCL package from ACDS 15.0
# 
package require -exact qsys 15.0


# 
# module vid_write_buffer
# 
set_module_property DESCRIPTION ""
set_module_property NAME vid_write_buffer
set_module_property VERSION 2.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME "Simple ST to MM Write Buffer"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL vid_write_buffer
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file NEEE.vhd VHDL PATH src/NEEE.vhd
add_fileset_file ip_fifo.vhd VHDL PATH src/ip_fifo.vhd
add_fileset_file vid_write_buffer.vhd VHDL PATH src/camera_st/vid_write_buffer.vhd TOP_LEVEL_FILE

add_fileset SIM_VERILOG SIM_VERILOG "" ""
set_fileset_property SIM_VERILOG TOP_LEVEL vid_write_buffer
set_fileset_property SIM_VERILOG ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VERILOG ENABLE_FILE_OVERWRITE_MODE true
add_fileset_file NEEE.vhd VHDL PATH src/NEEE.vhd
add_fileset_file ip_fifo.vhd VHDL PATH src/ip_fifo.vhd
add_fileset_file vid_write_buffer.vhd VHDL PATH src/camera_st/vid_write_buffer.vhd

add_fileset SIM_VHDL SIM_VHDL "" ""
set_fileset_property SIM_VHDL TOP_LEVEL vid_write_buffer
set_fileset_property SIM_VHDL ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VHDL ENABLE_FILE_OVERWRITE_MODE true
add_fileset_file NEEE.vhd VHDL PATH src/NEEE.vhd
add_fileset_file ip_fifo.vhd VHDL PATH src/ip_fifo.vhd
add_fileset_file vid_write_buffer.vhd VHDL PATH src/camera_st/vid_write_buffer.vhd


# 
# parameters
# 
add_parameter DATA_WIDTH POSITIVE 32
set_parameter_property DATA_WIDTH DEFAULT_VALUE 32
set_parameter_property DATA_WIDTH DISPLAY_NAME DATA_WIDTH
set_parameter_property DATA_WIDTH TYPE POSITIVE
set_parameter_property DATA_WIDTH UNITS None
set_parameter_property DATA_WIDTH ALLOWED_RANGES 1:2147483647
set_parameter_property DATA_WIDTH HDL_PARAMETER true
add_parameter ADDR_WIDTH POSITIVE 27
set_parameter_property ADDR_WIDTH DEFAULT_VALUE 27
set_parameter_property ADDR_WIDTH DISPLAY_NAME ADDR_WIDTH
set_parameter_property ADDR_WIDTH TYPE POSITIVE
set_parameter_property ADDR_WIDTH UNITS None
set_parameter_property ADDR_WIDTH ALLOWED_RANGES 1:2147483647
set_parameter_property ADDR_WIDTH HDL_PARAMETER true
add_parameter FIFO_LENGTH_LOG_2 POSITIVE 6
set_parameter_property FIFO_LENGTH_LOG_2 DEFAULT_VALUE 6
set_parameter_property FIFO_LENGTH_LOG_2 DISPLAY_NAME FIFO_LENGTH_LOG_2
set_parameter_property FIFO_LENGTH_LOG_2 TYPE POSITIVE
set_parameter_property FIFO_LENGTH_LOG_2 UNITS None
set_parameter_property FIFO_LENGTH_LOG_2 ALLOWED_RANGES 1:2147483647
set_parameter_property FIFO_LENGTH_LOG_2 HDL_PARAMETER true


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
# connection point st_clock
# 
add_interface st_clock clock end
set_interface_property st_clock clockRate 0
set_interface_property st_clock ENABLED true
set_interface_property st_clock EXPORT_OF ""
set_interface_property st_clock PORT_NAME_MAP ""
set_interface_property st_clock CMSIS_SVD_VARIABLES ""
set_interface_property st_clock SVD_ADDRESS_GROUP ""

add_interface_port st_clock st_clk clk Input 1


# 
# connection point st0
# 
add_interface st0 avalon_streaming end
set_interface_property st0 associatedClock st_clock
set_interface_property st0 associatedReset reset
set_interface_property st0 dataBitsPerSymbol 8
set_interface_property st0 errorDescriptor ""
set_interface_property st0 firstSymbolInHighOrderBits true
set_interface_property st0 maxChannel 0
set_interface_property st0 readyLatency 0
set_interface_property st0 ENABLED true
set_interface_property st0 EXPORT_OF ""
set_interface_property st0 PORT_NAME_MAP ""
set_interface_property st0 CMSIS_SVD_VARIABLES ""
set_interface_property st0 SVD_ADDRESS_GROUP ""

add_interface_port st0 st_data data Input data_width
add_interface_port st0 st_eop endofpacket Input 1
add_interface_port st0 st_sop startofpacket Input 1
add_interface_port st0 st_valid valid Input 1


# 
# connection point m0
# 
add_interface m0 avalon start
set_interface_property m0 addressUnits SYMBOLS
set_interface_property m0 associatedClock clock
set_interface_property m0 associatedReset reset
set_interface_property m0 bitsPerSymbol 8
set_interface_property m0 burstOnBurstBoundariesOnly false
set_interface_property m0 burstcountUnits WORDS
set_interface_property m0 doStreamReads false
set_interface_property m0 doStreamWrites false
set_interface_property m0 holdTime 0
set_interface_property m0 linewrapBursts false
set_interface_property m0 maximumPendingReadTransactions 0
set_interface_property m0 maximumPendingWriteTransactions 0
set_interface_property m0 readLatency 0
set_interface_property m0 readWaitTime 1
set_interface_property m0 setupTime 0
set_interface_property m0 timingUnits Cycles
set_interface_property m0 writeWaitTime 0
set_interface_property m0 ENABLED true
set_interface_property m0 EXPORT_OF ""
set_interface_property m0 PORT_NAME_MAP ""
set_interface_property m0 CMSIS_SVD_VARIABLES ""
set_interface_property m0 SVD_ADDRESS_GROUP ""

add_interface_port m0 address address Output addr_width
add_interface_port m0 write write Output 1
add_interface_port m0 writedata writedata Output data_width
add_interface_port m0 waitrequest waitrequest Input 1
add_interface_port m0 lock lock Output 1


# 
# connection point addr_gen
# 
add_interface addr_gen conduit end
set_interface_property addr_gen associatedClock st_clock
set_interface_property addr_gen associatedReset ""
set_interface_property addr_gen ENABLED true
set_interface_property addr_gen EXPORT_OF ""
set_interface_property addr_gen PORT_NAME_MAP ""
set_interface_property addr_gen CMSIS_SVD_VARIABLES ""
set_interface_property addr_gen SVD_ADDRESS_GROUP ""

add_interface_port addr_gen addr_fetch addr_fetch Output 1
add_interface_port addr_gen addr_vsync addr_vsync Output 1
add_interface_port addr_gen addr_gen addr_gen Input addr_width

