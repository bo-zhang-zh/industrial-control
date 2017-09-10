/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               EMC_nand.h
** Descriptions:            The FSMC NAND application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2011-2-16
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef __EMC_NAND_H
#define __EMC_NAND_H

/* Includes ------------------------------------------------------------------*/
#include "lpc_types.h"

#include <string.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint8_t Maker_ID;
  uint8_t Device_ID;
  uint8_t Third_ID;
  uint8_t Fourth_ID;
}
NAND_IDTypeDef;

typedef struct 
{
  uint16_t Zone;
  uint16_t Block;
  uint16_t Page;
} 
NAND_ADDRESS;

/* Private define ------------------------------------------------------------*/
#define NAND_FLASH_START_ADDR     ((uint32_t)0x80000000) 


/* NAND Area definition  for STM3210E-EVAL Board RevD */
#define CMD_AREA                   (uint32_t)(1<<20)  /* A16 = CLE  high */
#define ADDR_AREA                  (uint32_t)(1<<19)  /* A17 = ALE high */

#define DATA_AREA                  ((uint32_t)0x00000000) 

/* FSMC NAND memory command */
#define	NAND_CMD_READ_1             ((uint8_t)0x00)
#define	NAND_CMD_READ_TRUE          ((uint8_t)0x30)

#define	NAND_CMD_RDCOPYBACK         ((uint8_t)0x00)
#define	NAND_CMD_RDCOPYBACK_TRUE    ((uint8_t)0x35)

#define NAND_CMD_PAGEPROGRAM        ((uint8_t)0x80)
#define NAND_CMD_PAGEPROGRAM_TRUE   ((uint8_t)0x10)

#define NAND_CMD_COPYBACKPGM        ((uint8_t)0x85)
#define NAND_CMD_COPYBACKPGM_TRUE   ((uint8_t)0x10)
	
#define NAND_CMD_ERASE0             ((uint8_t)0x60)
#define NAND_CMD_ERASE1             ((uint8_t)0xD0)  

#define NAND_CMD_READID             ((uint8_t)0x90)	
#define NAND_CMD_STATUS             ((uint8_t)0x70)
#define NAND_CMD_RESET              ((uint8_t)0xFF)

#define NAND_CMD_CACHEPGM           ((uint8_t)0x80)
#define NAND_CMD_CACHEPGM_TRUE      ((uint8_t)0x15)

#define NAND_CMD_RANDOMIN           ((uint8_t)0x85)
#define NAND_CMD_RANDOMOUT          ((uint8_t)0x05)
#define NAND_CMD_RANDOMOUT_TRUE     ((uint8_t)0xE0)

#define NAND_CMD_CACHERD_START      ((uint8_t)0x00)
#define NAND_CMD_CACHERD_START2     ((uint8_t)0x31)
#define NAND_CMD_CACHERD_EXIT       ((uint8_t)0x34)

/* NAND memory status */
#define NAND_VALID_ADDRESS         ((uint32_t)0x00000100)
#define NAND_INVALID_ADDRESS       ((uint32_t)0x00000200)
#define NAND_TIMEOUT_ERROR         ((uint32_t)0x00000400)
#define NAND_BUSY                  ((uint32_t)0x00000000)
#define NAND_ERROR                 ((uint32_t)0x00000001)
#define NAND_READY                 ((uint32_t)0x00000040)

/* FSMC NAND memory parameters */
/* for K9F1G08 */
#define NAND_PAGE_SIZE             ((uint16_t)0x0800) /* 2 * 1024 bytes per page w/o Spare Area */
#define NAND_BLOCK_SIZE            ((uint16_t)0x0040) /* 64 pages per block */
#define NAND_ZONE_SIZE             ((uint16_t)0x0400) /* 1024 Block per zone */
#define NAND_SPARE_AREA_SIZE       ((uint16_t)0x0040) /* last 64 bytes as spare area */
#define NAND_MAX_ZONE              ((uint16_t)0x0001) /* 1 zones of 1024 block */

/* FSMC NAND memory address computation */
#define ADDR_1st_CYCLE(ADDR)       (uint8_t)((ADDR)& 0xFF)               /* 1st addressing cycle */
#define ADDR_2nd_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF00) >> 8)      /* 2nd addressing cycle */
#define ADDR_3rd_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF0000) >> 16)   /* 3rd addressing cycle */
#define ADDR_4th_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF000000) >> 24) /* 4th addressing cycle */

/* Private function prototypes -----------------------------------------------*/
void NandFlash_WaitForReady( void );
void EMC_NAND_Init(void);
void EMC_NAND_Test(void);
void EMC_NAND_ReadID(NAND_IDTypeDef* NAND_ID);
uint32_t EMC_NAND_WriteSmallPage(uint8_t *pBuffer, NAND_ADDRESS Address, uint32_t NumPageToWrite);
uint32_t EMC_NAND_ReadSmallPage (uint8_t *pBuffer, NAND_ADDRESS Address, uint32_t NumPageToRead);
uint32_t EMC_NAND_WriteSpareArea(uint8_t *pBuffer, NAND_ADDRESS Address, uint32_t NumSpareAreaTowrite);
uint32_t EMC_NAND_ReadSpareArea(uint8_t *pBuffer, NAND_ADDRESS Address, uint32_t NumSpareAreaToRead);
uint32_t EMC_NAND_EraseBlock(NAND_ADDRESS Address);
uint32_t EMC_NAND_Reset(void);
uint32_t EMC_NAND_GetStatus(void);
uint32_t EMC_NAND_ReadStatus(void);
uint32_t EMC_NAND_AddressIncrement(NAND_ADDRESS* Address);


#endif /* __EMC_NAND_H */

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
