/*
 * fsm.c
 *
 *  Created on: Jan 10, 2021
 *      Author: victor
 */

#include <fsm.h>
#include "main.h"

static void ClosedAction(fsm_t *fsm){
	if(xSemaphoreTake(xSemaphoreB1, (TickType_t)0) == pdPASS){
		// Blink
		HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, LED_OFF);
		vTaskDelay(100);
		HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, LED_ON);
		vTaskDelay(100);
		HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, LED_OFF);

		fsm->prevState	= Closed;
		fsm->state 		= Opening;

		fsm->time_r = fsm->time_c;
		SoftTimer_Start(xTimerR, fsm);
	}
}

static void OpeningAction(fsm_t *fsm){
	if(xSemaphoreTake(xSemaphoreB1, (TickType_t)0) == pdPASS){
		xSemaphoreGive(xSemaphoreB1);
		SoftTimer_Stop(xTimerR, fsm);
		fsm->prevState	= Opening;
		fsm->state 		= Stopped;
	}
	else if(xSemaphoreTake(xSemaphoreS, (TickType_t)0) == pdPASS){
		fsm->prevState	= Opening;
		fsm->state 		= Stopped;
	}
	else if(xSemaphoreTake(xSemaphoreC, (TickType_t)0) == pdPASS){
			fsm->prevState	= Opening;
			fsm->state 		= Opened;

			// Led off when completely openned
			HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, LED_OFF);
	}

	// Led on while the gate is opening
	HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, LED_ON);
}

static void OpenedAction(fsm_t *fsm){
	if(xSemaphoreTake(xSemaphoreB2, (TickType_t)0) == pdPASS){
		// Blink
		HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, LED_OFF);
		vTaskDelay(100);
		HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, LED_ON);
		vTaskDelay(100);
		HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, LED_OFF);

		fsm->prevState	= Opened;
		fsm->state 		= Closing;

		fsm->time_r = fsm->time_c;
		SoftTimer_Start(xTimerR, fsm);
	}
}

static void ClosingAction(fsm_t *fsm){
	if(xSemaphoreTake(xSemaphoreB2, (TickType_t)0) == pdPASS){
		xSemaphoreGive(xSemaphoreB2);
		SoftTimer_Stop(xTimerR, fsm);
		fsm->prevState	= Closing;
		fsm->state 		= Stopped;
	}
	else if(xSemaphoreTake(xSemaphoreS, (TickType_t)0) == pdPASS){
		fsm->prevState	= Closing;
		fsm->state 		= Stopped;
	}
	else if(xSemaphoreTake(xSemaphoreC, (TickType_t)0) == pdPASS){
			fsm->prevState	= Closing;
			fsm->state 		= Closed;
	}

	// Led off while the gate is opening
	HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, LED_OFF);
}

static void StoppedAction(fsm_t *fsm){
	if(xSemaphoreTake(xSemaphoreB1, (TickType_t)0) == pdPASS){
		fsm->prevState	= Stopped;
		fsm->state 		= Opening;
		SoftTimer_Start(xTimerR, fsm);

		xSemaphoreGive(xSemaphoreS);
	}
	else{
		if(xSemaphoreTake(xSemaphoreB2, (TickType_t)0) == pdPASS){
			fsm->prevState	= Stopped;
			fsm->state 		= Closing;
			SoftTimer_Start(xTimerR, fsm);

			xSemaphoreGive(xSemaphoreS);
		}
	}

	// Led blinks continuously
	HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, LED_OFF);
	vTaskDelay(100);
	HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, LED_ON);
	vTaskDelay(100);
	HAL_GPIO_WritePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin, LED_OFF);

}

void FSM_Init(fsm_t *fsm){
	/* Initial gate state */
	fsm->prevState	= Closed;
	fsm->state 		= Closed;

	/* Initial time in ms */
	fsm->time_c = 5000;
	fsm->time_r	= fsm->time_c;

	/* Action functions related to their respective states */
	fsm->action[Closed]		= ClosedAction;
	fsm->action[Opening]	= OpeningAction;
	fsm->action[Opened]		= OpenedAction;
	fsm->action[Closing]	= ClosingAction;
	fsm->action[Stopped]	= StoppedAction;
}

void FSM_Execute(fsm_t *fsm){
	fsm->action[fsm->state](fsm);		/* Calls the function related to current state */
}




void FSM_Task( void *param )
{
    /* Create a queue capable of containing 10 BaseType_t values. */
    xQueue = xQueueCreateStatic( QUEUE_LENGTH,
                                 ITEM_SIZE,
                                 ucQueueStorageArea,
                                 &xStaticQueue );

    /* pxQueueBuffer was not NULL so xQueue should not be NULL. */
    if( xQueue == NULL){
    	vTaskSuspend( NULL );	// Suspend this task
    }


    SoftTimer_Init(&fsm_var);	/* Initializing Software Timers */
    FSM_Init(&fsm_var);			/* Initializing FSM */

    for(;;){
    	FSM_Execute(&fsm_var);
    	vTaskDelay(100);
    }
 }

void FSM_Start(){

    /* Create the task without using any dynamic memory allocation. */
    xHandle = xTaskCreateStatic(
                  FSM_Task,       	/* Function that implements the task. */
                  "FSM",          	/* Text name for the task. */
                  STACK_SIZE,      	/* Number of indexes in the xStack array. */
                  ( void * ) 0,    	/* Parameter passed into the task. */
				  tskPriority,		/* Priority at which the task is created. */
                  xStack,          	/* Array to use as the task's stack. */
                  &xTaskBuffer );  	/* Variable to hold the task's data structure. */


    /* puxStackBuffer and pxTaskBuffer were not NULL, so the task will have
    been created, and xHandle will be the task's handle. */
    if(xHandle == NULL){
    	vTaskSuspend( xHandle );		// Suspend this task
    }

    /* Starts the scheduler */
    vTaskStartScheduler();
}
