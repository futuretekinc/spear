/*
 * arch/arm/mach-spear6xx/clock.c
 *
 * SPEAr6xx machines clock framework source file
 *
 * Copyright (C) 2009 ST Microelectronics
 * Viresh Kumar<viresh.kumar@st.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <mach/misc_regs.h>
#include <asm/mach-types.h>
#include <plat/clock.h>

/* root clks */
/* 32 KHz oscillator clock */
static struct clk osc_32k_clk = {
	.flags = ALWAYS_ENABLED | SYSTEM_CLK,
	.rate = 32000,
};

/* 30 MHz oscillator clock */
static struct clk osc_30m_clk = {
	.flags = ALWAYS_ENABLED | SYSTEM_CLK,
	.rate = 30000000,
};

/* clock derived from 32 KHz osc clk */
/* rtc clock */
static struct clk rtc_clk = {
	.pclk = &osc_32k_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = RTC_CLK_ENB,
	.recalc = &follow_parent,
};

/* clock derived from 30 MHz osc clk */
/* vco masks structure */
static struct vco_clk_masks vco_masks = {
	.mode_mask = PLL_MODE_MASK,
	.mode_shift = PLL_MODE_SHIFT,
	.norm_fdbk_m_mask = PLL_NORM_FDBK_M_MASK,
	.norm_fdbk_m_shift = PLL_NORM_FDBK_M_SHIFT,
	.dith_fdbk_m_mask = PLL_DITH_FDBK_M_MASK,
	.dith_fdbk_m_shift = PLL_DITH_FDBK_M_SHIFT,
	.div_p_mask = PLL_DIV_P_MASK,
	.div_p_shift = PLL_DIV_P_SHIFT,
	.div_n_mask = PLL_DIV_N_MASK,
	.div_n_shift = PLL_DIV_N_SHIFT,
	.pll_lock_mask = PLL_LOCK_MASK,
	.pll_lock_shift = PLL_LOCK_SHIFT,
};

/* vco rate configuration table, in ascending order of rates */
struct vco_rate_tbl vco_rtbl[] = {
	{.mode = 0, .m = 0x53, .n = 0x0F, .p = 0x1}, /* vco 332 & pll 166 MHz */
	{.mode = 0, .m = 0x85, .n = 0x0F, .p = 0x1}, /* vco 532 & pll 266 MHz */
	{.mode = 0, .m = 0xA6, .n = 0x0F, .p = 0x1}, /* vco 664 & pll 332 MHz */
};

/* vco1 configuration structure */
static struct vco_clk_config vco1_config = {
	.mode_reg = VA_PLL1_CTR,
	.cfg_reg = VA_PLL1_FRQ,
	.masks = &vco_masks,
};

/* vco1 clock */
static struct clk vco1_clk = {
	.flags = ENABLED_ON_INIT | SYSTEM_CLK,
	.pclk = &osc_30m_clk,
	.en_reg = VA_PLL1_CTR,
	.en_reg_bit = PLL_ENABLE,
	.calc_rate = &vco_calc_rate,
	.recalc = &vco_clk_recalc,
	.set_rate = &vco_clk_set_rate,
	.rate_config = {vco_rtbl, ARRAY_SIZE(vco_rtbl), 2},
	.private_data = &vco1_config,
};

/* pll1 clock */
static struct clk pll1_clk = {
	.flags = ALWAYS_ENABLED | SYSTEM_CLK,
	.pclk = &vco1_clk,
	.recalc = &pll_clk_recalc,
};

/* vco2 configuration structure */
static struct vco_clk_config vco2_config = {
	.mode_reg = VA_PLL2_CTR,
	.cfg_reg = VA_PLL2_FRQ,
	.masks = &vco_masks,
};

/* PLL2 clock */
static struct clk vco2_clk = {
	.flags = ENABLED_ON_INIT | SYSTEM_CLK,
	.pclk = &osc_30m_clk,
	.en_reg = VA_PLL2_CTR,
	.en_reg_bit = PLL_ENABLE,
	.calc_rate = &vco_calc_rate,
	.recalc = &vco_clk_recalc,
	.set_rate = &vco_clk_set_rate,
	.rate_config = {vco_rtbl, ARRAY_SIZE(vco_rtbl), 2},
	.private_data = &vco2_config,
};

