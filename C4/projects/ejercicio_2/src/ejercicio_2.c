/*=============================================================================
 * Author: Florencia Battocchia <florencia.battocchia@gmail.com>
 * Date: 2020/07/12
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "ejercicio_2.h"
#include "sapi.h"
#include "semaforo.h"
/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   boardInit();
   bool_t semState = FALSE;
   semInit();
   // ----- Repeat for ever -------------------------
   while( true ) {
	   if(!gpioRead(TEC1)){
		   semState = !semState;
	   	   semClearFlag();
	   }

	   if(semState){
		   semOnLine();
	   }else{
		   semOffLine();
	   }
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
