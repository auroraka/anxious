-- altera vhdl_input_version vhdl_2008

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity conver_hsv is
    generic (
        constant PIPELINE : natural := 16
    );
    port (
        clk        : in  std_logic;
        reset_n    : in  std_logic;

        -- Input Avalon-ST: RGB888
        din_valid  : in std_logic;
        din_data   : in std_logic_vector(23 downto 0);
        din_sop    : in std_logic;
        din_eop    : in std_logic;

        -- Output Avalon-ST: HSV
        dout_valid : out std_logic;
        dout_data  : out std_logic_vector(0 downto 0);
        dout_sop   : out std_logic;
        dout_eop   : out std_logic
    );
end entity conver_hsv;

architecture bhv of conver_hsv is
    subtype uint8 is unsigned(7 downto 0);
    subtype uint16 is unsigned(15 downto 0);
    subtype int16 is signed(15 downto 0);
    type max_type is (M_RED, M_GREEN, M_BLUE);

    constant HSV_HUE_L        : uint8 := to_unsigned(41, 8); -- 29 / 180.0 * 256
    constant HSV_HUE_R        : uint8 := to_unsigned(91, 8); -- 64 / 180.0 * 256
    constant HSV_SATURATION_L : uint8 := to_unsigned(86, 8);
    constant HSV_VALUE_L      : uint8 := to_unsigned(6, 8);

    type shift_reg_type is record
        in_range: std_logic;
        valid: std_logic;
        max: max_type;
        sop: std_logic;
        eop: std_logic;
    end record;

    constant DEFAULT_SHIFT_REG_VALUE: shift_reg_type := (
        in_range => '0',
        valid => '0',
        max => M_RED,
        sop => '0',
        eop => '0'
    );

    type shift_reg_arr_type is array (integer range <>) of shift_reg_type;

    type reg_type is record
        shift_regs: shift_reg_arr_type(PIPELINE - 1 downto 0);
        dout_valid: std_logic;
        dout_data: std_logic;
        dout_sop: std_logic;
        dout_eop: std_logic;
    end record;

    constant INIT_REGS: reg_type := (
        shift_regs => (others => DEFAULT_SHIFT_REG_VALUE),
        dout_valid => '0',
        dout_data => '0',
        dout_sop => '0',
        dout_eop => '0'
    );

    signal r: reg_type := INIT_REGS;
    signal rin: reg_type := INIT_REGS;

    signal numer0: uint16;
    signal numer1: int16;
    signal denom0, denom1: uint8;
    signal quotient0, quotient1: std_logic_vector(15 downto 0);

