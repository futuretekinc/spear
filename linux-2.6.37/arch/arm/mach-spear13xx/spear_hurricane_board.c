/*
 * arch/arm/mach-spear13xx/spear_hurricane_board.c
 *
 * NComputing SPEAr Hurricane board source file
 *
 * Copyright (C) 2011 ST Microelectronics
 * Vincenzo Frascino <vincenzo.frascino@st.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/clk.h>
#include <linux/types.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/i2c/l3g4200d.h>
#include <linux/irq.h>
#include <linux/mtd/fsmc.h>
#include <linux/mtd/nand.h>
#include <linux/netdevice.h>
#include <linux/pata_arasan_cf_data.h>
#include <linux/phy.h>
#include <linux/spi/spi.h>
#include <linux/stmmac.h>
#include <video/db9000fb.h>
#include <plat/fsmc.h>
#include <plat/keyboard.h>
#include <plat/smi.h>
#include <plat/spi.h>
#include <mach/generic.h>
#include <mach/gpio.h>
#include <mach/hardware.h>
#include <mach/plug_board.h>
#include <mach/spear1340_misc_regs.h>
#include <mach/spear_pcie.h>

#if 0
/* fsmc nor partition info */
#define PARTITION(n, off, sz)	{.name = n, .offset = off, .size = sz}
static struct mtd_partition partition_info[] = {
	PARTITION("X-loader", 0, 1 * 0x20000),
	PARTITION("U-Boot", 0x20000, 3 * 0x20000),
	PARTITION("Kernel", 0x80000, 24 * 0x20000),
	PARTITION("Root File System", 0x380000, 84 * 0x20000),
};
#endif

/* Ethernet phy-0 device registeration */
static struct plat_stmmacphy_data phy0_private_data = {
	.bus_id = 0,
	.phy_addr = -1,
	.phy_mask = 0,
	.interface = PHY_INTERFACE_MODE_GMII,
	.phy_clk_cfg = spear13xx_eth_phy_clk_cfg,
};

static struct resource phy0_resources = {
	.name = "phyirq",
	.start = -1,
	.end = -1,
	.flags = IORESOURCE_IRQ,
};

struct platform_device spear_hurricane_phy0_device = {
	.name		= "stmmacphy",
	.id		= 0,
	.num_resources	= 1,
	.resource	= &phy0_resources,
	.dev.platform_data = &phy0_private_data,
};

/*
 * Pad multiplexing for making few pads as plgpio's.
 * Please retain original values and addresses, and update only mask as
 * required.
 * For example: if we need to enable plgpio's on pads: 15, 28, 45 & 102.
 * They corresponds to following bits in registers: 16, 29, 46 & 103
 * So following mask entries will solve this purpose:
 * Reg1: .mask = 0x20010000,
 * Reg2: .mask = 0x00004000,
 * Reg4: .mask = 0x00000080,
 *
 * Note: Counting of bits and pads start from 0.
 */
static struct pmx_mux_reg pmx_plgpios_mux[] = {
	{
		.address = SPEAR1340_PAD_FUNCTION_EN_1,
		.mask = 0x0,
		.value = 0x0,
	}, {
		.address = SPEAR1340_PAD_FUNCTION_EN_2,
		.mask = 0x0,
		.value = 0x0,
	}, {
		.address = SPEAR1340_PAD_FUNCTION_EN_3,
		.mask = 0x0,
		.value = 0x0,
	}, {
		.address = SPEAR1340_PAD_FUNCTION_EN_4,
		.mask = 0x0,
		.value = 0x0,
	}, {
		.address = SPEAR1340_PAD_FUNCTION_EN_5,
		.mask = 0x0,
		.value = 0x0,
	}, {
		.address = SPEAR1340_PAD_FUNCTION_EN_6,
		.mask = 0x0,
		.value = 0x0,
	}, {
		.address = SPEAR1340_PAD_FUNCTION_EN_7,
		.mask = 0x0,
		.value = 0x0,
	}, {
		.address = SPEAR1340_PAD_FUNCTION_EN_8,
		.mask = 0x1000000,
		.value = 0x0,
	},
};

