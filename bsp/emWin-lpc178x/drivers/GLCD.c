/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               GLCD.c
** Descriptions:            LCD driver
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2012-8-16
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include "GLCD.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_pinsel.h"
#include "Asciilib.h"
/*******************************************************************************
* Function Name  : LCD_ClockDivide
* Description    : find closest clock divider to get the desired clock rate.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static uint32_t LCD_ClockDivide(uint32_t LCD_clock) 
{	
	uint32_t divide, result;
	
	divide = 1;
	
	while ( ( (SystemCoreClock / divide) > LCD_clock ) && (divide <= 0x3F) ) 
	{
		divide++;
	}
	if (divide <= 1) 
	{
		result = (1 << BCD_BIT);   /* Skip divider logic if clock divider is 1 */
	} 
	else 
	{
		result = 0 | (((divide >> 0) & 0x1F) | (((divide >> 5) & 0x1F) << 27));
	}
	return result;
}

/*******************************************************************************
* Function Name  : GLCD_Configuration
* Description    : Configures LCD
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void GLCD_Config (void)
{
	uint32_t i;
							
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

    GPIO_SetDir(2, (1<<1), 1);     /* Back light enable */
 	GPIO_SetValue(2, (1<<1) );  

	/* Turn on LCD clock */
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCLCD, ENABLE);
	
	/* Disable cursor */
	LPC_LCD->CRSR_CTRL &=~(1<<0);
	
	/* disable GLCD controller */	
	LPC_LCD->CTRL = 0;
    
    /* Set the display mode */
	LPC_LCD->CTRL &= ~(0x07 << 1);  /* LcdBpp clear 000 */

    /* Selects the number of bits per LCD pixel */
    /* 000 = 1 bpp. 001 = 2 bpp. 010 = 4 bpp. 011 = 8 bpp. 100 = 16 bpp. */
	/* 101 = 24 bpp (TFT panel only). 110 = 16 bpp, 5:6:5 mode. 111 = 12 bpp, 4:4:4 mode.*/
	LPC_LCD->CTRL |= (BPP << 1);	

    /* LCD panel TFT type selection */
    /* 0 = LCD is an STN display. Use gray scaler */
    /* 1 = LCD is a TFT display. Do not use gray scaler */
	LPC_LCD->CTRL |= (LCDTFT << 5);

	/* Color format selection */
    /* 0 = RGB: normal output */
    /* 1 = BGR: red and blue swapped */
    LPC_LCD->CTRL |= (BGR << 8);

	/* init pixel clock */
	LPC_SC->LCD_CFG = 0;

	/* panel clock divisor PCD_LO and PCD_HI setting */
	LPC_LCD->POL |= (LCD_ClockDivide(OPT_CLK)<<0);

	/* AC bias pin frequency */
	LPC_LCD->POL |= ( (ACB - 1)<<6 );

	/* Invert vertical synchronization */
    /* The IVS bit inverts the polarity of the LCD_FP signal */
    /* 0 = LCD_FP pin is active HIGH and inactive LOW */
    /* 1 = LCD_FP pin is active LOW and inactive HIGH */
	LPC_LCD->POL |= (IVS<<11);

    /* Invert horizontal synchronization */
    /* The IHS bit inverts the polarity of the LCD_LP signal */
    /* 0 = LCD_LP pin is active HIGH and inactive LOW */
    /* 1 = LCD_LP pin is active LOW and inactive HIGH */
	LPC_LCD->POL |= (IHS<<12);

    /* Invert panel clock */
    /* The IPC bit selects the edge of the panel clock on which pixel data 
       is driven out onto the LCD data lines */
    /* 0 = Data is driven on the LCD data lines on the rising edge of LCD_DCLK */
    /* 1 = Data is driven on the LCD data lines on the falling edge of LCD_DCLK */
	LPC_LCD->POL |= (IPC<<13);

	/* Clocks per line */
	LPC_LCD->POL |= (CPL-1)<<16;

	/* init Horizontal Timing */
	LPC_LCD->TIMH = 0;   /* reset TIMH before set value */

	/* Pixels-per-line */
 	LPC_LCD->TIMH |= ((PPL/16) - 1)<<2;

	/* Horizontal synchronization pulse width */
    /* The 8-bit HSW field specifies the pulse width of the line clock in passive mode, or the
       horizontal synchronization pulse in active mode. Program with desired value minus 1 */
	LPC_LCD->TIMH |= (HSW - 1)<<8;

	/* Horizontal front porch */
	LPC_LCD->TIMH |= (HFP - 1)<<16;
    
	/* Horizontal back porch */
	LPC_LCD->TIMH |= (HBP - 1)<<24;

	/* init Vertical Timing */
	LPC_LCD->TIMV = 0;   /* reset TIMV value before setting */
 
	/* Lines per panel */
 	LPC_LCD->TIMV |= (LPP - 1)<<0;

	/* Vertical synchronization pulse width */
	LPC_LCD->TIMV |= (VSW - 1)<<10;

	/* Vertical front porch */
	LPC_LCD->TIMV |= (VFP)<<16;

	/* Vertical back porch */
	LPC_LCD->TIMV |= (VBP)<<24;

	/* Frame Base Address doubleword aligned */
	LPC_LCD->UPBASE = LCD_VRAM_BASE_ADDR & ~7UL ;
	LPC_LCD->LPBASE = LCD_VRAM_BASE_ADDR & ~7UL ;

    /* init colour pallet */
    for (i = 0; i < ( sizeof(LPC_LCD->PAL) / sizeof(LPC_LCD->PAL[0]) ); i++) 
    {
        LPC_LCD->PAL[i] = 0;  /* Clear the color palette with black */
    }
}

