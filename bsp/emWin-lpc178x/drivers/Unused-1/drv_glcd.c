/*
*******************************************************************************
  Copyright (C), 2012-2014, Embest Tech. Co., Ltd.
  FileName		    : drv_glcd.c
  Author          : lichy       
  Version 		    : 1.0.0     
  Date			      : 2012/02/29
  Description	    : LCD Drive
  Function List   : 
  History		      :
  <author>  	    : lichy       
  <time>     	    : 2012/02/29
  <version >      : 1.0.0 	
  <desc>		      : build this moudle			 
*******************************************************************************
*/

//#include <stdio.h>
//#include <stdlib.h>

//#include "drv_glcd.h"
#include "GLCD.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_pinsel.h"
#include "sdram_HY57V281620_X2.h"
#include "Cursor.h"
       
/*
***************************************************************************
*                        macro declare
***************************************************************************
*/
//Matrix Arbitration register
#define MATRIX_ARB  (*(volatile uint32_t*)(0x400FC188))

/*
***************************************************************************
*                        local varible declare
***************************************************************************
*/


/*																																					 
******************************************************************************
  Function:      GLCD_Cursor_Cfg
  Description:   Configure the cursor
  Calls:           
  Called By:     
  Input:         cfg
  Output:        void
  Return:        none
  Others:         
******************************************************************************
*/
void GLCD_Cursor_Cfg(int Cfg)
{
  LPC_LCD->CRSR_CFG = Cfg;
}

/*																																					 
******************************************************************************
  Function:      GLCD_Cursor_En
  Description:   Enable Cursor
  Calls:           
  Called By:     
  Input:         cursor - Cursor Number
  Output:        void
  Return:        none
  Others:         
******************************************************************************
*/
void GLCD_Cursor_En(int cursor)
{
  LPC_LCD->CRSR_CTRL |= (cursor<<4);
  LPC_LCD->CRSR_CTRL |= 1;
}


/*																																					 
******************************************************************************
  Function:      GLCD_Cursor_Dis
  Description:   Disable Cursor
  Calls:           
  Called By:     
  Input:         cursor
  Output:        void
  Return:        none
  Others:         
******************************************************************************
*/
void GLCD_Cursor_Dis(int cursor)
{
  LPC_LCD->CRSR_CTRL &= (1<<0);
}

/*																																					 
******************************************************************************
  Function:      GLCD_Move_Cursor
  Description:   Moves cursor on position (x,y). Negativ values are posible.
  Calls:           
  Called By:     
  Input:         x - cursor x position
                 y - cursor y position
  Output:        void
  Return:        none
  Others:         
******************************************************************************
*/
void GLCD_Move_Cursor(int x, int y)
{
  LPC_LCD->CRSR_CLIP = 0;
  LPC_LCD->CRSR_XY = 0;

  if(0 <= x)
  {//no clipping
    LPC_LCD->CRSR_XY |= (x & 0x3FF);
  }
  else
  {//clip x
    LPC_LCD->CRSR_CLIP |= -x;
  }

  if(0 <= y)
  {//no clipping

    LPC_LCD->CRSR_XY |= (y << 16);
  }
  else
  {//clip y
    LPC_LCD->CRSR_CLIP |= (-y << 8);
  }
}

/*
******************************************************************************
  Function:      GLCD_Copy_Cursor
  Description:   Copy Cursor from const image to LCD RAM image
  Calls:           
  Called By:     
  Input:         pCursor - pointer to cursor conts image
                 cursor - cursor Number (0,1,2 or 3)
                         for 64x64(size 256) pix cursor always use 0
                 size - cursor size in words
  Output:        void
  Return:        none
  Others:         
******************************************************************************
*/
void GLCD_Copy_Cursor (const uint32_t *pCursor, int cursor, int size)
{
  uint32_t i ;
  uint32_t * pDst = (uint32_t *)LCD_CURSOR_BASE_ADDR;
  
	pDst += cursor*64;
	for(i = 0; i < size ; i++) {
	  *pDst = *pCursor;
	  pDst++;
	  pCursor++;
	}
}

