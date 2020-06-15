#include "flash.h"

uint32_t getSector(uint32_t adr) {
  uint32_t sector = 0;
  
  if ((adr < ADDR_FLASH_SECTOR_1) && (adr >= ADDR_FLASH_SECTOR_0)) {
          sector = FLASH_SECTOR_0;
  } else if ((adr < ADDR_FLASH_SECTOR_2)
                  && (adr >= ADDR_FLASH_SECTOR_1)) {
          sector = FLASH_SECTOR_1;
  } else if ((adr < ADDR_FLASH_SECTOR_3)
                  && (adr >= ADDR_FLASH_SECTOR_2)) {
          sector = FLASH_SECTOR_2;
  } else if ((adr < ADDR_FLASH_SECTOR_4)
                  && (adr >= ADDR_FLASH_SECTOR_3)) {
          sector = FLASH_SECTOR_3;
  } else /*if((adr < ADDR_FLASH_SECTOR_5) && (adr >= ADDR_FLASH_SECTOR_4))*/
  {
          sector = FLASH_SECTOR_4;

  }
  
  return sector;
}

void save_to_flash(uint8_t *data, uint32_t size, uint32_t flash_user_start_adr, uint32_t flash_user_end_adr ) {
  uint32_t x, y;
  
  x = size / 4;
  if (size % 4 != 0 ) y = 1; else y = 0;
  
  /* By using volatile I'm telling the compiler that 
      the value of the variable may change at any time
  */
  uint32_t data_to_FLASH = 0;
  

  printf("***** SAVE TO FLASH ****\n");
  
  uint32_t data_length = x+y;

  
  HAL_FLASH_Unlock();
  
  /* Allow Access to option bytes sector */
  HAL_FLASH_OB_Unlock();
  
  /* Fill EraseInit structure*/
  FLASH_EraseInitTypeDef EraseInitStruct;
  /* Get the 1st sector to erase */
  
  
  uint32_t FirstSector = 0;
  uint32_t NbOfSectors = 0;
  uint32_t Address = 0;
  
  FirstSector = getSector(flash_user_start_adr);
  /* Get the number of sector to erase from 1st sector*/
  NbOfSectors = getSector(flash_user_end_adr) - FirstSector + 1;
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase = TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = VOLTAGE_RANGE_1;
  EraseInitStruct.Sector = FirstSector;
  EraseInitStruct.NbSectors = NbOfSectors;
  uint32_t SectorError;

  uint32_t write_cnt = 0;
  uint32_t index;

  volatile HAL_StatusTypeDef status;
  status = HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);

  index = 0;
  
  while (index < data_length) {
    data_to_FLASH = 0;
    memcpy(&data_to_FLASH, data + write_cnt, sizeof(uint32_t));
    //printf("==> %u/%x\n", data_to_FLASH, data_to_FLASH);
    //printf("index  = %d, data_length=%d, data_to_FLASH %u\n", index, data_length, data_to_FLASH);
    if (status == HAL_OK) {
        //printf("status  = %d\n", status);
        if ((status = HAL_FLASH_Program(TYPEPROGRAM_WORD, flash_user_start_adr + write_cnt, data_to_FLASH)) == HAL_OK) {
          write_cnt += 4;
          index++;
        } else {
          printf("not ok status  = %d\n", status);
          break;
        }
        //printf("status is %d / %d\n", status, HAL_OK);
    } else {break;}
  }
  printf("finished with index == %d\n", index);
  HAL_FLASH_OB_Lock();
  HAL_FLASH_Lock();
        
  //free((uint32_t*)data_to_FLASH);
}

void read_flash(uint8_t* data, uint32_t flash_user_start_adr) {
  volatile uint32_t read_data;
  volatile uint32_t read_cnt = 0;
  do {
    read_data = *(uint32_t*) (flash_user_start_adr + read_cnt);
    if (read_data != 0xFFFFFFFF) {
      data[read_cnt] = (uint8_t) read_data;
      data[read_cnt + 1] = (uint8_t) (read_data >> 8);
      data[read_cnt + 2] = (uint8_t) (read_data >> 16);
      data[read_cnt + 3] = (uint8_t) (read_data >> 24);
      read_cnt += 4;
    }
  } while (read_data != 0xFFFFFFFF);
}
