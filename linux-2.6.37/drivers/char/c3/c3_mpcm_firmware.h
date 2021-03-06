/*
 * Copyright (C) 2007-2010 STMicroelectronics Ltd
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * The full GNU General Public License is included in this distribution in
 * the file called "COPYING".
 */

/* --------------------------------------------------------------------
 * C3 driver MPCM firmware
 *
 * ST - 2007-08-01
 * Alessandro Miglietti
 * ----------------------------------------------------------------- */

#ifndef __C3_MPCM_FIRMWARE_H__
#define __C3_MPCM_FIRMWARE_H__

/* -------------------------------------------------------------------
 * MACROS & CONSTANTS: MPCPM firmware
 * ---------------------------------------------------------------- */

/* Based on the Channel uSequence Library v1.0 */
#define MPCM_AES_CBC_CTR_ECB_KEY_LOAD_CODE {		\
	0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0xbc, 0x00,	\
	0x00, 0x6c, 0x90, 0x07, 0xfd, 0x60, 0x27, 0x00,	\
	0x08, 0x00, 0x10, 0x07, 0x05, 0x70, 0xa2, 0x05,	\
	0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0xbc, 0x00,	\
	0x00, 0x6c, 0x90, 0x07, 0xfd, 0x60, 0x27, 0x00,	\
	0x10, 0x00, 0x10, 0x07, 0x05, 0x70, 0xa2, 0x02,	\
	0x18, 0x00, 0x10, 0x07, 0x05, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x00, 0x00	\
	}

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#define MPCM_AES_CBC_ECB_KEY_SCHEDULE_CODE {		\
	0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0xbc, 0x00,	\
	0x00, 0x6c, 0x90, 0x07, 0xfd, 0x60, 0x27, 0x00,	\
	0x08, 0x00, 0x10, 0x07, 0x05, 0x70, 0xa2, 0x05,	\
	0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0xbc, 0x00,	\
	0x00, 0x6c, 0x90, 0x07, 0xfd, 0x60, 0x27, 0x00,	\
	0x10, 0x00, 0x10, 0x07, 0x05, 0x70, 0xa2, 0x02,	\
	0x18, 0x00, 0x10, 0x07, 0x05, 0x60, 0xbc, 0x00,	\
	0x24, 0x00, 0x10, 0x00, 0x05, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0xbc, 0x00,	\
	0x00, 0x6c, 0x90, 0x07, 0xfd, 0x60, 0x7f, 0x00,	\
	0x0c, 0x00, 0x00, 0x00, 0x00, 0x60, 0xa2, 0x05,	\
	0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0xbc, 0x00,	\
	0x00, 0x6c, 0x90, 0x07, 0xfd, 0x60, 0xbc, 0x00,	\
	0x16, 0x00, 0x00, 0x00, 0x00, 0x60, 0xa2, 0x02,	\
	0x1e, 0x00, 0x00, 0x00, 0x00, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x00, 0x00	\
	}

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#define MPCM_AES_ECB_ENC_APPEND_CODE {			\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x27, 0x00,	\
	0x20, 0x00, 0x10, 0x07, 0x05, 0x70, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x47, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x21, 0x85,	\
	0x00, 0x00, 0x11, 0x78, 0x75, 0x70, 0xbc, 0x00,	\
	0x30, 0x00, 0x11, 0xf9, 0x05, 0x00, 0x93, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x47, 0x00,	\
	0x00, 0x00, 0x16, 0xf8, 0x1d, 0x68, 0x22, 0x7d,	\
	0x00, 0x00, 0x11, 0xf8, 0x05, 0x00, 0x93, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x4b, 0x00,	\
	0x00, 0x00, 0x16, 0xf8, 0x1d, 0x68, 0x00, 0x00	\
	}

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#define MPCM_AES_ECB_DEC_APPEND_CODE {			\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x27, 0x00,	\
	0x28, 0x00, 0x10, 0x07, 0x05, 0x70, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x47, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x21, 0x85,	\
	0x00, 0x00, 0x11, 0x78, 0x75, 0x70, 0xbc, 0x00,	\
	0x38, 0x00, 0x11, 0xf9, 0x05, 0x00, 0x93, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x47, 0x00,	\
	0x00, 0x00, 0x16, 0xf8, 0x1d, 0x68, 0x22, 0x7d,	\
	0x00, 0x00, 0x11, 0xf8, 0x05, 0x00, 0x93, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x4b, 0x00,	\
	0x00, 0x00, 0x16, 0xf8, 0x1d, 0x68, 0x00, 0x00	\
	}

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#define MPCM_AES_CBC_CTR_INIT_IVEC_CODE	{		\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x27, 0x00,	\
	0x00, 0x00, 0x10, 0xf8, 0x75, 0x70, 0x00, 0x00	\
	}

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#define MPCM_AES_CBC_ENC_APPEND_CODE {			\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x27, 0x00,	\
	0x20, 0x00, 0x10, 0x00, 0xf4, 0x70, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x47, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x21, 0x85,	\
	0x00, 0x00, 0x11, 0x78, 0x75, 0x70, 0xbc, 0x00,	\
	0x30, 0x00, 0x10, 0xf8, 0x10, 0x00, 0x93, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x47, 0x00,	\
	0x00, 0x00, 0x16, 0xf8, 0x0d, 0x68, 0x22, 0x7d,	\
	0x00, 0x00, 0x10, 0xf8, 0x05, 0x00, 0x93, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x4b, 0x00,	\
	0x00, 0x00, 0x16, 0xf8, 0x0d, 0x68, 0x00, 0x00	\
	}

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#define MPCM_AES_CBC_DEC_APPEND_CODE {			\
	0x00, 0x00, 0x11, 0x78, 0x0d, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x27, 0x00,	\
	0x28, 0x00, 0x10, 0x7f, 0x75, 0x70, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x47, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x21, 0x87,	\
	0x00, 0x00, 0x10, 0xf8, 0x05, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0x78, 0x75, 0x70, 0xbc, 0x00,	\
	0x38, 0x00, 0x11, 0xf8, 0x05, 0x00, 0x93, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x47, 0x00,	\
	0x00, 0x00, 0x16, 0xf9, 0x95, 0x88, 0x21, 0x82,	\
	0x00, 0x00, 0x11, 0x78, 0x0d, 0x60, 0xbe, 0xfb,	\
	0x00, 0x00, 0x11, 0x78, 0x05, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x11, 0xf8, 0x05, 0x00, 0x93, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x4b, 0x00,	\
	0x00, 0x00, 0x16, 0xf9, 0x8d, 0x88, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0xf8, 0x05, 0x60, 0x00, 0x00	\
	}

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#define MPCM_AES_CTR_APPEND_CODE {			\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x27, 0x00,	\
	0x20, 0x00, 0x11, 0x78, 0xf5, 0x70, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x47, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x21, 0x89,	\
	0x00, 0x00, 0x90, 0x00, 0x0d, 0x60, 0xbc, 0x00,	\
	0x00, 0x08, 0x90, 0x88, 0x0d, 0x40, 0xa3, 0x02,	\
	0x00, 0x00, 0x10, 0x90, 0x05, 0x40, 0xbc, 0x00,	\
	0x00, 0x00, 0x11, 0xf8, 0x15, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x11, 0x78, 0x75, 0x70, 0xbc, 0x00,	\
	0x30, 0x00, 0x12, 0x78, 0x85, 0x00, 0x93, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x47, 0x00,	\
	0x00, 0x00, 0x16, 0xfa, 0x1d, 0x88, 0x22, 0x79,	\
	0x00, 0x00, 0x11, 0xf8, 0x15, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x90, 0x00, 0x0d, 0x60, 0xbc, 0x00,	\
	0x00, 0x08, 0x90, 0x88, 0x0d, 0x40, 0xa3, 0x02,	\
	0x00, 0x00, 0x10, 0x90, 0x05, 0x40, 0xbc, 0x00,	\
	0x00, 0x00, 0x12, 0x78, 0x05, 0x00, 0x93, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x4b, 0x00,	\
	0x00, 0x00, 0x16, 0xfa, 0x1d, 0x88, 0x00, 0x00	\
	}

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#define MPCM_AES_XCBC_MAC_96_KEY_LOAD_CODE {		\
	0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0xbc, 0x00,	\
	0x00, 0x09, 0x90, 0x00, 0x7d, 0x60, 0xbc, 0x00,	\
	0x00, 0x09, 0x90, 0xf8, 0x7d, 0x40, 0x27, 0x00,	\
	0x08, 0x00, 0x12, 0xff, 0x75, 0x70, 0xbc, 0x00,	\
	0x20, 0x00, 0x10, 0x00, 0x85, 0x60, 0xbc, 0x00,	\
	0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0xbc, 0x00,	\
	0x00, 0x09, 0x93, 0xff, 0xfd, 0x80, 0xbc, 0x00,	\
	0x00, 0x00, 0x13, 0x78, 0x05, 0x40, 0xbc, 0x00,	\
	0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0xbc, 0x00,	\
	0x00, 0x09, 0x90, 0x00, 0x7d, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x13, 0x88, 0x05, 0x40, 0xbc, 0x00,	\
	0x00, 0x00, 0x12, 0x7a, 0x25, 0x80, 0x93, 0x00,	\
	0x08, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00	\
	}

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#define MPCM_AES_XCBC_MAC_96_APPEND_CODE {		\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x27, 0x00,	\
	0x20, 0x00, 0x10, 0x02, 0x74, 0x70, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x47, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x21, 0x85,	\
	0x00, 0x00, 0x11, 0x78, 0x75, 0x70, 0xbc, 0x00,	\
	0x30, 0x00, 0x12, 0x78, 0x10, 0x00, 0x93, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x47, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x22, 0x7d,	\
	0x00, 0x00, 0x12, 0x78, 0x05, 0x00, 0x93, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x00, 0x00	\
	}

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#define MPCM_AES_XCBC_MAC_96_END_CODE {			\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xbc, 0x00,	\
	0x00, 0x6c, 0x90, 0x07, 0xfd, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0xa2, 0x08,	\
	0x00, 0x6c, 0x90, 0x07, 0xfd, 0x60, 0x27, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x70, 0xa3, 0x06,	\
	0x00, 0x02, 0x02, 0x02, 0x02, 0x01, 0xbc, 0x00,	\
	0x08, 0x09, 0x90, 0x02, 0xfd, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0xf8, 0x05, 0x40, 0xbc, 0x00,	\
	0x20, 0x00, 0x13, 0xf8, 0x8d, 0x80, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0xbe, 0x8c,	\
	0x00, 0x03, 0x03, 0x03, 0x03, 0x01, 0xbc, 0x00,	\
	0x08, 0x09, 0x90, 0x02, 0xfd, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0xf8, 0x05, 0x40, 0xbc, 0x00,	\
	0x20, 0x00, 0x10, 0x00, 0x85, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x00, 0x00, 0x0f, 0x01, 0xbc, 0x00,	\
	0x00, 0x04, 0x9b, 0x7b, 0x05, 0x80, 0xbc, 0x00,	\
	0x00, 0x3d, 0x91, 0xff, 0xfd, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x11, 0x88, 0x05, 0x40, 0xa2, 0x04,	\
	0x00, 0x00, 0x1b, 0x7b, 0x05, 0x80, 0xbc, 0x00,	\
	0x00, 0x00, 0x1b, 0xfb, 0x85, 0x80, 0xbc, 0x00,	\
	0x00, 0x01, 0x10, 0x00, 0x1d, 0x60, 0xbe, 0xfd,	\
	0x00, 0x00, 0x41, 0x7f, 0x35, 0x80, 0xbc, 0x00,	\
	0x00, 0x00, 0x49, 0x79, 0x3d, 0x80, 0xbc, 0x00,	\
	0x00, 0x00, 0x11, 0x79, 0x25, 0x80, 0xbc, 0x00,	\
	0x08, 0x00, 0x10, 0x00, 0x05, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x93, 0x00,	\
	0x20, 0x00, 0x10, 0x00, 0x10, 0x60, 0xbc, 0x00,	\
	0x00, 0x00, 0x12, 0x78, 0x05, 0x00, 0x93, 0x00,	\
	0x00, 0x00, 0x10, 0x00, 0x05, 0x60, 0x4b, 0x00,	\
	0x00, 0x00, 0x16, 0xf8, 0x25, 0x6e, 0x00, 0x00	\
	}

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#endif /* __C3_MPCM_FIRMWARE_H__ */
