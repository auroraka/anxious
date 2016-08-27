-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity bit_unpacker is
    generic (
        DIN_WIDTH  : positive := 8;
        DOUT_WIDTH : positive := 4
    );
    port (
        clk        : in  std_logic;
        reset_n    : in  std_logic;

        ---- Avalon-ST Sink Interface ----
        din_ready  : out std_logic;
        din_data   : in  std_logic_vector(DIN_WIDTH - 1 downto 0);

        ---- Avalon-ST Source Interface ----
        dout_ready : in  std_logic;
        dout_data  : out std_logic_vector(DOUT_WIDTH - 1 downto 0)
    );
end entity bit_unpacker;

architecture bhv of bit_unpacker is
    constant MULTIPLIER : positive := DIN_WIDTH / DOUT_WIDTH;

    type reg_type is record
        shift_data: std_logic_vector(DIN_WIDTH - 1 downto 0);
        cnt: natural range 0 to MULTIPLIER - 1;
    end record;

    constant INIT_REGS: reg_type := (
        shift_data => (others => '0'),
        cnt => 0
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

        if r.cnt = 0 then
            dout_data <= din_data(DOUT_WIDTH - 1 downto 0);
            v.shift_data := std_logic_vector(to_unsigned(0, DOUT_WIDTH)) &
                din_data(DIN_WIDTH - 1 downto DOUT_WIDTH);
            din_ready <= dout_ready;
            if dout_ready then
                v.cnt := MULTIPLIER - 1;
            end if;
        else
            dout_data <= r.shift_data(DOUT_WIDTH - 1 downto 0);
            v.shift_data := std_logic_vector(to_unsigned(0, DOUT_WIDTH)) &
                r.shift_data(DIN_WIDTH - 1 downto DOUT_WIDTH);
            v.cnt := r.cnt - 1;
            din_ready <= '0';
        end if;

        -- apply the new values
        rin <= v;
    end process;

end architecture bhv;
