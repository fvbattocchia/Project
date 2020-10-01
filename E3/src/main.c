

/*==================[inclusiones]============================================*/
#include "../../E3/inc/main.h"


/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/
DEBUG_PRINT_ENABLE;
/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------

	boardConfig();

	debugPrintConfigUart( UART, BAUD_RATE );
	printf( MSG_WELCOME );
	BaseType_t res;
	// Crear tareas en freeRTOS
	res = xTaskCreate (
			   tarea_led,
			  ( const char * )"tarea_led",
			  configMINIMAL_STACK_SIZE*2,
			  0,
			  tskIDLE_PRIORITY+1,
			  0
		  );


	configASSERT( res == pdPASS );


	keys_Init();
	queue_tec_pulsada = xQueueCreate( 1 , sizeof(t_data_queu) ); //preguntar tamaño???? tiene que coincidir con el nro de teclas?


	if(queue_tec_pulsada == NULL)
	{
		gpioWrite( LED_ERROR , ON );
		printf( MSG_ERROR_QUEUE );
		while(TRUE);
	}

    vTaskStartScheduler();

    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {

    }


    return TRUE;
}
/*==================[fin del archivo]========================================*/
