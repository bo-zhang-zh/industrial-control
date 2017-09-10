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


#include "PattenProc.h"


//EventsFunctionList
void OnBtnScaleRecoverClicked(WM_MESSAGE * pMsg);
void OnBtnScaleSubtractClicked(WM_MESSAGE * pMsg);
void OnBtnScaleAddClicked(WM_MESSAGE * pMsg);
//EndofEventsFunctionList


/*********************************************************************
*
*       static data
*
**********************************************************************
*/

#define GUI_ID_TEXT_POS_L   GUI_ID_USER+1
#define GUI_ID_TEXT_POS_R   GUI_ID_USER+2
#define GUI_ID_TEXT_POS_U   GUI_ID_USER+3
#define GUI_ID_TEXT_POS_D   GUI_ID_USER+4
#define GUI_ID_TEXT_SHOW_SCALE   GUI_ID_USER+5
#define GUI_ID_TEXT_OFFSET   GUI_ID_USER+6
#define GUI_ID_TEXT_PANE   GUI_ID_USER+7
#define GUI_ID_BTN_SCALE_ADD   GUI_ID_USER+8
#define GUI_ID_BTN_SCALE_SUBTRACT   GUI_ID_USER+9
#define GUI_ID_BTN_SCALE_RECOVER   GUI_ID_USER+10


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "Patten Viewing",    0,                       0,  0,  600,400,FRAMEWIN_CF_MOVEABLE,0},
    { TEXT_CreateIndirect,      "Patten Position:",  NULL,                    396,8,  103,17, 0,0},
    { TEXT_CreateIndirect,      "Left:",             NULL,                    413,25, 35, 16, 0,0},
    { TEXT_CreateIndirect,      "10000",             GUI_ID_TEXT_POS_L,       444,25, 42, 16, 0,0},
    { TEXT_CreateIndirect,      "Right:",            NULL,                    492,25, 42, 16, 0,0},
    { TEXT_CreateIndirect,      "15000",             GUI_ID_TEXT_POS_R,       531,25, 40, 16, 0,0},
    { TEXT_CreateIndirect,      "Up:",               NULL,                    413,41, 31, 16, 0,0},
    { TEXT_CreateIndirect,      "10000",             GUI_ID_TEXT_POS_U,       444,41, 36, 16, 0,0},
    { TEXT_CreateIndirect,      "Down:",             NULL,                    492,41, 36, 16, 0,0},
    { TEXT_CreateIndirect,      "12000",             GUI_ID_TEXT_POS_D,       531,41, 36, 16, 0,0},
    { TEXT_CreateIndirect,      "Show Scale:",       NULL,                    396,74, 72, 15, 0,0},
    { TEXT_CreateIndirect,      "1.0",               GUI_ID_TEXT_SHOW_SCALE,  492,74, 58, 13, 0,0},
    { TEXT_CreateIndirect,      "Patten offset:",    NULL,                    396,103,103,18, 0,0},
    { TEXT_CreateIndirect,      "0,0",               GUI_ID_TEXT_OFFSET,      492,103,78, 17, 0,0},
    { TEXT_CreateIndirect,      "A pane:",           NULL,                    396,130,47, 16, 0,0},
    { TEXT_CreateIndirect,      "100",               GUI_ID_TEXT_PANE,        456,130,30, 17, 0,0},
    { TEXT_CreateIndirect,      "mm",                NULL,                    488,130,23, 18, 0,0},
    { TEXT_CreateIndirect,      "Show Scale regulation:",NULL,                    396,174,138,18, 0,0},
    { BUTTON_CreateIndirect,    "Add 0.5",           GUI_ID_BTN_SCALE_ADD,    399,198,87, 32, 0,0},
    { BUTTON_CreateIndirect,    "Subtract 0.5",      GUI_ID_BTN_SCALE_SUBTRACT,492,198,87, 32, 0,0},
    { BUTTON_CreateIndirect,    "Recover",           GUI_ID_BTN_SCALE_RECOVER,492,245,87, 32, 0,0},
    { BUTTON_CreateIndirect,    "Close",             GUI_ID_CLOSE,            492,329,87, 32, 0,0}
};

//WM_HWIN hCanvas;
GUI_RECT pattenRect;


/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

void PattenView_PaintDialog(WM_MESSAGE * pMsg)
{
	GUI_RECT border;
	float factor;
	PattenFragment_t frag[6]={{PATTEN_LINE, 1, 4952, 20009, 15028, 20009, 0, 0},
                            {PATTEN_LINE, 1, 15028, 20009, 10019, 25019, 0, 0},
                            {PATTEN_LINE, 1, 10019, 25019, 4952, 20009, 0, 0},
                            {PATTEN_ARC, 1, 4952, 20009, 5009, 19174, 9990, 19933},
                            {PATTEN_DASH, 1, 5009, 19174, 10019, 25019, 0, 0},
                            {PATTEN_DASH, 1, 10019, 25019, 10019, 14942, 0, 0}};
	
    WM_HWIN hWin = pMsg->hWin;
	//if (hWin == hCanvas)
	{
		//WM_SelectWindow(hCanvas);
		GUI_SetColor(GUI_WHITE);
		GUI_FillRectEx(&pattenRect);
		GUI_SetColor(GUI_BLUE);
		//GUI_DrawCircle(100, 100, 180);
		//GUI_DrawArc(100, 100, 90, 90, -60, -10);
		//GUI_Clear();
		CalculateBoundary(frag, 6, &border);
		MoveToOrigin(frag, 6, &border);
		factor = CalculateMagFactor(&pattenRect, &border);
		MagnifyPatten(frag, 6, factor);
		ChangePosition(frag, 6, &pattenRect, 0, 0);
		DrawPatten(frag, 6);
	}

}



/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void PattenView_InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);

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
            PattenView_PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            PattenView_InitDialog(pMsg);
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
                case GUI_ID_BTN_SCALE_ADD:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnScaleAddClicked(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BTN_SCALE_SUBTRACT:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnScaleSubtractClicked(pMsg);
                            break;
                    }
                    break;
                case GUI_ID_BTN_SCALE_RECOVER:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            OnBtnScaleRecoverClicked(pMsg);
                            break;
                    }
                    break;
				case GUI_ID_CLOSE:
					switch(NCode)
					{
					case WM_NOTIFICATION_CLICKED:
						GUI_EndDialog(hWin, 0);
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

GUI_HWIN App_CreatePattenView(GUI_HWIN hParent, int x0, int y0)
{
	WM_HWIN hWin, hClient;
	WM_HWIN hBtn;
	GUI_RECT viewRect, addBtnRect;
	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate),  &_cbCallback, hParent, x0, y0);
	
	WM_GetClientRectEx(hWin, &viewRect);
	//hBtn = WM_GetDialogItem(hWin, GUI_ID_BTN_SCALE_ADD);
	//WM_GetClientRectEx(hBtn, &addBtnRect);
	//hClient = WM_GetClientWindow(hWin);
	pattenRect.x0 = 3;
	pattenRect.y0 = 3;
	pattenRect.x1 = viewRect.x1-220;
	pattenRect.y1 = viewRect.y1-20;
	//hCanvas = WM_CreateWindowAsChild(3, 3, viewRect.x1-220, viewRect.y1-8, 
	//                                 hClient, WM_CF_SHOW | WM_CF_STAYONTOP, 0, 0);
	//WM_SelectWindow(hCanvas);
  //GUI_SetBkColor(GUI_WHITE);
  //GUI_Clear();
	return hWin;
}