begin
    P_regs: process(clk, reset_n)
    begin
        if not reset_n then
            r <= INIT_REGS;
        elsif rising_edge(clk) then
            r <= rin;
        end if;
    end process;

    P_comb: process(all)
        variable v: reg_type;

        variable red, green, blue: uint8;
        variable red_signed, green_signed, blue_signed: signed(8 downto 0);
        variable tmp_delta: uint8;
        variable tmp_min, hsv_value: uint8;
        variable tmp_sub: signed(8 downto 0);

        variable shift_regs_app: shift_reg_type;
        variable cur_shift_reg: shift_reg_type;

        variable hue_offset: uint8;
        variable hue, saturation: uint8;
    begin
        -- default: hold the values
        v := r;

        -- C Code:
        -- V = max(max(R, G), B);
        -- if (V < V_l || V_r < V) return 0;
        --
        -- delta = V - min(min(R, G), B);
        -- if (!delta) return 0;
        -- S = ((delta << 8) - 1) / V;
        -- if (S < S_l || S_r < S) return 0;

        -- // coef = 60 * 256 / 360 = 42.6667
        -- if (R == V) H = (unsigned)(((int)G - (int)B) * 42 / (int)delta + 256);
        -- else if (G == V) H = (unsigned)(((int)B - (int)R) * 42 / (int)delta + 85 + 256);
        -- else H = (unsigned)(((int)R - (int)G) * 42 / (int)delta + 171 + 256);
        -- H &= 255;
        -- H = H * 180 >> 8;
        -- if (H < H_l || H_r < H) return 0;

        numer0 <= (others => '0');
        numer1 <= (others => '0');
        denom0 <= to_unsigned(1, 8);
        denom1 <= to_unsigned(1, 8);

        shift_regs_app.valid := '0';
        shift_regs_app.sop   := din_sop;
        shift_regs_app.eop   := din_eop;

        if din_valid then
            shift_regs_app.valid := '1';

            red   := unsigned(din_data(23 downto 16));
            green := unsigned(din_data(15 downto 8));
            blue  := unsigned(din_data(7 downto 0));

            red_signed   := signed("0" & std_logic_vector(red));
            green_signed := signed("0" & std_logic_vector(green));
            blue_signed  := signed("0" & std_logic_vector(blue));

            if red < green then
                if red < blue then
                    -- R < G && R < B
                    tmp_min := red;
                    if green < blue then
                        shift_regs_app.max := M_BLUE;
                    else
                        shift_regs_app.max := M_GREEN;
                    end if;
                else
                    -- B <= R < G
                    tmp_min := blue;
                    shift_regs_app.max := M_GREEN;
                end if;
            else
                -- G <= R
                if red < blue then
                    -- G <= R < B
                    tmp_min := green;
                    shift_regs_app.max := M_BLUE;
                else
                    -- G <= R, B <= R
                    shift_regs_app.max := M_RED;
                    if green < blue then
                        tmp_min := green;
                    else
                        tmp_min := blue;
                    end if;
                end if;
            end if;

            case shift_regs_app.max is
                when M_RED =>
                    hsv_value := red;
                    tmp_sub := green_signed - blue_signed;
                when M_GREEN =>
                    hsv_value := green;
                    tmp_sub := blue_signed - red_signed;
                when M_BLUE =>
                    hsv_value := blue;
                    tmp_sub := red_signed - green_signed;
            end case;

            tmp_delta := hsv_value - tmp_min;

            if hsv_value >= HSV_VALUE_L and tmp_delta /= 0 then
                shift_regs_app.in_range := '1';
                numer0 <= unsigned(std_logic_vector(tmp_delta) & X"00") - 1;
                denom0 <= hsv_value;
                numer1 <= resize(tmp_sub * 42, 16);
                denom1 <= tmp_delta;
            else
                shift_regs_app.in_range := '0';
            end if;
        end if;

        v.shift_regs := r.shift_regs(r.shift_regs'left - 1 downto 0) &
                shift_regs_app;

        cur_shift_reg := r.shift_regs(r.shift_regs'left);

        v.dout_valid := '0';
        v.dout_sop := cur_shift_reg.sop;
        v.dout_eop := cur_shift_reg.eop;

        if cur_shift_reg.valid then
            v.dout_valid := '1';
            case cur_shift_reg.max is
                when M_RED =>
                    hue_offset := (others => '0');
                when M_GREEN =>
                    hue_offset := to_unsigned(85, 8);
                when M_BLUE =>
                    hue_offset := to_unsigned(171, 8);
            end case;
            saturation := unsigned(quotient0(7 downto 0));
            hue := unsigned(quotient1(7 downto 0)) + hue_offset;
            if cur_shift_reg.in_range = '1' and
                    saturation >= HSV_SATURATION_L and
                    hue >= HSV_HUE_L and
                    hue <= HSV_HUE_R then
                v.dout_data := '1';
            else
                v.dout_data := '0';
            end if;
        end if;

        dout_valid   <= r.dout_valid;
        dout_data(0) <= r.dout_data;
        dout_sop     <= r.dout_sop;
        dout_eop     <= r.dout_eop;

        -- apply the new values
        rin <= v;
    end process;

    -- S = ((delta << 8) - 1) / V
    U_div0: entity work.ip_divider
        generic map (
            NUMER_WIDTH    => 16,
            DENOM_WIDTH    => 8,
            NUMER_SIGNNESS => "UNSIGNED",
            PIPELINE       => PIPELINE
        ) port map (
            clk      => clk,
            reset_n  => reset_n,
            denom    => std_logic_vector(denom0),
            numer    => std_logic_vector(numer0),
            quotient => quotient0,
            remain   => open
        );

    -- H = tmp_sub * 42 / delta + ? + 256
    U_div1: entity work.ip_divider
        generic map (
            NUMER_WIDTH    => 16,
            DENOM_WIDTH    => 8,
            NUMER_SIGNNESS => "SIGNED",
            PIPELINE       => PIPELINE
        ) port map (
            clk      => clk,
            reset_n  => reset_n,
            denom    => std_logic_vector(denom1),
            numer    => std_logic_vector(numer1),
            quotient => quotient1,
            remain   => open
        );

end architecture bhv;
