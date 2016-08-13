-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

package NEEE is
    -- VGA
	constant VIEW_WIDTH  : integer := 640;
	constant VIEW_HEIGHT : integer := 480;
	constant VGA_FIFO_LENGTH : positive  := 64;

    -- SDRAM
    constant SDRAM_MAX_BURST_LENGTH: positive := 64;
    constant SDRAM_ADDR_LENGTH: positive := 25;
    constant SDRAM_DATA_WIDTH: positive := 32;

    subtype sdram_addr_type is
    	std_logic_vector(SDRAM_ADDR_LENGTH - 1 downto 0); -- BANK(24-23) & ROW(22-10) & COL(9-0)
    subtype sdram_data_type is
    	std_logic_vector(SDRAM_DATA_WIDTH - 1 downto 0);
    subtype sdram_length_type is
        natural range 0 to SDRAM_MAX_BURST_LENGTH;

    type camera_ports_type is record
        CAM_VSYNC : std_logic;
        CAM_HREF  : std_logic;
        CAM_PCLK  : std_logic;
        CAM_XCLK  : std_logic;
        CAM_DIN   : std_logic_vector(7 downto 0);
        CAM_RESET : std_logic;
        CAM_PWDN  : std_logic;
    end record;
end package;
