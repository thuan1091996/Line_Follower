/*
 * Double_H_Bridges.c
 *
 *  Created on: Jul 8, 2019
 *      Author: Itachi
 */

#include "UserLibraries/Userlibs.h"
#include "UserLibraries/Double_H_Bridges.h"

void BridgeH_GPIO_Init(void)
{
    //BRIDGE 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
    GPIOPinTypeGPIOOutput(BRIDGEH1_GPIO_PORT,IN1_PIN| IN2_PIN| IN3_PIN| IN4_PIN);
    GPIOPadConfigSet(BRIDGEH1_GPIO_PORT, IN1_PIN|IN2_PIN|IN3_PIN|IN4_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    //BRIDGE 2
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));
    GPIOPinTypeGPIOOutput(BRIDGEH2_GPIO_PORT,IN4_PIN| IN5_PIN| IN6_PIN| IN7_PIN);
    GPIOPadConfigSet(BRIDGEH2_GPIO_PORT, IN5_PIN|IN6_PIN|IN7_PIN|IN8_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    DATA_PE_R=0;
    DATA_PD_R=0;
}
void BridgeH_PWM_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0));

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1));

    PWMClockSet(PWM0_BASE,PWM_SYSCLK_DIV_8);
    PWMClockSet(PWM1_BASE,PWM_SYSCLK_DIV_8);

    //BRIDGE 1
    GPIOPinConfigure(GPIO_PC4_M0PWM6);
    GPIOPinConfigure(GPIO_PC5_M0PWM7);

    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);

    GPIOPinTypePWM(BRIDGEH1_PWM_PORT, (ENA_PIN|ENB_PIN));
    GPIOPinTypePWM(BRIDGEH2_PWM_PORT, (ENC_PIN|END_PIN));

    PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN|PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN|PWM_GEN_MODE_NO_SYNC);

    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, PWM_PERIOD);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, PWM_PERIOD);

    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);
    //BRIDGE 2

}
void Motor1_Forward(void)
{
    uint8_t     temp=0;
    temp=DATA_PE_R;
    temp&= MOTOR1;
    if(temp!=MOTOR1_FORWARD)
    {
        DATA_PE_R&=~MOTOR1;
//        SysCtlDelay(SysCtlClockGet()/6);
        DATA_PE_R|=MOTOR1_FORWARD;

    }
}
void Motor1_Backward(void)
{
    uint8_t     temp=0;
    temp=DATA_PE_R;
    temp&= MOTOR1;
    if(temp!=MOTOR1_BACKWARD)
    {
        DATA_PE_R&=~MOTOR1;
//        SysCtlDelay(SysCtlClockGet()/6);
        DATA_PE_R|=MOTOR1_BACKWARD;

    }
}
void Motor1_Stop(void)
{
    DATA_PE_R&=~MOTOR1;
}
void Motor2_Forward(void)
{
    uint8_t     temp=0;
    temp=DATA_PE_R;
    temp&= MOTOR2;
    if(temp!=MOTOR2_FORWARD)
    {
        DATA_PE_R&=~MOTOR2;
//        SysCtlDelay(SysCtlClockGet()/6);
        DATA_PE_R|=MOTOR2_FORWARD;
    }
}
void Motor2_Backward(void)
{
    uint8_t     temp=0;
    temp=DATA_PE_R;
    temp&= MOTOR2;
    if(temp!=MOTOR2_BACKWARD)
    {
        DATA_PE_R&=~MOTOR2;
//        SysCtlDelay(SysCtlClockGet()/6);
        DATA_PE_R|=MOTOR2_BACKWARD;
    }
}
void Motor2_Stop(void)
{
    DATA_PE_R &= ~MOTOR2;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void Motor3_Forward(void)
{
    uint8_t     temp=0;
    temp=DATA_PD_R;
    temp&= MOTOR3;
    if(temp!=MOTOR3_FORWARD)
    {
        DATA_PD_R&=~MOTOR3;
//        SysCtlDelay(SysCtlClockGet()/6);
        DATA_PD_R|=MOTOR3_FORWARD;

    }
}
void Motor3_Backward(void)
{
    uint8_t     temp=0;
    temp=DATA_PD_R;
    temp&= MOTOR3;
    if(temp!=MOTOR3_BACKWARD)
    {
        DATA_PD_R&=~MOTOR3;
//        SysCtlDelay(SysCtlClockGet()/6);
        DATA_PD_R|=MOTOR3_BACKWARD;

    }
}
void Motor3_Stop(void)
{
    DATA_PD_R&=~MOTOR3;
}
void Motor4_Forward(void)
{
    uint8_t     temp=0;
    temp=DATA_PD_R;
    temp&= MOTOR4;
    if(temp!=MOTOR4_FORWARD)
    {
        DATA_PD_R&=~MOTOR4;
//        SysCtlDelay(SysCtlClockGet()/6);
        DATA_PD_R|=MOTOR4_FORWARD;
    }
}
void Motor4_Backward(void)
{
    uint8_t     temp=0;
    temp=DATA_PD_R;
    temp&= MOTOR4;
    if(temp!=MOTOR4_BACKWARD)
    {
        DATA_PD_R&=~MOTOR4;
//        SysCtlDelay(SysCtlClockGet()/6);
        DATA_PD_R|=MOTOR4_BACKWARD;
    }
}
void Motor4_Stop(void)
{
    DATA_PD_R &= ~MOTOR4;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void Update_Speed(uint8_t ui8_M1Duty, uint8_t ui8_M2Duty,uint8_t ui8_M3Duty,uint8_t ui8_M4Duty)
{
    static uint16_t temp1_prev=0;
    static uint16_t temp2_prev=0;
    static uint16_t temp3_prev=0;
    static uint16_t temp4_prev=0;
    uint16_t temp_load1=PWM_PERIOD/100*ui8_M1Duty;
    uint16_t temp_load2=PWM_PERIOD/100*ui8_M2Duty;
    uint16_t temp_load3=PWM_PERIOD/100*ui8_M3Duty;
    uint16_t temp_load4=PWM_PERIOD/100*ui8_M4Duty;
    if ( (temp_load1!=temp1_prev) || (temp_load2!=temp2_prev) || (temp_load3!=temp3_prev)|| (temp_load4!=temp4_prev) )
    {
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6,temp_load1 + M1_OFFSET);
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7,temp_load2 + M2_OFFSET);

        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,temp_load3 + M3_OFFSET);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,temp_load4 + M4_OFFSET);

        if(ui8_M1Duty!=0)
        PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
        else
        PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, false);

        if(ui8_M2Duty!=0)
        PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
        else
        PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, false);

        if(ui8_M3Duty!=0)
        PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
        else
        PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);

        if(ui8_M4Duty!=0)
        PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
        else
        PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, false);

        temp1_prev=temp_load1;
        temp2_prev=temp_load2;
        temp3_prev=temp_load3;
        temp4_prev=temp_load4;
    }
}
