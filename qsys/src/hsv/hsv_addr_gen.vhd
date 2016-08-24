-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity hsv_addr_gen is
    generic (
        ADDR_BASE  : std_logic_vector(31 downto 0) := (others => '0');
        ADDR_WIDTH : positive := 32;
        HEIGHT     : positive := 480;
        WIDTH      : positive := 640
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
end entity hsv_addr_gen;

architecture bhv of hsv_addr_gen is
    subtype addr_type is unsigned(ADDR_WIDTH - 1 downto 0);

    constant ADDR_BASE_CLIPPED: addr_type :=
                unsigned(ADDR_BASE(ADDR_WIDTH - 1 downto 0));

    constant HSV_FRAME_SIZE: addr_type :=
                to_unsigned(640 * 480 / 8, ADDR_WIDTH);

    type reg_type is record
        addr      : addr_type;
        vsync_reg : std_logic;
    end record;

    constant INIT_REGS: reg_type := (
        addr      => ADDR_BASE_CLIPPED,
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
        variable offset: addr_type;
    begin
        -- default: hold the values
        v := r;

        v.vsync_reg := addr_vsync;

        buffer_vsync <= addr_vsync;

        if addr_fetch then
            v.addr := r.addr + 1;
        end if;

        -- NOTE: we place this check AFTER `addr_fetch' check, this is
        -- intended, as `addr_vsync' might come with last `valid' data
        if not r.vsync_reg and addr_vsync then
            v.addr := ADDR_BASE_CLIPPED;
        end if;

        offset := resize(unsigned(buffer_port) * HSV_FRAME_SIZE, ADDR_WIDTH);
        addr_gen <= std_logic_vector(resize(r.addr + offset, ADDR_WIDTH));

        -- apply the new values
        rin <= v;
    end process;

end architecture bhv;
