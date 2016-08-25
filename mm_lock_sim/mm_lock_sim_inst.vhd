	component mm_lock_sim is
		port (
			clk_clk                              : in  std_logic                    := 'X';             -- clk
			reset_reset_n                        : in  std_logic                    := 'X';             -- reset_n
			camera_mm_0_conduit_clk_camera       : in  std_logic                    := 'X';             -- clk_camera
			camera_mm_0_conduit_enable_n         : in  std_logic                    := 'X';             -- enable_n
			camera_mm_0_conduit_cam_din          : in  std_logic_vector(7 downto 0) := (others => 'X'); -- cam_din
			camera_mm_0_conduit_cam_href         : in  std_logic                    := 'X';             -- cam_href
			camera_mm_0_conduit_cam_pclk         : in  std_logic                    := 'X';             -- cam_pclk
			camera_mm_0_conduit_cam_pwdn         : out std_logic;                                       -- cam_pwdn
			camera_mm_0_conduit_cam_reset        : out std_logic;                                       -- cam_reset
			camera_mm_0_conduit_cam_vsync        : in  std_logic                    := 'X';             -- cam_vsync
			camera_mm_0_conduit_cam_xclk         : out std_logic;                                       -- cam_xclk
			camera_mm_0_buffer_port_address      : in  std_logic_vector(1 downto 0) := (others => 'X'); -- address
			vga_mm_0_conduit_vga_b               : out std_logic_vector(7 downto 0);                    -- vga_b
			vga_mm_0_conduit_vga_blank_n         : out std_logic;                                       -- vga_blank_n
			vga_mm_0_conduit_vga_clk             : out std_logic;                                       -- vga_clk
			vga_mm_0_conduit_vga_g               : out std_logic_vector(7 downto 0);                    -- vga_g
			vga_mm_0_conduit_vga_hs              : out std_logic;                                       -- vga_hs
			vga_mm_0_conduit_vga_r               : out std_logic_vector(7 downto 0);                    -- vga_r
			vga_mm_0_conduit_vga_sync_n          : out std_logic;                                       -- vga_sync_n
			vga_mm_0_conduit_vga_vs              : out std_logic;                                       -- vga_vs
			vga_mm_0_conduit_clk_vga             : in  std_logic                    := 'X';             -- clk_vga
			vga_mm_0_conduit_buffer_buffer_port  : in  std_logic_vector(1 downto 0) := (others => 'X'); -- buffer_port
			vga_mm_0_conduit_buffer_buffer_vsync : out std_logic                                        -- buffer_vsync
		);
	end component mm_lock_sim;

	u0 : component mm_lock_sim
		port map (
			clk_clk                              => CONNECTED_TO_clk_clk,                              --                     clk.clk
			reset_reset_n                        => CONNECTED_TO_reset_reset_n,                        --                   reset.reset_n
			camera_mm_0_conduit_clk_camera       => CONNECTED_TO_camera_mm_0_conduit_clk_camera,       --     camera_mm_0_conduit.clk_camera
			camera_mm_0_conduit_enable_n         => CONNECTED_TO_camera_mm_0_conduit_enable_n,         --                        .enable_n
			camera_mm_0_conduit_cam_din          => CONNECTED_TO_camera_mm_0_conduit_cam_din,          --                        .cam_din
			camera_mm_0_conduit_cam_href         => CONNECTED_TO_camera_mm_0_conduit_cam_href,         --                        .cam_href
			camera_mm_0_conduit_cam_pclk         => CONNECTED_TO_camera_mm_0_conduit_cam_pclk,         --                        .cam_pclk
			camera_mm_0_conduit_cam_pwdn         => CONNECTED_TO_camera_mm_0_conduit_cam_pwdn,         --                        .cam_pwdn
			camera_mm_0_conduit_cam_reset        => CONNECTED_TO_camera_mm_0_conduit_cam_reset,        --                        .cam_reset
			camera_mm_0_conduit_cam_vsync        => CONNECTED_TO_camera_mm_0_conduit_cam_vsync,        --                        .cam_vsync
			camera_mm_0_conduit_cam_xclk         => CONNECTED_TO_camera_mm_0_conduit_cam_xclk,         --                        .cam_xclk
			camera_mm_0_buffer_port_address      => CONNECTED_TO_camera_mm_0_buffer_port_address,      -- camera_mm_0_buffer_port.address
			vga_mm_0_conduit_vga_b               => CONNECTED_TO_vga_mm_0_conduit_vga_b,               --        vga_mm_0_conduit.vga_b
			vga_mm_0_conduit_vga_blank_n         => CONNECTED_TO_vga_mm_0_conduit_vga_blank_n,         --                        .vga_blank_n
			vga_mm_0_conduit_vga_clk             => CONNECTED_TO_vga_mm_0_conduit_vga_clk,             --                        .vga_clk
			vga_mm_0_conduit_vga_g               => CONNECTED_TO_vga_mm_0_conduit_vga_g,               --                        .vga_g
			vga_mm_0_conduit_vga_hs              => CONNECTED_TO_vga_mm_0_conduit_vga_hs,              --                        .vga_hs
			vga_mm_0_conduit_vga_r               => CONNECTED_TO_vga_mm_0_conduit_vga_r,               --                        .vga_r
			vga_mm_0_conduit_vga_sync_n          => CONNECTED_TO_vga_mm_0_conduit_vga_sync_n,          --                        .vga_sync_n
			vga_mm_0_conduit_vga_vs              => CONNECTED_TO_vga_mm_0_conduit_vga_vs,              --                        .vga_vs
			vga_mm_0_conduit_clk_vga             => CONNECTED_TO_vga_mm_0_conduit_clk_vga,             --                        .clk_vga
			vga_mm_0_conduit_buffer_buffer_port  => CONNECTED_TO_vga_mm_0_conduit_buffer_buffer_port,  -- vga_mm_0_conduit_buffer.buffer_port
			vga_mm_0_conduit_buffer_buffer_vsync => CONNECTED_TO_vga_mm_0_conduit_buffer_buffer_vsync  --                        .buffer_vsync
		);

