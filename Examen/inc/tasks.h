/*
 * tasks.h
 *
 *  Created on: 22 oct. 2020
 *      Author: root
 */

#ifndef PROJECTS_EXAMEN_TASKS_H_
#define PROJECTS_EXAMEN_TASKS_H_
/*==================[inclusiones]============================================*/
#include "FreeRTOS.h"
#include "sapi.h"
#include "semphr.h"
#include "auxs.h"
#include <string.h>
#include "FreeRTOSConfig.h"
#include "keys.h"

/*==================[definiciones y macros]==================================*/
#define COUNT pdMS_TO_TICKS( 1 )
/*==================[definiciones de datos]=========================*/

/*==================[prototipos de tareas]====================*/
void task_led( void* taskParmPtr );
void wait_frame( void* pvParameters );

void user_buttonPressed( t_key_isr_signal* event_data );
void user_buttonReleased( t_key_isr_signal* event_data );

#endif /* PROJECTS_EXAMEN_TASKS_H_ */
