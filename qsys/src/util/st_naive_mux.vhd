-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity st_naive_mux is
    generic (
        DATA_WIDTH : positive := 32
    );
    port (
        clk                    : in  std_logic;
        clk_1                  : in  std_logic;
        reset_n                : in  std_logic;

        asi_din0_data          : in  std_logic_vector(DATA_WIDTH - 1 downto 0);
        asi_din0_startofpacket : in  std_logic;
        asi_din0_endofpacket   : in  std_logic;
        asi_din0_valid         : in  std_logic;

        asi_din1_data          : in  std_logic_vector(DATA_WIDTH - 1 downto 0);
        asi_din1_startofpacket : in  std_logic;
        asi_din1_endofpacket   : in  std_logic;
        asi_din1_valid         : in  std_logic;

        aso_dout_data          : out std_logic_vector(DATA_WIDTH - 1 downto 0);
        aso_dout_startofpacket : out std_logic;
        aso_dout_endofpacket   : out std_logic;
        aso_dout_valid         : out std_logic;

        coe_conduit_sel        : in  std_logic
    );
end entity st_naive_mux;

architecture bhv of st_naive_mux is
    type reg_type is record
        sel: std_logic;
        ena: std_logic;
    end record;

    constant INIT_REGS: reg_type := (
        sel => '0',
        ena => '0'
    );

    signal r: reg_type := INIT_REGS;
    signal rin: reg_type := INIT_REGS;

begin
    P_regs: process(clk, reset_n)
    begin
        if not reset_n then
            r <= INIT_REGS;
        elsif rising_edge(clk) then
            r <= rin;
        end if;
    end process;

    P_comb: process(all)
        variable v:reg_type;
        variable sop, eop: std_logic;
    begin
        -- default: hold the values
        v := r;

        if not r.sel then
            sop := asi_din0_startofpacket;
            eop := asi_din0_endofpacket;
        else
            sop := asi_din1_startofpacket;
            eop := asi_din1_endofpacket;
        end if;

        if r.ena and eop then
            v.ena := '0';
            v.sel := coe_conduit_sel;
        end if;

        if not v.ena and sop then
            v.ena := '1';
        end if;

        if v.ena then
            if not r.sel then
                aso_dout_data          <= asi_din0_data;
                aso_dout_startofpacket <= asi_din0_startofpacket;
                aso_dout_endofpacket   <= asi_din0_endofpacket;
                aso_dout_valid         <= asi_din0_valid;
            else
                aso_dout_data          <= asi_din1_data;
                aso_dout_startofpacket <= asi_din1_startofpacket;
                aso_dout_endofpacket   <= asi_din1_endofpacket;
                aso_dout_valid         <= asi_din1_valid;
            end if;
        else
            aso_dout_data          <= (others => '0');
            aso_dout_startofpacket <= '0';
            aso_dout_endofpacket   <= '0';
            aso_dout_valid         <= '0';
        end if;

        -- apply the new values
        rin <= v;
    end process;

end architecture bhv;
