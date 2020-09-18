
#include "led.h"
#include "sapi.h"

void ledsOn(){
	gpioWrite(LED1, ON );
	gpioWrite(LED2, ON );
	gpioWrite(LED3, ON );
}

void ledsOff(){
	gpioWrite(LED1, OFF);
	gpioWrite(LED2, OFF);
	gpioWrite(LED3, OFF);
}


void ledRojoOn(){
	gpioWrite(LED2, ON );
}
void ledRojoOff(){
	gpioWrite(LED2, OFF );
}
void ledVerdeOn(){
	gpioWrite(LED3, ON );
}
void ledVerdeOff(){
	gpioWrite(LED3, OFF );
}
void ledAmarilloOn(){
	gpioWrite(LED1, ON );
}
void ledAmarilloOff(){
	gpioWrite(LED1, OFF );
}
void ToggleAmarillo(){
	gpioToggle( LED1 );
}
