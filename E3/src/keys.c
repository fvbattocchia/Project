
/*==================[ Inclusions ]============================================*/
#include "../../E3/inc/keys.h"


#include "task.h"


/*=====[ Definitions of private data types ]===================================*/


/*=====[Definition macros of private constants]==============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

static void keys_ButtonError( uint32_t index );
static void buttonPressed( uint32_t index );
static void buttonReleased( uint32_t index );

/*=====[Definitions of private global variables]=============================*/

/*=====[Definitions of public global variables]==============================*/

gpioMap_t teclas[] = {TEC1,TEC2,TEC3,TEC4};
gpioMap_t leds[]   = {LEDB,LED1,LED2,LED3};

#define key_count   sizeof(teclas)/sizeof(gpioMap_t)
t_key_data keys_data[key_count];

/*=====[prototype of private functions]=================================*/
void task_tecla( void* taskParmPtr );

/*=====[Implementations of public functions]=================================*/
TickType_t get_diff(uint32_t index)
{
	TickType_t tiempo;

	tiempo = keys_data[index].time_diff;

	return tiempo;
}

void clear_diff(uint32_t index)
{

	keys_data[index].time_diff = 0;

}

void keys_Init( void )
{
	BaseType_t res;
	for(int i =0; i<key_count;i++){
		keys_data[i].state          = BUTTON_UP;  // Set initial state
		keys_data[i].time_down      = KEYS_INVALID_TIME;
		keys_data[i].time_up        = KEYS_INVALID_TIME;
		keys_data[i].time_diff      = KEYS_INVALID_TIME;
	}


	res = xTaskCreate (
			  task_tecla,
			  ( const char * )"task_tecla",
			  configMINIMAL_STACK_SIZE*2,
			  0,
			  tskIDLE_PRIORITY+1,
			  0
		  );


	configASSERT( res == pdPASS );
}


void keys_Update( uint32_t index )
{
	switch( keys_data[index].state )
	{
		case STATE_BUTTON_UP:
			/* CHECK TRANSITION CONDITIONS */
			if( !gpioRead( teclas[index] ) )
			{
				keys_data[index].state = STATE_BUTTON_FALLING;
			}
			break;

		case STATE_BUTTON_FALLING:
			/* ENTRY */

			/* CHECK TRANSITION CONDITIONS */
			if( !gpioRead( teclas[index] ) )
			{
				keys_data[index].state = STATE_BUTTON_DOWN;

				/* ACCION DEL EVENTO !*/
				buttonPressed( index );
			}
			else
			{
				keys_data[index].state = STATE_BUTTON_UP;
			}

			/* LEAVE */
			break;

		case STATE_BUTTON_DOWN:
			/* CHECK TRANSITION CONDITIONS */
			if( gpioRead( teclas[index]) )
			{
				keys_data[index].state = STATE_BUTTON_RISING;
			}
			break;

		case STATE_BUTTON_RISING:
			/* ENTRY */

			/* CHECK TRANSITION CONDITIONS */

			if( gpioRead( teclas[index] ) )
			{
				keys_data[index].state = STATE_BUTTON_UP;

				/* ACCION DEL EVENTO ! */
				buttonReleased( index );
			}
			else
			{
				keys_data[index].state = STATE_BUTTON_DOWN;
			}

			/* LEAVE */
			break;

		default:
			keys_ButtonError( index );
			break;
	}
}

/*=====[Implementations of private functions]================================*/

/* accion de el evento de tecla pulsada */
static void buttonPressed( uint32_t index )
{
	TickType_t current_tick_count = xTaskGetTickCount();

	keys_data[index].time_down = current_tick_count;

}

/* accion de el evento de tecla liberada */
static void buttonReleased( uint32_t index )
{
	TickType_t current_tick_count = xTaskGetTickCount();
	t_data_queu mjs_queu;

	keys_data[index].time_up    = current_tick_count;
	keys_data[index].time_diff  = keys_data[index].time_up - keys_data[index].time_down;

	mjs_queu.time_diff=keys_data[index].time_diff;
	mjs_queu.led=leds[index];
	xQueueSend( queue_tec_pulsada , &(mjs_queu),  portMAX_DELAY  );

}

static void keys_ButtonError( uint32_t index )
{

	keys_data[index].state = BUTTON_UP;

}

/*=====[Implementations of private functions]=================================*/
void task_tecla( void* taskParmPtr )
{

	while( 1 )
	{
		for(int i =0; i<key_count;i++){
			keys_Update( i );
			vTaskDelay( DEBOUNCE_TIME / portTICK_RATE_MS );
		}
	}
}
