# TCL File Generated by Component Editor 15.0
# Sun Oct 09 13:50:56 CST 2016
# DO NOT MODIFY


# 
# sdram_arbitrator "SDRAM Arbitrator" v1.0
#  2016.10.09.13:50:56
# 
# 

# 
# request TCL package from ACDS 15.0
# 
package require -exact qsys 15.0


# 
# module sdram_arbitrator
# 
set_module_property DESCRIPTION ""
set_module_property NAME sdram_arbitrator
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME "SDRAM Arbitrator"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL sdram_arbitrator
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file NEEE.vhd VHDL PATH src/NEEE.vhd
add_fileset_file sdram_arbitrator.vhd VHDL PATH src/arb/sdram_arbitrator.vhd TOP_LEVEL_FILE

add_fileset SIM_VERILOG SIM_VERILOG "" ""
set_fileset_property SIM_VERILOG TOP_LEVEL sdram_arbitrator
set_fileset_property SIM_VERILOG ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VERILOG ENABLE_FILE_OVERWRITE_MODE true
add_fileset_file NEEE.vhd VHDL PATH src/NEEE.vhd
add_fileset_file sdram_arbitrator.vhd VHDL PATH src/arb/sdram_arbitrator.vhd

add_fileset SIM_VHDL SIM_VHDL "" ""
set_fileset_property SIM_VHDL TOP_LEVEL sdram_arbitrator
set_fileset_property SIM_VHDL ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VHDL ENABLE_FILE_OVERWRITE_MODE true
add_fileset_file NEEE.vhd VHDL PATH src/NEEE.vhd
add_fileset_file sdram_arbitrator.vhd VHDL PATH src/arb/sdram_arbitrator.vhd


# 
# parameters
# 


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
# connection point s0
# 
add_interface s0 avalon end
set_interface_property s0 addressUnits WORDS
set_interface_property s0 associatedClock clock
set_interface_property s0 associatedReset reset
set_interface_property s0 bitsPerSymbol 8
set_interface_property s0 burstOnBurstBoundariesOnly false
set_interface_property s0 burstcountUnits WORDS
set_interface_property s0 explicitAddressSpan 0
set_interface_property s0 holdTime 0
set_interface_property s0 linewrapBursts false
set_interface_property s0 maximumPendingReadTransactions 7
set_interface_property s0 maximumPendingWriteTransactions 0
set_interface_property s0 readLatency 0
set_interface_property s0 readWaitTime 1
set_interface_property s0 setupTime 0
set_interface_property s0 timingUnits Cycles
set_interface_property s0 writeWaitTime 0
set_interface_property s0 ENABLED true
set_interface_property s0 EXPORT_OF ""
set_interface_property s0 PORT_NAME_MAP ""
set_interface_property s0 CMSIS_SVD_VARIABLES ""
set_interface_property s0 SVD_ADDRESS_GROUP ""

add_interface_port s0 avs_s0_address address Input 25
add_interface_port s0 avs_s0_byteenable byteenable Input 4
add_interface_port s0 avs_s0_read read Input 1
add_interface_port s0 avs_s0_readdata readdata Output 32
add_interface_port s0 avs_s0_readdatavalid readdatavalid Output 1
add_interface_port s0 avs_s0_waitrequest waitrequest Output 1
add_interface_port s0 avs_s0_write write Input 1
add_interface_port s0 avs_s0_writedata writedata Input 32
add_interface_port s0 avs_s0_lock lock Input 1
set_interface_assignment s0 embeddedsw.configuration.isFlash 0
set_interface_assignment s0 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment s0 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment s0 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point s1
# 
add_interface s1 avalon end
set_interface_property s1 addressUnits WORDS
set_interface_property s1 associatedClock clock
set_interface_property s1 associatedReset reset
set_interface_property s1 bitsPerSymbol 8
set_interface_property s1 burstOnBurstBoundariesOnly false
set_interface_property s1 burstcountUnits WORDS
set_interface_property s1 explicitAddressSpan 0
set_interface_property s1 holdTime 0
set_interface_property s1 linewrapBursts false
set_interface_property s1 maximumPendingReadTransactions 7
set_interface_property s1 maximumPendingWriteTransactions 0
set_interface_property s1 readLatency 0
set_interface_property s1 readWaitTime 1
set_interface_property s1 setupTime 0
set_interface_property s1 timingUnits Cycles
set_interface_property s1 writeWaitTime 0
set_interface_property s1 ENABLED true
set_interface_property s1 EXPORT_OF ""
set_interface_property s1 PORT_NAME_MAP ""
set_interface_property s1 CMSIS_SVD_VARIABLES ""
set_interface_property s1 SVD_ADDRESS_GROUP ""

