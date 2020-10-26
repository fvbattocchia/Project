/*
 * auxs.c
 *
 *  Created on: 22 oct. 2020
 *      Author: root
 */
#include "auxs.h"


//estructura de lesds, cola
t_tecla_led leds_data[] = { {.led= LEDB}, {.led= LED1},{.led= LED2}, {.led= LED3}};
#define led_count   sizeof(leds_data)/sizeof(leds_data[0])

SemaphoreHandle_t mutex;

TaskHandle_t task_handles[4];
// Funcion que crea la cantidad de tareas definida en N_TECLAS
void tareas_crear(TaskFunction_t tarea,const char * const nombre)
{
	mutex	=  xSemaphoreCreateMutex();

	uint16_t i;

	char nombre_aux[STR_AUX];		// "task_led_"

	for(i = 0 ; i < led_count ; i++)
	{
		strcpy(nombre_aux, nombre);
		strcat(nombre_aux, (i+1)+'0');		// 2+'0' = '2' "task_led_2"
		//          tarea    ,nombre      ,SIZE*STACK,parametro           ,idle+1,0;
		tarea_crear(tarea,nombre_aux,SIZE,&leds_data[i],PRIORITY,i);//NULL
		//cada tarea de led va a tener una cola y un led
	}
}


// Funcion que crea y valida las tareas de FreeRTOS
//void tarea_crear(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero)
//{
void tarea_crear(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,int puntero)
{
	// Crear tarea en freeRTOS
	BaseType_t res = xTaskCreate(tarea,nombre,configMINIMAL_STACK_SIZE*stack,parametros,tskIDLE_PRIORITY+prioridad, &task_handles[puntero]);                         		// Puntero a la tarea creada en el sistema

	// Gestion de errores
	if(res == pdFAIL)
	{
		gpioWrite( LED_ERROR , ON );
		while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
	}
}


