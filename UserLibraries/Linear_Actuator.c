/*
 * Linear_Actuator.c
 *
 *  Created on: Jul 19, 2019
 *      Author: Itachi
 */

#include "Userlibs.h"
#include "Linear_Actuator.h"

void LA_GPIO_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
    GPIOPinTypeGPIOOutput(LA_PORT,LA_PIN1|LA_PIN2);
    GPIOPadConfigSet(LA_PORT, LA_PIN1|LA_PIN2 , GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    LA_Pause();
}
void LA_Up(void)
{
    GPIOPinWrite(LA_PORT, LA_PIN1, ~(LA_PIN1));
    GPIOPinWrite(LA_PORT, LA_PIN2, LA_PIN2);
}
void LA_Down(void)
{
    GPIOPinWrite(LA_PORT, LA_PIN1, LA_PIN1);
    GPIOPinWrite(LA_PORT, LA_PIN2, ~(LA_PIN2));
}

void LA_Pause(void)
{
    GPIOPinWrite(LA_PORT, LA_PIN1, ~(LA_PIN1));
    GPIOPinWrite(LA_PORT, LA_PIN2, ~(LA_PIN2));
}
