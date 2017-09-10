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
File    : SysConf.h
Purpose : Configuration of components included in SeggerDemo
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef SYSCONF_H                       /* Avoid multiple inclusion */
#define SYSCONF_H

/*********************************************************************
*
*       GUI task configuration
*
**********************************************************************
*/

#ifndef   USE_GUI_TASK
  #define USE_GUI_TASK  (1)
#endif

/*********************************************************************
*
*       Included middleware components
*
**********************************************************************
*/

#ifndef   INCLUDE_IP
  #define INCLUDE_IP         (1)
#endif
#ifndef   INCLUDE_FTP
  #define INCLUDE_FTP        (1)
#endif
#ifndef   INCLUDE_WEBSERVER
  #define INCLUDE_WEBSERVER  (1)
#endif
#ifndef   INCLUDE_VNC
  #define INCLUDE_VNC        (1)
#endif
#ifndef   INCLUDE_FS
  #define INCLUDE_FS         (1)
#endif
#ifndef   INCLUDE_USB
  #define INCLUDE_USB        (1)
#endif
#ifndef   INCLUDE_USBH
  #define INCLUDE_USBH       (1)
#endif
#ifndef   USE_USB_HOST_HID
  #define USE_USB_HOST_HID   (0)
#endif

/*********************************************************************
*
*       Included GUIDEMO samples
*
**********************************************************************
*/

#ifndef   SHOW_GUIDEMO_BARGRAPH
  #define SHOW_GUIDEMO_BARGRAPH           (0)
#endif
#ifndef   SHOW_GUIDEMO_GRAPH
  #define SHOW_GUIDEMO_GRAPH              (0)
#endif
#ifndef   SHOW_GUIDEMO_LISTVIEW
  #define SHOW_GUIDEMO_LISTVIEW           (1)
#endif
#ifndef   SHOW_GUIDEMO_SKINNING
  #define SHOW_GUIDEMO_SKINNING           (0)
#endif
#ifndef   SHOW_GUIDEMO_SPEED
  #define SHOW_GUIDEMO_SPEED              (1)
#endif
#ifndef   SHOW_GUIDEMO_SPEEDOMETER
  #define SHOW_GUIDEMO_SPEEDOMETER        (0)
#endif
#ifndef   SHOW_GUIDEMO_WASHINGMACHINE
  #if ((INCLUDE_USBH == 0) || (USE_USB_HOST_HID == 0))
    #define SHOW_GUIDEMO_WASHINGMACHINE   (0)  // Include sample only if not using USB host HID as we do not have enough space
  #endif
#endif

/*********************************************************************
*
*       Included defaults for not configured options
*
**********************************************************************
*/

#include "SysConfDefaults.h"

#endif                                  /* Avoid multiple inclusion */

/****** End of File *************************************************/
