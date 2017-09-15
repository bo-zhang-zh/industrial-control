#include <rtthread.h>

#include <system_LPC177x_8x.h>
#include "LPC177x_8x.h"
#include "lpc177x_8x_pinsel.h"
#include "sdram.h"

#define EMC_NS2CLK(ns, nsPerClk)	((ns + nsPerClk - 1) / nsPerClk)

static void delayMs(int a,int b)
{
    volatile unsigned int i;
    for(i=0;i<10000;i++);
}

static void EMC_GPIO_Init (void)
{
	volatile uint32_t i;
//	PINSEL_ConfigPin(2,14,1);	   /* P2.14 - /EMC_CS2 */
//	PINSEL_ConfigPin(2,15,1);	   /* P2.15 - /EMC_CS3 */
	PINSEL_ConfigPin(2,16,1);	   /* P2.16 - /EMC_CAS */
	PINSEL_ConfigPin(2,17,1);	   /* P2.17 - /EMC_RAS */
	PINSEL_ConfigPin(2,18,1);	   /* P2.18 - EMC_CLK[0] */
//	PINSEL_ConfigPin(2,19,1);	   /* P2.19 - EMC_CLK[1] */
	PINSEL_ConfigPin(2,20,1);	   /* P2.20 - EMC_DYCS0 */
//	PINSEL_ConfigPin(2,21,1);	   /* P2.21 - EMC_DYCS1 */
//  PINSEL_ConfigPin(2,22,1);	   /* P2.22 - EMC_DYCS2 */
//	PINSEL_ConfigPin(2,23,1);	   /* P2.23 - EMC_DYCS3 */
	PINSEL_ConfigPin(2,24,1);	   /* P2.24 - EMC_CKE0 */
//	PINSEL_ConfigPin(2,25,1);	   /* P2.25 - EMC_CKE1 */
//	PINSEL_ConfigPin(2,26,1);	   /* P2.26 - EMC_CKE2 */
//	PINSEL_ConfigPin(2,27,1);	   /* P2.27 - EMC_CKE3 */
	PINSEL_ConfigPin(2,28,1);	   /* P2.28 - EMC_DQM0 */
	PINSEL_ConfigPin(2,29,1);	   /* P2.29 - EMC_DQM1 */
//	PINSEL_ConfigPin(2,30,1);	   /* P2.30 - EMC_DQM2 */
//	PINSEL_ConfigPin(2,31,1);	   /* P2.31 - EMC_DQM3 */

//	PINSEL_ConfigPin(4,24,1);	   /* P4.24 - /EMC_OE */
	PINSEL_ConfigPin(4,25,1);	   /* P4.25 - /EMC_WE */
//	PINSEL_ConfigPin(4,30,1);	   /* P4.30 - /EMC_CS0 */
//	PINSEL_ConfigPin(4,31,1);	   /* P4.31 - /EMC_CS1 */
	for(i = 0; i < 16; i++)
	{
		PINSEL_ConfigPin(3,i,1);   /* P3.0-P3.15 - EMC_D[0-15] */
	}

	for(i = 0; i < 15; i++)
	{
		PINSEL_ConfigPin(4,i,1);   /* P4.0-P4.14 - EMC_A[0-14] */
	}
}

