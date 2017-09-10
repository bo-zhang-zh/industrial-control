/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               EMC_nand.c
** Descriptions:            The FSMC Nand application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2012-2-2
** Version:                 v2.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include "emc_nand.h"
#include "debug_frmwrk.h"
#include "lpc177x_8x_emc.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_timer.h"

#include "lpc177x_8x_gpio.h"
/* Private define ------------------------------------------------------------*/
#define NAND_FLASH_START_ADDR     ((uint32_t)0x80000000) 

#define ROW_ADDRESS (Address.Page + (Address.Block + (Address.Zone * NAND_ZONE_SIZE)) * NAND_BLOCK_SIZE)

/* Private variables ---------------------------------------------------------*/
static uint32_t TxBuffer [NAND_PAGE_SIZE/4];
static uint32_t RxBuffer [NAND_PAGE_SIZE/4];

uint32_t wait;

/*******************************************************************************
* Function Name  : EMC_NAND_Init
* Description    : Configures the FSMC and GPIOs to interface with the NAND memory.
*                  This function must be called before any write/read operation
*                  on the NAND.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void EMC_NAND_Init(void)
{
//	uint32_t i;
//	TIM_TIMERCFG_Type TIM_ConfigStruct;
	EMC_STATIC_MEM_Config_Type config;

	/**************************************************************************
	* Initialize EMC for NAND FLASH
	**************************************************************************/
	config.CSn = 0;
	config.AddressMirror = 0;

	config.ByteLane = 1;
	config.DataWidth = 8;
	config.ExtendedWait = 0;
	config.PageMode = 0;
// 	config.WaitWEn = 2;
// 	config.WaitOEn = 2;
// 	config.WaitWr = 0x1f;
// 	config.WaitPage = 0x1f;
// 	config.WaitRd = 0x1f;
// 	config.WaitTurn = 0x1f;	
	
	config.WaitWEn = 2;
	config.WaitOEn = 2;
	config.WaitWr = 0x1f;
	config.WaitPage = 0x1f;
	config.WaitRd = 0x1f;
	config.WaitTurn = 0x1f;	
	if(StaticMem_Init(&config) != EMC_FUNC_OK)
	{
		rt_kprintf("nand init error!!\n");
	}
	 // init timer
// 	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
// 	TIM_ConfigStruct.PrescaleValue	= 1;

// 	// Set configuration for Tim_config and Tim_MatchConfig
// 	TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);

//   	// wait 2ms
// 	TIM_Waitms(2);
  rt_thread_delay(2);
// 	PINSEL_ConfigPin(4,23,0);	   /* P4.23 - GPIO NAND FLASH BUSY PIN*/
//   GPIO_SetDir(4, (1<<23), 0);
}

/*********************************************************************//**
 * @brief 		Ready/Busy check, no timeout, basically, R/B bit should
 * 				once to bail out from this routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void NandFlash_WaitForReady( void )
{	
    uint16_t i;

  //  while( (GPIO_ReadValue(4)&(1<<23)) );	/* from high to low once */

 //   while( !(GPIO_ReadValue(4)&(1<<23)) );	/* from low to high once */

    for( i=0; i < 5000; i++ );
}

/******************************************************************************
* Function Name  : EMC_NAND_ReadID
* Description    : Reads NAND memory's ID.
* Input          : - NAND_ID: pointer to a NAND_IDTypeDef structure which will hold
*                    the Manufacturer and Device ID.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void EMC_NAND_ReadID(NAND_IDTypeDef* NAND_ID)
{
  /* Send Command to the command area */ 	
  *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = 0x90;
  *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = 0x00;

   /* Sequence to read ID from NAND flash */	
   NAND_ID->Maker_ID   = *(volatile uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA);
   NAND_ID->Device_ID  = *(volatile uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA);
   NAND_ID->Third_ID   = *(volatile uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA);
   NAND_ID->Fourth_ID  = *(volatile uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA);
}

