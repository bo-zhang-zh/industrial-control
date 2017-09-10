/* Includes ------------------------------------------------------------------*/
#include "lpc177x_8x_can.h"
#include "lpc177x_8x_pinsel.h"
//#include "platform_config.h"

/* test_master */
#include "canfestival.h"
//#include "Master.h"
#include "Slave.h"
#include "TestMasterSlave.h"
#include "Slave_board.h"

#include <rtthread.h>

/** @addtogroup StdPeriph_Examples
  * @{
  */

/** @addtogroup CAN_Normal
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
//#define TRANSMIT_CAN_NO		(CAN_2)
#define NETWORK_CAN_NO		(CAN_1)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//CanTxMsg TxMessage;
//CanRxMsg RxMessage;

/* Private function prototypes -----------------------------------------------*/
static int  test_slave(void);

extern void CAN_Timer_start(void);

//uint32_t LED1_State;

#define 	RX_BUF_LEN		64
ALIGN(RT_ALIGN_SIZE)
Message   rx_msg_buf[RX_BUF_LEN];
int32_t	rx_save, rx_read;

#define TX_BUF_LEN		64
int32_t  tx_save, tx_read;
ALIGN(RT_ALIGN_SIZE)
Message   tx_msg_buf[TX_BUF_LEN];

static rt_sem_t recv_sem = RT_NULL;
static rt_sem_t tran_sem = RT_NULL;

char info_dev[]={"T1EA"};
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  can_slave_thread
  * @param  None
  * @retval : None
  */
void can_slave_init(void)
{
	/* Pin configuration
	 * CAN1: select P0.0 as RD1. P0.1 as TD1
	 * CAN2: select P0.4 as RD2, P0.5 as RD2
	 */

//#if (TRANSMIT_CAN_NO == CAN_1)
	PINSEL_ConfigPin (0, 0, 1);
	PINSEL_ConfigPin (0, 1, 1);
// #else
// 	PINSEL_ConfigPin (0, 4, 2);
// 	PINSEL_ConfigPin (0, 5, 2);
// #endif
	
	//Initialize CAN1 & CAN2
	//CAN_Init(TRANSMIT_CAN_NO, 125000);
	CAN_Init(NETWORK_CAN_NO, 250000);
	
//发送缓冲区的发送次序必须固定（FIFO）
	CAN_ModeConfig(NETWORK_CAN_NO, CAN_TXPRIORITY_MODE, ENABLE);
	
	//Enable Interrupt
	//CAN_IRQCmd(TRANSMIT_CAN_NO, CANINT_TIE1, ENABLE);
	CAN_IRQCmd(NETWORK_CAN_NO, CANINT_RIE, ENABLE);

	//Enable CAN Interrupt
	NVIC_EnableIRQ(CAN_IRQn);
	CAN_SetAFMode(CAN_ACC_BP);
	
	/* transmit queue init */
	tx_save = 0;
	tx_read = 0;
	/* receive  queue init */
	rx_save = 0;
	rx_read = 0;

}

/********************************************************************//**
 * @brief		Send message data
 * @param[in]	 canId			 The Id of the expected CAN component
 *
 * @param[in]	CAN_Msg point to the CAN_MSG_Type Structure, it contains message
 * 				information such as: ID, DLC, RTR, ID Format
 * @return 		Status:
 * 				- SUCCESS: send message successfully
 * 				- ERROR: send message unsuccessfully
 *********************************************************************/