void SDRAM_Init (void)
{
    uint32_t i, dwtemp = dwtemp;
    uint16_t wtemp = wtemp;
	uint32_t mhz, nsPerClk;
    /* Enable External Memory Controller power/clock */
    LPC_SC->PCONP      |= 0x00000800;
    LPC_SC->EMCDLYCTL   = 0x00001010;
    LPC_EMC->Control = 0x00000001;
    LPC_EMC->Config  = 0x00000000;

    EMC_GPIO_Init();

	mhz = SystemCoreClock / 1000000;
	if (LPC_SC->EMCCLKSEL)
		mhz >>= 1;
	nsPerClk = 1000 / mhz;
    LPC_EMC->DynamicRP         = EMC_NS2CLK(20, nsPerClk); /* 20ns,  */
    LPC_EMC->DynamicRAS        = /*EMC_NS2CLK(42, nsPerClk);*/ 15; /* 42ns to 100K ns,  */
    LPC_EMC->DynamicSREX       = 1 - 1; /* tSRE, 1clk, */
    LPC_EMC->DynamicAPR        = 2 - 1; /* Not found!!! Estimated as 2clk, */
    LPC_EMC->DynamicDAL        = EMC_NS2CLK(20, nsPerClk) + 2; /* tDAL = tRP + tDPL = 20ns + 2clk  */
    LPC_EMC->DynamicWR         = 2 - 1; /* 2CLK,  */
    LPC_EMC->DynamicRC         = EMC_NS2CLK(63, nsPerClk); /* H57V2562GTR-75C tRC=63ns(min)*/
    LPC_EMC->DynamicRFC        = EMC_NS2CLK(63, nsPerClk); /* H57V2562GTR-75C tRFC=tRC */
    LPC_EMC->DynamicXSR        = 0x0000000F; /* exit self-refresh to active, 不知道，设为最久  */
    LPC_EMC->DynamicRRD        = EMC_NS2CLK(63, nsPerClk); /* 3clk, tRRD=15ns(min) */
    LPC_EMC->DynamicMRD        = 2 - 1; /* 2clk, tMRD=2clk(min) */

//    LPC_EMC->DynamicRP         = 0x00000002; /* 3clk=24ns,  */
//    LPC_EMC->DynamicRAS        = 0x00000005; /* 6clk=48ns,  */
//    LPC_EMC->DynamicSREX       = 0x00000001; /* 2clk, */
//    LPC_EMC->DynamicAPR        = 0x00000001; /* 2clk, */
//    LPC_EMC->DynamicDAL        = 0x00000005; /* 6clk,  */
//    LPC_EMC->DynamicWR         = 0x00000001; /* 2CLK,  */
//    LPC_EMC->DynamicRC         = 0x00000008; /* 9clk, H57V2562GTR-75C tRC=63ns(min)*/
//    LPC_EMC->DynamicRFC        = 0x00000008; /* 9clk, H57V2562GTR-75C tRFC=63ns(min) */
//    LPC_EMC->DynamicXSR        = 0x00000007; /* 8clk,  */
//    LPC_EMC->DynamicRRD        = 0x00000002; /* 3clk, tRRD=15ns(min) */
//    LPC_EMC->DynamicMRD        = 0x00000001; /* 2clk, tMRD=2clk(min) */

    LPC_EMC->DynamicReadConfig = 0x00000001; /* Command delayed strategy, using EMCCLKDELAY */
    /* H57V2562GTR-75C: tCL=3CLK, tRCD=20ns(min), 3 CLK=24ns */
    LPC_EMC->DynamicRasCas0    = 0x00000303; /* 3 RAS, 3 CAS latency */

    /* For Manley lpc1778 SDRAM: H57V2562GTR-75C, 256Mb, 16Mx16, 4 banks, row=13, column=9 */
#ifdef SDRAM_CONFIG_16BIT
    LPC_EMC->DynamicConfig0    = 0x680; /* 256Mb, 16Mx16, 4 banks, row=13, column=9, RBC */
#elif defined SDRAM_CONFIG_32BIT
    LPC_EMC->DynamicConfig0	= 0x4680; /* 256Mb, 16Mx16, 4 banks, row=13, column=9, RBC */
#endif
    delayMs(0, 100);

    LPC_EMC->DynamicControl    = 0x00000183; /* Issue NOP command */
    delayMs(0, 200);							  /* wait 200ms */

    LPC_EMC->DynamicControl    = 0x00000103; /* Issue PALL command */
    LPC_EMC->DynamicRefresh    = 0x00000002; /* ( n * 16 ) -> 32 clock cycles */
    for(i = 0; i < 0x80; i++);	              /* wait 128 AHB clock cycles */
    /* 64ms/8192=7.8125us, nx16x8.33ns<7.8125us, n<58.6*/
	wtemp = 64000000 / (1 << 13);
	wtemp -= 16;
	wtemp >>= 4;
	wtemp = wtemp * mhz / 1000;
    LPC_EMC->DynamicRefresh    = wtemp;

    LPC_EMC->DynamicControl    = 0x00000083; /* Issue MODE command */

#ifdef SDRAM_CONFIG_16BIT
    wtemp = *((volatile uint16_t *)(SDRAM_BASE_ADDR | (0x33<<12))); /* 8 burst, 3 CAS latency */
#elif defined SDRAM_CONFIG_32BIT
    dwtemp = *((volatile uint32_t *)(SDRAM_BASE_ADDR | (0x32<<13)));	/* 4 burst, 3 CAS latency */
#endif

    LPC_EMC->DynamicControl    = 0x00000000; /* Issue NORMAL command */
    LPC_EMC->DynamicConfig0 |= 0x80000; /* enable buffer */
    delayMs(0, 1);
}


