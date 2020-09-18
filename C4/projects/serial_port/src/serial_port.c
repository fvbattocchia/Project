/*=============================================================================
 * Author: Florencia Battocchia <florencia.battocchia@gmail.com>
 * Date: 2020/07/16
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "serial_port.h"
#include "sapi.h"

/*=====[Definitions of public functions]=====================================*/

int miFuncion( void )
{
    static int acumulador = 0;
    return acumulador++;
}

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
    boardInit();

    while(true){
        if( !gpioRead(TEC1) ){
            printf( "acumulador = %d\r\n",miFuncion());
        }
    }
   return 0;
}
