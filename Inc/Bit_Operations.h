/*
 * Bit_Operations.h
 *
 *  Created on: Nov 11, 2024
 *      Author: User
 */

#ifndef BIT_OPERATIONS_H_
#define BIT_OPERATIONS_H_
#include <stdint.h>

#define CLR_BIT(REG,BIT)       (REG &=~ (1<<BIT))
#define SET_BIT(REG,BIT)       (REG |= (1<<BIT))
#define GET_BIT(REG,BIT)       ((REG >> BIT) & 0x01)
#define TOGGLE_BIT(REG,BIT)    (REG ^= (1<<BIT))

#define CLEAR_REG(REG)         ((REG) = (0x0))
#define WRITE_REG(REG, VAL)    ((REG) = (VAL))

#define ALIAS_BASE       0x22000000U
#define BITBAND_BASE     0x20000000U


/*uint8_t GetAliasAddress(uint8_t BitPos,uint8_t BitBandAdress){
	uint8_t *AliasAddress = (uint8_t*)(ALIAS_BASE + BitPos*4 + 32*(BitBandAdress - BITBAND_BASE));
	return *AliasAddress;
}
*/

#endif /* BIT_OPERATIONS_H_ */
