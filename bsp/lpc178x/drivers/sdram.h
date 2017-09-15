#ifndef SDRAM_H_INCLUDED
#define SDRAM_H_INCLUDED

#define SDRAM_CONFIG_16BIT
#define SDRAM_BASE_ADDR	    0xA0000000	   /* CS0 */
#define SDRAM_SIZE	        0x10000000     /* 256MB */

void SDRAM_Init (void);

#endif // SDRAM_H_INCLUDED
