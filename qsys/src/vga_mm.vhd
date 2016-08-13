-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

use work.NEEE.all;

entity vga_mm is
	generic(
		constant DATA_WIDTH        : positive := 24;
		constant FIFO_LENGTH_LOG_2 : positive := 6;
        constant ADDR_WIDTH        : positive := 27;
		constant BANK              : std_logic_vector(1 downto 0) := "00";
		constant FRAME_PIXELS      : positive := 640 * 480
	);
	port(
        ---- Avalon-MM Master Interface ----
        clk           : in  std_logic;
        reset_n       : in  std_logic;

        address       : out std_logic_vector(ADDR_WIDTH - 1 downto 0);
        read          : out std_logic;
        readdata      : in  std_logic_vector(DATA_WIDTH - 1 downto 0);
        waitrequest   : in  std_logic;
        readdatavalid : in  std_logic;
        burstcount    : out std_logic_vector(FIFO_LENGTH_LOG_2 downto 0);

        ---- VGA ----
		VGA_B         : out std_logic_vector(7 downto 0);
		VGA_BLANK_N   : out std_logic;
		VGA_CLK       : out std_logic;
		VGA_G         : out std_logic_vector(7 downto 0);
		VGA_HS        : out std_logic;
		VGA_R         : out std_logic_vector(7 downto 0);
		VGA_SYNC_N    : out std_logic;
		VGA_VS        : out std_logic;

        ---- Exported Connections ----
        clk_vga       : in  std_logic;
        
        ---- Frame Buffer Signals ----
        vsync_out     : out std_logic;
        buffer_port   : in  std_logic_vector(1 downto 0)
	);
end entity vga_mm;

architecture bhv of vga_mm is
    subtype color_type is std_logic_vector(7 downto 0);

    signal vga_data: std_logic_vector(DATA_WIDTH - 1 downto 0);

    signal disp_ena, next_disp_ena: std_logic;
    signal vsync: std_logic;

    signal addr_reset_n: std_logic;
    signal addr_fetch: std_logic;
    signal addr_gen: std_logic_vector(ADDR_WIDTH - 1 downto 0) := (others => '0');

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

        if disp_ena then
            rgb := vga_data(23 downto 0);
        else
            rgb := (others => '0');
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

    -- This process has a good reason to be a separate one from 2-process
    -- template - it works at Avalon-MM clock domain rather than the VGA one.
    P_addr_gen : process(clk, addr_reset_n)
        constant MAX_COLS : positive := (VIEW_WIDTH / (2 ** FIFO_LENGTH_LOG_2));
        variable row      : unsigned(8 downto 0) := (others => '0');
        variable col      : unsigned((9 - FIFO_LENGTH_LOG_2) downto 0) := (others => '0');
    begin
        if not addr_reset_n then
            row := (others => '0');
            col := (others => '0');
        elsif rising_edge(clk) then
            if addr_fetch then
                if col = MAX_COLS - 1 then
                    col := (others => '0');
                    if row = VIEW_HEIGHT - 1 then
                        row := (others => '0');
                    else
                        row := row + 1;
                    end if;
                else
                    col := col + 1;
                end if;
            end if;
        end if;
        addr_gen(20 downto 2) <= std_logic_vector(row) & std_logic_vector(col) &
                                 std_logic_vector(to_unsigned(0, FIFO_LENGTH_LOG_2));
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

    U_buffer: entity work.mm_read_buffer
        generic map (
            DATA_WIDTH => DATA_WIDTH,
            ADDR_WIDTH => 21,
            FIFO_LENGTH_LOG_2 => FIFO_LENGTH_LOG_2,
            FRAME_PIXELS => FRAME_PIXELS
        ) port map (
            -- Avalon-MM Master Interface --
            -- signals MUST be identical or that indicates an error
            clk           => clk,
            reset_n       => reset_n,
            address       => address(20 downto 0),
            read          => read,
            readdata      => readdata,
            waitrequest   => waitrequest,
            readdatavalid => readdatavalid,
            burstcount    => burstcount,
            vsync_out     => vsync_out,
            -- End of Avalon-MM Master Interface
            s_clk         => clk_vga,
            s_read        => next_disp_ena,
            s_readdata    => vga_data,
            s_vsync       => vsync,
            addr_reset_n  => addr_reset_n,
            addr_fetch    => addr_fetch,
            addr_gen      => addr_gen(20 downto 0)
        );
    
    address(26 downto 21) <= BANK & "00" & buffer_port;
    
end architecture bhv;
