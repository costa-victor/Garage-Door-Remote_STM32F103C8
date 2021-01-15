/*
 * soft_timer.h
 *
 *  Created on: Jan 11, 2021
 *      Author: victor
 */

#ifndef SOFTTIMER_INC_SOFT_TIMER_H_
#define SOFTTIMER_INC_SOFT_TIMER_H_

#include "FreeRTOS.h"
#include "timers.h"
#include "semphr.h"
#include "fsm.h"

/* ================= Variables ================= */
TimerHandle_t xTimerC;										/* Static Software timer handler - Completely time */
TimerHandle_t xTimerR;										/* Static Software timer handler - Remaining time */
StaticTimer_t xTimerBufferC;								/* StaticTimer_t structures, which are used to store the state of each created timer. */
StaticTimer_t xTimerBufferR;								/* Idem */

SemaphoreHandle_t xSemaphoreB1;								/* Static semaphore binary for synchronization with pressed event  B1*/
SemaphoreHandle_t xSemaphoreB2;								/* Static semaphore binary for synchronization with pressed event  B2*/
SemaphoreHandle_t xSemaphoreC;								/* Static semaphore binary for synchronization when xTimerC occurs completely */
SemaphoreHandle_t xSemaphoreS;								/* Static semaphore binary for synchronization when B1/B2 was pressed on stopped state */
StaticSemaphore_t xSemaphoreBufferB1;						/* StaticSemaphore_t structures, which are used to store the semaphore */
StaticSemaphore_t xSemaphoreBufferB2;						/* Idem */
StaticSemaphore_t xSemaphoreBufferC;						/* Idem */
StaticSemaphore_t xSemaphoreBufferS;						/* Idem */

#endif /* SOFTTIMER_INC_SOFT_TIMER_H_ */
