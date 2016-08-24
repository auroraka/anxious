-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity default_addr_gen is
    generic (
        ADDR_WIDTH : positive := 27;
        HEIGHT     : positive := 480;
        WIDTH      : positive := 640;
        BANK       : std_logic_vector(1 downto 0) := "00"
    );
    port (
        clk          : in  std_logic;
        reset_n      : in  std_logic;

        addr_fetch   : in  std_logic;
        addr_vsync   : in  std_logic;
        addr_gen     : out std_logic_vector(ADDR_WIDTH - 1 downto 0);

        buffer_port  : in  std_logic_vector(1 downto 0);
        buffer_vsync : out std_logic
    );
end entity default_addr_gen;

architecture bhv of default_addr_gen is
    type reg_type is record
        row       : natural range 0 to HEIGHT - 1;
        col       : natural range 0 to WIDTH - 1;
        vsync_reg : std_logic;
    end record;

    constant INIT_REGS: reg_type := (
        row       => 0,
        col       => 0,
        vsync_reg => '0'
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

        buffer_vsync <= addr_vsync;

        v.vsync_reg := addr_vsync;

        if addr_fetch then
            if r.col = WIDTH - 1 then
                v.col := 0;
                v.row := r.row + 1;
            else
                v.col := r.col + 1;
            end if;
        end if;

        -- NOTE: we place this check AFTER `addr_fetch' check, this is
        -- intended, as `addr_vsync' might come with last `valid' data
        if not r.vsync_reg and addr_vsync then
            v.row := 0;
            v.col := 0;
        end if;

        addr_gen(26 downto 0) <= BANK & "00" & buffer_port &
            std_logic_vector(to_unsigned(r.row, 9)) &
            std_logic_vector(to_unsigned(r.col, 10)) & "00";

        -- apply the new values
        rin <= v;
    end process;

end architecture bhv;
