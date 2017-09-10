/*********************************************************************
*                SEGGER MICROCONTROLLER GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 2003-2010     SEGGER Microcontroller GmbH & Co KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File        : GUIConf.c
Purpose     : Display controller initialization
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"

#ifndef _WINDOWS
//#include "drv_glcd.h"
#include "GLCD.h"
#endif
/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

//
// Define the available number of bytes available for the GUI
//
#define GUI_NUMBYTES  (1024 * 1024) * 2  // x MByte

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
#define LCD_GUI_RAM_BASE  (uint32_t)(LCD_VRAM_BASE_ADDR+(1024*1024*2))  
//__attribute__((aligned(4))) static U32 _aMemory[GUI_NUMBYTES / 4];
//__attribute__((aligned(4))) uint32_t _aMemory1[1024*10];
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
	//GUI_ALLOC_AssignMemory((void *)_aMemory, GUI_NUMBYTES);
	
  GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE);
  //Number of tasks from which emWin is used at most.
  //void GUITASK_SetMaxTask(int MaxTask);   
}

/*************************** End of file ****************************/
