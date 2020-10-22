/* Copyright 2020, Franco Bucafusco
 * All rights reserved.
 *
 * This file is part of sAPI Library.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*=====[Inclusions of function dependencies]=================================*/
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "sapi.h"
#include "keys.h"

/*=====[Definition & macros of public constants]==============================*/

/*=====[Definitions of extern global functions]==============================*/

// Prototipo de funcion de la tarea
void task_led1( void* taskParmPtr );
void task_led2( void* taskParmPtr );


/*=====[Definitions of public global variables]==============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
	BaseType_t res_B;
	BaseType_t res_A;

	// ---------- CONFIGURACIONES ------------------------------
	boardConfig();  // Inicializar y configurar la plataforma

	printf( "Ejercicio D4\n" );

	// Crear tareas en freeRTOS
	res_A = xTaskCreate (
			  task_led1,
			  ( const char * )"task_led1",
			  configMINIMAL_STACK_SIZE*2,
			  0,
			  tskIDLE_PRIORITY+1,
			  0
		  );

	res_B = xTaskCreate (
			  task_led2,
			  ( const char * )"task_led2",
			  configMINIMAL_STACK_SIZE*2,
			  0,
			  tskIDLE_PRIORITY+1,
			  0
		  );

	// Gestión de errores
	configASSERT( (res_A || res_B) == pdPASS );

	/* inicializo driver de teclas */
	keys_Init();

	// Iniciar scheduler
	vTaskStartScheduler();

	/* realizar un assert con "false" es equivalente al while(1) */
	configASSERT( 0 );
	return 0;
}

void task_led1( void* taskParmPtr )
{

	TickType_t xPeriodicity;  // Tarea periodica cada count tiks
	TickType_t dutty;
	TickType_t xLastWakeTime = xTaskGetTickCount();

	while( 1 )
	{

		taskENTER_CRITICAL();
		xPeriodicity = count;  // Tarea periodica cada count tiks
		taskEXIT_CRITICAL();
		dutty = xPeriodicity/2;

		gpioWrite( LED1, ON );
		vTaskDelay( dutty );
		gpioWrite( LED1, OFF );


		// Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
		vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
	}
}
void task_led2( void* taskParmPtr )
{
	TickType_t xPeriodicity = pdMS_TO_TICKS( 2000 ); // Tarea periodica cada 2000 ms
	TickType_t dutty;
	TickType_t xLastWakeTime = xTaskGetTickCount();

	while( 1 )
	{
		taskENTER_CRITICAL();
		dutty = 2*count;
		if(count>=MAX_COUNT)
			count=MAX_COUNT;
		taskEXIT_CRITICAL();


		gpioWrite( LED2, ON );
		vTaskDelay( dutty );
		gpioWrite( LED2, OFF );

		taskENTER_CRITICAL();
		count-=MIN_COUNT;
		if(count<=MIN_COUNT)
			count=MIN_COUNT;
		taskEXIT_CRITICAL();

		// Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
		vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
	}
}
/* hook que se ejecuta si al necesitar un objeto dinamico, no hay memoria disponible */
void vApplicationMallocFailedHook()
{
	printf( "Malloc Failed Hook!\n" );
	configASSERT( 0 );
}