Status CanFestival_SendMsg (uint8_t canId, Message *pmsg)
{
	LPC_CAN_TypeDef* pCan = canId == CAN_ID_1? LPC_CAN1 : LPC_CAN2;//CAN_GetPointer(canId);

	uint32_t data;

	//Check status of Transmit Buffer 1
	if (pCan->SR & (1 << 2))
	{
		/* Transmit Channel 1 is available */
		/* Write frame informations and frame data into its CANxTFI1,
		 * CANxTID1, CANxTDA1, CANxTDB1 register */
		//pCan->TFI1 |= 0x00000000;//最高优先级
		pCan->TFI1 &= ~ 0x000F0000;

		pCan->TFI1 |= (pmsg->len) << 16;

		if(pmsg->rtr == 1)//remote frame
		{
			pCan->TFI1 |= (1 << 30); //set bit RTR
		}
		else
		{
			pCan->TFI1 &= ~(1 << 30);
		}

// 		if(CAN_Msg->format == EXT_ID_FORMAT)
// 		{
// 			pCan->TFI1 |= (((uint32_t)1) << 31); //set bit FF
// 		}
// 		else
// 		{
			pCan->TFI1 &= ~(((uint32_t)1) << 31);//CanFestival only support standard format(11 bit)
//		}

		/* Write CAN ID*/
		pCan->TID1 = pmsg->cob_id;

		/*Write first 4 data bytes*/
		data = (pmsg->data[0]) | (((pmsg->data[1]))<< 8) | ((pmsg->data[2]) << 16) | ((pmsg->data[3]) << 24);
		pCan->TDA1 = data;

		/*Write second 4 data bytes*/
		data = (pmsg->data[4]) | (((pmsg->data[5])) << 8)|((pmsg->data[6]) << 16)|((pmsg->data[7]) << 24);
		pCan->TDB1 = data;

		 /*Write transmission request*/
		 pCan->CMR = 0x21;

		 return SUCCESS;
	}

	//check status of Transmit Buffer 2
	else if((pCan->SR) & (1 << 10))
	{
		/* Transmit Channel 2 is available */
		/* Write frame informations and frame data into its CANxTFI2,
		 * CANxTID2, CANxTDA2, CANxTDB2 register */
		//pCan->TFI2 |= 0x00000001;//次高优先级
		pCan->TFI2 &= ~ 0x000F0000;

		pCan->TFI2 |= (pmsg->len) << 16;

		if(pmsg->rtr == 1)//remote frame
		{
			pCan->TFI2 |= (1 << 30); //set bit RTR
		}
		else
		{
			pCan->TFI2 &= ~(1 << 30);
		}

// 		if(CAN_Msg->format == EXT_ID_FORMAT)
// 		{
// 			pCan->TFI2 |= (((uint32_t)1) << 31); //set bit FF
// 		}
// 		else
// 		{
			pCan->TFI2 &= ~(((uint32_t)1) << 31);//CanFestival only support standard format(11 bit)
//		}

		/* Write CAN ID*/
		pCan->TID2 = pmsg->cob_id;

		/*Write first 4 data bytes*/
		data = (pmsg->data[0]) | (((pmsg->data[1]))<< 8) | ((pmsg->data[2]) << 16) | ((pmsg->data[3]) << 24);
		pCan->TDA2 = data;

		/*Write second 4 data bytes*/
		data = (pmsg->data[4]) | (((pmsg->data[5])) << 8)|((pmsg->data[6]) << 16)|((pmsg->data[7]) << 24);
		pCan->TDB2 = data;

		/*Write transmission request*/
		pCan->CMR = 0x41;

		return SUCCESS;
	}

	//check status of Transmit Buffer 3
	else if (pCan->SR & (1<<18))
	{
		/* Transmit Channel 3 is available */
		/* Write frame informations and frame data into its CANxTFI3,
		 * CANxTID3, CANxTDA3, CANxTDB3 register */
		//pCan->TFI3 |= 0x00000002;//最低优先级
		pCan->TFI3 &= ~ 0x000F0000;

		pCan->TFI3 |= (pmsg->len) << 16;

		if(pmsg->rtr == 1)//remote frame
		{
			pCan->TFI3 |= (1 << 30); //set bit RTR
		}
		else
		{
			pCan->TFI3 &= ~(1 << 30);
		}

// 		if(CAN_Msg->format == EXT_ID_FORMAT)
// 		{
// 			pCan->TFI3 |= (((uint32_t)1) << 31); //set bit FF
// 		}
// 		else
// 		{
			pCan->TFI3 &= ~(((uint32_t)1) << 31);//CanFestival only support standard format(11 bit)
//		}

		/* Write CAN ID*/
		pCan->TID3 = pmsg->cob_id;

		/*Write first 4 data bytes*/
		data = (pmsg->data[0]) | (((pmsg->data[1]))<< 8) | ((pmsg->data[2]) << 16) | ((pmsg->data[3]) << 24);
		pCan->TDA3 = data;

		/*Write second 4 data bytes*/
		data = (pmsg->data[4]) | (((pmsg->data[5])) << 8)|((pmsg->data[6]) << 16)|((pmsg->data[7]) << 24);
		pCan->TDB3 = data;

		/*Write transmission request*/
		pCan->CMR = 0x81;

		return SUCCESS;
	}
	else
	{
		return ERROR;
	}
}

