-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity dummy_buffer_switcher is
    generic (
        FIXED_BUFFER_PORT : std_logic_vector(1 downto 0) := "00"
    );
    port (
        clk          : in  std_logic;

        ---- Buffer Switch Signals
        buffer_vsync : in  std_logic;
        buffer_port  : out std_logic_vector(1 downto 0)
    );
end entity dummy_buffer_switcher;

architecture bhv of dummy_buffer_switcher is
begin
    buffer_port <= FIXED_BUFFER_PORT;
end architecture bhv;
