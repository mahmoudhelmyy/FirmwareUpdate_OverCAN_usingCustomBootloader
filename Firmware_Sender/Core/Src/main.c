/**
=================================================================================
  * @file           : main.c
  * @brief          : Main program body
=================================================================================
  */

/*    |============================|0x08000000
  *   |         Bootloader         |               >>> 25KB
  *   |============================|0x08006400
  *   | 	 Firmware Receiver     |			   >>> 30KB
  *   |============================|0x0800dc00
  *   |         New Firmware       |			   >>> 30KB
  *   |============================|0x08015400
  */

#include "main.h"
#include "HAL/LED/LED.h"
#include "HAL/LCD/LCD.h"

CAN_FilterTypeDef FilterConfig;/* - Configuration for CAN message filtering settings. */
CAN_RxHeaderTypeDef RxHeader;  /* - Header information of received CAN messages. */
uint8_t RxData[8];			   /* - An array to store received CAN message data, with a maximum length of 8 bytes. */
/*====================================================================================================================*/
/*                                            GLOBAL VARIABLES                                                        */
/*====================================================================================================================*/
TIM_HandleTypeDef htim1;
CAN_HandleTypeDef hcan;        /* - Configuration and status of the CAN peripheral. */
CAN_TxHeaderTypeDef TxHeader;  /* - Header information for CAN messages to be transmitted. */
uint8_t TxData[8];			   /* - An array used to store data that will be transmitted via CAN, with a maximum length of 8 bytes. */
uint32_t TxMailbox;            /* - Stores the mailbox number where a transmitted CAN message is placed. */

uint8_t isFree = 0;     	   /* Represents the available space (level) in the mailbox for transmitting CAN messages. */
uint32_t FrameCount = 0;       /* Keeps track of the number of data frames transmitted so far. */
uint8_t dataCheck = 0;   	   /* Variable for checking data integrity or performing data validation (not used in the provided code). */
uint8_t ACK = 1;        	   /* Flag indicating the acknowledgment status for the last transmitted message. It is set to 1 when an acknowledgment is received. */
uint8_t txCompleted = 0;  	   /* Flag indicating whether the entire data transmission process is complete. It is set to 1 when all data frames have been transmitted successfully. */


/**
  * @brief Reset the transmit mailbox request bit for a specified mailbox.
  *
  * This function clears the transmit request bit for the specified mailbox in the CAN peripheral.
  * It is used to reset the request for transmitting data from the mailbox.
  *
  * @param[in] hcan: Pointer to the CAN_HandleTypeDef structure that contains the configuration
  *                  information for the CAN peripheral.
  * @param[in] mailbox: The mailbox number for which the transmit request bit should be reset.
  *
  * @retval None
  */
void resetTxMailbox(CAN_HandleTypeDef* hcan, uint32_t mailbox) {
    /* Clear the transmit request bit for the specified mailbox */
    hcan->Instance->TSR |= (1U << (CAN_TSR_ABRQ0_Pos + mailbox));
}


/*====================================================================================================================*/
/*                                            Rx Handler                                                              */
/*====================================================================================================================*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	HAL_GPIO_WritePin(GPIOC, LED_GREEN, GPIO_PIN_SET);
    /* Retrieve the received CAN message */
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
    {
        Error_Handler();
    }
    if(RxHeader.StdId == ACK_FRAME_ID || RxData[0] == 0x1F){
    	ACK = 1;
    	HAL_GPIO_WritePin(GPIOC, LED_GREEN, GPIO_PIN_RESET);
    }
}


/*====================================================================================================================*/
/*                                           Private function prototypes                                              */
/*====================================================================================================================*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_TIM1_Init(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_CAN_Init();
    HAL_TIM_Base_Start(&htim1);

    /* Configure CAN Filter */
    FilterConfig.FilterActivation = ENABLE;
    FilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    FilterConfig.FilterBank = 0; /* Filter bank number */
    FilterConfig.FilterMode = CAN_FILTERMODE_IDMASK; /* Use mask mode for filtering */
    FilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    /* Set the filter identifier and mask for the ACK frame from the receiver */
    FilterConfig.FilterIdHigh = (ACK_FRAME_ID << 5);
    FilterConfig.FilterIdLow = 0x0000;
    FilterConfig.FilterMaskIdHigh = (0xFFFF << 5); /*Mask all bits of the ID*/
    FilterConfig.FilterMaskIdLow = 0x0000;
    if (HAL_CAN_ConfigFilter(&hcan, &FilterConfig) != HAL_OK)
    {
  	  Error_Handler();
    }

    TxHeader.IDE = CAN_ID_STD;
    TxHeader.StdId = DATA_FRAME_ID;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = CHUNK_SIZE;
    HAL_CAN_Start(&hcan);

    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        Error_Handler();
    }
    while (!txCompleted)
    {
        HAL_GPIO_WritePin(GPIOC, LED_YELLOW, GPIO_PIN_SET);
        isFree = HAL_CAN_GetTxMailboxesFreeLevel(&hcan); /* Check the number of free transmit mailboxes. */

        if (FrameCount <= (APPLICATION_SIZE / 8))
        {
            /* Check if there are free transmit mailboxes and the transmission flag is not set. */
            if (isFree > 0 && ACK == 1)
            {
                /* Copy the data to be transmitted into TxData buffer. */
                for (uint8_t i = 0; i < 8; i++)
                {
                    TxData[i] = dataToWrite[i + FrameCount * CHUNK_SIZE];
                }
                FrameCount++;

                ACK = 0;
                while(HAL_CAN_IsTxMessagePending(&hcan, TxMailbox) == 1);
                /* Transmit the CAN message. */
                HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
                /* Update the record count. */
                resetTxMailbox(&hcan, 0);
            }
        }
        else
        {
            HAL_GPIO_WritePin(GPIOC, LED_YELLOW, GPIO_PIN_RESET);
        	HAL_GPIO_WritePin(GPIOC, LED_BLUE, GPIO_PIN_SET);
            txCompleted = 1; /*Set a flag to indicate transmission completion*/

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
  hcan.Init.AutoRetransmission = ENABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = ENABLE;

  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }

}

static void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 72-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xffff-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim1);
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);
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