/* pll2 clock */
static struct clk pll2_clk = {
	.flags = ALWAYS_ENABLED | SYSTEM_CLK,
	.pclk = &vco2_clk,
	.recalc = &pll_clk_recalc,
};

/* PLL3 48 MHz clock */
static struct clk pll3_48m_clk = {
	.flags = ALWAYS_ENABLED | SYSTEM_CLK,
	.pclk = &osc_30m_clk,
	.rate = 48000000,
};

/* watch dog timer clock */
static struct clk wdt_clk = {
	.flags = ALWAYS_ENABLED,
	.pclk = &osc_30m_clk,
	.recalc = &follow_parent,
};

/* clock derived from pll1 clk */
/* cpu clock */
static struct clk cpu_clk = {
	.flags = ALWAYS_ENABLED | SYSTEM_CLK,
	.pclk = &pll1_clk,
	.recalc = &follow_parent,
};

/* ahb masks structure */
static struct bus_clk_masks ahb_masks = {
	.mask = PLL_HCLK_RATIO_MASK,
	.shift = PLL_HCLK_RATIO_SHIFT,
};

/* ahb configuration structure */
static struct bus_clk_config ahb_config = {
	.reg = VA_CORE_CLK_CFG,
	.masks = &ahb_masks,
};

/* ahb rate configuration table, in ascending order of rates */
struct bus_rate_tbl bus_rtbl[] = {
	{.div = 3}, /* == parent divided by 4 */
	{.div = 2}, /* == parent divided by 3 */
	{.div = 1}, /* == parent divided by 2 */
	{.div = 0}, /* == parent divided by 1 */
};

/* ahb clock */
static struct clk ahb_clk = {
	.flags = ALWAYS_ENABLED | SYSTEM_CLK,
	.pclk = &pll1_clk,
	.calc_rate = &bus_calc_rate,
	.recalc = &bus_clk_recalc,
	.set_rate = &bus_clk_set_rate,
	.rate_config = {bus_rtbl, ARRAY_SIZE(bus_rtbl), 2},
	.private_data = &ahb_config,
};

/* auxiliary synthesizers masks */
static struct aux_clk_masks aux_masks = {
	.eq_sel_mask = AUX_EQ_SEL_MASK,
	.eq_sel_shift = AUX_EQ_SEL_SHIFT,
	.eq1_mask = AUX_EQ1_SEL,
	.eq2_mask = AUX_EQ2_SEL,
	.xscale_sel_mask = AUX_XSCALE_MASK,
	.xscale_sel_shift = AUX_XSCALE_SHIFT,
	.yscale_sel_mask = AUX_YSCALE_MASK,
	.yscale_sel_shift = AUX_YSCALE_SHIFT,
};

/* uart configurations */
static struct aux_clk_config uart_synth_config = {
	.synth_reg = VA_UART_CLK_SYNT,
	.masks = &aux_masks,
};

/* aux rate configuration table, in ascending order of rates */
struct aux_rate_tbl aux_rtbl[] = {
	/* For PLL1 = 332 MHz */
	{.xscale = 2, .yscale = 8, .eq = 0}, /* 41.5 MHz */
	{.xscale = 2, .yscale = 4, .eq = 0}, /* 83 MHz */
	{.xscale = 1, .yscale = 2, .eq = 1}, /* 166 MHz */
};

/* uart synth clock */
static struct clk uart_synth_clk = {
	.en_reg = VA_UART_CLK_SYNT,
	.en_reg_bit = AUX_SYNT_ENB,
	.pclk = &pll1_clk,
	.calc_rate = &aux_calc_rate,
	.recalc = &aux_clk_recalc,
	.set_rate = &aux_clk_set_rate,
	.rate_config = {aux_rtbl, ARRAY_SIZE(aux_rtbl), 2},
	.private_data = &uart_synth_config,
};

