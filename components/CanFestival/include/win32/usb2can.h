#ifndef __USB2CAN_H__
#define __USB2CAN_H__

#include "canfestival.h"
#include "timer.h"
#include "timers_driver.h"

#ifdef __cplusplus
extern "C" {
#endif
UNS8  canReceive_driver(CAN_HANDLE inst, Message *m);

UNS8  canSend_driver(CAN_HANDLE inst, const Message *m);

CAN_HANDLE  canOpen_driver(s_BOARD *board);

int  canClose_driver(CAN_HANDLE inst);

UNS8  canChangeBaudRate_driver(CAN_HANDLE fd, char* baud);

#ifdef __cplusplus
};
#endif


#endif