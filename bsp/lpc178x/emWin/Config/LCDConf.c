/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.42 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only  be used  in accordance  with  a license  and should  not be  re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"

#ifndef _WINDOWS
#include "rtthread.h"
//#include "SEGGER.h"

#include "GUIDRV_Lin.h"

#include "LPC177x_8x.h"
#include "system_LPC177x_8x.h"
#include "glcd.h"
#include "touch.h"

#endif
/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/
#ifndef USE_24BPP_MODE
  #define USE_24BPP_MODE  0  // Selection if using 16 BPP or 24 BPP mode
#endif
//
// Physical display size
//
#ifndef _WINDOWS
#define XSIZE_PHYS GLCD_X_SIZE
#define YSIZE_PHYS GLCD_Y_SIZE
#else
#define XSIZE_PHYS 800
#define YSIZE_PHYS 400
#endif
//
// Color conversion
//
#if USE_24BPP_MODE
    #define COLOR_CONVERSION  GUICC_888
#else
    #define COLOR_CONVERSION  GUICC_M565    /*GUICC_M565: RB SWAP GUICC_565:RB NORMAL*/
#endif
//
// Display driver
//
#if USE_24BPP_MODE
    #define DISPLAY_DRIVER  &GUIDRV_Lin_32_API
#else
    #define DISPLAY_DRIVER  &GUIDRV_Lin_16_API
#endif
//
// Pixel width in bytes
//
#if USE_24BPP_MODE
    #define PIXEL_WIDTH  4
#else
    #define PIXEL_WIDTH  2
#endif
/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   VRAM_ADDR
  #define VRAM_ADDR 0
#endif

#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif
#ifndef   DISPLAY_DRIVER
  #error No display driver defined!
#endif

/*********************************************************************
*
*       _SetDisplayOrigin()
*/
#ifndef _WINDOWS
static void _SetDisplayOrigin(int x, int y) {
  (void)x;
  //
  // Set start address for display data and enable LCD controller
  //
  LPC_LCD->UPBASE = LCD_VRAM_BASE_ADDR + (y * XSIZE_PHYS * PIXEL_WIDTH);  // Needs to be set, before LCDC is enabled
}
#endif
/*********************************************************************
*
*       _InitController
*
* Function description:
*   Initializes the LCD controller and touch screen
*
*/
#ifndef _WINDOWS
//
// Touch controller settings
//
#define TOUCH_AD_LEFT         3890//320       //触摸屏最左端ADC采样值
#define TOUCH_AD_RIGHT        190//3550      //触摸屏最右端ADC采样值
#define TOUCH_AD_TOP          3780//800       //触摸屏最上端ADC采样值
#define TOUCH_AD_BOTTOM       235//3050      //触摸屏最下端ADC采样值
#define TOUCH_TIMER_INTERVAL  10        //触摸屏采样间隔

static struct rt_thread _TCB_Touch;
static rt_uint8_t _StackTouch[512];
static void _InitController(unsigned LayerIndex) {
  //
  // Set display size and video-RAM address
  //
  LCD_SetSizeEx (XSIZE_PHYS, YSIZE_PHYS, LayerIndex);
  LCD_SetVSizeEx(VXSIZE_PHYS, VYSIZE_PHYS, LayerIndex);
  LCD_SetVRAMAddrEx(LayerIndex, (void*)LCD_VRAM_BASE_ADDR);
  //
  // Init LCD
  //
  GLCD_Init();

#if GUI_SUPPORT_TOUCH  // Used when touch screen support is enabled
  {
    U32 TouchOrientation;
    TouchOrientation = (GUI_MIRROR_X * LCD_GetMirrorXEx(0)) |
                       (GUI_MIRROR_Y * LCD_GetMirrorYEx(0)) |
                       (GUI_SWAP_XY  * LCD_GetSwapXYEx (0)) ;
    GUI_TOUCH_SetOrientation(TouchOrientation);
    if (LCD_GetSwapXYEx(0)) {
      GUI_TOUCH_Calibrate(GUI_COORD_X, 0, XSIZE_PHYS, TOUCH_AD_LEFT, TOUCH_AD_RIGHT);   // x axis swapped
      GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, YSIZE_PHYS, TOUCH_AD_TOP , TOUCH_AD_BOTTOM);  // y axis swapped
    } else {
      GUI_TOUCH_Calibrate(GUI_COORD_X, 0, XSIZE_PHYS, TOUCH_AD_LEFT, TOUCH_AD_RIGHT);   // x axis
      GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, YSIZE_PHYS, TOUCH_AD_TOP , TOUCH_AD_BOTTOM);  // y axis
    }
    //
    // Initialize task for touch support
    //
    rt_thread_init(&_TCB_Touch,
            "touch",
            _TouchTask, RT_NULL,
            &_StackTouch[0], sizeof(_StackTouch),
            13, 10);
    rt_thread_startup(&_TCB_Touch);
  }
#endif
}
#endif
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*   
*/
void LCD_X_Config(void) {
  //
  // Set display driver and color conversion for 1st layer
  //
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  //
  // Display driver configuration, required for Lin-driver
  //
  LCD_SetPosEx     (0, 0, 0);
  if (LCD_GetSwapXYEx(0)) {
    LCD_SetSizeEx  (0, YSIZE_PHYS , XSIZE_PHYS);
    LCD_SetVSizeEx (0, VYSIZE_PHYS, VXSIZE_PHYS);
  } else {
    LCD_SetSizeEx  (0, XSIZE_PHYS , YSIZE_PHYS);
    LCD_SetVSizeEx (0, VXSIZE_PHYS, VYSIZE_PHYS);
  }
  LCD_SetVRAMAddrEx(0, (void *)VRAM_ADDR);
  //
  // Set user palette data (only required if no fixed palette is used)
  //
  #if defined(PALETTE)
    LCD_SetLUTEx(0, PALETTE);
  #endif
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if 
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  LCD_X_SETORG_INFO      * pSetOrg;

  switch (Cmd) {
  //
  // Required
  //
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine this needs
    // to be adapted by the customer...
    //
#ifndef _WINDOWS
    _InitController(0);
#endif
    return 0;
  }
  case LCD_X_SETVRAMADDR: {
    //
    // Required for setting the address of the video RAM for drivers
    // with memory mapped video RAM which is passed in the 'pVRAM' element of p
    //
    LCD_X_SETVRAMADDR_INFO * p;
    p = (LCD_X_SETVRAMADDR_INFO *)pData;
    //...
    return 0;
  }
  case LCD_X_SETORG: {
    //
    // Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p
    //
#ifndef _WINDOWS
    pSetOrg = (LCD_X_SETORG_INFO *)pData;
    _SetDisplayOrigin(pSetOrg->xPos, pSetOrg->yPos);
#endif
    return 0;
  }
  case LCD_X_SETLUTENTRY: {
    //
    // Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p
    //
    LCD_X_SETLUTENTRY_INFO * p;
    p = (LCD_X_SETLUTENTRY_INFO *)pData;
    //...
    return 0;
  }
  case LCD_X_ON: {
    //
    // Required if the display controller should support switching on and off
    //
    return 0;
  }
  case LCD_X_OFF: {
    //
    // Required if the display controller should support switching on and off
    //
    // ...
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/
