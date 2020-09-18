#include "led.h"
#include "semaforo.h"
#include "sapi.h"

#define TIEMPOROJO     1000
#define TIEMPOVERDE    1000
#define TIEMPOAMARILLO 1000
#define TIEMPOOFFLINE  1000
#define TIEMPOINIT     1000
static void semaforo(void);
typedef enum {AMARILLO,ROJO,VERDE,OFFLINE,INIT} secuencia_t;

static bool_t flag;
static bool_t init;
static secuencia_t estado;
static delay_t timer;
static delay_t timerInit;
void semInit(){
	estado = INIT;
	flag = FALSE;
	init = TRUE;
	ledsOn();
	delayInit(&timerInit,TIEMPOINIT);

}

void semClearFlag(){
	flag= FALSE;
	ledsOff();
}

void semOnLine(){
	if(delayRead(&timerInit)){
		if(FALSE == flag){
			estado = ROJO;
			flag = TRUE;
		}
	}
	semaforo();
}

void semOffLine(){
	if(delayRead(&timerInit) && FALSE == init){
		if(FALSE == flag){
			estado = OFFLINE;
			flag = TRUE;
		}
	}
	semaforo();
}

void semaforo(){
	switch(estado){
	case ROJO:
		if(delayRead(&timer)){
			delayInit(&timer,TIEMPOROJO);
			ledVerdeOff();
			ledRojoOn();
			estado=AMARILLO;
		}
		break;
	case AMARILLO:
		if(delayRead(&timer)){
			delayInit(&timer,TIEMPOAMARILLO);
			ledRojoOff();
			ledAmarilloOn();
			estado = VERDE;
		}
		break;
	case VERDE:
		if(delayRead(&timer)){
			delayInit(&timer,TIEMPOVERDE);
			ledAmarilloOff();
			ledVerdeOn();
			estado = ROJO;
		}
		break;
	case OFFLINE:
		if(delayRead(&timer)){
			delayInit(&timer,TIEMPOOFFLINE);
			ToggleAmarillo();
		}
		break;
	case INIT:
		if(delayRead(&timerInit)){
			ledsOff();
			init = FALSE;
		}
		break;
	default:
		break;
	}
}




