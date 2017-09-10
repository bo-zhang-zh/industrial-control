/*
 * File      : rtthread.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006-2012, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-10-13     prife        the first version 
 * 2012-03-11     prife        use mtd device interface
*/

#include <rtdevice.h>
#include <emc_nand.h>
//#include <rtthread.h>

/* configurations */
#define PAGE_DATA_SIZE                  2048
#define BLOCK_MARK_SPARE_OFFSET         4
//#define CONFIG_USE_HW_ECC
static struct rt_mutex nand;

#define BLOCK_MARK_OFFSET  (PAGE_DATA_SIZE + BLOCK_MARK_SPARE_OFFSET)
/*
 * In a page, data's ecc code is stored in spare area, from BYTE 0 to BYTEE 3.
 * Block's status byte which indicate a block is bad or not is BYTE4.
 */
static void nand_hw_init(void)
{
// 	/* initialize GPIO£¬ nFWE£¬ALE£¬CLE£¬nFCE£¬nFRE */
// 	GPACON |= (1<<17) | (1<<18) | (1<<19) | (1<<20) | (1<<22);

// 	/* enable PCLK for nand controller */
// 	CLKCON |= 1 << 4;

// 	NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4)|(0<<0);
// 	NFCONT = (0<<13)|(0<<12)|(0<<10)|(0<<9)|(0<<8)|(1<<6)|(1<<5)|(1<<4)|(1<<1)|(1<<0);
// 	NFSTAT = 0;

// 	/* reset nand flash */
// 	NF_CE_L();
// 	NF_CLEAR_RB();
// 	NF_CMD(CMD_RESET);
// 	NF_DETECT_RB();
// 	NF_CE_H();
	
	EMC_NAND_Init();
	EMC_NAND_Reset();
	rt_thread_delay(2);
}

/*
 *check the first byte in spare of the block's first page
 *return
 * good block,  RT_EOK
 * bad  blcok, return -RT_ERROR
 */
