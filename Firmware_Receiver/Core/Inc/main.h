/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#define APPLICATION_SIZE  6856UL
#define CHUNK_SIZE 8
#define DATA_FRAME_ID 0x123


/*Start address for the "SENDER" application after the bootloader. */
#define RECEIVER_APPLICATION_START_ADDRESS (0x8006400UL)

/*Start address for new firmware after a reserved portion for Application 2. */
#define NEW_FIRMWARE_START_ADDRESS (0x800dc00UL)
#define NEW_FIRMWARE_END_ADDRESS  (0x8020000UL)

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);


#endif /* __MAIN_H */

