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
    type reg_type is record
        ena   : std_logic;
        data  : std_logic_vector(DATA_WIDTH - 1 downto 0);
        sop   : std_logic;
        eop   : std_logic;
        valid : std_logic;
    end record;

    constant INIT_REGS: reg_type := (
        ena   => '0',
        data  => (others => '0'),
        sop   => '0',
        eop   => '0',
        valid => '0'
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
    begin
        -- default: hold the values
        v := r;

        if not r.ena and asi_din_startofpacket then
            v.ena := coe_conduit_sel;
        end if;

        if v.ena then
            v.data  := asi_din_data;
            v.sop   := asi_din_startofpacket;
            v.eop   := asi_din_endofpacket;
            v.valid := asi_din_valid and coe_conduit_sel;
        else
            v := INIT_REGS;
        end if;

        -- apply the new values
        rin <= v;
    end process;

end architecture bhv;
