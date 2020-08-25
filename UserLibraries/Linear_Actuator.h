/*
 * Linear_Actuator.h
 *
 *  Created on: Jul 19, 2019
 *      Author: Itachi
 */

#ifndef USERLIBRARIES_LINEAR_ACTUATOR_H_
#define USERLIBRARIES_LINEAR_ACTUATOR_H_

#define LA_PIN1         GPIO_PIN_4
#define LA_PIN2         GPIO_PIN_5
#define LA_PORT         GPIO_PORTE_BASE
void LA_GPIO_Init(void);
void LA_Up(void);
void LA_Down(void);
void LA_Pause(void);





#endif /* USERLIBRARIES_LINEAR_ACTUATOR_H_ */
