/*================================================================
 *	Project Name: STM32F103-Drivers
 * 	File Name: FPEC_Cfg.h
 * 	Created on: Sep 18, 2023
 * 	Author: HELMY-PC
 *================================================================*/

#ifndef FPEC_CFG_H_
#define FPEC_CFG_H_

/*
 * FPEC_START_PAGE_BOOT : First Page of the bootLoader Page address
 * FPEC_END_PAGE_BOOT   : Second Page of bootLoader Page address
 */
#define FPEC_START_PAGE_BOOT    	0x08000001
#define FPEC_END_PAGE_BOOT      	0x08000000

#define FPEC_START_NUM_PAGE_FLASH    0
#define FPEC_END_NUM_PAGE_FLASH      127

/*Maximum Page*/
#define FPEC_MAX_NUM_PAGE    128

/*
 * FPEC_LATENCY:
 * 1- FPEC_ZERO_STATE
 * 2- FPEC_ONE_STATE
 * 3- FPEC_TWO_STATE
 */
#define FPEC_LATENCY    FPEC_ZERO_STATE

/*
Option of FPEC Latency:
	1- FPEC_ZERO_STATE
	2- FPEC_ONE_STATE
	3- FPEC_TWO_STATE
*/

#define FPEC_ZERO_STATE 	000
#define FPEC_ONE_STATE  	001
#define FPEC_TWO_STATE  	010


#endif
