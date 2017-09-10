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
File    : BSP.c
Purpose : BSP for IAR LPC1788-SK
--------  END-OF-HEADER  ---------------------------------------------
*/

#define BSP_C
#include "BSP.h"
#include "LPC177x_8x.h"         // Device specific header file, contains CMSIS
#include "system_LPC177x_8x.h"  // Device specific header file, contains CMSIS

/*********************************************************************
*
*       SDRAM settings
*/


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

//
// Not used
//

/*********************************************************************
*
*       Typedefs
*
**********************************************************************
*/

//
// Not used
//

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

//
// Not used
//

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_Init()
*/
void BSP_Init(void) {
}

/*********************************************************************
*
*       LED switching routines
*       LEDs are switched on with low level on port lines
*/
void BSP_SetLED(int Index) {
  BSP_USE_PARA(Index);
  //
  // No dedicated LEDs available
  //
}

void BSP_ClrLED(int Index) {
  BSP_USE_PARA(Index);
  //
  // No dedicated LEDs available
  //
}

void BSP_ToggleLED(int Index) {
  BSP_USE_PARA(Index);
  //
  // No dedicated LEDs available
  //
}


/*********************************************************************
*
*       _DelayMs()
*
* Function description:
*   Starts a timer and waits for the given delay in ms.
*/
static void _DelayMs(U32 ms) {
  LPC_TIM0->TCR = 0x02;  // Reset timer
  LPC_TIM0->PR  = 0x00;  // Set prescaler to zero
  LPC_TIM0->MR0 = ms * (SystemCoreClock / (LPC_SC->PCLKSEL & 0x1F) / 1000 - 1);
  LPC_TIM0->IR  = 0xFF;  // Reset all interrrupts
  LPC_TIM0->MCR = 0x04;  // Stop timer on match
  LPC_TIM0->TCR = 0x01;  // Start timer
  //
  // Wait until delay time has elapsed
  //
  while (LPC_TIM0->TCR & 1);
}


/*********************************************************************
*
*       __low_level_init()
*
*       Initialize memory controller, clock generation and pll
*
*       Has to be modified, if another CPU clock frequency should be
*       used. This function is called during startup and
*       has to return 1 to perform segment initialization
*/
#ifdef __cplusplus
extern "C" {
#endif
int __low_level_init(void);  // Avoid "no ptototype" warning
#ifdef __cplusplus
  }
#endif
int __low_level_init(void) {
  SystemCoreClockUpdate();  // Ensure, the SystemCoreClock is set
  //
  // Init SDRAM, NAND- and NOR-flash
  //
  //_EMC_Init();
  SDRAM_32M_16BIT_Init();
  //
  //  Perform other initialization here, if required
  //
  return 1;
}

/****** End of File *************************************************/
