/*
 * Double_H_Bridges.h
 *
 *  Created on: Jul 8, 2019
 *      Author: Itachi
 */

#ifndef DOUBLE_H_BRIDGES_H_
#define DOUBLE_H_BRIDGES_H_

#define BRIDGEH1_GPIO_PORT       GPIO_PORTE_BASE
#define BRIDGEH2_GPIO_PORT       GPIO_PORTD_BASE

#define BRIDGEH1_PWM_PORT        GPIO_PORTC_BASE
#define BRIDGEH2_PWM_PORT        GPIO_PORTF_BASE

#define DATA_PE_R               HWREG(0x400243FC)
#define DATA_PD_R               HWREG(0x400073FC)

#define IN1_PIN                 GPIO_PIN_0 //port e
#define IN2_PIN                 GPIO_PIN_1
#define IN3_PIN                 GPIO_PIN_2
#define IN4_PIN                 GPIO_PIN_3

#define IN5_PIN                 GPIO_PIN_0
#define IN6_PIN                 GPIO_PIN_1
#define IN7_PIN                 GPIO_PIN_2
#define IN8_PIN                 GPIO_PIN_3

#define ENA_PIN                 GPIO_PIN_4
#define ENB_PIN                 GPIO_PIN_5
#define ENC_PIN                 GPIO_PIN_2
#define END_PIN                 GPIO_PIN_3

#define MOTOR1_FORWARD          IN1_PIN
#define MOTOR1_BACKWARD         IN2_PIN
#define MOTOR2_FORWARD          IN3_PIN
#define MOTOR2_BACKWARD         IN4_PIN
#define MOTOR3_FORWARD          IN5_PIN
#define MOTOR3_BACKWARD         IN6_PIN
#define MOTOR4_FORWARD          IN7_PIN
#define MOTOR4_BACKWARD         IN8_PIN


#define MOTOR1                  (IN1_PIN|IN2_PIN)
#define MOTOR2                  (IN3_PIN|IN4_PIN)
#define MOTOR3                  (IN5_PIN|IN6_PIN)
#define MOTOR4                  (IN7_PIN|IN8_PIN)

#define PWM_PERIOD              20000 //PWM 5Khz - SysClock = 80MHz

//#define M1_OFFSET               780
//#define M2_OFFSET               0
//#define M3_OFFSET               790
//#define M4_OFFSET               450
////
//#define M1_OFFSET               780   //40 25 on
//#define M2_OFFSET               0
//#define M3_OFFSET               790
//#define M4_OFFSET               350


#define M1_OFFSET                1200   //40 25 on
#define M2_OFFSET                -300
#define M3_OFFSET                100
#define M4_OFFSET                -70


//#define M1_OFFSET                0   //40 25 on
//#define M2_OFFSET                0
//#define M3_OFFSET                0
//#define M4_OFFSET                0



void BridgeH_GPIO_Init(void);
void BridgeH_PWM_Init(void);
void Update_Speed(uint8_t ui8_M1Duty, uint8_t ui8_M2Duty,uint8_t ui8_M3sDuty,uint8_t ui8_M4Duty);
void Motor1_Forward(void);
void Motor1_Backward(void);
void Motor1_Stop(void);
void Motor2_Forward(void);
void Motor2_Backward(void);
void Motor2_Stop(void);
void Motor3_Forward(void);
void Motor3_Backward(void);
void Motor3_Stop(void);
void Motor4_Forward(void);
void Motor4_Backward(void);
void Motor4_Stop(void);


#endif /* DOUBLE_H_BRIDGES_H_ */