/*******************************************************************************
* Function Name  : GLCD_Ctrl
* Description    : 1 enable LCD, 0 disable LCD
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void GLCD_Ctrl (int bEna)
{
  volatile uint32_t i;

  if (bEna)
  {
    LPC_LCD->CTRL |= (1<<0);	/* LCD enable control bit */
    for(i = GLCD_PWR_ENA_DIS_DLY; i; i--);
    LPC_LCD->CTRL |= (1<<11);	/* enable power */
  }
  else
  {
    LPC_LCD->CTRL &= ~(1<<11);	/* disable power */
    for(i = GLCD_PWR_ENA_DIS_DLY; i; i--);
    LPC_LCD->CTRL &= ~(1<<0);	/* LCD disable control bit */
  }
}

/*******************************************************************************
* Function Name  : GLCD_Init
* Description    : Initialize TFT
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GLCD_Init (void)
{
    GLCD_Ctrl (0);
    GLCD_Config();
    GLCD_Ctrl (1);
}

/*******************************************************************************
* Function Name  : GLCD_Cursor_Config
* Description    : Configure the cursor
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GLCD_Cursor_Config(int config)
{
	LPC_LCD->CRSR_CFG = config;
}

/*******************************************************************************
* Function Name  : GLCD_Cursor_Enable
* Description    : Enable Cursor
* Input          : - cursor: Cursor Number 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GLCD_Cursor_Enable(int cursor)
{
	LPC_LCD->CRSR_CTRL |= (cursor<<4);
	LPC_LCD->CRSR_CTRL |= 1;
}

/*******************************************************************************
* Function Name  : GLCD_Cursor_Disable
* Description    : Disable Cursor
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GLCD_Cursor_Disable(void)
{
	LPC_LCD->CRSR_CTRL &= (1<<0);
}

/*******************************************************************************
* Function Name  : GLCD_Move_Cursor
* Description    : Moves cursor on position (x,y). Negativ values are posible
* Input          : - x: cursor x position
                   - y: cursor y position
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GLCD_Move_Cursor(int x, int y)
{
	LPC_LCD->CRSR_CLIP = 0;
	LPC_LCD->CRSR_XY = 0;

	if(x >= 0)
	{  
		LPC_LCD->CRSR_XY |= (x & 0x3FF);  /* no clipping */
	}
	else
	{
		LPC_LCD->CRSR_CLIP |= -x;		  /* clip x */
	}

	if(y >= 0)
	{
		LPC_LCD->CRSR_XY |= (y << 16);	  /* no clipping */
	}
	else
	{
		LPC_LCD->CRSR_CLIP |= (-y << 8);  /* clip y */
	}
}

