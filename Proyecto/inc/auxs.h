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

void config_i2s();
void tarea_crear(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero);



#endif /* PROJECTS_EXAMEN_AUXS_H_ */
