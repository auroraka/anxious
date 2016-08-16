-- altera vhdl_input_version vhdl_2008

-- Write buffer that stacks `FIFO_LENGTH` write commands before issueing a
-- write burst command to Avalon-MM system of that length. Note that this
-- assumes incoming write commands always refer to consecutive address every
-- `FIFO_LENGTH` writes.

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity up_write_buffer is
	generic(
		constant DATA_WIDTH        : positive;
		constant FIFO_LENGTH_LOG_2 : positive
	);
	port(
        ---- Avalon-ST Video Interface ----
        clk         : in  std_logic;
        reset_n     : in  std_logic;
        
        ready       : in  std_logic;
        valid       : out std_logic;
--        empty       : out std_logic_vector(0 downto 0);
        sop         : out std_logic;
        eop         : out std_logic;
        data        : out std_logic_vector(DATA_WIDTH - 1 downto 0);

        ---- Writes from another clock domain  ----
        s_clk       : in  std_logic;
        s_write     : in  std_logic;
        s_writedata : in  std_logic_vector(DATA_WIDTH - 1 downto 0)
--        s_end_frame : in  std_logic
	);
end entity up_write_buffer;

architecture ip of up_write_buffer is
    subtype data_type is std_logic_vector(DATA_WIDTH - 1 downto 0);
    subtype usedw_type is std_logic_vector(FIFO_LENGTH_LOG_2 - 1 downto 0);

    signal wr, rd, wrempty, wrfull, rdfull, rdempty: std_logic;
    signal q: data_type;
    signal data_reg: data_type;
    signal wrusedw: usedw_type;

	constant FRAME_PIXELS : natural := 640 * 480;
    type rdstate_type is (S_BLANK, S_INIT, S_WRITING);
    type reg_type is record
        rdstate: rdstate_type;
		pixcnt : natural range 0 to FRAME_PIXELS;
    end record;

    constant INIT_REGS: reg_type := (
        rdstate => S_BLANK,
        pixcnt => 0
    );

    signal r: reg_type := INIT_REGS;
    signal rin: reg_type := INIT_REGS;

    type s_clk_reg_type is record
        wr: std_logic;
        din: data_type;
        wrcnt: unsigned(FIFO_LENGTH_LOG_2 - 1 downto 0);
    end record;

    constant S_CLK_INIT_REGS: s_clk_reg_type := (
        wr => '0',
        din => (others => '0'),
        wrcnt => (others => '0')
    );

    signal sr: s_clk_reg_type := S_CLK_INIT_REGS;
    signal srin: s_clk_reg_type := S_CLK_INIT_REGS;
begin
    P_regs: process(clk, reset_n)
    begin
        if not reset_n then
            r <= INIT_REGS;
        elsif rising_edge(clk) then
            r <= rin;
        end if;
    end process;

    P_s_clk_regs: process(s_clk, reset_n)
    begin
        if not reset_n then
            sr <= S_CLK_INIT_REGS;
        elsif rising_edge(s_clk) then
            sr <= srin;
        end if;
    end process;

    P_comb: process(all)
        variable v: reg_type;
        variable sv: s_clk_reg_type;
    begin
        -- default: hold the values
        v := r;
        sv := sr;
        
        sv.wr  := '0';
        sv.din := s_writedata;

        if s_write then
            sv.wr := '1';
            sv.wrcnt := (sr.wrcnt + 1) mod (2 ** FIFO_LENGTH_LOG_2);
        end if;

        data  <= q;
        valid <= '0';
    	sop   <= '0';
    	eop   <= '0';
        rd    <= '0';

        case r.rdstate is
        	when S_BLANK =>
        		if not rdempty then
        			v.rdstate := S_INIT;
        		end if;
        	when S_INIT =>
        		if ready then
        			valid <= '1';
        			data <= (others => '0');
        			sop <= '1';
        			v.rdstate := S_WRITING;
        			v.pixcnt := 0;
        		end if;
        	when S_WRITING =>
            	if not rdempty and ready then
            		rd    <= '1';
	            	valid <= '1';
		            if r.pixcnt = FRAME_PIXELS - 1 then
		           		eop <= '1';
		           		v.pixcnt := 0;
	                	v.rdstate := S_BLANK;
	                else
	                	v.pixcnt := r.pixcnt + 1;
	                end if;
                end if;
        end case;

		wr <= sr.wr;

        data_reg <= sr.din;

        -- apply the new values
        rin <= v;
        srin <= sv;
    end process;

    U_fifo: entity work.ip_fifo
        generic map (
            WIDTH   => DATA_WIDTH,
            LENGTH_LOG_2 => FIFO_LENGTH_LOG_2,
            SHOW_AHEAD   => "ON"
        ) port map (
            aclr_n  => reset_n,
            data    => data_reg,
            rdclk   => clk,
            rdreq   => rd,
            wrclk   => s_clk,
            wrreq   => wr,
            wrempty => wrempty,
            rdempty => rdempty,
            rdfull  => rdfull,
            wrfull  => wrfull,
            q       => q,
            wrusedw => wrusedw
        );
end architecture ip;
