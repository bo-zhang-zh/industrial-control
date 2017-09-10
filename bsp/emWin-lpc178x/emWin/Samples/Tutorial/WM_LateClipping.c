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
File        : WM_LateClipping.c
Purpose     : Demonstrates early and late clipping
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "WM.h"
#include "FRAMEWIN.h"
#include "BUTTON.h"

/*********************************************************************
*
*       static data
*
**********************************************************************
*/

static WM_HWIN _hWin1;
static WM_HWIN _hWin2;
static WM_HWIN _hBut1;
static WM_HWIN _hBut2;

static int _PaintCount1;
static int _PaintCount2;

static GUI_COLOR _aColors[] = {
  0x0000FF, 0x00FF00, 0xFF0000, 0x00FFFF, 0xA000A0, 0xFFFF00
};

/*********************************************************************
*
*       static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _cbBkWin
*/
static void _cbBkWin(WM_MESSAGE* pMsg) {
  switch(pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetColor(0x0060FF);
    GUI_DispStringAt("PaintCount (Early):", 0, 0);
    GUI_DispDecAt(_PaintCount1, 120, 0, 5);
    GUI_SetColor(0x00FFC0);
    GUI_DispStringAt("PaintCount (Late):", 0, 12);
    GUI_DispDecAt(_PaintCount2, 120, 12, 5);
    break;
  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
      if (pMsg->hWinSrc == _hBut1) {
        WM_InvalidateWindow(_hWin1);
        WM_InvalidateWindow(_hWin2);
      } else if (pMsg->hWinSrc == _hBut2) {
        _PaintCount1 = 0;
        _PaintCount2 = 0;
        WM_InvalidateWindow(pMsg->hWin);
      }
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbTop
*/
static void _cbTop(WM_MESSAGE* pMsg) {
  switch(pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_MAGENTA);
    GUI_Clear();
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbFrameWin1
*/
static void _cbFrameWin1(WM_MESSAGE* pMsg) {
  switch(pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(_aColors[_PaintCount1 % 6]);
    GUI_Clear();
    GUI_SetColor(0x0060FF);
    GUI_FillCircle(25, 25, 15);
    GUI_SetColor(GUI_BLACK);
    GUI_DrawCircle(25, 25, 15);
    _PaintCount1++;
    WM_InvalidateWindow(WM_HBKWIN);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbFrameWin2
*/
static void _cbFrameWin2(WM_MESSAGE* pMsg) {
  switch(pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(_aColors[_PaintCount2 % 6]);
    GUI_Clear();
    GUI_SetColor(0x00FFC0);
    GUI_FillCircle(25, 25, 15);
    GUI_SetColor(GUI_BLACK);
    GUI_DrawCircle(25, 25, 15);
    _PaintCount2++;
    WM_InvalidateWindow(WM_HBKWIN);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _ShowDemo
*/
static void _ShowDemo(void) {
  WM_HWIN hWin0, hWin1, hWin2, hFrame1, hFrame2, hClient1, hClient2;
  WM_SetCallback(WM_HBKWIN, _cbBkWin);
  hFrame1 = FRAMEWIN_CreateEx( 10, 30, 140, 140, 0, WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, "Early Clipping", _cbFrameWin1);
  hFrame2 = FRAMEWIN_CreateEx(170, 30, 140, 140, 0, WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, "Late Clipping", _cbFrameWin2);
  hClient1 = WM_GetClientWindow(hFrame1);
  hClient2 = WM_GetClientWindow(hFrame2);
  _hWin1 = WM_CreateWindowAsChild(0, 0, WM_GetWindowSizeX(hClient1), WM_GetWindowSizeY(hClient1), hClient1, WM_CF_SHOW, _cbFrameWin1, 0);
  _hWin2 = WM_CreateWindowAsChild(0, 0, WM_GetWindowSizeX(hClient2), WM_GetWindowSizeY(hClient2), hClient2, WM_CF_SHOW | WM_CF_LATE_CLIP, _cbFrameWin2, 0);
  _hBut1 = BUTTON_CreateEx( 10, 210, 140, 20, 0, WM_CF_SHOW, 0, 1);
  _hBut2 = BUTTON_CreateEx(170, 210, 140, 20, 0, WM_CF_SHOW, 0, 2);
  hWin0 = FRAMEWIN_CreateEx( 60,  80, 40, 40, 0, WM_CF_SHOW | WM_CF_STAYONTOP, FRAMEWIN_CF_MOVEABLE, 0, "Top 0", _cbTop);
  hWin1 = FRAMEWIN_CreateEx(220,  80, 40, 40, 0, WM_CF_SHOW | WM_CF_STAYONTOP, FRAMEWIN_CF_MOVEABLE, 0, "Top 1", _cbTop);
  hWin2 = FRAMEWIN_CreateEx(140, 170, 40, 40, 0, WM_CF_SHOW | WM_CF_STAYONTOP, FRAMEWIN_CF_MOVEABLE, 0, "Top 2", _cbTop);
  FRAMEWIN_SetResizeable(hWin0, 1);
  FRAMEWIN_SetResizeable(hWin1, 1);
  FRAMEWIN_SetResizeable(hWin2, 1);
  BUTTON_SetText(_hBut1, "Invalidate");
  BUTTON_SetText(_hBut2, "Reset counters");
  while(1) {
    GUI_Delay(50);
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
*       MainTask
*/
void MainTask(void) {
  GUI_Init();
  while(1) {
    _ShowDemo();
  }
}

/*************************** End of file ****************************/

