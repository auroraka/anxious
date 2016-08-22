-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity bitpacker is
    generic (
        constant PACK_LENGTH : positive := 8
    );
    port (
        clk     : in  std_logic;
        reset_n : in  std_logic;

        ---- Avalon-ST Sink Interface
        din_valid  : in std_logic;
        din_data   : in std_logic_vector(0 downto 0);
        din_sop    : in std_logic;
        din_eop    : in std_logic;

        ---- Avalon-ST Source Interface
        dout_valid : out std_logic;
        dout_data  : out std_logic_vector(PACK_LENGTH - 1 downto 0);
        dout_sop   : out std_logic;
        dout_eop   : out std_logic
    );
end entity bitpacker;

architecture bhv of bitpacker is
    type reg_type is record
        shift_data : std_logic_vector(PACK_LENGTH - 1 downto 0);
        cnt        : natural range 0 to PACK_LENGTH - 1;
        valid      : std_logic;
        dout       : std_logic_vector(PACK_LENGTH - 1 downto 0);
        sop        : std_logic;
        eop        : std_logic;
    end record;

    constant INIT_REGS: reg_type := (
        shift_data => (others => '0'),
        cnt        => 0,
        valid      => '0',
        dout       => (others => '0'),
        sop        => '0',
        eop        => '0'
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
        variable v: reg_type;
    begin
        -- default: hold the values
        v := r;

        v.valid := '0';
        v.sop := din_sop;
        v.eop := din_eop;

        if din_valid then
            v.shift_data := r.shift_data(r.shift_data'left - 1 downto 0) &
                din_data(0);
            if r.cnt = PACK_LENGTH - 1 then
                v.cnt   := 0;
                v.valid := '1';
                v.dout  := v.shift_data;
            else
                v.cnt := r.cnt + 1;
            end if;
        end if;

        if din_eop then
            v.cnt := 0;
        end if;

        dout_valid <= r.valid;
        dout_data  <= r.dout;
        dout_sop   <= r.sop;
        dout_eop   <= r.eop;

        -- apply the new values
        rin <= v;
    end process;

end architecture bhv;
