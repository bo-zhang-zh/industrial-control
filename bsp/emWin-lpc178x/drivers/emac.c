#include <rtthread.h>
#include "emac.h"
#include "lwipopts.h"
#include <netif/ethernetif.h>
#include "LPC177x_8x.h"
#include "lpc177x_8x_pinsel.h"


#define EMAC_PHY_AUTO		0
#define EMAC_PHY_10MBIT		1
#define EMAC_PHY_100MBIT	2

#define MAX_ADDR_LEN 6
struct lpc17xx_emac
{
	/* inherit from ethernet device */
	struct eth_device parent;

	rt_uint8_t phy_mode;

	/* interface address info. */
	rt_uint8_t  dev_addr[MAX_ADDR_LEN];		/* hw address	*/
};
static struct lpc17xx_emac lpc17xx_emac_device;
static struct rt_semaphore sem_lock;
static struct rt_event tx_event;

/* Local Function Prototypes */
static void write_PHY (rt_uint32_t PhyAddr, rt_uint32_t PhyReg, rt_uint32_t Value);
static rt_uint16_t read_PHY (rt_uint32_t PhyAddr, rt_uint8_t PhyReg) ;

void ENET_IRQHandler(void)
{
	rt_uint32_t status;

    /* enter interrupt */
    rt_interrupt_enter();

	status = LPC_EMAC->IntStatus;

	if (status & INT_RX_DONE)
	{
		/* Disable EMAC RxDone interrupts. */
		LPC_EMAC->IntEnable = INT_TX_DONE;

		/* a frame has been received */
		eth_device_ready(&(lpc17xx_emac_device.parent));
	}
	else if (status & INT_TX_DONE)
	{
		/* set event */
		rt_event_send(&tx_event, 0x01);
	}

	if (status & INT_RX_OVERRUN)
	{
		rt_kprintf("rx overrun\n");
	}

	if (status & INT_TX_UNDERRUN)
	{
		rt_kprintf("tx underrun\n");
	}

	/* Clear the interrupt. */ 
	LPC_EMAC->IntClear = status; 
 
    /* leave interrupt */
    rt_interrupt_leave();
}

/* phy write */
//static void write_PHY (rt_uint32_t PhyReg, rt_uint32_t Value)
static void write_PHY (rt_uint32_t PhyAddr, rt_uint32_t PhyReg, rt_uint32_t Value)
{
	unsigned int tout;

	//LPC_EMAC->MADR = DP83848C_DEF_ADR | PhyReg;
	LPC_EMAC->MADR = (PhyAddr << 8) | PhyReg;
	LPC_EMAC->MWTD = Value;

	/* Wait utill operation completed */
	tout = 0;
	for (tout = 0; tout < MII_WR_TOUT; tout++)
	{
		if ((LPC_EMAC->MIND & MIND_BUSY) == 0)
		{
			break;
		}
	}
}

/* phy read */
//static rt_uint16_t read_PHY (rt_uint8_t PhyReg)
static rt_uint16_t read_PHY (rt_uint32_t PhyAddr, rt_uint8_t PhyReg)
{
	rt_uint32_t tout;

	//LPC_EMAC->MADR = DP83848C_DEF_ADR | PhyReg;
	LPC_EMAC->MADR = (PhyAddr << 8) | PhyReg;
	LPC_EMAC->MCMD = MCMD_READ;

	/* Wait until operation completed */
	tout = 0;
	for (tout = 0; tout < MII_RD_TOUT; tout++)
	{
		if ((LPC_EMAC->MIND & MIND_BUSY) == 0)
		{
			break;
		}
	}
	LPC_EMAC->MCMD = 0;
	return (LPC_EMAC->MRDD);
}

/* init rx descriptor */
rt_inline void rx_descr_init (void)
{
	rt_uint32_t i;

	for (i = 0; i < NUM_RX_FRAG; i++)
	{
		RX_DESC_PACKET(i)  = RX_BUF(i);
		RX_DESC_CTRL(i)    = RCTRL_INT | (ETH_FRAG_SIZE-1);
		RX_STAT_INFO(i)    = 0;
		RX_STAT_HASHCRC(i) = 0;
	}

	/* Set EMAC Receive Descriptor Registers. */
	LPC_EMAC->RxDescriptor    = RX_DESC_BASE;
	LPC_EMAC->RxStatus        = RX_STAT_BASE;
	LPC_EMAC->RxDescriptorNumber = NUM_RX_FRAG-1;

	/* Rx Descriptors Point to 0 */
	LPC_EMAC->RxConsumeIndex  = 0;
}

