-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

-- Accepts end-of-frame signals and returns address prefix
entity frame_buffer is
	port(
		reset_n      : in  std_logic;

		write_clk    : in  std_logic;
		write_vsync  : in  std_logic;
		write_buffer : out std_logic_vector(1 downto 0);

        read_clk     : in  std_logic;
		read_vsync   : in  std_logic;
		read_buffer  : out std_logic_vector(1 downto 0);

		enable       : in  std_logic
	);
end entity frame_buffer;

architecture frame_buffer_bhv of frame_buffer is
	constant PORT_WIDTH : natural := 2;

	constant DEFAULT_IDLE_PORT  : std_logic_vector(PORT_WIDTH - 1 downto 0) := "00";
	constant DEFAULT_WRITE_PORT : std_logic_vector(PORT_WIDTH - 1 downto 0) := "01";
	constant DEFAULT_READ_PORT  : std_logic_vector(PORT_WIDTH - 1 downto 0) := "10";

	subtype PORT_ADDR is std_logic_vector(PORT_WIDTH - 1 downto 0);

    type write_reg_data is record
        write_vsync_r : std_logic;
        req            : std_logic;
        req_tog : std_logic;
        write_port_syn : PORT_ADDR;
        write_port     : PORT_ADDR;
    end record;

    constant WRITE_REG_DEFAULT : write_reg_data := (
        write_vsync_r  => '0',
        req            => '0',
        req_tog => '0',
        write_port     => DEFAULT_WRITE_PORT,
        write_port_syn => DEFAULT_WRITE_PORT
    );

    signal wr, wrin : write_reg_data;

	type read_reg_data is record
        -- buffer switching logic
		write_port    : PORT_ADDR;
		read_port     : PORT_ADDR;
		idle_port     : PORT_ADDR;
		switch_ready  : std_logic;
        read_vsync_r  : std_logic;
        -- data exchanging with write_clk domain
        req_s0: std_logic;
        req_s1: std_logic;
        req_s2: std_logic;
        req: std_logic;
	end record;

	constant READ_REG_DEFAULT : read_reg_data := (
		write_port   => DEFAULT_WRITE_PORT,
		read_port    => DEFAULT_READ_PORT,
		idle_port    => DEFAULT_IDLE_PORT,
		switch_ready => '0',
        read_vsync_r => '0',
        req_s0       => '0',
        req_s1       => '0',
        req_s2       => '0',
        req          => '0'
	);

	signal rr, rrin : read_reg_data;

begin
	P_write_reg : process(write_clk, reset_n)
	begin
		if not reset_n then
			wr <= WRITE_REG_DEFAULT;
		elsif rising_edge(write_clk) then
			wr <= wrin;
		end if;
	end process P_write_reg;

    P_read_reg : process(read_clk, reset_n)
    begin
        if not reset_n then
            rr <= READ_REG_DEFAULT;
        elsif rising_edge(read_clk) then
            rr <= rrin;
        end if;
    end process P_read_reg;

	write_buffer <= wr.write_port when enable else DEFAULT_IDLE_PORT;
	read_buffer  <= rr.read_port  when enable else DEFAULT_IDLE_PORT;

	P_comb : process(all)
		variable w_switch : std_logic := '0';
		variable r_switch : std_logic := '0';

		variable wv : write_reg_data;
        variable rv : read_reg_data;
	begin
		wv := wr;
        rv := rr;

        rv.read_vsync_r := read_vsync;
		r_switch        := not rr.read_vsync_r and read_vsync;

        -- generates req as one-cycle-length pulse
        wv.write_vsync_r := write_vsync;
        wv.req           := not wr.write_vsync_r and write_vsync;
        -- req_tog as accumulated XOR of req
        wv.req_tog       := wr.req_tog xor wr.req;

        -- 2-stage sync of write_port from read_clk to write_clk
        wv.write_port_syn := rr.write_port;
        wv.write_port     := wr.write_port_syn;

        -- toggle synchronizer
        -- cf. http://www.edn.com/electronics-blogs/day-in-the-life-of-a-chip-designer/4435339/Synchronizer-techniques-for-multi-clock-domain-SoCs
        rv.req_s0 := wr.req_tog;
        rv.req_s1 := rr.req_s0;
        rv.req_s2 := rr.req_s1;
		w_switch  := rr.req_s1 xor rr.req_s2;

		if w_switch then
			rv.write_port   := rr.idle_port;
			rv.idle_port    := rr.write_port;
			rv.switch_ready := '1';
		end if;

		if r_switch and (rv.switch_ready or rr.switch_ready) then
			rv.read_port    := rv.idle_port;
			rv.idle_port    := rr.read_port;
			rv.switch_ready := '0';
		end if;

		wrin <= wv;
        rrin <= rv;
	end process P_comb;

end architecture frame_buffer_bhv;
