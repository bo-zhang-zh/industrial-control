//
// Touch screen
//
#include "GUI.h"
#include "rtthread.h"
#include "TouchPanel.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
#ifndef _WINDOWS
#if GUI_SUPPORT_TOUCH  // Used when touch screen support is enabled

static U8 _HasTouch;
static U16 _Touch_x;
static U16 _Touch_y;
/*********************************************************************
*
*       _TouchTask()
*
* Function decription:
*   Handles touch screen
*/
void _TouchTask(void * arg)
{
	Coordinate * Ptr;	
	
	TP_Init();

    while (1) {
		rt_thread_delay(10);
		Ptr = Read_Ads7846();
		
		if (Ptr != (void*)0)
		{
			_HasTouch = 1;//      1, if the touch screen is pressed
			_Touch_x = Ptr->x;
			_Touch_y = Ptr->y;
		}
		else
		{
			_HasTouch = 0;//      0, if the touch screen is released
		}
		
    GUI_TOUCH_Exec();
    }
}

#endif  // GUI_SUPPORT_TOUCH
#endif
/*********************************************************************
*
*       Global functions for GUI touch
*
**********************************************************************
*/
#ifndef _WINDOWS
#if GUI_SUPPORT_TOUCH  // Used when touch screen support is enabled
/*********************************************************************
*
*       GUI_TOUCH_X_ActivateX()4
*
* Function decription:
*   Called from GUI, if touch support is enabled.
*   Switches on voltage on X-axis,
*   prepares measurement for Y-axis.
*   Voltage on Y-axis is switched off.
*/
void GUI_TOUCH_X_ActivateX(void) {

}

/*********************************************************************
*
*       GUI_TOUCH_X_ActivateY()2
*
* Function decription:
*   Called from GUI, if touch support is enabled.
*   Switches on voltage on Y-axis,
*   prepares measurement for X-axis.
*   Voltage on X-axis is switched off.
*/
void GUI_TOUCH_X_ActivateY(void) {

}

/*********************************************************************
*
*       GUI_TOUCH_X_MeasureX()3
*
* Function decription:
*   Called from GUI, if touch support is enabled.
*   Measures voltage of X-axis.
*/
int  GUI_TOUCH_X_MeasureX(void) {
	if (_HasTouch)
	  return _Touch_x;
	else
		return -1;
}

/*********************************************************************
*
*       GUI_TOUCH_X_MeasureY()1
*
* Function decription:
*   Called from GUI, if touch support is enabled.
*   Measures voltage of Y-axis.
*/
int  GUI_TOUCH_X_MeasureY(void) {
	if (_HasTouch)
	  return _Touch_y;
	else
		return -1;
}

#endif  // GUI_SUPPORT_TOUCH
#endif
