/*
 * Buzzer.c
 *
 *  Created on: Aug 3, 2019
 *      Author: Itachi
 */

#include "Userlibs.h"
#include "Buzzer.h"

void Buzzer_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    GPIOPinTypeGPIOOutput(BUZZER_PORT, BUZZER_PIN);
    Buzzer_OFF();
}
void Buzzer_ON(void)
{
    GPIOPinWrite(BUZZER_PORT, BUZZER_PIN, 0);
}
void Buzzer_OFF(void)
{
    GPIOPinWrite(BUZZER_PORT, BUZZER_PIN, BUZZER_PIN);
}
void Buzzer_Toggle(void)
{
    DATA_PA5^=BUZZER_PIN;   //Toggle Buzzer
}


