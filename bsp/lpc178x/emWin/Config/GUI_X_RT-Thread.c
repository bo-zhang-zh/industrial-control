/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.16 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUI_X_RT-thread.c
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include <stdio.h>
#include "rtthread.h"               /* RT-thread include */
#include "GUI.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static struct rt_mutex gui_mtx;
static struct rt_event gui_evt;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*      Timing:
*                 GUI_GetTime()
*                 GUI_Delay(int)

  Some timing dependent routines require a GetTime
  and delay funtion. Default time unit (tick), normally is
  1 ms.
*/

int GUI_X_GetTime(void) {
  return rt_tick_get();
}

void GUI_X_Delay(int Period) {
  rt_thread_delay(Period);
}

/*********************************************************************
*
*       GUI_X_ExecIdle()
*
*/
void GUI_X_ExecIdle(void) {
  rt_thread_delay(1);
}

/*********************************************************************
*
*      Multitasking:
*
*                 GUI_X_InitOS()
*                 GUI_X_GetTaskId()
*                 GUI_X_Lock()
*                 GUI_X_Unlock()
*
* Note:
*   The following routines are required only if emWin is used in a
*   true multi task environment, which means you have more than one
*   thread using the emWin API.
*   In this case the
*                       #define GUI_OS 1
*  needs to be in GUIConf.h
*/

void GUI_X_InitOS(void)    { rt_mutex_init(&gui_mtx, "guimtx", RT_IPC_FLAG_FIFO);    
                             rt_event_init(&gui_evt, "guievt", RT_IPC_FLAG_FIFO);}
void GUI_X_Unlock(void)    { rt_mutex_release(&gui_mtx); }
void GUI_X_Lock(void)      { rt_mutex_take(&gui_mtx, 0);  }
U32  GUI_X_GetTaskId(void) { return (U32)rt_thread_self(); }


/*********************************************************************
*
*      Event driving (optional with multitasking)
*
*                 GUI_X_WaitEvent()
*                 GUI_X_WaitEventTimed()
*                 GUI_X_SignalEvent()
*/
rt_uint32_t  temp;
void GUI_X_WaitEvent(void)    {
  rt_event_recv(&gui_evt, 0x0001, RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &temp);
}

void GUI_X_SignalEvent(void)    {
    rt_event_send(&gui_evt, 0x0001);
}

void gui_timerout(void *arg)
{
  GUI_X_SignalEvent();
}

void GUI_X_WaitEventTimed(int Period) {
  static struct rt_timer gui_timer;
  static int Initialized = 0;
  
  if (Period > 0) {
    if (Initialized == 0) {
      rt_timer_init(&gui_timer,	"guitimer", gui_timerout, NULL, Period, RT_TIMER_FLAG_ONE_SHOT);
    }
    Initialized = 1;
    rt_timer_start(&gui_timer);
    GUI_X_WaitEvent();
  }
}

/*********************************************************************
*
*      Keyboard input from serial input
*
* Note:
*   If embOS is used, characters typed into the log window will be placed
*   in the keyboard buffer. This is a neat feature which allows you to
*   operate your target system without having to use or even to have
*   a keyboard connected to it. (nice for demos !)
*/


void GUI_X_Init(void) {
  GUI_SetWaitEventFunc(GUI_X_WaitEvent);
  GUI_SetSignalEventFunc(GUI_X_SignalEvent);
  GUI_SetWaitEventTimedFunc(GUI_X_WaitEventTimed);
}

/*********************************************************************
*
*      Logging: OS dependent

Note:
  Logging is used in higher debug levels only. The typical target
  build does not use logging and does therefor not require any of
  the logging routines below. For a release build without logging
  the routines below may be eliminated to save some space.
  (If the linker is not function aware and eliminates unreferenced
  functions automatically)

*/
#define OS_SendString(s) GUI_USE_PARA(s)
void GUI_X_Log     (const char *s) { OS_SendString(s); }
void GUI_X_Warn    (const char *s) { OS_SendString(s); }
void GUI_X_ErrorOut(const char *s) { OS_SendString(s); }

/*************************** End of file ****************************/
