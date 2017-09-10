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
void OnListViewSelChanged(WM_MESSAGE * pMsg);
void OnBtnExitClicked(WM_MESSAGE * pMsg);
void OnBtnSaveClicked(WM_MESSAGE * pMsg);
void OnBtnSelectAllClicked(WM_MESSAGE * pMsg);
//EndofEventsFunctionList


/*********************************************************************
*
*       static data
*
**********************************************************************
*/

#define GUI_ID_BTN_CHOOSE4USE   GUI_ID_USER+1
#define GUI_ID_BTN_CANCEL_SEL   GUI_ID_USER+2
#define GUI_ID_LISTVIEW_LOCAL_FILE   GUI_ID_USER+3
#define GUI_ID_LISTVIEW_REMOVEABLE_FILE   GUI_ID_USER+4
#define GUI_ID_BTN_COPY2REMOVEABLE   GUI_ID_USER+5
#define GUI_ID_BTN_COPY2LOCAL   GUI_ID_USER+5
#define GUI_ID_BTN_DEL   GUI_ID_USER+6
#define GUI_ID_EDIT_LOCAL_FOLDER   GUI_ID_USER+7
#define GUI_ID_BTN_LOCAL_FOLDER   GUI_ID_USER+8
#define GUI_ID_EDIT_UDISK_FOLDER   GUI_ID_USER+9
#define GUI_ID_BTN_REMOVEABLE_FOLDER   GUI_ID_USER+10
#define GUI_ID_TEXT_LOCAL_NUM   GUI_ID_USER+11
#define GUI_ID_TEXT_LOCAL_SEL   GUI_ID_USER+12
#define GUI_ID_TEXT_UDISK_NUM   GUI_ID_USER+13
#define GUI_ID_TEXT_UDISK_SEL   GUI_ID_USER+14
#define GUI_ID_RADIO_SEL_MODE   GUI_ID_USER+15


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "Select File",       0,                       0,  0,  600,400,FRAMEWIN_CF_MOVEABLE,0},
    { TEXT_CreateIndirect,      "Patten Preview",    NULL,                    2,  3,  93, 14, 0,0},
    { TEXT_CreateIndirect,      "Patten Management", NULL,                    299,3,  115,13, 0,0},
    { BUTTON_CreateIndirect,    "Select for Use",    GUI_ID_BTN_CHOOSE4USE,   309,296,135,23, 0,0},
    { BUTTON_CreateIndirect,    "Cancel select",     GUI_ID_BTN_CANCEL_SEL,   497,229,93, 23, 0,0},
    { BUTTON_CreateIndirect,    "Close",             GUI_ID_CLOSE,            509,355,81, 24, 0,0},
    { TEXT_CreateIndirect,      "Loacal Folder:",    NULL,                    299,28, 90, 18, 0,0},
    { LISTVIEW_CreateIndirect,   NULL,               GUI_ID_LISTVIEW_LOCAL_FILE,309,83, 118,136,0,0},
    { LISTVIEW_CreateIndirect,   NULL,               GUI_ID_LISTVIEW_REMOVEABLE_FILE,472,83, 118,136,0,0},
    { BUTTON_CreateIndirect,    ">>",                GUI_ID_BTN_COPY2REMOVEABLE,433,116,33, 30, 0,0},
    { BUTTON_CreateIndirect,    "<<",                GUI_ID_BTN_COPY2LOCAL,   433,158,33, 30, 0,0},
    { BUTTON_CreateIndirect,    "Delete",            GUI_ID_BTN_DEL,          497,268,93, 21, 0,0},
    { TEXT_CreateIndirect,      "COPY",              NULL,                    434,94, 31, 19, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_LOCAL_FOLDER,311,40, 107,18, 0,0},
    { BUTTON_CreateIndirect,     NULL,               GUI_ID_BTN_LOCAL_FOLDER, 424,40, 20, 18, 0,0},
    { TEXT_CreateIndirect,      "Udisk Folder:",     NULL,                    456,28, 89, 12, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_UDISK_FOLDER,474,40, 98, 17, 0,0},
    { BUTTON_CreateIndirect,     NULL,               GUI_ID_BTN_REMOVEABLE_FOLDER,575,40, 19, 18, 0,0},
    { TEXT_CreateIndirect,      "Num:",              NULL,                    301,62, 30, 18, 0,0},
    { TEXT_CreateIndirect,      "Num:",              NULL,                    456,62, 31, 14, 0,0},
    { TEXT_CreateIndirect,      "0",                 GUI_ID_TEXT_LOCAL_NUM,   331,61, 31, 14, 0,0},
    { TEXT_CreateIndirect,      "Selected:",         NULL,                    359,62, 59, 15, 0,0},
    { TEXT_CreateIndirect,      "0",                 GUI_ID_TEXT_LOCAL_SEL,   416,62, 28, 15, 0,0},
    { TEXT_CreateIndirect,      "0",                 GUI_ID_TEXT_UDISK_NUM,   484,62, 22, 15, 0,0},
    { TEXT_CreateIndirect,      "Selected:",         NULL,                    509,62, 61, 17, 0,0},
    { TEXT_CreateIndirect,      "0",                 GUI_ID_TEXT_UDISK_SEL,   564,62, 30, 16, 0,0},
    { TEXT_CreateIndirect,      "Select Mode:",      NULL,                    311,225,77, 14, 0,0},
    { RADIO_CreateIndirect,      NULL,               GUI_ID_RADIO_SEL_MODE,   385,225,69, 54, 0,3}
};



