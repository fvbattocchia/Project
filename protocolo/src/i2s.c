
#include "board.h"
#include "wave.h"
#include <string.h>

#define SOUND_I2S_PORT LPC_I2S1
#define SOUND_MIXER_SAMPLE_RATE 44100
#define SOUND_MIXER_BITS 16
#define SOUND_MIXER_CHANNELS 2


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define CHANEL 7



//BUFFER PARA EL ARCHIVO WAV
#define BUFFER_SIZE 256
uint32_t buffer[BUFFER_SIZE];
static uint8_t dmaChannelNum;

static uint8_t channelTC;
//BUFFER DE PRUEBA PARA DMA E INTERRUPCION
#define BUF_SIZE 8
uint32_t buf[BUF_SIZE];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/
void DMA_IRQHandler(void)
{
		NVIC_DisableIRQ(DMA_IRQn);
		printf("entro a DMA_IRQHandler\n");

		if (Chip_GPDMA_Interrupt(LPC_GPDMA, CHANEL) == SUCCESS) {
			channelTC++;
			printf("DMA_IRQHandler Success \n");
		}
		else {
			printf("DMA_IRQHandler Error\n");
			/* Process error here */
		}


}
/**
 * @brief	I2S0 interrupt handler sub-routine
 * @return	Nothing

 */

void I2S0_I2S1_QEI_IRQHandler(void)
{
	NVIC_ClearPendingIRQ( I2S0_IRQn );
	//printf("I2S0_I2S1_IRQHandler\n");


	//uint8_t  read_index=0;
	channelTC++;

	/*
	memset(buf,1,8);
	while ((Chip_I2S_GetTxLevel(SOUND_I2S_PORT) < 8)) {
		Chip_I2S_Send(SOUND_I2S_PORT, buf[read_index++]);
		if((read_index == BUF_SIZE)){
			break;
		}
	}
	*/
}

void I2S0_IRQHandler(void)
{
	printf("I2S0_IRQHandler\n");
	uint8_t  read_index=0;
	channelTC++;
	/*
	memset(buf,1,8);
	while ((Chip_I2S_GetTxLevel(SOUND_I2S_PORT) < 8)) {
		Chip_I2S_Send(SOUND_I2S_PORT, buf[read_index++]);
		if((read_index == BUF_SIZE)){
			break;
		}
	}
	*/
}
void I2S1_IRQHandler(void)
{
//	printf("I2S1_IRQHandler\n");
	channelTC++;
	//uint8_t  read_index=0;

	/*
	memset(buf,1,8);
	while ((Chip_I2S_GetTxLevel(SOUND_I2S_PORT) < 8)) {
		Chip_I2S_Send(SOUND_I2S_PORT, buf[read_index++]);
		if((read_index == BUF_SIZE)){
			break;
		}
	}
	*/
}
/* Interrupt routine for I2S example */
static void App_Interrupt_Test(void)
{
	channelTC=0;
	printf("I2S Interrupt mode\r\n");
	Chip_I2S_Int_TxCmd(SOUND_I2S_PORT, ENABLE, 4);
	NVIC_DisableIRQ(I2S0_IRQn);
	NVIC_SetPriority(I2S0_IRQn,3);

	NVIC_EnableIRQ(I2S0_IRQn);

	//NVIC_DisableIRQ(I2S1_IRQn);//I2S1_IRQn
	//NVIC_SetPriority(I2S1_IRQn,3);
	//NVIC_EnableIRQ(I2S1_IRQn);
	while(channelTC!=0){
		printf("I2S level low\r\n");
	}



}



/* DMA routine for I2S example */
static void App_DMA_Test(void)
{
	uint8_t status;


	Chip_I2S_DMA_TxCmd(SOUND_I2S_PORT, I2S_DMA_REQUEST_CHANNEL_1, ENABLE, 4);
	/* Initialize GPDMA controller */
	Chip_GPDMA_Init(LPC_GPDMA);
	/* Setting GPDMA interrupt */
	NVIC_DisableIRQ(DMA_IRQn);//DMA_IRQn
	NVIC_SetPriority(DMA_IRQn,3);



	/* Get the free channel for DMA transfer */
	//dmaChannelNum = Chip_GPDMA_GetFreeChannel(LPC_GPDMA, GPDMA_CONN_I2S1_Tx_Channel_0);//I2S1

	NVIC_EnableIRQ(DMA_IRQn);

	memset(buf,16777215,BUF_SIZE);

	channelTC=0;


	status=Chip_GPDMA_Transfer(LPC_GPDMA, CHANEL,
				   (uint32_t) &buf[0],
					GPDMA_CONN_I2S1_Tx_Channel_0,
					GPDMA_TRANSFERTYPE_M2P_CONTROLLER_PERIPHERAL,
					GPDMA_WIDTH_WORD);
	printf("App_DMA_Test status: %d\n",status);
	while(channelTC==0){}

	/* Disable interrupts, release DMA channel */
	Chip_GPDMA_Stop(LPC_GPDMA, dmaChannelNum);

	NVIC_DisableIRQ(DMA_IRQn);

}
/* Polling routine for I2S example */
static void App_Polling_Test(void)
{
	uint8_t status= 1;
	uint8_t  read_index=0;

	printf("I2S Polling mode\r\n");
	status=parse_data(buffer,BUFFER_SIZE);
	while (status) {
		//si el buffer esta a la mitad del nivel i2s
		if ((Chip_I2S_GetTxLevel(SOUND_I2S_PORT) < 4) ) {
			//mando datos hasta que se llene el buffer i2s
			while ((Chip_I2S_GetTxLevel(SOUND_I2S_PORT) < 8)) {
				Chip_I2S_Send(SOUND_I2S_PORT, buffer[read_index++]);
				if((read_index == BUFFER_SIZE)){
					break;
				}
			}
			status=parse_data(buffer,BUFFER_SIZE); //devuelve cero cdo termina el archivo wav
			read_index=0;
		}
	}
}


/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief  Main routine for I2S example
 * @return Nothing
 */
int init_i2s(void)
{
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
		//App_Polling_Test();
		App_DMA_Test();
		//App_Interrupt_Test();
	}else{
		printf("Error init_i2s: Chip_I2S_TxConfig status %d\n",status);
	}

	return 0;
}




