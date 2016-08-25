	mm_lock_sim u0 (
		.clk_clk                              (<connected-to-clk_clk>),                              //                     clk.clk
		.reset_reset_n                        (<connected-to-reset_reset_n>),                        //                   reset.reset_n
		.camera_mm_0_conduit_clk_camera       (<connected-to-camera_mm_0_conduit_clk_camera>),       //     camera_mm_0_conduit.clk_camera
		.camera_mm_0_conduit_enable_n         (<connected-to-camera_mm_0_conduit_enable_n>),         //                        .enable_n
		.camera_mm_0_conduit_cam_din          (<connected-to-camera_mm_0_conduit_cam_din>),          //                        .cam_din
		.camera_mm_0_conduit_cam_href         (<connected-to-camera_mm_0_conduit_cam_href>),         //                        .cam_href
		.camera_mm_0_conduit_cam_pclk         (<connected-to-camera_mm_0_conduit_cam_pclk>),         //                        .cam_pclk
		.camera_mm_0_conduit_cam_pwdn         (<connected-to-camera_mm_0_conduit_cam_pwdn>),         //                        .cam_pwdn
		.camera_mm_0_conduit_cam_reset        (<connected-to-camera_mm_0_conduit_cam_reset>),        //                        .cam_reset
		.camera_mm_0_conduit_cam_vsync        (<connected-to-camera_mm_0_conduit_cam_vsync>),        //                        .cam_vsync
		.camera_mm_0_conduit_cam_xclk         (<connected-to-camera_mm_0_conduit_cam_xclk>),         //                        .cam_xclk
		.camera_mm_0_buffer_port_address      (<connected-to-camera_mm_0_buffer_port_address>),      // camera_mm_0_buffer_port.address
		.vga_mm_0_conduit_vga_b               (<connected-to-vga_mm_0_conduit_vga_b>),               //        vga_mm_0_conduit.vga_b
		.vga_mm_0_conduit_vga_blank_n         (<connected-to-vga_mm_0_conduit_vga_blank_n>),         //                        .vga_blank_n
		.vga_mm_0_conduit_vga_clk             (<connected-to-vga_mm_0_conduit_vga_clk>),             //                        .vga_clk
		.vga_mm_0_conduit_vga_g               (<connected-to-vga_mm_0_conduit_vga_g>),               //                        .vga_g
		.vga_mm_0_conduit_vga_hs              (<connected-to-vga_mm_0_conduit_vga_hs>),              //                        .vga_hs
		.vga_mm_0_conduit_vga_r               (<connected-to-vga_mm_0_conduit_vga_r>),               //                        .vga_r
		.vga_mm_0_conduit_vga_sync_n          (<connected-to-vga_mm_0_conduit_vga_sync_n>),          //                        .vga_sync_n
		.vga_mm_0_conduit_vga_vs              (<connected-to-vga_mm_0_conduit_vga_vs>),              //                        .vga_vs
		.vga_mm_0_conduit_clk_vga             (<connected-to-vga_mm_0_conduit_clk_vga>),             //                        .clk_vga
		.vga_mm_0_conduit_buffer_buffer_port  (<connected-to-vga_mm_0_conduit_buffer_buffer_port>),  // vga_mm_0_conduit_buffer.buffer_port
		.vga_mm_0_conduit_buffer_buffer_vsync (<connected-to-vga_mm_0_conduit_buffer_buffer_vsync>)  //                        .buffer_vsync
	);