/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

void SelectFile_PaintDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;

}



/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void SelectFile_InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    //
    //GUI_ID_LISTVIEW_LOCAL_FILE
    //
    LISTVIEW_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW_LOCAL_FILE),LISTBOX_CI_UNSEL,0xffffff);
    //LISTVIEW_SetAutoScrollH(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW_LOCAL_FILE),1);
    //LISTVIEW_SetAutoScrollV(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW_LOCAL_FILE),1);
    LISTVIEW_AddColumn(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW_LOCAL_FILE),73,"Name",GUI_TA_VCENTER|GUI_TA_LEFT);
    LISTVIEW_AddColumn(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW_LOCAL_FILE),38,"Size",GUI_TA_VCENTER|GUI_TA_LEFT);
    //
    //GUI_ID_LISTVIEW_REMOVEABLE_FILE
    //
    LISTVIEW_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW_REMOVEABLE_FILE),LISTBOX_CI_UNSEL,0xffffff);
    //LISTVIEW_SetAutoScrollH(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW_REMOVEABLE_FILE),1);
    //LISTVIEW_SetAutoScrollV(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW_REMOVEABLE_FILE),1);
    LISTVIEW_AddColumn(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW_REMOVEABLE_FILE),73,"Name",GUI_TA_VCENTER|GUI_TA_LEFT);
    LISTVIEW_AddColumn(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW_REMOVEABLE_FILE),38,"Size",GUI_TA_VCENTER|GUI_TA_LEFT);
    //
    //GUI_ID_RADIO_SEL_MODE
    //
    RADIO_SetGroupId(WM_GetDialogItem(hWin,GUI_ID_RADIO_SEL_MODE),0);
    RADIO_SetText(WM_GetDialogItem(hWin,GUI_ID_RADIO_SEL_MODE),"Single",0);
    RADIO_SetText(WM_GetDialogItem(hWin,GUI_ID_RADIO_SEL_MODE),"Multi",1);
    RADIO_SetText(WM_GetDialogItem(hWin,GUI_ID_RADIO_SEL_MODE),"All",2);
    RADIO_SetValue(WM_GetDialogItem(hWin,GUI_ID_RADIO_SEL_MODE),0);

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
            SelectFile_PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            SelectFile_InitDialog(pMsg);
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
                case GUI_ID_BTN_CHOOSE4USE:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnSelectAllClicked(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BTN_CANCEL_SEL:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnSaveClicked(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_CLOSE:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            //OnBtnExitClicked(pMsg);
							GUI_EndDialog(hWin, 0);
                            break;
                    }
                    break;
                case GUI_ID_LISTVIEW_LOCAL_FILE:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_SEL_CHANGED:
                            OnListViewSelChanged(pMsg);
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

GUI_HWIN App_CreateSelectFile(GUI_HWIN hParent, int x0, int y0)
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate),  &_cbCallback, hParent, x0, y0);
	return hWin;
}
