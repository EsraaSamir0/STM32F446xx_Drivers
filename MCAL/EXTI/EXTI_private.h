/*
 * EXTI_private.h
 *
 *  Created on: Dec 11, 2024
 *      Author: User
 */

#ifndef EXTI_EXTI_PRIVATE_H_
#define EXTI_EXTI_PRIVATE_H_

#define Raised     1
#define EXTI_LINES_NUM   15

#define EXTI5_FLAG     GET_BIT(EXTI->PR,5)
#define EXTI6_FLAG     GET_BIT(EXTI->PR,6)
#define EXTI7_FLAG     GET_BIT(EXTI->PR,7)
#define EXTI8_FLAG     GET_BIT(EXTI->PR,8)
#define EXTI9_FLAG     GET_BIT(EXTI->PR,9)

#define EXTI10_FLAG    GET_BIT(EXTI->PR,10)
#define EXTI11_FLAG    GET_BIT(EXTI->PR,11)
#define EXTI12_FLAG    GET_BIT(EXTI->PR,12)
#define EXTI13_FLAG    GET_BIT(EXTI->PR,13)
#define EXTI14_FLAG    GET_BIT(EXTI->PR,14)
#define EXTI15_FLAG    GET_BIT(EXTI->PR,15)



#endif /* EXTI_EXTI_PRIVATE_H_ */
