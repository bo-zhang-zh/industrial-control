/**********************************************************************
* $Id$		sdram_HY57V281620_X2.c			2011-06-02
*//**
* @file		sdram_HY57V281620_X2.c
* @brief	Contains all functions support for SAMSUNG K4S561632J
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
#include "lpc177x_8x_emc.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_timer.h"
#include "sdram_HY57V281620_X2.h"

/*********************************************************************//**
 * @brief 		Initialize external SDRAM memory Micron MT48LC8M32LFB5
 *				256Mbit(8M x 32)
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SDRAMInit( void )
{
	volatile uint32_t i;
	volatile unsigned long Dummy;

//	PINSEL_ConfigPin(2,14,1);	   /* P2.14 - /EMC_CS2 */
//	PINSEL_ConfigPin(2,15,1);	   /* P2.15 - /EMC_CS3 */
	PINSEL_ConfigPin(2,16,1);	   /* P2.16 - /EMC_CAS */
	PINSEL_ConfigPin(2,17,1);	   /* P2.17 - /EMC_RAS */
	PINSEL_ConfigPin(2,18,1);	   /* P2.18 - EMC_CLK[0] */
//	PINSEL_ConfigPin(2,19,1);	   /* P2.19 - EMC_CLK[1] */
	PINSEL_ConfigPin(2,20,1);	   /* P2.20 - EMC_DYCS0 */
//	PINSEL_ConfigPin(2,21,1);	   /* P2.21 - EMC_DYCS1 */
//  PINSEL_ConfigPin(2,22,1);	   /* P2.22 - EMC_DYCS2 */
//	PINSEL_ConfigPin(2,23,1);	   /* P2.23 - EMC_DYCS3 */
	PINSEL_ConfigPin(2,24,1);	   /* P2.24 - EMC_CKE0 */
//	PINSEL_ConfigPin(2,25,1);	   /* P2.25 - EMC_CKE1 */
//	PINSEL_ConfigPin(2,26,1);	   /* P2.26 - EMC_CKE2 */
//	PINSEL_ConfigPin(2,27,1);	   /* P2.27 - EMC_CKE3 */
	PINSEL_ConfigPin(2,28,1);	   /* P2.28 - EMC_DQM0 */
	PINSEL_ConfigPin(2,29,1);	   /* P2.29 - EMC_DQM1 */
//	PINSEL_ConfigPin(2,30,1);	   /* P2.30 - EMC_DQM2 */
//	PINSEL_ConfigPin(2,31,1);	   /* P2.31 - EMC_DQM3 */

//	PINSEL_ConfigPin(4,24,1);	   /* P4.24 - /EMC_OE */
	PINSEL_ConfigPin(4,25,1);	   /* P4.25 - /EMC_WE */
//	PINSEL_ConfigPin(4,30,1);	   /* P4.30 - /EMC_CS0 */
//	PINSEL_ConfigPin(4,31,1);	   /* P4.31 - /EMC_CS1 */

	for(i = 0; i < 16; i++)
	{
		PINSEL_ConfigPin(3,i,1);   /* P3.0-P3.15 - EMC_D[0-15] */
	}

	for(i = 0; i < 15; i++)
	{
		PINSEL_ConfigPin(4,i,1);   /* P4.0-P4.14 - EMC_A[0-14] */
	}

	//	EMC_Init();
	// Init SDRAM controller
	LPC_SC->PCONP   	|= 0x00000800;
	/*Init SDRAM controller*/
	LPC_SC->EMCDLYCTL |= (8<<0);
	/*Set data read delay*/
	LPC_SC->EMCDLYCTL |=(8<<8);
	LPC_SC->EMCDLYCTL |= (0x08 <<16);

	LPC_EMC->Control =1;
	LPC_EMC->DynamicReadConfig = 1;
	LPC_EMC->DynamicRasCas0 = 0;
	LPC_EMC->DynamicRasCas0 |=(3<<8);
	LPC_EMC->DynamicRasCas0 |= (3<<0);
	LPC_EMC->DynamicRP = P2C(SDRAM_TRP);
	LPC_EMC->DynamicRAS = P2C(SDRAM_TRAS);
	LPC_EMC->DynamicSREX = P2C(SDRAM_TXSR);
	LPC_EMC->DynamicAPR = SDRAM_TAPR;
	LPC_EMC->DynamicDAL = SDRAM_TDAL+P2C(SDRAM_TRP);
	LPC_EMC->DynamicWR = SDRAM_TWR;
	LPC_EMC->DynamicRC = P2C(SDRAM_TRC);
	LPC_EMC->DynamicRFC = P2C(SDRAM_TRFC);
	LPC_EMC->DynamicXSR = P2C(SDRAM_TXSR);
	LPC_EMC->DynamicRRD = P2C(SDRAM_TRRD);
	LPC_EMC->DynamicMRD = SDRAM_TMRD;

	// SDRAM
	LPC_EMC->DynamicConfig0 = 0x0000680;
	// JEDEC General SDRAM Initialization Sequence
	// DELAY to allow power and clocks to stabilize ~100 us
	// NOP
	LPC_EMC->DynamicControl = 0x0183;
	for(i= 200*30; i;i--);
	// PALL
	LPC_EMC->DynamicControl = 0x0103;
	LPC_EMC->DynamicRefresh = 2;
	for(i= 256; i; --i); // > 128 clk
	LPC_EMC->DynamicRefresh = P2C(SDRAM_REFRESH) >> 4;
	// COMM

    LPC_EMC->DynamicControl    = 0x00000083; /* Issue MODE command */
	Dummy = *((volatile uint32_t *)(SDRAM_BASE_ADDR | (0x33<<12)));
	// NORM
	LPC_EMC->DynamicControl = 0x0000;
	LPC_EMC->DynamicConfig0 |=(1<<19);
	for(i = 100000; i;i--);
}
/*********************************************************************************
**                            End Of File
*********************************************************************************/
