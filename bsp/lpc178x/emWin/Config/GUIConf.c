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
File        : GUIConf.c
Purpose     : Display controller initialization
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "glcd.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Define the available number of bytes available for the GUI
//
#define GUI_NUMBYTES    (2 << 20)    // 2MByte

//
// Define the average block size
//
#define GUI_BLOCKSIZE 128

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/* the addr by manual , be careful cal ,now space 2M byte to LCD memory*/

#ifndef _WINDOWS
#define LCD_GUI_RAM_BASE  (U32)(LCD_VRAM_BASE_ADDR+(1024*1024*2))  
#else
static U32 _aMemory[GUI_NUMBYTES / 4];
#define LCD_GUI_RAM_BASE _aMemory
#endif 

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   available memory for the GUI.
*/
void GUI_X_Config(void) {
  //
  // Assign memory to emWin
  //
  GUI_ALLOC_AssignMemory((void *)LCD_GUI_RAM_BASE, GUI_NUMBYTES);
	
  GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE);
  //Number of tasks from which emWin is used at most.
  //void GUITASK_SetMaxTask(int MaxTask);   
}

/*************************** End of file ****************************/
