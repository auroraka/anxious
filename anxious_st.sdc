# vim: filetype=tcl :

#**************************************************************
# This .sdc file is created by Terasic Tool.
# Users are recommended to modify this file to match users logic.
#**************************************************************

#**************************************************************
# Create Clock
#**************************************************************
create_clock -period 20 [get_ports CLOCK_50]
create_clock -period 20 [get_ports CLOCK2_50]
create_clock -period 20 [get_ports CLOCK3_50]

create_clock -period 41.667 [get_ports {GPIO[24]}]
create_clock -period 41.667 [get_ports {GPIO[6]}]

set clk_pclk0 [get_clocks {GPIO[24]}]
set clk_pclk1 [get_clocks {GPIO[6]}]

#**************************************************************
# Create Generated Clock
#**************************************************************
derive_pll_clocks

set clk_qsys [get_clocks {U_cam_pll|altpll_component|auto_generated|pll1|clk[1]}]
set clk_sdram_shifted [get_clocks {U_cam_pll|altpll_component|auto_generated|pll1|clk[2]}]
# set clk_sdram_shifted $clk_qsys
set clk_vga [get_clocks {U_cam_pll|altpll_component|auto_generated|pll1|clk[3]}]
set clk_ssram $clk_sdram_shifted

#**************************************************************
# Set Clock Latency
#**************************************************************



#**************************************************************
# Set Clock Uncertainty
#**************************************************************
derive_clock_uncertainty



#**************************************************************
# Set Input Delay
#**************************************************************

set sdram_input_ports [get_ports {DRAM_DQ[0] DRAM_DQ[1] DRAM_DQ[2] DRAM_DQ[3] DRAM_DQ[4] DRAM_DQ[5] DRAM_DQ[6] DRAM_DQ[7] DRAM_DQ[8] DRAM_DQ[9] DRAM_DQ[10] DRAM_DQ[11] DRAM_DQ[12] DRAM_DQ[13] DRAM_DQ[14] DRAM_DQ[15] DRAM_DQ[16] DRAM_DQ[17] DRAM_DQ[18] DRAM_DQ[19] DRAM_DQ[20] DRAM_DQ[21] DRAM_DQ[22] DRAM_DQ[23] DRAM_DQ[24] DRAM_DQ[25] DRAM_DQ[26] DRAM_DQ[27] DRAM_DQ[28] DRAM_DQ[29] DRAM_DQ[30] DRAM_DQ[31]}]

set sdram_output_ports [get_ports {DRAM_ADDR[0] DRAM_ADDR[1] DRAM_ADDR[2] DRAM_ADDR[3] DRAM_ADDR[4] DRAM_ADDR[5] DRAM_ADDR[6] DRAM_ADDR[7] DRAM_ADDR[8] DRAM_ADDR[9] DRAM_ADDR[10] DRAM_ADDR[11] DRAM_ADDR[12] DRAM_BA[0] DRAM_BA[1] DRAM_CAS_N DRAM_CKE DRAM_CS_N DRAM_DQM[0] DRAM_DQM[1] DRAM_DQM[2] DRAM_DQM[3] DRAM_DQ[0] DRAM_DQ[1] DRAM_DQ[2] DRAM_DQ[3] DRAM_DQ[4] DRAM_DQ[5] DRAM_DQ[6] DRAM_DQ[7] DRAM_DQ[8] DRAM_DQ[9] DRAM_DQ[10] DRAM_DQ[11] DRAM_DQ[12] DRAM_DQ[13] DRAM_DQ[14] DRAM_DQ[15] DRAM_DQ[16] DRAM_DQ[17] DRAM_DQ[18] DRAM_DQ[19] DRAM_DQ[20] DRAM_DQ[21] DRAM_DQ[22] DRAM_DQ[23] DRAM_DQ[24] DRAM_DQ[25] DRAM_DQ[26] DRAM_DQ[27] DRAM_DQ[28] DRAM_DQ[29] DRAM_DQ[30] DRAM_DQ[31] DRAM_RAS_N DRAM_WE_N}]

# set_input_delay -clock $clk_sdram_shifted -max 6.4 $sdram_input_ports
# set_input_delay -clock $clk_sdram_shifted -min 1.0 $sdram_input_ports
# set_output_delay -clock $clk_sdram_shifted -max 1.5 $sdram_output_ports
# set_output_delay -clock $clk_sdram_shifted -min -0.8 $sdram_output_ports

