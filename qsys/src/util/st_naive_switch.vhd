-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity st_naive_switch is
    generic (
        DATA_WIDTH : positive := 32
    );
    port (
        clk                    : in  std_logic;
        reset_n                : in  std_logic;

        asi_din_data           : in  std_logic_vector(DATA_WIDTH - 1 downto 0);
        asi_din_startofpacket  : in  std_logic;
        asi_din_endofpacket    : in  std_logic;
        asi_din_valid          : in  std_logic;

        aso_dout_data          : out std_logic_vector(DATA_WIDTH - 1 downto 0);
        aso_dout_startofpacket : out std_logic;
        aso_dout_endofpacket   : out std_logic;
        aso_dout_valid         : out std_logic;

        coe_conduit_sel        : in  std_logic
    );
end entity st_naive_switch;

architecture bhv of st_naive_switch is
begin
    P_comb: process(all)
    begin
        aso_dout_data          <= asi_din_data;
        aso_dout_startofpacket <= asi_din_startofpacket;
        aso_dout_endofpacket   <= asi_din_endofpacket;
        aso_dout_valid         <= asi_din_valid and coe_conduit_sel;
    end process;

end architecture bhv;
