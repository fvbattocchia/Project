#include "sapi.h"
#include "i2s_18xx_43xx.h"
#define SOUND_I2S_PORT LPC_I2S1

int main( void )
{
   Chip_I2S_TxStart(LPC_I2S1);
   boardInit();
   tick_t count1   = 0,count2   = 0,count3   = 0;
   delay_t timer1,timer2,timer3;
   bool_t ledOn1=FALSE,ledOn2=FALSE,ledOn3=FALSE;

   while(TRUE){
	   if(!gpioRead(TEC1)){
			count1++;
	   }
	   if(!gpioRead(TEC2)){
			count2++;
	   }
	   if(!gpioRead(TEC3)){
			count3++;
	   }
	   if(gpioRead(TEC1) && count1!=0 && ledOn1==FALSE){
		   delayInit(&timer1,count1);
		   gpioWrite(LED1, ON );
		   ledOn1=TRUE;
	   }
	   if(gpioRead(TEC2) && count2!=0 && ledOn2==FALSE){
		   delayInit(&timer2,count2);
		   gpioWrite(LED2, ON );
		   ledOn2=TRUE;
	   }
	   if(gpioRead(TEC3) && count3!=0 && ledOn3==FALSE){
		   delayInit(&timer3,count3);
		   gpioWrite(LED3, ON );
		   ledOn3=TRUE;
	   }
	   if(delayRead(&timer1)){
		   gpioWrite(LED1, OFF );
		   count1=0;
		   ledOn1=FALSE;
	   }
	   if(delayRead(&timer2)){
		   gpioWrite(LED2, OFF );
		   count2=0;
		   ledOn2=FALSE;
	   }
	   if(delayRead(&timer3)){
		   gpioWrite(LED3, OFF );
		   count3=0;
		   ledOn3=FALSE;
	   }
	   delay(1);
   }
}
