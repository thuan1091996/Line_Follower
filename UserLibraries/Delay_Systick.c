/*
 * Delay_Systick.c
 *
 *  Created on: Jul 11, 2019
 *      Author: Itachi
 */

#include "Userlibs.h"
#include "Delay_Systick.h"

/* SYSTICK Initialization
 * Function:- Initialization for Systick timer then load 80  ->(80 * 1/(80*10^6) = 1us)
            - STAY IN LOOP until delay enough amount of time except when Systick interrupt execute
            - Use "HWREG" to change register value (more info in HW_type.h)
            - Enable SYSTICK Interrupt
 * Input: No
 * Output: No
 * Change this function:
            + Change the Load value to create different time delay interval
            + Change the interrupt handler by "SystickIntRegister" function
*/
void Systick_Init(){
    SysTickIntRegister(Systick_ISR);    //Define  Systick ISR
    HWREG(NVIC_ST_CTRL)&=~0x07;         //Disable Systick Timer during initialization
    HWREG(NVIC_ST_RELOAD)=800-1;         //(80 * 1/(80*10^6) = 1us)
    HWREG(NVIC_ST_CURRENT)=0;           //Write any value to clear the counter
    HWREG(NVIC_ST_CTRL)|=0x07;          //Enable Systick Timer and ARM for Systick interrupt
}

void Systick_ISR(){
    Tick++;                                         //Increase every 1 us corresponding to Reload value
    if(ui8Flag_StartCount==1)                       //Timing
    {
        Tick_Counting++;
    }
    else
    {
        Tick_Counting=0;
    }
    if(ui8Flag_L3G4200D_Wait==1)
    {
        Tick_L3G4200D++;
    }
    //For periodic tasks
    if(ui8Flag_ReadIRSensors_Wait==1)
    {
        Tick_ReadIRSensors++;
    }
    if(ui8Flag_DetectObstacle_Wait==1)
    {
        Tick_DetectObstacle++;
    }
    if(ui8Flag_Buzzer_Wait==1)
    {
        Tick_Buzzer++;
    }

    //Task frequency's
    if(Tick_ReadIRSensors>=2000)        //100Hz
     {
         ui8Flag_ReadIRSensors_Wait=0;
         Tick_ReadIRSensors=0;
     }
//    if(Tick_Calib_Rotation>=100000)
//    {
//        ui8Flag_Calib_Rotation_Wait=0;
//        Tick_Calib_Rotation=0;
//    }
    if(Tick_L3G4200D>=500)          //200Hz
    {
        ui8Flag_L3G4200D_Wait=0;
        Tick_L3G4200D=0;
    }
    if(Tick_DetectObstacle>=5000)   //20Hz
    {
        ui8Flag_DetectObstacle_Wait=0;
        Tick_DetectObstacle=0;
    }
    if(Tick_Buzzer>=10000)   //20Hz
    {
        ui8Flag_Buzzer_Wait=0;
        Tick_Buzzer=0;
    }

}

/* Delay us second by SYSTICK
 * Function: Delay the amount of "microseconds" micro second
 * Input: The amount of microseconds to delay
 * Output: No
 * Change: Use flag so the system is able to jump to another thread
           rather than stuck in 1 line code
-------------------------------------------------------*/
void delay_us(unsigned int microseconds) {
    Tick=0;                             //Reset count value
    while(Tick < microseconds);
}