/* init tx descriptor */
rt_inline void tx_descr_init (void)
{
	rt_uint32_t i;

	for (i = 0; i < NUM_TX_FRAG; i++)
	{
		TX_DESC_PACKET(i) = TX_BUF(i);
		TX_DESC_CTRL(i)   = (1ul<<31) | (1ul<<30) | (1ul<<29) | (1ul<<28) | (1ul<<26) | (ETH_FRAG_SIZE-1);
		TX_STAT_INFO(i)   = 0;
	}

	/* Set EMAC Transmit Descriptor Registers. */
	LPC_EMAC->TxDescriptor    = TX_DESC_BASE;
	LPC_EMAC->TxStatus        = TX_STAT_BASE;
	LPC_EMAC->TxDescriptorNumber = NUM_TX_FRAG-1;

	/* Tx Descriptors Point to 0 */
	LPC_EMAC->TxProduceIndex  = 0;
}
static rt_err_t lpc17xx_linkup_init(rt_device_t dev)
{
	/* Initialize the EMAC ethernet controller. */
	rt_uint32_t regv, tout;//, id1, id2;

	/* Reset all EMAC internal modules. */
	LPC_EMAC->MAC1 = MAC1_RES_TX | MAC1_RES_MCS_TX | MAC1_RES_RX | MAC1_RES_MCS_RX |
				 MAC1_SIM_RES | MAC1_SOFT_RES;
	LPC_EMAC->Command = CR_REG_RES | CR_TX_RES | CR_RX_RES;

	/* A short delay after reset. */
	for (tout = 100; tout; tout--);

	/* Initialize MAC control registers. */
	LPC_EMAC->MAC1 = MAC1_PASS_ALL;
	LPC_EMAC->MAC2 = MAC2_CRC_EN | MAC2_PAD_EN;
	LPC_EMAC->MAXF = ETH_MAX_FLEN;
	LPC_EMAC->CLRT = CLRT_DEF;
	LPC_EMAC->IPGR = IPGR_DEF;

	/* PCLK=18MHz, clock select=6, MDC=18/6=3MHz */
	/* Enable Reduced MII interface. */
	LPC_EMAC->MCFG = MCFG_CLK_DIV20 | MCFG_RES_MII;
	for (tout = 100; tout; tout--);
	LPC_EMAC->MCFG = MCFG_CLK_DIV20;

	/* Enable Reduced MII interface. */
	LPC_EMAC->Command = CR_RMII | CR_PASS_RUNT_FRM | CR_PASS_RX_FILT;

	/* Reset Reduced MII Logic. */
	LPC_EMAC->SUPP = SUPP_RES_RMII | SUPP_SPEED;
	for (tout = 100; tout; tout--);
	LPC_EMAC->SUPP = SUPP_SPEED;

#if ETH_PHY_DEVICE == DP83848C
  regv = read_PHY (ETH_PHY_DEF_ADDR, PHY_REG_STS);
	/* Configure Full/Half Duplex mode. */
	if (regv & 0x0004)
	{
		/* Full duplex is enabled. */
		LPC_EMAC->MAC2    |= MAC2_FULL_DUP;
		LPC_EMAC->Command |= CR_FULL_DUP;
		LPC_EMAC->IPGT     = IPGT_FULL_DUP;
	}
	else
	{
		/* Half duplex mode. */
		LPC_EMAC->IPGT = IPGT_HALF_DUP;
	}

	/* Configure 100MBit/10MBit mode. */
	if (regv & 0x0002)
	{
		/* 10MBit mode. */
		LPC_EMAC->SUPP = 0;
	}
	else
	{
		/* 100MBit mode. */
		LPC_EMAC->SUPP = SUPP_SPEED;
	}
#elif ETH_PHY_DEVICE == LAN8720A
	  regv = read_PHY (ETH_PHY_DEF_ADDR, LAN8720_PHY_REG_PHYCSR);
    regv &= 0x001C;
	if (regv & 0x0018)//100MBit full-duplex
	{
		/* 100MBit mode. */
		LPC_EMAC->SUPP = SUPP_SPEED;
		/* Full duplex is enabled. */
		LPC_EMAC->MAC2    |= MAC2_FULL_DUP;
		LPC_EMAC->Command |= CR_FULL_DUP;
		LPC_EMAC->IPGT     = IPGT_FULL_DUP;
	}
	else if (regv & 0x0008)//100MBit half-duplex
	{
		/* 100MBit mode. */
		LPC_EMAC->SUPP = SUPP_SPEED;
		/* Half duplex mode. */
		LPC_EMAC->IPGT = IPGT_HALF_DUP;
	}	
	else if (regv & 0x0014)//10MBit full-duplex
	{
		/* 10MBit mode. */
		LPC_EMAC->SUPP = 0;
		/* Full duplex is enabled. */
		LPC_EMAC->MAC2    |= MAC2_FULL_DUP;
		LPC_EMAC->Command |= CR_FULL_DUP;
		LPC_EMAC->IPGT     = IPGT_FULL_DUP;
	}	
	else// if (regv & 0x0004)//10MBit half-duplex
	{
		/* 10MBit mode. */
		LPC_EMAC->SUPP = 0;
		/* Half duplex mode. */
		LPC_EMAC->IPGT = IPGT_HALF_DUP;
	}
#endif

	/* Set the Ethernet MAC Address registers */
	LPC_EMAC->SA0 = (lpc17xx_emac_device.dev_addr[1]<<8) | lpc17xx_emac_device.dev_addr[0];
	LPC_EMAC->SA1 = (lpc17xx_emac_device.dev_addr[3]<<8) | lpc17xx_emac_device.dev_addr[2];
	LPC_EMAC->SA2 = (lpc17xx_emac_device.dev_addr[5]<<8) | lpc17xx_emac_device.dev_addr[4];

	/* Initialize Tx and Rx DMA Descriptors */
	rx_descr_init ();
	tx_descr_init ();

	/* Receive Broadcast and Perfect Match Packets */
	LPC_EMAC->RxFilterCtrl = RFC_BCAST_EN | RFC_PERFECT_EN;

	/* Reset all interrupts */
	LPC_EMAC->IntClear  = 0xFFFF;

	/* Enable EMAC interrupts. */
	LPC_EMAC->IntEnable = INT_RX_DONE | INT_TX_DONE;

	/* Enable receive and transmit mode of MAC Ethernet core */
	LPC_EMAC->Command  |= (CR_RX_EN | CR_TX_EN);
	LPC_EMAC->MAC1     |= MAC1_REC_EN;

	/* Enable the ENET Interrupt */
	NVIC_EnableIRQ(ENET_IRQn);

	return RT_EOK;
}

