/*
 * tasks.c
 *
 *  Created on: 22 oct. 2020
 *      Author: root
 */



#include "tasks.h"
#include "task.h"

#define BUFFER_SIZE 256
#define EMPTY 1
#define FULL  0
uint32_t buffer2[BUFFER_SIZE];
uint32_t buffer1[BUFFER_SIZE];
int updatebuff1;
int updatebuff2;

extern SemaphoreHandle_t isr_signal_i2s;

void send_buffer_i2s( void* taskParmPtr )
{
	uint8_t  read_index=0;


	 while( 1 )
	{
		xSemaphoreTake(isr_signal_i2s, portMAX_DELAY );
		read_index=0;
		if(updatebuff1==FULL){
			printf("Send Buffer 1\n");
			while ((Chip_I2S_GetTxLevel(SOUND_I2S_PORT) < 8)) {
				Chip_I2S_Send(SOUND_I2S_PORT, buffer1[read_index++]);
				if((read_index == BUFFER_SIZE)){
					break;
				}
			}
			updatebuff1=EMPTY;
		}else{
			printf("Send Buffer 2\n");
			if(updatebuff2==FULL){
				while ((Chip_I2S_GetTxLevel(SOUND_I2S_PORT) < 8)) {
					Chip_I2S_Send(SOUND_I2S_PORT, buffer2[read_index++]);
					if((read_index == BUFFER_SIZE)){
						break;
					}
				}
				updatebuff2=EMPTY;
			}else{
				printf("Error Ningun buffer lleno para enviar\n");
			}
		}

	}
}
void control_satus_buffer( void* taskParmPtr )
{
	uint8_t status;
	status=parse_data(buffer1,BUFFER_SIZE);
	updatebuff1=FULL;
	updatebuff2=EMPTY;
	Chip_I2S_Int_TxCmd(SOUND_I2S_PORT, ENABLE, 4);

	NVIC_DisableIRQ(I2S1_IRQn);
	NVIC_SetPriority(I2S1_IRQn,3);
	NVIC_EnableIRQ(I2S1_IRQn);

	 while( 1 )
	{
		 if(updatebuff1==EMPTY){
			 status=parse_data(buffer1,BUFFER_SIZE);
			 updatebuff1=FULL;
			 NVIC_EnableIRQ(I2S1_IRQn);
		 }
		 if(updatebuff2==EMPTY){
			 status=parse_data(buffer2,BUFFER_SIZE);
			 updatebuff2=FULL;
			 NVIC_EnableIRQ(I2S1_IRQn);
		 }
		 vTaskDelay( pdMS_TO_TICKS( 500 ) );

	}
}

void I2S1_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	NVIC_ClearPendingIRQ( I2S1_IRQn );
	NVIC_DisableIRQ(I2S1_IRQn);
	printf("I2S1_IRQHandler\n");
	xSemaphoreGiveFromISR( isr_signal_i2s, &xHigherPriorityTaskWoken );
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
