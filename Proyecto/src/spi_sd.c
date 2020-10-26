/*
 * spi.c
 *
 *  Created on: 3 oct. 2020
 *      Author: root
 */


#include "sapi_sdcard.h"
#include "sapi.h"
#include "ff.h"
#include "spi_sd.h"
#include <string.h>
// Wrappers para manejar el dispositivo  tarjeta SD
sdcard_t sdcard;

static FIL file;

// Se guarda el ultimo estado para enviar un mensaje solo cuando el estado
// cambie.
static sdcardStatus_t sdcardUltimoEstado = -1;


void diskTickHook ( void *ptr );


static void mostrarEstadoTarjetaSD( void )
{
    // El estado actual es distinto al ultimo mostrado?
    if ( sdcardUltimoEstado == sdcardStatus() )
    {
        // Es igual, no hay nada que hacer aca
        return;
    }

    // Es diferente, guardo y envio estado
    sdcardUltimoEstado = sdcardStatus();

    switch( sdcardUltimoEstado )
    {
        case SDCARD_Status_Removed:
            uartWriteString( UART_USB, "STATUS: Por favor inserte Tarjeta SD.\r\n" );
            break;

        case SDCARD_Status_Inserted:
            uartWriteString( UART_USB, "STATUS: Tarjeta SD insertada.\r\n" );
            break;

        case SDCARD_Status_NativeMode:
            uartWriteString( UART_USB, "STATUS: Configurando tarjeta SD.\r\n" );
            break;

        case SDCARD_Status_Initializing:
            uartWriteString( UART_USB, "STATUS: Iniciando tarjeta SD.\r\n" );
            break;

        case SDCARD_Status_ReadyUnmounted:
            uartWriteString( UART_USB, "STATUS: Tarjeta SD lista pero desmontada.\r\n" );
            break;

        case SDCARD_Status_ReadyMounted:
            uartWriteString( UART_USB, "STATUS: Tarjeta SD lista y montada.\r\n" );
            break;

        case SDCARD_Status_Error:
            uartWriteString( UART_USB, "STATUS: Tarjeta SD en estado de Error.\r\n" );
            break;
    }
}


static void fatFsTestStart( const char* test )
{
    char msg[256];
    sprintf( msg, "Ejecutando '%s'...", test );
    uartWriteString( UART_USB, msg );
}


static void fatFsTestOK( void )
{
    uartWriteString( UART_USB, "OK!\r\n" );
}


static void fatFsTestERROR( int error )
{
    char msg[256];
    sprintf( msg, "ERROR %i.\r\n", error );
    uartWriteString( UART_USB, msg );
}


uint32_t  read_sd_spi( void *buf , uint32_t size)
{
	char msg[256];
    FRESULT fr;
    // Carga el contenido del archivo
    UINT bytesLeidos = 0;
    memset(buf,0 ,sizeof(buf));

    fr = f_read( &file, buf,size, &bytesLeidos );

    if (fr != FR_OK)
    {
       fatFsTestERROR( fr );
       gpioWrite( LEDG, ON );
       while(1);
    }


    return bytesLeidos;
}
void close_sd_spi(){
	 f_close( &file );
}
void open_sd_spi(){
    char filename[64];
    FRESULT fr;

    sprintf( filename, "%s/a2002011001-e02.wav", sdcardDriveName() );



    fatFsTestStart( "f_open( READ )" );
    fr = f_open( &file, filename, FA_READ );
    if( fr != FR_OK )
    {
       fatFsTestERROR( fr );
       gpioWrite( LEDR, ON );
       while(1);

    }
    fatFsTestOK( );

}

void init_sd_spi( void )
{
    char msg[256];
    // ---------- CONFIGURACIONES ------------------------------

    // SPI configuration
    spiConfig( SPI0 );

    // Inicializar el conteo de Ticks con resolucion de 10ms,
    // con tickHook diskTickHook
    tickConfig( 10 );
    tickCallbackSet( diskTickHook, NULL );

    printf("start\n");

    uartWriteString( UART_USB, "Iniciando sdcard con configuracion:\r\n" );
    sprintf( msg, "  velocidad inicial %i Hz.\r\n", FSSDC_GetSlowClock() );
    uartWriteString( UART_USB, msg );
    sprintf( msg, "  velocidad de trabajo %i Hz.\r\n", FSSDC_GetFastClock() );
    uartWriteString( UART_USB, msg );

    if (sdcardInit( &sdcard ) == false)
    {
        uartWriteString( UART_USB, "Inicio de sdcard FALLO.\r\n**FIN**\r\n" );
        gpioWrite( LEDR, ON );
        while( 1 );
    }
    else {
        uartWriteString( UART_USB, "Inicio de sdcard OK! Unidad FatFs '" );
        uartWriteString( UART_USB, sdcardDriveName());
        uartWriteString( UART_USB, "'.\r\n" );
    }

    // Se paso la primer etapa con exito, enciende LED1
    gpioWrite( LED1, ON );

    /*
        En el siguiente bucle se consulta el estado del dispositivo,
        pudiendo presenciar como transcurre su estado desde que se conecta
        hasta que se inicia y auto-montan.

        una vez iniciados y montados, podremos realizar un test estandar de
        lectura y escritura de archivos con la libreria FatFs.
    */
    uartWriteString( UART_USB, "Logueando STATUS de dipositivos...\r\n" );
    while( 1 )
    {
        // Muestra el estado actual de los dispositivos
        mostrarEstadoTarjetaSD();

        // Si el dispositivo ya esta iniciado y montado, salimos de este
        // bucle y comenzamos los testeos sobre archivos utilizando FatFs
        if( sdcardUltimoEstado == SDCARD_Status_ReadyMounted )
        {
            break;
        }
    }

    // Se paso la segunda etapa con exito, enciende LED2
    gpioWrite( LED2, ON );

}


// FUNCION que se ejecuta cada vez que ocurre un Tick
void diskTickHook( void *ptr )
{
    disk_timerproc();
}



