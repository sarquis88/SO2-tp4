#include "DriverLib.h"
#include "printf-stdarg.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <string.h>

#define mainPUSH_BUTTON          GPIO_PIN_4

#define TOP_PRIORITY		         ( tskIDLE_PRIORITY + 1 )
#define PRODUCER_PRIORITY		     ( tskIDLE_PRIORITY + 3 )
#define CONSUMER_PRIORITY		     ( tskIDLE_PRIORITY + 3 )

#define PRODUCER_DELAY_MS			1
#define CONSUMER_DELAY_MS		  1
#define TOP_DELAY_MS					2000

#define QUEUE_SIZE				    10

#define PRODUCER_STACK        ( configMINIMAL_STACK_SIZE )
#define CONSUMER_STACK        ( configMINIMAL_STACK_SIZE )
#define TOP_STACK             ( configMINIMAL_STACK_SIZE + 10 )

#define STATUS_RUNNING		( 'X' )
#define STATUS_BLOCKED		( 'B' )
#define STATUS_READY			( 'R' )
#define STATUS_DELETED		( 'D' )
#define STATUS_SUSPENDED	( 'S' )

/**
 *  Tarea encargada de consumir del buffer
 *  @param puntero a void (no usado)
 */
static void vTaskConsumer			  ( void * );

/**
 *  Tarea encargada de producir en el buffer
 *  @param puntero a void (no usado)
 */
static void vTaskProducer			  ( void * );

/**
 *  Tarea encargada de loggear información de las demas tareas
 *  @param puntero a void (no usado)
 */
static void vTaskTop				    ( void * );

/**
 *  Convierte status de int a char
 *  @param int que representa algun status
 *  @return char que representa el status indicado por el int
 */
static char get_task_status			( int );

/**
 *  Funcion utilizada por vTaskTop. Realiza todo el trabajo: reune la
 *  información y la loggea
 */
static void print_runtime_stats		( void );

/**
 *  Impresión de mensaje mediante UART
 *  @param mensaje a imprimir
 *  @param int longitud del mensaje
 */
static void print_in_uart				( char *, int );

/**
 *  Imprime una linea entera de '#' usando print_in_uart
 */
static void print_line          ( void );

/**
 *  Funcion de sistema encargada de inicializar el timer
 */
void vSetupHighFrequencyTimer   ( void );
