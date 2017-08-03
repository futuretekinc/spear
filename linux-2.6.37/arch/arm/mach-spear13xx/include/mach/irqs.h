/*
 * arch/arm/mach-spear13xx/include/mach/irqs.h
 *
 * IRQ helper macros for spear13xx machine family
 *
 * Copyright (C) 2010 ST Microelectronics
 * Shiraz Hashim <shiraz.hashim@st.com>
 * Bhupesh Sharma <bhupesh.sharma@st.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef __MACH_IRQS_H
#define __MACH_IRQS_H

#include <linux/kernel.h>
#include <linux/mfd/stmpe.h>
#include <mach/gpio.h>

/* IRQ definitions */
/*
 * SGI : ID0 - ID15
 * PPI : ID16 - ID31
 * SHPI : ID32 - ID224
 */

#define IRQ_LOCALTIMER		29
#define IRQ_LOCALWDOG		30

/* Shared Peripheral Interrupt (SHPI) */
#define IRQ_SHPI_START		32

#if defined(CONFIG_CPU_SPEAR1300) || defined(CONFIG_CPU_SPEAR1310_REVA) || \
			defined(CONFIG_CPU_SPEAR900) || \
			defined(CONFIG_CPU_SPEAR1310)
#define SPEAR13XX_IRQ_PLAY_I2S0		(IRQ_SHPI_START + 10)
#define SPEAR13XX_IRQ_REC_I2S0		(IRQ_SHPI_START + 11)
#endif

#define SPEAR13XX_IRQ_PMU0		(IRQ_SHPI_START + 6)
#define SPEAR13XX_IRQ_PMU1		(IRQ_SHPI_START + 7)

#define SPEAR13XX_IRQ_ADC		(IRQ_SHPI_START + 12)
#define SPEAR13XX_IRQ_CLCD		(IRQ_SHPI_START + 13)
#define SPEAR13XX_IRQ_DMAC0_FLAG_0	(IRQ_SHPI_START + 14)
#define SPEAR13XX_IRQ_DMAC0_FLAG_1	(IRQ_SHPI_START + 15)
#define SPEAR13XX_IRQ_DMAC0_FLAG_2	(IRQ_SHPI_START + 16)
#define SPEAR13XX_IRQ_DMAC0_FLAG_3	(IRQ_SHPI_START + 17)
#define SPEAR13XX_IRQ_DMAC0_FLAG_4	(IRQ_SHPI_START + 18)
#define SPEAR13XX_IRQ_DMAC0_COMBINED	(IRQ_SHPI_START + 19)
#define SPEAR13XX_IRQ_FSMC0		(IRQ_SHPI_START + 20)
#define SPEAR13XX_IRQ_FSMC1		(IRQ_SHPI_START + 21)
#define SPEAR13XX_IRQ_FSMC2		(IRQ_SHPI_START + 22)
#define SPEAR13XX_IRQ_FSMC3		(IRQ_SHPI_START + 23)
#define SPEAR13XX_IRQ_GPIO0		(IRQ_SHPI_START + 24)
#define SPEAR13XX_IRQ_GPIO1		(IRQ_SHPI_START + 25)

#if defined(CONFIG_CPU_SPEAR1300) || defined(CONFIG_CPU_SPEAR1310_REVA) || \
			defined(CONFIG_CPU_SPEAR900) || \
			defined(CONFIG_CPU_SPEAR1310)
#define SPEAR13XX_IRQ_PLAY_I2S1		(IRQ_SHPI_START + 26)
#define SPEAR13XX_IRQ_JPEG		(IRQ_SHPI_START + 27)
#endif

#define SPEAR13XX_IRQ_SDHCI		(IRQ_SHPI_START + 28)
#define SPEAR13XX_IRQ_CF		(IRQ_SHPI_START + 29)
#define SPEAR13XX_IRQ_SMI		(IRQ_SHPI_START + 30)
#define SPEAR13XX_IRQ_SSP		(IRQ_SHPI_START + 31)
#define SPEAR13XX_IRQ_C3		(IRQ_SHPI_START + 32)
#define SPEAR13XX_IRQ_GETH_SBD		(IRQ_SHPI_START + 33)
#define SPEAR13XX_IRQ_GETH_PMT		(IRQ_SHPI_START + 34)
#define SPEAR13XX_IRQ_UART		(IRQ_SHPI_START + 35)
#define SPEAR13XX_IRQ_RTC		(IRQ_SHPI_START + 36)
#define SPEAR13XX_IRQ_GPT0_TMR0		(IRQ_SHPI_START + 37)
#define SPEAR13XX_IRQ_GPT0_TMR1		(IRQ_SHPI_START + 38)
#define SPEAR13XX_IRQ_GPT1_TMR0		(IRQ_SHPI_START + 39)
#define SPEAR13XX_IRQ_GPT1_TMR1		(IRQ_SHPI_START + 40)
#define SPEAR13XX_IRQ_I2C		(IRQ_SHPI_START + 41)
#define SPEAR13XX_IRQ_GPT2_TMR0		(IRQ_SHPI_START + 42)
#define SPEAR13XX_IRQ_GPT2_TMR1		(IRQ_SHPI_START + 43)
#define SPEAR13XX_IRQ_GPT3_TMR0		(IRQ_SHPI_START + 44)
#define SPEAR13XX_IRQ_GPT3_TMR1		(IRQ_SHPI_START + 45)

