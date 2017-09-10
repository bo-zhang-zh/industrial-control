/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2010-05-02     Aozima       add led function
 */

#include <rtthread.h>

#include <board.h>

#ifdef RT_USING_DFS
/* dfs init */
#include <dfs_init.h>
/* dfs filesystem:ELM FatFs filesystem init */
#include <dfs_elm.h>
/* dfs Filesystem APIs */
#include <dfs_fs.h>

#include <dfs_posix.h>
#endif

#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
extern void lwip_system_init(void);
#endif

#ifdef RT_USING_RTGUI
#include <rtgui/driver.h>
#endif

#ifdef RT_USING_EMWIN
//#include "lcd.h"
#define LCD_THREAD_STACK_SIZE	(1024 * 6)
#define RT_LCD_THREAD_PRIORITY	28
static struct rt_thread lcd_thread;
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_lcd_thread_stack[LCD_THREAD_STACK_SIZE];
#endif

#ifdef RT_USING_USB_HOST
#include "usbhost_inc.h"
extern void rt_thread_entry_usbhost(void *parameter);
#define USB_THREAD_STACK_SIZE	(1024)
#define RT_USB_THREAD_PRIORITY	18
static struct rt_thread usb_thread;
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_usb_thread_stack[USB_THREAD_STACK_SIZE];
#endif

#ifdef RT_USING_CAN
extern void can_recv_thread(void* parameter);
extern void can_send_thread(void *parameter);
#define CANRX_THREAD_STACK_SIZE	(512)
#define CANTX_THREAD_STACK_SIZE	(512)
#define RT_CANRX_THREAD_PRIORITY	17
//这里发送的优先级需要比接收的优先级高，因为接收线程初始化会发送数据，需要发送线程完成初始化准备好
#define RT_CANTX_THREAD_PRIORITY	16

static struct rt_thread canrx_thread;
static struct rt_thread cantx_thread;
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_canrx_thread_stack[CANRX_THREAD_STACK_SIZE];
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_cantx_thread_stack[CANTX_THREAD_STACK_SIZE];
#endif


/* thread phase init */
void rt_init_thread_entry(void *parameter)
{
    /* Filesystem Initialization */
#ifdef RT_USING_DFS
		int ret;
// 		int fd;
// 	  char text[] = {"Hello everyone!!"};
    {
			extern void rt_hw_sdcard_init(void);
			  rt_hw_sdcard_init();
        /* init the device filesystem */
        dfs_init();

        /* init the elm FAT filesystam*/
        elm_init();

        /* mount sd card fat partition 1 as root directory */
        if (dfs_mount("sd0", "/", "elm", 0, 0) == 0)
            rt_kprintf("File System initialized!\n");
        else
            rt_kprintf("File System init failed!\n");
				
				ret = mkdir("/udisk", 0x777);    
				if(ret < 0)
				{
						rt_kprintf("[mkdir udisk error!]\n");
				}
				else
				{ 
						rt_kprintf("[mkdir udisk ok!]\n");
				}
				ret = mkdir("/nand", 0x777);    
				if(ret < 0)
				{
						rt_kprintf("[mkdir nand error!]\n");
				}
				else
				{ 
						rt_kprintf("[mkdir nand ok!]\n");
				}
				

// 	fd = open("/transfile.txt",O_WRONLY | O_CREAT, 0);
// 	if (fd >= 0)
// 	{
// 		ret = write(fd, text, sizeof(text));
// 		if (ret != sizeof(text))
// 		{
// 			rt_kprintf("write failed\n");
// 		}
// 		close(fd);
// 		rt_kprintf("store file successfully\n");
// 	}
// 	else
// 	{
// 		rt_kprintf("store file failed\n");
// 	}
// 				
#if defined(RT_USING_DFS_UFFS) 
//	extern void k9f1g08_mtd_init(void);
//  rt_thread_delay(50);
  k9f1g08_mtd_init();

	/* init the uffs filesystem */
	dfs_uffs_init();	/* mount flash device as flash directory */
	if (dfs_mount("nand0", "/nand", "uffs", 0, 0) == 0)
		rt_kprintf("UFFS File System initialized!\n");
	else
		rt_kprintf("UFFS File System initialzation failed!\n");
#endif
	
#ifdef RT_USING_USB_HOST				
			
			/* initialize thread */
    	rt_thread_init(&usb_thread,
			"udisk",
			rt_thread_entry_usbhost, RT_NULL,
			&rt_usb_thread_stack[0], sizeof(rt_usb_thread_stack),
			RT_USB_THREAD_PRIORITY, 10);

		  /* startup */
		  rt_thread_startup(&usb_thread);
#endif
    }
#endif

    /* LwIP Initialization */
#ifdef RT_USING_LWIP
    {
			rt_thread_t tid;
//         extern void lwip_sys_init(void);
         extern void lpc17xx_emac_hw_init(void);
         extern void telnet_srv();
			   extern void tcpserv(void *parameter);
//         eth_system_device_init();

//         /* register ethernetif device */
//         lpc17xx_emac_hw_init();
//         /* init all device */
//         rt_device_init_all();

//         /* init lwip system */
//         lwip_sys_init();
//         rt_kprintf("TCP/IP initialized!\n");
			
				/* register Ethernet interface device */
				lpc17xx_emac_hw_init();
				/* initialize lwip stack */
				/* register ethernetif device */
				eth_system_device_init();

				/* initialize lwip system */
				lwip_system_init();
				rt_kprintf("TCP/IP initialized!\n");
				
				//telnet_srv();
				//tcpserv(0); 
				tid = rt_thread_create("tcpsvr", tcpserv, RT_NULL,
                           2048, 25, 5);
				if (tid != RT_NULL)
						rt_thread_startup(tid);
    }
#endif

#ifdef RT_USING_EMWIN
    {
	    extern void MainTask(void);

	    //rt_hw_lcd_init();
			/* initialize thread */
    	rt_thread_init(&lcd_thread,
			"tlcd",
			MainTask, RT_NULL,
			&rt_lcd_thread_stack[0], sizeof(rt_lcd_thread_stack),
			RT_LCD_THREAD_PRIORITY, 20);

		/* startup */
		rt_thread_startup(&lcd_thread);
    }
#endif
		
#ifdef RT_USING_CAN
    {
			/* initialize thread */
    	rt_thread_init(&cantx_thread,
			"cantx",
			can_send_thread, RT_NULL,
			&rt_cantx_thread_stack[0], sizeof(rt_cantx_thread_stack),
			RT_CANTX_THREAD_PRIORITY, 10);

		  /* startup */
		  rt_thread_startup(&cantx_thread);		
			
			/* initialize thread */
    	rt_thread_init(&canrx_thread,
			"canrx",
			can_recv_thread, RT_NULL,
			&rt_canrx_thread_stack[0], sizeof(rt_canrx_thread_stack),
			RT_CANRX_THREAD_PRIORITY, 10);

		  /* startup */
		  rt_thread_startup(&canrx_thread);
    }
#endif
		
#ifdef RT_USING_RTGUI
    {
    	extern void rtgui_system_server_init(void);
		extern void application_init(void);
		
		rt_device_t lcd;

		/* init lcd */
		rt_hw_lcd_init();
		/* re-init device driver */
		rt_device_init_all();

		/* find lcd device */
		lcd = rt_device_find("lcd");
		if (lcd != RT_NULL)
		{
			/* set lcd device as rtgui graphic driver */
			rtgui_graphic_set_device(lcd);

			/* init rtgui system server */
			rtgui_system_server_init();

			/* startup rtgui in demo of RT-Thread/GUI examples */
			application_init();
		}
    }
#endif
}

