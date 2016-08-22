-- altera vhdl_input_version vhdl_2008

-- Accepts a non-stoppable, i.e., do not have `ready' signal Avalon-ST
-- as input, treats it as pixels of a HEIGHT x WIDTH video.

-- Stacks `FIFO_LENGTH` pixel data before issueing continuous
-- `FIFO_LENGTH` write commands to Avalon-MM system.

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity mm_write_buffer is
    generic(
        constant DATA_WIDTH        : positive := 32;
        constant AV_ADDR_WIDTH     : positive := 27;
        constant ST_ADDR_WIDTH     : positive := 21;
        constant FIFO_LENGTH_LOG_2 : positive := 6;
        constant FRAME_PIXELS      : positive := 307200;
        constant BANK              : std_logic_vector(1 downto 0) := "00"
    );
    port(
        ---- Avalon-MM Master Interface ----
        clk         : in  std_logic;
        reset_n     : in  std_logic;

        address     : out std_logic_vector(AV_ADDR_WIDTH - 1 downto 0);
        write       : out std_logic;
        writedata   : out std_logic_vector(DATA_WIDTH - 1 downto 0);
        waitrequest : in  std_logic;

        ---- Non-stoppable Avalon-ST Interface ----
        st_clk      : in  std_logic;
        st_valid    : in  std_logic;
        st_sop      : in  std_logic;
        st_eop      : in  std_logic;
        st_data     : in  std_logic_vector(DATA_WIDTH - 1 downto 0);

        ---- Address Generator ----
        addr_fetch  : out std_logic;
        addr_vsync  : out std_logic;
        addr_gen    : in  std_logic_vector(ST_ADDR_WIDTH - 1 downto 0);

        ---- Frame Buffer Signals ----
        vsync_out   : out std_logic;
        buffer_port : in  std_logic_vector(1 downto 0)
    );
end entity mm_write_buffer;

architecture ip of mm_write_buffer is
    subtype data_type is std_logic_vector(DATA_WIDTH - 1 downto 0);
    subtype usedw_type is std_logic_vector(FIFO_LENGTH_LOG_2 - 1 downto 0);
    -- a vector which stores concat(addr, data)
    subtype st_addr_data_type is std_logic_vector(
        ST_ADDR_WIDTH + DATA_WIDTH - 1 downto 0);

    type data_arr_type is array (integer range <>) of data_type;
    type usedw_arr_type is array (integer range <>) of usedw_type;
    type st_addr_data_arr_type is array (integer range <>) of st_addr_data_type;

    subtype pair is std_logic_vector(0 to 1);

    signal wr, rd, wrempty, wrfull, rdfull, rdempty: pair;
    signal q: st_addr_data_arr_type(0 to 1);
    signal st_addr_data_reg: st_addr_data_type;
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
        din: st_addr_data_type;
        wrcnt: unsigned(FIFO_LENGTH_LOG_2 - 1 downto 0);
        failed: std_logic;
        fail_on: natural range 0 to 1;
        row: unsigned(8 downto 0);
        col: unsigned(9 downto 0);
    end record;

    constant S_CLK_INIT_REGS: s_clk_reg_type := (
        wrsel => 0,
        wr => (others => '0'),
        din => (others => '0'),
        wrcnt => (others => '0'),
        failed => '0',
        fail_on => 0,
        row => (others => '0'),
        col => (others => '0')
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

    P_s_clk_regs: process(st_clk, reset_n)
    begin
        if not reset_n then
            sr <= S_CLK_INIT_REGS;
        elsif rising_edge(st_clk) then
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
        variable avalon_st_addr_data: st_addr_data_type;
        variable st_addr_gen: std_logic_vector(ST_ADDR_WIDTH - 1 downto 0) :=
            (others => '0');
    begin
        -- default: hold the values
        v := r;
        sv := sr;

        -- hand necessary signals to address generator
        -- get the address generated
        addr_fetch <= st_valid;
        addr_vsync <= st_eop;
        st_addr_gen := addr_gen;

        for i in 0 to 1 loop
            sv.wr(i) := '0';
        end loop;

        sv.din := st_addr_gen & st_data;

        if st_valid then
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

        avalon_st_addr_data := q(r.rdsel);

        address(ST_ADDR_WIDTH + 5 downto 0) <= BANK & "00" & buffer_port &
            avalon_st_addr_data(ST_ADDR_WIDTH + DATA_WIDTH - 1 downto DATA_WIDTH);
        writedata <= avalon_st_addr_data(DATA_WIDTH - 1 downto 0);

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

        st_addr_data_reg <= sr.din;

        -- apply the new values
        rin <= v;
        srin <= sv;
    end process;

    U_fifo_arr_gen: for i in 0 to 1 generate
        U_fifo: entity work.ip_fifo
            generic map (
                WIDTH        => DATA_WIDTH + ST_ADDR_WIDTH,
                LENGTH_LOG_2 => FIFO_LENGTH_LOG_2,
                SHOW_AHEAD   => "ON"
            ) port map (
                aclr_n  => reset_n,
                data    => st_addr_data_reg,
                rdclk   => clk,
                rdreq   => rd(i),
                wrclk   => st_clk,
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
