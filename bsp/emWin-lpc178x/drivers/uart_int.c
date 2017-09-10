/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2010-03-08     Bernard      The first version for LPC17xx
 * 2010-05-02     Aozima       update CMSIS to 130
 */

#include <rthw.h>
#include <rtthread.h>

#include "board.h"
#include "LPC177x_8x.h"
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_pinsel.h"

/**
 * @addtogroup LPC11xx
 */

/*@{*/
#define RT_UART_TX_BUFFER_SIZE    2048

struct rt_uart_lpc
{
	struct rt_device parent;

	LPC_UART_TypeDef * UART;
	UART_ID_Type UART_ID;
	IRQn_Type UART_IRQn;

	/* buffer for reception */
	rt_int32_t read_index, save_index;
	rt_uint8_t rx_buffer[RT_UART_RX_BUFFER_SIZE];
	/* buffer for transmit */
	rt_int32_t write_index, transmit_index;
	rt_uint8_t tx_buffer[RT_UART_TX_BUFFER_SIZE];
};

#ifdef RT_USING_UART0
struct rt_uart_lpc uart0_device;
#endif
#ifdef RT_USING_UART1
struct rt_uart_lpc uart1_device;
#endif

#ifdef RT_USING_UART0
void UART0_IRQHandler(void)
{
	rt_ubase_t level, iir;
    struct rt_uart_lpc* uart = &uart0_device;

	/* enter interrupt */
	rt_interrupt_enter();

    /* read IIR and clear it */
	iir = uart->UART->IIR;

	if (iir == UART_IIR_INTID_RDA)	    /* Receive Data Available */
	{
		/* Receive Data Available */
		level = rt_hw_interrupt_disable();
		if ((uart->save_index + 1) % RT_UART_RX_BUFFER_SIZE != uart->read_index)
		{
      uart->rx_buffer[uart->save_index] = uart->UART->RBR;
			uart->save_index = (uart->save_index + 1) % RT_UART_RX_BUFFER_SIZE;
		}
    rt_hw_interrupt_enable(level);

		/* invoke callback */
		if(uart->parent.rx_indicate != RT_NULL)
		{
      uart->parent.rx_indicate(&uart->parent, (uart->save_index - uart->read_index + RT_UART_RX_BUFFER_SIZE) % RT_UART_RX_BUFFER_SIZE);
		}
	}
	
	if (iir == UART_IIR_INTID_THRE)	    /* Transmit Data FIFO empty */
	{
		level = rt_hw_interrupt_disable();
		if (uart->write_index != uart->transmit_index)
		{
			//while (!(uart->UART->LSR & UART_LSR_THRE));
			uart->UART->THR = uart->tx_buffer[uart->transmit_index];			
			uart->transmit_index = (uart->transmit_index + 1) % RT_UART_TX_BUFFER_SIZE;
		}
		rt_hw_interrupt_enable(level);

		/* invoke callback */
// 		if(uart->parent.tx_complete != RT_NULL){}
	}
	/* leave interrupt */
	rt_interrupt_leave();

	return;
}
#endif

#ifdef RT_USING_UART1
void UART1_IRQHandler(void)
{
	rt_ubase_t level, iir;
    struct rt_uart_lpc* uart = &uart1_device;

	/* enter interrupt */
	rt_interrupt_enter();

    /* read IIR and clear it */
	iir = uart->UART->IIR;

	if (iir == UART_IIR_INTID_RDA)	    /* Receive Data Available */
	{
		/* Receive Data Available */
		level = rt_hw_interrupt_disable();
		if ((uart->save_index + 1) % RT_UART_RX_BUFFER_SIZE != uart->read_index)
		{
      uart->rx_buffer[uart->save_index] = uart->UART->RBR;
			uart->save_index = (uart->save_index + 1) % RT_UART_RX_BUFFER_SIZE;
		}
    rt_hw_interrupt_enable(level);

		/* invoke callback */
		if(uart->parent.rx_indicate != RT_NULL)
		{
      uart->parent.rx_indicate(&uart->parent, (uart->save_index - uart->read_index + RT_UART_RX_BUFFER_SIZE) % RT_UART_RX_BUFFER_SIZE);
		}
	}

	if (iir == UART_IIR_INTID_THRE)	    /* Transmit Data FIFO empty */
	{
		level = rt_hw_interrupt_disable();
		if (uart->write_index != uart->transmit_index)
		{
			//while (!(uart->UART->LSR & UART_LSR_THRE));
			uart->UART->THR = uart->tx_buffer[uart->transmit_index];			
			uart->transmit_index = (uart->transmit_index + 1) % RT_UART_TX_BUFFER_SIZE;
		}
		rt_hw_interrupt_enable(level);

		/* invoke callback */
// 		if(uart->parent.tx_complete != RT_NULL){}
	}
	/* leave interrupt */
	rt_interrupt_leave();

	return;
}
#endif