static rt_err_t lpc17xx_emac_reg_init(rt_device_t dev)
{
	/* Initialize the EMAC ethernet controller. */
	rt_uint32_t tout;//, regv, id1, id2;

	/* Power Up the EMAC controller. */
	LPC_SC->PCONP |= 0x40000000;

	/* Enable P1 Ethernet Pins. */
	//LPC_PINCON->PINSEL2 = 0x50150105;
	//LPC_PINCON->PINSEL3 = (LPC_PINCON->PINSEL3 & ~0x0000000F) | 0x00000005;
	PINSEL_ConfigPin(1,0,1);
	PINSEL_ConfigPin(1,1,1);
	PINSEL_ConfigPin(1,4,1);
	PINSEL_ConfigPin(1,8,1);
	PINSEL_ConfigPin(1,9,1);
	PINSEL_ConfigPin(1,10,1);
	PINSEL_ConfigPin(1,14,1);
	PINSEL_ConfigPin(1,15,1);
	PINSEL_ConfigPin(1,16,1);
	PINSEL_ConfigPin(1,17,1);
	

	/* Reset all EMAC internal modules. */
	LPC_EMAC->MAC1 = MAC1_RES_TX | MAC1_RES_MCS_TX | MAC1_RES_RX | MAC1_RES_MCS_RX |
				 MAC1_SIM_RES | MAC1_SOFT_RES;
	LPC_EMAC->Command = CR_REG_RES | CR_TX_RES | CR_RX_RES;

	/* A short delay after reset. */
	for (tout = 100; tout; tout--);

	/* Initialize MAC control registers. */
	LPC_EMAC->MAC1 = MAC1_PASS_ALL;
	LPC_EMAC->MAC2 = MAC2_CRC_EN | MAC2_PAD_EN;
	LPC_EMAC->MAXF = ETH_MAX_FLEN;
	LPC_EMAC->CLRT = CLRT_DEF;
	LPC_EMAC->IPGR = IPGR_DEF;

	/* PCLK=18MHz, clock select=6, MDC=18/6=3MHz */
	/* Enable Reduced MII interface. */
	LPC_EMAC->MCFG = MCFG_CLK_DIV20 | MCFG_RES_MII;
	for (tout = 100; tout; tout--);
	LPC_EMAC->MCFG = MCFG_CLK_DIV20;

	/* Enable Reduced MII interface. */
	LPC_EMAC->Command = CR_RMII | CR_PASS_RUNT_FRM | CR_PASS_RX_FILT;

	/* Reset Reduced MII Logic. */
	LPC_EMAC->SUPP = SUPP_RES_RMII | SUPP_SPEED;
	for (tout = 100; tout; tout--);
	LPC_EMAC->SUPP = SUPP_SPEED;

#if ETH_PHY_DEVICE == DP83848C
		/* Full duplex is enabled. */
		LPC_EMAC->MAC2    |= MAC2_FULL_DUP;
		LPC_EMAC->Command |= CR_FULL_DUP;
		LPC_EMAC->IPGT     = IPGT_FULL_DUP;

		/* 100MBit mode. */
		LPC_EMAC->SUPP = SUPP_SPEED;
#elif ETH_PHY_DEVICE == LAN8720A
		/* 100MBit mode. */
		LPC_EMAC->SUPP = SUPP_SPEED;
		/* Full duplex is enabled. */
		LPC_EMAC->MAC2    |= MAC2_FULL_DUP;
		LPC_EMAC->Command |= CR_FULL_DUP;
		LPC_EMAC->IPGT     = IPGT_FULL_DUP;
#endif

	/* Set the Ethernet MAC Address registers */
	LPC_EMAC->SA0 = (lpc17xx_emac_device.dev_addr[1]<<8) | lpc17xx_emac_device.dev_addr[0];
	LPC_EMAC->SA1 = (lpc17xx_emac_device.dev_addr[3]<<8) | lpc17xx_emac_device.dev_addr[2];
	LPC_EMAC->SA2 = (lpc17xx_emac_device.dev_addr[5]<<8) | lpc17xx_emac_device.dev_addr[4];

	/* Initialize Tx and Rx DMA Descriptors */
	rx_descr_init ();
	tx_descr_init ();

	/* Receive Broadcast and Perfect Match Packets */
	LPC_EMAC->RxFilterCtrl = RFC_BCAST_EN | RFC_PERFECT_EN;

	/* Reset all interrupts */
	LPC_EMAC->IntClear  = 0xFFFF;

	/* Enable EMAC interrupts. */
	LPC_EMAC->IntEnable = INT_RX_DONE | INT_TX_DONE;

	/* Enable receive and transmit mode of MAC Ethernet core */
	LPC_EMAC->Command  |= (CR_RX_EN | CR_TX_EN);
	LPC_EMAC->MAC1     |= MAC1_REC_EN;

	/* Enable the ENET Interrupt */
	NVIC_EnableIRQ(ENET_IRQn);

	return RT_EOK;
}

