onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -expand -group Camera /anxious_qsys_tb/anxious_qsys_inst/camera_mm_0/address
add wave -noupdate -expand -group Camera /anxious_qsys_tb/anxious_qsys_inst/camera_mm_0/write
add wave -noupdate -expand -group Camera /anxious_qsys_tb/anxious_qsys_inst/camera_mm_0/writedata
add wave -noupdate -expand -group Camera /anxious_qsys_tb/anxious_qsys_inst/camera_mm_0/waitrequest
add wave -noupdate -expand -group Camera /anxious_qsys_tb/anxious_qsys_inst/camera_mm_0/vsync_out
add wave -noupdate -expand -group {Camera external} /anxious_qsys_tb/anxious_qsys_inst/camera_mm_0/CAM_VSYNC
add wave -noupdate -expand -group {Camera external} /anxious_qsys_tb/anxious_qsys_inst/camera_mm_0/CAM_HREF
add wave -noupdate -expand -group {Camera external} /anxious_qsys_tb/anxious_qsys_inst/camera_mm_0/CAM_PCLK
add wave -noupdate -expand -group {Camera external} /anxious_qsys_tb/anxious_qsys_inst/camera_mm_0/CAM_XCLK
add wave -noupdate -expand -group {Camera external} /anxious_qsys_tb/anxious_qsys_inst/camera_mm_0/CAM_DIN
add wave -noupdate -expand -group {Camera external} /anxious_qsys_tb/anxious_qsys_inst/camera_mm_0/cam_we
add wave -noupdate -expand -group {Frame buffer} /anxious_qsys_tb/anxious_qsys_inst/frame_buffer_switcher_0/write_port
add wave -noupdate -expand -group {Frame buffer} /anxious_qsys_tb/anxious_qsys_inst/frame_buffer_switcher_0/read_port
add wave -noupdate -expand -group {Frame buffer} /anxious_qsys_tb/anxious_qsys_inst/frame_buffer_switcher_0/idle_port
add wave -noupdate -expand -group {Frame buffer} /anxious_qsys_tb/anxious_qsys_inst/frame_buffer_switcher_0/switch_ready_r
add wave -noupdate -expand -group VGA /anxious_qsys_tb/anxious_qsys_inst/vga_mm_0/address
add wave -noupdate -expand -group VGA /anxious_qsys_tb/anxious_qsys_inst/vga_mm_0/read
add wave -noupdate -expand -group VGA /anxious_qsys_tb/anxious_qsys_inst/vga_mm_0/readdata
add wave -noupdate -expand -group VGA /anxious_qsys_tb/anxious_qsys_inst/vga_mm_0/waitrequest
add wave -noupdate -expand -group VGA /anxious_qsys_tb/anxious_qsys_inst/vga_mm_0/vsync_out
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {0 ps} 0}
quietly wave cursor active 0
configure wave -namecolwidth 441
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {0 ps} {780 ps}
