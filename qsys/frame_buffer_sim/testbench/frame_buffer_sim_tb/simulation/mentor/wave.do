onerror {resume}
quietly WaveActivateNextPane {} 0
delete wave *

set tb anxious_up_tb
set dut $tb/anxious_up_tb_inst
set camera $dut/camera_mm_0
set sdram $dut/sdram_mm_0
set frb $dut/frame_buffer_switcher_0
set vga $dut/vga_mm_0

proc wav {args} {add wave -noupdate {*}$args}
proc divider {name} {wav -divider $name}

divider Camera
wav $camera/address
wav $camera/waitrequest
wav $camera/write
wav $camera/writedata
wav $camera/vsync_out

divider {Camera external}
wav $camera/CAM_VSYNC
wav $camera/CAM_HREF
wav $camera/CAM_PCLK
wav $camera/CAM_XCLK
wav $camera/CAM_DIN
wav $camera/cam_we

divider VGA
wav $vga/address
wav $vga/waitrequest
wav $vga/read
wav $vga/readdatavalid
wav $vga/readdata
wav $vga/vsync_out

divider {Frame buffer}
wav $frb/r.write_port
wav $frb/r.read_port
wav $frb/r.idle_port
wav $frb/r.switch_ready

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

