#
# Copyright (C) 2010-2012 ARM Limited. All rights reserved.
# 
# This program is free software and is provided to you under the terms of the GNU General Public License version 2
# as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
# 
# A copy of the licence is included with the program, and can also be obtained from Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#

UMP_FILE_PREFIX =
UDD_FILE_PREFIX = ../mali/

ifneq ($(KBUILD_EXTMOD),)
DRIVER_DIR=$(KBUILD_EXTMOD)
include $(KBUILD_EXTMOD)/Makefile.common
else
src?=.
srctree?=.
DRIVER_DIR?=$(srctree)/$(src)
endif
include $(DRIVER_DIR)/Makefile.common

# build options for STMicroelectronics SPEAr1340 platform.

ifeq ($(CONFIG_CPU_SPEAR1340),y)
ARCH ?= arm
USING_MMU ?= 1
USING_UMP ?= 1
CONFIG = SPEAr1340
#MALI_FLEXIBLE_ALLOC = 1
$(shell [ -L $(DRIVER_DIR)/arch ] && rm $(DRIVER_DIR)/arch)
$(shell ln -sf $(DRIVER_DIR)/arch-$(CONFIG) $(DRIVER_DIR)/arch)
$(shell touch $(DRIVER_DIR)/arch/config.h)
endif

ifeq ($(CONFIG_GPU_MALI_DEBUG),y)
BUILD ?= debug
else
BUILD ?= release
endif

CONFIG ?= SPEAr1340
CPU = $(CONFIG)
KDIR-$(CPU)=$(srctree)

# For each arch check: CROSS_COMPILE , KDIR , CFLAGS += -DARCH

ARCH ?= arm
## @note Should allow overriding of building UMP for non-debug:
EXTRA_CFLAGS += -DDEBUG -DMALI_STATE_TRACKING=0

# linux build system integration

ifneq ($(KERNELRELEASE),)
# Inside the kernel build system

EXTRA_CFLAGS += -I$(DRIVER_DIR) -I$(DRIVER_DIR)/common -I$(DRIVER_DIR)/linux -I$(DRIVER_DIR)/../mali/common -I$(DRIVER_DIR)/../mali/linux -Iinclude/ump

# For customer releases the Linux Device Drivers will be provided as ARM proprietary and GPL releases:
# The ARM proprietary product will only include the license/proprietary directory
# The GPL product will only include the license/gpl directory

ifeq ($(wildcard $(DRIVER_DIR)/linux/license/gpl/*),)
EXTRA_CFLAGS += -I$(DRIVER_DIR)/linux/license/proprietary
else
EXTRA_CFLAGS += -I$(DRIVER_DIR)/linux/license/gpl
endif

SRC +=	$(UMP_FILE_PREFIX)linux/ump_kernel_linux.c \
	$(UMP_FILE_PREFIX)linux/ump_kernel_memory_backend_os.c \
	$(UMP_FILE_PREFIX)linux/ump_kernel_memory_backend_dedicated.c \
	$(UMP_FILE_PREFIX)linux/ump_memory_backend.c \
	$(UMP_FILE_PREFIX)linux/ump_ukk_wrappers.c \
	$(UMP_FILE_PREFIX)linux/ump_ukk_ref_wrappers.c \
	$(UMP_FILE_PREFIX)linux/ump_osk_atomics.c \
	$(UMP_FILE_PREFIX)linux/ump_osk_low_level_mem.c \
	$(UMP_FILE_PREFIX)linux/ump_osk_misc.c \
	$(UDD_FILE_PREFIX)linux/mali_osk_atomics.c \
	$(UDD_FILE_PREFIX)linux/mali_osk_locks.c \
	$(UDD_FILE_PREFIX)linux/mali_osk_memory.c \
	$(UDD_FILE_PREFIX)linux/mali_osk_math.c \
	$(UDD_FILE_PREFIX)linux/mali_osk_misc.c

# Selecting files to compile by parsing the config file

MODULE:=ump.ko

obj-$(CONFIG_GPU_MALI_UMP) := $(MODULE:.ko=.o)
$(MODULE:.ko=-y) := $(SRC:.c=.o)

else
# Outside the kernel build system

# Get any user defined KDIR-<names> or maybe even a hardcoded KDIR
-include KDIR_CONFIGURATION

# Define host system directory
KDIR-$(shell uname -m):=/lib/modules/$(shell uname -r)/build

ifeq ($(ARCH), arm)
	# when compiling for ARM we're cross compiling
	export CROSS_COMPILE ?= arm-none-linux-gnueabi-
	# default to Virtex5
	CONFIG ?= pb-virtex5
else
	# Compiling for the host
	CONFIG ?= $(shell uname -m)
endif

# default cpu to select
CPU ?= ct11mp

# look up KDIR based om CPU selection
KDIR ?= $(KDIR-$(CPU))

ifeq ($(KDIR),)
$(error No KDIR found for platform $(CPU))
endif

# Validate selected config
ifneq ($(shell [ -d $(DRIVER_DIR)/arch-$(CONFIG) ] && [ -f $(DRIVER_DIR)/arch-$(CONFIG)/config.h ] && echo "OK"), OK)
$(warning Current directory is $(shell pwd))
$(error No configuration found for config $(CONFIG). Check that $(DRIVER_DIR)/arch-$(CONFIG)/config.h exists)
else
# Link arch to the selected arch-config directory
$(shell [ -L $(DRIVER_DIR)/arch ] && rm $(DRIVER_DIR)/arch)
endif

all:
	$(MAKE) ARCH=$(ARCH) -C $(KDIR) M=$(CURDIR) modules

kernelrelease:
	$(MAKE) -C $(KDIR) kernelrelease

clean:
	$(MAKE) ARCH=$(ARCH) -C $(KDIR) M=$(CURDIR) clean
	$(MAKE) ARCH=$(ARCH) -C $(KDIR) M=$(CURDIR)/../mali clean

endif