/*-------------------------PRIVATE FUNCTIONS----------------------------*/
/*********************************************************************//**
 * @brief		Print Message via COM1
 * param[in]	msg: point to CAN_MSG_Type object that will be printed
 * @return 		none
 **********************************************************************/
void PrintSendMsg(Message* m)
{
// 	rt_kprintf("Send{id:%x,rtr:%d,len:%d,data:%x %x %x %x %x %x %x %x}\n", m->cob_id, m->rtr, m->len,
// 			m->data[0],m->data[1],m->data[2],m->data[3],m->data[4],m->data[5],m->data[6],m->data[7]);
		rt_kprintf("Send{id:%x,rtr:%d,len:%d,data:%x %x %x %x %x %x %x %x}\n", m->cob_id, m->rtr, m->len,
			m->data[0],m->data[1],m->data[2],m->data[3],m->data[4],m->data[5],m->data[6],m->data[7]);
}
void PrintRecvMsg(Message* m)
{
	rt_kprintf("Received{id:%x,rtr:%d,len:%d,data:%x %x %x %x %x %x %x %x}\n", m->cob_id, m->rtr, m->len,
			m->data[0],m->data[1],m->data[2],m->data[3],m->data[4],m->data[5],m->data[6],m->data[7]);
}
/**
  * @brief  can_slave_thread
  * @param  None
  * @retval : None
  */
void can_recv_thread(void* parameter)
{
	recv_sem = rt_sem_create("recvsem", 0, RT_IPC_FLAG_FIFO);

	can_slave_init();

	test_slave();

  /* Infinite loop*/
  while(1)
  {
		rt_sem_take(recv_sem, RT_WAITING_FOREVER);

		{
			//uint32_t next;
			Message *pmsg;
			
			NVIC_DisableIRQ(CAN_IRQn);
			if (rx_read == rx_save)
			{	
				NVIC_EnableIRQ(CAN_IRQn);
				continue;
			}
			pmsg = &rx_msg_buf[rx_read];
			rx_read = (rx_read + 1) % RX_BUF_LEN;
			NVIC_EnableIRQ(CAN_IRQn);
			
    	/* Disable the Interrupt sources */
			NVIC_DisableIRQ(BRD_TIM_INTR_CAN);
			//PrintRecvMsg(pmsg);
			canDispatch(&TestSlave_Data, pmsg);
			
			/* Enable the Interrupt sources */
			NVIC_EnableIRQ(BRD_TIM_INTR_CAN);

		}
  }
}

