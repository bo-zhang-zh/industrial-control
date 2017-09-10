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
File        : LCD_X_SERIAL_3Wire.c
Purpose     : Port routines
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
*           Hardware configuration
*
**********************************************************************

  Needs to be adapted to your target hardware.
*/

/* Configuration example:

#define LCD_CLR_RESET()   P7 &= ~(1<<2)
#define LCD_SET_RESET()   P7 |=  (1<<2) 
#define LCD_CLR_CLK()     P7 &= ~(1<<3)
#define LCD_SET_CLK()     P7 |=  (1<<3) 
#define LCD_CLR_DATA()    P7 &= ~(1<<4)
#define LCD_SET_DATA()    P7 |=  (1<<4) 
#define LCD_SET_DIR_OUT() P7D = 0xff
#define LCD_CLR_CS()      P7 &= ~(1<<0)      // Optional
#define LCD_SET_CS()      P7 |=  (1<<0)      // Optional
#define LCD_DELAY(ms)     GUI_Delay(ms)      // Optional

*/

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static unsigned char  _aBuffer[10];
static int            _NumBytesInBuffer;
static int            _BufferMode;

/*********************************************************************
*
*           Initialisation
*
**********************************************************************
  This routine should be called from your application program
  to set port pins to their initial values
*/

void LCD_X_Init(void) {
  LCD_CLR_RESET();
  LCD_SET_DIR_OUT();
  LCD_SET_CS();
  LCD_SET_RESET();
  LCD_SET_CLK();
  LCD_SET_DATA();
  LCD_DELAY(20);
  LCD_SET_RESET();
  LCD_DELAY(20);
}

/*********************************************************************
*
*           Send1 routine
*
**********************************************************************

  Usually, there is no need to modify this routine.
  It should be sufficient ot modify the low-level macros above or
  in a configuration file.
*/

/* Write 1 byte, MSB first */
static void Send1(unsigned char Data, unsigned char IsData) {
  if (IsData)          LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 7) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 6) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 5) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 4) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 3) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 2) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 1) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
  if ((Data >> 0) & 1) LCD_SET_DATA(); else LCD_CLR_DATA(); LCD_CLR_CLK(); LCD_SET_CLK();
}

/*********************************************************************
*
*       _FlushBuffer
*/
static void _FlushBuffer(void) {
  unsigned char * pData;

  if (_NumBytesInBuffer) {
    pData = _aBuffer;
    LCD_CLR_CS();
    do {
      Send1(*pData++, _BufferMode);
    } while (--_NumBytesInBuffer);
    LCD_SET_CS();
  }
}

/*********************************************************************
*
*       _BufferWrite
*/
static void _BufferWrite(unsigned char Byte, int Mode) {
  if (Mode != _BufferMode) {
    _FlushBuffer();
  }
  _BufferMode = Mode;
  _aBuffer[_NumBytesInBuffer++] = Byte;
  if (_NumBytesInBuffer == GUI_COUNTOF(_aBuffer)) {
    _FlushBuffer();
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Write01
*/
void LCD_X_Write01(unsigned char Data) {
  _BufferWrite(Data, 1);
}

/*********************************************************************
*
*       LCD_X_WriteM01
*/
void LCD_X_WriteM01(unsigned char * pData, int NumBytes) {
  for (; NumBytes; NumBytes--) {
    _BufferWrite(*pData++, 1);
  }
}

/*********************************************************************
*
*       LCD_X_Write00
*/
void LCD_X_Write00(unsigned char Cmd) {
  _BufferWrite(Cmd, 0);
}

/*********************************************************************
*
*       LCD_X_FlushBuffer
*/
void LCD_X_FlushBuffer(void) {
  _FlushBuffer();
}