#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"

extern FRAMEWIN_Handle _hFrame;
extern GUI_HWIN App_CreateSetting(GUI_HWIN hParent, int x0, int y0);
extern GUI_HWIN App_CreateTesting(GUI_HWIN hParent, int x0, int y0);
extern GUI_HWIN App_CreateSelectFile(GUI_HWIN hParent, int x0, int y0);
extern GUI_HWIN App_CreatePattenView(GUI_HWIN hParent, int x0, int y0);


void OnButtonClicked(WM_MESSAGE * pMsg)
{
}

void OnBtnPattenClicked(WM_MESSAGE * pMsg)
{
	WM_HWIN hFrame = App_CreateSelectFile(WM_HBKWIN, 0, 0);
	WM_SetFocus(hFrame);
	WM_MakeModal(hFrame);
	//GUI_ExecCreatedDialog(hFrame);
	//WM_SetFocus(_hFrame);
}

void OnBtnSettingClicked(WM_MESSAGE * pMsg)
{
	WM_HWIN hFrame = App_CreateSetting(WM_HBKWIN, 0, 0);
	WM_SetFocus(hFrame);
	WM_MakeModal(hFrame);
	//GUI_ExecCreatedDialog(hFrame);
	//WM_SetFocus(_hFrame);
}

void OnBtnMovingClicked(WM_MESSAGE * pMsg)
{
	WM_HWIN hFrame = App_CreateSetting(WM_HBKWIN, 0, 0);
	WM_SetFocus(hFrame);
	WM_MakeModal(hFrame);
	//GUI_ExecCreatedDialog(hFrame);
	//WM_SetFocus(_hFrame);
}

void OnBtnTestingClicked(WM_MESSAGE * pMsg)
{
	WM_HWIN hFrame = App_CreateTesting(WM_HBKWIN, 0, 0);
	WM_SetFocus(hFrame);
	WM_MakeModal(hFrame);
	//GUI_ExecCreatedDialog(hFrame);
	//WM_SetFocus(_hFrame);
}

void OnBtnViewClicked(WM_MESSAGE * pMsg)
{
	WM_HWIN hFrame = App_CreatePattenView(WM_HBKWIN, 0, 0);
	WM_SetFocus(hFrame);
	WM_MakeModal(hFrame);
	//GUI_ExecCreatedDialog(hFrame);
	//WM_SetFocus(_hFrame);
}

void OnBtnManagementClicked(WM_MESSAGE * pMsg)
{
	WM_HWIN hFrame = App_CreateSelectFile(WM_HBKWIN, 0, 0);
	WM_SetFocus(hFrame);
	WM_MakeModal(hFrame);
	//GUI_ExecCreatedDialog(hFrame);
	//WM_SetFocus(_hFrame);
}

void OnBtnRunClicked(WM_MESSAGE * pMsg)
{
}

