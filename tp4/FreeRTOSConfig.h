#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configUSE_PREEMPTION		1
#define configUSE_IDLE_HOOK			0
#define configUSE_TICK_HOOK			0
#define configCPU_CLOCK_HZ			( ( unsigned long ) 20000000 )
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 70 )
#define configTOTAL_HEAP_SIZE		( ( size_t ) ( 5000 ) ) //antes 7000
#define configMAX_TASK_NAME_LEN		( 10 )
#define configUSE_TRACE_FACILITY	1
#define configUSE_16_BIT_TICKS		0
#define configIDLE_SHOULD_YIELD		0
#define configUSE_CO_ROUTINES 		0

#define configMAX_PRIORITIES		( 5 )
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				0
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			0
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1

#define configKERNEL_INTERRUPT_PRIORITY 		255
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191 /* equivalent to 0xa0, or priority 5. */

/* Optional functions - most linkers will remove unused functions anyway. nuevo para TP4 */
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_xTaskGetCurrentTaskHandle       1

#define INCLUDE_xTaskGetIdleTaskHandle          1
#define INCLUDE_eTaskGetState                   1
#define configSUPPORT_DYNAMIC_ALLOCATION        1

#define configGENERATE_RUN_TIME_STATS           1
#define configUSE_STATS_FORMATTING_FUNCTIONS    1

extern volatile unsigned long ulHighFrequencyTimerTicks;
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() ( ulHighFrequencyTimerTicks = 0UL )
#define portGET_RUN_TIME_COUNTER_VALUE()	ulHighFrequencyTimerTicks

/* Software timer related constants. */
#define configUSE_TIMERS				0
#define configTIMER_TASK_PRIORITY		2
#define configTIMER_QUEUE_LENGTH		20
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )
#endif /* FREERTOS_CONFIG_H */
