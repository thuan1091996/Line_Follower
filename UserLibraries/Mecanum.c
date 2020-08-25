/*
 * Mecanum.c
 *
 *  Created on: Jul 11, 2019
 *      Author: Itachi
 */

#include "UserLibraries/Userlibs.h"
#include "Double_H_Bridges.h"
#include "UserLibraries/Mecanum.h"
#include "Delay_Systick.h"

void Mobile_Start(void)
{
    uint8_t ui8temp_count=0;
    uint8_t ui8temp_dutystart=10;
    for(ui8temp_count=0;ui8temp_count<7;ui8temp_count++)
    {
        Update_Speed(ui8temp_dutystart,ui8temp_dutystart,ui8temp_dutystart,ui8temp_dutystart);
        delay_us(5000);
        ui8temp_dutystart+=5;
    }
}

void Mobile_Stop(void)
{
    uint8_t ui8temp_count=0;
    uint8_t ui8temp_dutystart=25;
    for(ui8temp_count=0;ui8temp_count<5;ui8temp_count++)
    {
        Update_Speed(ui8temp_dutystart,ui8temp_dutystart,ui8temp_dutystart,ui8temp_dutystart);
        delay_us(5000);
        ui8temp_dutystart-=5;
    }
}
void Forward(void)
{
    Motor1_Forward();
    Motor2_Forward();
    Motor3_Forward();
    Motor4_Forward();
}
void Forward_L(void)
{
    Motor1_Forward();
    Motor2_Stop();
    Motor3_Stop();
    Motor4_Forward();

}
void Forward_R(void)
{
    Motor1_Stop();
    Motor2_Forward();
    Motor3_Forward();
    Motor4_Stop();

}
void Backward(void)
{
    Motor1_Backward();
    Motor2_Backward();
    Motor3_Backward();
    Motor4_Backward();
}
void Backward_L(void)
{
    Motor1_Stop();
    Motor2_Backward();
    Motor3_Backward();
    Motor4_Stop();

}
void Backward_R(void)
{
    Motor1_Backward();
    Motor2_Stop();
    Motor3_Stop();
    Motor4_Backward();
}
void Left(void)
{
    Motor1_Forward();
    Motor2_Backward();
    Motor3_Backward();
    Motor4_Forward();
}
void Right(void)
{
    Motor1_Backward();
    Motor2_Forward();
    Motor3_Forward();
    Motor4_Backward();
}
void Stop(void)
{
    Motor1_Stop();
    Motor2_Stop();
    Motor3_Stop();
    Motor4_Stop();
    Update_Speed(0, 0, 0, 0); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT


}
void Turn_R(void)
{
    Motor1_Forward();
    Motor2_Backward();
    Motor3_Forward();
    Motor4_Backward();
}
void Turn_L(void)
{
    Motor1_Backward();
    Motor2_Forward();
    Motor3_Backward();
    Motor4_Forward();
}
