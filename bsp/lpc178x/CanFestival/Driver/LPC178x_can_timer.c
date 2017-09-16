#include "Slave_board.h"
//#include <stdbool.h>
#include "canfestival.h"
#include "can_timer.h"


/************************** Modul variables **********************************/
// Store the last timer value to calculate the elapsed time
static TIMEVAL last_time_set = TIMEVAL_MAX;

void setTimer(TIMEVAL value)
{
	uint32_t capture;

	capture = BRD_TIMER_CAN->TC;
	TIM_UpdateMatchValue(BRD_TIMER_CAN, 0, capture + value);
}

TIMEVAL getElapsedTime(void)
{
	uint32_t timer = BRD_TIMER_CAN->TC;
  /* Calculate the time difference */
  /* return timer > last_time_set ? timer - last_time_set : last_time_set - timer; */
	/* I think this should be fixed to
	   16 bit substraction */
	timer = timer - last_time_set;
//	return timer > last_time_set ? timer - last_time_set : TIMEVAL_MAX - (last_time_set - timer);
	return timer;
}

void CAN_Timer_start(void)
{
	TIM_TIMERCFG_Type TIM_ConfigStruct;
  TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
	// Initialize timer 0, prescale count time of 1uS
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;//;TIM_PRESCALE_TICKVAL
	TIM_ConfigStruct.PrescaleValue	= 1;

	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = FALSE;//TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	//Toggle MR0.0 pin if MR0 matches it
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_NOTHING;
	// Set Match value, count value of TIMEVAL_MAX (TIMEVAL_MAX * 1uS = TIMEVAL_MAXus)
	TIM_MatchConfigStruct.MatchValue   = 0x35000;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(BRD_TIMER_CAN, TIM_TIMER_MODE, &TIM_ConfigStruct);
	TIM_ConfigMatch(BRD_TIMER_CAN, &TIM_MatchConfigStruct);

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(BRD_TIM_INTR_CAN, ((0x01<<3)|0x01));

	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(BRD_TIM_INTR_CAN);

	// To start timer
	TIM_Cmd(BRD_TIMER_CAN, ENABLE);
	
	last_time_set = BRD_TIMER_CAN->TC;
	
// // 	  BRD_TIMER_CAN->TCR  = 0x02;
// //     BRD_TIMER_CAN->IR   = 1;
// //     BRD_TIMER_CAN->CTCR = 0;
// //     BRD_TIMER_CAN->TC   = 0;
// //     BRD_TIMER_CAN->PR   = 60;
// //     BRD_TIMER_CAN->MR0  = 5000;
// //     BRD_TIMER_CAN->MCR  = 0x03;

// //     NVIC_EnableIRQ(BRD_TIM_INTR_CAN);
// //     BRD_TIMER_CAN->TCR  = 0x01;

//     LPC_SC->PCONP |= (1<<1);                       //??????  
//       
//     LPC_TIM1->TCR |= (1<<1);                       //?????  
//     LPC_TIM1->PR   = 0x00;                           //?????  
//     LPC_TIM0->MR0  = 100;                          //????????????????  
//     LPC_TIM0->IR  |= 0XFF;                           //????????  
//     LPC_TIM0->MCR |= (/*(1<<1)|*/(1<<0));                //MR0?TC?????????,TC??  
//     LPC_TIM0->TCR  = (1<<0);                       //?????  
//   
//     NVIC_EnableIRQ(TIMER0_IRQn);                    /* enable irq in nvic              */

}

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		TIMER0 interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
uint32_t time;
void TIMER0_IRQHandler(void)
{
	if (TIM_GetIntStatus(BRD_TIMER_CAN, TIM_MR0_INT)== SET)
	{
		TIM_ClearIntPending(BRD_TIMER_CAN, TIM_MR0_INT);
		last_time_set = BRD_TIMER_CAN->TC;
		TimeDispatch();

	}
// 	
// 		time++;
// 		if(time >= 5)
// 		{
// 			rt_kprintf("time inc");
// 			time = 0;
// 		}
// 		TIM_ClearIntPending(BRD_TIMER_CAN, TIM_MR0_INT);
}
