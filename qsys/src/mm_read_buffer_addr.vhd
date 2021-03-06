-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity mm_read_buffer_addr is
    generic(
        DATA_WIDTH        : positive;
        ADDR_WIDTH        : positive;
        FIFO_LENGTH_LOG_2 : positive;
        VIEW_WIDTH        : positive;
        VIEW_HEIGHT       : positive
    );
    port(
        ---- Avalon-MM Master Interface ----
        clk           : in  std_logic;
        reset_n       : in  std_logic;

        address       : out std_logic_vector(ADDR_WIDTH - 1 downto 0);
        read          : out std_logic;
        readdata      : in  std_logic_vector(DATA_WIDTH - 1 downto 0);
        waitrequest   : in  std_logic;
        readdatavalid : in  std_logic;
        lock          : out std_logic;
        vsync_out     : out std_logic;

        ---- Reads from another clock domain  ----
        s_clk         : in  std_logic;
        s_read        : in  std_logic;
        s_readdata    : out std_logic_vector(DATA_WIDTH - 1 downto 0);
        s_vsync       : in  std_logic
    );
end entity mm_read_buffer_addr;

architecture bhv of mm_read_buffer_addr is
    subtype data_type is std_logic_vector(DATA_WIDTH - 1 downto 0);
    subtype usedw_type is std_logic_vector(FIFO_LENGTH_LOG_2 - 1 downto 0);
    type data_arr_type is array (integer range <>) of data_type;
    type usedw_arr_type is array (integer range <>) of usedw_type;
    subtype pair is std_logic_vector(0 to 1);

    type wrstate_type is (S_WAIT, S_READING);
    type rdstate_type is (S_NORMAL, S_SKIP);

    -- generated reset_n for FIFOs and address generator
    signal wrreset_n: std_logic;

    signal wr, rd, wrempty, wrfull, rdfull, rdempty: pair;
    signal q: data_arr_type(0 to 1);
    signal wrusedw: usedw_arr_type(0 to 1);

    type reg_type is record
        wrstate: wrstate_type;
        wrsel: natural range 0 to 1;
        wrcnt: unsigned(FIFO_LENGTH_LOG_2 - 1 downto 0);
        cmdcnt: unsigned(FIFO_LENGTH_LOG_2 - 1 downto 0);
        read: std_logic;
        lock: std_logic;
        resetreq: std_logic;
        resetreq_sync: std_logic;
        wrreset_n: std_logic;
    end record;

    constant INIT_REGS: reg_type := (
        wrstate => S_WAIT,
        wrsel => 0,
        wrcnt => (others => '0'),
        cmdcnt => (others => '0'),
        read => '0',
        lock => '0',
        resetreq => '0',
        resetreq_sync => '0',
        wrreset_n => '1'
    );

    signal r: reg_type := INIT_REGS;
    signal rin: reg_type := INIT_REGS;

    type s_clk_reg_type is record
        rdstate: rdstate_type;
        rdsel: natural range 0 to 1;
        rdcnt: unsigned(FIFO_LENGTH_LOG_2 - 1 downto 0);
        resetreq: std_logic;
    end record;

    constant S_CLK_INIT_REGS: s_clk_reg_type := (
        rdstate => S_NORMAL,
        rdsel => 0,
        rdcnt => (others => '0'),
        resetreq => '0'
    );

    signal sr: s_clk_reg_type := S_CLK_INIT_REGS;
    signal srin: s_clk_reg_type := S_CLK_INIT_REGS;

    signal addr_fetch : std_logic;

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

        variable read_cmd_accepted: std_logic;
    begin
        -- default: hold the values
        v := r;
        sv := sr;

        for i in 0 to 1 loop
            wr(i) <= '0';
            rd(i) <= '0';
        end loop;

        addr_fetch <= '0';
        wrreset_n <= r.wrreset_n;

        v.wrreset_n := '1';

        case r.wrstate is
            when S_WAIT =>
                if wrempty(r.wrsel) and not r.resetreq then
                    v.wrstate := S_READING;
                    v.read := '1';
                    v.lock := '1';
                    v.cmdcnt := (others => '0');
                else
                    v.read := '0';
                end if;
            when S_READING =>
                -- Address phase: issue `FIFO_LENGTH' read commands with `lock'
                read_cmd_accepted := r.read and not waitrequest;
                addr_fetch <= read_cmd_accepted;
                if read_cmd_accepted then
                    v.cmdcnt := (r.cmdcnt + 1) mod (2 ** FIFO_LENGTH_LOG_2);
                    if v.cmdcnt = 0 then
                        v.read := '0';
                    end if;
                    if v.cmdcnt = 2 ** FIFO_LENGTH_LOG_2 - 1 then
                        v.lock := '0';
                    end if;
                end if;
                -- Data phase
                wr(r.wrsel) <= readdatavalid;
                if readdatavalid then
                    v.wrcnt := (r.wrcnt + 1) mod (2 ** FIFO_LENGTH_LOG_2);
                    if v.wrcnt = 0 then
                        if r.resetreq then
                            v := INIT_REGS;
                            -- sv := S_CLK_INIT_REGS;
                            v.wrreset_n := '0';
                            v.wrstate := S_WAIT;
                        else
                            v.wrsel   := 1 - r.wrsel;
                            v.wrstate := S_WAIT;
                        end if;
                    end if;
                end if;
        end case;

        case sr.rdstate is
            when S_NORMAL =>
                sv.resetreq := '0';
                if s_read and rdempty(sr.rdsel) then
                    sv := S_CLK_INIT_REGS;
                    sv.rdstate := S_SKIP;
                end if;
            when S_SKIP =>
                sv.resetreq := '1';
                if not s_vsync then
                    sv.rdstate := S_NORMAL;
                end if;
        end case;

        v.resetreq_sync := sr.resetreq;
        v.resetreq := r.resetreq_sync; -- 2-stage sync

        -- use new state to determine the values
        case sv.rdstate is
            when S_NORMAL =>
                s_readdata <= q(sr.rdsel);
                rd(sr.rdsel) <= s_read;
                if s_read then
                    sv.rdcnt := (sr.rdcnt + 1) mod (2 ** FIFO_LENGTH_LOG_2);
                    if sv.rdcnt = 0 then
                        sv.rdsel := 1 - sr.rdsel;
                    end if;
                end if;
            when S_SKIP =>
                s_readdata <= (others => '0');
        end case;

        read         <= r.read;
        lock         <= r.lock;

        -- apply the new values
        rin <= v;
        srin <= sv;
    end process;

    P_addr_gen : process(clk, wrreset_n)
        variable row : unsigned(8 downto 0) := (others => '0');
        variable col : unsigned(9 downto 0) := (others => '0');
    begin
        if not wrreset_n then
            row := (others => '0');
            col := (others => '0');
            vsync_out <= '0';
        elsif rising_edge(clk) then
            vsync_out <= '0';
            if addr_fetch then
                if col = VIEW_WIDTH - 1 then
                    col := (others => '0');
                    if row = VIEW_HEIGHT - 1 then
                        row := (others => '0');
                        vsync_out <= '1';
                    else
                        row := row + 1;
                    end if;
                else
                    col := col + 1;
                end if;
            end if;
        end if;
        address(20 downto 0) <= std_logic_vector(row) & std_logic_vector(col) & "00";
    end process;

    U_fifo_arr_gen: for i in 0 to 1 generate
        U_fifo: entity work.ip_fifo
            generic map (
                WIDTH   => DATA_WIDTH,
                LENGTH_LOG_2 => FIFO_LENGTH_LOG_2,
                SHOW_AHEAD   => "ON"
            ) port map (
                aclr_n  => wrreset_n,
                data    => readdata,
                rdclk   => s_clk,
                rdreq   => rd(i),
                wrclk   => clk,
                wrreq   => wr(i),
                wrempty => wrempty(i),
                rdempty => rdempty(i),
                rdfull  => rdfull(i),
                wrfull  => wrfull(i),
                q       => q(i),
                wrusedw => wrusedw(i)
            );
    end generate;
end architecture bhv;
