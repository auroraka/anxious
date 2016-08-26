library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity ir_receiver is
	port(
		clk         : in  std_logic;
		reset_n     : in  std_logic;

		-- Avalon MM
		chipselect  : in  std_logic;
		address     : in  std_logic_vector(24 downto 0); -- BANK(24-23) & ROW(22-10) & COL(9-0)
		byteenable  : in  std_logic_vector(3 downto 0);
		waitrequest : out std_logic;
		rd          : in  std_logic;
		readdata    : out std_logic_vector(31 downto 0);

		--ir
		ir_clk      : in  std_logic;
		IRDA        : in  std_logic     -- IR code input
	);
end entity ir_receiver;

architecture bhv of ir_receiver is
	component ir_decoder is
		port(
			iCLK        : in  std_logic; -- clk 50MHz
			iRST_n      : in  std_logic; -- reset
			iIRDA       : in  std_logic; -- IR code input
			oDATA_READY : out std_logic; -- data ready
			oDATA       : out std_logic_vector(31 downto 0) -- decode data output
		);
	end component ir_decoder;

	type device_state_type is (DEV_IDLE, DEV_RD_REQ, DEV_RD_DONE);
	signal device_state : device_state_type;

	signal mydata : std_logic_vector(31 downto 0);
	signal myaddr : std_logic_vector(24 downto 0);

	constant DEFAULT_DATA : std_logic_vector(31 downto 0) := "00000000111111110000000000000000";

	signal ir_ready   : std_logic := '0';
	signal ir_ready_r : std_logic := '0';
	signal ir_data    : std_logic_vector(31 downto 0);

	signal count : integer := 0;
begin
	ir_device : ir_decoder
		port map(
			iCLK        => ir_clk,
			iRST_n      => reset_n,
			iIRDA       => IRDA,
			oDATA_READY => ir_ready,
			oDATA       => ir_data
		);

	-- read priority higher than write
	process(clk, reset_n)
	begin
		if not reset_n then
			device_state <= DEV_IDLE;
			waitrequest  <= '1';
			mydata       <= DEFAULT_DATA; -- defaut(23 downto 16) <= 0xFF
			ir_ready_r   <= '0';
		elsif rising_edge(clk) then
			count <= count + 1;
			if count = 10000 then
				count <= 0;
				if not ir_ready_r and ir_ready then
					mydata <= ir_data;
				end if;
				ir_ready_r <= ir_ready;
			end if;
			-- read data
			if device_state = DEV_RD_DONE then
				device_state <= DEV_IDLE;
				waitrequest  <= '1';
			elsif rd then
				case device_state is
					when DEV_IDLE =>
						device_state <= DEV_RD_REQ;
						myaddr       <= address;
					when DEV_RD_REQ =>
						device_state <= DEV_RD_DONE;
						waitrequest  <= '0';
						if byteenable(0) then
							readdata(7 downto 0) <= mydata(7 downto 0);
						end if;
						if byteenable(1) then
							readdata(15 downto 8) <= mydata(15 downto 8);
						end if;
						if byteenable(2) then
							readdata(23 downto 16) <= mydata(23 downto 16);
						end if;
						if byteenable(3) then
							readdata(31 downto 24) <= mydata(31 downto 24);
						end if;
						mydata <= DEFAULT_DATA;
					when DEV_RD_DONE =>
						null;
				end case;
			else
				device_state <= DEV_IDLE;
				waitrequest  <= '1';
			end if;
		end if;
	end process;

end architecture bhv;
