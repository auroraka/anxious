onerror {resume}
quietly WaveActivateNextPane {} 0
delete wave *

set tb mm_lock_sim_tb
set dut $tb/mm_lock_sim_inst
set camera $dut/camera_mm_0
set sdram $dut/sdram_mm_0
set vga $dut/vga_composer_0

proc wav {args} {add wave -noupdate {*}$args}
proc divider {name} {wav -divider $name}

divider Camera
wav $camera/address
wav $camera/waitrequest
wav $camera/write
wav $camera/writedata
wav $camera/lock
wav $camera/vsync_out
wav $camera/U_buffer/r.burstcnt

divider {Camera external}
wav $camera/CAM_VSYNC
wav $camera/CAM_HREF
wav $camera/CAM_PCLK
wav $camera/CAM_XCLK
wav $camera/CAM_DIN
wav $camera/cam_we

divider {VGA Composer: Camera}
wav $vga/cam_address
wav $vga/cam_waitrequest
wav $vga/cam_read
wav $vga/cam_readdatavalid
wav $vga/cam_readdata
wav $vga/cam_lock
wav $vga/cam_vsync_out
wav $vga/U_cam_buffer/r.wrcnt

TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {480277 ps} 0}
configure wave -namecolwidth 342
configure wave -valuecolwidth 85
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
configure wave -timelineunits ps
update
WaveRestoreZoom {0 ps} {66412500 ps}

