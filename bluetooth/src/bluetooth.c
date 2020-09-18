/*=============================================================================
 * Author: Florencia Battocchia <florencia.battocchia@gmail.com>
 * Date: 2020/09/04
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "bluetooth.h"
#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
	uint8_t dataBluetooth;
   // ----- Setup -----------------------------------
	boardConfig();
   //consigura la uart 232 a 9600 baudios
   uartConfig(UART_232,9600);
   uartWriteString(UART_232,"Hola mundo APP Inventor!\n\r");

   // ----- Repeat for ever -------------------------
   while( true ) {
	   if(uartReadByte(UART_232,&dataBluetooth)){
		   if(dataBluetooth == 'h'){
			   gpioWrite(LEDB,HIGH);
			   uartWriteString(UART_232,"LEDB Encendido!\n\r");
		   }else  if(dataBluetooth == 'l'){
			   gpioWrite(LEDB,LOW);
			   uartWriteString(UART_232,"LEDB Apagado!\n\r");
		   }
	   }
	   if(!gpioRead(TEC1)){
		   uartWriteString(UART_232,"LED ON\n\r");
	   }
	   if(!gpioRead(TEC2)){
		   uartWriteString(UART_232,"LED OFF\n\r");
	   }



      delay(1000);
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