/*******************************************************************************
* Function Name  : GLCD_Copy_Cursor
* Description    : Copy Cursor from const image to LCD RAM image
* Input          : - pCursor: pointer to cursor conts image
                   - cursor: cursor Number (0,1,2 or 3) for 64x64(size 256) 
				             pix cursor always use 0
				   - size: cursor size in words
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GLCD_Copy_Cursor (const uint32_t *pCursor, int cursor, int size)
{
   	uint32_t i ;
   	uint32_t * dest_address = (uint32_t *)LCD_CURSOR_BASE_ADDR;
   
   	dest_address += cursor * 64;

   	for(i = 0; i < size ; i++) 
	{
		*dest_address = *pCursor;
		dest_address++;
		pCursor++;
	}
}

/******************************************************************************
* Function Name  : GLCD_SetPixel_16bpp
* Description    : Set Point
* Input          : - Xpos: 
*                  - Ypos: 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
/* if declare to __inline will more faster */
rt_inline void GLCD_SetPixel_16bpp (uint16_t Xpos, uint16_t Ypos, uint16_t color)
{
    volatile uint16_t *pLCDbuf = (uint16_t *)LCD_VRAM_BASE_ADDR;  /* LCD buffer start address */

	pLCDbuf[ Ypos * GLCD_X_SIZE + Xpos ] = color;	
}

/*******************************************************************************
* Function Name  : GLCD_Clear
* Description    : Clear screen
* Input          : - color: screen color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GLCD_Clear(uint16_t color)
{
	uint16_t x_pos, y_pos;

    for( y_pos = 0; y_pos < GLCD_Y_SIZE; y_pos++ )
    {
		for( x_pos = 0; x_pos < GLCD_X_SIZE; x_pos++ )
		{
            GLCD_SetPixel_16bpp(x_pos, y_pos, color);
		}
    }
}


/******************************************************************************
* Function Name  : PutChar
* Description    : 将Lcd屏上任意位置显示一个字符
* Input          : - Xpos: 水平坐标 
*                  - Ypos: 垂直坐标  
*				   - ASCI: 显示的字符
*				   - charColor: 字符颜色   
*				   - bkColor: 背景颜色 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);  /* 取字模数据 */
    for( i=0; i<16; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( (tmp_char >> 7 - j) & 0x01 == 0x01 )
            {
                GLCD_SetPixel_16bpp( Xpos + j, Ypos + i, charColor );  /* 字符颜色 */
            }
            else
            {
                GLCD_SetPixel_16bpp( Xpos + j, Ypos + i, bkColor );  /* 背景颜色 */
            }
        }
    }
}

/******************************************************************************
* Function Name  : GUI_Text
* Description    : 在指定座标显示字符串
* Input          : - Xpos: 行座标
*                  - Ypos: 列座标 
*				   - str: 字符串
*				   - charColor: 字符颜色   
*				   - bkColor: 背景颜色 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
    do
    {
        TempChar = *str++;  
        PutChar( Xpos, Ypos, TempChar, Color, bkColor );    
        if( Xpos < GLCD_X_SIZE - 8 )
        {
            Xpos += 8;
        } 
        else if ( Ypos < GLCD_Y_SIZE - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }   
        else
        {
            Xpos = 0;
            Ypos = 0;
        }    
    }
    while ( *str != 0 );
}

/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Bresenham's line algorithm
* Input          : - x1: 
*                  - y1: 
*				   - x2: 
*				   - y2: 
*				   - color:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void GLCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
    short dx,dy;
    short temp;

    if( x0 > x1 )
    {
	    temp = x1;
		x1 = x0;
		x0 = temp;   
    }
    if( y0 > y1 )
    {
		temp = y1;
		y1 = y0;
		y0 = temp;   
    }
  
	dx = x1-x0;
	dy = y1-y0;  

    if( dx == 0 )
    {
        do
        { 
            GLCD_SetPixel_16bpp(x0, y0, color);
            y0++;
        }
        while( y1 >= y0 ); 
		return; 
    }
    if( dy == 0 )
    {
        do
        {
            GLCD_SetPixel_16bpp(x0, y0, color);
            x0++;
        }
        while( x1 >= x0 ); 
		return;
    }

    if( dx > dy ) 
    {
	    temp = 2 * dy - dx;   
        while( x0 != x1 )
        {
	        GLCD_SetPixel_16bpp(x0,y0,color);
	        x0++;
	        if( temp > 0 )           
	        {
	            y0++;                
	            temp += 2 * dy - 2 * dx; 
	 	    }
            else         
            {
			    temp += 2 * dy;   
			}       
        }
        GLCD_SetPixel_16bpp(x0,y0,color);
    }  
    else
    {
	    temp = 2 * dx - dy;              
        while( y0 != y1 )
        {
	 	    GLCD_SetPixel_16bpp(x0,y0,color);     
            y0++;                 
            if( temp > 0 )           
            {
                x0++;               
                temp+=2*dy-2*dx; 
            }
            else
			{
                temp += 2 * dy;
			}
        } 
        GLCD_SetPixel_16bpp(x0,y0,color);
	}
} 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
