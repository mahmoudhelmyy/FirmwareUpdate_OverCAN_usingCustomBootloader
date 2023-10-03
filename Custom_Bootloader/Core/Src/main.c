/**
  *================================================================================================
  * @file           : main.c
  * @brief          : Main program body
  *================================================================================================
  *
  * @details
  * This file contains the main program code for the bootloader application. It defines the
  * entry point (main function) and various other functions for system initialization, handling
  * button input, and transitioning to different applications.
  *
  * Memory Layout:
  * The bootloader occupies the first 10KB of flash memory, followed by the OTA (Over-The-Air)
  * Application, and the firmware application.
  *
  *   |============================|0x08000000
  *   |         Bootloader         |               >>> 25KB
  *   |============================|0x08006400
  *   | 	 Firmware Receiver     |			   >>> 30KB
  *   |============================|0x0800dc00
  *   |         New Firmware       |			   >>> 30KB
  *   |============================|0x08015400
  *
  *================================================================================================
  */
/*================================================================================================*/
/*									Includes													  */
/*================================================================================================*/
/* Include the main header file for the project */
#include "main.h"
/* Include the header file for the LCD module */
#include "LCD.h"
/*================================================================================================*/
/*									  	Addresses						            			  */
/*================================================================================================*/
/*Start address for the "SENDER" application after the bootloader. */
#define RECEIVER_APPLICATION_START_ADDRESS (0x8006400UL)

/*Start address for new firmware after a reserved portion for Application 2. */
#define NEW_FIRMWARE_START_ADDRESS 		   (0x800dc00UL)


TIM_HandleTypeDef htim1;
/*================================================================================================*/
/*									 Global Variables						          			  */
/*================================================================================================*/
/* Variable to indicate whether new firmware has been found (0 = not found, 1 = found) */
uint8_t firmwareFound = 0;
/* Memory address where the new firmware starts (initialized to 0) */
uint32_t firmwareStart = 0;

/*================================================================================================*/
/*								 Bootloader Main Menue						          			  */
/*================================================================================================*/
void mainMenue(){
  HAL_LCD_clearScreen();
  HAL_LCD_moveCursor(0, 0);
  HAL_LCD_sendString("1: Update");
  HAL_LCD_moveCursor(1, 0);
  HAL_LCD_sendString("2: Start");
}
/*================================================================================================*/
/*					 Function to read word from certain Address						   			  */
/*================================================================================================*/
uint32_t ReadWord(uint32_t DataAddress)
{
    uint32_t Data = 0;
    Data = *((volatile uint32_t*)(DataAddress));
    return Data;
}
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);

/* Function to perform a jump to a different address */
static void StartApplication(uint32_t *address);
void EnsurePrivilegedMode(void);
void ActivateMSP(void);
/* Assembly function for the jump to a different address */
void JumpToApplication(uint32_t newStackPointer, uint32_t newProgramCounter);

