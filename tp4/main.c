#include "prod_cons.h"

static volatile char *pcNextChar;
SemaphoreHandle_t xButtonSemaphore;
QueueHandle_t xQueue;

int main( void )
	{
		SysCtlClockSet( SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL |
			 							SYSCTL_OSC_MAIN | SYSCTL_XTAL_6MHZ );
		vSetupHighFrequencyTimer();

		xQueue = xQueueCreate( QUEUE_SIZE, sizeof( char * ) );

		xTaskCreate( 	vTaskProducer, 	"P1", 	PRODUCER_STACK, &xQueue,
		 							PRODUCER_PRIORITY, NULL );
		xTaskCreate( 	vTaskProducer, 	"P2", 	PRODUCER_STACK, &xQueue,
									PRODUCER_PRIORITY, NULL );
		xTaskCreate( 	vTaskProducer, 	"P3", 	PRODUCER_STACK, &xQueue,
									PRODUCER_PRIORITY, NULL );
		xTaskCreate( 	vTaskProducer, 	"P4", 	PRODUCER_STACK, &xQueue,
									PRODUCER_PRIORITY, NULL );
		xTaskCreate( 	vTaskConsumer, 	"C1", 	CONSUMER_STACK, &xQueue,
									CONSUMER_PRIORITY, NULL );
		xTaskCreate( 	vTaskTop, 			"TOP", 	TOP_STACK, 			NULL,
									TOP_PRIORITY, NULL );

		vTaskStartScheduler();

		return 0;
	}

/* ######################################################################## */
/* ################################# TAREAS ############################### */
/* ######################################################################## */

static void
vTaskProducer( void *pvParameters )
	{
	 	const TickType_t xDelay = PRODUCER_DELAY_MS / portTICK_PERIOD_MS;
		char * p = "p";

		for( ;; )
			{
				if( xQueueSend( xQueue, p, 0 ) == pdFALSE )
					for(;;)
				vTaskDelay( xDelay );
			}
	}

static void
vTaskConsumer( void *pvParameters )
	{
		const TickType_t xDelay = CONSUMER_DELAY_MS / portTICK_PERIOD_MS;

		for( ;; )
			{
				if( xQueueReceive( xQueue, NULL, 0 ) == pdFALSE )
					for(;;)
				vTaskDelay(xDelay);
			}
	}

static void
vTaskTop(void *pvParameters)
	{
		const TickType_t xDelay = TOP_DELAY_MS / portTICK_PERIOD_MS;
		int i = 0;
		char * title = "\t\tTOP number";
		char msg[ strlen(title) + 3 ];

		for( ;; )
		{
			vTaskDelay( xDelay );

			sprintf(msg, "%s %d", title, i++);
			print_in_uart( msg, strlen(msg) );
			print_runtime_stats();
		}
	}

/* ######################################################################## */
/* ################################# FUNCIONES ############################ */
/* ######################################################################## */

static void
print_runtime_stats( void )
	{
		TaskStatus_t *pxTaskStatusArray;
		char * pcWriteBuffer = pvPortMalloc(sizeof(char)*50);
		char * pcHeader = pvPortMalloc(sizeof(char)*50);
		volatile UBaseType_t uxArraySize, x;
		unsigned long ulTotalRunTime, ulStatsAsPercentage;

		* pcWriteBuffer = 0x00;
		uxArraySize = uxTaskGetNumberOfTasks();

		print_line();
		sprintf(pcHeader,
				" # %s\t%s\t%s\t%s\t%s\t%s\t#",
				"Task",
				"State",
				"Prio",
				"Num",
				"WM",
				"%");
		print_in_uart( pcHeader, strlen(pcHeader) );
		print_line();

		pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

		if( pxTaskStatusArray != NULL )
		{
			uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
												uxArraySize,
												&ulTotalRunTime );

			/* 	For percentage calculations. */
			ulTotalRunTime /= 100UL;

			if( ulTotalRunTime > 0 )
			{
				/* 	For each populated position in the pxTaskStatusArray array,
					format the raw data as human readable ASCII data. */

				for( x = 0; x < uxArraySize; x++ )
				{
					/* 	What percentage of the total run time has the task used?
						This will always be rounded down to the nearest integer.
						ulTotalRunTimeDiv100 has already been divided by 100. */
					ulStatsAsPercentage =
						pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;

					sprintf(pcWriteBuffer,
							" # %s\t%c\t%d\t%d\t%d\t%d%%\t#",
							pxTaskStatusArray[ x ].pcTaskName,
							get_task_status((int) pxTaskStatusArray[ x ].eCurrentState),
							pxTaskStatusArray[ x ].uxCurrentPriority,
							pxTaskStatusArray[ x ].xTaskNumber,
							pxTaskStatusArray[ x ].usStackHighWaterMark,
							ulStatsAsPercentage);
					print_in_uart( pcWriteBuffer, strlen(pcWriteBuffer) );
				}
			}
		print_line();
		print_in_uart( "\n", 1 );

		vPortFree( pxTaskStatusArray );
		vPortFree( pcWriteBuffer );
		vPortFree( pcHeader );
	 }
	}

static void
print_in_uart(char * msg, int len)
	{
		int i;

		for(i = 0; i < len; i++)
			UARTCharPut( UART0_BASE, msg[i] );
		UARTCharPut( UART0_BASE, '\r');
		UARTCharPut( UART0_BASE, '\n');
	}

static void
print_line()
	{
		char * line = " ################################################";
		print_in_uart( line, strlen(line) );
	}

static char
get_task_status(int status)
	{
		char cStatus = ( char ) 0x00;

		switch (status)
			{
				case eRunning:
					cStatus = STATUS_RUNNING;
					break;
				case eReady:
					cStatus = STATUS_READY;
					break;
				case eBlocked:
					cStatus = STATUS_BLOCKED;
					break;
				case eSuspended:
					cStatus = STATUS_SUSPENDED;
					break;
				case eDeleted:
					cStatus = STATUS_DELETED;
					break;
				case eInvalid:
				default:
					break;
				}
		return cStatus;
	}

void
vUART_ISR(void)
	{
		unsigned long ulStatus;
		ulStatus = UARTIntStatus( UART0_BASE, pdTRUE );
		UARTIntClear( UART0_BASE, ulStatus );
		if( ulStatus & UART_INT_TX )
		{
			if( *pcNextChar != 0 )
			{
				if( !( HWREG( UART0_BASE + UART_O_FR ) & UART_FR_TXFF ) )
				{
					HWREG( UART0_BASE + UART_O_DR ) = *pcNextChar;
				}
				pcNextChar++;
			}
		}
	}

void
vGPIO_ISR( void )
	{
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		GPIOPinIntClear(GPIO_PORTC_BASE, mainPUSH_BUTTON);
		xSemaphoreGiveFromISR( xButtonSemaphore, &xHigherPriorityTaskWoken );
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	}
