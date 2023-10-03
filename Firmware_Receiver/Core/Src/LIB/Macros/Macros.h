/*================================================================
 * 	File Name: Macros.h
 * 	Created on: Sep 10, 2023
 * 	Author: HELMY-PC
 */
#ifndef BIT_MATH_H
#define BIT_MATH_H

#define SET_BIT(VAR, BIT)	VAR |=   1 << (BIT)
#define CLEAR_BIT(VAR, BIT)	VAR &= ~(1 << (BIT))
#define GET_BIT(VAR, BIT)	((VAR >> BIT) & 1)
#define TOGGLE_BIT(VAR, BIT)	VAR ^=   1 << (BIT)

#endif