static rt_err_t rt_uart_init (rt_device_t dev)
{
	struct rt_uart_lpc *uart = (struct rt_uart_lpc*)dev;
	UART_CFG_Type UART_ConfigStruct;

#ifdef RT_USING_UART0
	if( uart->UART_ID == UART_0 )
    {
        /*
         * Initialize UART0 pin connect
         * P0.2: TXD
         * P0.3: RXD
         */
        PINSEL_ConfigPin(0, 2, 1);
        PINSEL_ConfigPin(0, 3, 1);

        UART_ConfigStruct.Baud_rate = 115200;
        UART_ConfigStruct.Databits = UART_DATABIT_8;
        UART_ConfigStruct.Parity = UART_PARITY_NONE;
        UART_ConfigStruct.Stopbits = UART_STOPBIT_1;

        UART_Init( uart->UART_ID, &UART_ConfigStruct);

        // Enable UART Transmit
        UART_TxCmd( uart->UART_ID, ENABLE);

        UART_IntConfig( uart->UART_ID, UART_INTCFG_RBR, ENABLE);
				UART_IntConfig( uart->UART_ID, UART_INTCFG_THRE, ENABLE);
    }
#endif

#ifdef RT_USING_UART1
    if( (uart->UART_ID) == UART_1 )
    {
        /*
         * Initialize UART1 pin connect
         * P3.16: TXD
         * P3.17: RXD
         */
        PINSEL_ConfigPin(3, 16, 3);
        PINSEL_ConfigPin(3, 17, 3);

        UART_ConfigStruct.Baud_rate = 115200;
        UART_ConfigStruct.Databits = UART_DATABIT_8;
        UART_ConfigStruct.Parity = UART_PARITY_NONE;
        UART_ConfigStruct.Stopbits = UART_STOPBIT_1;

        UART_Init( uart->UART_ID,&UART_ConfigStruct);

        // Enable UART Transmit
        UART_TxCmd( uart->UART_ID, ENABLE);

        UART_IntConfig( uart->UART_ID, UART_INTCFG_RBR, ENABLE);
			  UART_IntConfig( uart->UART_ID, UART_INTCFG_THRE, ENABLE);
    }
#endif

#ifdef RT_USING_UART2
	if( uart->UART_ID == UART_2 )
    {
    }
#endif

	return RT_EOK;
}

static rt_err_t rt_uart_open(rt_device_t dev, rt_uint16_t oflag)
{
    struct rt_uart_lpc *uart = (struct rt_uart_lpc*)dev;

	RT_ASSERT(dev != RT_NULL);
	if (dev->flag & RT_DEVICE_FLAG_INT_RX)
	{
		/* Enable the UART Interrupt */
		NVIC_EnableIRQ( uart->UART_IRQn );
	}

	return RT_EOK;
}

static rt_err_t rt_uart_close(rt_device_t dev)
{
    struct rt_uart_lpc *uart = (struct rt_uart_lpc*)dev;

	RT_ASSERT(dev != RT_NULL);
	if (dev->flag & RT_DEVICE_FLAG_INT_RX)
	{
		/* Disable the UART Interrupt */
		NVIC_DisableIRQ( uart->UART_IRQn );
	}

	return RT_EOK;
}

static rt_size_t rt_uart_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
	rt_uint8_t* ptr;
	struct rt_uart_lpc *uart = (struct rt_uart_lpc*)dev;
	RT_ASSERT(uart != RT_NULL);

	/* point to buffer */
	ptr = (rt_uint8_t*) buffer;
	if (dev->flag & RT_DEVICE_FLAG_INT_RX)
	{
		while (size)
		{
			/* interrupt receive */
			rt_base_t level;

			/* disable interrupt */
			level = rt_hw_interrupt_disable();
			if (uart->read_index != uart->save_index)
			{
				*ptr = uart->rx_buffer[uart->read_index];
				uart->read_index = (uart->read_index + 1) % RT_UART_RX_BUFFER_SIZE;
			}
			else
			{
				/* no data in rx buffer */

				/* enable interrupt */
				rt_hw_interrupt_enable(level);
				break;
			}

			/* enable interrupt */
			rt_hw_interrupt_enable(level);

			ptr ++;
			size --;
		}

		return (rt_uint32_t)ptr - (rt_uint32_t)buffer;
	}

	return 0;
}

