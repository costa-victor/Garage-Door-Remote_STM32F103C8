/*
 * fsm.h
 *
 *  Created on: Jan 10, 2021
 *      Author: victor
 */

#ifndef FSM_FSM_H_
#define FSM_FSM_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "soft_timer.h"

/* ================= Defines ================= */
#define QUEUE_LENGTH    10
#define ITEM_SIZE       sizeof( BaseType_t )
#define tskPriority 	(UBaseType_t)(tskIDLE_PRIORITY + 5)
#define STACK_SIZE 		512


/* ================= Data types ================= */
typedef enum{
	Closed = 0,
	Opening,
	Opened,
	Closing,
	Stopped
} gate_state;

typedef struct StateMachine fsm_t;
typedef void (*action_t)(fsm_t *fsm);

struct StateMachine{
	gate_state prevState;		/* Holds the previous state of FSM */
	gate_state state;		/* Holds the current state of FSM */
	action_t action[5];		/* Function pointer to the action to be performed */
	uint16_t time_c;		/* Holds the time to open/close completely from open/close state*/
	uint16_t time_r;		/* Holds the remaining time to open/close completely from stopped state*/
};


/* ================= Variables ================= */
StaticQueue_t xStaticQueue;									/* The variable used to hold the queue's data structure. */
QueueHandle_t xQueue;										/* Handler to queue */
uint8_t ucQueueStorageArea[ QUEUE_LENGTH * ITEM_SIZE ];		/* The array to use as the queue's storage are */
StaticTask_t xTaskBuffer;									/* Structure that will hold the TCB of the task being created. */
StackType_t xStack[ STACK_SIZE ];							/* Buffer that the task being created will use as its stack */
TaskHandle_t xHandle;										/* Task handler */
fsm_t fsm_var;												/* Instance of state machine */


/* ================= Headers ================= */
void FSM_Init(fsm_t *fsm);
void FSM_Start();
extern void SoftTimer_Init(fsm_t *fsm);
extern void SoftTimer_Start(TimerHandle_t xTimer, fsm_t *fsm);
extern void SoftTimer_Stop(TimerHandle_t xTimer, fsm_t *fsm);


#endif /* FSM_FSM_H_ */
