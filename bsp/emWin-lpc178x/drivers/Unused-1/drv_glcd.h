/*
*******************************************************************************
  Copyright (C), 2012-2014, Embest Tech. Co., Ltd.
  FileName		    : drv_glcd.h
  Author          : lichy       
  Version 		    : 1.0.0     
  Date			      : 2012/02/29
  Description	    : 
  Function List   : 
  History		      :
  <author>  	    : lichy       
  <time>     	    : 2012/02/29
  <version >      : 1.0.0 	
  <desc>		      : build this moudle			 
*******************************************************************************
*/
#include "lpc_types.h"
#include "sdram_HY57V281620_X2.h"

#ifndef __GLCD_DRV_H
#define __GLCD_DRV_H

/**
 * @brief A struct for Bitmap on LCD screen
 */
typedef struct _Bmp_t 
{
  uint32_t  H_Size;
  uint32_t  V_Size;
  uint32_t  BitsPP;
  uint32_t  BytesPP;
  uint32_t *pPalette;
  uint32_t *pPicStream;
  uint8_t *pPicDesc;
} Bmp_t, *pBmp_t;

/**
 * @brief A struct for Font Type on LCD screen
 */
typedef struct _FontType_t 
{
  uint32_t H_Size;
  uint32_t V_Size;
  uint32_t CharacterOffset;
  uint32_t CharactersNuber;
  uint8_t *pFontStream;
  uint8_t *pFontDesc;
} FontType_t, *pFontType_t;

typedef uint32_t LdcPixel_t, *pLdcPixel_t;

/*
***************************************************************************
*                         LCD Type Config
***************************************************************************
*/
//#define LCD_480272


/*
***************************************************************************
*                        macro declare
***************************************************************************
*/
#define C_GLCD_PWR_ENA_DIS_DLY  10000
#define C_GLCD_ENA_DIS_DLY      10000

#define CRSR_PIX_32             0
#define CRSR_PIX_64             1
#define CRSR_ASYNC              0
#define CRSR_FRAME_SYNC         2

#define TEXT_DEF_TAB_SIZE       5

#define TEXT_BEL1_FUNC()

/*
***************************************************************************
*                         LCD VRAM ADDR Define
***************************************************************************
*/
#define LCD_VRAM_BASE_ADDR 	  ((uint32_t)SDRAM_BASE_ADDR + 0x00200000)
#define LCD_CURSOR_BASE_ADDR 	((uint32_t)0x20088800)

/*
***************************************************************************
*                         LCDC configuration
***************************************************************************
*/

/*
***************************************************************************
*                         LCDC configuration
***************************************************************************
*/
#ifdef LCD_480272
#define C_GLCD_H_SIZE      480      // Pixels per line
#define HSW                41       // HSYNC pulse width
#define HFP                10        // Horizontal front porch
#define HBP                10        // Horizontal back porch

#define C_GLCD_V_SIZE      272      // Lines per panel
#define VSW                12       // VSYNC pulse width
#define VFP                2        // Vertical front porch
#define VBP                2        // Vertical back porch

#define IVS                1        // Invert VSYNC
#define IHS                1        // Invert HSYNC
#define IPC                1        // Invert panel clock
#define CPL                C_GLCD_H_SIZE // Clock per line (identical to pixels per line)
#define BGR                1        // Swap red and blue
#define LCDTFT             1        // LCD TFT panel
#define BCD                1        // Bypass pixel clock divider
#define C_GLCD_PIX_CLK    (6.4*1000000l)	 //Optimal clock rate
#endif /* end of the #ifdef LCD_480272 */

#if USE_24BPP_MODE
  #define BPP              5        // Bits per pixel b101 = 24 bpp (TFT panel only)
#else
  #define BPP              6        // Bits per pixel b110 = 16 bpp 5:6:5 mode
#endif

#ifdef LCD_800600
#define C_GLCD_H_SIZE      800      // Pixels per line
#define HSW                41       // HSYNC pulse width
#define HFP                2        // Horizontal front porch
#define HBP                25        // Horizontal back porch


#define C_GLCD_V_SIZE      480      // Lines per panel
#define VSW                3       // VSYNC pulse width
#define VFP                3        // Vertical front porch
#define VBP                3        // Vertical back porch

#define IVS                1        // Invert VSYNC
#define IHS                1        // Invert HSYNC
#define IPC                0        // Invert panel clock

#define CPL                C_GLCD_H_SIZE // Clock per line (identical to pixels per line)
#define BGR                1        // Swap red and blue
#define LCDTFT             1        // LCD TFT panel
#define BCD                1        // Bypass pixel clock divider
#define C_GLCD_PIX_CLK    (6.4*1000000l)	 //Optimal clock rate
#endif /* end of the #ifdef LCD_480272 */

#if USE_24BPP_MODE
  #define BPP              5        // Bits per pixel b101 = 24 bpp (TFT panel only)
#else
  #define BPP              6        // Bits per pixel b110 = 16 bpp 5:6:5 mode
#endif



/*
***************************************************************************
*                        PIN macro declare
***************************************************************************
*/
#define LCD_PWM_PROT		(1)
#define LCD_PWM_PIN			(3)
#define LCD_PWM_MASK		(1 << LCD_PWM_PIN)

/*
***************************************************************************
*                        Function declare
***************************************************************************
*/
void GLCD_Init (const uint32_t *pPain, const uint32_t * pPallete);
void GLCD_SetPallet (const uint32_t * pPallete);
void GLCD_Ctrl (Bool bEna);
void GLCD_Cursor_Cfg(int Cfg);
void GLCD_Cursor_En(int cursor);
void GLCD_Cursor_Dis(int cursor);
void GLCD_Move_Cursor(int x, int y);
void GLCD_Copy_Cursor (const uint32_t *pCursor, int cursor, int size);
void GLCD_Turn_On_LCD (void);
void GLCD_Turn_Off_LCD (void);


#endif // __GLCD_DRV_H
