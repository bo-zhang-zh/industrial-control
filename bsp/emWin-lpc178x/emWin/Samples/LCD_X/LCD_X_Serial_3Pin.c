/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2011  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.12 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCD_X_SERIAL_3PIN.c
Purpose     : Port routines for 3 pin serial interface
----------------------------------------------------------------------
*/

#ifndef LCD_DELAY
  #define LCD_DELAY(x)
#endif

#ifndef LCD_SET_CS
  #define LCD_SET_CS()
  #define LCD_CLR_CS()
#endif

/*********************************************************************
*
*       Hardware configuration
*
**********************************************************************

  Needs to be adapted to your target hardware.
*/

/* Configuration example:

  #define LCD_CLR_CS()      P0 &= ~(1<<0)
  #define LCD_SET_CS()      P0 |=  (1<<0) 
  #define LCD_CLR_RESET()   P0 &= ~(1<<2)
  #define LCD_SET_RESET()   P0 |=  (1<<2) 
  #define LCD_CLR_CLK()     P0 &= ~(1<<3)
  #define LCD_SET_CLK()     P0 |=  (1<<3) 
  #define LCD_CLR_SDI()     P0 &= ~(1<<4)
  #define LCD_SET_SDI()     P0 |=  (1<<4) 
  #define LCD_CLR_SDO()     P0 &= ~(1<<6)
  #define LCD_SET_SDO()     P0 |=  (1<<6) 

*/

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _Send1
*
* Purpose:
*   Writes 1 byte, MSB first
*/
static void _Send1(unsigned char Data) {
  if ((Data >> 7) & 1) LCD_SET_SDI(); else LCD_CLR_SDI(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 6) & 1) LCD_SET_SDI(); else LCD_CLR_SDI(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 5) & 1) LCD_SET_SDI(); else LCD_CLR_SDI(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 4) & 1) LCD_SET_SDI(); else LCD_CLR_SDI(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 3) & 1) LCD_SET_SDI(); else LCD_CLR_SDI(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 2) & 1) LCD_SET_SDI(); else LCD_CLR_SDI(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 1) & 1) LCD_SET_SDI(); else LCD_CLR_SDI(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 0) & 1) LCD_SET_SDI(); else LCD_CLR_SDI(); LCD_CLR_CLK(); LCD_SET_CLK();
}

/*********************************************************************
*
*       _Read1
*
* Purpose:
*   Reads 1 byte, MSB first
*/
static unsigned char _Read1(void) {
  unsigned char Data = 0;
  LCD_CLR_CLK(); LCD_SET_CLK(); if (LCD_SDI()) Data |= (1 << 7);
  LCD_CLR_CLK(); LCD_SET_CLK(); if (LCD_SDI()) Data |= (1 << 6);
  LCD_CLR_CLK(); LCD_SET_CLK(); if (LCD_SDI()) Data |= (1 << 5);
  LCD_CLR_CLK(); LCD_SET_CLK(); if (LCD_SDI()) Data |= (1 << 4);
  LCD_CLR_CLK(); LCD_SET_CLK(); if (LCD_SDI()) Data |= (1 << 3);
  LCD_CLR_CLK(); LCD_SET_CLK(); if (LCD_SDI()) Data |= (1 << 2);
  LCD_CLR_CLK(); LCD_SET_CLK(); if (LCD_SDI()) Data |= (1 << 1);
  LCD_CLR_CLK(); LCD_SET_CLK(); if (LCD_SDI()) Data |= (1 << 0);
  return Data;
}

/*********************************************************************
*
*       Exported code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_ClrCS
*
* Purpose:
*   Sets the chip select pin to low
*/
void LCD_X_ClrCS(void) {
  LCD_CLR_CS();
}

/*********************************************************************
*
*       LCD_X_SetCS
*
* Purpose:
*   Sets the chip select pin to high
*/
void LCD_X_SetCS(void) {
  LCD_SET_CS();
}

/*********************************************************************
*
*       LCD_X_WriteM
*
* Purpose:
*   Writes multiple bytes to controller
*/
void LCD_X_WriteM(unsigned char * pData, int NumBytes) {
  for (; NumBytes; NumBytes--) {
    _Send1(*pData++);
  }
}

/*********************************************************************
*
*       LCD_X_ReadM
*
* Purpose:
*   Reads multiple bytes from the controller
*/
void LCD_X_ReadM(unsigned char * pData, int NumBytes) {
  P0D = 0xAF;
  for (; NumBytes; NumBytes--) {
    *pData++ = _Read1();
  }
  P0D = 0xBF;
}

/*********************************************************************
*
*       LCD_X_Init
*
* Purpose:
*   This routine should be called from your application program
*   to set port pins to their initial values
*/
void LCD_X_Init(void) {
  LCD_CLR_RESET();
  LCD_SET_DDR();
  LCD_SET_CS();
  LCD_SET_CLK();
  LCD_SET_SDI();
  LCD_SET_RESET();
}