/* uart parents */
static struct pclk_info uart_pclk_info[] = {
	{
		.pclk = &uart_synth_clk,
		.pclk_val = AUX_CLK_PLL1_VAL,
	}, {
		.pclk = &pll3_48m_clk,
		.pclk_val = AUX_CLK_PLL3_VAL,
	},
};

/* uart parent select structure */
static struct pclk_sel uart_pclk_sel = {
	.pclk_info = uart_pclk_info,
	.pclk_count = ARRAY_SIZE(uart_pclk_info),
	.pclk_sel_reg = VA_PERIP_CLK_CFG,
	.pclk_sel_mask = UART_CLK_MASK,
};

/* uart0 clock */
static struct clk uart0_clk = {
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = UART0_CLK_ENB,
	.pclk_sel = &uart_pclk_sel,
	.pclk_sel_shift = UART_CLK_SHIFT,
	.recalc = &follow_parent,
};

/* uart1 clock */
static struct clk uart1_clk = {
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = UART1_CLK_ENB,
	.pclk_sel = &uart_pclk_sel,
	.pclk_sel_shift = UART_CLK_SHIFT,
	.recalc = &follow_parent,
};

/* firda configurations */
static struct aux_clk_config firda_synth_config = {
	.synth_reg = VA_FIRDA_CLK_SYNT,
	.masks = &aux_masks,
};

/* firda synth clock */
static struct clk firda_synth_clk = {
	.en_reg = VA_FIRDA_CLK_SYNT,
	.en_reg_bit = AUX_SYNT_ENB,
	.pclk = &pll1_clk,
	.calc_rate = &aux_calc_rate,
	.recalc = &aux_clk_recalc,
	.set_rate = &aux_clk_set_rate,
	.rate_config = {aux_rtbl, ARRAY_SIZE(aux_rtbl), 2},
	.private_data = &firda_synth_config,
};

/* firda parents */
static struct pclk_info firda_pclk_info[] = {
	{
		.pclk = &firda_synth_clk,
		.pclk_val = AUX_CLK_PLL1_VAL,
	}, {
		.pclk = &pll3_48m_clk,
		.pclk_val = AUX_CLK_PLL3_VAL,
	},
};

/* firda parent select structure */
static struct pclk_sel firda_pclk_sel = {
	.pclk_info = firda_pclk_info,
	.pclk_count = ARRAY_SIZE(firda_pclk_info),
	.pclk_sel_reg = VA_PERIP_CLK_CFG,
	.pclk_sel_mask = FIRDA_CLK_MASK,
};

/* firda clock */
static struct clk firda_clk = {
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = FIRDA_CLK_ENB,
	.pclk_sel = &firda_pclk_sel,
	.pclk_sel_shift = FIRDA_CLK_SHIFT,
	.recalc = &follow_parent,
};

/* clcd configurations */
static struct aux_clk_config clcd_synth_config = {
	.synth_reg = VA_CLCD_CLK_SYNT,
	.masks = &aux_masks,
};

/* firda synth clock */
static struct clk clcd_synth_clk = {
	.en_reg = VA_CLCD_CLK_SYNT,
	.en_reg_bit = AUX_SYNT_ENB,
	.pclk = &pll1_clk,
	.calc_rate = &aux_calc_rate,
	.recalc = &aux_clk_recalc,
	.set_rate = &aux_clk_set_rate,
	.rate_config = {aux_rtbl, ARRAY_SIZE(aux_rtbl), 2},
	.private_data = &clcd_synth_config,
};

/* clcd parents */
static struct pclk_info clcd_pclk_info[] = {
	{
		.pclk = &clcd_synth_clk,
		.pclk_val = AUX_CLK_PLL1_VAL,
	}, {
		.pclk = &pll3_48m_clk,
		.pclk_val = AUX_CLK_PLL3_VAL,
	},
};

/* clcd parent select structure */
static struct pclk_sel clcd_pclk_sel = {
	.pclk_info = clcd_pclk_info,
	.pclk_count = ARRAY_SIZE(clcd_pclk_info),
	.pclk_sel_reg = VA_PERIP_CLK_CFG,
	.pclk_sel_mask = CLCD_CLK_MASK,
};

