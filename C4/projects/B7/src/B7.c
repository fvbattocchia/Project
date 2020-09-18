/*=============================================================================
 * Copyright (c) 2020, Martin N. Menendez <menendezmartin81@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/03
 * Version: v1.1
 *===========================================================================*/

/*==================[inclusiones]============================================*/

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"

#include "sapi.h"

/*==================[definiciones y macros]==================================*/

#define RATE 40
#define LED_RATE pdMS_TO_TICKS(RATE)
#define BUTTON_RATE 1

#define PERIOD_RATE 7000
#define LOADING_RATE pdMS_TO_TICKS(PERIOD_RATE)

#define STEP 500
#define MAX  (PERIOD_RATE - STEP)
#define MAX_RATE pdMS_TO_TICKS(MAX)

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/


DEBUG_PRINT_ENABLE;

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/
TickType_t get_diff();
void clear_diff();

// Prototipo de funcion de la tarea
void tarea_led( void* taskParmPtr );
void tarea_tecla( void* taskParmPtr );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------
	boardConfig();

	debugPrintConfigUart( UART_USB, 115200 );
	printf( "Ejercicio B_3.\r\n" );



    // Crear tarea en freeRTOS
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

    // Gestion de errores
	if((res_A || res_B) == pdFAIL)
	{
		gpioWrite( LEDR, ON );
		printf( "Error al crear las tareas.\r\n" );
		while(TRUE);
	}


    vTaskStartScheduler();

    // ---------- REPETIR POR SIEMPRE --------------------------
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


    TickType_t xPeriodicity =  LOADING_RATE;					// Idem anterior pero mas elegante

    TickType_t xLastWakeTime = xTaskGetTickCount();

    gpioMap_t led = LEDB;

    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
    	TickType_t duty = get_diff();

    	if( duty == 0  )
		{
			vTaskDelay ( LED_RATE );
		}
    	else{
			gpioWrite( LEDB , ON );
			vTaskDelay( duty );
			gpioWrite( LEDB , OFF );

			if (duty >= MAX_RATE)
			   	duty = MAX_RATE;

			vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
		}
    }
}

/*==================[fin del archivo]========================================*/
