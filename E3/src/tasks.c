/*=============================================================================
 * Copyright (c) 2020, Martin N. Menendez <menendezmartin81@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/03
 * Version: v1.1
 *===========================================================================*/

/*==================[inclusiones]============================================*/

#include "../../E3/inc/tasks.h"



/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[tareas]====================*/


void tarea_led( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------

	TickType_t xPeriodicity =  MAX_RATE;
	TickType_t xLastWakeTime = xTaskGetTickCount();
    // ---------- REPETIR POR SIEMPRE --------------------------

	t_data_queu mjs;
	mjs.time_diff=0;
	mjs.led=LED1;
	while( TRUE )
	{
		//si hay dato nuevo actualiza si no sigue con la periocidad como esta
		//lectuta no bloqueante
		xQueueReceive( queue_tec_pulsada , &mjs, 0);

		if (mjs.time_diff > xPeriodicity)
			mjs.time_diff = xPeriodicity;

		gpioWrite( mjs.led  , ON );
		vTaskDelay( mjs.time_diff );
		gpioWrite( mjs.led , OFF );

		vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
	}
}