static rt_err_t lpc17xx_emac_init(rt_device_t dev)
{
	/* Initialize the EMAC ethernet controller. */
	rt_uint32_t tout, regv;//, id1, id2;

	/* Power Up the EMAC controller. */
	LPC_SC->PCONP |= 0x40000000;

	/* Enable P1 Ethernet Pins. */
	//LPC_PINCON->PINSEL2 = 0x50150105;
	//LPC_PINCON->PINSEL3 = (LPC_PINCON->PINSEL3 & ~0x0000000F) | 0x00000005;
	PINSEL_ConfigPin(1,0,1);
	PINSEL_ConfigPin(1,1,1);
	PINSEL_ConfigPin(1,4,1);
	PINSEL_ConfigPin(1,8,1);
	PINSEL_ConfigPin(1,9,1);
	PINSEL_ConfigPin(1,10,1);
	PINSEL_ConfigPin(1,14,1);
	PINSEL_ConfigPin(1,15,1);
	PINSEL_ConfigPin(1,16,1);
	PINSEL_ConfigPin(1,17,1);
	

	/* Reset all EMAC internal modules. */
	LPC_EMAC->MAC1 = MAC1_RES_TX | MAC1_RES_MCS_TX | MAC1_RES_RX | MAC1_RES_MCS_RX |
				 MAC1_SIM_RES | MAC1_SOFT_RES;
	LPC_EMAC->Command = CR_REG_RES | CR_TX_RES | CR_RX_RES;

	/* A short delay after reset. */
	for (tout = 100; tout; tout--);

	/* Initialize MAC control registers. */
	LPC_EMAC->MAC1 = MAC1_PASS_ALL;
	LPC_EMAC->MAC2 = MAC2_CRC_EN | MAC2_PAD_EN;
	LPC_EMAC->MAXF = ETH_MAX_FLEN;
	LPC_EMAC->CLRT = CLRT_DEF;
	LPC_EMAC->IPGR = IPGR_DEF;

	/* PCLK=18MHz, clock select=6, MDC=18/6=3MHz */
	/* Enable Reduced MII interface. */
	LPC_EMAC->MCFG = MCFG_CLK_DIV20 | MCFG_RES_MII;
	for (tout = 100; tout; tout--);
	LPC_EMAC->MCFG = MCFG_CLK_DIV20;

	/* Enable Reduced MII interface. */
	LPC_EMAC->Command = CR_RMII | CR_PASS_RUNT_FRM | CR_PASS_RX_FILT;

	/* Reset Reduced MII Logic. */
	LPC_EMAC->SUPP = SUPP_RES_RMII | SUPP_SPEED;
	for (tout = 100; tout; tout--);
	LPC_EMAC->SUPP = SUPP_SPEED;

	/* Put the PHY in reset mode */
	write_PHY (ETH_PHY_DEF_ADDR, PHY_REG_BMCR, 0x8000);
	for (tout = 1000; tout; tout--);

	/* Wait for hardware reset to end. */
	for (tout = 0; tout < 0x100000; tout++)
	{
		regv = read_PHY (ETH_PHY_DEF_ADDR, PHY_REG_BMCR);
		if (!(regv & 0x8000))
		{
			/* Reset complete */
			break;
		}
	}
	if (tout >= 0x100000){ rt_kprintf("eth_e1");return -RT_ERROR; }/* reset failed */

// 	/* Check if this is a DP83848C PHY. */
// 	id1 = read_PHY (ETH_PHY_DEF_ADDR, PHY_REG_IDR1);
// 	id2 = read_PHY (ETH_PHY_DEF_ADDR, PHY_REG_IDR2);

/*#if ETH_PHY_DEVICE == DP83848C
  if (((id1 << 16) | (id2 & 0xFFF0)) != DP83848C_ID)
#else if ETH_PHY_DEVICE == LAN8720A
  if (((id1 << 16) | (id2 & 0xFFF0)) != LAN8720A_ID)
#endif
		//return -RT_ERROR;
	{ rt_kprintf("eth_e2:%x", (id1 << 16) | (id2 & 0xFFF0));return -RT_ERROR; }
*/
	/* Configure the PHY device */
	switch (lpc17xx_emac_device.phy_mode)
	{
	case EMAC_PHY_AUTO:
		/* Use auto negotiation about the link speed. */
		write_PHY (ETH_PHY_DEF_ADDR, PHY_REG_BMCR, PHY_AUTO_NEG);
		/* Wait to complete Auto_Negotiation. */
	  for (tout = 0; tout < 0x100000; tout++)
		{
			regv = read_PHY (ETH_PHY_DEF_ADDR, PHY_REG_BMSR);
			if (regv & 0x0020)
			{
				/* Auto negotiation Complete. */
				break;
			}
		}
		break;
	case EMAC_PHY_10MBIT:
		/* Connect at 10MBit */
		write_PHY (ETH_PHY_DEF_ADDR, PHY_REG_BMCR, PHY_FULLD_10M);
		break;
	case EMAC_PHY_100MBIT:
		/* Connect at 100MBit */
		write_PHY (ETH_PHY_DEF_ADDR, PHY_REG_BMCR, PHY_FULLD_100M);
		break;
	}
	if (tout >= 0x100000){ rt_kprintf("eth_e3");return -RT_ERROR; }// return -RT_ERROR; // auto_neg failed

	/* Check the link status. */
	for (tout = 0; tout < 0x10000; tout++)
	//for (tout = 0; tout < 0x1000; tout++)
	{
#if ETH_PHY_DEVICE == DP83848C
		regv = read_PHY (ETH_PHY_DEF_ADDR, PHY_REG_STS);
		if (regv & 0x0001)
		{
			/* Link is on. */
			break;
		}
#elif ETH_PHY_DEVICE == LAN8720A
	  regv = read_PHY (ETH_PHY_DEF_ADDR, PHY_REG_BMSR);
		if (regv & 0x0004)
		{
			/* Link is on. */
			break;
		}
#endif
	}
	if (tout >= 0x10000){ rt_kprintf("eth_e4");return -RT_ERROR; }// return -RT_ERROR;
  //if (tout >= 0x1000){ rt_kprintf("eth_e4");return -RT_ERROR; }// return -RT_ERROR;
	
#if ETH_PHY_DEVICE == DP83848C
  regv = read_PHY (ETH_PHY_DEF_ADDR, PHY_REG_STS);
	/* Configure Full/Half Duplex mode. */
	if (regv & 0x0004)
	{
		/* Full duplex is enabled. */
		LPC_EMAC->MAC2    |= MAC2_FULL_DUP;
		LPC_EMAC->Command |= CR_FULL_DUP;
		LPC_EMAC->IPGT     = IPGT_FULL_DUP;
	}
	else
	{
		/* Half duplex mode. */
		LPC_EMAC->IPGT = IPGT_HALF_DUP;
	}

	/* Configure 100MBit/10MBit mode. */
	if (regv & 0x0002)
	{
		/* 10MBit mode. */
		LPC_EMAC->SUPP = 0;
	}
	else
	{
		/* 100MBit mode. */
		LPC_EMAC->SUPP = SUPP_SPEED;
	}
#elif ETH_PHY_DEVICE == LAN8720A
	  regv = read_PHY (ETH_PHY_DEF_ADDR, LAN8720_PHY_REG_PHYCSR);
    regv &= 0x001C;
	if (regv & 0x0018)//100MBit full-duplex
	{
		/* 100MBit mode. */
		LPC_EMAC->SUPP = SUPP_SPEED;
		/* Full duplex is enabled. */
		LPC_EMAC->MAC2    |= MAC2_FULL_DUP;
		LPC_EMAC->Command |= CR_FULL_DUP;
		LPC_EMAC->IPGT     = IPGT_FULL_DUP;
	}
	else if (regv & 0x0008)//100MBit half-duplex
	{
		/* 100MBit mode. */
		LPC_EMAC->SUPP = SUPP_SPEED;
		/* Half duplex mode. */
		LPC_EMAC->IPGT = IPGT_HALF_DUP;
	}	
	else if (regv & 0x0014)//10MBit full-duplex
	{
		/* 10MBit mode. */
		LPC_EMAC->SUPP = 0;
		/* Full duplex is enabled. */
		LPC_EMAC->MAC2    |= MAC2_FULL_DUP;
		LPC_EMAC->Command |= CR_FULL_DUP;
		LPC_EMAC->IPGT     = IPGT_FULL_DUP;
	}	
	else// if (regv & 0x0004)//10MBit half-duplex
	{
		/* 10MBit mode. */
		LPC_EMAC->SUPP = 0;
		/* Half duplex mode. */
		LPC_EMAC->IPGT = IPGT_HALF_DUP;
	}
#endif

	/* Set the Ethernet MAC Address registers */
	LPC_EMAC->SA0 = (lpc17xx_emac_device.dev_addr[1]<<8) | lpc17xx_emac_device.dev_addr[0];
	LPC_EMAC->SA1 = (lpc17xx_emac_device.dev_addr[3]<<8) | lpc17xx_emac_device.dev_addr[2];
	LPC_EMAC->SA2 = (lpc17xx_emac_device.dev_addr[5]<<8) | lpc17xx_emac_device.dev_addr[4];

	/* Initialize Tx and Rx DMA Descriptors */
	rx_descr_init ();
	tx_descr_init ();

	/* Receive Broadcast and Perfect Match Packets */
	LPC_EMAC->RxFilterCtrl = RFC_BCAST_EN | RFC_PERFECT_EN;

	/* Reset all interrupts */
	LPC_EMAC->IntClear  = 0xFFFF;

	/* Enable EMAC interrupts. */
	LPC_EMAC->IntEnable = INT_RX_DONE | INT_TX_DONE;

	/* Enable receive and transmit mode of MAC Ethernet core */
	LPC_EMAC->Command  |= (CR_RX_EN | CR_TX_EN);
	LPC_EMAC->MAC1     |= MAC1_REC_EN;

	/* Enable the ENET Interrupt */
	NVIC_EnableIRQ(ENET_IRQn);

	return RT_EOK;
}

