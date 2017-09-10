/*********************************************************************
*                SEGGER MICROCONTROLLER GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 2003-2012     SEGGER Microcontroller GmbH & Co KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : SEGGERDEMO.c
Purpose : SEGGER product presentation
--------  END-OF-HEADER  ---------------------------------------------
*/

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
#define SEGGERDEMO_DELAY  5000  // Generic delay for presentations [ms]

/*********************************************************************
*
*       Includes, generic
*
**********************************************************************
*/
#include "GUIDEMO.h"

/*********************************************************************
*
*       Externals
*
**********************************************************************
*/
extern GUI_CONST_STORAGE GUI_BITMAP bmIMAGE_SeggerLogo_300x150_565c;

/*********************************************************************
*
*       Static data, SEGGERDEMO
*
**********************************************************************
*/
#if ((!defined _WINDOWS) && USE_GUI_TASK)
//
// GUI task stack can be linked to a specific section by defining the section GUIDEMO_STACK in the linker file
//
#ifdef __ICCARM__
  #pragma location="GUIDEMO_STACK"
#endif
#ifdef __CC_ARM
  U32 static _GUITask_Stack[1024] __attribute__ ((section ("GUIDEMO_STACK"), zero_init));
#endif
#ifdef _WINDOWS
  static U32 _aMemory[GUI_NUMBYTES / 4];
#endif

#endif

/*********************************************************************
*
*       GUIDEMO routine table
*/
static void (* _apfTest[])(void) = {
#if (SHOW_GUIDEMO_SPEEDOMETER && GUI_SUPPORT_MEMDEV)
  GUIDEMO_Speedometer,
#endif
#if (SHOW_GUIDEMO_ZOOMANDROTATE && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)
  GUIDEMO_ZoomAndRotate,
#endif
#if (SHOW_GUIDEMO_SKINNING && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)
  GUIDEMO_Skinning,
#endif
#if (SHOW_GUIDEMO_FADING && GUI_SUPPORT_MEMDEV)
  GUIDEMO_Fading,
#endif
#if (SHOW_GUIDEMO_TRANSPARENTDIALOG && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)
  GUIDEMO_TransparentDialog,
#endif
#if (SHOW_GUIDEMO_ICONVIEW && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)
  GUIDEMO_IconView,
#endif
#if (SHOW_GUIDEMO_IMAGEFLOW && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)
  GUIDEMO_ImageFlow,
#endif
#if (SHOW_GUIDEMO_TREEVIEW && GUI_WINSUPPORT)
 // GUIDEMO_Treeview,
#endif
#if (SHOW_GUIDEMO_LISTVIEW && GUI_WINSUPPORT)
  GUIDEMO_Listview,
#endif
#if SHOW_GUIDEMO_VSCREEN
  GUIDEMO_VScreen,
#endif
#if (SHOW_GUIDEMO_GRAPH && GUI_SUPPORT_MEMDEV)
  GUIDEMO_Graph,
#endif
#if SHOW_GUIDEMO_SPEED
  GUIDEMO_Speed,
#endif
#if SHOW_GUIDEMO_BITMAP
  GUIDEMO_Bitmap,
#endif
#if (SHOW_GUIDEMO_CURSOR && GUI_SUPPORT_CURSOR && GUI_SUPPORT_TOUCH)
  GUIDEMO_Cursor,
#endif
#if SHOW_GUIDEMO_COLORBAR
  GUIDEMO_ColorBar,
#endif
#if (SHOW_GUIDEMO_AUTOMOTIVE && GUI_SUPPORT_MEMDEV)
  GUIDEMO_Automotive,
#endif
#if (SHOW_GUIDEMO_BARGRAPH && GUI_SUPPORT_MEMDEV)
  GUIDEMO_BarGraph,
#endif
#if (SHOW_GUIDEMO_WASHINGMACHINE && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)
  GUIDEMO_WashingMachine,
#endif
  0
};

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       GUIDEMO_Config
*/
void GUIDEMO_Config(GUIDEMO_CONFIG * pConfig) {
  pConfig->apFunc   = _apfTest;
  pConfig->NumDemos = GUI_COUNTOF(_apfTest);
  pConfig->Flags    = GUIDEMO_CF_SHOW_SPRITES | GUIDEMO_CF_USE_AUTO_BK;
}

/*********************************************************************
*
*       MainTask()
*/
void MainTask(void);
void MainTask(void) {
  //
  // Init GUI
  //
  WM_SetCreateFlags(WM_CF_MEMDEV);
  GUI_Init();
  #if GUI_SUPPORT_CURSOR
    GUI_CURSOR_Hide();  // Hide cursor before first page is shown
  #endif
  //
  // Show SEGGER logo
  //
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_FillRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
  GUI_DrawBitmap(&bmIMAGE_SeggerLogo_300x150_565c, (LCD_GetXSize() - 300) / 2, (LCD_GetYSize() - 150) / 2);
  GUI_X_Delay(SEGGERDEMO_DELAY);
  GUI_Clear();
  //
  // Start GUIDEMO samples, GUIDEMO_Main is an endless loop itself. Therefor we can start it as task directly.
  //
  GUIDEMO_Main();
}

/****** End of File *************************************************/