add_interface_port s1 avs_s1_address address Input 25
add_interface_port s1 avs_s1_byteenable byteenable Input 4
add_interface_port s1 avs_s1_read read Input 1
add_interface_port s1 avs_s1_readdata readdata Output 32
add_interface_port s1 avs_s1_readdatavalid readdatavalid Output 1
add_interface_port s1 avs_s1_waitrequest waitrequest Output 1
add_interface_port s1 avs_s1_write write Input 1
add_interface_port s1 avs_s1_writedata writedata Input 32
add_interface_port s1 avs_s1_lock lock Input 1
set_interface_assignment s1 embeddedsw.configuration.isFlash 0
set_interface_assignment s1 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment s1 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment s1 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point s2
# 
add_interface s2 avalon end
set_interface_property s2 addressUnits WORDS
set_interface_property s2 associatedClock clock
set_interface_property s2 associatedReset reset
set_interface_property s2 bitsPerSymbol 8
set_interface_property s2 burstOnBurstBoundariesOnly false
set_interface_property s2 burstcountUnits WORDS
set_interface_property s2 explicitAddressSpan 0
set_interface_property s2 holdTime 0
set_interface_property s2 linewrapBursts false
set_interface_property s2 maximumPendingReadTransactions 7
set_interface_property s2 maximumPendingWriteTransactions 0
set_interface_property s2 readLatency 0
set_interface_property s2 readWaitTime 1
set_interface_property s2 setupTime 0
set_interface_property s2 timingUnits Cycles
set_interface_property s2 writeWaitTime 0
set_interface_property s2 ENABLED true
set_interface_property s2 EXPORT_OF ""
set_interface_property s2 PORT_NAME_MAP ""
set_interface_property s2 CMSIS_SVD_VARIABLES ""
set_interface_property s2 SVD_ADDRESS_GROUP ""

add_interface_port s2 avs_s2_address address Input 25
add_interface_port s2 avs_s2_byteenable byteenable Input 4
add_interface_port s2 avs_s2_read read Input 1
add_interface_port s2 avs_s2_readdata readdata Output 32
add_interface_port s2 avs_s2_readdatavalid readdatavalid Output 1
add_interface_port s2 avs_s2_waitrequest waitrequest Output 1
add_interface_port s2 avs_s2_write write Input 1
add_interface_port s2 avs_s2_writedata writedata Input 32
add_interface_port s2 avs_s2_lock lock Input 1
set_interface_assignment s2 embeddedsw.configuration.isFlash 0
set_interface_assignment s2 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment s2 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment s2 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point s3
# 
add_interface s3 avalon end
set_interface_property s3 addressUnits WORDS
set_interface_property s3 associatedClock clock
set_interface_property s3 associatedReset reset
set_interface_property s3 bitsPerSymbol 8
set_interface_property s3 burstOnBurstBoundariesOnly false
set_interface_property s3 burstcountUnits WORDS
set_interface_property s3 explicitAddressSpan 0
set_interface_property s3 holdTime 0
set_interface_property s3 linewrapBursts false
set_interface_property s3 maximumPendingReadTransactions 7
set_interface_property s3 maximumPendingWriteTransactions 0
set_interface_property s3 readLatency 0
set_interface_property s3 readWaitTime 1
set_interface_property s3 setupTime 0
set_interface_property s3 timingUnits Cycles
set_interface_property s3 writeWaitTime 0
set_interface_property s3 ENABLED true
set_interface_property s3 EXPORT_OF ""
set_interface_property s3 PORT_NAME_MAP ""
set_interface_property s3 CMSIS_SVD_VARIABLES ""
set_interface_property s3 SVD_ADDRESS_GROUP ""