static rt_err_t lpc17xx_emac_open(rt_device_t dev, rt_uint16_t oflag)
{
	return RT_EOK;
}

static rt_err_t lpc17xx_emac_close(rt_device_t dev)
{
	return RT_EOK;
}

static rt_size_t lpc17xx_emac_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
	rt_set_errno(-RT_ENOSYS);
	return 0;
}

static rt_size_t lpc17xx_emac_write (rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
	rt_set_errno(-RT_ENOSYS);
	return 0;
}

static rt_err_t lpc17xx_emac_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
	switch (cmd)
	{
	case NIOCTL_GADDR:
		/* get mac address */
		if (args) rt_memcpy(args, lpc17xx_emac_device.dev_addr, 6);
		else return -RT_ERROR;
		break;

	default :
		break;
	}

	return RT_EOK;
}

/* EtherNet Device Interface */
/* transmit packet. */
rt_err_t lpc17xx_emac_tx( rt_device_t dev, struct pbuf* p)
{
	rt_uint32_t Index, IndexNext;
	struct pbuf *q;
	rt_uint8_t *ptr;

	/* calculate next index */
	IndexNext = LPC_EMAC->TxProduceIndex + 1;
	if(IndexNext > LPC_EMAC->TxDescriptorNumber) IndexNext = 0;

	/* check whether block is full */
	while (IndexNext == LPC_EMAC->TxConsumeIndex)
	{
		rt_err_t result;
		rt_uint32_t recved;
		
		/* there is no block yet, wait a flag */
		result = rt_event_recv(&tx_event, 0x01, 
			RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &recved);

		RT_ASSERT(result == RT_EOK);
	}

	/* lock EMAC device */
	rt_sem_take(&sem_lock, RT_WAITING_FOREVER);

	/* get produce index */
	Index = LPC_EMAC->TxProduceIndex;

	/* calculate next index */
	IndexNext = LPC_EMAC->TxProduceIndex + 1;
	if(IndexNext > LPC_EMAC->TxDescriptorNumber)
		IndexNext = 0;

	/* copy data to tx buffer */
	q = p;
	ptr = (rt_uint8_t*)TX_BUF(Index);
	while (q)
	{
		memcpy(ptr, q->payload, q->len);
		ptr += q->len;
		q = q->next;
	}

	TX_DESC_CTRL(Index) &= ~0x7ff;
	TX_DESC_CTRL(Index) |= (p->tot_len - 1) & 0x7ff;

	/* change index to the next */
	LPC_EMAC->TxProduceIndex = IndexNext;

	/* unlock EMAC device */
	rt_sem_release(&sem_lock);

	return RT_EOK;
}

