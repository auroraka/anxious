-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity fl_adapter is
    port (
        clk                    : in  std_logic;

        tcs_write_n_out        : in  std_logic;
        tcs_chipselect_n_out   : in  std_logic;
        tcs_outputenable_n_out : in  std_logic;
        tcs_request            : in  std_logic;
        tcs_grant              : out std_logic;
        tcs_address_out        : in  std_logic_vector(25 downto 0);
        tcs_data_out           : in  std_logic_vector(15 downto 0);
        tcs_data_outen         : in  std_logic;
        tcs_data_in            : out std_logic_vector(15 downto 0);

        tcm_write_n_out        : out std_logic;
        tcm_chipselect_n_out   : out std_logic;
        tcm_outputenable_n_out : out std_logic;
        tcm_request            : out std_logic;
        tcm_grant              : in  std_logic;
        tcm_address_out        : out std_logic_vector(26 downto 1);
        tcm_data_out           : out std_logic_vector(31 downto 0);
        tcm_data_outen         : out std_logic;
        tcm_data_in            : in  std_logic_vector(31 downto 0)
    );
end entity fl_adapter;

architecture bhv of fl_adapter is
begin
    tcm_write_n_out              <= tcs_write_n_out;
    tcm_chipselect_n_out         <= tcs_chipselect_n_out;
    tcm_outputenable_n_out       <= tcs_outputenable_n_out;
    tcm_request                  <= tcs_request;
    tcs_grant                    <= tcm_grant;
    tcm_address_out(25 downto 1) <= tcs_address_out(25 downto 1);
    tcm_data_out(15 downto 0)    <= tcs_data_out;
    tcm_data_outen               <= tcs_data_outen;
    tcs_data_in                  <= tcm_data_in(15 downto 0);
end architecture bhv;
