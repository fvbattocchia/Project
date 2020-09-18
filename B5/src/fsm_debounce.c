/*=============================================================================
 * Copyright (c) 2020, Martin N. Menendez <menendezmartin81@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/03
 * Version: v1.1
 *===========================================================================*/

#include "sapi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
typedef enum
{
    STATE_BUTTON_UP,
    STATE_BUTTON_DOWN,
    STATE_BUTTON_FALLING,
    STATE_BUTTON_RISING
} fsmButtonState_t;

void fsmButtonError( void );
void fsmButtonInit( void );
void fsmButtonUpdate( gpioMap_t tecla );
void buttonPressed( void );
void buttonReleased( void );

/* prototipo de la tarea led   */
void tarea_led( void* taskParmPtr );

fsmButtonState_t fsmButtonState;

TickType_t tiempo_down;
TickType_t tiempo_up;
TickType_t tiempo_diff;

TickType_t get_diff()
{
	return tiempo_diff;
}

void clear_diff()
{
	tiempo_diff = 0;
}


/* accion de el evento de tecla pulsada */
void buttonPressed( void )
{
	tiempo_down = xTaskGetTickCount();
}

/* accion de el evento de tecla liberada */
void buttonReleased( void )
{
		tiempo_up = xTaskGetTickCount();
		tiempo_diff = tiempo_up - tiempo_down;
		//se podria chacer echo como el ej 4 tener 1 funcion que crea las tareas y llamar a esa funcion
	 // Crear tarea en freeRTOS
		BaseType_t res_A =
		xTaskCreate(
			tarea_led,                     // Funcion de la tarea a ejecutar
			( const char * )"tarea_led",   // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
			0,                          // Parametros de tarea
			tskIDLE_PRIORITY+1,         // Prioridad de la tarea
			0                           // Puntero a la tarea creada en el sistema
		);

	    // Gestion de errores
		if(res_A  == pdFAIL)
		{
			gpioWrite( LEDR, ON );
			printf( "Error al crear las tareas.\r\n" );
			while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
		}
}

void fsmButtonError( void )
{
    fsmButtonState = BUTTON_UP;
}

void fsmButtonInit( void )
{
    fsmButtonState = BUTTON_UP;  // Set initial state
}

#define DEBOUNCE_TIME 40

// FSM Update Sate Function
void fsmButtonUpdate( gpioMap_t tecla )
{

    static uint8_t contFalling = 0;
    static uint8_t contRising = 0;

    switch( fsmButtonState )
    {
        case STATE_BUTTON_UP:
            /* CHECK TRANSITION CONDITIONS */
            if( !gpioRead( tecla ) )
            {
                fsmButtonState = STATE_BUTTON_FALLING;
            }
            break;

        case STATE_BUTTON_FALLING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */
            if( contFalling >= DEBOUNCE_TIME )
            {
                if( !gpioRead( tecla ) )
                {
                    fsmButtonState = STATE_BUTTON_DOWN;

                    /* ACCION DEL EVENTO !*/
                    buttonPressed();
                }
                else
                {
                    fsmButtonState = STATE_BUTTON_UP;
                }

                contFalling = 0;
            }

            contFalling++;

            /* LEAVE */
            break;

        case STATE_BUTTON_DOWN:
			/* CHECK TRANSITION CONDITIONS */
			if( gpioRead( tecla ) )
			{
				fsmButtonState = STATE_BUTTON_RISING;
			}
			break;

        case STATE_BUTTON_RISING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */

            if( contRising >= DEBOUNCE_TIME )
            {
                if( gpioRead( tecla ) )
                {
                    fsmButtonState = STATE_BUTTON_UP;

                    /* ACCION DEL EVENTO ! */
                    buttonReleased();
                }
                else
                {
                    fsmButtonState = STATE_BUTTON_DOWN;
                }
                contRising = 0;
            }
            contRising++;

            /* LEAVE */
            break;

        default:
            fsmButtonError();
            break;
    }
}
