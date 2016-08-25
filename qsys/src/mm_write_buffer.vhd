-- altera vhdl_input_version vhdl_2008

-- Write buffer that stacks `FIFO_LENGTH` write commands before issueing
-- `FIFO_LENGTH` write commands to Avalon-MM system.

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity mm_write_buffer is
    generic(
        constant DATA_WIDTH        : positive;
        constant ADDR_WIDTH        : positive;
        constant FIFO_LENGTH_LOG_2 : positive;
        constant FRAME_PIXELS      : positive
    );
    port(
        ---- Avalon-MM Master Interface ----
        clk         : in  std_logic;
        reset_n     : in  std_logic;

        address     : out std_logic_vector(ADDR_WIDTH - 1 downto 0);
        write       : out std_logic;
        writedata   : out std_logic_vector(DATA_WIDTH - 1 downto 0);
        waitrequest : in  std_logic;
        vsync_out   : out std_logic;

        ---- Writes from another clock domain  ----
        s_clk       : in  std_logic;
        s_write     : in  std_logic;
        s_writedata : in  std_logic_vector(DATA_WIDTH - 1 downto 0);
        s_addr      : in  std_logic_vector(ADDR_WIDTH - 1 downto 0)
    );
end entity mm_write_buffer;

architecture ip of mm_write_buffer is
    subtype addr_type is std_logic_vector(ADDR_WIDTH - 1 downto 0);
    subtype data_type is std_logic_vector(DATA_WIDTH - 1 downto 0);
    subtype usedw_type is std_logic_vector(FIFO_LENGTH_LOG_2 - 1 downto 0);
    -- a vector which stores concat(addr, data)
    subtype addr_data_type is std_logic_vector(ADDR_WIDTH + DATA_WIDTH - 1 downto 0);

    type data_arr_type is array (integer range <>) of data_type;
    type addr_arr_type is array (integer range <>) of addr_type;
    type usedw_arr_type is array (integer range <>) of usedw_type;
    type addr_data_arr_type is array (integer range <>) of addr_data_type;

    subtype pair is std_logic_vector(0 to 1);

    signal wr, rd, wrempty, wrfull, rdfull, rdempty: pair;
    signal q: addr_data_arr_type(0 to 1);
    signal addr_data_reg: addr_data_type;
    signal wrusedw: usedw_arr_type(0 to 1);

    type rdstate_type is (S_WAIT, S_WRITING);
    type reg_type is record
        rdsel: natural range 0 to 1;
        rdstate: rdstate_type;
        pixcnt : natural range 0 to FRAME_PIXELS;
    end record;

    constant INIT_REGS: reg_type := (
        rdsel => 0,
        rdstate => S_WAIT,
        pixcnt => 0
    );

    signal r: reg_type := INIT_REGS;
    signal rin: reg_type := INIT_REGS;

    type s_clk_reg_type is record
        wrsel: natural range 0 to 1;
        wr: pair;
        din: addr_data_type;
        wrcnt: unsigned(FIFO_LENGTH_LOG_2 - 1 downto 0);
        failed: std_logic;
        fail_on: natural range 0 to 1;
    end record;

    constant S_CLK_INIT_REGS: s_clk_reg_type := (
        wrsel => 0,
        wr => (others => '0'),
        din => (others => '0'),
        wrcnt => (others => '0'),
        failed => '0',
        fail_on => 0
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
        variable rdfull_c: std_logic;
        variable rdempty_c: std_logic;
        variable almost_full: boolean;
        variable switching_empty: std_logic;
        variable avalon_addr_data: addr_data_type;
    begin
        -- default: hold the values
        v := r;
        sv := sr;

        for i in 0 to 1 loop
            sv.wr(i) := '0';
        end loop;

        sv.din := s_addr & s_writedata;

        if s_write then
            -- switch FIFO every `length` writes regardless of the state
            -- of FIFOs.
            sv.wrcnt := (sr.wrcnt + 1) mod (2 ** FIFO_LENGTH_LOG_2);
            almost_full := sv.wrcnt = 0;
            if almost_full then
                sv.wrsel := 1 - sr.wrsel;
                -- enter `FAIL' state if the FIFO we are switching to is
                -- not ready, disabling the subsequent `length` write
                -- attempts.
                switching_empty := wrempty(sv.wrsel);
                if not sr.failed and not switching_empty then
                    sv.failed := '1';
                    sv.fail_on := sv.wrsel;
                end if;
                if sr.failed = '1' and switching_empty = '1' and
                        sr.fail_on = sv.wrsel then
                    sv.failed := '0';
                end if;
            end if;
            if not sr.failed then
                sv.wr(sr.wrsel) := '1';
            end if;
        end if;

        rdfull_c   := rdfull(r.rdsel);
        rdempty_c  := rdempty(r.rdsel);

        vsync_out  <= '0';

        avalon_addr_data := q(r.rdsel);
        address          <= avalon_addr_data(ADDR_WIDTH + DATA_WIDTH - 1 downto DATA_WIDTH);
        writedata        <= avalon_addr_data(DATA_WIDTH - 1 downto 0);

        for i in 0 to 1 loop
            rd(i) <= '0';
        end loop;

        case r.rdstate is
            when S_WAIT =>
                write <= '0';
                if rdfull_c then
                    v.rdstate := S_WRITING;
                end if;
            when S_WRITING =>
                write <= not rdempty_c;
                rd(r.rdsel) <= not waitrequest;
                if not rdempty_c and not waitrequest then
                    if r.pixcnt = FRAME_PIXELS - 1 then
                        vsync_out <= '1';
                        v.pixcnt := 0;
                    else
                        v.pixcnt := r.pixcnt + 1;
                    end if;
                end if;
                if rdempty_c then
                    v.rdsel := 1 - r.rdsel;
                    v.rdstate := S_WAIT;
                end if;
        end case;

        for i in 0 to 1 loop
            wr(i) <= sr.wr(i);
        end loop;

        addr_data_reg <= sr.din;

        -- apply the new values
        rin <= v;
        srin <= sv;
    end process;

    U_fifo_arr_gen: for i in 0 to 1 generate
        U_fifo: entity work.ip_fifo
            generic map (
                WIDTH        => DATA_WIDTH + ADDR_WIDTH,
                LENGTH_LOG_2 => FIFO_LENGTH_LOG_2,
                SHOW_AHEAD   => "ON"
            ) port map (
                aclr_n  => reset_n,
                data    => addr_data_reg,
                rdclk   => clk,
                rdreq   => rd(i),
                wrclk   => s_clk,
                wrreq   => wr(i),
                wrempty => wrempty(i),
                rdempty => rdempty(i),
                rdfull  => rdfull(i),
                wrfull  => wrfull(i),
                q       => q(i),
                wrusedw => wrusedw(i)
            );
    end generate;
end architecture ip;