/* reception packet. */
struct pbuf *lpc17xx_emac_rx(rt_device_t dev)
{
	struct pbuf* p;
	rt_uint32_t size;
	rt_uint32_t Index;

	/* init p pointer */
	p = RT_NULL;

	/* lock EMAC device */
	rt_sem_take(&sem_lock, RT_WAITING_FOREVER);

	Index = LPC_EMAC->RxConsumeIndex;
	if(Index != LPC_EMAC->RxProduceIndex)
	{
		size = (RX_STAT_INFO(Index) & 0x7ff)+1;
		if (size > ETH_FRAG_SIZE) size = ETH_FRAG_SIZE;

		/* allocate buffer */
		p = pbuf_alloc(PBUF_LINK, size, PBUF_RAM);
		if (p != RT_NULL)
		{
			struct pbuf* q;
			rt_uint8_t *ptr;

			ptr = (rt_uint8_t*)RX_BUF(Index);
			for (q = p; q != RT_NULL; q= q->next)
			{
				memcpy(q->payload, ptr, q->len);
				ptr += q->len;
			}
		}
		
		/* move Index to the next */
		if(++Index > LPC_EMAC->RxDescriptorNumber)
			Index = 0;

		/* set consume index */
		LPC_EMAC->RxConsumeIndex = Index;
	}
	else
	{
		/* Enable RxDone interrupt */
		LPC_EMAC->IntEnable = INT_RX_DONE | INT_TX_DONE;
	}

