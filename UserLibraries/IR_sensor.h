/*
 * IR_sensor.h
 *
 *  Created on: Jul 14, 2019
 *      Author: Itachi
 */

#ifndef USERLIBRARIES_IR_SENSOR_H_
#define USERLIBRARIES_IR_SENSOR_H_

#define IR_GPIO_PORT    GPIO_PORTB_BASE
#define IR0_PIN         GPIO_PIN_0
#define IR1_PIN         GPIO_PIN_1
#define IR2_PIN         GPIO_PIN_2
#define IR3_PIN         GPIO_PIN_3
#define IR4_PIN         GPIO_PIN_4

#define DATA_PB_R       HWREG(0x400053FC)

#define RIGHT100       30
#define RIGHT75        28
#define RIGHT50        29
#define RIGHT25        25

#define BALANCED       27

#define LEFT25         19
#define LEFT50         23
#define LEFT75         7
#define LEFT100        15


#define OFFLINE        31

void                IR_GPIO_Init(void);
unsigned char       Get_Status(void);



#endif /* USERLIBRARIES_IR_SENSOR_H_ */
