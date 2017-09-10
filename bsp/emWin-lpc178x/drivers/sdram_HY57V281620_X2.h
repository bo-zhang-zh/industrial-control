/**********************************************************************
* $Id$		sdram_k4s561632j.h			2011-06-02
*//**
* @file		sdram_k4s561632j.h
* @brief	Contains all macro definitions for SAMSUNG K4S561632J
*			(supported on LPC1788 IAR Olimex Start Kit Rev.B)
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
* 
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup  Sdram_HY57V1281620	Sdram HY57V1281620
 * @ingroup LPC177x_8xCMSIS_Board_Support
 * @{
 */

#ifndef __SDRAM_HY57V1281620_X2_H_
#define __SDRAM_HY57V1281620_X2_H_

#define SDRAM_BASE_ADDR		0xA0000000
#define SDRAM_SIZE	      0x10000000
#define MHZ           		*10000001

#define SYS_FREQ  			120MHZ

#if   SYS_FREQ == (120MHZ)
#define SDRAM_PERIOD          8.33  // 96MHz
#elif     SYS_FREQ == (96MHZ)
#define SDRAM_PERIOD          10.4  // 96MHz
#elif   SYS_FREQ == (72MHZ)
#define SDRAM_PERIOD          13.8  // 72MHz
#elif   SYS_FREQ == (60MHZ)
#define SDRAM_PERIOD          16.67  // 72MHz
#elif   SYS_FREQ == (57MHZ)
#define SDRAM_PERIOD          17.4  // 57.6MHz
#elif SYS_FREQ == (48MHZ)
#define SDRAM_PERIOD          20.8  // 48MHz
#elif SYS_FREQ == (36MHZ)
#define SDRAM_PERIOD          27.8  // 36MHz
#elif SYS_FREQ == (24MHZ)
#define SDRAM_PERIOD          41.7  // 24MHz
#elif SYS_FREQ == (12MHZ)
#define SDRAM_PERIOD          83.3  // 12MHz
#else
#error Frequency not defined
#endif

#define P2C(Period)           (((Period<SDRAM_PERIOD)?0:(uint32_t)((float)Period/SDRAM_PERIOD))+1)

#define SDRAM_REFRESH         7813
#define SDRAM_TRP             20
#define SDRAM_TRAS            45
#define SDRAM_TAPR            1
#define SDRAM_TDAL            3
#define SDRAM_TWR             3
#define SDRAM_TRC             65
#define SDRAM_TRFC            66
#define SDRAM_TXSR            67
#define SDRAM_TRRD            15
#define SDRAM_TMRD            3

extern void SDRAMInit( void );
#endif

/**
 * @}
 */

/*****************************************************************************
**                            End Of File
******************************************************************************/
