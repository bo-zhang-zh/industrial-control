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
File        : 2DGL_DrawJPEGScaled.c
Purpose     : Example for drawing scaled JPG files without loading
              them into memory
----------------------------------------------------------------------
*/

#include <windows.h>
#include <stdio.h>

#include "GUI.h"
#include "EDIT.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static char _acBuffer[0x200];

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetData
*
* Purpose:
*   This routine is called by GUI_JPEG_DrawEx(). The routine is responsible
*   for setting the data pointer to a valid data location with at least
*   one valid byte.
*
* Parameters:
*   p           - Pointer to application defined data.
*   NumBytesReq - Number of bytes requested.
*   ppData      - Pointer to data pointer. This pointer should be set to
*                 a valid location.
*   StartOfFile - If this flag is 1, the data pointer should be set to the
*                 beginning of the data stream.
*
* Return value:
*   Number of data bytes available.
*/
static int _GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off) {
  DWORD NumBytesRead;
  HANDLE * phFile;

  phFile = (HANDLE *)p;
  /*
  * Check buffer size
  */
  if (NumBytesReq > sizeof(_acBuffer)) {
    NumBytesReq = sizeof(_acBuffer);
  }
  /*
  * Set file pointer to the required position
  */
  SetFilePointer(*phFile, Off, 0, FILE_BEGIN);
  /*
  * Read data into buffer
  */
  ReadFile(*phFile, _acBuffer, NumBytesReq, &NumBytesRead, NULL);
  /*
  * Set data pointer to the beginning of the buffer
  */
  *ppData = _acBuffer;
  /*
  * Return number of available bytes
  */
  return NumBytesRead;
}

/*********************************************************************
*
*       _DrawJPEG
*/
static void _DrawJPEG(const char * sFilename) {
  int XPos, YPos, XSize, YSize, nx, ny, n;
  HANDLE hFile;
  GUI_JPEG_INFO Info;
  hFile = CreateFile(sFilename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  GUI_JPEG_GetInfoEx(_GetData, &hFile, &Info);
  GUI_ClearRect(0, 60, 319, 239);
  nx = 320000 / Info.XSize;
  ny = 180000 / Info.YSize;
  if (nx < ny) {
    n = nx;
  } else {
    n = ny;
  }
  XSize = Info.XSize * n / 1000;
  YSize = Info.YSize * n / 1000;
  XPos = (320 - XSize) / 2;
  YPos = (180 - YSize) / 2 + 60;
  if (n > 1000) {
    int temp=0;
  }
  GUI_JPEG_DrawScaledEx(_GetData, &hFile, XPos, YPos, n, 1000);
  GUI_Delay(1000);
  CloseHandle(hFile);
}

/*******************************************************************
*
*       _GetPath
*
* Returns the path from where the JPEGs should be searched
*/
static void _GetPath(char * pPath, unsigned MaxSize) {
  WM_HWIN hWin;
  unsigned Len;
  if (!strlen(pPath)) {
    GetWindowsDirectory(pPath, MaxSize);
    strcat(pPath, "\\Web\\Wallpaper\\");
  }
  GUI_SetFont(&GUI_Font10_ASCII);
  GUI_DispStringHCenterAt("Please enter the path from where the JPEGs should be searched:", 160, 60);
  hWin = EDIT_Create(10, 80, 300, 20, 0, MaxSize, WM_CF_SHOW);
  EDIT_SetText(hWin, pPath);
  WM_SetFocus(hWin);
  while (GUI_GetKey() != GUI_KEY_ENTER) {
    GUI_Delay(100);
  }
  EDIT_GetText(hWin, pPath, MaxSize);
  WM_DeleteWindow(hWin);
  Len = strlen(pPath);
  if (*(pPath + Len - 1) != '\\') {
    if (Len < MaxSize) {
      strcat(pPath, "\\");
    }
  }
  GUI_ClearRect(0, 60, 319, 239);
}

/*******************************************************************
*
*       _DrawJPEGS
*
* Iterates over all JPEG files of a directory
*/
static void _DrawJPEGS(void) {
  static char acPath[_MAX_PATH];
  char acMask[_MAX_PATH];
  char acFile[_MAX_PATH];
  WIN32_FIND_DATA Context;
  HANDLE hFind;
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringHCenterAt("Drawing scaled JPEGs - Sample", 160, 5);
  _GetPath(acPath, sizeof(acPath));
  GUI_SetFont(&GUI_Font8x16);
  sprintf(acMask, "%s*.jp*", acPath);
  hFind = FindFirstFile(acMask, &Context);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      sprintf(acFile, "%s%s", acPath, Context.cFileName);
      GUI_DispStringAtCEOL(acFile, 5, 40);
      _DrawJPEG(acFile);
    } while (FindNextFile(hFind, &Context));
  } else {
    GUI_DispStringHCenterAt("No JPEG files found!", 160, 60);
    GUI_Delay(2000);
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
    _DrawJPEGS();
  }
}

/*************************** End of file ****************************/

