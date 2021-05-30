/* FreeRTOS includes. */
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		( 0xfffff )
#define TASKSAME 1
/* The task functions prototype*/
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );
void vTaskFunc( void *pvParameters );
TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;
/*-----------------------------------------------------------*/

int main( void )
{
  
	/* essential Board initializations */
	SystemInit();

	
	/* This function initializes the MCU clock, PLL will be used to generate Main MCU clock */
  	SystemCoreClockUpdate();
	
	/* Initialize the serial I/O(console ), This function configures Due's CONSOLE_UART */
	const char * firstpara = "This is task 1\r\n";
	const char * secondpara = "This is task 2\r\n";

	
	printf("FreeRTOS running on STM32F407 Discovery Board\n");
#if (TASKSAME == 1)
	/* Create one of the two tasks. */
	xTaskCreate(	vTask1,		/* Pointer to the function that implements the task. */
					"Task 1",	/* Text name for the task.  This is to facilitate debugging only. */
					240,		/* Stack depth in words. */
					NULL,		/* We are not using the task parameter. */
					2,			/* This task will run at priority 1. */
					&xTask1Handle);		/* We are not using the task handle. */

	/* Create the other task in exactly the same way. */
	xTaskCreate( vTask2, "Task 1", 240, NULL, 1, &xTask2Handle);
#else
	xTaskCreate( vTaskFunc, "Task 1", 240, (void*) firstpara, 1, NULL );
	xTaskCreate( vTaskFunc, "Task 2", 240, (void*) secondpara, 2, NULL );
#endif
	/* Start the scheduler so our tasks start executing. */
	vTaskStartScheduler();
	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vTask1( void *pvParameters )
{
	const char *pcTaskName = "Task 1 is running\n";
	volatile unsigned long ul;
	UBaseType_t ulGetPriorityTsk1 = uxTaskPriorityGet(NULL);
	UBaseType_t ulGetPriorityTsk2 = uxTaskPriorityGet(xTask2Handle);
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
    	printf("%s\r\n",pcTaskName);
		printf("Current priority of task 1 is %d\r\n",ulGetPriorityTsk1);
		printf("Current priority of task 2 is %d\r\n",ulGetPriorityTsk2);
		ul = 0; // Reset counter since task is continuing at that point
		while(ul < mainDELAY_LOOP_COUNT) ul++;// Loop doing nothing other than delaying
		vTaskPrioritySet(xTask2Handle, ulGetPriorityTsk1 + 1); // Prioritize task 2
	}
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
	const char *pcTaskName = "Task 2 is running\n";
	volatile unsigned long ul;
	UBaseType_t ulGetPriorityTsk1 = uxTaskPriorityGet(xTask1Handle);
	UBaseType_t ulGetPriorityTsk2 = uxTaskPriorityGet(NULL);	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
		printf("%s\n",pcTaskName);
		printf("Current priority of task 1 is %d\r\n",ulGetPriorityTsk1);
		printf("Current priority of task 2 is %d\r\n",ulGetPriorityTsk2);
		ul = 0;
		while(ul < mainDELAY_LOOP_COUNT) ul++; // Loop doing nothing other than delaying
		/* for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ ) */
		vTaskPrioritySet(xTask2Handle, ulGetPriorityTsk1 - 1); // Prioritize task 1
	}
}
/*-----------------------------------------------------------*/
		
void vTaskFunc( void *pvParameters )
{
	volatile unsigned long count = 0;
	while(1)
	{
		printf("%s",pvParameters);
		//while(++count < mainDELAY_LOOP_COUNT); // Brutal loop to delay the task, CPU consumes time for this task.
		vTaskDelay(pdMS_TO_TICKS(2000)); // Pushing task into "Blocked State", CPU is free for other tasks.
	}
}
/*-----------------------------------------------------------*/


void vApplicationMallocFailedHook( void )
{
	/* This function will only be called if an API call to create a task, queue
	or semaphore fails because there is too little heap RAM remaining. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                               char * pcTaskName )
{
	/* This function will only be called if a task overflows its stack.  Note
	that stack overflow checking does slow down the context switch
	implementation. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* This example does not use the idle hook to perform any processing. */
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This example does not use the tick hook to perform any processing. */
}

/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
