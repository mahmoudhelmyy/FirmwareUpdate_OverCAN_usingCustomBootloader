/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  */
/**
  * The StartApplication function is a pivotal part of the bootloader code and is responsible
  * for orchestrating the transition to a different application. It is called when the bootloader
  * determines that a switch to a new application, such as App1 or App2, is necessary.
  *
  * The StartApplication function performs the following crucial steps:
  *
  * 1. Privilege Mode: It first ensures that the Central Processing Unit (CPU) is in privilege mode.
  *    If it's not in privilege mode, it invokes the "EnablePrivilegedMode" function to switch to
  *    privilege mode. Privilege mode provides full access to the CPU's resources and is essential
  *    for executing sensitive operations.
  *
  * 2. NVIC Interrupts: All Nested Vectored Interrupt Controller (NVIC) interrupts are disabled
  *    to prevent any interruptions during the application jump. This ensures a clean transition
  *    to the new application.
  *
  * 3. Clear Pending Interrupts: The function clears all pending NVIC interrupts. This is
  *    necessary to avoid any lingering interrupt requests that might affect the stability of
  *    the new application.
  *
  * 4. SysTick Timer: The SysTick timer is disabled to prevent any timing-related interruptions
  *    during the jump. This ensures that the timing behavior of the new application remains
  *    unaffected by previous settings.
  *
  * 5. Fault Handlers: Fault handlers, including Usage Fault, Bus Fault, and Memory Management Fault
  *    handlers, are disabled. This ensures a stable execution environment in the new application.
  *
  * 6. Main Stack Pointer (MSP): If the Main Stack Pointer (MSP) is not currently active, it is
  *    activated. The MSP is set to the value of the Process Stack Pointer (PSP), and the SPSEL
  *    bit in the CONTROL register is cleared. This step ensures that the correct stack is active
  *    for the new application.
  *
  * 7. Vector Table: The Vector Table Offset Register (VTOR) is loaded with the memory address
  *    specified by the "address" parameter. This ensures that the CPU uses the correct
  *    vector table for interrupt handling in the new application.
  *
  * 8. Jump to Application: Finally, the function calls the "JumpToApplication" function, passing
  *    the relevant stack pointer and program counter values to initiate the jump to the new
  *    application's code. This effectively transitions control to the new application, and the
  *    bootloader's role in the process is complete.
  *
  * @param address: A pointer to a memory address where the new application code starts.
  *
  * @note
  * - This function is specifically designed for use in bootloader scenarios, enabling a smooth
  *   transition between different applications while maintaining proper stack and program counter
  *   values for correct execution.
  * - After calling this function, program execution does not return to the calling function.
  *   Instead, it continues in the new application.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f1xx_hal.h"


/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);



#endif /* __MAIN_H */


