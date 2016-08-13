-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

use work.NEEE.all;

entity camera_st is
	port(
        clk_camera : in  std_logic;
        reset_n    : in  std_logic;

        ---- Camera Wires ----
        CAM_VSYNC   : in  std_logic;
        CAM_HREF    : in  std_logic;
        CAM_PCLK    : in  std_logic;
        CAM_XCLK    : out std_logic;
        CAM_DIN     : in  std_logic_vector(7 downto 0);
        CAM_RESET   : out std_logic;
        CAM_PWDN    : out std_logic;

        ---- Exported Connections ----
		enable_n   : in  std_logic;    -- Capture enable
		CLK_o      : out std_logic;    -- Write clock
		WE_o       : out std_logic;    -- Write enabled (active high)
		DATA_o     : out std_logic_vector(15 downto 0);
		ROW_o      : out unsigned(8 downto 0);
		COL_o      : out unsigned(9 downto 0);
		VSYNC_o    : out std_logic
	);
end entity camera_st;

architecture camera_st_bhv of camera_st is
	subtype pixel_type is std_logic_vector(7 downto 0);
	type type_cap_state is (C_IDLE, C_WAIT, C_BUSY);

    type reg_type is record
        cap_state           : type_cap_state;
        y0                  : pixel_type;
        cb, cr              : pixel_type;
        cap_x               : unsigned(8 downto 0);
        cap_y               : unsigned(9 downto 0);
        cap_count           : natural range 0 to 3;
        vsync_reg, href_reg : std_logic;
        we: std_logic;
        dout: std_logic_vector(15 downto 0);
    end record;

    constant INIT_REGS: reg_type := (
        cap_state => C_IDLE,
        y0 => (others => '0'),
        cb => (others => '0'),
        cr => (others => '0'),
        cap_x => (others => '0'),
        cap_y => (others => '0'),
        cap_count => 0,
        vsync_reg => '1',
        href_reg => '0',
        we => '0',
        dout => (others => '0')
    );

    signal r: reg_type := INIT_REGS;
    signal rin: reg_type := INIT_REGS;
begin
    P_reg: process(CAM_PCLK, reset_n)
    begin
        if not reset_n then
            r <= INIT_REGS;
        elsif rising_edge(CAM_PCLK) then
            r <= rin;
        end if;
    end process;

    P_comb: process(all)
        variable v: reg_type;
        variable tmp: std_logic_vector(9 downto 0);
		variable cap_y_mock : pixel_type;
	begin
        v := r; -- default: hold the values

        v.vsync_reg := CAM_VSYNC;
        v.href_reg  := CAM_HREF;

        -- Note that standard VHDL do NOT allow slicing of
        -- type conversion results (while quartus does)
        tmp := std_logic_vector(to_unsigned(640 - to_integer(r.cap_y), 10));
        cap_y_mock := tmp(9 downto 2);
		-- cap_y_mock := std_logic_vector(
            -- to_unsigned(640 - to_integer(r.cap_y), 10))(9 downto 2);

        v.we := '0';

        case r.cap_state is
            when C_IDLE =>
                if enable_n = '0' then
                    v.cap_state := C_WAIT;
                end if;
            when C_WAIT =>
                if r.vsync_reg and not CAM_VSYNC then
                    v.cap_state := C_BUSY;
                    v.cap_x     := (others => '0');
                    v.cap_y     := (others => '0');
                    v.cap_count := 0;
                end if;
            when C_BUSY =>
                if CAM_VSYNC then
                    v.cap_state := C_IDLE;
                end if;
                if CAM_HREF then
                    v.cap_count := (r.cap_count + 1) mod 4;
                    case r.cap_count is
                        when 0 =>
                            if r.cap_y /= 0 then
                                v.cap_y := r.cap_y + 1;
                            end if;
                            v.cb := CAM_DIN;
                        when 1 =>
                            v.we    := '1';
                            -- Convert to RGB and output
                            -- v.dout := toRGB(y0, cb, CAM_DIN);
                            v.dout  := CAM_DIN & r.cb;
                            -- Mock data for testing (overwrite previous assignments)
                            -- v.dout := cap_y_mock & cap_y_mock & cap_y_mock;
                        when 2 =>
                            v.cap_y := r.cap_y + 1;
                            v.cr := CAM_DIN;
                        when 3 =>
                            v.we    := '1';
                            -- Convert to RGB and output
                            -- DATA_o <= toRGB(CAM_DIN, cb, cr);
                            v.dout  := CAM_DIN & r.cr;
                            -- Mock data for testing (overwrite previous assignments)
                            -- DATA_o <= cap_y_mock & cap_y_mock & cap_y_mock;
                    end case;
                end if;
                if r.href_reg and not CAM_HREF then
                    v.cap_y := to_unsigned(0, 10);
                    v.cap_x := r.cap_x + 1;
                end if;
        end case;

        -- drive the outputs
        CAM_XCLK <= clk_camera;
        CAM_RESET <= reset_n;
        CAM_PWDN <= '0';
        CLK_o <= CAM_PCLK;
        VSYNC_o <= CAM_VSYNC;
        ROW_o <= r.cap_x;
        COL_o <= r.cap_y;
        WE_O  <= r.we;
        DATA_o <= r.dout;

        rin <= v; -- apply the new values
    end process;
end architecture camera_st_bhv;
