/**
=================================================================================
  * @file           : main.c
  * @brief          : Main program body
=================================================================================
  */
/*
 *   |============================|0x08000000
 *   |         Bootloader         |               >>> 25KB
 *   |============================|0x08006400
 *   | 	 Firmware Receiver        |			      >>> 30KB
 *   |============================|0x0800dc00
 *   |         New Firmware       |			      >>> 30KB
 *   |============================|0x08015400
*/

#include "main.h"
#include "HAL/LED/LED.h"
#include "MCAL/FPEC/FPEC.h"



/*====================================================================================================================*/
/*                                            GLOBAL VARIABLES                                                        */
/*====================================================================================================================*/
CAN_HandleTypeDef hcan;        /* - Configuration and status of the CAN peripheral. */
CAN_FilterTypeDef FilterConfig;/* - Configuration for CAN message filtering settings. */
CAN_RxHeaderTypeDef RxHeader;  /* - Header information of received CAN messages. */
uint8_t RxData[8];			   /* - An array to store received CAN message data, with a maximum length of 8 bytes. */
/*====================================================================================================================*/
/*Sender Configurations*/
CAN_TxHeaderTypeDef TxHeader;  /* - Header information for CAN messages to be transmitted. */
uint8_t TxData[8] = {0x1F};    /* ACK frame data*/
uint32_t TxMailbox;            /* - Stores the mailbox number where a transmitted CAN message is placed. */
/*====================================================================================================================*/

uint8_t isFlashed = FALSE;
uint8_t dataToWrite[APPLICATION_SIZE] = {0x00};
volatile uint32_t ReceivedFrameCount = 0;
volatile uint32_t dataCheck = 0;

void resetTxMailbox(CAN_HandleTypeDef* hcan, uint32_t mailbox) {

    /*Clear the transmit request bit for the specified mailbox*/
    hcan->Instance->TSR |= (1U << (CAN_TSR_ABRQ0_Pos + mailbox));
}

/* Function to perform a software reset */
static void SoftwareReset(void)
{
    /* Perform a software reset using NVIC_SystemReset() */
    NVIC_SystemReset();
}
/*====================================================================================================================*/
/*                                            Rx Handler                                                              */
/*====================================================================================================================*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	HAL_GPIO_WritePin(GPIOC, LED_BLUE, GPIO_PIN_SET);

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
    {
        Error_Handler();
        return;
    }

    if (RxHeader.StdId == 0x123 && RxHeader.DLC == 8)
        {
            /* Calculate the index to start storing data in the buffer */
            uint32_t startIndex = ReceivedFrameCount * 8;

            /* Copy the received data into the buffer starting from the calculated index */
            for (uint8_t i = 0; i < 8; i++)
            {
                dataToWrite[startIndex + i] = RxData[i];
            }

            ReceivedFrameCount++;

            /* Send an ACK frame */
            TxHeader.IDE = CAN_ID_STD;
            TxHeader.StdId = 0x456; /* ACK frame ID */
            TxHeader.RTR = CAN_RTR_DATA;
            TxHeader.DLC = 1; /* ACK frame DLC */
            if (HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK)
            {
                Error_Handler();
            }

            /* Check if the desired number of frames has been received */
            if (ReceivedFrameCount >= (APPLICATION_SIZE / 8))
            {
                dataCheck = 1;
                /* Disable CAN RX FIFO0 message pending interrupt since data reception is complete */
                if (HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
                {
                    Error_Handler();
                }
            }
        }
}
/*====================================================================================================================*/
/*                                           Private function prototypes                                              */
/*====================================================================================================================*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_NVIC_Init(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

/*====================================================================================================================*/
/*                                           System Initializations                                             	  */
/*====================================================================================================================*/
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_NVIC_Init();
  MX_CAN_Init();
  MCAL_FPEC_Init();
  SCB->VTOR = RECEIVER_APPLICATION_START_ADDRESS;
/*====================================================================================================================*/
  HAL_GPIO_WritePin(GPIOC, LED_GREEN, GPIO_PIN_SET);
/*====================================================================================================================*/
  /* Configure CAN Filter */
  FilterConfig.FilterActivation = ENABLE;
  FilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  FilterConfig.FilterBank = 0; /* Filter bank number */
  FilterConfig.FilterMode = CAN_FILTERMODE_IDMASK; /* Use mask mode for filtering */
  FilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  /* Set the filter identifier and mask for the ACK frame from the receiver */
  FilterConfig.FilterIdHigh = (DATA_FRAME_ID << 5);
  FilterConfig.FilterIdLow = 0x0000;
  FilterConfig.FilterMaskIdHigh = (0xFFFF << 5); /*Mask all bits of the ID*/
  FilterConfig.FilterMaskIdLow = 0x0000;

  if (HAL_CAN_ConfigFilter(&hcan, &FilterConfig) != HAL_OK)
  {
      Error_Handler();
  }
/*====================================================================================================================*/

  /* Enable CAN RX FIFO0 message pending interrupt */
  if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
      Error_Handler();
  }
  HAL_CAN_Start(&hcan);
  while (1)
      {
          if (dataCheck == 1)
          {
              if (!isFlashed)
              {
                  /* Erase a section of Flash memory */
                  MCAL_FPEC_EraseFlashArea(NEW_FIRMWARE_START_ADDRESS, NEW_FIRMWARE_END_ADDRESS);
                  /* Write data to Flash memory */
                  MCAL_FPEC_FlashWrite(NEW_FIRMWARE_START_ADDRESS, (uint16_t*)dataToWrite, APPLICATION_SIZE);
                  isFlashed = 1;
                  dataCheck = 0;
                  SoftwareReset();
              }
          }


      }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USB_LP_CAN1_RX0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}
/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 16;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_2TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = ENABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA2 PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

}

