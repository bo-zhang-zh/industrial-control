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
File        : WIDGET_SimpleButton.c
Purpose     : Example demonstrating the use of a BUTTON widget
----------------------------------------------------------------------
*/

#include "GUI.h"
#include "BUTTON.h"

/*******************************************************************
*
*       static code
*
********************************************************************
*/

/*******************************************************************
*
*       _DemoButton
*/
static void _DemoButton(void) {
  BUTTON_Handle hButton;
  GUI_SetFont(&GUI_Font8x16);
  GUI_DispStringHCenterAt("Click on button...", 160, 90);
  /* Create the button*/
  hButton = BUTTON_Create(110, 110, 100, 40, GUI_ID_OK, WM_CF_SHOW);
  /* Set the button text */
  BUTTON_SetText(hButton, "Click me...");
  /* Let window manager handle the button */
  while (GUI_WaitKey() != GUI_ID_OK);
  /* Delete the button*/
  BUTTON_Delete(hButton);
  GUI_ClearRect(0, 50, 319, 239);
  GUI_Delay(1000);
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
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringHCenterAt("WIDGET_SimpleButton - Sample", 160, 5);
  while (1) {
    _DemoButton();
  }
}

/*************************** End of file ****************************/

