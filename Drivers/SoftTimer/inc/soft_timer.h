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
#include "fsm.h"

/* ================= Variables ================= */
TimerHandle_t xTimerC;										/* Software timer handler - Completely time */
TimerHandle_t xTimerR;										/* Software timer handler - Remaining time */
StaticTimer_t xTimerBufferC;								/* StaticTimer_t structures, which are used to store the state of each created timer. */
StaticTimer_t xTimerBufferR;								/* Idem */


#endif /* SOFTTIMER_INC_SOFT_TIMER_H_ */