set vga_output_ports [get_ports {VGA_BLANK_N VGA_B[0] VGA_B[1] VGA_B[2] VGA_B[3] VGA_B[4] VGA_B[5] VGA_B[6] VGA_B[7] VGA_G[0] VGA_G[1] VGA_G[2] VGA_G[3] VGA_G[4] VGA_G[5] VGA_G[6] VGA_G[7] VGA_HS VGA_R[0] VGA_R[1] VGA_R[2] VGA_R[3] VGA_R[4] VGA_R[5] VGA_R[6] VGA_R[7] VGA_SYNC_N VGA_VS}]

set_output_delay -clock $clk_vga -max 0.2 $vga_output_ports
set_output_delay -clock $clk_vga -min -1.5 $vga_output_ports

set ssram_input_ports [get_ports {FS_DQ[0] FS_DQ[1] FS_DQ[2] FS_DQ[3] FS_DQ[4] FS_DQ[5] FS_DQ[6] FS_DQ[7] FS_DQ[8] FS_DQ[9] FS_DQ[10] FS_DQ[11] FS_DQ[12] FS_DQ[13] FS_DQ[14] FS_DQ[15] FS_DQ[16] FS_DQ[17] FS_DQ[18] FS_DQ[19] FS_DQ[20] FS_DQ[21] FS_DQ[22] FS_DQ[23] FS_DQ[24] FS_DQ[25] FS_DQ[26] FS_DQ[27] FS_DQ[28] FS_DQ[29] FS_DQ[30] FS_DQ[31]}]
set ssram_output_ports [get_ports {FS_ADDR[1] FS_ADDR[2] FS_ADDR[3] FS_ADDR[4] FS_ADDR[5] FS_ADDR[6] FS_ADDR[7] FS_ADDR[8] FS_ADDR[9] FS_ADDR[10] FS_ADDR[11] FS_ADDR[12] FS_ADDR[13] FS_ADDR[14] FS_ADDR[15] FS_ADDR[16] FS_ADDR[17] FS_ADDR[18] FS_ADDR[19] FS_ADDR[20] FS_ADDR[21] FS_ADDR[22] FS_ADDR[23] FS_ADDR[24] FS_ADDR[25] FS_ADDR[26] FS_DQ[0] FS_DQ[1] FS_DQ[2] FS_DQ[3] FS_DQ[4] FS_DQ[5] FS_DQ[6] FS_DQ[7] FS_DQ[8] FS_DQ[9] FS_DQ[10] FS_DQ[11] FS_DQ[12] FS_DQ[13] FS_DQ[14] FS_DQ[15] FS_DQ[16] FS_DQ[17] FS_DQ[18] FS_DQ[19] FS_DQ[20] FS_DQ[21] FS_DQ[22] FS_DQ[23] FS_DQ[24] FS_DQ[25] FS_DQ[26] FS_DQ[27] FS_DQ[28] FS_DQ[29] FS_DQ[30] FS_DQ[31] SSRAM0_CE_N SSRAM1_CE_N SSRAM_ADSC_N SSRAM_ADSP_N SSRAM_ADV_N SSRAM_BE[0] SSRAM_BE[1] SSRAM_BE[2] SSRAM_BE[3] SSRAM_GW_N SSRAM_OE_N SSRAM_WE_N}]

# set_input_delay -clock $clk_ssram -max 3.1 $ssram_input_ports
# set_input_delay -clock $clk_ssram -min 0.0 $ssram_input_ports
# set_output_delay -clock $clk_ssram -max 1.4 $ssram_output_ports
# set_output_delay -clock $clk_ssram -min -0.4 $ssram_output_ports

#**************************************************************
# Set Output Delay
#**************************************************************



#**************************************************************
# Set Clock Groups
#**************************************************************



#**************************************************************
# Set False Path
#**************************************************************

set_false_path -from $clk_qsys -to $clk_pclk0
set_false_path -from $clk_pclk0 -to $clk_qsys
set_false_path -from $clk_qsys -to $clk_pclk1
set_false_path -from $clk_pclk1 -to $clk_qsys

#**************************************************************
# Set Multicycle Path
#**************************************************************

# set_multicycle_path -from $clk_sdram_shifted -to $clk_qsys -setup -end 2


#**************************************************************
# Set Maximum Delay
#**************************************************************



#**************************************************************
# Set Minimum Delay
#**************************************************************



#**************************************************************
# Set Input Transition
#**************************************************************



#**************************************************************
# Set Load
#**************************************************************



