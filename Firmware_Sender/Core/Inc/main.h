/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#define APPLICATION_SIZE  6856
extern uint8_t dataToWrite[APPLICATION_SIZE];

/* Define the chunk size for data transmission */
#define CHUNK_SIZE 8
/* CAN message IDs */
#define DATA_FRAME_ID 0x123
#define ACK_FRAME_ID 0x456

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

#endif /* __MAIN_H */

