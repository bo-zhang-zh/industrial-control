/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.c
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

/* Includes ------------------------------------------------------------------*/
#include "TouchPanel.h"



/* Private define ------------------------------------------------------------*/
#define THRESHOLD   20   /* ��ֵ���� */

/*******************************************************************************
* Function Name  : TP_Init
* Description    : TSC2046 Initialization
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TP_Init(void) 
{
    SSP_CFG_Type SSP_ConfigStruct;
	/*
	 * Initialize SPI pin connect
	 * P2.23 - TP_CS - used as GPIO
	 * P2.22 - SCK
	 * P2.26 - MISO
	 * P2.27 - MOSI
	 */

	PINSEL_ConfigPin(2, 23, 0);
	PINSEL_ConfigPin(2, 22, 2);
	PINSEL_ConfigPin(2, 26, 2);
	PINSEL_ConfigPin(2, 27, 2);

    /* P2.23 CS is output */
    GPIO_SetDir(TP_CS_PORT_NUM, (1<<TP_CS_PIN_NUM), 1);
	GPIO_SetValue(TP_CS_PORT_NUM, (1<<TP_CS_PIN_NUM));  

  	PINSEL_ConfigPin(2, 11, 0);	  
	GPIO_SetDir(2, (1<<11), 0);	  /* P2.11 TP_INT is input */

	/* initialize SSP configuration structure to default */
	SSP_ConfigStructInit(&SSP_ConfigStruct);

	SSP_ConfigStruct.ClockRate = 250000;
	SSP_ConfigStruct.CPHA = SSP_CPHA_FIRST;	   //SSP_CPHA_SECOND   SSP_CPHA_FIRST
	SSP_ConfigStruct.CPOL = SSP_CPOL_HI;		  //SSP_CPOL_LO	  SSP_CPOL_HI

	/* Initialize SSP peripheral with parameter given in structure above */
	SSP_Init(LPC_SSP0, &SSP_ConfigStruct);
	/* Enable SSP peripheral */
	SSP_Cmd(LPC_SSP0, ENABLE);
}

/*******************************************************************************
* Function Name  : DelayUS
* Description    : ��ʱ1us
* Input          : - cnt: ��ʱֵ
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void DelayUS(uint32_t cnt)
{
  uint32_t i;
  for(i = 0;i<cnt;i++)
  {
     uint8_t us = 100; /* ����ֵΪ12����Լ��1΢�� */    
     while (us--)     /* ��1΢��	*/
     {
       ;   
     }
  }
}

/*******************************************************************************
* Function Name  : WR_CMD
* Description    : �� ADS7843д����
* Input          : - cmd: ���������
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static uint8_t WR_CMD (uint8_t cmd)  
{ 
	/* wait for current SSP activity complete */
	//while (SSP_GetStatus(LPC_SSP0, SSP_STAT_BUSY) ==  SET);
	while((LPC_SSP0->SR & SSP_STAT_BUSY));

	//SSP_SendData(LPC_SSP0, (uint16_t) cmd);
	LPC_SSP0->DR = SSP_DR_BITMASK(cmd);

	//while (SSP_GetStatus(LPC_SSP0, SSP_STAT_RXFIFO_NOTEMPTY) == RESET);
	while(!(LPC_SSP0->SR & SSP_STAT_RXFIFO_NOTEMPTY));
	return (uint16_t)(SSP_DR_BITMASK(LPC_SSP0->DR));//(SSP_ReceiveData(LPC_SSP0));
} 

/*******************************************************************************
* Function Name  : RD_AD
* Description    : ��ȡADCֵ
* Input          : None
* Output         : None
* Return         : ADS7843���ض��ֽ�����
* Attention		 : None
*******************************************************************************/
static int RD_AD(void)  
{ 
  unsigned short buf,temp; 

  temp = WR_CMD(0x00);
  buf = temp<<8; 
  DelayUS(1); 
  temp = WR_CMD(0x00);;
  buf |= temp; 
  buf>>=3; 
  buf&=0xfff; 
  return buf; 
} 

