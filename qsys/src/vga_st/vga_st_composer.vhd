-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity vga_composer is
    generic(
        constant DATA_WIDTH        : positive := 32;
        constant FIFO_LENGTH_LOG_2 : positive := 6
    );
    port(
        clk_vga      : in  std_logic;
        reset_n      : in  std_logic;

        ---- Avalon-ST Sink: Camera ----
        cam_ready    : out std_logic;
        cam_data     : in  std_logic_vector(DATA_WIDTH - 1 downto 0);

        ---- Avalon-ST Sink: Render
        render_ready : out std_logic;
        render_data  : in  std_logic_vector(DATA_WIDTH - 1 downto 0);

        ---- VGA ----
        VGA_B        : out std_logic_vector(7 downto 0);
        VGA_BLANK_N  : out std_logic;
        VGA_CLK      : out std_logic;
        VGA_G        : out std_logic_vector(7 downto 0);
        VGA_HS       : out std_logic;
        VGA_R        : out std_logic_vector(7 downto 0);
        VGA_SYNC_N   : out std_logic;
        VGA_VS       : out std_logic
    );
end entity vga_composer;

architecture bhv of vga_composer is
    subtype color_type is std_logic_vector(7 downto 0);

    signal disp_ena, next_disp_ena: std_logic;
    signal vsync: std_logic;

    -- NOTE: these regs are synchronized with VGA clk, i.e. `clk_vga`
    type reg_type is record
        vga_red: color_type;
        vga_green: color_type;
        vga_blue: color_type;
    end record;

    constant INIT_REGS: reg_type := (
        vga_red => (others => '0'),
        vga_green => (others => '0'),
        vga_blue => (others => '0')
    );

    signal r: reg_type := INIT_REGS;
    signal rin: reg_type := INIT_REGS;

begin
    P_regs: process(clk_vga, reset_n)
    begin
        if not reset_n then
            r <= INIT_REGS;
        elsif rising_edge(clk_vga) then
            r <= rin;
        end if;
    end process;

    P_comb: process(all)
        variable v: reg_type;
        variable rgb: std_logic_vector(23 downto 0);
    begin
        -- default: hold the values
        v := r;

        if not disp_ena then
            rgb := (others => '0');
        elsif render_data(24) then
            rgb := cam_data(23 downto 0);   --- !!!
        else
            rgb := render_data(23 downto 0);
        end if;

        v.vga_red   := rgb(23 downto 16);
        v.vga_green := rgb(15 downto 8);
        v.vga_blue  := rgb(7 downto 0);

        VGA_R   <= r.vga_red;
        VGA_G   <= r.vga_green;
        VGA_B   <= r.vga_blue;
        VGA_VS  <= vsync;
        VGA_CLK <= clk_vga;

        -- apply the new values
        rin <= v;
    end process;

    U_vga_ctl: entity work.vga_controller
        port map (
            pixel_clk   => clk_vga,
            reset_n     => reset_n,
            h_sync      => VGA_HS,
            v_sync      => vsync,
            next_disp_e => next_disp_ena,
            disp_ena    => disp_ena,
            column      => open,
            row         => open,
            n_blank     => VGA_BLANK_N,
            n_sync      => VGA_SYNC_N
        );

    cam_ready <= next_disp_ena;
    render_ready <= next_disp_ena;

end architecture bhv;