void can_send_thread(void *parameter)
{
//	uint32_t next;
	Status  rst;
	Message *ptx_msg;	
	tran_sem = rt_sem_create("transem", 0, RT_IPC_FLAG_FIFO); 	

	while(1)
	{
		rt_sem_take(tran_sem, RT_WAITING_FOREVER);

		NVIC_DisableIRQ(CAN_IRQn);
		if (tx_read == tx_save)
		{
			NVIC_EnableIRQ(CAN_IRQn);
			continue;
		}
		ptx_msg = &tx_msg_buf[tx_read];
		NVIC_EnableIRQ(CAN_IRQn);
		
		rst = CanFestival_SendMsg(NETWORK_CAN_NO, ptx_msg);
		
		if(rst == SUCCESS)
		{
			tx_read = (tx_read + 1) % TX_BUF_LEN;
			//PrintSendMsg(ptx_msg);
		}
		else
		{
			rt_thread_delay(1);
			rt_sem_release(tran_sem);
		} 
	}		
}

#include <applicfg.h>
unsigned char canSend(CAN_PORT notused, Message *m)
{
	uint32_t	i;

	NVIC_DisableIRQ(CAN_IRQn);
	if((tx_save + 1) % TX_BUF_LEN == tx_read)//full
	{
		NVIC_EnableIRQ(CAN_IRQn);
		rt_kprintf("buf overflow!!");
		return 1;
	}
	else  //not full
	{
		NVIC_EnableIRQ(CAN_IRQn);
		rt_memcpy(&tx_msg_buf[tx_save], m, sizeof(Message));
// 		Message *ptx_msg;

// 		ptx_msg = &tx_msg_buf[tx_save];
// 		ptx_msg->cob_id = m->cob_id;

// 		ptx_msg->rtr = m->rtr;
//   	ptx_msg->len = m->len;
// 		for(i = 0; i < m->len; i++)
// 		{
// 			  ptx_msg->data[i] = m->data[i];
// 		}

		tx_save = (tx_save + 1) % TX_BUF_LEN;
		rt_sem_release(tran_sem);
		return 0x00;
	}

	return 0xFF;
}

uint32_t TranslateBaudeRate(char* optarg){
	if(!strcmp( optarg, "1M"))   return 1000000;
  if(!strcmp( optarg, "800K")) return  800000;
  if(!strcmp( optarg, "500K")) return  500000;
  if(!strcmp( optarg, "250K")) return  250000;
  if(!strcmp( optarg, "125K")) return  125000;
	if(!strcmp( optarg, "100K")) return  100000;
	if(!strcmp( optarg, "50K"))  return   50000;
	if(!strcmp( optarg, "20K"))  return   20000;
	if(!strcmp( optarg, "10K"))  return   10000;
 
	return 0x0000;
}

UNS8 canChangeBaudRate(CAN_PORT port, char* baud)
{
	uint32_t baudrate;
	LPC_CAN_TypeDef* pCan = NETWORK_CAN_NO == CAN_ID_1? LPC_CAN1 : LPC_CAN2;
	
	baudrate = TranslateBaudeRate(baud);
	//can_SetBaudrate (pCan, baudrate);
	
//    if(port){
// 		UNS8 res;
// 	    //LeaveMutex();
// 		res = m_canChangeBaudRate(((CANPort*)port)->fd, baud);
// 		//EnterMutex();
// 		return res; // OK
// 	}
	return 0; // OK
}
/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		CAN_IRQ Handler, control receive message operation
 * param[in]	none
 * @return 		none
 **********************************************************************/
