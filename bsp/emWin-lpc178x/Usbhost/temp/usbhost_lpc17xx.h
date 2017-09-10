/*
**************************************************************************************************************
*                                                 NXP USB Host Stack
*
*                                     (c) Copyright 2008, NXP SemiConductors
*                                     (c) Copyright 2008, OnChip  Technologies LLC
*                                                 All Rights Reserved
*
*                                                  www.nxp.com
*                                               www.onchiptech.com
*
* File           : usbhost_lpc17xx.h
* Programmer(s)  : Ravikanth.P
* Version        :
*
**************************************************************************************************************
*/

#ifndef USBHOST_LPC17xx_H
#define USBHOST_LPC17xx_H

/*
**************************************************************************************************************
*                                       INCLUDE HEADER FILES
**************************************************************************************************************
*/

#include    "usbhost_inc.h"
#include    "rtthread.h"
/*
**************************************************************************************************************
*                                        PRINT CONFIGURATION
**************************************************************************************************************
*/

#define  PRINT_ENABLE         1

#if PRINT_ENABLE
#define  PRINT_Log(...)       rt_kprintf(__VA_ARGS__)
#define  PRINT_Err(rc)        rt_kprintf("ERROR: In %s at Line %u - rc = %d\n", __FUNCTION__, __LINE__, rc)

#else 
#define  PRINT_Log(...)       do {} while(0)
#define  PRINT_Err(rc)        do {} while(0)

#endif

/*
**************************************************************************************************************
*                                        GENERAL DEFINITIONS
**************************************************************************************************************
*/

#define HOST_BASE_ADDR		0x20000000

#define  DESC_LENGTH(x)  x[0]
#define  DESC_TYPE(x)    x[1]


#define  HOST_GET_DESCRIPTOR(descType, descIndex, data, length)                      \
         Host_CtrlRecv(USB_DEVICE_TO_HOST | USB_RECIPIENT_DEVICE, GET_DESCRIPTOR,    \
         (descType << 8)|(descIndex), 0, length, data)

#define  HOST_SET_ADDRESS(new_addr)                                                  \
         Host_CtrlSend(USB_HOST_TO_DEVICE | USB_RECIPIENT_DEVICE, SET_ADDRESS,       \
         new_addr, 0, 0, NULL)

#define  USBH_SET_CONFIGURATION(configNum)                                           \
         Host_CtrlSend(USB_HOST_TO_DEVICE | USB_RECIPIENT_DEVICE, SET_CONFIGURATION, \
         configNum, 0, 0, NULL)

#define  USBH_SET_INTERFACE(ifNum, altNum)                                           \
         Host_CtrlSend(USB_HOST_TO_DEVICE | USB_RECIPIENT_INTERFACE, SET_INTERFACE,  \
         altNum, ifNum, 0, NULL)

/*
**************************************************************************************************************
*                                  OHCI OPERATIONAL REGISTER FIELD DEFINITIONS
**************************************************************************************************************
*/

                                            /* ------------------ HcControl Register ---------------------  */
#define  OR_CONTROL_CLE                 0x00000010
#define  OR_CONTROL_BLE                 0x00000020
#define  OR_CONTROL_HCFS                0x000000C0
#define  OR_CONTROL_HC_OPER             0x00000080
                                            /* ----------------- HcCommandStatus Register ----------------- */
#define  OR_CMD_STATUS_HCR              0x00000001
#define  OR_CMD_STATUS_CLF              0x00000002
#define  OR_CMD_STATUS_BLF              0x00000004
                                            /* --------------- HcInterruptStatus Register ----------------- */
#define  OR_INTR_STATUS_WDH             0x00000002
#define  OR_INTR_STATUS_UE              (0x1<<4)
#define  OR_INTR_STATUS_RHSC            0x00000040
                                            /* --------------- HcInterruptEnable Register ----------------- */
#define  OR_INTR_ENABLE_WDH             0x00000002
#define  OR_INTR_ENABLE_UE              (0x1<<4)
#define  OR_INTR_ENABLE_RHSC            0x00000040
#define  OR_INTR_ENABLE_MIE             0x80000000
                                            /* ---------------- HcRhDescriptorA Register ------------------ */
#define  OR_RH_STATUS_LPSC              0x00010000
#define  OR_RH_STATUS_DRWE              0x00008000
                                            /* -------------- HcRhPortStatus[1:NDP] Register -------------- */
#define  OR_RH_PORT_CCS                 0x00000001
#define  OR_RH_PORT_PRS                 0x00000010
#define  OR_RH_PORT_CSC                 0x00010000
#define  OR_RH_PORT_PRSC                0x00100000


/*
**************************************************************************************************************
*                                               FRAME INTERVAL
**************************************************************************************************************
*/

#define  FI                     0x2EDF           /* 12000 bits per frame (-1)                               */
#define  DEFAULT_FMINTERVAL     ((((6 * (FI - 210)) / 7) << 16) | FI)

/*
**************************************************************************************************************
*                                       TRANSFER DESCRIPTOR CONTROL FIELDS
**************************************************************************************************************
*/

#define  TD_ROUNDING        (uint32_t)(0x00040000)        /* Buffer Rounding                             */
#define  TD_SETUP           (uint32_t)(0)                  /* Direction of Setup Packet                   */
#define  TD_IN              (uint32_t)(0x00100000)         /* Direction In                                */
#define  TD_OUT             (uint32_t)(0x00080000)         /* Direction Out                               */
#define  TD_DELAY_INT(x)    (uint32_t)((x) << 21)          /* Delay Interrupt                             */
#define  TD_TOGGLE_0        (uint32_t)(0x02000000)         /* Toggle 0                                    */
#define  TD_TOGGLE_1        (uint32_t)(0x03000000)         /* Toggle 1                                    */
#define  TD_CC              (uint32_t)(0xF0000000)         /* Completion Code                             */