/*******************************************************************************
* Function Name  : Read_X
* Description    : ��ȡADS7843ͨ��X+��ADCֵ 
* Input          : None
* Output         : None
* Return         : ADS7843����ͨ��X+��ADCֵ
* Attention		 : None
*******************************************************************************/
int Read_X(void)  
{  
  int i; 
  TP_CS_LOW(); 
  DelayUS(1); 
  WR_CMD(CHX); 
  DelayUS(1); 
  i=RD_AD(); 
  TP_CS_HIGH(); 
  return i;    
} 

/*******************************************************************************
* Function Name  : Read_Y
* Description    : ��ȡADS7843ͨ��Y+��ADCֵ
* Input          : None
* Output         : None
* Return         : ADS7843����ͨ��Y+��ADCֵ
* Attention		 : None
*******************************************************************************/
int Read_Y(void)  
{  
  int i; 
  TP_CS_LOW(); 
  DelayUS(1); 
  WR_CMD(CHY); 
  DelayUS(1); 
  i=RD_AD(); 
  TP_CS_HIGH(); 
  return i;     
} 

/*******************************************************************************
* Function Name  : TP_GetAdXY
* Description    : ��ȡADS7843 ͨ��X+ ͨ��Y+��ADCֵ
* Input          : None
* Output         : None
* Return         : ADS7843���� ͨ��X+ ͨ��Y+��ADCֵ 
* Attention		 : None
*******************************************************************************/
void TP_GetAdXY(int *x,int *y)  
{ 
  int adx,ady; 
  adx=Read_X(); 
  DelayUS(1); 
  ady=Read_Y(); 
  *x=adx; 
  *y=ady; 
} 

/*******************************************************************************
* Function Name  : Read_Ads7846
* Description    : �õ��˲�֮���X Y
* Input          : None
* Output         : None
* Return         : Coordinate�ṹ���ַ
* Attention		 : None
*******************************************************************************/
Coordinate *Read_Ads7846(void)
{
  static Coordinate  screen;
  int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
  uint8_t count=0;
  int buffer[2][9]={{0},{0}};  /* ����X��Y���ж�β��� */
  do					       /* ѭ������9�� */
  {		   
    TP_GetAdXY(TP_X,TP_Y);  
	buffer[0][count]=TP_X[0];  
	buffer[1][count]=TP_Y[0];
	count++;  
  }
  while(!TP_INT_IN&& count<9);  /* TP_INT_INΪ�������ж�����,���û����������ʱTP_INT_IN�ᱻ�õ� */
  if(count==9)   /* �ɹ�����9��,�����˲� */ 
  {  
    /* Ϊ����������,�ֱ��3��ȡƽ��ֵ */
    temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
	temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
	temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;
	/* ����3�����ݵĲ�ֵ */
	m0=temp[0]-temp[1];
	m1=temp[1]-temp[2];
	m2=temp[2]-temp[0];
	/* ��������ֵȡ����ֵ */
	m0=m0>0?m0:(-m0);
    m1=m1>0?m1:(-m1);
	m2=m2>0?m2:(-m2);
	/* �жϾ��Բ�ֵ�Ƿ񶼳�����ֵ���ޣ������3�����Բ�ֵ����������ֵ�����ж���β�����ΪҰ��,���������㣬��ֵ����ȡΪ2 */
	if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) return 0;
	/* �������ǵ�ƽ��ֵ��ͬʱ��ֵ��screen */ 
	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.x=(temp[0]+temp[2])/2;
	  else 
	    screen.x=(temp[0]+temp[1])/2;	
	}
	else if(m2<m1) 
	  screen.x=(temp[0]+temp[2])/2;
	else 
	  screen.x=(temp[1]+temp[2])/2;

	/* ͬ�� ����Y��ƽ��ֵ */
    temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3;
	temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3;
	temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3;
	m0=temp[0]-temp[1];
	m1=temp[1]-temp[2];
	m2=temp[2]-temp[0];
	m0=m0>0?m0:(-m0);
	m1=m1>0?m1:(-m1);
	m2=m2>0?m2:(-m2);
	if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) return 0;

	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.y=(temp[0]+temp[2])/2;
	  else 
	    screen.y=(temp[0]+temp[1])/2;	
  }
	else if(m2<m1) 
	   screen.y=(temp[0]+temp[2])/2;
	else
	   screen.y=(temp[1]+temp[2])/2;

	return &screen;
  }  
  return 0; 
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
