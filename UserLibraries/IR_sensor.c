/*
 * IR_sensor.c
 *
 *  Created on: Jul 14, 2019
 *      Author: Itachi
 */

#include "UserLibraries/Userlibs.h"
#include "IR_sensor.h"

void    IR_GPIO_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
    GPIOPinTypeGPIOInput(IR_GPIO_PORT, IR0_PIN|IR1_PIN|IR2_PIN|IR3_PIN|IR4_PIN);
}

unsigned char  Get_Status(void)
{
    uint8_t ui8_value;
    ui8_value= DATA_PB_R& 0x1F;
    return ui8_value;
}
