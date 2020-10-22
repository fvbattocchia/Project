/*
 * main.c
 *
 *  Created on: 3 oct. 2020
 *      Author: root
 */


//#include "spi_sd.h"

#include "sapi.h"
#include "wave.h"
#include "i2s.h"
//chip.h
#include "scu_18xx_43xx.h"
int main( void )
{
    // Inicializar y configurar la plataforma
    boardConfig();

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
    // Inicializar UART_USB a 115200 baudios
    uartConfig( UART_USB, 115200 );

  //  init_parse();
  //  parse_header();
    gpioWrite( LED3, ON );
    init_i2s();


    while( 1 );

    return 0;

}