/******************************************************************************
* Function Name  : EMC_NAND_WriteSmallPage
* Description    : This routine is for writing one or several 512 Bytes Page size.
* Input          : - pBuffer: pointer on the Buffer containing data to be written   
*                  - Address: First page address
*                  - NumPageToWrite: Number of page to write  
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*                   - NAND_TIMEOUT_ERROR: when the previous operation generate 
*                     a Timeout error
*                   - NAND_READY: when memory is ready for the next operation 
*                  And the new status of the increment address operation. It can be:
*                  - NAND_VALID_ADDRESS: When the new address is valid address
*                  - NAND_INVALID_ADDRESS: When the new address is invalid address
* Attention		 : None
*******************************************************************************/
uint32_t EMC_NAND_WriteSmallPage(uint8_t *pBuffer, NAND_ADDRESS Address, uint32_t NumPageToWrite)
{
  uint32_t index = 0x00, numpagewritten = 0x00, addressstatus = NAND_VALID_ADDRESS;
  uint32_t status = NAND_READY, size = 0x00;

  while((NumPageToWrite != 0x00) && (addressstatus == NAND_VALID_ADDRESS) && (status == NAND_READY))
  {
    /* Page write command and address */
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM;

    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = 0x00;  
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = 0X00;  
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(ROW_ADDRESS);  
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(ROW_ADDRESS);   

    /* Calculate the size */
    size = NAND_PAGE_SIZE + (NAND_PAGE_SIZE * numpagewritten);

    /* Write data */
    for(; index < size; index++)
    {
      *(volatile uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA) = pBuffer[index];
    }
    
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM_TRUE;

    NandFlash_WaitForReady();

    /* Check status for successful operation */
    status = EMC_NAND_GetStatus();
    
    if(status == NAND_READY)
    {
      numpagewritten++;

      NumPageToWrite--;

      /* Calculate Next small page Address */
      addressstatus = EMC_NAND_AddressIncrement(&Address);    
    }    
  }
  
  return (status | addressstatus);
}

/******************************************************************************
* Function Name  : EMC_NAND_ReadSmallPage
* Description    : This routine is for sequential read from one or several 
*                  512 Bytes Page size.
* Input          : - pBuffer: pointer on the Buffer to fill  
*                  - Address: First page address
*                  - NumPageToRead: Number of page to read
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*                   - NAND_TIMEOUT_ERROR: when the previous operation generate 
*                     a Timeout error
*                   - NAND_READY: when memory is ready for the next operation 
*                  And the new status of the increment address operation. It can be:
*                  - NAND_VALID_ADDRESS: When the new address is valid address
*                  - NAND_INVALID_ADDRESS: When the new address is invalid address
* Attention		 : None
*******************************************************************************/
uint32_t EMC_NAND_ReadSmallPage(uint8_t *pBuffer, NAND_ADDRESS Address, uint32_t NumPageToRead)
{
  uint32_t index = 0x00, numpageread = 0x00, addressstatus = NAND_VALID_ADDRESS;
  uint32_t status = NAND_READY, size = 0x00;

  while((NumPageToRead != 0x0) && (addressstatus == NAND_VALID_ADDRESS))
  {	   
    /* Page Read command and page address */
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_1; 
   
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = 0x00; 
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = 0X00; 
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(ROW_ADDRESS);  
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(ROW_ADDRESS);  
    
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_TRUE; 
   
    NandFlash_WaitForReady();
    
    /* Calculate the size */
    size = NAND_PAGE_SIZE + (NAND_PAGE_SIZE * numpageread);
    
    /* Get Data into Buffer */    
    for(; index < size; index++)
    {
      pBuffer[index]= *(volatile uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA);
    }

    numpageread++;
    
    NumPageToRead--;

    /* Calculate page address */           			 
    addressstatus = EMC_NAND_AddressIncrement(&Address);
  }

  status = EMC_NAND_GetStatus();
  
  return (status | addressstatus);
}

/******************************************************************************
* Function Name  : EMC_NAND_WriteSpareArea
* Description    : This routine write the spare area information for the specified 
*                  pages addresses.
* Input          : - pBuffer: pointer on the Buffer containing data to be written 
*                  - Address: First page address
*                  - NumSpareAreaTowrite: Number of Spare Area to write
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*                   - NAND_TIMEOUT_ERROR: when the previous operation generate 
*                     a Timeout error
*                   - NAND_READY: when memory is ready for the next operation 
*                  And the new status of the increment address operation. It can be:
*                  - NAND_VALID_ADDRESS: When the new address is valid address
*                  - NAND_INVALID_ADDRESS: When the new address is invalid address
* Attention		 : None
*******************************************************************************/
uint32_t EMC_NAND_WriteSpareArea(uint8_t *pBuffer, NAND_ADDRESS Address, uint32_t NumSpareAreaTowrite)
{
  uint32_t index = 0x00, numsparesreawritten = 0x00, addressstatus = NAND_VALID_ADDRESS;
  uint32_t status = NAND_READY, size = 0x00; 

  while((NumSpareAreaTowrite != 0x00) && (addressstatus == NAND_VALID_ADDRESS) && (status == NAND_READY))
  {
    /* Page write Spare area command and address */
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM;

    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = 0x00; 
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = 0x08; 
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(ROW_ADDRESS);  
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(ROW_ADDRESS);   

    /* Calculate the size */ 
    size = NAND_SPARE_AREA_SIZE + (NAND_SPARE_AREA_SIZE * numsparesreawritten);

    /* Write the data */ 
    for(; index < size; index++)
    {
      *(volatile uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA) = pBuffer[index];
    }
    
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM_TRUE;

    NandFlash_WaitForReady();

    /* Check status for successful operation */
    status = EMC_NAND_GetStatus();

    if(status == NAND_READY)
    {
      numsparesreawritten++;      

      NumSpareAreaTowrite--;  
    
      /* Calculate Next page Address */
      addressstatus = EMC_NAND_AddressIncrement(&Address);
    }       
  }
  
  return (status | addressstatus);
}

