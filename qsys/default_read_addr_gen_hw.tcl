# TCL File Generated by Component Editor 15.0
# Sat Aug 27 20:46:30 CST 2016
# DO NOT MODIFY


# 
# default_read_addr_gen "Default Read Address Generator" v1.0
#  2016.08.27.20:46:30
# 
# 

# 
# request TCL package from ACDS 15.0
# 
package require -exact qsys 15.0


# 
# module default_read_addr_gen
# 
set_module_property DESCRIPTION ""
set_module_property NAME default_read_addr_gen
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME "Default Read Address Generator"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL default_read_addr_gen
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file default_read_addr_gen.vhd VHDL PATH src/vga_st/default_read_addr_gen.vhd TOP_LEVEL_FILE

add_fileset SIM_VERILOG SIM_VERILOG "" ""
set_fileset_property SIM_VERILOG TOP_LEVEL default_read_addr_gen
set_fileset_property SIM_VERILOG ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VERILOG ENABLE_FILE_OVERWRITE_MODE true
add_fileset_file default_read_addr_gen.vhd VHDL PATH src/vga_st/default_read_addr_gen.vhd

add_fileset SIM_VHDL SIM_VHDL "" ""
set_fileset_property SIM_VHDL TOP_LEVEL default_read_addr_gen
set_fileset_property SIM_VHDL ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VHDL ENABLE_FILE_OVERWRITE_MODE true
add_fileset_file default_read_addr_gen.vhd VHDL PATH src/vga_st/default_read_addr_gen.vhd


# 
# parameters
# 
add_parameter ADDR_WIDTH POSITIVE 27
set_parameter_property ADDR_WIDTH DEFAULT_VALUE 27
set_parameter_property ADDR_WIDTH DISPLAY_NAME ADDR_WIDTH
set_parameter_property ADDR_WIDTH TYPE POSITIVE
set_parameter_property ADDR_WIDTH UNITS None
set_parameter_property ADDR_WIDTH ALLOWED_RANGES 1:2147483647
set_parameter_property ADDR_WIDTH HDL_PARAMETER true
add_parameter HEIGHT POSITIVE 480
set_parameter_property HEIGHT DEFAULT_VALUE 480
set_parameter_property HEIGHT DISPLAY_NAME HEIGHT
set_parameter_property HEIGHT TYPE POSITIVE
set_parameter_property HEIGHT UNITS None
set_parameter_property HEIGHT ALLOWED_RANGES 1:2147483647
set_parameter_property HEIGHT HDL_PARAMETER true
add_parameter WIDTH POSITIVE 640
set_parameter_property WIDTH DEFAULT_VALUE 640
set_parameter_property WIDTH DISPLAY_NAME WIDTH
set_parameter_property WIDTH TYPE POSITIVE
set_parameter_property WIDTH UNITS None
set_parameter_property WIDTH ALLOWED_RANGES 1:2147483647
set_parameter_property WIDTH HDL_PARAMETER true
add_parameter BANK STD_LOGIC_VECTOR 0
set_parameter_property BANK DEFAULT_VALUE 0
set_parameter_property BANK DISPLAY_NAME BANK
set_parameter_property BANK TYPE STD_LOGIC_VECTOR
set_parameter_property BANK UNITS None
set_parameter_property BANK ALLOWED_RANGES 0:3
set_parameter_property BANK HDL_PARAMETER true


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
# connection point buffer_switch
# 
add_interface buffer_switch conduit end
set_interface_property buffer_switch associatedClock clock
set_interface_property buffer_switch associatedReset ""
set_interface_property buffer_switch ENABLED true
set_interface_property buffer_switch EXPORT_OF ""
set_interface_property buffer_switch PORT_NAME_MAP ""
set_interface_property buffer_switch CMSIS_SVD_VARIABLES ""
set_interface_property buffer_switch SVD_ADDRESS_GROUP ""

add_interface_port buffer_switch buffer_port buffer_port Input 2
add_interface_port buffer_switch buffer_vsync buffer_vsync Output 1


# 
# connection point addr_gen
# 
add_interface addr_gen conduit end
set_interface_property addr_gen associatedClock clock
set_interface_property addr_gen associatedReset ""
set_interface_property addr_gen ENABLED true
set_interface_property addr_gen EXPORT_OF ""
set_interface_property addr_gen PORT_NAME_MAP ""
set_interface_property addr_gen CMSIS_SVD_VARIABLES ""
set_interface_property addr_gen SVD_ADDRESS_GROUP ""

add_interface_port addr_gen addr_fetch addr_fetch Input 1
add_interface_port addr_gen addr_reset_n addr_reset_n Input 1
add_interface_port addr_gen addr_gen addr_gen Output addr_width