static rt_err_t k9f1g08_mtd_check_block(
		struct rt_mtd_nand_device* device,
		rt_uint32_t block)
{
	rt_uint8_t status;

	*(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_1;

	*(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t)BLOCK_MARK_OFFSET;//0x00; 
	*(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t)((BLOCK_MARK_OFFSET >> 8) & 0xff);//0x08;     
	*(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(block * NAND_BLOCK_SIZE);  
	*(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(block * NAND_BLOCK_SIZE);    

	*(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_TRUE;

	NandFlash_WaitForReady();
	status = *(volatile uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA);
	/* TODO: more check about status */
	return status == 0xFF ? RT_EOK : -RT_ERROR;

}

static rt_err_t k9f1g08_mtd_mark_bad_block(
		struct rt_mtd_nand_device* device,
		rt_uint32_t block)
{
	/* get address of the fisrt page in the block */
	rt_err_t result = RT_EOK;

	/* Page write Spare area command and address */
	*(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM;

	*(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t)BLOCK_MARK_OFFSET;//0x00; 
	*(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t)((BLOCK_MARK_OFFSET >> 8) & 0xff);//0x08; 
	*(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(block * NAND_BLOCK_SIZE);  
	*(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(block * NAND_BLOCK_SIZE);   

	*(volatile uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA) = 0x00;
	
	*(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM_TRUE;

	NandFlash_WaitForReady();

	/* Check status for successful operation */
	result = EMC_NAND_GetStatus();

	if(result == NAND_ERROR)
	{
			result = -RT_ERROR;
	}
  return result;
}

static rt_err_t k9f1g08_mtd_erase_block(
		struct rt_mtd_nand_device* device,
		rt_uint32_t block)
{
	/* 1 block = 64 page= 2^6*/
    rt_err_t result = RT_EOK;

	rt_mutex_take(&nand, RT_WAITING_FOREVER);

  *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_ERASE0;
  
  *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(block * NAND_BLOCK_SIZE);  
  *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(block * NAND_BLOCK_SIZE);  
		
  *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_ERASE1; 

    NandFlash_WaitForReady();
  
  result = EMC_NAND_GetStatus();
	if(result == NAND_ERROR)
	{
			result = -RT_ERROR;
	}
	rt_mutex_release(&nand);
	return result;
}

/* return 0, ecc ok, 1, can be fixed , -1 can not be fixed */
static rt_err_t k9f1g08_mtd_read(
		struct rt_mtd_nand_device * dev,
		rt_off_t page,
		rt_uint8_t * data, rt_uint32_t data_len, //may not always be 2048
		rt_uint8_t * spare, rt_uint32_t spare_len)
{
	rt_uint32_t i;
// 	rt_uint32_t mecc;
//	rt_uint32_t status;
	rt_err_t result = RT_EOK;
	
	rt_mutex_take(&nand, RT_WAITING_FOREVER);

	if (data != RT_NULL && data_len != 0)
	{
		/* Page Read command and page address */
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_1; 
   
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = 0x00; 
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = 0X00; 
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(page);  
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(page);  
    
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_TRUE; 
   
    NandFlash_WaitForReady();

    /* Get Data into Buffer */    
    for(i=0; i < data_len; i++)
    {
      data[i]= *(volatile uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA);
    }
		
#if defined(CONFIG_USE_HW_ECC)
// 		/* if read whole page data, then check ecc status */
// 		if (data_len == PAGE_DATA_SIZE)
// 		{
// 		}
#endif
	}

	if (spare != RT_NULL && spare_len != 0)
	{
    /* Page Read command and page address */     
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_1;

    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t)(PAGE_DATA_SIZE);//0x00; 
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t)((PAGE_DATA_SIZE >> 8) & 0xff);//0x08;     
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(page);  
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(page);    

    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_READ_TRUE;

    NandFlash_WaitForReady();

    /* Get Data into Buffer */
    for (i=0;i < spare_len; i++)
    {
      spare[i] = *(volatile uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA);
    }
		//result = RT_EOK;
	}
	rt_mutex_release(&nand);

	/* TODO: more check about status */
	return result;
}

static rt_err_t k9f1g08_mtd_write (
		struct rt_mtd_nand_device * dev,
		rt_off_t page,
		const rt_uint8_t * data, rt_uint32_t data_len,//will be 2048 always!
		const rt_uint8_t * spare, rt_uint32_t spare_len)
{
	rt_uint32_t i;
//	rt_uint32_t mecc0;
	rt_err_t result = RT_EOK;
#if defined(CONFIG_USE_HW_ECC)
	rt_uint8_t ecc_data[4];
#endif

	rt_mutex_take(&nand, RT_WAITING_FOREVER);

	if (data != RT_NULL && data_len != 0)
	{
		RT_ASSERT(data_len == PAGE_DATA_SIZE);

    /* Page write command and address */
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM;

    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = 0x00;  
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = 0X00;  
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(page);  
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(page);   

    /* Write data */
    for(i=0; i < data_len; i++)
    {
      *(volatile uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA) = data[i];
    }
    
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM_TRUE;

    NandFlash_WaitForReady();

    /* Check status for successful operation */
    result = EMC_NAND_GetStatus();
    
    if(result == NAND_ERROR)
    {
				result = -RT_ERROR;
				goto __ret;
    }
		
#if defined(CONFIG_USE_HW_ECC)
// 		/* produce HARDWARE ECC */
// 		mecc0=NFMECC0;
// 		ecc_data[0]=(rt_uint8_t)(mecc0 & 0xff);
// 		ecc_data[1]=(rt_uint8_t)((mecc0 >> 8) & 0xff);
// 		ecc_data[2]=(rt_uint8_t)((mecc0 >> 16) & 0xff);
// 		ecc_data[3]=(rt_uint8_t)((mecc0 >> 24) & 0xff);

// 		/* write ecc to spare[0]..[3] */
// 		for(i=0; i<4; i++)
// 			NF_WRDATA8(ecc_data[i]);
#endif
	}

	if (spare != RT_NULL && spare_len != 0)
	{
    /* Page write command and address */
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM;
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t)(PAGE_DATA_SIZE);//0x00;  
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = (uint8_t)((PAGE_DATA_SIZE >> 8) & 0xff);//0X00;  
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(page);  
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(page);   

    /* Write data */
    for(i=0; i < spare_len; i++)
    {
      *(volatile uint8_t *)(NAND_FLASH_START_ADDR | DATA_AREA) = spare[i];
    }
    
    *(volatile uint8_t *)(NAND_FLASH_START_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM_TRUE;

    NandFlash_WaitForReady();

    /* Check status for successful operation */
    result = EMC_NAND_GetStatus();
    if(result == NAND_ERROR)
    {
				result = -RT_ERROR;
				goto __ret;
    }
	}

__ret:
	rt_mutex_release(&nand);
	return result;
}

static rt_err_t k9f1g08_read_id(
		struct rt_mtd_nand_device * dev)
{
	return RT_EOK;
}

const static struct rt_mtd_nand_driver_ops k9f1g08_mtd_ops =
{
	k9f1g08_read_id,
	k9f1g08_mtd_read,
	k9f1g08_mtd_write,
	NULL,
	k9f1g08_mtd_erase_block,
	k9f1g08_mtd_check_block,
	k9f1g08_mtd_mark_bad_block,
};

/* interface of nand and rt-thread device */
static struct rt_mtd_nand_device nand_part[4];

void k9f1g08_mtd_init()
{
	/* initialize nand controller of S3C2440 */
	nand_hw_init();

    /* initialize mutex */
	if (rt_mutex_init(&nand, "nand", RT_IPC_FLAG_FIFO) != RT_EOK)
	{
		rt_kprintf("init nand lock mutex failed\n");
	}
	/* the first partition of nand */
	nand_part[0].page_size = PAGE_DATA_SIZE;
	nand_part[0].pages_per_block = 64;
	nand_part[0].block_total = 256;
	nand_part[0].block_start = 0;
	nand_part[0].block_end = 255;
	nand_part[0].oob_size = 64;
	nand_part[0].ops = &k9f1g08_mtd_ops;
	rt_mtd_nand_register_device("nand0", &nand_part[0]);

	/* the second partition of nand */
	nand_part[1].page_size = PAGE_DATA_SIZE;
	nand_part[1].pages_per_block = 64;
	nand_part[1].block_total = 256;
	nand_part[1].block_start = 256;
	nand_part[1].block_end = 512-1;
	nand_part[1].oob_size = 64;
	nand_part[1].ops = &k9f1g08_mtd_ops;
	rt_mtd_nand_register_device("nand1", &nand_part[1]);

	/* the third partition of nand */
	nand_part[2].page_size = PAGE_DATA_SIZE;
	nand_part[2].pages_per_block = 64;
	nand_part[2].block_total = 256;
	nand_part[2].block_start = 512;
	nand_part[2].block_end = 512+256-1;
	nand_part[2].oob_size = 64;
	nand_part[2].ops = &k9f1g08_mtd_ops;
	rt_mtd_nand_register_device("nand2", &nand_part[2]);

	/* the 4th partition of nand */
	nand_part[3].page_size = PAGE_DATA_SIZE;
	nand_part[3].pages_per_block = 64;
	nand_part[3].block_total = 256;
	nand_part[3].block_start = 512+256;
	nand_part[3].block_end = 1024-1;
	nand_part[3].oob_size = 64;
	nand_part[3].ops = &k9f1g08_mtd_ops;
	rt_mtd_nand_register_device("nand3", &nand_part[3]);
}

// #include "finsh.h"
// static char buf[PAGE_DATA_SIZE+64];
// static char spare[64];

// void nand_erase(int start, int end)
// {
// 	int page;
// 	for(; start <= end; start ++)
// 	{
// 		page = start * 64;
// 		rt_memset(buf, 0, PAGE_DATA_SIZE);
// 		rt_memset(spare, 0, 64);

// 		k9f1g08_mtd_erase_block(RT_NULL, start);

// 		k9f1g08_mtd_read(RT_NULL, page, buf, PAGE_DATA_SIZE, spare, 64);
// 		if (spare[0] != 0xFF)
// 		{
// 			rt_kprintf("block %d is bad, mark it bad\n", start);

// 			//rt_memset(spare, 0xFF, 64);
// 			if (spare[4] == 0xFF)
// 			{
// 				spare[4] = 0x00;
// 				k9f1g08_mtd_write(RT_NULL, page, RT_NULL, 0, spare, 64);
// 			}
// 		}
// 	}
// }

// int nand_read(int page)
// {
// 	int i;
// 	int res;
// 	rt_memset(buf, 0, sizeof(buf));
// //	rt_memset(spare, 0, 64);

// //	res = k9f1g08_mtd_read(RT_NULL, page, buf, PAGE_DATA_SIZE, spare, 64);
// 	res = k9f1g08_mtd_read(RT_NULL, page, buf, PAGE_DATA_SIZE+64, RT_NULL, 0);
// 	rt_kprintf("block=%d, page=%d\n", page/64, page%64);
// 	for(i=0; i<PAGE_DATA_SIZE; i++)
// 	{
// 		rt_kprintf("%02x ", buf[i]);
// 		if((i+1)%16 == 0)
// 			rt_kprintf("\n");
// 	}

// 	rt_kprintf("spare:\n");
// 	for(i=0; i<64; i++)
// 	{
// //		rt_kprintf("%02x ", spare[i]);
// 		rt_kprintf("%02x ", buf[2048+i]);
// 		if((i+1)%8 == 0)
// 			rt_kprintf("\n");
// 	}
// 	return res;
// }
// int nand_write(int page)
// {
// 	int i;
// 	rt_memset(buf, 0, PAGE_DATA_SIZE);
// 	for(i=0; i<PAGE_DATA_SIZE; i++)
// 		buf[i] = (i % 2) + i / 2;
// 	return k9f1g08_mtd_write(RT_NULL, page, buf, PAGE_DATA_SIZE, RT_NULL, 0);
// }

// int nand_read2(int page)
// {
// 	int i;
// 	int res;
// 	rt_memset(buf, 0, sizeof(buf));

// 	res = k9f1g08_mtd_read(RT_NULL, page, buf, PAGE_DATA_SIZE, RT_NULL, 0);
// 	rt_kprintf("block=%d, page=%d\n", page/64, page%64);
// 	for(i=0; i<PAGE_DATA_SIZE; i++)
// 	{
// 		rt_kprintf("%02x ", buf[i]);
// 		if((i+1)%16 == 0)
// 			rt_kprintf("\n");
// 	}

// 	rt_memset(spare, 0, 64);
// 	res = k9f1g08_mtd_read(RT_NULL, page, RT_NULL, 0, spare, 64);
// 	rt_kprintf("spare:\n");
// 	for(i=0; i<64; i++)
// 	{
// 		rt_kprintf("%02x ", spare[i]);
// 		if((i+1)%8 == 0)
// 			rt_kprintf("\n");
// 	}
// 	return res;
// }
// int nand_read3(int page)
// {
// 	int i;
// 	int res;
// 	rt_memset(buf, 0, sizeof(buf));
// 	rt_memset(spare, 0, 64);

// 	res = k9f1g08_mtd_read(RT_NULL, page, buf, PAGE_DATA_SIZE, spare, 64);
// 	rt_kprintf("block=%d, page=%d\n", page/64, page%64);
// 	for(i=0; i<PAGE_DATA_SIZE; i++)
// 	{
// 		rt_kprintf("%02x ", buf[i]);
// 		if((i+1)%16 == 0)
// 			rt_kprintf("\n");
// 	}

// 	rt_kprintf("spare:\n");
// 	for(i=0; i<64; i++)
// 	{
// 		rt_kprintf("%02x ", spare[i]);
// 		if((i+1)%8 == 0)
// 			rt_kprintf("\n");
// 	}
// 	return res;
// }

// int nand_check(int block)
// {
// 	if ( k9f1g08_mtd_check_block(RT_NULL, block) != RT_EOK)
// 		rt_kprintf("block %d is bad\n", block);
// 	else
// 		rt_kprintf("block %d is good\n", block);
// 	
// 	return 0;
// }

// int nand_mark(int block)
// {
// 	return k9f1g08_mtd_mark_bad_block(RT_NULL, block);
// }
// FINSH_FUNCTION_EXPORT(nand_read, nand_read(1).);
// FINSH_FUNCTION_EXPORT(nand_read2, nand_read(1).);
// FINSH_FUNCTION_EXPORT(nand_read3, nand_read(1).);
// FINSH_FUNCTION_EXPORT(nand_write, nand_write(1).);
// FINSH_FUNCTION_EXPORT(nand_check, nand_check(1).);
// FINSH_FUNCTION_EXPORT(nand_mark, nand_mark(1).);
// FINSH_FUNCTION_EXPORT(nand_erase, nand_erase(100, 200). erase block in nand);
