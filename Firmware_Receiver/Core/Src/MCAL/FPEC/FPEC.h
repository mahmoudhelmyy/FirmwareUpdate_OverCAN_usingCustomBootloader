/*================================================================
 *	Project Name: STM32F103-Drivers
 * 	File Name: FPEC.h
 * 	Created on: Sep 18, 2023
 * 	Author: HELMY-PC
 *================================================================*/

#ifndef FPEC_H_
#define FPEC_H_
#include "../../LIB/Macros/Macros.h"
#include "../../LIB/Std_Types/Std_Types.h"


/*========================= Error Status Definitions =========================*/
#define E_OK        1U   /**< Operation successful */
#define NOT_OK      0U   /**< Operation not successful */

/*========================= General Definitions =========================*/
#define SET         1U   /**< Set bit or enable feature */
#define CLEAR       0U   /**< Clear bit or disable feature */

/*========================= FPEC Constants =========================*/
#define FPEC_CATCH_LEAST_HALF_WORD 0x0000FFFF   /**< Mask for least significant half-word */
#define FPEC_CATCH_MOST_HALF_WORD  0xFFFF0000   /**< Mask for most significant half-word */
#define CLEAR_REGISTER               0x00000080   /**< Clear register value */
#define FLASH_START_ADDRESS        0x08000000   /**< Start address of flash memory */
#define FPEC_RDPRT                 0x00A5       /**< Read Protection value */
#define FPEC_KEY_1                 0x45670123   /**< First FPEC key */
#define FPEC_KEY_2                 0xCDEF89AB   /**< Second FPEC key */
#define FLASH_PAGE_SIZE                  1024         /**< Flash page size in bytes */
#define TWO_BYTE                   2            /**< Size of two bytes in bytes */
#define ONE_WORD_SIZE              4            /**< Size of one word (32 bits) in bytes */
#define TWO_BYTES_IN_BITS          16           /**< Size of two bytes in bits */


/*========================= Register Bits Definitions =========================*/

/* ACR register bits definition */
#define LATENCY     0   /**< Flash latency bit position in ACR register */
#define HLFCYA      3   /**< Half cycle access bit position in ACR register */
#define PRFTBE      4   /**< Prefetch buffer enable bit position in ACR register */
#define PRFTBS      5   /**< Prefetch buffer status bit position in ACR register */

/* SR register bits definition */
#define BSY         0   /**< Busy bit position in SR register */
#define PGERR       2   /**< Programming error bit position in SR register */
#define WRPRTERR    4   /**< Write protection error bit position in SR register */
#define EOP         5   /**< End of operation bit position in SR register */

/* CR register bits definition */
#define PG          0   /**< Programming bit position in CR register */
#define PER         1   /**< Page erase bit position in CR register */
#define MER         2   /**< Mass erase bit position in CR register */
#define OPTPG       4   /**< Option byte programming bit position in CR register */
#define OPTER       5   /**< Option byte erase bit position in CR register */
#define STRT        6   /**< Start bit position in CR register */
#define LOCK        7   /**< Lock bit position in CR register */
#define OPTWRE      9   /**< Option byte write enable bit position in CR register */
#define ERRIE       10  /**< Error interrupt enable bit position in CR register */
#define EOPIE       12  /**< End of operation interrupt enable bit position in CR register */

/**
 * @brief Initialize the Flash Program and Erase Controller (FPEC).
 *
 * @details This function sets the flash latency based on the configuration in FPEC_Cfg.h,
 * unlocks the flash and the FPEC_CR register for write access.
 *
 * @param None
 * @return None
 */
void MCAL_FPEC_Init(void);

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
uint8_t MCAL_FPEC_EraseFlashArea(uint32_t StartPageAddress, uint32_t EndPageAdress);

/**
 * @brief Erase the entire flash memory.
 *
 * @details This function erases the entire flash memory by calling MCAL_FPEC_FlashPageErase
 * for each page in the flash memory range.
 *
 * @param None
 * @return None
 */
void MCAL_FPEC_EraseFlash(void);

/**
 * @brief Erase a specific flash page.
 *
 * @details This function erases a specific flash page by setting the corresponding page address
 * and starting the erase operation. It waits for the flash to be ready before and after the erase operation.
 *
 * @param PageNumber The page number to be erased.
 * @return uint8_t ErrorState (OK if successful, NOK if an error occurred)
 */
uint8_t MCAL_FPEC_FlashPageErase(uint8_t PageNumber);

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
void MCAL_FPEC_FlashWrite(uint32_t Address, uint16_t *Data, uint32_t Length);

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
uint32_t MCAL_FPEC_ReadWord(uint32_t DataAddress);


