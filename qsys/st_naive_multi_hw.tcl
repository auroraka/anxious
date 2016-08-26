# TCL File Generated by Component Editor 15.0
# Fri Aug 26 21:42:37 CST 2016
# DO NOT MODIFY


# 
# st_naive_multi "Naive-ST Multiplier" v1.0
#  2016.08.26.21:42:37
# 
# 

# 
# request TCL package from ACDS 15.0
# 
package require -exact qsys 15.0


# 
# module st_naive_multi
# 
set_module_property DESCRIPTION ""
set_module_property NAME st_naive_multi
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME "Naive-ST Multiplier"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL st_naive_multi
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file st_naive_multi.vhd VHDL PATH src/util/st_naive_multi.vhd TOP_LEVEL_FILE


# 
# parameters
# 
add_parameter DIN_DATA_WIDTH POSITIVE 24
set_parameter_property DIN_DATA_WIDTH DEFAULT_VALUE 24
set_parameter_property DIN_DATA_WIDTH DISPLAY_NAME DIN_DATA_WIDTH
set_parameter_property DIN_DATA_WIDTH TYPE POSITIVE
set_parameter_property DIN_DATA_WIDTH UNITS None
set_parameter_property DIN_DATA_WIDTH HDL_PARAMETER true
add_parameter DOUT0_DATA_WIDTH POSITIVE 24
set_parameter_property DOUT0_DATA_WIDTH DEFAULT_VALUE 24
set_parameter_property DOUT0_DATA_WIDTH DISPLAY_NAME DOUT0_DATA_WIDTH
set_parameter_property DOUT0_DATA_WIDTH TYPE POSITIVE
set_parameter_property DOUT0_DATA_WIDTH UNITS None
set_parameter_property DOUT0_DATA_WIDTH HDL_PARAMETER true
add_parameter DOUT1_DATA_WIDTH POSITIVE 32
set_parameter_property DOUT1_DATA_WIDTH DEFAULT_VALUE 32
set_parameter_property DOUT1_DATA_WIDTH DISPLAY_NAME DOUT1_DATA_WIDTH
set_parameter_property DOUT1_DATA_WIDTH TYPE POSITIVE
set_parameter_property DOUT1_DATA_WIDTH UNITS None
set_parameter_property DOUT1_DATA_WIDTH HDL_PARAMETER true


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
# connection point dout0
# 
add_interface dout0 avalon_streaming start
set_interface_property dout0 associatedClock clock
set_interface_property dout0 associatedReset reset
set_interface_property dout0 dataBitsPerSymbol 8
set_interface_property dout0 errorDescriptor ""
set_interface_property dout0 firstSymbolInHighOrderBits true
set_interface_property dout0 maxChannel 0
set_interface_property dout0 readyLatency 0
set_interface_property dout0 ENABLED true
set_interface_property dout0 EXPORT_OF ""
set_interface_property dout0 PORT_NAME_MAP ""
set_interface_property dout0 CMSIS_SVD_VARIABLES ""
set_interface_property dout0 SVD_ADDRESS_GROUP ""

add_interface_port dout0 dout0_data data Output dout0_data_width
add_interface_port dout0 dout0_eop endofpacket Output 1
add_interface_port dout0 dout0_sop startofpacket Output 1
add_interface_port dout0 dout0_valid valid Output 1


# 
# connection point dout1
# 
add_interface dout1 avalon_streaming start
set_interface_property dout1 associatedClock clock
set_interface_property dout1 associatedReset reset
set_interface_property dout1 dataBitsPerSymbol 8
set_interface_property dout1 errorDescriptor ""
set_interface_property dout1 firstSymbolInHighOrderBits true
set_interface_property dout1 maxChannel 0
set_interface_property dout1 readyLatency 0
set_interface_property dout1 ENABLED true
set_interface_property dout1 EXPORT_OF ""
set_interface_property dout1 PORT_NAME_MAP ""
set_interface_property dout1 CMSIS_SVD_VARIABLES ""
set_interface_property dout1 SVD_ADDRESS_GROUP ""

add_interface_port dout1 dout1_data data Output dout1_data_width
add_interface_port dout1 dout1_eop endofpacket Output 1
add_interface_port dout1 dout1_sop startofpacket Output 1
add_interface_port dout1 dout1_valid valid Output 1


# 
# connection point din
# 
add_interface din avalon_streaming end
set_interface_property din associatedClock clock
set_interface_property din associatedReset reset
set_interface_property din dataBitsPerSymbol 8
set_interface_property din errorDescriptor ""
set_interface_property din firstSymbolInHighOrderBits true
set_interface_property din maxChannel 0
set_interface_property din readyLatency 0
set_interface_property din ENABLED true
set_interface_property din EXPORT_OF ""
set_interface_property din PORT_NAME_MAP ""
set_interface_property din CMSIS_SVD_VARIABLES ""
set_interface_property din SVD_ADDRESS_GROUP ""

add_interface_port din din_data data Input din_data_width
add_interface_port din din_valid valid Input 1
add_interface_port din din_sop startofpacket Input 1
add_interface_port din din_eop endofpacket Input 1

