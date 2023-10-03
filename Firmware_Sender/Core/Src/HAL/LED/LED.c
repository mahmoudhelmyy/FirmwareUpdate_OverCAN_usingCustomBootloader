/*================================================================
 * 	File Name: LED.c
 * 	Created on: Sep 12, 2023
 * 	Author: HELMY-PC
 *================================================================
 *
 */
#include "LED.h"

void HAL_LEDs_Init(void) {
    /* Turn off all LEDs initially */
    HAL_LED_Off(LED_BLUE);
    HAL_LED_Off(LED_GREEN);
    HAL_LED_Off(LED_YELLOW);
    HAL_LED_Off(LED_RED1);
    HAL_LED_Off(LED_RED2);
}

void HAL_LED_On(uint8_t ledColor) {
    switch(ledColor) {
        case LED_BLUE:
        case LED_GREEN:
        case LED_YELLOW:
            HAL_GPIO_WritePin(GPIOC, ledColor, GPIO_PIN_SET);
            break;
        case LED_RED1:
        case LED_RED2:
            HAL_GPIO_WritePin(GPIOA, ledColor, GPIO_PIN_SET);
            break;
        default:
            break;
    }
}

void HAL_LED_Off(uint8_t ledColor) {
    switch(ledColor) {
        case LED_BLUE:
        case LED_GREEN:
        case LED_YELLOW:
            HAL_GPIO_WritePin(GPIOC, ledColor, GPIO_PIN_RESET);
            break;
        case LED_RED1:
        case LED_RED2:
            HAL_GPIO_WritePin(GPIOA, ledColor, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void HAL_LED_Toggle(uint8_t ledColor) {
    switch(ledColor) {
        case LED_BLUE:
        case LED_GREEN:
        case LED_YELLOW:
            if (HAL_GPIO_ReadPin(GPIOC, ledColor) == GPIO_PIN_SET) {
                HAL_GPIO_WritePin(GPIOC, ledColor, GPIO_PIN_RESET);
            } else {
                HAL_GPIO_WritePin(GPIOC, ledColor, GPIO_PIN_SET);
            }
            break;
        case LED_RED1:
        case LED_RED2:
            if (HAL_GPIO_ReadPin(GPIOA, ledColor) == GPIO_PIN_SET) {
                HAL_GPIO_WritePin(GPIOA, ledColor, GPIO_PIN_RESET);
            } else {
                HAL_GPIO_WritePin(GPIOA, ledColor, GPIO_PIN_SET);
            }
            break;
        default:
            break;
    }
}
