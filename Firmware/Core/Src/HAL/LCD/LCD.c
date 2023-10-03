/*=============================================================
	 File Name: LCD.c
	 Created on: Sep 18, 2023
	 Author: HELMY-PC
===============================================================
					  _       ____  ____
					 | |     / ___||  _ \
					 | |    | |    | | | |
					 | |___ | |___ | |_| |
					 |_____| \____||____/
==============================================================*/

#include "LCD.h"
#include "stm32f1xx_hal.h"

#define timer htim1

extern TIM_HandleTypeDef timer;

/* Function to provide a delay in microseconds */
void delay(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&timer, 0);
    while (__HAL_TIM_GET_COUNTER(&timer) < us);
}

/* Function to write data to the LCD with optional RS control */
void HAL_LCD_WriteData(char data, int rs)
{
    HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, rs); /* rs = 1 for data, rs = 0 for command */

    /* Write the data to the respective pins */
    HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, ((data >> 3) & 0x01));
    HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, ((data >> 2) & 0x01));
    HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, ((data >> 1) & 0x01));
    HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, ((data >> 0) & 0x01));

    /* Toggle EN PIN to send the data
     * If the HCLK > 100 MHz, use a 20 us delay
     * If the LCD still doesn't work, increase the delay to 50, 80, or 100...
     */
    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, 1);
    delay(20);
    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, 0);
    delay(20);
}

/* Function to send a command to the LCD */
void HAL_LCD_sendCommand(char cmd)
{
    char datatosend;

    /* Send upper nibble first */
    datatosend = ((cmd >> 4) & 0x0f);
    HAL_LCD_WriteData(datatosend, 0); /* RS must be 0 while sending a command */

    /* Send lower nibble */
    datatosend = ((cmd) & 0x0f);
    HAL_LCD_WriteData(datatosend, 0);
}

/* Function to send data to the LCD */
void HAL_LCD_sendData(char data)
{
    char datatosend;

    /* Send higher nibble */
    datatosend = ((data >> 4) & 0x0f);
    HAL_LCD_WriteData(datatosend, 1); /* rs = 1 for sending data */

    /* Send lower nibble */
    datatosend = ((data) & 0x0f);
    HAL_LCD_WriteData(datatosend, 1);
}

/* Function to clear the LCD screen */
void HAL_LCD_clearScreen(void)
{
    HAL_LCD_sendCommand(0x01);
    HAL_Delay(2);
}

/* Function to move the cursor to a specific row and column */
void HAL_LCD_moveCursor(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    HAL_LCD_sendCommand(col);
}

/* Function to initialize the LCD */
void HAL_LCD_Init(void)
{
    /* 4-bit initialization */
    HAL_Delay(50); /* Wait for >40ms */
    HAL_LCD_sendCommand(0x30);
    HAL_Delay(5); /* Wait for >4.1ms */
    HAL_LCD_sendCommand(0x30);
    HAL_Delay(1); /* Wait for >100us */
    HAL_LCD_sendCommand(0x30);
    HAL_Delay(10);
    HAL_LCD_sendCommand(0x20); /* 4-bit mode */
    HAL_Delay(10);

    /* Display initialization */
    HAL_LCD_sendCommand(0x28); /* Function set --> DL=0 (4-bit mode), N=1 (2-line display), F=0 (5x8 characters) */
    HAL_Delay(1);
    HAL_LCD_sendCommand(0x08); /* Display on/off control --> D=0, C=0, B=0 (display off) */
    HAL_Delay(1);
    HAL_LCD_sendCommand(0x01); /* Clear display */
    HAL_Delay(1);
    HAL_Delay(1);
    HAL_LCD_sendCommand(0x06); /* Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift) */
    HAL_Delay(1);
    HAL_LCD_sendCommand(0x0C); /* Display on/off control --> D = 1, C and B = 0 (Cursor and blink, last two bits) */
}

/* Function to send a string to the LCD */
void HAL_LCD_sendString(char *str)
{
    while (*str) HAL_LCD_sendData(*str++);
}
