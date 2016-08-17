onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -divider Camera
add wave -noupdate /anxious_up_tb/anxious_up_inst/camera_up_0/ready
add wave -noupdate /anxious_up_tb/anxious_up_inst/camera_up_0/valid
add wave -noupdate /anxious_up_tb/anxious_up_inst/camera_up_0/sop
add wave -noupdate /anxious_up_tb/anxious_up_inst/camera_up_0/eop
add wave -noupdate /anxious_up_tb/anxious_up_inst/camera_up_0/data
add wave -noupdate -divider {Camera external}
add wave -noupdate /anxious_up_tb/anxious_up_inst/camera_up_0/CAM_VSYNC
add wave -noupdate /anxious_up_tb/anxious_up_inst/camera_up_0/CAM_HREF
add wave -noupdate /anxious_up_tb/anxious_up_inst/camera_up_0/CAM_PCLK
add wave -noupdate /anxious_up_tb/anxious_up_inst/camera_up_0/CAM_DIN
add wave -noupdate -divider {Chroma Resampler}
add wave -noupdate /anxious_up_tb/anxious_up_inst/video_chroma_resampler_0/stream_in_ready
add wave -noupdate /anxious_up_tb/anxious_up_inst/video_chroma_resampler_0/stream_out_valid
add wave -noupdate /anxious_up_tb/anxious_up_inst/video_chroma_resampler_0/stream_out_startofpacket
add wave -noupdate /anxious_up_tb/anxious_up_inst/video_chroma_resampler_0/stream_out_endofpacket
add wave -noupdate /anxious_up_tb/anxious_up_inst/video_chroma_resampler_0/stream_out_data
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {71575964 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 334
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
WaveRestoreZoom {49238125 ps} {116451982 ps}
