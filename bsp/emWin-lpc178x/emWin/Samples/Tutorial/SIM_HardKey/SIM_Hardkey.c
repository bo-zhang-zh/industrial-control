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
File        : SIM_Hardkey.c
Purpose     : Example demonstrating the HARDKEY simulation
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "SIM.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include <stddef.h>

/*******************************************************************
*
*       Static data
*
********************************************************************
*/

static const GUI_ConstString _aListBox[] = {
  "English", "Deutsch", "Fran�ais", "Japanese", "Italiano", "Espa�ol", NULL
};
static int _Ready;

/*******************************************************************
*
*       Static code
*
********************************************************************
*/

/*******************************************************************
*
*       _DemoHardkey

  Demonstrates HARDKEY simulation with a listbox
*/
static void _DemoHardkey(void) {
  LISTBOX_Handle hListBox;
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_SetTextAlign(GUI_TA_HCENTER);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringAt("Hardkey - Sample", 160, 5);
  GUI_SetFont(&GUI_Font8x8);
  GUI_DispStringHCenterAt("Exit",  65, 230);
  GUI_DispStringHCenterAt("Up",   160, 230);
  GUI_DispStringHCenterAt("Down", 255, 230);
  /* Create the listbox */
  hListBox = LISTBOX_Create(_aListBox, 100, 100, 120, 75, WM_CF_SHOW);
  WM_SetFocus(hListBox);
  LISTBOX_SetFont(hListBox, &GUI_Font13B_1);
  SCROLLBAR_CreateAttached(hListBox, SCROLLBAR_CF_VERTICAL);
  /* Handle the listbox */
  while (!_Ready) {
    GUI_Delay(10);
  }
  /* Delete listbox widget */
  LISTBOX_Delete(hListBox);
  GUI_Clear();
}

/*******************************************************************
*
*       _cbReady

  Callback for the ready-key
*/
static void _cbReady(int key, int state) {
  _Ready = 1;
};

/*******************************************************************
*
*       _cbKeyUp

  Callback for the up-key
*/
static void _cbKeyUp(int key, int state) {
  GUI_StoreKeyMsg(GUI_KEY_UP, state);
};

/*******************************************************************
*
*       _cbKeyDown

  Callback for the down-key
*/
static void _cbKeyDown(int key, int state) {
  GUI_StoreKeyMsg(GUI_KEY_DOWN, state);
};

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
  /* set callback routine for hardkey */
  SIM_HARDKEY_SetCallback(0, &_cbReady);
  SIM_HARDKEY_SetCallback(1, &_cbKeyUp);
  SIM_HARDKEY_SetCallback(2, &_cbKeyDown);
  /* start demo */
  _DemoHardkey();
}

/*************************** End of file ****************************/