#define SPEAR13XX_IRQ_KBD		(IRQ_SHPI_START + 52)

#if defined(CONFIG_CPU_SPEAR1300) || defined(CONFIG_CPU_SPEAR1310_REVA) || \
			defined(CONFIG_CPU_SPEAR900) || \
			defined(CONFIG_CPU_SPEAR1310)
#define SPEAR13XX_IRQ_REC_I2S1		(IRQ_SHPI_START + 53)
#endif

#define SPEAR13XX_IRQ_DMAC1_FLAG_0	(IRQ_SHPI_START + 54)
#define SPEAR13XX_IRQ_DMAC1_FLAG_1	(IRQ_SHPI_START + 55)
#define SPEAR13XX_IRQ_DMAC1_FLAG_2	(IRQ_SHPI_START + 56)
#define SPEAR13XX_IRQ_DMAC1_FLAG_3	(IRQ_SHPI_START + 57)
#define SPEAR13XX_IRQ_DMAC1_FLAG_4	(IRQ_SHPI_START + 58)
#define SPEAR13XX_IRQ_DMAC1_COMBINED	(IRQ_SHPI_START + 59)


#if defined(CONFIG_CPU_SPEAR1300) || defined(CONFIG_CPU_SPEAR1310_REVA) || \
			defined(CONFIG_CPU_SPEAR900)
#define SPEAR13XX_IRQ_UDC		(IRQ_SHPI_START + 62)
#define SPEAR13XX_IRQ_UPD		(IRQ_SHPI_START + 63)
#endif

#define SPEAR13XX_IRQ_USBH_EHCI0	(IRQ_SHPI_START + 64)
#define SPEAR13XX_IRQ_USBH_OHCI0	(IRQ_SHPI_START + 65)
#define SPEAR13XX_IRQ_USBH_EHCI1	(IRQ_SHPI_START + 66)
#define SPEAR13XX_IRQ_USBH_OHCI1	(IRQ_SHPI_START + 67)
#define SPEAR13XX_IRQ_PCIE0		(IRQ_SHPI_START + 68)

#if defined(CONFIG_CPU_SPEAR1300) || defined(CONFIG_CPU_SPEAR1310_REVA) || \
			defined(CONFIG_CPU_SPEAR900) || \
			defined(CONFIG_CPU_SPEAR1310)
#define SPEAR13XX_IRQ_PCIE1		(IRQ_SHPI_START + 69)
#define SPEAR13XX_IRQ_PCIE2		(IRQ_SHPI_START + 70)
#endif