/*
******************************************************************************
  Function:      GLCD_Init
  Description:   GLCD controller init
  Calls:           
  Called By:     main()
  Input:         const uint32_t *pPain, const uint32_t * pPallete
  Output:        void
  Return:        none
  Others:         
******************************************************************************
*/
void GLCD_Init (const uint32_t *pPain, const uint32_t * pPallete)
{
	uint32_t i;
	uint32_t *pDst = (uint32_t *)LCD_VRAM_BASE_ADDR;

	PINSEL_ConfigPin(2,12,5);	   /* P2.12 - R0_VD3 */
	PINSEL_ConfigPin(2,6,7);	   /* P2.6 - R1_VD4 */
	PINSEL_ConfigPin(2,7,7);	   /* P2.7 - R2_VD5 */
	PINSEL_ConfigPin(2,8,7);	   /* P2.8 - R3_VD6 */
	PINSEL_ConfigPin(2,9,7);	   /* P2.9 - R4_VD7 */

	PINSEL_ConfigPin(1,20,7);	   /* P1.20 - G0_VD10 */
	PINSEL_ConfigPin(1,21,7);	   /* P1.21 - G1_VD11 */
	PINSEL_ConfigPin(1,22,7);	   /* P1.22 - G2_VD12 */
	PINSEL_ConfigPin(1,23,7);	   /* P1.23 - G3_VD13 */
	PINSEL_ConfigPin(1,24,7);	   /* P1.24 - G4_VD14 */
	PINSEL_ConfigPin(1,25,7);	   /* P1.25 - G5_VD15 */

	PINSEL_ConfigPin(2,13,7);	   /* P2.13 - B0_VD19 */
	PINSEL_ConfigPin(1,26,7);	   /* P1.26 - B1_VD20 */
	PINSEL_ConfigPin(1,27,7);	   /* P1.27 - B2_VD21 */
	PINSEL_ConfigPin(1,28,7);	   /* P1.28 - B3_VD22 */
	PINSEL_ConfigPin(1,29,7);	   /* P1.29 - B4_VD23 */

	PINSEL_ConfigPin(2,2,7);	   /* P2.2 - LCD_DCLK */
	PINSEL_ConfigPin(2,0,7);	   /* P2.0 - LCD_DISP */
	PINSEL_ConfigPin(2,5,7);	   /* P2.5 - LCD_HSYNC */
	PINSEL_ConfigPin(2,3,7);	   /* P2.3 - LCD_VSYNC */
	PINSEL_ConfigPin(2,4,7);	   /* P2.4 - LCD_DEN */


	/*Turn on LCD clock*/
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCLCD, ENABLE);
	
	/* Disable cursor*/
	LPC_LCD->CRSR_CTRL &=~(1UL<<0);
	
	/* disable GLCD controller */
	LPC_LCD->CTRL = 0;

  LPC_SC->LCD_CFG  = CLKPWR_GetCLK(CLKPWR_CLKTYPE_PER) / ((uint32_t)C_GLCD_PIX_CLK);

  /* Configure operating mode and panel parameters */
	LPC_LCD->CTRL    = 0            
	                 | (BPP    << 1)
	                 | (BGR    << 8)
	                 | (LCDTFT << 5)
									 ;
	
	/* Configure horizontal axis */
  LPC_LCD->TIMH    = 0            
                     | (((C_GLCD_H_SIZE / 16) - 1) <<  2)
                     | ((HSW - 1)        <<  8)
                     | ((HFP - 1)        << 16)
                     | ((HBP - 1)        << 24)
                     ;

  /* Configure vertical axis */
  LPC_LCD->TIMV    = 0             
                     | ((C_GLCD_V_SIZE - 1) <<  0)
                     | ((VSW - 1) << 10)
                     | ((VFP)     << 16)
                     | ((VBP)     << 24)
                     ;

  /* Configure clock and signal polarity */
  LPC_LCD->POL     = 0
                     | ((IVS)                      << 11)
                     | ((IHS)                      << 12)
                     | ((IPC)                      << 13)
                     | ((CPL - 1)                  << 16)
                     | ((BCD)                      << 26)
										 ;

	/* 
	Set VRAM addr 
	Frame Base Address doubleword aligned
	*/
	LPC_LCD->UPBASE = LCD_VRAM_BASE_ADDR & ~7UL ;
	LPC_LCD->LPBASE = LCD_VRAM_BASE_ADDR & ~7UL ;
  
	/* init colour pallet*/
	if(NULL != pPallete) {
		GLCD_SetPallet(pPallete);
	}
	
	/* clear display memory */
  for(i = 0; (C_GLCD_H_SIZE * C_GLCD_V_SIZE / 2) > i; i++) {
    *pDst++ = 0x00;
  }

  for(i = C_GLCD_ENA_DIS_DLY; i; i--);

	GLCD_Ctrl (TRUE);
}

/*
******************************************************************************
  Function:      GLCD_SetPallet
  Description:   GLCD init colour pallete
  Calls:           
  Called By:     
  Input:         const uint32_t * pPallete
  Output:        void
  Return:        none
  Others:         
******************************************************************************
*/
void GLCD_SetPallet (const uint32_t * pPallete)
{
	uint32_t i;
	uint32_t * pDst = (uint32_t *)LPC_LCD->PAL;

//	assert(pPallete);
	for (i = 0; i < 128; i++) {
	  *pDst++ = *pPallete++;
	}
}

/*
******************************************************************************
  Function:      GLCD_Ctrl
  Description:   GLCD enable disabe sequence
  Calls:           
  Called By:     
  Input:         Bool bEna
  Output:        void
  Return:        none
  Others:         
******************************************************************************
*/
void GLCD_Ctrl (Bool bEna)
{
	volatile uint32_t i;

  if (bEna) {
    LPC_LCD->CTRL |= (1<<0);
    for(i = C_GLCD_PWR_ENA_DIS_DLY; i; i--);
    // enable power
    LPC_LCD->CTRL |= (1<<11);
  } else {
    LPC_LCD->CTRL &= ~(1<<11);
    for(i = C_GLCD_PWR_ENA_DIS_DLY; i; i--);
    LPC_LCD->CTRL &= ~(1<<0);
  }
}

/*
******************************************************************************
  Function:      GLCD_Turn_On_LCD
  Description:   trun on the LCD,user can output PWM adjust the LCD light
  Calls:           
  Called By:     
  Input:         Bool bEna
  Output:        void
  Return:        none
  Others:         
******************************************************************************
*/
void GLCD_Turn_On_LCD (void)
{
  GPIO_OutputValue(LCD_PWM_PROT, LCD_PWM_MASK, 1);
}

/*
******************************************************************************
  Function:      GLCD_Turn_Off_LCD
  Description:   trun off the LCD
  Calls:           
  Called By:     
  Input:         Bool bEna
  Output:        void
  Return:        none
  Others:         
******************************************************************************
*/
void GLCD_Turn_Off_LCD (void)
{
  GPIO_OutputValue(LCD_PWM_PROT, LCD_PWM_MASK, 0);
}
