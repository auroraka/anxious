-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity overlay_decoder is
	port(
		clk       : in  std_logic;
		reset_n   : in  std_logic;

		---- Avalon-ST Source Interface ----
		in_ready  : out std_logic;
		in_data   : in  std_logic_vector(3 downto 0);

		---- Avalon-ST Source Interface ----
		out_ready : in  std_logic;
		out_data  : out std_logic_vector(31 downto 0)
	);
end entity overlay_decoder;

architecture bhv of overlay_decoder is
begin
	main : process(all)
	begin
		in_ready <= out_ready;
		case in_data is
			when "0000" => out_data <= 32X"003F51B5"; -- Indigo
			when "0001" => out_data <= 32X"0000BCD4"; -- Cyan
			when "0010" => out_data <= 32X"00EC407A"; -- Pink
			when "0011" => out_data <= 32X"00AB47BC"; -- Violet
			when "0100" => out_data <= 32X"00009688"; -- Teal
			when "0101" => out_data <= 32X"0064FFDA"; -- Pale Teal
			when "0110" => out_data <= 32X"004CAF50"; -- Green
			when "0111" => out_data <= 32X"002196F3"; -- Blue
			when "1000" => out_data <= 32X"00FFD740"; -- Amber
			when "1001" => out_data <= 32X"00607D8B"; -- Blue Grey
			when "1010" => out_data <= 32X"00424242"; -- Grey
			when "1011" => out_data <= 32X"00ECEFF1"; -- White
			when "1100" => out_data <= 32X"00000000"; -- Black
			when "1101" => out_data <= 32X"00ECECEC"; -- Window BG
			when "1110" => out_data <= 32X"00CCCCCC"; -- Window Border
			when "1111" => out_data <= (24 => '1', others => '0'); -- Transparent
            when others => null;
		end case;
	end process main;
end architecture bhv;