/* Add spear1310_reva specific IRQs here */
#ifdef CONFIG_CPU_SPEAR1310_REVA
#define SPEAR1310_REVA_IRQ_FSMC_PC1		(IRQ_SHPI_START + 76)
#define SPEAR1310_REVA_IRQ_FSMC_PC2		(IRQ_SHPI_START + 77)
#define SPEAR1310_REVA_IRQ_FSMC_PC3		(IRQ_SHPI_START + 78)
#define SPEAR1310_REVA_IRQ_FSMC_PC4		(IRQ_SHPI_START + 79)
#define SPEAR1310_REVA_IRQ_RS4850		(IRQ_SHPI_START + 80)
#define SPEAR1310_REVA_IRQ_RS4851		(IRQ_SHPI_START + 81)
#define SPEAR1310_REVA_IRQ_CCAN0		(IRQ_SHPI_START + 82)
#define SPEAR1310_REVA_IRQ_CCAN1		(IRQ_SHPI_START + 83)
#define SPEAR1310_REVA_IRQ_TDM0			(IRQ_SHPI_START + 84)
#define SPEAR1310_REVA_IRQ_TDM1			(IRQ_SHPI_START + 85)
#define SPEAR1310_REVA_IRQ_UART1		(IRQ_SHPI_START + 86)
#define SPEAR1310_REVA_IRQ_UART2		(IRQ_SHPI_START + 87)
#define SPEAR1310_REVA_IRQ_UART3		(IRQ_SHPI_START + 88)
#define SPEAR1310_REVA_IRQ_UART4		(IRQ_SHPI_START + 89)
#define SPEAR1310_REVA_IRQ_UART5		(IRQ_SHPI_START + 90)
#define SPEAR1310_REVA_IRQ_I2C_CNTR		(IRQ_SHPI_START + 91)
#define SPEAR1310_REVA_IRQ_GETH1_SBD		(IRQ_SHPI_START + 92)
#define SPEAR1310_REVA_IRQ_GETH1_PMT		(IRQ_SHPI_START + 93)
#define SPEAR1310_REVA_IRQ_GETH2_SBD		(IRQ_SHPI_START + 94)
#define SPEAR1310_REVA_IRQ_GETH2_PMT		(IRQ_SHPI_START + 95)
#define SPEAR1310_REVA_IRQ_GETH3_SBD		(IRQ_SHPI_START + 96)
#define SPEAR1310_REVA_IRQ_GETH3_PMT		(IRQ_SHPI_START + 97)
#define SPEAR1310_REVA_IRQ_GETH4_SBD		(IRQ_SHPI_START + 98)
#define SPEAR1310_REVA_IRQ_GETH4_PMT		(IRQ_SHPI_START + 99)
#define SPEAR1310_REVA_IRQ_PLGPIO		(IRQ_SHPI_START + 100)
#define SPEAR1310_REVA_IRQ_PCI_BRDG_HOST_FATAL	(IRQ_SHPI_START + 101)
#define SPEAR1310_REVA_IRQ_PCI_INTA		(IRQ_SHPI_START + 102)
#define SPEAR1310_REVA_IRQ_PCI_INTB		(IRQ_SHPI_START + 103)
#define SPEAR1310_REVA_IRQ_PCI_INTC		(IRQ_SHPI_START + 104)
#define SPEAR1310_REVA_IRQ_PCI_INTD		(IRQ_SHPI_START + 105)
#define SPEAR1310_REVA_IRQ_PCI_ME_TO_ARM	(IRQ_SHPI_START + 106)
#define SPEAR1310_REVA_IRQ_PCI_SERR_TO_ARM	(IRQ_SHPI_START + 107)
#endif /* CONFIG_CPU_SPEAR1310_REVA */

/* Add spear1310 specific IRQs here */
#ifdef CONFIG_CPU_SPEAR1310
#define SPEAR1310_IRQ_FSMC_FF		(IRQ_SHPI_START + 51) /* Fill FIFO */
#define SPEAR1310_IRQ_UOC		(IRQ_SHPI_START + 62)
#define SPEAR1310_IRQ_FSMC_CR		(IRQ_SHPI_START + 63) /* Code Ready */
#define SPEAR1310_IRQ_SATA0		(IRQ_SHPI_START + 68)
#define SPEAR1310_IRQ_SATA1		(IRQ_SHPI_START + 69)
#define SPEAR1310_IRQ_SATA2		(IRQ_SHPI_START + 70)
#define SPEAR1310_IRQ_GMAC		(IRQ_SHPI_START + 71)

