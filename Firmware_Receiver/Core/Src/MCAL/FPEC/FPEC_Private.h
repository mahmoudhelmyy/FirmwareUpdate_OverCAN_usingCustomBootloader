/*================================================================
 *	Project Name: STM32F103-Drivers
 * 	File Name: FPEC_Private.h
 * 	Created on: Sep 18, 2023
 * 	Author: HELMY-PC
 *================================================================*/

#ifndef FPEC_REGISTER_H_
#define FPEC_REGISTER_H_

/*FPEC Registers*/
typedef struct
{
	volatile uint32_t FLASH_ACR;
	volatile uint32_t FLASH_KEYR;
	volatile uint32_t FLASH_OPTKE;
	volatile uint32_t FLASH_SR;
	volatile uint32_t FLASH_CR;
	volatile uint32_t FLASH_AR;
	volatile uint32_t RESERVED;
	volatile uint32_t FLASH_OBR;
	volatile uint32_t FLASH_WRPR;
}FPEC_t;

/*FPEC Base Address*/
#define FPEC			((volatile FPEC_t*)0x40022000)


#endif
