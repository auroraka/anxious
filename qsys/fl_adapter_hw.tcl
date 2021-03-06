# TCL File Generated by Component Editor 15.0
# Sun Oct 02 16:46:28 CST 2016
# DO NOT MODIFY


# 
# fl_adapter "Flash Tri-State Adapter" v1.0
#  2016.10.02.16:46:28
# 
# 

# 
# request TCL package from ACDS 15.0
# 
package require -exact qsys 15.0


# 
# module fl_adapter
# 
set_module_property DESCRIPTION ""
set_module_property NAME fl_adapter
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME "Flash Tri-State Adapter"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL fl_adapter
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file fl_adapter.vhd VHDL PATH src/fs/fl_adapter.vhd TOP_LEVEL_FILE


# 
# parameters
# 


# 
# display items
# 


# 
# connection point tcm
# 
add_interface tcm tristate_conduit start
set_interface_property tcm associatedClock clock
set_interface_property tcm ENABLED true
set_interface_property tcm EXPORT_OF ""
set_interface_property tcm PORT_NAME_MAP ""
set_interface_property tcm CMSIS_SVD_VARIABLES ""
set_interface_property tcm SVD_ADDRESS_GROUP ""

add_interface_port tcm tcm_write_n_out write_n_out Output 1
add_interface_port tcm tcm_chipselect_n_out chipselect_n_out Output 1
add_interface_port tcm tcm_outputenable_n_out outputenable_n_out Output 1
add_interface_port tcm tcm_request request Output 1
add_interface_port tcm tcm_grant grant Input 1
add_interface_port tcm tcm_data_outen data_outen Output 1
add_interface_port tcm tcm_data_in data_in Input 32
add_interface_port tcm tcm_data_out data_out Output 32
add_interface_port tcm tcm_address_out address_out Output 26


# 
# connection point tcs
# 
add_interface tcs tristate_conduit end
set_interface_property tcs associatedClock clock
set_interface_property tcs ENABLED true
set_interface_property tcs EXPORT_OF ""
set_interface_property tcs PORT_NAME_MAP ""
set_interface_property tcs CMSIS_SVD_VARIABLES ""
set_interface_property tcs SVD_ADDRESS_GROUP ""

add_interface_port tcs tcs_write_n_out write_n_out Input 1
add_interface_port tcs tcs_chipselect_n_out chipselect_n_out Input 1
add_interface_port tcs tcs_outputenable_n_out outputenable_n_out Input 1
add_interface_port tcs tcs_request request Input 1
add_interface_port tcs tcs_grant grant Output 1
add_interface_port tcs tcs_data_outen data_outen Input 1
add_interface_port tcs tcs_data_in data_in Output 16
add_interface_port tcs tcs_data_out data_out Input 16
add_interface_port tcs tcs_address_out address_out Input 26


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

