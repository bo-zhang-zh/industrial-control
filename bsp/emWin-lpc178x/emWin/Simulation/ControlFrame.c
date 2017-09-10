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
void OnBtnRunClicked(WM_MESSAGE * pMsg);
void OnBtnManagementClicked(WM_MESSAGE * pMsg);
void OnBtnViewClicked(WM_MESSAGE * pMsg);
void OnBtnTestingClicked(WM_MESSAGE * pMsg);
void OnBtnMovingClicked(WM_MESSAGE * pMsg);
void OnBtnSettingClicked(WM_MESSAGE * pMsg);
void OnBtnPattenClicked(WM_MESSAGE * pMsg);
//EndofEventsFunctionList


/*********************************************************************
*
*       static data
*
**********************************************************************
*/

#define GUI_ID_BTN_PATTEN   GUI_ID_USER+1
#define GUI_ID_BTN_SETTING   GUI_ID_USER+2
#define GUI_ID_BTN_MOVING   GUI_ID_USER+3
#define GUI_ID_BTN_TESTING   GUI_ID_USER+4
#define GUI_ID_BTN_VIEW   GUI_ID_USER+5
#define GUI_ID_BTN_MGT   GUI_ID_USER+6
#define GUI_ID_BTN_RUN   GUI_ID_USER+7

#define GUI_ID_TEXT_FILE_NAME   GUI_ID_USER+101
#define GUI_ID_TEXT_HANDLES   GUI_ID_USER+102
#define GUI_ID_TEXT_COUNTER   GUI_ID_USER+103
#define GUI_ID_TEXT_STITCH   GUI_ID_USER+104
#define GUI_ID_TEXT_SCALE   GUI_ID_USER+105
#define GUI_ID_TEXT_SIZE   GUI_ID_USER+106
#define GUI_ID_TEXT_POSX   GUI_ID_USER+107
#define GUI_ID_TEXT_POSY   GUI_ID_USER+108
#define GUI_ID_TEXT_DIR   GUI_ID_USER+109
#define GUI_ID_TEXT_ZMOTOR   GUI_ID_USER+110
#define GUI_ID_TEXT_DETECT   GUI_ID_USER+111
#define GUI_ID_TEXT_SPEED   GUI_ID_USER+112


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ FRAMEWIN_CreateIndirect,  "Control",           0,                       0,  0,  300,480,FRAMEWIN_CF_MOVEABLE,0},
	{ BUTTON_CreateIndirect,    "Patten",            GUI_ID_BTN_PATTEN,       8,  6,  79, 23, 0,0},
	{ BUTTON_CreateIndirect,    "Setting",           GUI_ID_BTN_SETTING,      8,  35, 79, 23, 0,0},
	{ BUTTON_CreateIndirect,    "Moving",            GUI_ID_BTN_MOVING,       8,  65, 79, 23, 0,0},
	{ BUTTON_CreateIndirect,    "Testing",           GUI_ID_BTN_TESTING,      8,  93, 79, 23, 0,0},
	{ BUTTON_CreateIndirect,    "View",              GUI_ID_BTN_VIEW,         8,  122,79, 23, 0,0},
	{ BUTTON_CreateIndirect,    "Management",        GUI_ID_BTN_MGT,          8,  151,79, 23, 0,0},
	{ BUTTON_CreateIndirect,    "Run",               GUI_ID_BTN_RUN,          119,7,  77, 21, 0,0},
	{ TEXT_CreateIndirect,      "File Name:",        NULL,                    5,  248,66, 16, 0,0},
	{ TEXT_CreateIndirect,      "xxx.dat",           GUI_ID_TEXT_FILE_NAME,   69, 248,71, 16, 0,0},
	{ TEXT_CreateIndirect,      "Total Handle:",     NULL,                    5,  264,90, 16, 0,0},
	{ TEXT_CreateIndirect,      "1234",              GUI_ID_TEXT_HANDLES,     90, 264,50, 16, 0,0},
	{ TEXT_CreateIndirect,      "Counter:",          NULL,                    5,  279,63, 16, 0,0},
	{ TEXT_CreateIndirect,      "0",                 GUI_ID_TEXT_COUNTER,     64, 279,76, 16, 0,0},
	{ TEXT_CreateIndirect,      "Stitch Length:",    NULL,                    5,  294,93, 16, 0,0},
	{ TEXT_CreateIndirect,      "4.5mm",             GUI_ID_TEXT_STITCH,      103,294,37, 16, 0,0},
	{ TEXT_CreateIndirect,      "Scale:",            NULL,                    5,  308,42, 16, 0,0},
	{ TEXT_CreateIndirect,      "X:0.5  Y:0.5",      GUI_ID_TEXT_SCALE,       52, 308,88, 16, 0,0},
	{ TEXT_CreateIndirect,      "Size:",             NULL,                    5,  321,39, 16, 0,0},
	{ TEXT_CreateIndirect,      "Sx:1000 Sy:1500",   GUI_ID_TEXT_SIZE,        44, 321,96, 16, 0,0},
	{ TEXT_CreateIndirect,      "Pos:",              NULL,                    8,  349,36, 16, 0,0},
	{ TEXT_CreateIndirect,      "X:unKnown",         GUI_ID_TEXT_POSX,        40, 349,58, 17, 0,0},
	{ TEXT_CreateIndirect,      "Y:unKnown",         GUI_ID_TEXT_POSY,        40, 367,58, 17, 0,0},
	{ TEXT_CreateIndirect,      "Direction:",        NULL,                    156,248,65, 16, 0,0},
	{ TEXT_CreateIndirect,      "Forward",           GUI_ID_TEXT_DIR,         227,248,61, 16, 0,0},
	{ TEXT_CreateIndirect,      "Z motor ON/OFF:",   NULL,                    156,264,96, 16, 0,0},
	{ TEXT_CreateIndirect,      "ON",                GUI_ID_TEXT_ZMOTOR,      251,264,37, 16, 0,0},
	{ TEXT_CreateIndirect,      "Thread Detect:",    NULL,                    156,279,93, 16, 0,0},
	{ TEXT_CreateIndirect,      "Yes",               GUI_ID_TEXT_DETECT,      256,279,32, 16, 0,0},
	{ TEXT_CreateIndirect,      "Sewing Speed:",     NULL,                    156,294,93, 16, 0,0},
	{ TEXT_CreateIndirect,      "1500",              GUI_ID_TEXT_SPEED,       241,294,47, 16, 0,0}
};



