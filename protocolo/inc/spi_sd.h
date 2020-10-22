/*
 * spi.h
 *
 *  Created on: 3 oct. 2020
 *      Author: root
 */

#ifndef PROJECTS_PROTOCOLO_SPI_SD_H_
#define PROJECTS_PROTOCOLO_SPI_SD_H_



void init_sd_spi();
void open_sd_spi();
void close_sd_spi();
uint32_t read_sd_spi(void *buf,uint32_t size);
#endif /* PROJECTS_PROTOCOLO_SPI_SD_H_ */
