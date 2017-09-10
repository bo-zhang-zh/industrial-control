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
void OnBtnManMoveDownClicked(WM_MESSAGE * pMsg);
void OnBtnManMoveDownReleased(WM_MESSAGE * pMsg);
void OnBtnManMoveUpClicked(WM_MESSAGE * pMsg);
void OnBtnManMoveUpReleased(WM_MESSAGE * pMsg);
void OnBtnManMoveRightClicked(WM_MESSAGE * pMsg);
void OnBtnManMoveRightReleased(WM_MESSAGE * pMsg);
void OnBtnManMoveLeftClicked(WM_MESSAGE * pMsg);
void OnBtnManMoveLeftReleased(WM_MESSAGE * pMsg);
//EndofEventsFunctionList


/*********************************************************************
*
*       static data
*
**********************************************************************
*/

#define GUI_ID_EDIT_SEWING_SPEED   GUI_ID_USER+1
#define GUI_ID_EDIT_STITCH_LENGTH   GUI_ID_USER+2
#define GUI_ID_EDIT_TIGHTEN_DST   GUI_ID_USER+3
#define GUI_ID_EDIT_LEFT_LIMIT   GUI_ID_USER+4
#define GUI_ID_EDIT_RIGHT_LIMIT   GUI_ID_USER+5
#define GUI_ID_EDIT_UP_LIMIT   GUI_ID_USER+6
#define GUI_ID_EDIT_DOWN_LIMIT   GUI_ID_USER+7
#define GUI_ID_EDIT_OPX   GUI_ID_USER+8
#define GUI_ID_EDIT_OPY   GUI_ID_USER+9
#define GUI_ID_BTN_MOVE_RIGHT   GUI_ID_USER+10
#define GUI_ID_BTN_MOVE_LEFT   GUI_ID_USER+11
#define GUI_ID_EDIT_LEFT_SPACE   GUI_ID_USER+12
#define GUI_ID_EDIT_RIGHT_SPACE   GUI_ID_USER+13
#define GUI_ID_EDIT_UP_SPACE   GUI_ID_USER+14
#define GUI_ID_EDIT_DOWN_SPACE   GUI_ID_USER+15
#define GUI_ID_BTN_GO_ORG   GUI_ID_USER+16
#define GUI_ID_TEXT_PATTEN_SIZE   GUI_ID_USER+17
#define GUI_ID_TEXT_PATTEN_SCALE   GUI_ID_USER+18
#define GUI_ID_BTN_MAN_MOVE_L   GUI_ID_USER+19
#define GUI_ID_BTN_MAN_MOVE_R   GUI_ID_USER+20
#define GUI_ID_BTN_MAN_MOVE_U   GUI_ID_USER+21
#define GUI_ID_BTN_MAN_MOVE_D   GUI_ID_USER+22
#define GUI_ID_DROPDOWN_MOVE_SPEED  GUI_ID_USER+23
#define GUI_ID_DROPDOWN_PATTEN_DIR  GUI_ID_USER+24


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "Setting",           0,                       0,  0,  400,320,FRAMEWIN_CF_MOVEABLE,0},
    { TEXT_CreateIndirect,      "Sewing speed:",     NULL,                    6,  4,  83, 15, 0,0},
    { TEXT_CreateIndirect,      "s.p.m",             NULL,                    181,4,  37, 15, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_SEWING_SPEED,121,1,  55, 16, 0,0},
    { TEXT_CreateIndirect,      "Stitch length:",    NULL,                    6,  22, 89, 15, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_STITCH_LENGTH,121,19, 55, 14, 0,0},
    { TEXT_CreateIndirect,      "mm",                NULL,                    181,22, 21, 15, 0,0},
    { TEXT_CreateIndirect,      "Tighten distance:", NULL,                    4,  39, 115,15, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_TIGHTEN_DST, 121,36, 55, 17, 0,0},
    { TEXT_CreateIndirect,      "mm",                NULL,                    181,39, 25, 15, 0,0},
    { TEXT_CreateIndirect,      "Patten Direction:", NULL,                    6,  58, 113,15, 0,0},
    { TEXT_CreateIndirect,      "Limit range and outpoint",NULL,                    7,  88, 152,19, 0,0},
    { TEXT_CreateIndirect,      "Left Limit:",       NULL,                    7,  107,75, 17, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_LEFT_LIMIT,  90, 104,59, 20, 0,0},
    { TEXT_CreateIndirect,      "Right Limit:",      NULL,                    7,  132,77, 17, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_RIGHT_LIMIT, 90, 129,59, 20, 0,0},
    { TEXT_CreateIndirect,      "Up Limit:",         NULL,                    7,  159,60, 17, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_UP_LIMIT,    90, 154,59, 20, 0,0},
    { TEXT_CreateIndirect,      "Down Limit:",       NULL,                    7,  182,75, 17, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_DOWN_LIMIT,  90, 179,59, 20, 0,0},
    { TEXT_CreateIndirect,      "Outpoint: X:",      NULL,                    7,  207,77, 17, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_OPX,         90, 204,59, 20, 0,0},
    { TEXT_CreateIndirect,      "Y:",                NULL,                    68, 234,21, 17, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_OPY,         90, 229,59, 20, 0,0},
    { BUTTON_CreateIndirect,    "MOVE >>",           GUI_ID_BTN_MOVE_RIGHT,   155,132,56, 38, 0,0},
    { BUTTON_CreateIndirect,    "MOVE <<",           GUI_ID_BTN_MOVE_LEFT,    155,176,55, 38, 0,0},
    { TEXT_CreateIndirect,      "Space of Patten",   NULL,                    224,116,115,16, 0,0},
    { TEXT_CreateIndirect,      "Left Space:",       NULL,                    232,132,72, 13, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_LEFT_SPACE,  310,129,50, 20, 0,0},
    { TEXT_CreateIndirect,      "Right Space:",      NULL,                    232,157,78, 13, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_RIGHT_SPACE, 310,154,50, 20, 0,0},
    { TEXT_CreateIndirect,      "Up Space:",         NULL,                    232,182,60, 13, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_UP_SPACE,    310,179,50, 20, 0,0},
    { TEXT_CreateIndirect,      "Down Space:",       NULL,                    232,207,72, 13, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT_DOWN_SPACE,  310,204,50, 20, 0,0},
    { BUTTON_CreateIndirect,    "HOME",              GUI_ID_BTN_GO_ORG,       30, 255,75, 38, 0,0},
    { TEXT_CreateIndirect,      "Size:",             NULL,                    232,226,41, 17, 0,0},
    { TEXT_CreateIndirect,      "X:1000 Y:1500",     GUI_ID_TEXT_PATTEN_SIZE, 279,226,90, 15, 0,0},
    { TEXT_CreateIndirect,      "Scale:",            NULL,                    232,243,43, 12, 0,0},
    { TEXT_CreateIndirect,      "X:0.50 Y:0.50",     GUI_ID_TEXT_PATTEN_SCALE,279,243,94, 13, 0,0},
    { TEXT_CreateIndirect,      "Manual Moving",     NULL,                    224,4,  103,15, 0,0},
    { BUTTON_CreateIndirect,    "Left <-",           GUI_ID_BTN_MAN_MOVE_L,   246,22, 60, 27, 0,0},
    { BUTTON_CreateIndirect,    "Right ->",          GUI_ID_BTN_MAN_MOVE_R,   320,22, 60, 27, 0,0},
    { BUTTON_CreateIndirect,    "Up ",              GUI_ID_BTN_MAN_MOVE_U,   246,60, 60, 27, 0,0},
    { BUTTON_CreateIndirect,    "Down ",            GUI_ID_BTN_MAN_MOVE_D,   320,60, 60, 27, 0,0},
    { TEXT_CreateIndirect,      "Move Speed:",       NULL,                    246,92, 72, 15, 0,0},
    { DROPDOWN_CreateIndirect,  "DROPDOWN",          GUI_ID_DROPDOWN_MOVE_SPEED,321,92, 59, 80, 0,0},
    { BUTTON_CreateIndirect,    "OK",                GUI_ID_OK,               217,270,83, 24, 0,0},
    { BUTTON_CreateIndirect,    "CLOSE",             GUI_ID_CLOSE,            310,270,70, 24, 0,0},
    { DROPDOWN_CreateIndirect,  "Forward",           GUI_ID_DROPDOWN_PATTEN_DIR,121,57, 89, 80, 0,0},
    { TEXT_CreateIndirect,      "mm",                NULL,                    363,133,17, 16, 0,0},
    { TEXT_CreateIndirect,      "mm",                NULL,                    363,160,17, 16, 0,0},
    { TEXT_CreateIndirect,      "mm",                NULL,                    363,185,17, 14, 0,0},
    { TEXT_CreateIndirect,      "mm",                NULL,                    362,209,18, 15, 0,0}
};