static rt_size_t rt_uart_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
	struct rt_uart_lpc *uart = (struct rt_uart_lpc*)dev;
	char *ptr;
	rt_base_t level;
	rt_size_t num = size;
	ptr = (char*)buffer;

	if (dev->flag & RT_DEVICE_FLAG_STREAM)
	{
		/* stream mode */
		
		while (size)
		{
			level = rt_hw_interrupt_disable();
			if ((uart->write_index + 1) % RT_UART_TX_BUFFER_SIZE == uart->transmit_index)//buffer is full
				goto WAITE;
      
// 			if (*ptr == '\n')
// 			{
// 			}
			uart->tx_buffer[uart->write_index] = *ptr;
			uart->write_index = (uart->write_index + 1) % RT_UART_TX_BUFFER_SIZE;

			rt_hw_interrupt_enable(level);
			
			ptr ++;
			size --;
			continue;
WAITE:
			rt_hw_interrupt_enable(level);
			rt_thread_delay(2);//waite for transmit data
		}
		
		level = rt_hw_interrupt_disable();
		if (uart->write_index != uart->transmit_index)
		{
			//while (!(uart->UART->LSR & UART_LSR_THRE));
				uart->UART->THR = uart->tx_buffer[uart->transmit_index] & 0xFF;			
			uart->transmit_index = (uart->transmit_index + 1) % RT_UART_TX_BUFFER_SIZE;
		}
		rt_hw_interrupt_enable(level);	
	}
	else
	{
        UART_Send( uart->UART_ID, (uint8_t *)buffer, size, BLOCKING);
    }

	return num;//(rt_size_t) ptr - (rt_size_t) buffer;
}

void rt_hw_uart_init(void)
{
	struct rt_uart_lpc* uart;

#ifdef RT_USING_UART0
	/* get uart device */
	uart = &uart0_device;
	uart0_device.UART_ID = UART_0;
	uart0_device.UART = LPC_UART0;
	uart0_device.UART_IRQn = UART0_IRQn;

	/* device initialization */
	uart->parent.type = RT_Device_Class_Char;
	rt_memset(uart->rx_buffer, 0, sizeof(uart->rx_buffer));
	uart->read_index = uart->save_index = 0;
	uart->write_index = uart->transmit_index = 0;

	/* device interface */
	uart->parent.init 	    = rt_uart_init;
	uart->parent.open 	    = rt_uart_open;
	uart->parent.close      = rt_uart_close;
	uart->parent.read 	    = rt_uart_read;
	uart->parent.write      = rt_uart_write;
	uart->parent.control    = RT_NULL;
	uart->parent.user_data  = RT_NULL;

	rt_device_register(&uart->parent,
		"uart0", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STREAM | RT_DEVICE_FLAG_INT_RX);
#endif

#ifdef RT_USING_UART1
	/* get uart device */
	uart = &uart1_device;
	uart1_device.UART_ID = UART_1;
	uart0_device.UART = LPC_UART1;
	uart1_device.UART_IRQn = UART1_IRQn;

	/* device initialization */
	uart->parent.type = RT_Device_Class_Char;
	rt_memset(uart->rx_buffer, 0, sizeof(uart->rx_buffer));
	uart->read_index = uart->save_index = 0;
	uart->write_index = uart->transmit_index = 0;

	/* device interface */
	uart->parent.init 	    = rt_uart_init;
	uart->parent.open 	    = rt_uart_open;
	uart->parent.close      = rt_uart_close;
	uart->parent.read 	    = rt_uart_read;
	uart->parent.write      = rt_uart_write;
	uart->parent.control    = RT_NULL;
	uart->parent.user_data  = RT_NULL;

	rt_device_register(&uart->parent,
		"uart1", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STREAM | RT_DEVICE_FLAG_INT_RX);
#endif
}

/*@}*/