/* clcd clock */
static struct clk clcd_clk = {
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = CLCD_CLK_ENB,
	.pclk_sel = &clcd_pclk_sel,
	.pclk_sel_shift = CLCD_CLK_SHIFT,
	.recalc = &follow_parent,
};

/* gpt synthesizer masks */
static struct gpt_clk_masks gpt_masks = {
	.mscale_sel_mask = GPT_MSCALE_MASK,
	.mscale_sel_shift = GPT_MSCALE_SHIFT,
	.nscale_sel_mask = GPT_NSCALE_MASK,
	.nscale_sel_shift = GPT_NSCALE_SHIFT,
};

/* gpt rate configuration table, in ascending order of rates */
struct gpt_rate_tbl gpt_rtbl[] = {
	/* For pll1 = 332 MHz */
	{.mscale = 4, .nscale = 0}, /* 41.5 MHz */
	{.mscale = 2, .nscale = 0}, /* 55.3 MHz */
	{.mscale = 1, .nscale = 0}, /* 83 MHz */
};

/* gpt0 synth clk config*/
static struct gpt_clk_config gpt0_synth_config = {
	.synth_reg = VA_PRSC1_CLK_CFG,
	.masks = &gpt_masks,
};

/* gpt synth clock */
static struct clk gpt0_synth_clk = {
	.flags = ALWAYS_ENABLED,
	.pclk = &pll1_clk,
	.calc_rate = &gpt_calc_rate,
	.recalc = &gpt_clk_recalc,
	.set_rate = &gpt_clk_set_rate,
	.rate_config = {gpt_rtbl, ARRAY_SIZE(gpt_rtbl), 2},
	.private_data = &gpt0_synth_config,
};

/* gpt parents */
static struct pclk_info gpt0_pclk_info[] = {
	{
		.pclk = &gpt0_synth_clk,
		.pclk_val = AUX_CLK_PLL1_VAL,
	}, {
		.pclk = &pll3_48m_clk,
		.pclk_val = AUX_CLK_PLL3_VAL,
	},
};

/* gpt parent select structure */
static struct pclk_sel gpt0_pclk_sel = {
	.pclk_info = gpt0_pclk_info,
	.pclk_count = ARRAY_SIZE(gpt0_pclk_info),
	.pclk_sel_reg = VA_PERIP_CLK_CFG,
	.pclk_sel_mask = GPT_CLK_MASK,
};

/* gpt0 ARM1 subsystem timer clock */
static struct clk gpt0_clk = {
	.flags = ALWAYS_ENABLED,
	.pclk_sel = &gpt0_pclk_sel,
	.pclk_sel_shift = GPT0_CLK_SHIFT,
	.recalc = &follow_parent,
};


/* Note: gpt0 and gpt1 share same parent clocks */
/* gpt parent select structure */
static struct pclk_sel gpt1_pclk_sel = {
	.pclk_info = gpt0_pclk_info,
	.pclk_count = ARRAY_SIZE(gpt0_pclk_info),
	.pclk_sel_reg = VA_PERIP_CLK_CFG,
	.pclk_sel_mask = GPT_CLK_MASK,
};

/* gpt1 timer clock */
static struct clk gpt1_clk = {
	.flags = ALWAYS_ENABLED,
	.pclk_sel = &gpt1_pclk_sel,
	.pclk_sel_shift = GPT1_CLK_SHIFT,
	.recalc = &follow_parent,
};

/* gpt2 synth clk config*/
static struct gpt_clk_config gpt2_synth_config = {
	.synth_reg = VA_PRSC2_CLK_CFG,
	.masks = &gpt_masks,
};

/* gpt synth clock */
static struct clk gpt2_synth_clk = {
	.flags = ALWAYS_ENABLED,
	.pclk = &pll1_clk,
	.calc_rate = &gpt_calc_rate,
	.recalc = &gpt_clk_recalc,
	.set_rate = &gpt_clk_set_rate,
	.rate_config = {gpt_rtbl, ARRAY_SIZE(gpt_rtbl), 2},
	.private_data = &gpt2_synth_config,
};

