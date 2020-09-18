/*
 * fsmDebounce.h
 *
 *  Created on: 12 ago. 2020
 *      Author: root
 */

#ifndef PROJECTS_DIGITALAUDIOPLAYER_INC_FSMDEBOUNCE_H_
#define PROJECTS_DIGITALAUDIOPLAYER_INC_FSMDEBOUNCE_H_
#include "sapi.h"



void fsmButtonInit( void );
bool_t fsmButtonUpdate( gpioMap_t tecla );

#endif /* PROJECTS_DIGITALAUDIOPLAYER_INC_FSMDEBOUNCE_H_ */
