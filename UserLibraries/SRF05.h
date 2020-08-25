/*
 * SRF05.h
 *
 *  Created on: Jul 30, 2019
 *      Author: Itachi
 */

#ifndef USERLIBRARIES_SRF05_H_
#define USERLIBRARIES_SRF05_H_

#define TIMER_VALUE     (*((volatile unsigned long *)0x40032050))
#define Trigger_PIN     GPIO_PIN_7
#define Trigger_PORT    GPIO_PORTC_BASE
#define Echo_PIN        GPIO_PIN_6
#define Echo_PORT       GPIO_PORTC_BASE



/* -----------SRF05_Init----------------
 * GPIO Init for SRF05 (Trigger pin and Echo pin)
         * Trigger pin - output
         * Echo pin - input
 * Timer Init for SRF05 (To measure the pulse width of Echo pin)
         * Timer 32-bit full width, periodic up
 * Input: No
 * Output: No
 */
void SRF05_Init(void);

/* -----------SRF05_Handler----------------
 * Echo pin interrupt both edges handler
 * If detect rising edge, then reset timer value and start timer
 * If detect falling edge, then get timer value (Pulse_Width) and disable timer
 * Input: No
 * Output: No
 * Affect global variable:
     * Echo_wait - is true if SRF05 receiveed respond signal
     * Pulse_Width - The duration between two edges
 */
void SRF05_Handler(void);

/* -----------SRF05_GetDistance----------------
* SRF05 Get Distance to obstacle, to get rid of disturbs, only return value <100 cm
* Input: No
* Output: uint16_t ui16Distance
*/
uint16_t SRF05_GetDistance(void);

#endif /* USERLIBRARIES_SRF05_H_ */