/*
**************************************************************************************************************
*                                       USB STANDARD REQUEST DEFINITIONS
**************************************************************************************************************
*/

#define  USB_DESCRIPTOR_TYPE_DEVICE                     1
#define  USB_DESCRIPTOR_TYPE_CONFIGURATION              2
#define  USB_DESCRIPTOR_TYPE_INTERFACE                  4
#define  USB_DESCRIPTOR_TYPE_ENDPOINT                   5
                                                    /*  ----------- Control RequestType Fields  ----------- */
#define  USB_DEVICE_TO_HOST         0x80
#define  USB_HOST_TO_DEVICE         0x00
#define  USB_REQUEST_TYPE_CLASS     0x20
#define  USB_RECIPIENT_DEVICE       0x00
#define  USB_RECIPIENT_INTERFACE    0x01
                                                    /* -------------- USB Standard Requests  -------------- */
#define  SET_ADDRESS                 5
#define  GET_DESCRIPTOR              6
#define  SET_CONFIGURATION           9
#define  SET_INTERFACE              11

/*
**************************************************************************************************************
*                                       TYPE DEFINITIONS
**************************************************************************************************************
*/

typedef struct hcEd {                       /* ----------- HostController EndPoint Descriptor ------------- */
    volatile  uint32_t  Control;              /* Endpoint descriptor control                              */
    volatile  uint32_t  TailTd;               /* Physical address of tail in Transfer descriptor list     */
    volatile  uint32_t  HeadTd;               /* Physcial address of head in Transfer descriptor list     */
    volatile  uint32_t  Next;                 /* Physical address of next Endpoint descriptor             */
} HCED;

typedef struct hcTd {                       /* ------------ HostController Transfer Descriptor ------------ */
    volatile  uint32_t  Control;              /* Transfer descriptor control                              */
    volatile  uint32_t  CurrBufPtr;           /* Physical address of current buffer pointer               */
    volatile  uint32_t  Next;                 /* Physical pointer to next Transfer Descriptor             */
    volatile  uint32_t  BufEnd;               /* Physical address of end of buffer                        */
} HCTD;

typedef struct hcca {                       /* ----------- Host Controller Communication Area ------------  */
    volatile  uint32_t  IntTable[32];         /* Interrupt Table                                          */
    volatile  uint32_t  FrameNumber;          /* Frame Number                                             */
    volatile  uint32_t  DoneHead;             /* Done Head                                                */
    volatile  uint8_t  Reserved[116];        /* Reserved for future use                                  */
    volatile  uint8_t  Unknown[4];           /* Unused                                                   */
} HCCA;

/*
**************************************************************************************************************
*                                     EXTERN DECLARATIONS
**************************************************************************************************************
*/

extern  volatile  HCED     *EDBulkIn;        /* BulkIn endpoint descriptor  structure                    */
extern  volatile  HCED     *EDBulkOut;       /* BulkOut endpoint descriptor structure                    */
extern  volatile  HCTD     *TDHead;          /* Head transfer descriptor structure                       */
extern  volatile  HCTD     *TDTail;          /* Tail transfer descriptor structure                       */
extern  volatile  uint8_t  *TDBuffer;        /* Current Buffer Pointer of transfer descriptor            */
extern  volatile  uint8_t  *FATBuffer;       /* Buffer used by FAT file system                           */
extern  volatile  uint8_t  *UserBuffer;      /* Buffer used by application                               */

/*
**************************************************************************************************************
*                                       FUNCTION PROTOTYPES
**************************************************************************************************************
*/

void        Host_Init     (void);

void        USB_IRQHandler(void);

int32_t     Host_EnumDev  (void);

int32_t     Host_ProcessTD(volatile  HCED       *ed,
                           volatile  uint32_t  token,
                           volatile  uint8_t *buffer,
                                     uint32_t  buffer_len);

void        Host_DelayUS  (          uint32_t    delay);
void        Host_DelayMS  (          uint32_t    delay);


void        Host_TDInit   (volatile  HCTD *td);
void        Host_EDInit   (volatile  HCED *ed);
void        Host_HCCAInit (volatile  HCCA  *hcca);

int32_t     Host_CtrlRecv (          uint8_t   bm_request_type,
                                     uint8_t   b_request,
                                     uint16_t   w_value,
                                     uint16_t   w_index,
                                     uint16_t   w_length,
                           volatile  uint8_t  *buffer);

int32_t     Host_CtrlSend (          uint8_t   bm_request_type,
                                     uint8_t   b_request,
                                     uint16_t   w_value,
                                     uint16_t   w_index,
                                     uint16_t   w_length,
                           volatile  uint8_t  *buffer);

void        Host_FillSetup(          uint8_t   bm_request_type,
                                     uint8_t   b_request,
                                     uint16_t   w_value,
                                     uint16_t   w_index,
                                     uint16_t   w_length);


void        Host_WDHWait  (void);


uint32_t  ReadLE32U     (volatile  uint8_t  *pmem);
void      WriteLE32U    (volatile  uint8_t  *pmem, uint32_t   val);
uint16_t  ReadLE16U     (volatile  uint8_t  *pmem);
void      WriteLE16U    (volatile  uint8_t  *pmem, uint16_t   val);
uint32_t  ReadBE32U     (volatile  uint8_t  *pmem);
void      WriteBE32U    (volatile  uint8_t  *pmem, uint32_t   val);
uint16_t  ReadBE16U     (volatile  uint8_t  *pmem);
void      WriteBE16U    (volatile  uint8_t  *pmem, uint16_t   val);

#endif
