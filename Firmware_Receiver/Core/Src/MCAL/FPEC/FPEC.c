/*================================================================
 * 	File Name: FPEC.c
 * 	Created on: Sep 18, 2023
 * 	Author: HELMY-PC
 *================================================================*/
#include "FPEC.h"
#include "FPEC_Cfg.h"
#include "FPEC_private.h"

/**
 * @brief Initialize the Flash Program and Erase Controller (FPEC).
 *
 * @details This function sets the flash latency based on the configuration in FPEC_Cfg.h,
 * unlocks the flash and the FPEC_CR register for write access.
 *
 * @param None
 * @return None
 */
void MCAL_FPEC_Init(void)
{
	/* Set the flash latency to the specified value */
	FPEC->FLASH_ACR = FPEC_LATENCY;

	/* Unlock the flash and FPEC_CR for write access */
	if (GET_BIT(FPEC->FLASH_CR, LOCK) == SET)
	{	/*Perform Unlock Sequence To Unlock FPEC Peripheral.*/
		FPEC->FLASH_KEYR = FPEC_KEY_1;
		FPEC->FLASH_KEYR = FPEC_KEY_2;
	}
}

/**
 * @brief Erase a specified range of flash memory.
 *
 * @details This function erases a range of flash memory specified by the start and end page addresses.
 * It waits for the flash to be ready before starting the erase operation.
 *
 * @param StartPageAddress Start address of the flash memory range to be erased.
 * @param EndPageAdress End address of the flash memory range to be erased.
 * @return uint8_t ErrorState (OK if successful, NOK if an error occurred)
 */
uint8_t MCAL_FPEC_EraseFlashArea(uint32_t StartPageAddress, uint32_t EndPageAddress)
{
    uint8_t ErrorState = E_OK;

    if ((StartPageAddress >= FLASH_START_ADDRESS) && (EndPageAddress <= FLASH_END_ADDRESS))
    {
        /* Waiting until the E operation is end */
        while (GET_BIT(FPEC->FLASH_SR, BSY) == SET);

        for (uint32_t PageAddress = StartPageAddress; PageAddress <= EndPageAddress; PageAddress += FLASH_PAGE_SIZE)
        {
            /* Start the erase operation */
            SET_BIT(FPEC->FLASH_CR, PER);

            FPEC->FLASH_AR = PageAddress;
            /* Start operation erase */
            SET_BIT(FPEC->FLASH_CR, STRT);
            /* Waiting until the operation is finished */
            while (GET_BIT(FPEC->FLASH_SR, BSY) == SET);
            /* END OPERATION */
            SET_BIT(FPEC->FLASH_SR, EOP);
            CLEAR_BIT(FPEC->FLASH_CR, PER);
        }
    }
    else
    {
        ErrorState = NOT_OK;
    }

    return ErrorState;
}


/**
 * @brief Erase the entire flash memory.
 *
 * @details This function erases the entire flash memory by calling MCAL_FPEC_FlashPageErase
 * for each page in the flash memory range.
 *
 * @param None
 * @return None
 */
void MCAL_FPEC_EraseFlash(void)
{
	uint8_t Counter = 0;

	for (Counter = FPEC_START_NUM_PAGE_FLASH; Counter <= FPEC_END_NUM_PAGE_FLASH; Counter++){
		MCAL_FPEC_FlashPageErase(Counter);
	}
}

/**
 * @brief Erase a specific flash page.
 *
 * @details This function erases a specific flash page by setting the corresponding page address
 * and starting the erase operation. It waits for the flash to be ready before and after the erase operation.
 *
 * @param PageNumber The page number to be erased.
 * @return uint8_t ErrorState (OK if successful, NOK if an error occurred)
 */
uint8_t MCAL_FPEC_FlashPageErase(uint8_t PageNumber)
{
	uint8_t ErrorState = E_OK;

	if (PageNumber <= FPEC_MAX_NUM_PAGE)
	{	/*Check that no main Flash memory operation is ongoing*/
		while (GET_BIT(FPEC->FLASH_SR, BSY) == SET);
		/*Set the PER bit in the FLASH_CR register to erase page.*/
		SET_BIT(FPEC->FLASH_CR, PER);
		/*Program the FLASH_AR register to select a page to erase.*/
		FPEC->FLASH_AR = (uint32_t)(PageNumber * FLASH_PAGE_SIZE) + FLASH_START_ADDRESS;
		/* Set the STRT bit in the FLASH_CR register to start erase operation. */
		SET_BIT(FPEC->FLASH_CR, STRT);
		/* Waiting until the write operation is end */
		while (GET_BIT(FPEC->FLASH_SR, BSY) == SET);

		/* END OPERATION */
		SET_BIT(FPEC->FLASH_SR, EOP);
		CLEAR_BIT(FPEC->FLASH_CR, PER);
	}
	else
		ErrorState = NOT_OK;

	return ErrorState;
}

/**
 * @brief Write data to the flash memory.
 *
 * @details This function writes data to the flash memory at the specified address.
 * It waits for the write operation to finish before returning.
 *
 * @param Address The address in flash memory where data will be written.
 * @param Data Pointer to an array of data to be written.
 * @param Length Number of data elements to write.
 * @return None
 */
void MCAL_FPEC_FlashWrite(uint32_t Address, uint16_t *Data, uint32_t Length)
{
	for (uint32_t Counter = 0; Counter < Length; Counter++)
	{	/*Set the PG bit in the FLASH_CR register to write on the flash.*/
		SET_BIT(FPEC->FLASH_CR, PG);
		/*Half word write operation.*/
		*((volatile uint16_t *)(Address)) = Data[Counter];
		/* Waiting until the write operation is end */
		while (GET_BIT(FPEC->FLASH_SR, BSY) == SET);

		SET_BIT(FPEC->FLASH_SR, EOP);
		CLEAR_BIT(FPEC->FLASH_CR, PG);
		Address = Address + 2;
	}
}

/**
 * @brief Read a 32-bit word from a specified data address in flash memory.
 *
 * This function reads a 32-bit word from the specified data address in the flash memory.
 * It returns the data read from the given address.
 *
 * @param DataAddress The address in flash memory to read the 32-bit word from.
 * @return uint32_t The 32-bit word read from the specified address.
 *
 * @example
 * uint32_t dataValue;
 * uint32_t addressToRead = 0x08001000; => Example data address in flash memory
 * dataValue = MCAL_FPEC_ReadWord(addressToRead);
 * // Now 'dataValue' contains the 32-bit word read from 'addressToRead' in flash memory.
 */
uint32_t MCAL_FPEC_ReadWord(uint32_t DataAddress)
{
    uint32_t Data = 0;
    Data = *((volatile uint32_t*)(DataAddress));
    return Data;
}