static struct pmx_dev_mode pmx_plgpios_modes[] = {
	{
		.mux_regs = pmx_plgpios_mux,
		.mux_reg_cnt = ARRAY_SIZE(pmx_plgpios_mux),
	},
};

static struct pmx_dev spear1340_pmx_plgpios = {
	.name = "plgpios",
	.modes = pmx_plgpios_modes,
	.mode_count = ARRAY_SIZE(pmx_plgpios_modes),
};

/* padmux devices to enable */
static struct pmx_dev *pmx_devs[] = {
	/*
	 * Keep pads_as_gpio as the first element in this array. Don't ever
	 * remove it. It makes all pads as gpio's in starting, and then pads are
	 * configured as peripherals wherever required.
	 */
	&spear1340_pmx_pads_as_gpio,
	&spear1340_pmx_fsmc_8bit,
	&spear1340_pmx_keyboard_row_col,
#if !defined(CONFIG_PM)
	&spear1340_pmx_keyboard_col5,
#endif
	&spear1340_pmx_uart0_enh,
	&spear1340_pmx_i2c1,
	&spear1340_pmx_spdif_in,
	&spear1340_pmx_ssp0_cs1,
#if !defined(CONFIG_PM)
	&spear1340_pmx_pwm2,
#endif
	&spear1340_pmx_pwm3,
	&spear1340_pmx_smi,
	&spear1340_pmx_ssp0,
	&spear1340_pmx_uart0,
	&spear1340_pmx_i2s_in,
	&spear1340_pmx_i2s_out,
	&spear1340_pmx_gmac,
	&spear1340_pmx_ssp0_cs3,
	&spear1340_pmx_i2c0,
	&spear1340_pmx_cec0,
	&spear1340_pmx_cec1,
	&spear1340_pmx_spdif_out,
	&spear1340_pmx_mcif,
	&spear1340_pmx_sdhci,
	&spear1340_pmx_clcd,
	&spear1340_pmx_clcd_gpio_pd,
	&spear1340_pmx_devs_grp,
	&spear1340_pmx_gmii,
	&spear1340_pmx_pcie,

	/* Keep this entry at the bottom of table to override earlier setting */
	&spear1340_pmx_plgpios,
};

static struct amba_device *amba_devs[] __initdata = {
	/* spear13xx specific devices */
	&spear13xx_gpio_device[0],
	&spear13xx_gpio_device[1],
	&spear13xx_ssp_device,
	&spear13xx_uart_device,

	/* spear1340 specific devices */
	&spear1340_uart1_device,
};

static struct platform_device *plat_devs[] __initdata = {
	/* spear13xx specific devices */
	&spear13xx_adc_device,
	&spear13xx_db9000_clcd_device,
	&spear13xx_dmac_device[0],
	&spear13xx_dmac_device[1],
	&spear13xx_ehci0_device,
	&spear13xx_ehci1_device,
	&spear13xx_eth_device,
	&spear13xx_i2c_device,
	&spear1340_nand_device,
	&spear1340_i2s_play_device,
	&spear1340_i2s_record_device,
	&spear13xx_ohci0_device,
	&spear13xx_ohci1_device,
	&spear13xx_pcm_device,
	&spear13xx_pcie_host0_device,
	&spear13xx_rtc_device,
	&spear13xx_sdhci_device,
	&spear13xx_smi_device,
	&spear1340_spdif_out_device,
	&spear13xx_wdt_device,

	/* spear1340 specific devices */
	&spear1340_cec0_device,
	&spear1340_cec1_device,
	&spear1340_cpufreq_device,
#if defined(CONFIG_KEYBOARD_GPIO) || defined(CONFIG_KEYBOARD_GPIO_MODULE)
	&spear1340_gpiokeys_device,
#endif
	&spear1340_i2c1_device,
	&spear1340_pwm_device,
	&spear1340_phy0_device,
	&spear1340_plgpio_device,
	&spear1340_otg_device,
	&spear1340_thermal_device,
};

static struct arasan_cf_pdata cf_pdata = {
	.cf_if_clk = CF_IF_CLK_166M,
	.quirk = CF_BROKEN_UDMA,
	.dma_priv = &cf_dma_priv,
};

