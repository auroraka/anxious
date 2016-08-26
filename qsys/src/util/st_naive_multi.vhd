-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity st_naive_multi is
    generic (
        DIN_DATA_WIDTH: positive := 24;
        DOUT0_DATA_WIDTH: positive := 24;
        DOUT1_DATA_WIDTH: positive := 32
    );
    port (
        clk     : in  std_logic;
        reset_n : in  std_logic;

        -- Non-stoppable Avalon-ST Sink Interface
        din_data    : in  std_logic_vector(DIN_DATA_WIDTH - 1 downto 0);
        din_valid   : in  std_logic;
        din_sop     : in  std_logic;
        din_eop     : in  std_logic;

        -- Non-stoppable Avalon-ST Source Interface
        dout0_data  : out std_logic_vector(DOUT0_DATA_WIDTH - 1 downto 0);
        dout0_valid : out std_logic;
        dout0_sop   : out std_logic;
        dout0_eop   : out std_logic;

        -- Non-stoppable Avalon-ST Source Interface
        dout1_data  : out std_logic_vector(DOUT1_DATA_WIDTH - 1 downto 0);
        dout1_valid : out std_logic;
        dout1_sop   : out std_logic;
        dout1_eop   : out std_logic
    );
end entity st_naive_multi;

architecture bhv of st_naive_multi is
begin
    P_comb: process(all)
    begin
        dout0_data                              <= (others => '0');
        dout0_data(DIN_DATA_WIDTH - 1 downto 0) <= din_data;
        dout0_valid                             <= din_valid;
        dout0_sop                               <= din_sop;
        dout0_eop                               <= din_eop;

        dout1_data                              <= (others => '0');
        dout1_data(DIN_DATA_WIDTH - 1 downto 0) <= din_data;
        dout1_valid                             <= din_valid;
        dout1_sop                               <= din_sop;
        dout1_eop                               <= din_eop;
    end process;
end architecture bhv;
