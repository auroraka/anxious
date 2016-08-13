--------------------------------------------------------------------------------
--
--   FileName:         vga_controller.vhd
--   Dependencies:     none
--   Design Software:  Quartus II 64-bit Version 12.1 Build 177 SJ Full Version
--
--   HDL CODE IS PROVIDED "AS IS."  DIGI-KEY EXPRESSLY DISCLAIMS ANY
--   WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
--   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
--   PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO event SHALL DIGI-KEY
--   BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
--   DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
--   PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
--   BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
--   ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
--
--   Version History
--   Version 1.0 05/10/2013 Scott Larson
--     Initial Public Release
--
--------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;

entity vga_controller is
	generic(
		h_pixels : integer   := 640;    --horiztonal display width in pixels
		h_fp     : integer   := 16;     --horiztonal front porch width in pixels
		h_pulse  : integer   := 96;     --horiztonal sync pulse width in pixels
		h_bp     : integer   := 48;     --horiztonal back porch width in pixels
		h_pol    : std_logic := '0';    --horizontal sync pulse polarity (1 = positive, 0 = negative)

		v_pixels : integer   := 480;    --vertical display width in rows
		v_fp     : integer   := 10;     --vertical front porch width in rows
		v_pulse  : integer   := 2;      --vertical sync pulse width in rows
		v_bp     : integer   := 33;     --vertical back porch width in rows
		v_pol    : std_logic := '0'     --vertical sync pulse polarity (1 = positive, 0 = negative)
	);
	port(
		pixel_clk   : in  std_logic;    --pixel clock at frequency of VGA mode being used
		reset_n     : in  std_logic;    --active low asycnchronous reset
		h_sync      : out std_logic;    --horiztonal sync pulse
		v_sync      : out std_logic;    --vertical sync pulse
		next_disp_e : out std_logic;    --display enable in next cycle
		disp_ena    : out std_logic;    --display enable ('1' = display time, '0' = blanking time)
		column      : out integer;      --horizontal pixel coordinate
		row         : out integer;      --vertical pixel coordinate
		n_blank     : out std_logic;    --direct blacking output to DAC
		n_sync      : out std_logic     --sync-on-green output to DAC
	);
end entity vga_controller;

architecture vga_controller_bhv of vga_controller is
	constant h_period : integer := h_pulse + h_bp + h_pixels + h_fp; --total number of pixel clocks in a row
	constant v_period : integer := v_pulse + v_bp + v_pixels + v_fp; --total number of rows in column

begin
	n_blank <= '1';                     --no direct blanking
	n_sync  <= '0';                     --no sync on green

	main : process(pixel_clk, reset_n)
        ---------- Notice ----------
        -- h_count and v_count was variable, now changed to signal to
        -- output rows and columns one cycle ahead of other signals
        variable h_count : integer range 0 to h_period - 1 := 0; --horizontal counter (counts the columns)
        variable v_count : integer range 0 to v_period - 1 := 0; --vertical counter (counts the rows)
		variable next_h : integer range 0 to h_period - 1;
		variable next_v : integer range 0 to v_period - 1;
	begin
		if (reset_n = '0') then         --reset asserted
			h_count  := 0;              --reset horizontal counter
			v_count  := 0;              --reset vertical counter
			h_sync   <= not h_pol;      --deassert horizontal sync
			v_sync   <= not v_pol;      --deassert vertical sync
			disp_ena <= '0';            --disable display
			column   <= 0;              --reset column pixel coordinate
			row      <= 0;              --reset row pixel coordinate

		elsif (rising_edge(pixel_clk)) then

			--counters
			if (h_count < h_period - 1) then --horizontal counter (pixels)
				next_h := h_count + 1;
			else
				next_h := 0;
				if (v_count < v_period - 1) then --veritcal counter (rows)
					next_v := v_count + 1;
				else
					next_v := 0;
				end if;
			end if;
			h_count := next_h;
			v_count := next_v;

			--horizontal sync signal
			if (h_count < h_pixels + h_fp or h_count > h_pixels + h_fp + h_pulse) then
				h_sync <= not h_pol;    --deassert horiztonal sync pulse
			else
				h_sync <= h_pol;        --assert horiztonal sync pulse
			end if;

			--vertical sync signal
			if (v_count < v_pixels + v_fp or v_count > v_pixels + v_fp + v_pulse) then
				v_sync <= not v_pol;    --deassert vertical sync pulse
			else
				v_sync <= v_pol;        --assert vertical sync pulse
			end if;

			--set pixel coordinates
			if (h_count < h_pixels) then --horiztonal display time
				column <= h_count;      --set horiztonal pixel coordinate
			end if;
			if (v_count < v_pixels) then --vertical display time
				row <= v_count;         --set vertical pixel coordinate
			end if;

			--set display enable output
			if (h_count < h_pixels and v_count < v_pixels) then --display time
				disp_ena <= '1';        --enable display
			else                        --blanking time
				disp_ena <= '0';        --disable display
			end if;
			if (next_h < h_pixels and next_v < v_pixels) then
				next_disp_e <= '1';
			else
				next_disp_e <= '0';
			end if;
		end if;
	end process main;

end architecture vga_controller_bhv;
