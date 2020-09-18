/*=====[Inclusions of function dependencies]=================================*/

#include "led.h"
#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

 void setLedOff(){
	  gpioWrite(LED1, OFF );
	  gpioWrite(LED2, OFF );
	  gpioWrite(LED3, OFF );
 }
 void setLedPlay(){
	  gpioWrite(LED2, OFF );
	  gpioWrite(LED3, OFF );
	  gpioWrite(LED1, ON );
 }

 void setLedPichBendInc(){
	  gpioWrite(LED1, OFF );
	  gpioWrite(LED3, OFF );
	  gpioWrite(LED2, ON );
 }

 void setLedPichBendDec(){
	  gpioWrite(LED1, OFF );
	  gpioWrite(LED3, ON );
	  gpioWrite(LED2, OFF);
 }
