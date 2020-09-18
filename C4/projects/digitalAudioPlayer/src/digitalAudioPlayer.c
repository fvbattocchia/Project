/*=============================================================================
 * Author: Florencia Battocchia <florencia.battocchia@gmail.com>
 * Date: 2020/08/10
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "digitalAudioPlayer.h"
#include "fsmConsole.h"
#include "fsmDebounce.h"


/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   // Inicializo la placa
   boardConfig();

   fsmConsoleInit( );

   delay_t actualizarMefBoton;
   delayInit( &actualizarMefBoton, 40 );
   fsmButtonInit();

   // ----- Repeat for ever -------------------------
   bool_t statusTec1;
   while(true) {

	  if( delayRead(&actualizarMefBoton) ){
		  statusTec1=fsmButtonUpdate(TEC1);
	  }else{
		  statusTec1=FALSE;
	  }
	  fsmConsoleUpdate(statusTec1);

   }

   return 0;
}
