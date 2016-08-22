# TCL File Generated by Component Editor 15.0
# Mon Aug 22 17:01:43 CST 2016
# DO NOT MODIFY


# 
# default_addr_gen "Default Address Generator" v1.0
#  2016.08.22.17:01:43
# 
# 

# 
# request TCL package from ACDS 15.0
# 
package require -exact qsys 15.0


# 
# module default_addr_gen
# 
set_module_property DESCRIPTION ""
set_module_property NAME default_addr_gen
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME "Default Address Generator"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL default_addr_gen
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file NEEE.vhd VHDL PATH NEEE.vhd
add_fileset_file default_addr_gen.vhd VHDL PATH camera_st/default_addr_gen.vhd TOP_LEVEL_FILE

add_fileset SIM_VERILOG SIM_VERILOG "" ""
set_fileset_property SIM_VERILOG TOP_LEVEL default_addr_gen
set_fileset_property SIM_VERILOG ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VERILOG ENABLE_FILE_OVERWRITE_MODE true
add_fileset_file NEEE.vhd VHDL PATH NEEE.vhd
add_fileset_file default_addr_gen.vhd VHDL PATH camera_st/default_addr_gen.vhd

add_fileset SIM_VHDL SIM_VHDL "" ""
set_fileset_property SIM_VHDL TOP_LEVEL default_addr_gen
set_fileset_property SIM_VHDL ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VHDL ENABLE_FILE_OVERWRITE_MODE true
add_fileset_file NEEE.vhd VHDL PATH NEEE.vhd
add_fileset_file default_addr_gen.vhd VHDL PATH camera_st/default_addr_gen.vhd


# 
# parameters
# 
add_parameter ADDR_WIDTH POSITIVE 21
set_parameter_property ADDR_WIDTH DEFAULT_VALUE 21
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
# connection point conduit_addr
# 
add_interface conduit_addr conduit end
set_interface_property conduit_addr associatedClock clock
set_interface_property conduit_addr associatedReset ""
set_interface_property conduit_addr ENABLED true
set_interface_property conduit_addr EXPORT_OF ""
set_interface_property conduit_addr PORT_NAME_MAP ""
set_interface_property conduit_addr CMSIS_SVD_VARIABLES ""
set_interface_property conduit_addr SVD_ADDRESS_GROUP ""

add_interface_port conduit_addr addr_fetch addr_fetch Input 1
add_interface_port conduit_addr addr_vsync addr_vsync Input 1
add_interface_port conduit_addr addr_gen addr_gen Output addr_width

