/*
 * SRF05.c
 *
 *  Created on: Jul 30, 2019
 *      Author: Itachi
 */

#include "Userlibs.h"
#include "SRF05.h"
#include "Delay_Systick.h"



unsigned char Echo_wait=0;
unsigned int  Pulse_Width=0;
const double temp = 1.0/80.0;
void SRF05_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));
    //Trigger pin - output
    GPIOPinTypeGPIOOutput(Trigger_PORT, Trigger_PIN);
    GPIOPinWrite(Trigger_PORT, Trigger_PIN, 0);                                     //LOW is initial state
    //Echo pin - input interrupt both edges
    GPIOPinTypeGPIOInput(Echo_PORT, Echo_PIN);
    GPIOIntEnable(Echo_PORT, Echo_PIN);
    GPIOIntTypeSet(Echo_PORT, Echo_PIN,GPIO_BOTH_EDGES);
    GPIOIntRegister(Echo_PORT,SRF05_Handler);
    //Timer periodic up full width (32-bit)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER2));
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC_UP);
    TimerEnable(TIMER2_BASE,TIMER_A);
}

void SRF05_Handler(void)
{
    GPIOIntClear(Echo_PORT, Echo_PIN);
    if(GPIOPinRead(Echo_PORT, Echo_PIN)== Echo_PIN) //Rising edge
    {
        TIMER_VALUE=0;                              //Reset timer
        TimerEnable(TIMER2_BASE, TIMER_A);
        Echo_wait=0;
    }
    if(GPIOPinRead(Echo_PORT, Echo_PIN) == 0)       //Falling edge
    {
        Pulse_Width=TimerValueGet(TIMER2_BASE, TIMER_A);
        TimerDisable(TIMER2_BASE, TIMER_A);
        Echo_wait=1;
    }
}

uint16_t SRF05_GetDistance(void)
{
    uint16_t ui16distance=0;
    GPIOPinWrite(Trigger_PORT, Trigger_PIN, Trigger_PIN);
    delay_us(1);
    GPIOPinWrite(Trigger_PORT, Trigger_PIN, 0);
    if(Echo_wait!=0)
    {
        ui16distance =(uint32_t)(temp * Pulse_Width);
        ui16distance/=58;
    }
    return ui16distance;
}