/* keyboard specific platform data */
static DECLARE_6x6_KEYMAP(keymap);
static struct matrix_keymap_data keymap_data = {
	.keymap = keymap,
	.keymap_size = ARRAY_SIZE(keymap),
};

static struct kbd_platform_data kbd_data = {
	.keymap = &keymap_data,
	.rep = 1,
	.mode = KEYPAD_2x2,
	.suspended_rate = 2000000,
};

/* Ethernet specific plat data */
static struct plat_stmmacenet_data eth_data = {
	.bus_id = 0,
	.has_gmac = 1,
	.enh_desc = 1,
	.tx_coe = 1,
	.pbl = 16,
	.csum_off_engine = STMAC_TYPE_2,
	.bugged_jumbo = 1,
	.features = NETIF_F_HW_CSUM,
	.pmt = 1,
};

/* Initializing platform data for spear1340 evb specific I2C devices */
/* Gyroscope platform data */
static struct l3g4200d_gyr_platform_data l3g4200d_pdata = {
	.poll_interval = 5,
	.min_interval = 2,
	.axis_map_x = 0,
	.axis_map_y = 1,
	.axis_map_z = 2,
};

struct i2c_board_info spear_hurricane_board_i2c_l3g4200d_gyr = {
	/* gyroscope board info */
	.type = "l3g4200d_gyr",
	.addr = 0x69,
	.platform_data = &l3g4200d_pdata,
};

struct i2c_board_info spear_hurricane_board_i2c_eeprom0 = {
	.type = "eeprom",
	.addr = 0x50,
};

struct i2c_board_info spear_hurricane_board_i2c_eeprom1 = {
	.type = "eeprom",
	.addr = 0x51,
};

struct i2c_board_info spear_hurricane_board_i2c_sta529 = {
	.type = "sta529",
	.addr = 0x1a,
};

static struct i2c_board_info *i2c_board[] __initdata = {
	&spear_hurricane_board_i2c_l3g4200d_gyr,
	&spear_hurricane_board_i2c_eeprom0,
	&spear_hurricane_board_i2c_eeprom1,
	&spear_hurricane_board_i2c_sta529,
};

/* Definitions for SPI Devices*/

/* spi master's configuration routine */
DECLARE_SPI_CS_CFG(0, VA_SPEAR1340_PERIP_CFG, SPEAR1340_SSP_CS_SEL_MASK,
		SPEAR1340_SSP_CS_SEL_SHIFT, SPEAR1340_SSP_CS_CTL_MASK,
		SPEAR1340_SSP_CS_CTL_SHIFT, SPEAR1340_SSP_CS_CTL_SW,
		SPEAR1340_SSP_CS_VAL_MASK, SPEAR1340_SSP_CS_VAL_SHIFT);

/* spi0 flash Chip Select Control function */
DECLARE_SPI_CS_CONTROL(0, flash, SPEAR1340_SSP_CS_SEL_CS0);
/* spi0 flash Chip Info structure */
DECLARE_SPI_CHIP_INFO(0, flash, spi0_flash_cs_control);

/* spi0 spidev Chip Select Control function */
DECLARE_SPI_CS_CONTROL(0, dev, SPEAR1340_SSP_CS_SEL_CS2);
/* spi0 spidev Chip Info structure */
DECLARE_SPI_CHIP_INFO(0, dev, spi0_dev_cs_control);

struct spi_board_info spear_hurricane_board_spi_m25p80 = {
	.modalias = "m25p80",
	.controller_data = &spi0_flash_chip_info,
	.max_speed_hz = 12000000,
	.bus_num = 0,
	.chip_select = SPEAR1340_SSP_CS_SEL_CS0,
	.mode = SPI_MODE_3,
};
struct spi_board_info spear_hurricane_board_spi_spidev = {
	.modalias = "spidev",
	.controller_data = &spi0_dev_chip_info,
	.max_speed_hz = 25000000,
	.bus_num = 0,
	.chip_select = SPEAR1340_SSP_CS_SEL_CS2,
	.mode = SPI_MODE_1,
};

