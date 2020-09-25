
/*==================[inclusiones]============================================*/

#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"
#include "semphr.h"
#include "FreeRTOSConfig.h"

/*==================[definiciones y macros]==================================*/

#define RATE 4000
#define LED_RATE pdMS_TO_TICKS(RATE)
#define BUTTON_RATE 1

/*==================[definiciones de datos internos]=========================*/

SemaphoreHandle_t sem_tec_pulsada;

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;

/*==================[declaraciones de funciones internas]====================*/
void clear_diff();
void fsmButtonInit();
void fsmButtonUpdate( gpioMap_t tecla );
/*==================[declaraciones de funciones externas]====================*/


void tarea_led( void* taskParmPtr );
void tarea_tecla( void* taskParmPtr );

/*==================[funcion principal]======================================*/

int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------
	boardConfig();

	debugPrintConfigUart( UART_USB, 115200 );
	printf( "Ejercicio C_4.\r\n" );


    BaseType_t res_A =
    xTaskCreate(
        tarea_led,
        ( const char * )"tarea_led",
        configMINIMAL_STACK_SIZE*2,
        0,
        tskIDLE_PRIORITY+1,
        0
    );

    BaseType_t res_B =
    xTaskCreate(
    	tarea_tecla,
        ( const char * )"tarea_tecla",
        configMINIMAL_STACK_SIZE*2,
        0,
        tskIDLE_PRIORITY+1,
        0
    );


	if((res_A || res_B) == pdFAIL)
	{
		gpioWrite( LEDR, ON );
		printf( "Error al crear las tareas.\r\n" );
		while(TRUE);
	}

	sem_tec_pulsada = xSemaphoreCreateBinary();

	if( sem_tec_pulsada == NULL)
	{
		gpioWrite( LEDR, ON );
		printf( "Error al crear los semaforos.\r\n" );
		while(TRUE);
	}


    vTaskStartScheduler();


    while( TRUE )
    {

    }

    return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

void tarea_tecla( void* taskParmPtr )
{
	fsmButtonInit();

	while( TRUE )
	{
		fsmButtonUpdate( TEC1 );
	 	vTaskDelay( BUTTON_RATE / portTICK_RATE_MS );
	 	//vTaskDelay ( pdMS_TO_TICKS(BUTTON_RATE) );
	}
}

// Implementacion de funcion de la tarea
void tarea_led( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------
	gpioMap_t led;
	TickType_t xPeriodicity =  LED_RATE;		// Tarea periodica cada 1000 ms
	TickType_t dutty =  xPeriodicity/2;
	TickType_t xTickSem = 0;
	TickType_t xLastWakeTime = xTaskGetTickCount();
    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
    	//siempre que hay tiempo finito ramificar
    	if(xSemaphoreTake( sem_tec_pulsada ,xTickSem)== pdTRUE){
			//se libera el semaforo
			led=LEDB;
			//clear_diff();
    	}else{
    		//se cumplio el tiempo y no se libero
    		led= LEDR;
    	}
		gpioWrite( led , ON );
		vTaskDelay( dutty );
		gpioWrite( led , OFF );
		vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
    }
}

/*==================[fin del archivo]========================================*/