/******************************************************************************
* Function Name  : EMC_NAND_ReadSpareArea
* Description    : This routine read the spare area information from the specified
*                  pages addresses.
* Input          : - pBuffer: pointer on the Buffer to fill  
*                  - Address: First page address
*                  - NumSpareAreaToRead: Number of Spare Area to read
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*                   - NAND_TIMEOUT_ERROR: when the previous operation generate 
*                     a Timeout error
*                   - NAND_READY: when memory is ready for the next operation 
*                  And the new status of the increment address operation. It can be:
*                  - NAND_VALID_ADDRESS: When the new address is valid address
*                  - NAND_INVALID_ADDRESS: When the new address is invalid address
* Attention		 : None
*******************************************************************************/
uint32_t EMC_NAND_ReadSpareArea(uint8_t *pBuffer, NAND_ADDRESS Address, uint32_t NumSpareAreaToRead)
{
  uint32_t numsparearearead = 0x00, index = 0x00, addressstatus = NAND_VALID_ADDRESS;
  uint32_t status = NAND_READY, size = 0x00;

  while((NumSpareAreaToRead != 0x0) && (addressstatus == NAND_VALID_ADDRESS))
  {     
    /* Page Read command and page address */     
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_1;

    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = 0x00; 
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = 0x08;     
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(ROW_ADDRESS);  
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(ROW_ADDRESS);    

    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_TRUE;

    NandFlash_WaitForReady();
    
    /* Data Read */
    size = NAND_SPARE_AREA_SIZE +  (NAND_SPARE_AREA_SIZE * numsparearearead);
	
    /* Get Data into Buffer */
    for ( ;index < size; index++)
    {
      pBuffer[index] = *(volatile uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA);
    }
    
    numsparearearead++;
    
    NumSpareAreaToRead--;

    /* Calculate page address */           			 
    addressstatus = EMC_NAND_AddressIncrement(&Address);
  }

  status = EMC_NAND_GetStatus();

  return (status | addressstatus);
}

/******************************************************************************
* Function Name  : EMC_NAND_EraseBlock
* Description    : This routine erase complete block from NAND FLASH
* Input          : - Address: Any address into block to be erased
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*                   - NAND_TIMEOUT_ERROR: when the previous operation generate 
*                     a Timeout error
*                   - NAND_READY: when memory is ready for the next operation 
* Attention		 : None
*******************************************************************************/
uint32_t EMC_NAND_EraseBlock(NAND_ADDRESS Address)
{
  *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_ERASE0;
  
  *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(ROW_ADDRESS);  
  *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(ROW_ADDRESS);  
		
  *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_ERASE1; 

    NandFlash_WaitForReady();
  
  return (EMC_NAND_GetStatus());
}

/******************************************************************************
* Function Name  : EMC_NAND_Reset
* Description    : This routine reset the NAND FLASH
* Input          : None
* Output         : None
* Return         : NAND_READY
* Attention		 : None
*******************************************************************************/
uint32_t EMC_NAND_Reset(void)
{
  *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_RESET;

  return (NAND_READY);
}

/******************************************************************************
* Function Name  : EMC_NAND_GetStatus
* Description    : Get the NAND operation status
* Input          : None
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*                   - NAND_TIMEOUT_ERROR: when the previous operation generate 
*                     a Timeout error
*                   - NAND_READY: when memory is ready for the next operation  
* Attention		 : None  
*******************************************************************************/
uint32_t EMC_NAND_GetStatus(void)
{
  uint32_t timeout = 0x1000, status = NAND_READY;

  status = EMC_NAND_ReadStatus(); 

  /* Wait for a NAND operation to complete or a TIMEOUT to occur */
  while ((status != NAND_READY) &&( timeout != 0x00))
  {
     status = EMC_NAND_ReadStatus();
     timeout --;      
  }

  if(timeout == 0x00)
  {          
    status =  NAND_TIMEOUT_ERROR;      
  } 

  /* Return the operation status */
  return (status);      
}
/******************************************************************************
* Function Name  : EMC_NAND_ReadStatus
* Description    : Reads the NAND memory status using the Read status command 
* Input          : None
* Output         : None
* Return         : The status of the NAND memory. This parameter can be:
*                   - NAND_BUSY: when memory is busy
*                   - NAND_READY: when memory is ready for the next operation    
*                   - NAND_ERROR: when the previous operation gererates error  
* Attention		 : None 
*******************************************************************************/
uint32_t EMC_NAND_ReadStatus(void)
{
  uint32_t data = 0x00, status = NAND_BUSY;

  /* Read status operation ------------------------------------ */
  *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_STATUS;
  data = *(volatile uint8_t *)(NAND_FLASH_START_ADDR);

  if((data & NAND_ERROR) == NAND_ERROR)
  {
    status = NAND_ERROR;
  } 
  else if((data & NAND_READY) == NAND_READY)
  {
    status = NAND_READY;
  }
  else
  {
    status = NAND_BUSY; 
  }
  
  return (status);
}

