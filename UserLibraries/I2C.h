/*
 * I2C.h
 *
 *  Created on: Jul 27, 2019
 *      Author: Itachi
 */

#ifndef USERLIBRARIES_I2C_H_
#define USERLIBRARIES_I2C_H_

#define SCL_PIN             GPIO_PIN_6
#define SDA_PIN             GPIO_PIN_7
#define I2C_PORT            I2C1_BASE
#define I2C_GPIO_PORT       GPIO_PORTA_BASE

void        I2C_Init(void);
void        I2C_Write_Byte(uint8_t ui8Device_Addr, uint8_t ui8Reg_Addr, uint8_t ui8Data);
uint8_t     I2C_Read_Byte(uint8_t ui8Device_Addr, uint8_t ui8Reg_Addr);


#endif /* USERLIBRARIES_I2C_H_ */
