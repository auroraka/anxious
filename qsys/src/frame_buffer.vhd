-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

use work.NEEE.all;

-- Accepts end-of-frame signals and returns address prefix
entity frame_buffer is
	port(
		clk          : in  std_logic;
		reset_n      : in  std_logic;
		write_vsync  : in  std_logic;
		write_buffer : out std_logic_vector(1 downto 0);
		read_vsync   : in  std_logic;
		read_buffer  : out std_logic_vector(1 downto 0);
		
		enable       : in  std_logic
	);
end entity frame_buffer;

architecture frame_buffer_bhv of frame_buffer is
	constant PORT_WIDTH : natural := 2;

	constant DEFAULT_WRITE_PORT : std_logic_vector(PORT_WIDTH - 1 downto 0) := "00";
	constant DEFAULT_READ_PORT  : std_logic_vector(PORT_WIDTH - 1 downto 0) := "01";
	constant DEFAULT_IDLE_PORT  : std_logic_vector(PORT_WIDTH - 1 downto 0) := "10";

	signal write_vsync_r : std_logic;
	signal read_vsync_r  : std_logic;

	subtype PORT_ADDR is std_logic_vector(PORT_WIDTH - 1 downto 0);
	type reg_data is record
		write_port   : PORT_ADDR;
		read_port    : PORT_ADDR;
		idle_port    : PORT_ADDR;
		switch_ready : std_logic;
	end record;
	constant REG_DEFAULT : reg_data := (
		write_port   => DEFAULT_WRITE_PORT,
		read_port    => DEFAULT_READ_PORT,
		idle_port    => DEFAULT_IDLE_PORT,
		switch_ready => '0'
	);
	signal r, rin : reg_data;
begin
	P_reg : process(clk, reset_n)
	begin
		if not reset_n then
			r <= REG_DEFAULT;
			write_vsync_r <= '0';
			read_vsync_r  <= '0';
		elsif rising_edge(clk) then
			r <= rin;
			write_vsync_r <= write_vsync;
			read_vsync_r  <= read_vsync;
		end if;
	end process P_reg;

	write_buffer <= r.write_port when enable else DEFAULT_IDLE_PORT;
	read_buffer  <= r.read_port  when enable else DEFAULT_IDLE_PORT;

	P_comb : process(all)
		variable w_switch : std_logic := '0';
		variable r_switch : std_logic := '0';

		variable v : reg_data;
	begin
		v := r;

		w_switch := not write_vsync_r and write_vsync;
		r_switch := not read_vsync_r and read_vsync;

		if w_switch then
			v.write_port   := r.idle_port;
			v.idle_port    := r.write_port;
			v.switch_ready := '1';
		end if;

		if r_switch and (v.switch_ready or r.switch_ready) then
			v.read_port    := v.idle_port;
			v.idle_port    := r.read_port;
			v.switch_ready := '0';
		end if;

		rin <= v;
	end process P_comb;
	
end architecture frame_buffer_bhv;