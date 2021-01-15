/*
 * soft_timer.c
 *
 *  Created on: Jan 11, 2021
 *      Author: victor
 */

#include "soft_timer.h"

/* ================= Callback function ================= */
void vTimerCallbackR(TimerHandle_t xTimer){
	xSemaphoreGive(xSemaphoreC);
}

/* ================= Functions ================= */
void SoftTimer_Init(fsm_t *fsm){

	xSemaphoreB1 = xSemaphoreCreateBinaryStatic(&xSemaphoreBufferB1);
	xSemaphoreB2 = xSemaphoreCreateBinaryStatic(&xSemaphoreBufferB2);
	xSemaphoreC  = xSemaphoreCreateBinaryStatic(&xSemaphoreBufferC);
	xSemaphoreS  = xSemaphoreCreateBinaryStatic(&xSemaphoreBufferS);

	xTimerR = xTimerCreateStatic(
                "TimerR",
                fsm->time_r,
                pdFALSE,
                ( void * ) 0,
                vTimerCallbackR,
                &xTimerBufferR);

}

void SoftTimer_Start(TimerHandle_t xTimer, fsm_t *fsm){
	/* ChangePeriod calls xTimerStart internally */
	xTimerChangePeriod( xTimer, fsm->time_r , (TickType_t)0 );
}

void SoftTimer_Stop(TimerHandle_t xTimer, fsm_t *fsm){
	xTimerStop( xTimerR, 100 );

	/* In this state, i can close or open it. So i need to know when
	 * opening/closing time has passed before i got here
	 */
	if(xSemaphoreTake(xSemaphoreB1, (TickType_t)0) == pdPASS){
		fsm->time_r = xTimerGetExpiryTime( xTimerR ) - xTaskGetTickCount();
	}
	else{
		if(xSemaphoreTake(xSemaphoreB2, (TickType_t)0) == pdPASS){
			uint16_t remainingToOpen;
			remainingToOpen = xTimerGetExpiryTime( xTimerR ) - xTaskGetTickCount();
			fsm->time_r = fsm->time_c - remainingToOpen;
		}
	}
}
