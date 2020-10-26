/*
 * auxs.c
 *
 *  Created on: 22 oct. 2020
 *      Author: root
 */
#include "auxs.h"

void config_i2s(){
    // Configuración I2S1_TX_WS (ENET_RXD1)
    Chip_SCU_PinMux(
        0,
        0,
		SCU_MODE_INACT | SCU_MODE_ZIF_DIS,
        FUNC7
    );
    // Configuración I2S1_TX_SDA (ENET_TX_EN)
    Chip_SCU_PinMux(
        0,
        1,
		SCU_MODE_INACT | SCU_MODE_ZIF_DIS,
        FUNC7
    );
    // Configuración I2S1_TX_SCK (ENET_REF_CLK)
    Chip_SCU_PinMux(
        1,
        19,
		SCU_MODE_INACT | SCU_MODE_ZIF_DIS,
        FUNC7
    );

	int status=0;
	I2S_AUDIO_FORMAT_T audio_Confg;

	audio_Confg.SampleRate = SOUND_MIXER_SAMPLE_RATE;
	/* Select audio data is 2 channels (1 is mono, 2 is stereo) */
	audio_Confg.ChannelNumber = SOUND_MIXER_CHANNELS;
	/* Select audio data is 16 bits */
	audio_Confg.WordWidth = SOUND_MIXER_BITS;

	printf("Init I2S\r\n");
	Chip_I2S_Init(SOUND_I2S_PORT);

	status=Chip_I2S_TxConfig(SOUND_I2S_PORT, &audio_Confg);
	if(status==SUCCESS){
		Chip_I2S_TxStop(SOUND_I2S_PORT);
		Chip_I2S_DisableMute(SOUND_I2S_PORT);
		Chip_I2S_TxStart(SOUND_I2S_PORT);
	}else{
		printf("Error init_i2s: Chip_I2S_TxConfig status %d\n",status);
	}



}

// Funcion que crea y valida las tareas de FreeRTOS
void tarea_crear(TaskFunction_t tarea,const char * const nombre,uint8_t stack,void * const parametros,uint8_t prioridad,TaskHandle_t * const puntero)
{
	// Crear tarea en freeRTOS
	BaseType_t res = xTaskCreate(tarea,nombre,configMINIMAL_STACK_SIZE*stack,parametros,tskIDLE_PRIORITY+prioridad, puntero);                         		// Puntero a la tarea creada en el sistema

	// Gestion de errores
	if(res == pdFAIL)
	{
		gpioWrite( LED_ERROR , ON );
		while(TRUE);
	}
}