/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

void PaintDialog(WM_MESSAGE * pMsg)
{
	WM_HWIN hWin = pMsg->hWin;

}



/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void InitDialog(WM_MESSAGE * pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	//
	//FRAMEWIN
	//
	FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
	//
	//GUI_ID_TEXT_FILE_NAME
	//
	TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT_FILE_NAME),GUI_TA_TOP|GUI_TA_RIGHT);
	//
	//GUI_ID_TEXT_HANDLES
	//
	TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT_HANDLES),GUI_TA_TOP|GUI_TA_RIGHT);
	//
	//GUI_ID_TEXT_COUNTER
	//
	TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT_COUNTER),GUI_TA_TOP|GUI_TA_RIGHT);
	//
	//GUI_ID_TEXT_STITCH
	//
	TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT_STITCH),GUI_TA_TOP|GUI_TA_RIGHT);
	//
	//GUI_ID_TEXT_SCALE
	//
	TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT_SCALE),GUI_TA_TOP|GUI_TA_RIGHT);
	//
	//GUI_ID_TEXT_SIZE
	//
	TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT_SIZE),GUI_TA_TOP|GUI_TA_RIGHT);
	//
	//GUI_ID_TEXT_POSX
	//
	TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT_POSX),GUI_TA_TOP|GUI_TA_RIGHT);
	//
	//GUI_ID_TEXT_POSY
	//
	TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT_POSY),GUI_TA_TOP|GUI_TA_RIGHT);
	//
	//GUI_ID_TEXT_DIR
	//
	TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT_DIR),GUI_TA_TOP|GUI_TA_RIGHT);
	//
	//GUI_ID_TEXT_ZMOTOR
	//
	TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT_ZMOTOR),GUI_TA_TOP|GUI_TA_RIGHT);
	//
	//GUI_ID_TEXT_DETECT
	//
	TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT_DETECT),GUI_TA_TOP|GUI_TA_RIGHT);
	//
	//GUI_ID_TEXT_SPEED
	//
	TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT_SPEED),GUI_TA_TOP|GUI_TA_RIGHT);

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
		PaintDialog(pMsg);
		break;
	case WM_INIT_DIALOG:
		InitDialog(pMsg);
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
		case GUI_ID_BTN_PATTEN:
			switch(NCode)
			{
			case WM_NOTIFICATION_CLICKED:
				OnBtnPattenClicked(pMsg);
				break;
			}
			break;
		case GUI_ID_BTN_SETTING:
			switch(NCode)
			{
			case WM_NOTIFICATION_CLICKED:
				OnBtnSettingClicked(pMsg);
				break;
			}
			break;
		case GUI_ID_BTN_MOVING:
			switch(NCode)
			{
			case WM_NOTIFICATION_CLICKED:
				OnBtnMovingClicked(pMsg);
				break;
			}
			break;
		case GUI_ID_BTN_TESTING:
			switch(NCode)
			{
			case WM_NOTIFICATION_CLICKED:
				OnBtnTestingClicked(pMsg);
				break;
			}
			break;
		case GUI_ID_BTN_VIEW:
			switch(NCode)
			{
			case WM_NOTIFICATION_CLICKED:
				OnBtnViewClicked(pMsg);
				break;
			}
			break;
		case GUI_ID_BTN_MGT:
			switch(NCode)
			{
			case WM_NOTIFICATION_CLICKED:
				OnBtnManagementClicked(pMsg);
				break;
			}
			break;
		case GUI_ID_BTN_RUN:
			switch(NCode)
			{
			case WM_NOTIFICATION_CLICKED:
				OnBtnRunClicked(pMsg);
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
// 	GUI_Init();
// 	WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */
// 	WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
// 	//PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
// 	//FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
// 	//PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
// 	//BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
// 	//CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
// 	//DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
// 	//SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
// 	//SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
// 	//HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
// 	//RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
// 	GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
// }

GUI_HWIN App_CreateControlFrame(GUI_HWIN hParent, int x0, int y0)
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate),  &_cbCallback, hParent, x0, y0);
	return hWin;
}
