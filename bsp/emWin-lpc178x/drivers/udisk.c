/*
 * RT-Thread SD Card Driver
 * 20090705 Yi.Qiu
 */
#include <rtthread.h>
#include <dfs_fs.h>
#include "usbhost_inc.h"
#include "diskio.h"
#include "rtdef.h"

struct rt_device udisk_device;
struct dfs_partition udisk_part;
static rt_uint32_t  numBlks, blkSize;

/* RT-Thread Device Driver Interface */
static rt_err_t rt_udisk_init(rt_device_t dev)
{
	return RT_EOK;
}

static rt_err_t rt_udisk_open(rt_device_t dev, rt_uint16_t oflag)
{

	return RT_EOK;
}

static rt_err_t rt_udisk_close(rt_device_t dev)
{
	return RT_EOK;
}

static rt_size_t rt_udisk_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
	rt_int32_t status;

	//status = sdcard_read(0, buffer, part.offset + pos, size);
	status = MS_BulkRecv (udisk_part.offset + pos, size, FATBuffer);
	if (status != MS_FUNC_OK)
	{
		rt_kprintf("udisk read failed\n");
		return 0;
	}
	rt_memcpy(buffer, (void *)FATBuffer, blkSize * size);

	return size;
}

static rt_size_t rt_udisk_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
	rt_int32_t status;

	rt_memcpy(FATBuffer, buffer, blkSize * size);
	//status = sdcard_write(0, buffer, part.offset + pos, size);
	status = MS_BulkSend (udisk_part.offset + pos, size, FATBuffer);
	if (status != MS_FUNC_OK)
	{
		rt_kprintf("udisk write failed\n");
		return 0;
	}

	return size;
}

static rt_err_t rt_udisk_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
	DRESULT res;
  struct rt_device_blk_geometry *geometry;

	res = RES_ERROR;

	switch (cmd) {
	case RT_DEVICE_CTRL_BLK_SYNC :		/* Make sure that no pending write process */
			res = RES_OK;
		break;

	case RT_DEVICE_CTRL_BLK_GETGEOME :	/* Get number of sectors on the disk (DWORD) */
		geometry = (struct rt_device_blk_geometry *)args;
		geometry->sector_count = numBlks;
	  geometry->bytes_per_sector = numBlks;
	  geometry->block_size = numBlks;
		res = RES_OK;
		break;

	case RT_DEVICE_CTRL_BLK_ERASE :		/* Get card type flags (1 byte) */
		/********/
		res = RES_OK;
		break;

	default:
		res = RES_PARERR;
	}

	return res;
}

void rt_udisk_ms_init(void)
{
//	rt_uint32_t  numBlks, blkSize;
  rt_uint8_t  inquiryResult[INQUIRY_LENGTH];
	if (MS_Init(&blkSize, &numBlks, inquiryResult) == MS_FUNC_OK)
	{
		rt_int32_t status;
//		rt_uint8_t *sector;
		
		//rt_kprintf("Udisk Size is %d * %d\n", blkSize, numBlks);

		/* get the first sector to read partition table */
// 		sector = (rt_uint8_t*) rt_malloc (512);
// 		if (sector == RT_NULL) 
// 		{
// 			rt_kprintf("allocate partition sector buffer failed\n");
// 			return;
// 		}
		//status = sdcard_read(0, sector, 0, 1);
		status = MS_BulkRecv (0, 1, FATBuffer);
		if (status == MS_FUNC_OK)
		{
			/* get the first partition */
			if (dfs_filesystem_get_partition(&udisk_part, FATBuffer, 0) != 0)
			{
			    /* there is no partition */
			    udisk_part.offset = 0;
			    udisk_part.size   = 0;
			}
		}
		else
		{
			/* there is no partition table */
			udisk_part.offset = 0;
			udisk_part.size   = 0;
		}
		
		/* release sector buffer */
// 		rt_free(sector);
		
		/* register sdcard device */
		udisk_device.type  = RT_Device_Class_Block;
		udisk_device.init 	= rt_udisk_init;
		udisk_device.open 	= rt_udisk_open;
		udisk_device.close = rt_udisk_close;
		udisk_device.read 	= rt_udisk_read;
		udisk_device.write = rt_udisk_write;
		udisk_device.control = rt_udisk_control;
		
		/* no private */
		udisk_device.user_data = RT_NULL;
		
		rt_device_register(&udisk_device, "ud0", 
			RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_REMOVABLE | RT_DEVICE_FLAG_STANDALONE);
		
		return;
	}
	rt_kprintf("udisk init failed\n");
}

void rt_thread_entry_usbhost(void* parameter)
{
		rt_int32_t  rc;
	  rt_int32_t bMounted = 0;
    extern volatile  int32_t gUSBConnected;
	
    Host_Init();               /* Initialize the lpc17xx host controller                                    */
//     rc = Host_EnumDev();       /* Enumerate the device connected                                            */

// 	  if ((rc == USB_HOST_FUNC_OK) && (Host_GetDeviceType() == MASS_STORAGE_DEVICE))
// 		{
// 			rt_udisk_ms_init();
// 			rt_device_init(&udisk_device);
//       //elm_init();

// 			
// 		  /* mount sd card fat partition 1 as root directory */
// 			if (dfs_mount("ud0", "/udisk", "elm", 0, 0) == 0)
// 					rt_kprintf("Mount Udisk OK!\n");
// 			else
// 					rt_kprintf("Mount Udisk failed!\n");
// 			
// 			//fd = open("/udisk/text1.txt", O_WRONLY | O_CREAT | O_TRUNC,0);
// 			//if (fd >= 0)
// 			//{
// 			//		close(fd);
// 			//}
// 			//rt_kprintf("Error number is %d\n",rt_get_errno());
// 		}
// 		else
// 		{
// 			rt_kprintf("Enumerate device failed!\n");
// 		}

    while (1)
    {
			if (!bMounted)
			{
				if(gUSBConnected)
				{
					rc = Host_EnumDev();       /* Enumerate the device connected */
					if ((rc == USB_HOST_FUNC_OK) && (Host_GetDeviceType() == MASS_STORAGE_DEVICE))
					{
						rt_udisk_ms_init();
						rt_device_init(&udisk_device);

						/* mount sd card fat partition 1 as root directory */
						if (dfs_mount("ud0", "/udisk", "elm", 0, 0) == 0)
						{
							bMounted = 1;
							rt_kprintf("Mount Udisk OK!\n");
						}
						else
							rt_kprintf("Mount Udisk failed!\n");
					}
					else
					{
						rt_kprintf("Enumerate device failed!\n");
					}
				}
			}
			else
			{
				if(!gUSBConnected)
				{
					if (dfs_unmount("/udisk") == 0)
							rt_kprintf("UnMount Udisk OK!\n");
					else
							rt_kprintf("UnMount Udisk failed!\n");
					
					rt_device_unregister(&udisk_device);
					bMounted = 0;
				}
				
			}
      rt_thread_delay(RT_TICK_PER_SECOND/100);
    }
}