/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

void Setting_PaintDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    GUI_SetColor(0x000000);
    GUI_DrawLine(219,115,398,115);
    GUI_SetColor(0x000000);
    GUI_DrawLine(219,-1,219,248);
    GUI_SetColor(0x696969);
    GUI_DrawLine(1,82,220,82);

}



/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void Setting_InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    //
    //NULL
    //
//     TEXT_SetTextAlign(WM_GetDialogItem(hWin,NULL),GUI_TA_TOP|GUI_TA_RIGHT);
//     //
//     //NULL
//     //
//     TEXT_SetTextAlign(WM_GetDialogItem(hWin,NULL),GUI_TA_TOP|GUI_TA_RIGHT);
//     //
//     //NULL
//     //
//     TEXT_SetTextAlign(WM_GetDialogItem(hWin,NULL),GUI_TA_TOP|GUI_TA_RIGHT);
    //
    //GUI_ID_DROPDOWN_MOVE_SPEED
    //
    DROPDOWN_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_MOVE_SPEED),0,0xffffff);
    DROPDOWN_SetAutoScroll(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_MOVE_SPEED),1);
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_MOVE_SPEED),"Quickly");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_MOVE_SPEED),"Meidum");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_MOVE_SPEED),"Slowly");
    //
    //GUI_ID_DROPDOWN_PATTEN_DIR
    //
    DROPDOWN_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_PATTEN_DIR),0,0xffffff);
    DROPDOWN_SetAutoScroll(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_PATTEN_DIR),1);
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_PATTEN_DIR),"Forward");
    DROPDOWN_AddString(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_PATTEN_DIR),"Backward");
    DROPDOWN_SetSel(WM_GetDialogItem(hWin,GUI_ID_DROPDOWN_PATTEN_DIR),0);

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
            Setting_PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            Setting_InitDialog(pMsg);
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
                case GUI_ID_BTN_MAN_MOVE_L:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnManMoveLeftClicked(pMsg);
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            OnBtnManMoveLeftReleased(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BTN_MAN_MOVE_R:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnManMoveRightClicked(pMsg);
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            OnBtnManMoveRightReleased(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BTN_MAN_MOVE_U:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnManMoveUpClicked(pMsg);
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            OnBtnManMoveUpReleased(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BTN_MAN_MOVE_D:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnManMoveDownClicked(pMsg);
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            OnBtnManMoveDownReleased(pMsg);
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

GUI_HWIN App_CreateSetting(GUI_HWIN hParent, int x0, int y0)
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate),  &_cbCallback, hParent, x0, y0);
	return hWin;
}
