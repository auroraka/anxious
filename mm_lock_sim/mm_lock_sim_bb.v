
module mm_lock_sim (
	clk_clk,
	reset_reset_n,
	camera_mm_0_conduit_clk_camera,
	camera_mm_0_conduit_enable_n,
	camera_mm_0_conduit_cam_din,
	camera_mm_0_conduit_cam_href,
	camera_mm_0_conduit_cam_pclk,
	camera_mm_0_conduit_cam_pwdn,
	camera_mm_0_conduit_cam_reset,
	camera_mm_0_conduit_cam_vsync,
	camera_mm_0_conduit_cam_xclk,
	camera_mm_0_buffer_port_address,
	vga_mm_0_conduit_vga_b,
	vga_mm_0_conduit_vga_blank_n,
	vga_mm_0_conduit_vga_clk,
	vga_mm_0_conduit_vga_g,
	vga_mm_0_conduit_vga_hs,
	vga_mm_0_conduit_vga_r,
	vga_mm_0_conduit_vga_sync_n,
	vga_mm_0_conduit_vga_vs,
	vga_mm_0_conduit_clk_vga,
	vga_mm_0_conduit_buffer_buffer_port,
	vga_mm_0_conduit_buffer_buffer_vsync);	

	input		clk_clk;
	input		reset_reset_n;
	input		camera_mm_0_conduit_clk_camera;
	input		camera_mm_0_conduit_enable_n;
	input	[7:0]	camera_mm_0_conduit_cam_din;
	input		camera_mm_0_conduit_cam_href;
	input		camera_mm_0_conduit_cam_pclk;
	output		camera_mm_0_conduit_cam_pwdn;
	output		camera_mm_0_conduit_cam_reset;
	input		camera_mm_0_conduit_cam_vsync;
	output		camera_mm_0_conduit_cam_xclk;
	input	[1:0]	camera_mm_0_buffer_port_address;
	output	[7:0]	vga_mm_0_conduit_vga_b;
	output		vga_mm_0_conduit_vga_blank_n;
	output		vga_mm_0_conduit_vga_clk;
	output	[7:0]	vga_mm_0_conduit_vga_g;
	output		vga_mm_0_conduit_vga_hs;
	output	[7:0]	vga_mm_0_conduit_vga_r;
	output		vga_mm_0_conduit_vga_sync_n;
	output		vga_mm_0_conduit_vga_vs;
	input		vga_mm_0_conduit_clk_vga;
	input	[1:0]	vga_mm_0_conduit_buffer_buffer_port;
	output		vga_mm_0_conduit_buffer_buffer_vsync;
endmodule