void MCAL_FPEC_EraseAppArea(void);


/*Define page addresses from 0 to 63*/
#define FLASH_PAGE0_ADDRESS     (FLASH_START_ADDRESS + (0 * FLASH_PAGE_SIZE))
#define FLASH_PAGE1_ADDRESS     (FLASH_START_ADDRESS + (1 * FLASH_PAGE_SIZE))
#define FLASH_PAGE2_ADDRESS     (FLASH_START_ADDRESS + (2 * FLASH_PAGE_SIZE))
#define FLASH_PAGE3_ADDRESS     (FLASH_START_ADDRESS + (3 * FLASH_PAGE_SIZE))
#define FLASH_PAGE4_ADDRESS     (FLASH_START_ADDRESS + (4 * FLASH_PAGE_SIZE))
#define FLASH_PAGE5_ADDRESS     (FLASH_START_ADDRESS + (5 * FLASH_PAGE_SIZE))
#define FLASH_PAGE6_ADDRESS     (FLASH_START_ADDRESS + (6 * FLASH_PAGE_SIZE))
#define FLASH_PAGE7_ADDRESS     (FLASH_START_ADDRESS + (7 * FLASH_PAGE_SIZE))
#define FLASH_PAGE8_ADDRESS     (FLASH_START_ADDRESS + (8 * FLASH_PAGE_SIZE))
#define FLASH_PAGE9_ADDRESS     (FLASH_START_ADDRESS + (9 * FLASH_PAGE_SIZE))
#define FLASH_PAGE10_ADDRESS    (FLASH_START_ADDRESS + (10 * FLASH_PAGE_SIZE))
#define FLASH_PAGE11_ADDRESS    (FLASH_START_ADDRESS + (11 * FLASH_PAGE_SIZE))
#define FLASH_PAGE12_ADDRESS    (FLASH_START_ADDRESS + (12 * FLASH_PAGE_SIZE))
#define FLASH_PAGE13_ADDRESS    (FLASH_START_ADDRESS + (13 * FLASH_PAGE_SIZE))
#define FLASH_PAGE14_ADDRESS    (FLASH_START_ADDRESS + (14 * FLASH_PAGE_SIZE))
#define FLASH_PAGE15_ADDRESS    (FLASH_START_ADDRESS + (15 * FLASH_PAGE_SIZE))
#define FLASH_PAGE16_ADDRESS    (FLASH_START_ADDRESS + (16 * FLASH_PAGE_SIZE))
#define FLASH_PAGE17_ADDRESS    (FLASH_START_ADDRESS + (17 * FLASH_PAGE_SIZE))
#define FLASH_PAGE18_ADDRESS    (FLASH_START_ADDRESS + (18 * FLASH_PAGE_SIZE))
#define FLASH_PAGE19_ADDRESS    (FLASH_START_ADDRESS + (19 * FLASH_PAGE_SIZE))
#define FLASH_PAGE20_ADDRESS    (FLASH_START_ADDRESS + (20 * FLASH_PAGE_SIZE))
#define FLASH_PAGE21_ADDRESS    (FLASH_START_ADDRESS + (21 * FLASH_PAGE_SIZE))
#define FLASH_PAGE22_ADDRESS    (FLASH_START_ADDRESS + (22 * FLASH_PAGE_SIZE))
#define FLASH_PAGE23_ADDRESS    (FLASH_START_ADDRESS + (23 * FLASH_PAGE_SIZE))
#define FLASH_PAGE24_ADDRESS    (FLASH_START_ADDRESS + (24 * FLASH_PAGE_SIZE))
#define FLASH_PAGE25_ADDRESS    (FLASH_START_ADDRESS + (25 * FLASH_PAGE_SIZE))
#define FLASH_PAGE26_ADDRESS    (FLASH_START_ADDRESS + (26 * FLASH_PAGE_SIZE))
#define FLASH_PAGE27_ADDRESS    (FLASH_START_ADDRESS + (27 * FLASH_PAGE_SIZE))
#define FLASH_PAGE28_ADDRESS    (FLASH_START_ADDRESS + (28 * FLASH_PAGE_SIZE))
#define FLASH_PAGE29_ADDRESS    (FLASH_START_ADDRESS + (29 * FLASH_PAGE_SIZE))
#define FLASH_PAGE30_ADDRESS    (FLASH_START_ADDRESS + (30 * FLASH_PAGE_SIZE))
#define FLASH_PAGE31_ADDRESS    (FLASH_START_ADDRESS + (31 * FLASH_PAGE_SIZE))
#define FLASH_PAGE32_ADDRESS    (FLASH_START_ADDRESS + (32 * FLASH_PAGE_SIZE))
#define FLASH_PAGE33_ADDRESS    (FLASH_START_ADDRESS + (33 * FLASH_PAGE_SIZE))
#define FLASH_PAGE34_ADDRESS    (FLASH_START_ADDRESS + (34 * FLASH_PAGE_SIZE))
#define FLASH_PAGE35_ADDRESS    (FLASH_START_ADDRESS + (35 * FLASH_PAGE_SIZE))
#define FLASH_PAGE36_ADDRESS    (FLASH_START_ADDRESS + (36 * FLASH_PAGE_SIZE))
#define FLASH_PAGE37_ADDRESS    (FLASH_START_ADDRESS + (37 * FLASH_PAGE_SIZE))
#define FLASH_PAGE38_ADDRESS    (FLASH_START_ADDRESS + (38 * FLASH_PAGE_SIZE))
#define FLASH_PAGE39_ADDRESS    (FLASH_START_ADDRESS + (39 * FLASH_PAGE_SIZE))
#define FLASH_PAGE40_ADDRESS    (FLASH_START_ADDRESS + (40 * FLASH_PAGE_SIZE))
#define FLASH_PAGE41_ADDRESS    (FLASH_START_ADDRESS + (41 * FLASH_PAGE_SIZE))
#define FLASH_PAGE42_ADDRESS    (FLASH_START_ADDRESS + (42 * FLASH_PAGE_SIZE))
#define FLASH_PAGE43_ADDRESS    (FLASH_START_ADDRESS + (43 * FLASH_PAGE_SIZE))
#define FLASH_PAGE44_ADDRESS    (FLASH_START_ADDRESS + (44 * FLASH_PAGE_SIZE))
#define FLASH_PAGE45_ADDRESS    (FLASH_START_ADDRESS + (45 * FLASH_PAGE_SIZE))
#define FLASH_PAGE46_ADDRESS    (FLASH_START_ADDRESS + (46 * FLASH_PAGE_SIZE))
#define FLASH_PAGE47_ADDRESS    (FLASH_START_ADDRESS + (47 * FLASH_PAGE_SIZE))
#define FLASH_PAGE48_ADDRESS    (FLASH_START_ADDRESS + (48 * FLASH_PAGE_SIZE))
#define FLASH_PAGE49_ADDRESS    (FLASH_START_ADDRESS + (49 * FLASH_PAGE_SIZE))
#define FLASH_PAGE50_ADDRESS    (FLASH_START_ADDRESS + (50 * FLASH_PAGE_SIZE))
#define FLASH_PAGE51_ADDRESS    (FLASH_START_ADDRESS + (51 * FLASH_PAGE_SIZE))
#define FLASH_PAGE52_ADDRESS    (FLASH_START_ADDRESS + (52 * FLASH_PAGE_SIZE))
#define FLASH_PAGE53_ADDRESS    (FLASH_START_ADDRESS + (53 * FLASH_PAGE_SIZE))
#define FLASH_PAGE54_ADDRESS    (FLASH_START_ADDRESS + (54 * FLASH_PAGE_SIZE))
#define FLASH_PAGE55_ADDRESS    (FLASH_START_ADDRESS + (55 * FLASH_PAGE_SIZE))
#define FLASH_PAGE56_ADDRESS    (FLASH_START_ADDRESS + (56 * FLASH_PAGE_SIZE))
#define FLASH_PAGE57_ADDRESS    (FLASH_START_ADDRESS + (57 * FLASH_PAGE_SIZE))
#define FLASH_PAGE58_ADDRESS    (FLASH_START_ADDRESS + (58 * FLASH_PAGE_SIZE))
#define FLASH_PAGE59_ADDRESS    (FLASH_START_ADDRESS + (59 * FLASH_PAGE_SIZE))
#define FLASH_PAGE60_ADDRESS    (FLASH_START_ADDRESS + (60 * FLASH_PAGE_SIZE))
#define FLASH_PAGE61_ADDRESS    (FLASH_START_ADDRESS + (61 * FLASH_PAGE_SIZE))
#define FLASH_PAGE62_ADDRESS    (FLASH_START_ADDRESS + (62 * FLASH_PAGE_SIZE))
#define FLASH_PAGE63_ADDRESS    (FLASH_START_ADDRESS + (63 * FLASH_PAGE_SIZE))


/*End address of the flash memory*/
#define FLASH_END_ADDRESS       (FLASH_START_ADDRESS + (127 * FLASH_PAGE_SIZE))


#endif

