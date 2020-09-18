/*=============================================================================
 * Author: Florencia Battocchia <florencia.battocchia@gmail.com>
 * Date: 2020/08/29
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "freertos_a1.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "sapi.h"
#include "userTasks.h"
#include "config.h"

print_t debugPrint; //??

TaskHandle_t task_handles[3];

int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------
    PRINTF_CONFIGURE; //??
    PRINTF( EXAMPLE_WELCOME_TEXT );

    // Crear tarea en freeRTOS
    BaseType_t res1 =
        xTaskCreate(
            Tarea1_Code,                // Funcion de la tarea a ejecutar
            ( const char * )"myTask 1", // Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE*2, /* tamaño del stack de cada tarea (words) */
            NULL,                       // Parametros de tarea
            tskIDLE_PRIORITY+1,         // Prioridad de la tarea
            &task_handles[0]            // Puntero a la tarea creada en el sistema
        );

    BaseType_t res2 =
        xTaskCreate(
            Tarea2_Code,                // Funcion de la tarea a ejecutar
            ( const char * )"myTask 2", // Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE*2, /* tamaño del stack de cada tarea (words) */
            NULL,                       // Parametros de tarea
            tskIDLE_PRIORITY+1,         // Prioridad de la tarea
            &task_handles[1]            // Puntero a la tarea creada en el sistema
        );

    BaseType_t res3=
        xTaskCreate(
            Tarea_Monitor_Code,         // Funcion de la tarea a ejecutar
            ( const char * )"monitor",  // Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE*4, /* tamaño del stack de cada tarea (words) */
            NULL,                       // Parametros de tarea
            tskIDLE_PRIORITY+1,         // Prioridad de la tarea
            &task_handles[2]            // Puntero a la tarea creada en el sistema
        );



    configASSERT( res1 == pdPASS && res2 == pdPASS );


    // Iniciar scheduler
    vTaskStartScheduler();

    uxTaskGetStackHighWaterMark( task_handles[0] ) ;

    // ---------- REPETIR POR SIEMPRE --------------------------
    while( 1 )
    {
        // Si cae en este while 1 significa que no pudo iniciar el scheduler
    }


    return 0;
}
