/*
 * soft_timer.c
 *
 *  Created on: Jan 11, 2021
 *      Author: victor
 */

#include "soft_timer.h"

/* ================= Callback functions ================= */
void vTimerCallbackC(TimerHandle_t xTimer){

}

void vTimerCallbackR(TimerHandle_t xTimer){

}

/* ================= Functions ================= */
void SoftTimer_Init(fsm_t *fsm){
	xTimerC = xTimerCreateStatic(
                "TimerC",
                fsm->time_c,
                pdFALSE,
                ( void * ) 0,
                vTimerCallbackC,
                &xTimerBufferC);

	xTimerR = xTimerCreateStatic(
                "TimerR",
                fsm->time_r,
                pdFALSE,
                ( void * ) 0,
                vTimerCallbackR,
                &xTimerBufferR);

	if((xTimerC == NULL) || (xTimerR == NULL)){
		vTaskSuspend( NULL );		// Suspend this task
	}
}

void SoftTimer_Start(TimerHandle_t xTimer, fsm_t *fsm){
	const char *xTimerName 		= pcTimerGetName(xTimer);
	const char *xTimerCName 	= pcTimerGetName(xTimerC);
	const char *xTimerRName 	= pcTimerGetName(xTimerR);

	if(xTimerName[5] == xTimerCName[5]){
		xTimerChangePeriod( xTimer, fsm->time_c , 100 );
	}
	if(xTimerName[5] == xTimerRName[5]){

	}
}

void SoftTimer_Stop(TimerHandle_t xTimer, fsm_t *fsm){
	const char *xTimerName 		= pcTimerGetName(xTimer);
	const char *xTimerCName 	= pcTimerGetName(xTimerC);
	const char *xTimerRName 	= pcTimerGetName(xTimerR);

	if(xTimerName[5] == xTimerCName[5]){
		xTimerStop( xTimerC, 100 );
		fsm->time_r = xTimerGetExpiryTime( xTimerC ) - xTaskGetTickCount();
	}
	if(xTimerName[5] == xTimerRName[5]){
		xTimerStop( xTimerR, 100 );
		fsm->time_r = xTimerGetExpiryTime( xTimerR ) - xTaskGetTickCount();
	}
}