#define SPEAR1310_IRQ_CAN0		(IRQ_SHPI_START + 76)
#define SPEAR1310_IRQ_CAN1		(IRQ_SHPI_START + 77)
#define SPEAR1310_IRQ_RS485_0		(IRQ_SHPI_START + 78)
#define SPEAR1310_IRQ_RS485_1		(IRQ_SHPI_START + 79)
#define SPEAR1310_IRQ_TDM0		(IRQ_SHPI_START + 80)
#define SPEAR1310_IRQ_TDM1		(IRQ_SHPI_START + 81)
#define SPEAR1310_IRQ_UART1		(IRQ_SHPI_START + 82)
#define SPEAR1310_IRQ_UART2		(IRQ_SHPI_START + 83)
#define SPEAR1310_IRQ_UART3		(IRQ_SHPI_START + 84)
#define SPEAR1310_IRQ_UART4		(IRQ_SHPI_START + 85)
#define SPEAR1310_IRQ_UART5		(IRQ_SHPI_START + 86)
#define SPEAR1310_IRQ_I2C1		(IRQ_SHPI_START + 87)
#define SPEAR1310_IRQ_I2C2		(IRQ_SHPI_START + 88)
#define SPEAR1310_IRQ_I2C3		(IRQ_SHPI_START + 89)
#define SPEAR1310_IRQ_I2C4		(IRQ_SHPI_START + 90)
#define SPEAR1310_IRQ_I2C5		(IRQ_SHPI_START + 91)
#define SPEAR1310_IRQ_I2C6		(IRQ_SHPI_START + 92)
#define SPEAR1310_IRQ_I2C7		(IRQ_SHPI_START + 93)
#define SPEAR1310_IRQ_GPT64		(IRQ_SHPI_START + 94)
#define SPEAR1310_IRQ_MAC1		(IRQ_SHPI_START + 95)
#define SPEAR1310_IRQ_MAC2		(IRQ_SHPI_START + 96)
#define SPEAR1310_IRQ_MAC3		(IRQ_SHPI_START + 97)
#define SPEAR1310_IRQ_MAC4		(IRQ_SHPI_START + 98)
#define SPEAR1310_IRQ_SSP1		(IRQ_SHPI_START + 99)
#define SPEAR1310_IRQ_PLGPIO		(IRQ_SHPI_START + 100)
#define SPEAR1310_IRQ_PCI_BRIDGE	(IRQ_SHPI_START + 101)
#define SPEAR1310_IRQ_PCI_RST_IN	(IRQ_SHPI_START + 102)
#define SPEAR1310_IRQ_PCI_BRIDGE_RDY	(IRQ_SHPI_START + 103)
#define SPEAR1310_IRQ_PCI_INT_ABCD	(IRQ_SHPI_START + 104)
#define SPEAR1310_IRQ_PCI_ME_TO_ARM	(IRQ_SHPI_START + 105)
#define SPEAR1310_IRQ_PCI_SERR_TO_ARM	(IRQ_SHPI_START + 106)
#endif /* CONFIG_CPU_SPEAR1310 */

/* Add spear1340 specific IRQs here */
#ifdef CONFIG_CPU_SPEAR1340
#define SPEAR1340_IRQ_FSMC_FF		(IRQ_SHPI_START + 51) /* Fill FIFO */
#define SPEAR1340_IRQ_UOC		(IRQ_SHPI_START + 62)
#define SPEAR1340_IRQ_FSMC_CR		(IRQ_SHPI_START + 63) /* Code Ready */
#define SPEAR1340_IRQ_GMAC		(IRQ_SHPI_START + 71)
#define SPEAR1340_IRQ_SATA		(IRQ_SHPI_START + 72) /* SATA */
#define SPEAR1340_IRQ_VIDEO_DEC		(IRQ_SHPI_START + 81)
#define SPEAR1340_IRQ_VIDEO_ENC		(IRQ_SHPI_START + 82)
#define SPEAR1340_IRQ_SPDIF_IN		(IRQ_SHPI_START + 84)
#define SPEAR1340_IRQ_SPDIF_OUT		(IRQ_SHPI_START + 85)
#define SPEAR1340_IRQ_VIP		(IRQ_SHPI_START + 86)
#define SPEAR1340_IRQ_CAM0_CE		(IRQ_SHPI_START + 87) /* Camline End */
#define SPEAR1340_IRQ_CAM0_FVE		(IRQ_SHPI_START + 88) /* Frame vsync */
#define SPEAR1340_IRQ_CAM1_CE		(IRQ_SHPI_START + 89) /* Camline End */
#define SPEAR1340_IRQ_CAM1_FVE		(IRQ_SHPI_START + 90) /* Frame vsync */
#define SPEAR1340_IRQ_CAM2_CE		(IRQ_SHPI_START + 91) /* Camline End */
#define SPEAR1340_IRQ_CAM2_FVE		(IRQ_SHPI_START + 92) /* Frame vsync */
#define SPEAR1340_IRQ_CAM3_CE		(IRQ_SHPI_START + 93) /* Camline End */
#define SPEAR1340_IRQ_CAM3_FVE		(IRQ_SHPI_START + 94) /* Frame vsync */
#define SPEAR1340_IRQ_MALI_GPU_M200	(IRQ_SHPI_START + 95) /* M200 */
#define SPEAR1340_IRQ_MALI_GPU_MGP2	(IRQ_SHPI_START + 96) /* MGP2 */
#define SPEAR1340_IRQ_MALI_GPU_MMU	(IRQ_SHPI_START + 97) /* MMU */
#define SPEAR1340_IRQ_I2S_PLAY_EMP_M	(IRQ_SHPI_START + 98) /* Empty */
#define SPEAR1340_IRQ_I2S_PLAY_OR_M	(IRQ_SHPI_START + 99) /* Over Run */
#define SPEAR1340_IRQ_I2S_REC_DA_S	(IRQ_SHPI_START + 100) /* Data Avail */
#define SPEAR1340_IRQ_I2S_REC_OR_S	(IRQ_SHPI_START + 101) /* Over Run */
#define SPEAR1340_IRQ_CEC0		(IRQ_SHPI_START + 102)
#define SPEAR1340_IRQ_CEC1		(IRQ_SHPI_START + 103)
#define SPEAR1340_IRQ_I2C1		(IRQ_SHPI_START + 104)
#define SPEAR1340_IRQ_UART1		(IRQ_SHPI_START + 105)
#define SPEAR1340_IRQ_PLGPIO		(IRQ_SHPI_START + 107)
#define SPEAR1340_IRQ_MIPHY		(IRQ_SHPI_START + 110)
#endif /* CONFIG_CPU_SPEAR1340 */

