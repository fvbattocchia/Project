/*=============================================================================
 * Author: Florencia Battocchia <florencia.battocchia@gmail.com>
 * Date: 2020/08/29
 *===========================================================================*/


#include "userTasks.h"

// Includes de FreeRTOS
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "config.h"
#include "task.h"
#include <stdio.h>


#include "sapi.h"

#include <string.h>

extern TaskHandle_t task_handles[2];

// Task implementation
void delay_con_for( uint32_t ms )
{
    volatile uint32_t i;
    volatile uint32_t j;

    for( i=0 ; i<ms ; i++ )
    {
        /* delay de aprox 1 ms */
        for( j=0 ; j<CUENTAS_1MS ; j++ )
        {
        }
    }
}

/*==================[definiciones de funciones externas]=====================*/

void Tarea1_Code( void* taskParmPtr )
{
    PRINTF( "Tarea 1\r\n" );

    while( 1 )
    {
        delay_con_for( 200 );
        //PRINTF( "Blink %u at %u ms!\r\n", 1 , xTaskGetTickCount() );
        gpioToggle( LEDR );
    }
}

void Tarea2_Code( void* taskParmPtr )
{
    PRINTF( "Tarea 2\r\n" );

#if HACER_FALLAR==1
    uint32_t local_array[140];
#endif

    while( 1 )
    {
        delay_con_for( 500 );

        //PRINTF( "Blink %u at %u ms!\r\n", 2 , xTaskGetTickCount() );
        gpioToggle( LEDB );

#if HACER_FALLAR==1
        memset( local_array,0,20 );
#endif
    }
}

void Tarea_Monitor_Code( void* taskParmPtr )
{
    PRINTF( "Tarea 3\r\n" );

    while( 1 )
    {
        delay_con_for( 1000 );
      //  uxTaskGetStackHighWaterMark( task_handles[0] ) ;
        /*
        PRINTF( "Task %u min %u words\r\n", 1,  uxTaskGetStackHighWaterMark( task_handles[0] ) );
        PRINTF( " Task %u min %u words\r\n", 2,  uxTaskGetStackHighWaterMark( task_handles[1] ) );
        PRINTF( " Task %u min %u words\r\n", 3,  uxTaskGetStackHighWaterMark( task_handles[2] ) );
        */
    }
}


/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

