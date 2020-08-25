/* --------0.Project information--------------------
 * Line follower
 * Author : TRAN MINH THUAN
 * Supported by: TRAN ANH TAI, NGUYEN TRAN MINH NGHIA
 * Date: July 7th 2019
 * Project associate with TM4C123, CCS version 9.
---------------------------------------------------*/


/* --------1.System requirement---------------------
    1.Setup driver for 2 H-bridges to control 4 motors (UserLibraries/Double_H_Bridges)
        a. GPIO Initialization for IN pins H-bridge 1  & IN pins H-bridge 2 to control motors direction
                                    (PE0->PE3)            (PD0 -> PD3)
        b. PWM Initialization for ENA, ENB H-bridge 1 & ENC, END H-bridge 2 to control motors speed
                                    (PC4,PC5)             ()
        c. Update_Speed function for changing the PWM duty cycles to vary speed of motors
        d. Motors 1->4 direction functions
    2. MECANUM wheels direction control
    3. SYSTICK timer for timing purpose
    4. Reading value of 5 IR Sensors            (PB0 - PB4)
    5. Algorithm for line-follower as smooth as possible (use PID if needed)
    6. SRF05 sensor to detect object
    7. Control cylinder to lift gripper (PE4-PE5) //CHANGE IMMEDIATELY
    8. L3G4200D gyro sensor (PA6,PA7)
    8. Running all together (with or without operating system, RTOS maybe)
---------------------------------------------------*/


////// ----------------2. Pre-processor Directives Section--------------------///////
#include <UserLibraries/Userlibs.h>
#include "UserLibraries/Double_H_Bridges.h"
#include "UserLibraries/Mecanum.h"
#include "UserLibraries/Delay_Systick.h"
#include "UserLibraries/IR_sensor.h"
#include "UserLibraries/Linear_Actuator.h"
#include "UserLibraries/I2C.h"
#include "UserLibraries/L3G4200D.h"
#include "UserLibraries/SRF05.h"
#include "UserLibraries/Buzzer.h"

#define    MAX_ANGLE                    4
#define    MIN_ANGLE                    -4
#define    DEFLECT_CW                   1           //RIGHT
#define    DEFLECT_CCW                  2           //LEFT
#define    BELOW_STOPA                  10
#define    AT_STOPA                     11
#define    BELOW_CONTAINER              12
#define    BELOW_STOPB                  13
#define    AT_STOPB                     14
#define    RETURN                       15
#define    RETURN_AFTER_B               16
#define    RETURN_AFTER_A               17
#define    DISTANCE_2_OBSTACLE          45
//////------------------------------------------------------------------------///////

//testing variables
uint8_t    ui8_test=4;
uint8_t    ui8_LineStatus=0;
uint32_t   prev_status=0;
uint32_t   i=0;
uint16_t   ui16_testmotor=0;
uint8_t    ui8_M1Duty=25;
uint8_t    ui8_M2Duty=32;
uint8_t    ui8_M3Duty=25;
uint8_t    ui8_M4Duty=32;
uint8_t    Record[100]={0};


//system variables
uint8_t    ui8Flag_L3G4200D_Wait=0;
uint8_t    ui8Flag_ReadIRSensors_Wait=0;
uint8_t    ui8Flag_DetectObstacle_Wait=0;
uint8_t    ui8Flag_Buzzer_Wait=0;
uint8_t    ui8Flag_StartCount=0;

uint8_t    ui8_Position=BELOW_STOPA;
uint8_t    ui8_Angle_Balanced=0;
uint8_t    ui8_Obstacle=0;
uint8_t    ui8_BuzzerOn=0;
uint8_t    ui8_LineFollow=0;
uint16_t   ui16_Obstacle_Distance=0;

unsigned long Tick=0;
unsigned long Tick_L3G4200D=0;
//unsigned long Tick_Calib_Rotation=0;
unsigned long Tick_ReadIRSensors=0;
unsigned long Tick_DetectObstacle=0;
unsigned long Tick_Buzzer=0;
unsigned long Tick_Counting=0;
float fl_gyrorate=0;
float fl_cur_angle=0;
float fl_offsetz=0;

