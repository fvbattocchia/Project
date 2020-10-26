/*
 * tasks.c
 *
 *  Created on: 22 oct. 2020
 *      Author: root
 */



#include "tasks.h"
#include "task.h"
extern t_tecla_led leds_data[];

#define MAX_COUNT pdMS_TO_TICKS(20)
#define MIN_COUNT pdMS_TO_TICKS(0)
#define COUNTT pdMS_TO_TICKS(1)
#define SEC1 0
#define SEC2 1
#define SEC3 2

extern SemaphoreHandle_t mutex;
extern TaskHandle_t task_handles[];
void user_buttonPressed( t_key_isr_signal* event_data )
{
    //nada
}

void user_buttonReleased( t_key_isr_signal* event_data )
{
	static uint8_t secuencia=0;
	static TickType_t dutty_led=pdMS_TO_TICKS(500);


	   /* asociacion tecla - led */
	    if( event_data->tecla == TEC1_INDEX )
	    {
	    	dutty_led+=COUNTT;
	    	if(dutty_led>MAX_COUNT)
	    		dutty_led=MAX_COUNT;
	    }

	    if( event_data->tecla == TEC2_INDEX )
	    {
	    	dutty_led-=COUNTT;;
	    	if(dutty_led<MIN_COUNT)
	    		dutty_led=MIN_COUNT;
	    }

	    if( event_data->tecla == TEC3_INDEX )
	    {
	    	secuencia=(secuencia++) % 3;
	    }

	    if( event_data->tecla == TEC4_INDEX )
	    {
	    	secuencia=(secuencia--) % 3;
	    }

	    switch(secuencia){
			case SEC1:
			   xQueueSend( leds_data[0].queue, &dutty_led, portMAX_DELAY );
			   xQueueSend( leds_data[1].queue, &dutty_led, portMAX_DELAY );
			   xQueueSend( leds_data[2].queue, &dutty_led, portMAX_DELAY );
			   xQueueSend( leds_data[3].queue, &dutty_led, portMAX_DELAY );
				break;
			case SEC2:
				/*
				 * Nota: queria hacer el cambio de secuencia asi
				 * pero me dice que no tengo definido vTaskSuspend
				 * */
				//vTaskSuspend( task_handles[0]);
				//vTaskSuspend( task_handles[1] );
				//vTaskSuspend( task_handles[2] );
				//vTaskSuspend( task_handles[3] );
				//vTaskResume( task_handles[0] );
				//vTaskResume( task_handles[2] );
				//vTaskResume( task_handles[1] );
				//vTaskResume( task_handles[3] );
			   xQueueSend( leds_data[0].queue, &dutty_led, portMAX_DELAY );
			   xQueueSend( leds_data[2].queue, &dutty_led, portMAX_DELAY );
			   xQueueSend( leds_data[1].queue, &dutty_led, portMAX_DELAY );
			   xQueueSend( leds_data[3].queue, &dutty_led, portMAX_DELAY );
				break;
			case SEC3:
			   xQueueSend( leds_data[0].queue, &dutty_led, portMAX_DELAY );
			   xQueueSend( leds_data[1].queue, &dutty_led, portMAX_DELAY );
			   xQueueSend( leds_data[3].queue, &dutty_led, portMAX_DELAY );
			   xQueueSend( leds_data[2].queue, &dutty_led, portMAX_DELAY );
				break;
			default:
				break;
	    }

}


void task_led( void* taskParmPtr )
{
	TickType_t evnt;
    t_tecla_led* led_data = taskParmPtr;

    led_data->queue = xQueueCreate( 2, sizeof(TickType_t));

    TickType_t dif =  pdMS_TO_TICKS( 10 );
    TickType_t xPeriodicity = pdMS_TO_TICKS( 20 ); // Tarea periodica cada 1000 ms

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while( 1 )
    {
        if( xQueueReceive( led_data->queue, &evnt, 0 ) == pdPASS  )
        {
            dif = evnt;
        }
        xSemaphoreTake( mutex , portMAX_DELAY );
        gpioWrite( led_data->led, ON );
        vTaskDelay( dif );
        gpioWrite( led_data->led, OFF );
        xSemaphoreGive( mutex );
        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
    }
}