static struct spi_board_info *spi_board[] __initdata = {
	&spear_hurricane_board_spi_m25p80,
	&spear_hurricane_board_spi_spidev,
};

#ifdef CONFIG_SPEAR_PCIE_REV370
/* This function is needed for board specific PCIe initilization */
static void __init spear_hurricane_pcie_board_init(void)
{
	void *plat_data;

	plat_data = dev_get_platdata(&spear13xx_pcie_host0_device.dev);
	PCIE_PORT_INIT((struct pcie_port_info *)plat_data, SPEAR_PCIE_REV_3_70);
}
#endif

static void spear_hurricane_fixup(struct machine_desc *desc, struct tag *tags,
		char **cmdline, struct meminfo *mi)
{
#if defined(CONFIG_FB_DB9000) || defined(CONFIG_FB_DB9000_MODULE)
	spear13xx_panel_fixup(mi);
#endif
}

static void __init spear_hurricane_init(void)
{
	unsigned int i;

	/* set compact flash plat data */
	set_arasan_cf_pdata(&spear13xx_cf_device, &cf_pdata);

#if (defined(CONFIG_FB_DB9000) || defined(CONFIG_FB_DB9000_MODULE))
	/* db9000_clcd plat data */
	spear13xx_panel_init(&spear13xx_db9000_clcd_device);
#endif

	/* set keyboard plat data */
	kbd_set_plat_data(&spear13xx_kbd_device, &kbd_data);

	/*
	 * SPEAr1340 has gmac configured differently. Hence set its plat
	 * data separately.
	 */
	spear13xx_eth_device.dev.platform_data = &eth_data;

	/* initialize serial nor related data in smi plat data */
	smi_init_board_info(&spear13xx_smi_device);

	/*
	 * SPEAr1340 FSMC cannot used as NOR and NAND at the same time
	 * For the moment, disable NOR and use NAND only
	 * If NOR is needed, enable NOR's code and disable all code for NOR.
	 */
	/* set nand device's plat data */
	/* set nand device's plat data */
	fsmc_nand_set_plat_data(&spear1340_nand_device, NULL, 0,
			NAND_SKIP_BBTSCAN, FSMC_NAND_BW8, NULL, 1);
	nand_mach_init(FSMC_NAND_BW8);

#if 0
	/* fixed part fsmc nor device */
	/* initialize fsmc related data in fsmc plat data */
	fsmc_init_board_info(&spear13xx_fsmc_nor_device, partition_info,
			ARRAY_SIZE(partition_info), FSMC_FLASH_WIDTH8);
	/* Initialize fsmc regiters */
	fsmc_nor_init(&spear13xx_fsmc_nor_device, SPEAR13XX_FSMC_BASE, 0,
			FSMC_FLASH_WIDTH8);
#endif

#ifdef CONFIG_SPEAR_PCIE_REV370
	spear_hurricane_pcie_board_init();
#endif

	/* call spear1340 machine init function */
	spear1340_init(NULL, pmx_devs, ARRAY_SIZE(pmx_devs));

	/* Register spear1340 evb board specific i2c slave devices */
	for (i = 0; i < ARRAY_SIZE(i2c_board); i++)
		i2c_register_board_info(0, i2c_board[i], 1);

	/* Register SPI Board */
	for (i = 0; i < ARRAY_SIZE(spi_board); i++)
		spi_register_board_info(spi_board[i], 1);

	/* Add Platform Devices */
	platform_add_devices(plat_devs, ARRAY_SIZE(plat_devs));

	/* Add Amba Devices */
	for (i = 0; i < ARRAY_SIZE(amba_devs); i++)
		amba_device_register(amba_devs[i], &iomem_resource);
}

MACHINE_START(SPEAR_HURRICANE, "NCOMPUTING-SPEAR-HURRICANE-BOARD")
	.boot_params	=	0x00000100,
	.fixup		=	spear_hurricane_fixup,
	.map_io		=	spear13xx_map_io,
	.init_irq	=	spear13xx_init_irq,
	.timer		=	&spear13xx_timer,
	.init_machine	=	spear_hurricane_init,
MACHINE_END
