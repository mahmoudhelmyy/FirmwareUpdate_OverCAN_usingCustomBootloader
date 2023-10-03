/*=============================================================
	 File Name: LCD.h
	 Created on: Sep 18, 2023
	 Author: HELMY-PC
===============================================================
					  _       ____  ____
					 | |     / ___||  _ \
					 | |    | |    | | | |
					 | |___ | |___ | |_| |
					 |_____| \____||____/
==============================================================*/

#ifndef INC_LCD_H_
#define INC_LCD_H_





/* Define the GPIO pin for the RS (Register Select) pin of the LCD */
#define RS_Pin GPIO_PIN_11
#define RS_GPIO_Port GPIOA
/* Define the GPIO pin for the EN (Enable) pin of the LCD */
#define EN_Pin GPIO_PIN_8
#define EN_GPIO_Port GPIOA
/* Define the GPIO pins for the data bus (D4-D7) of the LCD */
#define D4_Pin 			GPIO_PIN_15
#define D4_GPIO_Port 	GPIOB
#define D5_Pin 			GPIO_PIN_14
#define D5_GPIO_Port 	GPIOB
#define D6_Pin 			GPIO_PIN_13
#define D6_GPIO_Port 	GPIOB
#define D7_Pin 			GPIO_PIN_12
#define D7_GPIO_Port 	GPIOB
/**
 * @brief Initialize the LCD.
 *
 * @details This function initializes the LCD display, configuring it for use.
 * It should be called once at the beginning of your program.
 *
 * @param None
 * @return None
 */
void HAL_LCD_Init(void);

/**
 * @brief Send a command to the LCD.
 *
 * @details This function sends a command to the LCD display to control its behavior.
 * Commands are used to configure the display, cursor, and other settings.
 *
 * @param cmd The command byte to send to the LCD.
 * @return None
 */
void HAL_LCD_sendCommand(char cmd);

/**
 * @brief Send data to the LCD.
 *
 * @details This function sends data (characters) to be displayed on the LCD screen.
 * It is used to display characters, numbers, and other information on the screen.
 *
 * @param data The data (character) to send to the LCD for display.
 * @return None
 */
void HAL_LCD_sendData(char data);

/**
 * @brief Send a string to the LCD.
 *
 * @details This function sends a null-terminated string to be displayed on the LCD screen.
 * It is a convenient way to display text or messages on the screen.
 *
 * @param str Pointer to the null-terminated string to be displayed on the LCD.
 * @return None
 */
void HAL_LCD_sendString(char *str);

/**
 * @brief Move the cursor to a specific position on the LCD screen.
 *
 * @details This function moves the cursor to a specified position on the LCD screen.
 * The position is defined by the row (0 or 1) and column (0-15) parameters.
 *
 * @param row The row where the cursor should be moved (0 or 1).
 * @param col The column where the cursor should be moved (0-15).
 * @return None
 */
void HAL_LCD_moveCursor(int row, int col);

/**
 * @brief Clear the LCD screen.
 *
 * @details This function clears the entire LCD screen, removing all characters and data.
 * It sets the display to a blank state.
 *
 * @param None
 * @return None
 */
void HAL_LCD_clearScreen(void);


#endif /* INC_LCD_H_ */