/* gpt parents */
static struct pclk_info gpt2_pclk_info[] = {
	{
		.pclk = &gpt2_synth_clk,
		.pclk_val = AUX_CLK_PLL1_VAL,
	}, {
		.pclk = &pll3_48m_clk,
		.pclk_val = AUX_CLK_PLL3_VAL,
	},
};

/* gpt parent select structure */
static struct pclk_sel gpt2_pclk_sel = {
	.pclk_info = gpt2_pclk_info,
	.pclk_count = ARRAY_SIZE(gpt2_pclk_info),
	.pclk_sel_reg = VA_PERIP_CLK_CFG,
	.pclk_sel_mask = GPT_CLK_MASK,
};

/* gpt2 timer clock */
static struct clk gpt2_clk = {
	.flags = ALWAYS_ENABLED,
	.pclk_sel = &gpt2_pclk_sel,
	.pclk_sel_shift = GPT2_CLK_SHIFT,
	.recalc = &follow_parent,
};

/* gpt3 synth clk config*/
static struct gpt_clk_config gpt3_synth_config = {
	.synth_reg = VA_PRSC3_CLK_CFG,
	.masks = &gpt_masks,
};

/* gpt synth clock */
static struct clk gpt3_synth_clk = {
	.flags = ALWAYS_ENABLED,
	.pclk = &pll1_clk,
	.calc_rate = &gpt_calc_rate,
	.recalc = &gpt_clk_recalc,
	.set_rate = &gpt_clk_set_rate,
	.rate_config = {gpt_rtbl, ARRAY_SIZE(gpt_rtbl), 2},
	.private_data = &gpt3_synth_config,
};

/* gpt parents */
static struct pclk_info gpt3_pclk_info[] = {
	{
		.pclk = &gpt3_synth_clk,
		.pclk_val = AUX_CLK_PLL1_VAL,
	}, {
		.pclk = &pll3_48m_clk,
		.pclk_val = AUX_CLK_PLL3_VAL,
	},
};

/* gpt parent select structure */
static struct pclk_sel gpt3_pclk_sel = {
	.pclk_info = gpt3_pclk_info,
	.pclk_count = ARRAY_SIZE(gpt3_pclk_info),
	.pclk_sel_reg = VA_PERIP_CLK_CFG,
	.pclk_sel_mask = GPT_CLK_MASK,
};

/* gpt3 timer clock */
static struct clk gpt3_clk = {
	.flags = ALWAYS_ENABLED,
	.pclk_sel = &gpt3_pclk_sel,
	.pclk_sel_shift = GPT3_CLK_SHIFT,
	.recalc = &follow_parent,
};

/* clock derived from pll3 clk */
/* usbh0 clock */
static struct clk usbh0_clk = {
	.pclk = &pll3_48m_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = USBH0_CLK_ENB,
	.recalc = &follow_parent,
};

/* usbh1 clock */
static struct clk usbh1_clk = {
	.pclk = &pll3_48m_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = USBH1_CLK_ENB,
	.recalc = &follow_parent,
};

/* usbd clock */
static struct clk usbd_clk = {
	.pclk = &pll3_48m_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = USBD_CLK_ENB,
	.recalc = &follow_parent,
};

/* clock derived from ahb clk */
/* ahb multiplied by 2 clock */
static struct clk ahbmult2_clk = {
	.flags = ALWAYS_ENABLED | SYSTEM_CLK,
	.pclk = &ahb_clk,
	.recalc = &ahbmult2_clk_recalc,
};

/* ddr clock */
struct ddr_rate_tbl ddr_rate_tbl = {
	.minrate = 166000000,
	.maxrate = 332000000,
};

static struct pclk_info ddr_pclk_info[] = {
	{
		.pclk = &ahb_clk,
		.pclk_val = MCTR_CLK_HCLK_VAL,
	}, {
		.pclk = &ahbmult2_clk,
		.pclk_val = MCTR_CLK_2HCLK_VAL,
	}, {
		.pclk = &pll2_clk,
		.pclk_val = MCTR_CLK_PLL2_VAL,
	},
};

