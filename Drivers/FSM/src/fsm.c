/*
 * fsm.c
 *
 *  Created on: Jan 10, 2021
 *      Author: victor
 */

#include <fsm.h>


static void ClosedAction(fsm_t *fsm){
	fsm->state = Opening;
}

static void OpeningAction(fsm_t *fsm){
	fsm->state = Opened;
}

static void OpenedAction(fsm_t *fsm){
	fsm->state = Closing;

}

static void ClosingAction(fsm_t *fsm){
	fsm->state = Closed;

}

static void StoppedAction(fsm_t *fsm){
}

void FSM_Init(fsm_t *fsm){
	/* Initial gate state */
	fsm->state 	= Closed;

	/* Initial time in ms */
	fsm->time_c = 3000;
	fsm->time_r	= 3000;

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
