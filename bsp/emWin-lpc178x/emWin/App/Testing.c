/*
******************************************************************
**                      uCGUIBuilder                            **
**                  Version:   4.0.0.0                          **
**                     2012 / 04                               **
**                   CpoyRight to: wyl                          **
**              Email:ucguibuilder@163.com                        **
**          This text was Created by uCGUIBuilder               **
******************************************************************/

#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"

#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"





//EventsFunctionList
void OnBtnXYRightClicked(WM_MESSAGE * pMsg);
void OnBtnXYRightReleased(WM_MESSAGE * pMsg);
void OnBtnXYLeftClicked(WM_MESSAGE * pMsg);
void OnBtnXYLeftReleased(WM_MESSAGE * pMsg);
void OnBtnXYDownClicked(WM_MESSAGE * pMsg);
void OnBtnXYDownReleased(WM_MESSAGE * pMsg);
void OnBtnXYUpClicked(WM_MESSAGE * pMsg);
void OnBtnXYUpReleased(WM_MESSAGE * pMsg);
void OnDropDownXYDistSelChanged(WM_MESSAGE * pMsg);
void OnBtnZStartClicked(WM_MESSAGE * pMsg);
void OnBtnElemgONOFFClicked(WM_MESSAGE * pMsg);
//EndofEventsFunctionList


/*********************************************************************
*
*       static data
*
**********************************************************************
*/

#define GUI_ID_TEXT_SEN_X_ORG   GUI_ID_USER+1
#define GUI_ID_TEXT_SEN_Y_ORG   GUI_ID_USER+2
#define GUI_ID_TEXT_SEN_QUILT   GUI_ID_USER+3
#define GUI_ID_TEXT_SEN_FORWARD   GUI_ID_USER+4
#define GUI_ID_TEXT_SEN_BACKWARD   GUI_ID_USER+5
#define GUI_ID_TEXT_SEN_THREAD   GUI_ID_USER+6
#define GUI_ID_TEXT_SEN_POWER   GUI_ID_USER+7
#define GUI_ID_TEXT_SEN_HIGH_POS   GUI_ID_USER+8
#define GUI_ID_TEXT_SEN_BREAK   GUI_ID_USER+9
#define GUI_ID_BTN_ELEMG_ONOFF   GUI_ID_USER+10
#define GUI_ID_TEXT_STATUS_ELEMG   GUI_ID_USER+11
#define GUI_ID_TEXT_Z_SPEED   GUI_ID_USER+12
#define GUI_ID_BTN_Z_START   GUI_ID_USER+13
#define GUI_ID_BTN_Z_ACC   GUI_ID_USER+14
#define GUI_ID_BTN_Z_DEC   GUI_ID_USER+15
#define GUI_ID_BTN_XY_UP   GUI_ID_USER+16
#define GUI_ID_BTN_XY_DOWN   GUI_ID_USER+17
#define GUI_ID_BTN_XY_LEFT   GUI_ID_USER+18
#define GUI_ID_BTN_XY_RIGHT   GUI_ID_USER+19
#define GUI_ID_DROPDOWN_XY_DIST  GUI_ID_USER+20

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "Testing",           0,                       0,  0,  360,300,FRAMEWIN_CF_MOVEABLE,0},
    { TEXT_CreateIndirect,      "Sensor of X origin:",NULL,                    10, 19, 119,17, 0,0},
    { TEXT_CreateIndirect,      "OFF",               GUI_ID_TEXT_SEN_X_ORG,   143,19, 31, 17, 0,0},
    { TEXT_CreateIndirect,      "Sensor of Y origin:",NULL,                    10, 37, 119,17, 0,0},
    { TEXT_CreateIndirect,      "OFF",               GUI_ID_TEXT_SEN_Y_ORG,   143,37, 31, 17, 0,0},
    { TEXT_CreateIndirect,      "Quilt/Pause button:",NULL,                    10, 55, 119,17, 0,0},
    { TEXT_CreateIndirect,      "ON",                GUI_ID_TEXT_SEN_QUILT,   143,55, 31, 17, 0,0},
    { TEXT_CreateIndirect,      "Forward button:",   NULL,                    10, 73, 99, 17, 0,0},
    { TEXT_CreateIndirect,      "ON",                GUI_ID_TEXT_SEN_FORWARD, 143,73, 30, 17, 0,0},
    { TEXT_CreateIndirect,      "Backward button:",  NULL,                    10, 91, 105,17, 0,0},
    { TEXT_CreateIndirect,      "ON",                GUI_ID_TEXT_SEN_BACKWARD,143,91, 31, 17, 0,0},
    { TEXT_CreateIndirect,      "Thread button:",    NULL,                    10, 109,97, 17, 0,0},
    { TEXT_CreateIndirect,      "ON",                GUI_ID_TEXT_SEN_THREAD,  143,109,30, 17, 0,0},
    { TEXT_CreateIndirect,      "Motor power:",      NULL,                    10, 127,79, 17, 0,0},
    { TEXT_CreateIndirect,      "ON",                GUI_ID_TEXT_SEN_POWER,   143,127,30, 17, 0,0},
    { TEXT_CreateIndirect,      "High position sensor:",NULL,                    10, 145,132,17, 0,0},
    { TEXT_CreateIndirect,      "LOW",               GUI_ID_TEXT_SEN_HIGH_POS,143,145,30, 17, 0,0},
    { TEXT_CreateIndirect,      "Break sensor:",     NULL,                    10, 163,96, 17, 0,0},
    { TEXT_CreateIndirect,      "ON",                GUI_ID_TEXT_SEN_BREAK,   143,163,30, 17, 0,0},
    { BUTTON_CreateIndirect,    "ON/OFF",            GUI_ID_BTN_ELEMG_ONOFF,  212,32, 67, 31, 0,0},
    { TEXT_CreateIndirect,      "OFF",               GUI_ID_TEXT_STATUS_ELEMG,299,41, 27, 14, 0,0},
    { TEXT_CreateIndirect,      "Z motor speed:",    NULL,                    212,118,94, 17, 0,0},
    { TEXT_CreateIndirect,      "unKnown",           GUI_ID_TEXT_Z_SPEED,     298,118,48, 17, 0,0},
    { BUTTON_CreateIndirect,    "START",             GUI_ID_BTN_Z_START,      215,138,43, 28, 0,0},
    { BUTTON_CreateIndirect,    "ACC",               GUI_ID_BTN_Z_ACC,        266,140,40, 25, 0,0},
    { BUTTON_CreateIndirect,    "DEC",               GUI_ID_BTN_Z_DEC,        312,140,40, 25, 0,0},
    { DROPDOWN_CreateIndirect,  "DROPDOWN",          GUI_ID_DROPDOWN_XY_DIST, 115,197,49, 80, 0,0},
    { TEXT_CreateIndirect,      "Move distance:",    NULL,                    18, 200,91, 17, 0,0},
    { BUTTON_CreateIndirect,    "UP",                GUI_ID_BTN_XY_UP,        74, 220,45, 26, 0,0},
    { BUTTON_CreateIndirect,    "DOWN",              GUI_ID_BTN_XY_DOWN,      74, 257,45, 26, 0,0},
    { BUTTON_CreateIndirect,    "LEFT",              GUI_ID_BTN_XY_LEFT,      21, 234,45, 26, 0,0},
    { BUTTON_CreateIndirect,    "RIGHT",             GUI_ID_BTN_XY_RIGHT,     125,234,45, 26, 0,0},
    { BUTTON_CreateIndirect,    "OK",                GUI_ID_OK,               251,197,75, 28, 0,0},
    { BUTTON_CreateIndirect,    "CLOSE",             GUI_ID_CLOSE,            251,244,74, 26, 0,0},
    { TEXT_CreateIndirect,      "Sensor Status",     NULL,                    4,  -1, 90, 19, 0,0},
    { TEXT_CreateIndirect,      "X/Y Motor test",    NULL,                    4,  182,91, 18, 0,0},
    { TEXT_CreateIndirect,      "Electromagnet Test",NULL,                    192,4,  133,16, 0,0},
    { TEXT_CreateIndirect,      "Z Motor test",      NULL,                    195,97, 88, 21, 0,0},
    { TEXT_CreateIndirect,      "mm",                NULL,                    170,200,25, 15, 0,0}
};



