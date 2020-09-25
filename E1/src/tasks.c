/*=============================================================================
 * Copyright (c) 2020, Martin N. Menendez <menendezmartin81@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/03
 * Version: v1.1
 *===========================================================================*/

/*==================[inclusiones]============================================*/
#include "tasks.h"
/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[tareas]====================*/

void tarea_tecla( void* taskParmPtr )
{
	tLedTecla* config = (tLedTecla*) taskParmPtr;

	fsmButtonInit( config );

	while( TRUE )
	{
		fsmButtonUpdate( config );
		vTaskDelay( BUTTON_RATE );
	}
}

void tarea_led( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------
	tLedTecla* config = (tLedTecla*) taskParmPtr;

	TickType_t xPeriodicity =  MAX_RATE;
	TickType_t xLastWakeTime = xTaskGetTickCount();
    // ---------- REPETIR POR SIEMPRE --------------------------

	TickType_t dif = 0;

	while( TRUE )
	{
		//la diferencia con el semaforo es que el semaforo pide un permiso, o se le da permiso y luego se toma el valor
		//la cola toma el valor directamente
		//cdo haya un valor en la cola se va aponer en ready y se va  atomar cdo este en running
		//la cola sirve tb cdo se quiere pasar un dato de un tipo especifico
		xQueueReceive( config->queue_tec_pulsada , &dif,  portMAX_DELAY );			// Esperamos tecla

		if (dif >= xPeriodicity)
			dif = xPeriodicity;

		gpioWrite( config->led , ON );
		vTaskDelay( dif );
		gpioWrite( config->led , OFF );
	}
}
