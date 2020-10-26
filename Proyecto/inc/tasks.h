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
#include <string.h>
#include "FreeRTOSConfig.h"

#include "wave.h"
/*==================[definiciones y macros]==================================*/
#define SOUND_I2S_PORT LPC_I2S1
#define SOUND_MIXER_SAMPLE_RATE 44100
#define SOUND_MIXER_BITS 16
#define SOUND_MIXER_CHANNELS 2
/*==================[definiciones de datos]=========================*/

/*==================[prototipos de tareas]====================*/
void send_buffer_i2s( void* taskParmPtr );
void control_satus_buffer( void* taskParmPtr );


#endif /* PROJECTS_EXAMEN_TASKS_H_ */