#define IRQ_GIC_END		(IRQ_SHPI_START + 128)

#define VIRQ_START		IRQ_GIC_END

/* GPIO pins virtual irqs */
#define SPEAR_GPIO0_INT_BASE	(VIRQ_START + 0)
#define SPEAR_GPIO1_INT_BASE	(SPEAR_GPIO0_INT_BASE + 8)

#define SPEAR_PLGPIO_COUNT	250
#if defined(CONFIG_CPU_SPEAR1310_REVA) || defined(CONFIG_CPU_SPEAR1310) || \
		defined(CONFIG_CPU_SPEAR1340)
#define SPEAR_PLGPIO_INT_BASE	(SPEAR_GPIO1_INT_BASE + 8)
#define SPEAR_GPIO_INT_END	(SPEAR_PLGPIO_INT_BASE + \
				DIV_ROUND_UP(SPEAR_PLGPIO_COUNT,\
					PLGPIO_GROUP_SIZE))
#else
#define SPEAR_GPIO_INT_END	(SPEAR_GPIO1_INT_BASE + 8)
#endif

/* PCIE MSI virtual irqs */
#define SPEAR_NUM_MSI_IRQS	64
#define SPEAR_MSI0_INT_BASE	(SPEAR_GPIO_INT_END + 0)
#define SPEAR_MSI0_INT_END	(SPEAR_MSI0_INT_BASE + SPEAR_NUM_MSI_IRQS)
#define SPEAR_MSI1_INT_BASE	(SPEAR_MSI0_INT_END + 0)
#define SPEAR_MSI1_INT_END	(SPEAR_MSI1_INT_BASE + SPEAR_NUM_MSI_IRQS)
#define SPEAR_MSI2_INT_BASE	(SPEAR_MSI1_INT_END + 0)
#define SPEAR_MSI2_INT_END	(SPEAR_MSI2_INT_BASE + SPEAR_NUM_MSI_IRQS)

#define SPEAR_NUM_INTX_IRQS	4
#define SPEAR_INTX0_BASE	(SPEAR_MSI2_INT_END + 0)
#define SPEAR_INTX0_END		(SPEAR_INTX0_BASE + SPEAR_NUM_INTX_IRQS)
#define SPEAR_INTX1_BASE	(SPEAR_INTX0_END + 0)
#define SPEAR_INTX1_END		(SPEAR_INTX1_BASE + SPEAR_NUM_INTX_IRQS)
#define SPEAR_INTX2_BASE	(SPEAR_INTX1_END + 0)
#define SPEAR_INTX2_END		(SPEAR_INTX2_BASE + SPEAR_NUM_INTX_IRQS)

#ifdef CONFIG_ARCH_SPEAR13XX
 #ifdef CONFIG_MACH_SPEAR1340_EVB
  #define SPEAR_STMPE801_GPIO_INT_BASE	SPEAR_INTX2_END
  #define SPEAR_STMPE801_GPIO_INT_END	(SPEAR_STMPE801_GPIO_INT_BASE + STMPE_NR_IRQS)
  #define SPEAR_STMPE610_INT_BASE	SPEAR_STMPE801_GPIO_INT_END
 #else
  #define SPEAR_STMPE610_INT_BASE	SPEAR_INTX2_END
 #endif

 #define VIRQ_END			(SPEAR_STMPE610_INT_BASE + STMPE_NR_INTERNAL_IRQS)
#else
 #define VIRQ_END			SPEAR_INTX2_END
#endif

#define NR_IRQS			VIRQ_END

#endif /* __MACH_IRQS_H */