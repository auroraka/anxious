-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity st_naive_mux is
    generic (
        DATA_WIDTH : positive := 32
    );
    port (
        reset_n                : in  std_logic;

        clk                    : in  std_logic;
        asi_din0_data          : in  std_logic_vector(DATA_WIDTH - 1 downto 0);
        asi_din0_startofpacket : in  std_logic;
        asi_din0_endofpacket   : in  std_logic;
        asi_din0_valid         : in  std_logic;

        clk_1                  : in  std_logic;
        asi_din1_data          : in  std_logic_vector(DATA_WIDTH - 1 downto 0);
        asi_din1_startofpacket : in  std_logic;
        asi_din1_endofpacket   : in  std_logic;
        asi_din1_valid         : in  std_logic;

        clk_o                  : out std_logic;
        aso_dout_data          : out std_logic_vector(DATA_WIDTH - 1 downto 0);
        aso_dout_startofpacket : out std_logic;
        aso_dout_endofpacket   : out std_logic;
        aso_dout_valid         : out std_logic;

        coe_conduit_sel        : in  std_logic
    );
end entity st_naive_mux;

architecture bhv of st_naive_mux is
begin
    P_comb: process(all)
    begin
        if not coe_conduit_sel then
            clk_o                  <= clk;
            aso_dout_data          <= asi_din0_data;
            aso_dout_startofpacket <= asi_din0_startofpacket;
            aso_dout_endofpacket   <= asi_din0_endofpacket;
            aso_dout_valid         <= asi_din0_valid;
        else
            clk_o                  <= clk_1;
            aso_dout_data          <= asi_din1_data;
            aso_dout_startofpacket <= asi_din1_startofpacket;
            aso_dout_endofpacket   <= asi_din1_endofpacket;
            aso_dout_valid         <= asi_din1_valid;
        end if;
    end process;

end architecture bhv;