void CAN_IRQHandler(void)
{
	uint32_t data;
	Message *pmsg;
//	uint32_t next;//,i;		
	uint8_t IntStatus;
	/* get interrupt status
	 * Note that: Interrupt register CANICR will be reset after read.
	 * So function "CAN_IntGetStatus" should be call only one time
	 */
	LPC_CAN_TypeDef* pCan = NETWORK_CAN_NO == CAN_ID_1? LPC_CAN1 : LPC_CAN2;//CAN_GetPointer(NETWORK_CAN_NO);
	IntStatus = pCan->ICR;//CAN_IntGetStatus(NETWORK_CAN_NO);
	
	//check receive interrupt
	if((IntStatus >> 0) & 0x01)
	{
		//check status of Receive Buffer
		if((pCan->SR &0x00000001))
		{
			/* Receive message is available */
			/* Read frame informations */
			//CAN_Msg->format = (uint8_t)(((pCan->RFS) & 0x80000000) >> 31);
			if (((pCan->RFS) & 0x80000000) >> 31)//29 bit format
			{
			}
			else//11 bit format, CanFestival only support 11 bit format
			{
				if ((rx_save + 1) % RX_BUF_LEN == rx_read)//full
				{
					pCan->CMR = 0x04;	
					return;
				}
				pmsg = &rx_msg_buf[rx_save];
				rx_save = (rx_save + 1) % RX_BUF_LEN;
				
				//CAN_Msg->type = (uint8_t)(((pCan->RFS) & 0x40000000) >> 30);
				pmsg->rtr = (uint8_t)(((pCan->RFS) & 0x40000000) >> 30);
				//CAN_Msg->len = (uint8_t)(((pCan->RFS) & 0x000F0000) >> 16);
				pmsg->len = (uint8_t)(((pCan->RFS) & 0x000F0000) >> 16);
				/* Read CAN message identifier */
				//CAN_Msg->id = pCan->RID;
				pmsg->cob_id = (uint16_t)pCan->RID;
				/* Read the data if received message was DATA FRAME */
				//if (CAN_Msg->type == DATA_FRAME)
				if (pmsg->rtr == 0)//data frame
				{
					/* Read first 4 data bytes */
					data = pCan->RDA;
					//*((uint8_t *) &CAN_Msg->dataA[0])= data & 0x000000FF;
					//*((uint8_t *) &CAN_Msg->dataA[1])= (data & 0x0000FF00) >> 8;;
					//*((uint8_t *) &CAN_Msg->dataA[2])= (data & 0x00FF0000) >> 16;
					//*((uint8_t *) &CAN_Msg->dataA[3])= (data & 0xFF000000) >> 24;
					pmsg->data[0] = (uint8_t)(data & 0x000000FF);
					pmsg->data[1] = (uint8_t)((data & 0x0000FF00) >> 8);
					pmsg->data[2] = (uint8_t)((data & 0x00FF0000) >> 16);
					pmsg->data[3] = (uint8_t)((data & 0xFF000000) >> 24);
					/* Read second 4 data bytes */
					data = pCan->RDB;
					//*((uint8_t *) &CAN_Msg->dataB[0])= data & 0x000000FF;
					//*((uint8_t *) &CAN_Msg->dataB[1])= (data & 0x0000FF00) >> 8;
					//*((uint8_t *) &CAN_Msg->dataB[2])= (data & 0x00FF0000) >> 16;
					//*((uint8_t *) &CAN_Msg->dataB[3])= (data & 0xFF000000) >> 24;
					pmsg->data[4] = (uint8_t)(data & 0x000000FF);
					pmsg->data[5] = (uint8_t)((data & 0x0000FF00) >> 8);
					pmsg->data[6] = (uint8_t)((data & 0x00FF0000) >> 16);
					pmsg->data[7] = (uint8_t)((data & 0xFF000000) >> 24);
					/*release receive buffer*/
					pCan->CMR = 0x04;
				}
				else
				{
					/* Received Frame is a Remote Frame, not have data, we just receive
					 * message information only */
					pCan->CMR = 0x04; /*release receive buffer*/
				}
				//rx_save = next;
				rt_sem_release(recv_sem);
			}
		}
		else
		{
			// no receive message available
		}
	}
}

static TimerCallback_t init_callback;

s_BOARD SlaveBoard  = {"0", "1M"};

