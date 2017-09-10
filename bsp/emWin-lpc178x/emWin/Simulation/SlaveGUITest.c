/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.16 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : Dashboard.c
Purpose     : dashboard demo
---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "GUI.h"
#include "LCDConf.h"
#include "MULTIPAGE.h"
#include "FRAMEWIN.h"
#include "DROPDOWN.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/


/*********************************************************************
*
*       _cbBkWindow
*/
static void _cbBkWindow(WM_MESSAGE* pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
/*    GUI_MEMDEV_Write(_hBkMemDev);*/
    break;
  case WM_KEY:
//     if (((WM_KEY_INFO *)pMsg->Data.p)->Key == GUI_KEY_ESCAPE) {
//       _Break = 1;
//     }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbWinMap
*/
static void _cbWinMap(WM_MESSAGE* pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
//     GUI_DrawBitmap(_NaviMap.pBitmap, -_NaviMap.x, -_NaviMap.y);
//     GUI_SetPenSize(3);
//     GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
//     GUI_SetColor(GUI_RED);
//     GUI_DrawCircle(_NaviMap.xHere, _NaviMap.yHere, 5);
//     GUI_SetTextMode(GUI_TM_TRANS);
//     GUI_SetFont(&GUI_FontComic18B_ASCII);
//     GUI_DispStringAt("You are here", _NaviMap.xHere - 20, _NaviMap.yHere - 20);
    /* Leave code for test purpose
    GUI_SetFont(&GUI_Font6x8);
    GUI_DispDecAt(_tDiff, 220, 15, 3);
    */
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

FRAMEWIN_Handle _hFrame, hWinMap;
extern GUI_HWIN App_CreateControlFrame(GUI_HWIN hParent, int x0, int y0);
/*********************************************************************
*
*       _DashboardDemo
*/
static void _DashboardDemo(void) {
  //WM_HWIN hClient;
  //CHECKBOX_Handle hCheck;
  //GUI_PID_STATE CurrentState;
  //WM_HWIN hItem;
  //GUI_PID_STATE State = {0};
  //GUI_PID_STATE OldState;

  /* Use memory devices */
  WM_SetCreateFlags(WM_CF_MEMDEV);
  WM_EnableMemdev(WM_HBKWIN);
  /* Set callback for background */
  WM_SetCallback(WM_HBKWIN, &_cbBkWindow);
  /* Create framewindow */
  _hFrame = App_CreateControlFrame(WM_HBKWIN, 500, 0);
  hWinMap  = FRAMEWIN_CreateEx(0, 0, 500, 480, WM_HBKWIN, WM_CF_SHOW, 0/*FRAMEWIN_CF_MOVEABLE*/, 0, "Running Status", &_cbWinMap);
  //FRAMEWIN_SetResizeable (hWinMap, 1);
  //FRAMEWIN_SetActive(hWinMap, 0);
  //hWinMap = WM_GetClientWindow(hWinMap);
  /* Handle the demo */
  GUI_CURSOR_Show();
}

/*********************************************************************
*
*       AppDashBoard
*
**********************************************************************
*/

int AppDashBoard(void);
int AppDashBoard(void) {
  WM_SetCallback(WM_HBKWIN, NULL);
  GUI_Clear();
  _DashboardDemo();
  return 1;
}
/*********************************************************************
*
*       MainTask
*/
void MainTask(void);
void MainTask(void) {
	GUI_Init();
	GUI_CURSOR_Select(&GUI_CursorArrowM);
	GUI_CURSOR_Show();  
	WM_SetDesktopColor(GUI_DARKGREEN);
	WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
	WM_SetCallback(WM_HBKWIN, NULL);
	GUI_Clear();
	_DashboardDemo();
	while (1) {
		//AppDashBoard();
		GUI_Delay(1000);
	}
}
/*************************** End of file ****************************/