	/* unlock EMAC device */
	rt_sem_release(&sem_lock);

	return p;
}


/* PHY: LAN8720 */
static uint8_t phy_speed = 0;
static void phy_monitor_thread_entry(void *parameter)
{
    uint8_t phy_addr = 0xFF;
    uint8_t phy_speed_new = 0;

    /* phy search */
    {
        rt_uint32_t i;
        rt_uint16_t temp;

        for(i=1; i<=0x1F; i++)
        {
            temp = read_PHY(i, 0x02);

            if( temp != 0xFFFF )
            {
                phy_addr = i;
                break;
            }
        }
    } /* phy search */

    if(phy_addr == 0xFF)
    {
        rt_kprintf("phy not probe!\r\n");
        return;
    }
    else
    {
        rt_kprintf("found a phy, address:0x%02X\r\n", phy_addr);
    }
		
//Has been initialized already before it
    /* RESET PHY */  
    rt_kprintf("RESET PHY!\r\n");
    write_PHY (phy_addr, PHY_REG_BMCR, 0x8000);
    rt_thread_delay(RT_TICK_PER_SECOND * 2);
    write_PHY (phy_addr, PHY_REG_BMCR, PHY_AUTO_NEG);

    while(1)
    {
        uint16_t status  = read_PHY(phy_addr, PHY_REG_BMSR);
        //rt_kprintf("LAN8720A status:0x%04X\r\n", status);

        phy_speed_new = 0;

        if(status & (PHY_AutoNego_Complete | PHY_Linked_Status))
        {
						phy_speed_new = read_PHY (ETH_PHY_DEF_ADDR, LAN8720_PHY_REG_PHYCSR);
					  phy_speed_new &= 0x001C;
					  //rt_kprintf("LAN8720A REG 31:0x%04X\r\n", phy_speed_new);
        }

        /* linkchange */
        if(phy_speed_new != phy_speed)
        {
            if(status & PHY_Linked_Status)
            {
							  rt_kprintf("link up ");
								if (phy_speed_new & 0x0018)//100MBit full-duplex
								{
									rt_kprintf("100Mbps full-duplex\r\n");
								}
								else if (phy_speed_new & 0x0008)//100MBit half-duplex
								{
									rt_kprintf("100Mbps half-duplex\r\n");
								}	
								else if (phy_speed_new & 0x0014)//10MBit full-duplex
								{
									rt_kprintf("10Mbps full-duplex\r\n");
								}	
								else if (phy_speed_new & 0x0004)//10MBit half-duplex
								{
									rt_kprintf("10Mbps half-duplex\r\n");
								}	
                lpc17xx_linkup_init((rt_device_t)&lpc17xx_emac_device);

                /* send link up. */
                eth_device_linkchange(&lpc17xx_emac_device.parent, RT_TRUE);
            } /* link up. */
            else
            {
							  if (phy_speed != 0)
                    rt_kprintf("link down\r\n");
                /* send link down. */
                eth_device_linkchange(&lpc17xx_emac_device.parent, RT_FALSE);
            } /* link down. */

            phy_speed = phy_speed_new;
        } /* linkchange */

        rt_thread_delay(RT_TICK_PER_SECOND);
    } /* while(1) */
}