////// ----------------3.Global Declarations Section--------------------------///////
//////------------------------------------------------------------------------///////

////// ----------------4. Subroutines Section---------------------------------///////
void main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5| SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); //80MHz
    Systick_Init();
    LA_GPIO_Init();
    LA_Down();
    BridgeH_GPIO_Init();        //Bridge H 1,2 GPIO(control reversing) Initialization
    BridgeH_PWM_Init();         //Bridge H 1,2 PWM (control speed) Initialization
    IR_GPIO_Init();             //IR sensors Initialization
    Buzzer_Init();
    SRF05_Init();
    I2C_Init();
    L3G4200D_Init();
//    fl_offsetz=L3G4200D_Cal_Offset(500);
    if (ui8_test==0) LA_Down();
    if (ui8_test==1) LA_Up();
    if (ui8_test==2) LA_Pause();
    while(1)
    {
//----------------------------------------------Task Scheduler---------------------------------------------------//

/////-----------------------------------Main tasks-------------------------------/////
//
////---------------------------Determine angle flag-------------------------------//
        if( (fl_cur_angle >=MAX_ANGLE) || (fl_cur_angle<= MIN_ANGLE))
        {
            if(fl_cur_angle >=MAX_ANGLE) ui8_Angle_Balanced=DEFLECT_CW;
            if(fl_cur_angle <=MIN_ANGLE) ui8_Angle_Balanced=DEFLECT_CCW;
        }
//---------------------------Rotation to "0" angle-------------------------------//
        if (ui8_Angle_Balanced==DEFLECT_CW)
        {
            Update_Speed(25, 29, 25, 29);
            Turn_L();
            if (fl_cur_angle<=0) ui8_Angle_Balanced=0;
        }
        if (ui8_Angle_Balanced==DEFLECT_CCW)
        {
            Update_Speed(25, 29, 25, 29);
            Turn_R();
            if (fl_cur_angle>=0) ui8_Angle_Balanced=0;
        }
////---------------------------Detect obstacle-------------------------------//
        if( (ui16_Obstacle_Distance>0) && (ui16_Obstacle_Distance<=DISTANCE_2_OBSTACLE ))
        {
            ui8_Obstacle=1;
        }
        else
        {
            ui8_Obstacle=0;
        }
//---------------------------Handler obstacle-------------------------------//
        if( (ui8_Obstacle==1) && (ui8_Position==BELOW_STOPA) )
        {
            Stop();
            ui8_BuzzerOn=1;
        }
        else
        {
            ui8_BuzzerOn=0;
        }
//--------------------------Line follow permission------------------------------//
        if(ui8_Angle_Balanced==0)
        {
            if( (ui8_Obstacle==0) || (ui8_Position!=BELOW_STOPA) )
            {
                ui8_LineFollow=1;
            }
            else
            {
                ui8_LineFollow=0;
            }
        }
        else //angle !=0
        {
            ui8_LineFollow=0;
        }

//--------------------------Reach Stop A-------------------------------//
        if(ui8_Position==AT_STOPA)
        {
            if(ui8_Obstacle==1)
            {
                ui8_BuzzerOn=0;
                ui8_Position=BELOW_CONTAINER;
            }
            else
            {
                ui8Flag_StartCount=1;
                if(Tick_Counting>=500000)
                {
                    ui8_BuzzerOn=1;
                }
            }
        }
//--------------------------Below Container-------------------------------//
        if(ui8_Position==BELOW_CONTAINER)
        {
            if(ui16_Obstacle_Distance<=7)
            {
                Stop();
                ui8_LineFollow=0;
                LA_Up();
                delay_us(100000);
                LA_Pause();
                ui8_Position=BELOW_STOPB;
                ui8_LineFollow=1;
            }
        }
//--------------------------At Stop B-------------------------------//
        if(ui8_Position==AT_STOPB)
        {
            Stop();
            ui8_LineFollow=0;
            LA_Down();
            delay_us(300000);
            ui8_Position=RETURN;
        }

////---------------------------Line follow-------------------------------//
//
///////------------------------------------Periodic tasks----------------------------------------------/////

//---------------------------Calculate distance to obstacle-------------------------------//
        if(ui8Flag_DetectObstacle_Wait==0)
        {
            ui16_Obstacle_Distance=SRF05_GetDistance();
            ui8Flag_DetectObstacle_Wait=1;
        }
//---------------------------Read IR sensors--------------------------//
        if( (ui8Flag_ReadIRSensors_Wait==0) && (ui8_LineFollow==1) )
        {
                ui8_LineStatus=Get_Status();
                ui8Flag_ReadIRSensors_Wait=1;
                if(ui8_Position< RETURN)
                {
                    switch (ui8_LineStatus)
                    {
                        case LEFT100:
                            Forward_R();
                            Update_Speed(0, 33, 38, 0); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case LEFT75:
                            Forward_R();
                            Update_Speed(0, 33, 38, 0); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case LEFT50:
                            Forward();
                            Update_Speed(28, 35, 28, 35); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case LEFT25:
                            Forward();
                            Update_Speed(28, 35, 28, 35); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case BALANCED:
                            Forward();
                            Update_Speed(28, 35, 28, 35); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case RIGHT25:
                            Forward();
                            Update_Speed(28, 35, 28, 35); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case RIGHT50:
                            Forward();
                            Update_Speed(28, 35, 28, 35); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case RIGHT75:
                            Forward_L();
                            Update_Speed(33, 0, 0, 38); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case RIGHT100:
                            Forward_L();
                            Update_Speed(33, 0, 0, 38); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case 0:
                            if(ui8_Position==BELOW_STOPA)
                            {
                                Stop();
                                ui8_Position=AT_STOPA;
                            }
                            if(ui8_Position==BELOW_CONTAINER)
                            {
                                Forward();
                                Update_Speed(28, 35, 28, 35); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            }
                            if(ui8_Position==BELOW_STOPB)
                            {
                                Stop();
                                ui8_Position=AT_STOPB;
                            }
                            break;
                    }
                }
                if(ui8_Position>= RETURN)
                {
                    switch (ui8_LineStatus)
                    {
                        case LEFT100:
                            Backward_R();
                            Update_Speed(31, 0, 0, 35); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case LEFT75:
                            Backward_R();
                            Update_Speed(31, 0, 0, 35); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case LEFT50:
                            Backward();
                            Update_Speed(26, 33, 26, 33); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case LEFT25:
                            Backward();
                            Update_Speed(26, 33, 26, 33); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case BALANCED:
                            Backward();
                            Update_Speed(26, 33, 26, 33); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case RIGHT25:
                            Backward();
                            Update_Speed(26, 33, 26, 33); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case RIGHT50:
                            Backward();
                            Update_Speed(26, 33, 26, 33); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case RIGHT75:
                            Backward_L();
                            Update_Speed(0, 31, 35, 0); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case RIGHT100:
                            Backward_L();
                            Update_Speed(0, 31, 35, 0); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                            break;
                        case 0:
                            if(ui8_Position==RETURN)
                            {
                                Backward();
                                Update_Speed(25, 33, 25, 33); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                                ui8_Position=RETURN_AFTER_B;

                            }
                            if(ui8_Position==RETURN_AFTER_B)
                            {
                                Backward();
                                Update_Speed(25, 33, 25, 33); // Wheel 1,3 LEFT. Wheel 2, 4 RIGHT
                                ui8_Position=RETURN_AFTER_A;
                            }
                            break;
                        case OFFLINE:
                            if(ui8_Position==RETURN_AFTER_A)
                            Stop();
                            break;
                    }
                }
                if(prev_status!=ui8_LineStatus)  //Backup old position
                {
                    Record[i]=ui8_LineStatus;
                    i++;
                    prev_status=ui8_LineStatus;
                }
        }
//---------------------------Update current orientation--------------------------//
        if(ui8Flag_L3G4200D_Wait==0)
        {
            fl_gyrorate=L3G4200D_Get_RawValue();
            if(fl_gyrorate!=0)
            {
                fl_gyrorate=(fl_gyrorate-OFFSETZ)*0.00875;
                fl_cur_angle+=fl_gyrorate*0.005;
                ui8Flag_L3G4200D_Wait=1;
            }
        }
//---------------------------Buzzer control--------------------------//
        if(ui8Flag_Buzzer_Wait==0)
        {
            if(ui8_BuzzerOn==1)
            {
                Buzzer_Toggle();
            }
            else
            {
                Buzzer_OFF();
            }
            ui8Flag_Buzzer_Wait=1;
        }

//---------------------------------------------- Test  modules---------------------------------------------------

//        LINE FOLLOWER TEST 2
//        ui8_LineStatus=Get_Status();
//        if(ui8_center==0)
//        {
//            while( (ui8_LineStatus==25) || (ui8_LineStatus>=28 ) )   //lech phai
//            {
//                Forward_L();
//                ui8_LineStatus=Get_Status();
//                if ( (ui8_LineStatus==BALANCED) || (ui8_LineStatus==LEFT25) || (ui8_LineStatus==LEFT25))
//                {
//                    ui8_center=1;
//                    break;
//                }
//                delay_us(100);
//            }
//            while (ui8_LineStatus<=23)
//            {
//                Forward_R();
//                ui8_LineStatus=Get_Status();
//                if ( (ui8_LineStatus==BALANCED) || (ui8_LineStatus==LEFT25) || (ui8_LineStatus==LEFT25))
//                {
//                    ui8_center=1;
//                    break;
//                }
//                delay_us(100);
//            }
//            if ( (ui8_LineStatus==BALANCED) || (ui8_LineStatus==LEFT25) || (ui8_LineStatus==LEFT25))
//            {
//                ui8_center=1;
//            }
//        }
//        if(ui8_center==1)
//        {
//            Forward();
//        }
//        if((ui8_LineStatus==LEFT75) || (ui8_LineStatus==LEFT100) || (ui8_LineStatus==RIGHT75) || (ui8_LineStatus==RIGHT100))
//        {
//            ui8_center=0;
//        }
//        delay_us(10000);

//        MECANUM rotation test
//        if(ui8_test==0)     Stop();
//        if(ui8_test==2)     Forward();
//        if(ui8_test==8)     Backward();
//        if(ui8_test==6)     Left();
//        if(ui8_test==4)     Right();
//        if(ui8_test==3)     Forward_L();
//        if(ui8_test==1)     Forward_R();
//        if(ui8_test==9)     Backward_L();
//        if(ui8_test==7)     Backward_R();
//        if(ui8_test==16)    Turn_L();
//        if(ui8_test==14)    Turn_R();
//        Update_Speed(ui8_M1Duty,ui8_M2Duty,ui8_M3Duty,ui8_M4Duty);

// 	      MOTORs test
//        if(ui16_testmotor==10)  Motor1_Stop();
//        if(ui16_testmotor==20)  Motor2_Stop();
//        if(ui16_testmotor==30)  Motor3_Stop();
//        if(ui16_testmotor==40)  Motor4_Stop();
//
//        if(ui16_testmotor==11) Motor1_Forward();
//        if(ui16_testmotor==21) Motor2_Forward();
//        if(ui16_testmotor==31) Motor3_Forward();
//        if(ui16_testmotor==41) Motor4_Forward();
//
//        if(ui16_testmotor==12) Motor1_Backward();
//        if(ui16_testmotor==22) Motor2_Backward();
//        if(ui16_testmotor==32) Motor3_Backward();
//        if(ui16_testmotor==42) Motor4_Backward();
//        Update_Speed(ui8_M1Duty,ui8_M2Duty,ui8_M3Duty,ui8_M4Duty);

//        LA TEST
//        if (ui8_test==0) LA_Down();
//        if (ui8_test==1) LA_Up();
//        if (ui8_test==2) LA_Pause();
//        delay_us(3000000);
//        Mobile_Stop();
    }
}

//////------------------------------------------------------------------------///////
