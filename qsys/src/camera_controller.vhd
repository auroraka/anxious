-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

use work.NEEE.all;

entity camera_controller is
    generic(
        DATA_WIDTH        : positive := 24;
        FIFO_LENGTH_LOG_2 : positive := 6;
        ADDR_WIDTH        : positive := 27;
        BANK              : std_logic_vector(1 downto 0) := "00";
        FRAME_PIXELS      : positive := 640 * 480
    );
    port(
        ---- Avalon-MM Master Interface ----
        clk         : in  std_logic;
        reset_n     : in  std_logic;

        address     : out std_logic_vector(ADDR_WIDTH - 1 downto 0);
        write       : out std_logic;
        writedata   : out std_logic_vector(DATA_WIDTH - 1 downto 0);
        waitrequest : in  std_logic;
        lock        : out std_logic;

        ---- Camera side ----
        clk_camera  : in  std_logic;
        -- Qsys does not support custom types, unrolling the camera_ports_type
        CAM_VSYNC   : in  std_logic;
        CAM_HREF    : in  std_logic;
        CAM_PCLK    : in  std_logic;
        CAM_XCLK    : out std_logic;
        CAM_DIN     : in  std_logic_vector(7 downto 0);
        CAM_RESET   : out std_logic;
        CAM_PWDN    : out std_logic;

        ---- Exported Connections ----
        enable_n    : in  std_logic;

        ---- Frame Buffer Signals ----
        vsync_out   : out std_logic;
        buffer_port : in  std_logic_vector(1 downto 0);

        black_cnt   : out std_logic_vector(19 downto 0)
    );
end entity camera_controller;

architecture camera_controller_bhv of camera_controller is
    subtype camera_pixel is std_logic_vector(DATA_WIDTH - 1 downto 0);

    signal pclk      : std_logic;
    signal cam_we    : std_logic;
    signal cam_data  : camera_pixel := (others => '0');
    signal row       : unsigned(8 downto 0);
    signal col       : unsigned(9 downto 0);
    signal addr      : std_logic_vector(ADDR_WIDTH - 1 downto 0) := (others => '0');
    signal vsync     : std_logic;

    signal black_cnt_u : unsigned(19 downto 0);
begin
    U_camera : entity work.camera port map(
            clk_camera => clk_camera,
            reset_n    => reset_n,
            CAM_VSYNC  => CAM_VSYNC,
            CAM_HREF   => CAM_HREF,
            CAM_PCLK   => CAM_PCLK,
            CAM_XCLK   => CAM_XCLK,
            CAM_DIN    => CAM_DIN,
            CAM_RESET  => CAM_RESET,
            CAM_PWDN   => CAM_PWDN,
            enable_n   => enable_n,
            CLK_o      => pclk,
            WE_o       => cam_we,
            DATA_o     => cam_data(23 downto 0),
            ROW_o      => row,
            COL_o      => col,
            VSYNC_o    => vsync,

            black_cnt  => black_cnt_u
        );
    black_cnt <= std_logic_vector(black_cnt_u);

    addr(20 downto 2) <= std_logic_vector(row) & std_logic_vector(col);

    U_buffer : entity work.mm_write_buffer
        generic map (
            DATA_WIDTH        => DATA_WIDTH,
            ADDR_WIDTH        => 21,
            FIFO_LENGTH_LOG_2 => FIFO_LENGTH_LOG_2,
            FRAME_PIXELS      => FRAME_PIXELS
        ) port map (
            -- Avalon-MM Master Interface --
            -- signals MUST be identical or there's an error --
            clk         => clk,
            reset_n     => reset_n,
            address     => address(20 downto 0),
            write       => write,
            writedata   => writedata,
            waitrequest => waitrequest,
            lock        => lock,
            vsync_out   => vsync_out,
            -- End of Avalon-MM Master Interface --
            s_clk       => pclk,
            s_write     => cam_we,
            s_writedata => cam_data,
            s_addr      => addr(20 downto 0)
        );

	address(26 downto 21) <= BANK & "00" & buffer_port;

end architecture camera_controller_bhv;