/******************************************************************************
* Function Name  : NAND_AddressIncrement
* Description    : Increment the NAND memory address
* Input          : - Address: address to be incremented.
* Output         : None
* Return         : The new status of the increment address operation. It can be:
*                  - NAND_VALID_ADDRESS: When the new address is valid address
*                  - NAND_INVALID_ADDRESS: When the new address is invalid address
* Attention		 : None
*******************************************************************************/
uint32_t EMC_NAND_AddressIncrement(NAND_ADDRESS* Address)
{
  uint32_t status = NAND_VALID_ADDRESS;
 
  Address->Page++;

  if(Address->Page == NAND_BLOCK_SIZE)
  {
    Address->Page = 0;
    Address->Block++;
    
    if(Address->Block == NAND_ZONE_SIZE)
    {
      Address->Block = 0;
      Address->Zone++;

      if(Address->Zone == NAND_MAX_ZONE)
      {
        status = NAND_INVALID_ADDRESS;
      }
    }
  } 
  
  return (status);
}
  NAND_IDTypeDef NAND_ID;

uint32_t count;

/******************************************************************************
* Function Name  : EMC_NAND_Test
* Description    : NAND test
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void EMC_NAND_Test(void)
{
	uint16_t index;
	uint8_t error_nand;
	
	NAND_ADDRESS WriteReadAddr;
	
	EMC_NAND_Init();
	
	count = ADDR_AREA;
	/* test NAND Flash */
	EMC_NAND_ReadID(&NAND_ID);
	
	if ((NAND_ID.Maker_ID == 0xEC) && (NAND_ID.Device_ID == 0xF1)
	&& (NAND_ID.Third_ID == 0x80) && (NAND_ID.Fourth_ID == 0x15))
	{
		_DBG("\r\nNAND Type = K9F1G08U0A 128M NAND Flash  ---------  ");
	}
	else if ((NAND_ID.Maker_ID == 0xEC) && (NAND_ID.Device_ID == 0xF1)
	&& (NAND_ID.Third_ID == 0x00) && (NAND_ID.Fourth_ID == 0x95))
	{
		_DBG("\r\nNAND Type = K9F1G08U0B 128M NAND Flash  ---------  ");		
	}
	else if ((NAND_ID.Maker_ID == 0xAD) && (NAND_ID.Device_ID == 0xF1)
	&& (NAND_ID.Third_ID == 0x80) && (NAND_ID.Fourth_ID == 0x1D))
	{
		_DBG("\r\nNAND Type = HY27UF081G2A 128M NAND Flash  ---------  ");		
	}
	else
	{
		_DBG("\r\nNAND Type = Unknow  ---------  ");
	}
	
	/* NAND memory address to write to */
	WriteReadAddr.Zone = 0x00;
	WriteReadAddr.Block = 100;
	WriteReadAddr.Page = 0x00;
	
	error_nand = 0;
	
	/* Erase the NAND first Block */
	EMC_NAND_EraseBlock(WriteReadAddr);
	for( WriteReadAddr.Page = 0; WriteReadAddr.Page < NAND_BLOCK_SIZE; WriteReadAddr.Page++ )
	{	
		/* Write data to FSMC NOR memory */
		/* Fill the buffer to send */
		for (index = 0; index < NAND_PAGE_SIZE/4; index++ )
		{
			TxBuffer[index] = index + WriteReadAddr.Block * WriteReadAddr.Page;
		}
		
		EMC_NAND_WriteSmallPage((void*)TxBuffer, WriteReadAddr, 1);
		
		/* Read back the written data */
		memset( (void*)RxBuffer, 0, sizeof(RxBuffer) );
		
		EMC_NAND_ReadSmallPage ((void*)RxBuffer, WriteReadAddr, 1);
		
		if( memcmp( (char*)TxBuffer, (char*)RxBuffer, NAND_PAGE_SIZE ) == 0 )
		{

		}
		else
		{
			error_nand++;
		}	      
	}

	if( error_nand == 0 )
	{
		_DBG_("Nand Flash OK \r\n");
	}
	else
	{
		_DBG_("Nand Flash error \r\n");
	}
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