/* ddr parent select structure */
static struct pclk_sel ddr_pclk_sel = {
	.pclk_info = ddr_pclk_info,
	.pclk_count = ARRAY_SIZE(ddr_pclk_info),
	.pclk_sel_reg = VA_PLL_CLK_CFG,
	.pclk_sel_mask = MCTR_CLK_MASK,
};

static struct clk ddr_clk = {
	.flags = ALWAYS_ENABLED | SYSTEM_CLK,
	.recalc = &follow_parent,
	.pclk_sel = &ddr_pclk_sel,
	.pclk_sel_shift = MCTR_CLK_SHIFT,
	.private_data = &ddr_rate_tbl,
};

/* apb masks structure */
static struct bus_clk_masks apb_masks = {
	.mask = HCLK_PCLK_RATIO_MASK,
	.shift = HCLK_PCLK_RATIO_SHIFT,
};

/* apb configuration structure */
static struct bus_clk_config apb_config = {
	.reg = VA_CORE_CLK_CFG,
	.masks = &apb_masks,
};

/* apb clock */
static struct clk apb_clk = {
	.flags = ALWAYS_ENABLED | SYSTEM_CLK,
	.pclk = &ahb_clk,
	.calc_rate = &bus_calc_rate,
	.recalc = &bus_clk_recalc,
	.set_rate = &bus_clk_set_rate,
	.rate_config = {bus_rtbl, ARRAY_SIZE(bus_rtbl), 2},
	.private_data = &apb_config,
};

/* i2c clock */
static struct clk i2c_clk = {
	.pclk = &ahb_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = I2C_CLK_ENB,
	.recalc = &follow_parent,
};

/* dma clock */
static struct clk dma_clk = {
	.pclk = &ahb_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = DMA_CLK_ENB,
	.recalc = &follow_parent,
};

/* jpeg clock */
static struct clk jpeg_clk = {
	.pclk = &ahb_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = JPEG_CLK_ENB,
	.recalc = &follow_parent,
};

/* gmac clock */
static struct clk gmac_clk = {
	.pclk = &ahb_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = GMAC_CLK_ENB,
	.recalc = &follow_parent,
};

/* smi clock */
static struct clk smi_clk = {
	.pclk = &ahb_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = SMI_CLK_ENB,
	.recalc = &follow_parent,
};

/* fsmc clock */
static struct clk fsmc_clk = {
	.pclk = &ahb_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = FSMC_CLK_ENB,
	.recalc = &follow_parent,
};

/* clock derived from apb clk */
/* adc clock */
static struct clk adc_clk = {
	.pclk = &apb_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = ADC_CLK_ENB,
	.recalc = &follow_parent,
};

/* ssp0 clock */
static struct clk ssp0_clk = {
	.pclk = &apb_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = SSP0_CLK_ENB,
	.recalc = &follow_parent,
};

/* ssp1 clock */
static struct clk ssp1_clk = {
	.pclk = &apb_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = SSP1_CLK_ENB,
	.recalc = &follow_parent,
};

/* ssp2 clock */
static struct clk ssp2_clk = {
	.pclk = &apb_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = SSP2_CLK_ENB,
	.recalc = &follow_parent,
};

/* gpio0 ARM subsystem clock */
static struct clk gpio0_clk = {
	.flags = ALWAYS_ENABLED,
	.pclk = &apb_clk,
	.recalc = &follow_parent,
};

/* gpio1 clock */
static struct clk gpio1_clk = {
	.pclk = &apb_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = GPIO1_CLK_ENB,
	.recalc = &follow_parent,
};

/* gpio2 clock */
static struct clk gpio2_clk = {
	.pclk = &apb_clk,
	.en_reg = VA_PERIP1_CLK_ENB,
	.en_reg_bit = GPIO2_CLK_ENB,
	.recalc = &follow_parent,
};

static struct clk dummy_apb_pclk;