add_interface_port s3 avs_s3_address address Input 25
add_interface_port s3 avs_s3_byteenable byteenable Input 4
add_interface_port s3 avs_s3_read read Input 1
add_interface_port s3 avs_s3_readdata readdata Output 32
add_interface_port s3 avs_s3_readdatavalid readdatavalid Output 1
add_interface_port s3 avs_s3_waitrequest waitrequest Output 1
add_interface_port s3 avs_s3_write write Input 1
add_interface_port s3 avs_s3_writedata writedata Input 32
add_interface_port s3 avs_s3_lock lock Input 1
set_interface_assignment s3 embeddedsw.configuration.isFlash 0
set_interface_assignment s3 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment s3 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment s3 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point s4
# 
add_interface s4 avalon end
set_interface_property s4 addressUnits WORDS
set_interface_property s4 associatedClock clock
set_interface_property s4 associatedReset reset
set_interface_property s4 bitsPerSymbol 8
set_interface_property s4 burstOnBurstBoundariesOnly false
set_interface_property s4 burstcountUnits WORDS
set_interface_property s4 explicitAddressSpan 0
set_interface_property s4 holdTime 0
set_interface_property s4 linewrapBursts false
set_interface_property s4 maximumPendingReadTransactions 7
set_interface_property s4 maximumPendingWriteTransactions 0
set_interface_property s4 readLatency 0
set_interface_property s4 readWaitTime 1
set_interface_property s4 setupTime 0
set_interface_property s4 timingUnits Cycles
set_interface_property s4 writeWaitTime 0
set_interface_property s4 ENABLED true
set_interface_property s4 EXPORT_OF ""
set_interface_property s4 PORT_NAME_MAP ""
set_interface_property s4 CMSIS_SVD_VARIABLES ""
set_interface_property s4 SVD_ADDRESS_GROUP ""

add_interface_port s4 avs_s4_address address Input 25
add_interface_port s4 avs_s4_byteenable byteenable Input 4
add_interface_port s4 avs_s4_read read Input 1
add_interface_port s4 avs_s4_readdata readdata Output 32
add_interface_port s4 avs_s4_readdatavalid readdatavalid Output 1
add_interface_port s4 avs_s4_waitrequest waitrequest Output 1
add_interface_port s4 avs_s4_write write Input 1
add_interface_port s4 avs_s4_writedata writedata Input 32
add_interface_port s4 avs_s4_lock lock Input 1
set_interface_assignment s4 embeddedsw.configuration.isFlash 0
set_interface_assignment s4 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment s4 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment s4 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point s5
# 
add_interface s5 avalon end
set_interface_property s5 addressUnits WORDS
set_interface_property s5 associatedClock clock
set_interface_property s5 associatedReset reset
set_interface_property s5 bitsPerSymbol 8
set_interface_property s5 burstOnBurstBoundariesOnly false
set_interface_property s5 burstcountUnits WORDS
set_interface_property s5 explicitAddressSpan 0
set_interface_property s5 holdTime 0
set_interface_property s5 linewrapBursts false
set_interface_property s5 maximumPendingReadTransactions 7
set_interface_property s5 maximumPendingWriteTransactions 0
set_interface_property s5 readLatency 0
set_interface_property s5 readWaitTime 1
set_interface_property s5 setupTime 0
set_interface_property s5 timingUnits Cycles
set_interface_property s5 writeWaitTime 0
set_interface_property s5 ENABLED true
set_interface_property s5 EXPORT_OF ""
set_interface_property s5 PORT_NAME_MAP ""
set_interface_property s5 CMSIS_SVD_VARIABLES ""
set_interface_property s5 SVD_ADDRESS_GROUP ""

add_interface_port s5 avs_s5_address address Input 25
add_interface_port s5 avs_s5_byteenable byteenable Input 4
add_interface_port s5 avs_s5_read read Input 1
add_interface_port s5 avs_s5_readdata readdata Output 32
add_interface_port s5 avs_s5_readdatavalid readdatavalid Output 1
add_interface_port s5 avs_s5_waitrequest waitrequest Output 1
add_interface_port s5 avs_s5_write write Input 1
add_interface_port s5 avs_s5_writedata writedata Input 32
add_interface_port s5 avs_s5_lock lock Input 1
set_interface_assignment s5 embeddedsw.configuration.isFlash 0
set_interface_assignment s5 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment s5 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment s5 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point m0
# 
add_interface m0 avalon start
set_interface_property m0 addressUnits WORDS
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

add_interface_port m0 avm_m0_address address Output 25
add_interface_port m0 avm_m0_byteenable byteenable Output 4
add_interface_port m0 avm_m0_read read Output 1
add_interface_port m0 avm_m0_readdata readdata Input 32
add_interface_port m0 avm_m0_readdatavalid readdatavalid Input 1
add_interface_port m0 avm_m0_waitrequest waitrequest Input 1
add_interface_port m0 avm_m0_write write Output 1
add_interface_port m0 avm_m0_writedata writedata Output 32