// init led
#define rt_hw_led_init()   LPC_GPIO2->DIR |= 1<<21;
// trun on led n
#define rt_hw_led_on(n)    LPC_GPIO2->CLR |= 1<<21;
// trun off led n
#define rt_hw_led_off(n)   LPC_GPIO2->SET |= 1<<21;

//#include "canfestival.h"
ALIGN(RT_ALIGN_SIZE)
static char thread_led_stack[1024];
struct rt_thread thread_led;
static void rt_thread_entry_led(void* parameter)
{
    unsigned int count=0;
    rt_hw_led_init();

    while (1)
    {
        /* led on */
#ifndef RT_USING_FINSH
        rt_kprintf("led on,count : %d\r\n",count);
#endif
        count++;
        rt_hw_led_on(1);
        /* sleep 0.5 second and switch to other thread */
        rt_thread_delay(RT_TICK_PER_SECOND/2);

        /* led off */
#ifndef RT_USING_FINSH
        rt_kprintf("led off\r\n");
#endif
        rt_hw_led_off(1);
        rt_thread_delay(RT_TICK_PER_SECOND/2);
    }
}

int rt_application_init(void)
{
	rt_thread_t tid;

	rt_thread_init(&thread_led,
			"led",
			rt_thread_entry_led,
			RT_NULL,
			&thread_led_stack[0],
			sizeof(thread_led_stack),11,5);
	rt_thread_startup(&thread_led);

	tid = rt_thread_create("init",
			rt_init_thread_entry, RT_NULL,
			2048, RT_THREAD_PRIORITY_MAX/3, 20);
	if (tid != RT_NULL) rt_thread_startup(tid);

	return 0;
}

#if defined(RT_USING_RTGUI) && defined(RT_USING_FINSH)
#include <rtgui/rtgui_server.h>
#include <rtgui/event.h>
#include <rtgui/kbddef.h>

#include <finsh.h>

void key(rt_uint32_t key)
{
	struct rtgui_event_kbd ekbd;

	RTGUI_EVENT_KBD_INIT(&ekbd);
	ekbd.mod  = RTGUI_KMOD_NONE;
	ekbd.unicode = 0;
	ekbd.key = key;

	ekbd.type = RTGUI_KEYDOWN;
	rtgui_server_post_event((struct rtgui_event*)&ekbd, sizeof(ekbd));

	rt_thread_delay(2);

	ekbd.type = RTGUI_KEYUP;
	rtgui_server_post_event((struct rtgui_event*)&ekbd, sizeof(ekbd));
}
FINSH_FUNCTION_EXPORT(key, send a key to gui server);
#endif
