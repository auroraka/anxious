-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

use work.NEEE.all;

entity camera_controller_up is
    generic(
        DATA_WIDTH        : positive := 16;
        FIFO_LENGTH_LOG_2 : positive := 8
    );
    port(
        ---- Avalon-ST Video Interface
        clk         : in  std_logic;
        reset_n     : in  std_logic;
        
        ready       : in  std_logic;
        valid       : out std_logic;
        sop         : out std_logic;
        eop         : out std_logic;
        data        : out std_logic_vector(DATA_WIDTH - 1 downto 0);

        -- Camera side --
        clk_camera  : in  std_logic;
        -- Qsys does not support custom types, unrolling the camera_ports_type
        CAM_VSYNC   : in  std_logic;
        CAM_HREF    : in  std_logic;
        CAM_PCLK    : in  std_logic;
        CAM_XCLK    : out std_logic;
        CAM_DIN     : in  std_logic_vector(7 downto 0);
        CAM_RESET   : out std_logic;
        CAM_PWDN    : out std_logic;

        -- Exported Connections
        enable_n    : in  std_logic
    );
end entity camera_controller_up;

architecture camera_controller_up_bhv of camera_controller_up is
    subtype camera_pixel is std_logic_vector(15 downto 0);

    signal pclk      : std_logic;
    signal cam_we    : std_logic;
    signal cam_data  : camera_pixel := (others => '0');
    signal row       : unsigned(8 downto 0);
    signal col       : unsigned(9 downto 0);
    signal vsync     : std_logic;

begin
    U_camera : entity work.camera_24 port map(
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
            DATA_o     => cam_data(15 downto 0),
            ROW_o      => row,
            COL_o      => col,
            VSYNC_o    => vsync
        );

    U_buffer : entity work.up_write_buffer
        generic map (
            DATA_WIDTH        => DATA_WIDTH,
            FIFO_LENGTH_LOG_2 => FIFO_LENGTH_LOG_2
        ) port map (
            -- Avalon-ST Video Interface --
            -- signals MUST be identical or there's an error --
            clk         => clk,
            reset_n     => reset_n,
            ready       => ready,
            valid       => valid,
            sop         => sop,
            eop         => eop,
            data        => data,
            -- End of Avalon-MM Master Interface --
            s_clk       => pclk,
            s_write     => cam_we,
            s_writedata => cam_data
        );

end architecture camera_controller_up_bhv;