// void help(void)
// {
//   eprintf("**************************************************************\n");
//   eprintf("*  TestMasterSlave                                           *\n");
//   eprintf("*                                                            *\n");
//   eprintf("*  A simple example for PC. It does implement 2 CanOpen      *\n");
//   eprintf("*  nodes in the same process. A master and a slave. Both     *\n");
//   eprintf("*  communicate together, exchanging periodically NMT, SYNC,  *\n");
//   eprintf("*  SDO and PDO. Master configure heartbeat producer time     *\n");
//   eprintf("*  at 1000 ms for slave node-id 0x02 by concise DCF.         *\n");                                  
//   eprintf("*                                                            *\n");
//   eprintf("*   Usage:                                                   *\n");
//   eprintf("*   ./TestMasterSlave  [OPTIONS]                             *\n");
//   eprintf("*                                                            *\n");
//   eprintf("*   OPTIONS:                                                 *\n");
//   eprintf("*     -l : Can library [\"libcanfestival_can_virtual.so\"]     *\n");
//   eprintf("*                                                            *\n");
//   eprintf("*    Slave:                                                  *\n");
//   eprintf("*     -s : bus name [\"0\"]                                    *\n");
//   eprintf("*     -S : 1M,500K,250K,125K,100K,50K,20K,10K,none(disable)  *\n");
//   eprintf("*                                                            *\n");
//   eprintf("*    Master:                                                 *\n");
//   eprintf("*     -m : bus name [\"1\"]                                    *\n");
//   eprintf("*     -M : 1M,500K,250K,125K,100K,50K,20K,10K,none(disable)  *\n");
//   eprintf("*                                                            *\n");
//   eprintf("**************************************************************\n");
// }

/***************************  INIT  *****************************************/
UNS8 NodeID = 0x02;
void InitNodes(CO_Data* d, UNS32 id)
{
	/****************************** INITIALISATION SLAVE *******************************/
	if(rt_strcmp(SlaveBoard.baudrate, "none")) {
		setNodeId(&TestSlave_Data, NodeID);

		/* init */
		setState(&TestSlave_Data, Initialisation);
	}
}

static StartTimerLoop(TimerCallback_t _init_callback) 
{
	init_callback = _init_callback;
	
  CAN_Timer_start();
	SetAlarm(NULL, 0, init_callback, 10, 0);//这里时间值不能太小或为0，否则可能计数已过，无法匹配进入中断
	
}

/***************************  EXIT  *****************************************/
void Exit(CO_Data* d, UNS32 id)
{

}

/****************************************************************************/
/*****************************  test_salve  *********************************/
/****************************************************************************/
static int test_slave(void)
{
	char *ptr = info_dev;
	uint32_t size = sizeof(char *);
	UNS32 TestSlave_COB_ID_TPDO = 0x180 + NodeID;
	size = sizeof(UNS32);
	if(rt_strcmp(SlaveBoard.baudrate, "none")){
		
		TestSlave_Data.heartbeatError = TestSlave_heartbeatError;
		TestSlave_Data.initialisation = TestSlave_initialisation;
		TestSlave_Data.preOperational = TestSlave_preOperational;
		TestSlave_Data.operational = TestSlave_operational;
		TestSlave_Data.stopped   = TestSlave_stopped;
		TestSlave_Data.post_sync = TestSlave_post_sync;
		TestSlave_Data.post_TPDO = TestSlave_post_TPDO;
		TestSlave_Data.storeODSubIndex = TestSlave_storeODSubIndex;
		TestSlave_Data.post_emcy = TestSlave_post_emcy;
		TestSlave_Data.post_SlaveBootup = TestSlave_post_SlaveBootup;
	  TestSlave_Data.post_SlaveStateChange = TestSlave_post_SlaveStateChange;
	}

	//writeLocalDict( &TestSlave_Data, 0x1800, 1, &TestSlave_COB_ID_TPDO, &size, RW);
	// Start timer thread
	StartTimerLoop(&InitNodes);

	//writeLocalDict( &TestSlave_Data, 0x1F50, 1, &ptr, &size, RW);
	return 0;
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
