//ControlCAN.h@DSP Studio

#if !defined(__CONTROL__CAN__H__)
#define __CONTROL__CAN__H__

#include <Windows.h>

//接口卡类型定义
#define DEV_USBCAN			3	//单通道USB-CAN适配器
#define DEV_USBCAN2			4	//双通道USB-CAN适配器

//函数调用返回状态值
#define	STATUS_OK			1	//操作成功
#define STATUS_ERR1			0	//操作失败
#define STATUS_ERR2			-1	//设备未打开

/*------------兼容ZLG的函数及数据类型---------------------------------*/

//USB-CAN总线适配器板卡信息的数据类型。
typedef  struct  _VCI_BOARD_INFO{
	USHORT	hw_Version;
	USHORT	fw_Version;
	USHORT	dr_Version;
	USHORT	in_Version;
	USHORT	irq_Num;
	BYTE	can_Num;
	CHAR	str_Serial_Num[20];
	CHAR	str_hw_Type[40];
	USHORT	Reserved[4];
} VCI_BOARD_INFO,*PVCI_BOARD_INFO;


//定义CAN信息帧的数据类型。
typedef  struct  _VCI_CAN_OBJ{
	UINT	ID;
	UINT	TimeStamp;	 			//时间标识
	BYTE	TimeFlag;					//是否使用时间标识
	BYTE	SendType;					//发送标志。保留，未用
	BYTE	RemoteFlag;				//是否是远程帧
	BYTE	ExternFlag;				//是否是扩展帧
	BYTE	DataLen;
	BYTE	Data[8];
	BYTE	Reserved[3];			//保留
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//定义CAN控制器状态的数据类型。
typedef struct _VCI_CAN_STATUS{
	UCHAR	ErrInterrupt;
	UCHAR	regMode;
	UCHAR	regStatus;
	UCHAR	regALCapture;
	UCHAR	regECCapture; 
	UCHAR	regEWLimit;
	UCHAR	regRECounter; 
	UCHAR	regTECounter;
	DWORD	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//定义错误信息的数据类型。
typedef struct _ERR_INFO{
	UINT	ErrCode;
	BYTE	Passive_ErrData[3];
	BYTE	ArLost_ErrData;
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//定义初始化CAN的数据类型
typedef struct _INIT_CONFIG{
	DWORD	AccCode;
	DWORD	AccMask;
	DWORD	InitFlag;
	UCHAR	Filter;   //0,1接收所有帧。2标准帧滤波，3是扩展帧滤波。
	UCHAR	Timing0;
	UCHAR	Timing1;
	UCHAR	Mode;     //模式，0表示正常模式，1表示只听模式,2自测模式
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

DWORD __stdcall VCI_OpenDevice(DWORD DevType,DWORD DevIndex,DWORD Reserved);
DWORD __stdcall VCI_CloseDevice(DWORD DevType,DWORD DevIndex);
DWORD __stdcall VCI_InitCAN(DWORD DevType, DWORD DevIndex, DWORD CANIndex, PVCI_INIT_CONFIG pInitConfig);

DWORD __stdcall VCI_ReadBoardInfo(DWORD DevType,DWORD DevIndex,PVCI_BOARD_INFO pInfo);
DWORD __stdcall VCI_ReadErrInfo(DWORD DevType,DWORD DevIndex,DWORD CANIndex,PVCI_ERR_INFO pErrInfo);
DWORD __stdcall VCI_ReadCANStatus(DWORD DevType,DWORD DevIndex,DWORD CANIndex,PVCI_CAN_STATUS pCANStatus);

DWORD __stdcall VCI_GetReference(DWORD DevType,DWORD DevIndex,DWORD CANIndex,DWORD Reserved,BYTE *pData);
DWORD __stdcall VCI_SetReference(DWORD DevType,DWORD DevIndex,DWORD CANIndex,DWORD RefType,BYTE *pData);

DWORD __stdcall VCI_GetReceiveNum(DWORD DevType,DWORD DevIndex,DWORD CANIndex);
DWORD __stdcall VCI_ClearBuffer(DWORD DevType,DWORD DevIndex,DWORD CANIndex);

DWORD __stdcall VCI_StartCAN(DWORD DevType,DWORD DevIndex,DWORD CANIndex);
DWORD __stdcall VCI_ResetCAN(DWORD DevType,DWORD DevIndex,DWORD CANIndex);

DWORD __stdcall VCI_Receive(DWORD DevType,DWORD DevIndex,DWORD CANIndex,PVCI_CAN_OBJ pReceive,ULONG Len,INT WaitTime);
DWORD __stdcall VCI_Transmit(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pSend,DWORD Length);

/*------------其他函数及数据结构描述---------------------------------*/

//USB-CAN总线适配器板卡信息的数据类型1，该类型为VCI_FindUsbDevice函数的返回参数。
typedef  struct  _VCI_BOARD_INFO1{
	USHORT	hw_Version;
	USHORT	fw_Version;
	USHORT	dr_Version;
	USHORT	in_Version;
	USHORT	irq_Num;
	BYTE	can_Num;
	BYTE	Reserved;
	CHAR	str_Serial_Num[8];
	CHAR	str_hw_Type[16];
	CHAR	str_Usb_Serial[4][4];
} VCI_BOARD_INFO1,*PVCI_BOARD_INFO1;

//定义常规参数类型
typedef struct _REF_NORMAL{
	BYTE Mode;				//工作模式
	BYTE Filter;			//滤波方式
	DWORD AccCode;			//接收滤波验收码
	DWORD AccMask;			//接收滤波屏蔽码
	BYTE kBaudRate;			//波特率索引号，0-SelfDefine,1-5Kbps(未用),2-18依次为：10kbps,20kbps,40kbps,50kbps,80kbps,100kbps,125kbps,200kbps,250kbps,400kbps,500kbps,666kbps,800kbps,1000kbps,33.33kbps,66.66kbps,83.33kbps
	BYTE Timing0;
	BYTE Timing1;
	BYTE CANRX_EN;			//保留，未用
	BYTE UARTBAUD;			//保留，未用
}VCI_REF_NORMAL,*PVCI_REF_NORMAL;	

//定义波特率设置参数类型
typedef struct _BAUD_TYPE{
	DWORD Baud;				//存储波特率实际值
	BYTE SJW;				//同步跳转宽度，取值1-4
	BYTE BRP;				//预分频值，取值1-64
	BYTE SAM;				//采样点，取值0=采样一次，1=采样三次
	BYTE PHSEG2_SEL;		//相位缓冲段2选择位，取值0=由相位缓冲段1时间决定,1=可编程
	BYTE PRSEG;				//传播时间段，取值1-8
	BYTE PHSEG1;			//相位缓冲段1，取值1-8
	BYTE PHSEG2;			//相位缓冲段2，取值1-8
}VCI_BAUD_TYPE,*PVCI_BAUD_TYPE;

//定义Reference参数类型
typedef struct _REF_STRUCT{
	VCI_REF_NORMAL RefNormal;
	BYTE Reserved;
	VCI_BAUD_TYPE BaudType;
}VCI_REF_STRUCT,*PVCI_REF_STRUCT; 

DWORD __stdcall VCI_GetReference2(DWORD DevType,DWORD DevIndex,DWORD CANIndex,DWORD Reserved,PVCI_REF_STRUCT pRefStruct);
DWORD __stdcall VCI_SetReference2(DWORD DevType,DWORD DevIndex,DWORD CANIndex,DWORD RefType,PVOID *pData);
DWORD __stdcall VCI_ResumeConfig(DWORD DevType,DWORD DevIndex,DWORD CANIndex);

DWORD __stdcall VCI_ConnectDevice(DWORD DevType,DWORD DevIndex);
DWORD __stdcall VCI_UsbDeviceReset(DWORD DevType,DWORD DevIndex,DWORD Reserved);
DWORD __stdcall VCI_FindUsbDevice(PVCI_BOARD_INFO1 pInfo);

#endif //#define __CONTROL__CAN__H__