/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.h
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
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

#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

/* Includes ------------------------------------------------------------------*/
#include "lpc177x_8x.h"
#include "lpc_types.h"
#include "lpc177x_8x_ssp.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_pinsel.h"

/* Private typedef -----------------------------------------------------------*/
typedef	struct POINT 
{
   uint16_t x;
   uint16_t y;
}Coordinate;


/* Private define ------------------------------------------------------------*/
/* AD通道选择命令字和工作寄存器 */
#define	CHX 	        0x90 	/* 通道Y+的选择控制字 */	
#define	CHY 	        0xd0	/* 通道X+的选择控制字 */

#define SSPSR_RNE       2
#define SSPSR_BSY       4

/* PORT number that TP_CS pin assigned on */
#define TP_CS_PORT_NUM		2
/* PIN number that TP_CS pin assigned on */
#define TP_CS_PIN_NUM		23

#define TP_CS_LOW()    GPIO_ClearValue(TP_CS_PORT_NUM, (1<<TP_CS_PIN_NUM));   
#define TP_CS_HIGH()   GPIO_SetValue(TP_CS_PORT_NUM, (1<<TP_CS_PIN_NUM));   

#define TP_INT_IN   ( GPIO_ReadValue(2) & (1<<11) ) 

/* Private function prototypes -----------------------------------------------*/				
void TP_Init(void);	
Coordinate *Read_Ads7846(void);

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