/*================================================================================================*/
/*					          Bootloader Entry Point						        			  */
/*================================================================================================*/
int main(void)
{
	  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	  HAL_Init();
	  /* Configure the system clock */
	  SystemClock_Config();
	  /* Initialize all configured peripherals */
	  MX_GPIO_Init();
	  MX_TIM1_Init();
	  HAL_TIM_Base_Start(&htim1);
	  HAL_LCD_Init();
	  HAL_LCD_clearScreen();
	  HAL_LCD_moveCursor(0, 0);
	  HAL_LCD_sendString("Bootloader Started ...");
	  //HAL_Delay(2000);

	  /* Display the main menu on the LCD */
	  mainMenue();
	  /* Read the starting address of the new firmware */
	   firmwareStart = ReadWord(NEW_FIRMWARE_START_ADDRESS);

	   /* Check if valid firmware address is found */
	   if (firmwareStart != 0xFFFFFFFF) {
	     firmwareFound = 1;
	   }

	  /* Variables to store button states */
	  GPIO_PinState ButtonOne;
	  GPIO_PinState ButtonTwo;

  while (1)
  {
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2, GPIO_PIN_SET); /* Turn on the red LED */
    ButtonOne = HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_11); /* Read the state of button 1 */
    ButtonTwo = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10); /* Read the state of button 2 */

    if (GPIO_PIN_RESET == ButtonOne)
    {
    	HAL_LCD_clearScreen();
    	HAL_LCD_moveCursor(0, 3);
    	HAL_LCD_sendString("Updating...");
      HAL_DeInit(); /* De-initialize HAL */
      /* Jump to Reciever Application */
      StartApplication((uint32_t *)RECEIVER_APPLICATION_START_ADDRESS);
    }

    else if (GPIO_PIN_RESET == ButtonTwo)
    {
      if(firmwareFound == 1){
      HAL_LCD_clearScreen();
	  HAL_LCD_moveCursor(0, 3);
	  HAL_LCD_sendString("Starting...");
      HAL_DeInit(); /* De-initialize HAL */
      /* Jump to New Firmware*/
      StartApplication((uint32_t *)NEW_FIRMWARE_START_ADDRESS);
      }
      else {
        /* Clear the LCD and display "No Updates" message */
        HAL_LCD_clearScreen();
        HAL_LCD_moveCursor(0, 3);
        HAL_LCD_sendString("No Updates");
        HAL_Delay(1500);
        HAL_LCD_clearScreen();
        /* Display the main menu again */
        mainMenue();
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
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_8|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA2 PA8 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_8|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void EnsurePrivilegedMode()
{
    /*Check if the CPU is already in privilege mode*/
    if (__get_CONTROL() & CONTROL_nPRIV_Msk)
    {
        /*If not in privilege mode, request privilege mode*/
        __asm volatile("SVC %[immediate]" : : [immediate] "I" (0): "memory");
    }
}
void ActivateMSP()
{
    /* Check if PSP (Process Stack Pointer) is active */
    if ((__get_CONTROL() & CONTROL_SPSEL_Msk) != 0)
    {
        /* Activate MSP (Main Stack Pointer) by setting the SPSEL bit to 0 */
        __set_CONTROL(__get_CONTROL() & ~CONTROL_SPSEL_Msk);

        /* Set MSP to the value of PSP */
        __set_MSP(__get_PSP());
    }
}

static void StartApplication(uint32_t *address)
{
  EnsurePrivilegedMode();

  /* Disable all NVIC interrupts */
  NVIC->ICER[0] = 0xFFFFFFFF;
  NVIC->ICER[1] = 0xFFFFFFFF;
  NVIC->ICER[2] = 0xFFFFFFFF;
  NVIC->ICER[3] = 0xFFFFFFFF;
  NVIC->ICER[4] = 0xFFFFFFFF;
  NVIC->ICER[5] = 0xFFFFFFFF;
  NVIC->ICER[6] = 0xFFFFFFFF;
  NVIC->ICER[7] = 0xFFFFFFFF;

  /* Clear all pending NVIC interrupts */
  NVIC->ICPR[0] = 0xFFFFFFFF;
  NVIC->ICPR[1] = 0xFFFFFFFF;
  NVIC->ICPR[2] = 0xFFFFFFFF;
  NVIC->ICPR[3] = 0xFFFFFFFF;
  NVIC->ICPR[4] = 0xFFFFFFFF;
  NVIC->ICPR[5] = 0xFFFFFFFF;
  NVIC->ICPR[6] = 0xFFFFFFFF;
  NVIC->ICPR[7] = 0xFFFFFFFF;

  /* Disable Systick */
  SysTick->CTRL = 0;
  SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;

  /* Disable fault handlers */
  SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);

  /* Activate MSP */
  ActivateMSP();

  /* Load vector table */
  SCB->VTOR = (uint32_t)address;
  /* Jump MSP/SP */
  JumpToApplication(address[0],address[1]);
}

/* Assembly function for the jump to a different address:

   This assembly function is a critical part of the bootloader code and is responsible for performing
   the jump to a different memory address. It is invoked when the bootloader decides to switch to
   a different application, such as App1 or App2.

   Execution Steps:
     1. MSR (Move to Special Register) instruction is used to load the Stack Pointer (SP) register
        with the value contained in r0. This step sets up the new stack pointer, ensuring that
        the stack is correctly configured for the target application.
     2. BX (Branch and Exchange) instruction is used to initiate the jump to the address stored in r1.
        When BX is executed, it transfers control to the specified address, effectively switching
        the program's execution context to the new application.
*/
void JumpToApplication(uint32_t newStackPointer, uint32_t newProgramCounter)
{
    /*Set the stack's location to a new spot in memory*/
	__set_MSP(newStackPointer);
    /*Start running code from a new memory address*/
	__asm("BX       r1");     /* Jump to the address specified in r1 (Return Address), (you can use pointer to function call) */
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
}