/* array of all spear 6xx clock lookups */
static struct clk_lookup spear_clk_lookups[] = {
	{ .con_id = "apb_pclk",		.clk = &dummy_apb_pclk},
	/* root clks */
	{ .con_id = "osc_32k_clk",	.clk = &osc_32k_clk},
	{ .con_id = "osc_30m_clk",	.clk = &osc_30m_clk},
	/* clock derived from 32 KHz os		 clk */
	{ .dev_id = "rtc-spear",	.clk = &rtc_clk},
	/* clock derived from 30 MHz os		 clk */
	{ .con_id = "vco1_clk",		.clk = &vco1_clk},
	{ .con_id = "vco2_clk",		.clk = &vco2_clk},
	{ .con_id = "pll3_48m_clk",	.clk = &pll3_48m_clk},
	{ .dev_id = "wdt",		.clk = &wdt_clk},
	/* clock derived from vco1 clk */
	{ .con_id = "pll1_clk",		.clk = &pll1_clk},
	/* clock derived from vco2 clk */
	{ .con_id = "pll2_clk",		.clk = &pll2_clk},
	/* clock derived from pll1 clk */
	{ .con_id = "cpu_clk",		.clk = &cpu_clk},
	{ .con_id = "ahb_clk",		.clk = &ahb_clk},
	{ .con_id = "uart_synth_clk",	.clk = &uart_synth_clk},
	{ .con_id = "firda_synth_clk",	.clk = &firda_synth_clk},
	{ .con_id = "clcd_synth_clk",	.clk = &clcd_synth_clk},
	{ .con_id = "gpt0_synth_clk",	.clk = &gpt0_synth_clk},
	{ .con_id = "gpt2_synth_clk",	.clk = &gpt2_synth_clk},
	{ .con_id = "gpt3_synth_clk",	.clk = &gpt3_synth_clk},
	{ .dev_id = "uart0",		.clk = &uart0_clk},
	{ .dev_id = "uart1",		.clk = &uart1_clk},
	{ .dev_id = "dice_irda",	.clk = &firda_clk},
	{ .dev_id = "clcd",		.clk = &clcd_clk},
	{ .dev_id = "gpt0",		.clk = &gpt0_clk},
	{ .dev_id = "gpt1",		.clk = &gpt1_clk},
	{ .dev_id = "gpt2",		.clk = &gpt2_clk},
	{ .dev_id = "gpt3",		.clk = &gpt3_clk},
	/* clock derived from pll3 clk */
	{ .dev_id = "designware_udc",	.clk = &usbd_clk},
	{ .con_id = "usbh.0_clk",	.clk = &usbh0_clk},
	{ .con_id = "usbh.1_clk",	.clk = &usbh1_clk},
	/* clock derived from ahb clk */
	{ .con_id = "ahbmult2_clk",	.clk = &ahbmult2_clk},
	{ .con_id = "ddr_clk",		.clk = &ddr_clk},
	{ .con_id = "apb_clk",		.clk = &apb_clk},
	{ .dev_id = "i2c_designware.0",	.clk = &i2c_clk},
	{ .dev_id = "pl080_dmac",	.clk = &dma_clk},
	{ .dev_id = "jpeg-designware",	.clk = &jpeg_clk},
	{ .dev_id = "stmmaceth",	.clk = &gmac_clk},
	{ .dev_id = "smi",		.clk = &smi_clk},
	{ .dev_id = "fsmc-nand",	.clk = &fsmc_clk},
	/* clock derived from apb clk */
	{ .dev_id = "adc",		.clk = &adc_clk},
	{ .dev_id = "ssp-pl022.0",	.clk = &ssp0_clk},
	{ .dev_id = "ssp-pl022.1",	.clk = &ssp1_clk},
	{ .dev_id = "ssp-pl022.2",	.clk = &ssp2_clk},
	{ .dev_id = "gpio0",		.clk = &gpio0_clk},
	{ .dev_id = "gpio1",		.clk = &gpio1_clk},
	{ .dev_id = "gpio2",		.clk = &gpio2_clk},
};

/* machine clk init */
void __init spear6xx_clk_init(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(spear_clk_lookups); i++)
		clk_register(&spear_clk_lookups[i]);

	clk_init(&ddr_clk);
}
