/*================================================================
 *	Project Name: helmyOS
 * 	File Name: LED.h
 * 	Created on: Sep 12, 2023
 * 	Author: HELMY-PC
 *================================================================
 *  					File Description
 *================================================================
 *
 */
#ifndef LED_H_
#define LED_H_


#include "stm32f1xx_hal.h"

/* LED colors */
#define LED_BLUE    GPIO_PIN_14
#define LED_GREEN   GPIO_PIN_15
#define LED_YELLOW  GPIO_PIN_13
#define LED_RED1    GPIO_PIN_2
#define LED_RED2    GPIO_PIN_1

/**
 * @brief  Initializes all LEDs, turning them off initially.
 * @note   This function should be called before using any other LED functions.
 * @retval None
 */
void HAL_LEDs_Init(void);

/**
 * @brief  Turns on the specified LED.
 * @param  ledColor: The color of the LED to be turned on (e.g., LED_BLUE, LED_GREEN).
 * @retval None
 */
void HAL_LED_On(uint8_t ledColor);

/**
 * @brief  Turns off the specified LED.
 * @param  ledColor: The color of the LED to be turned off (e.g., LED_BLUE, LED_GREEN).
 * @retval None
 */
void HAL_LED_Off(uint8_t ledColor);

/**
 * @brief  Toggles the state of the specified LED.
 * @param  ledColor: The color of the LED to be toggled (e.g., LED_BLUE, LED_GREEN).
 * @retval None
 */
void HAL_LED_Toggle(uint8_t ledColor);


#endif /* LED_H_ */
