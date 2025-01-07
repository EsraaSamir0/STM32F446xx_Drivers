/*
 * SCB_interface.h
 *
 *  Created on: Dec 1, 2024
 *      Author: User
 */

#ifndef SCB_SCB_INTERFACE_H_
#define SCB_SCB_INTERFACE_H_
#include <stdint.h>
#include <stdio.h>

typedef enum {
	GROUP4_SUB0 = 3,
	GROUP3_SUB1,
	GROUP2_SUB2,
	GROUP1_SUB3,
	GROUP0_SUB4
}PriorityGroup_t;


uint8_t SCB_SetPRIGROUP(PriorityGroup_t PRIGROUP);


#endif /* SCB_SCB_INTERFACE_H_ */
