/*
 * auxs.h
 *
 *  Created on: 22 oct. 2020
 *      Author: root
 */

#ifndef PROJECTS_EXAMEN_AUXS_H_
#define PROJECTS_EXAMEN_AUXS_H_

/*==================[inclusiones]============================================*/
#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"
#include "semphr.h"

#include <string.h>
#include "FreeRTOSConfig.h"
#include "tasks.h"

#define LED_ERROR 		LEDR

#define PRIORITY 1
#define SIZE     1
#define STR_AUX  20


typedef struct
{
    gpioMap_t led;
    xQueueHandle queue; //almacenara el evento en una cola
} t_tecla_led;

void tareas_crear(TaskFunction_t tarea,const char * const nombre);
void tarea_crear(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,int puntero);//TaskHandle_t * const puntero



#endif /* PROJECTS_EXAMEN_AUXS_H_ */