void lpc17xx_emac_hw_init(void)
{
	rt_event_init(&tx_event, "tx_event", RT_IPC_FLAG_FIFO);
	rt_sem_init(&sem_lock, "eth_lock", 1, RT_IPC_FLAG_FIFO);

	/* set auto negotiation mode */
	lpc17xx_emac_device.phy_mode = EMAC_PHY_AUTO;

	// OUI 00-60-37 NXP Semiconductors
	lpc17xx_emac_device.dev_addr[0] = 0x00;
	lpc17xx_emac_device.dev_addr[1] = 0x60;
	lpc17xx_emac_device.dev_addr[2] = 0x37;
	/* set mac address: (only for test) */
	lpc17xx_emac_device.dev_addr[3] = 0x12;
	lpc17xx_emac_device.dev_addr[4] = 0x34;
	lpc17xx_emac_device.dev_addr[5] = 0x56;

	lpc17xx_emac_device.parent.parent.init		= lpc17xx_emac_reg_init;
	lpc17xx_emac_device.parent.parent.open		= lpc17xx_emac_open;
	lpc17xx_emac_device.parent.parent.close		= lpc17xx_emac_close;
	lpc17xx_emac_device.parent.parent.read		= lpc17xx_emac_read;
	lpc17xx_emac_device.parent.parent.write		= lpc17xx_emac_write;
	lpc17xx_emac_device.parent.parent.control	= lpc17xx_emac_control;
	lpc17xx_emac_device.parent.parent.user_data	= RT_NULL;

	lpc17xx_emac_device.parent.eth_rx			= lpc17xx_emac_rx;
	lpc17xx_emac_device.parent.eth_tx			= lpc17xx_emac_tx;

	eth_device_init(&(lpc17xx_emac_device.parent), "e0");
	
	/* start phy monitor */
	{
			rt_thread_t tid;
			tid = rt_thread_create("phy",
														 phy_monitor_thread_entry,
														 RT_NULL,
														 512,
														 RT_THREAD_PRIORITY_MAX - 2,
														 2);
			if (tid != RT_NULL)
					rt_thread_startup(tid);
	}
}
