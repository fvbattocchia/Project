/*=====[Inclusions of function dependencies]=================================*/

#include "fsmConsole.h"
#include "fsmDebounce.h"
#include "led.h"
#include <stdint.h>
#include <stddef.h>

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/
// FSM state names
typedef enum{
   PAUSE,
   PLAY,
   PICH_BEND_INC,
   PICH_BEND_DEC,
   TEMPO_INC,
   TEMPO_DEC,
} fsmConsoleState_t;

static void fsmConsoleError( void );

static fsmConsoleState_t fsmConsoleState;

/*=====[Main function, program entry point after power on or reset]==========*/

void fsmConsoleError( void )
{
    fsmConsoleState = PAUSE;
}

void fsmConsoleInit( void )
{
   uartConfig(UART_USB,115200);
   adcInit(ADC_ENABLE);
   fsmConsoleState = PAUSE;
}

void fsmConsoleUpdate( bool_t statusTec1 )
{
   static bool_t flag = TRUE;

   switch( fsmConsoleState ){
      case PAUSE:
    	  if(flag){
    		  uartWriteString(UART_USB,"estado: PAUSE\n\r");
    		  flag=FALSE;
    	  }

    	  setLedOff();

    	  if(statusTec1 ){
    		  fsmConsoleState = PLAY;
    		  flag=TRUE;
    	  }
      break;
      case PLAY:
    	  if(flag){
    		  uartWriteString(UART_USB,"estado: PLAY\n\r");
    		  flag=FALSE;
    	  }

    	  setLedPlay();

    	  if(statusTec1 ){
    		  fsmConsoleState = PAUSE;
    		  flag=TRUE;
    	  }else if(!gpioRead(TEC2)){
    		  fsmConsoleState = PICH_BEND_INC;
    		  flag=TRUE;
    	  }else if(!gpioRead(TEC3)){
    		  fsmConsoleState = PICH_BEND_DEC;
    		  flag=TRUE;
    	  }else if(adcRead(CH1)> 560 ){
			  fsmConsoleState = TEMPO_INC;
			  flag=TRUE;
    	  }else if(adcRead(CH1)< 460){
 			 fsmConsoleState = TEMPO_DEC;
 			 flag=TRUE;
     	  }
      break;
      case PICH_BEND_INC:

    	  if(flag){
    		  uartWriteString(UART_USB,"estado: PICH_BEND_INC\n\r");
		  	  flag=FALSE;
    	  }

    	  setLedPichBendInc();

    	  if(gpioRead(TEC2)){
    		  fsmConsoleState = PLAY;
    		  flag=TRUE;
    	  }
      break;
      case PICH_BEND_DEC:

    	  if(flag){
    		  uartWriteString(UART_USB,"estado: PICH_BEND_DEC\n\r");
		  	  flag=FALSE;
    	  }
    	  setLedPichBendDec();

    	  if(gpioRead(TEC3)){
    		  fsmConsoleState = PLAY;
    		  flag=TRUE;
    	  }
      break;
      case TEMPO_INC:

    	  if(flag){
    		  uartWriteString(UART_USB,"estado: TEMPO_INC\n\r");
		  	  flag=FALSE;
    	  }
    	  setLedOff();
    	  printf("ADC: %d\n\r",adcRead(CH1));

    	  if(adcRead(CH1)>=460 && adcRead(CH1) <=560){
    		  fsmConsoleState = PLAY;
    		  flag=TRUE;
    	  }

      break;
      case TEMPO_DEC:

    	  if(flag){
    		  uartWriteString(UART_USB,"estado: TEMPO_DEC\n\r");
		  	  flag=FALSE;
    	  }
    	  setLedOff();
    	  printf("ADC: %d\n\r",adcRead(CH1));

    	  if(adcRead(CH1)>=460 && adcRead(CH1) <=560){
    		  fsmConsoleState = PLAY;
    		  flag=TRUE;
    	  }

      break;
      default:
         fsmConsoleError();
      break;
   }
}
