/*
 * Buzzer.h
 *
 *  Created on: Aug 3, 2019
 *      Author: Itachi
 */

#ifndef USERLIBRARIES_BUZZER_H_
#define USERLIBRARIES_BUZZER_H_


#define BUZZER_PIN          GPIO_PIN_5
#define BUZZER_PORT         GPIO_PORTA_BASE
#define DATA_PA5            HWREG(0x40004080)


void Buzzer_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);
void Buzzer_Toggle(void);


#endif /* USERLIBRARIES_BUZZER_H_ */
