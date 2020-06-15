/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H_
#define __FLASH_H_


#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>


/* USER CODE END Private defines */
#define FIRST_SECTOR 0
#define NB_SECTORS 0
#define SECTOR_ERROR 0
#define ADDRESS 0

/* STM32 NUCLEO64-F410RB */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */


//static __IO uint32_t MemoryProgramStatus = 0;

/* Functions Declaration*/

void save_to_flash(uint8_t *data, uint32_t size, uint32_t flash_start_adr, uint32_t flash_end_adr);
void  read_flash(uint8_t* data, uint32_t flash_start_adr);
uint32_t getSector(uint32_t adr);

#endif  /* __FLASH_H_ */