/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

void Testing_PaintDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    GUI_SetColor(0x000000);
    GUI_DrawLine(0,179,179,179);
    GUI_SetColor(0x000000);
    GUI_DrawLine(179,-1,179,178);
    GUI_SetColor(0x000000);
    GUI_DrawLine(180,81,359,81);

}



/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void Testing_InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    //
    //GUI_ID_TEXT_STATUS_ELEMG
    //
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT_STATUS_ELEMG),0x0000ff);
    //
    //GUI_ID_DROPDOWN_XY_DIST
    //
    DROPDOWN_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_XY_DIST),0,0xffffff);
    DROPDOWN_SetAutoScroll(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_XY_DIST),1);
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_XY_DIST),"100");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_XY_DIST),"10");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_XY_DIST),"1");

}




/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            Testing_PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            Testing_InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_BTN_ELEMG_ONOFF:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnElemgONOFFClicked(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BTN_Z_START:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnZStartClicked(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_DROPDOWN_XY_DIST:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_SEL_CHANGED:
                            OnDropDownXYDistSelChanged(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BTN_XY_UP:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnXYUpClicked(pMsg);
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            OnBtnXYUpReleased(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BTN_XY_DOWN:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnXYDownClicked(pMsg);
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            OnBtnXYDownReleased(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BTN_XY_LEFT:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnXYLeftClicked(pMsg);
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            OnBtnXYLeftReleased(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BTN_XY_RIGHT:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnXYRightClicked(pMsg);
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            OnBtnXYRightReleased(pMsg);
                            break;
                    }
                    break;

            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}


/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
// void MainTask(void) 
// { 
//     GUI_Init();
//     WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */
//     WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
//     //PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
//     //FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
//     //PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
//     //BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
//     //CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
//     //DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
//     //SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
//     //SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
//     //HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
//     //RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
//     GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
// }

GUI_HWIN App_CreateTesting(GUI_HWIN hParent, int x0, int y0)
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate),  &_cbCallback, hParent, x0, y0);
	return hWin;
}
