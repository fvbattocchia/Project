/*
 * wave.c
 *
 *  Created on: 3 oct. 2020
 *      Author: root
 *
 *      Read and parse a wave file
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wave.h"
#include "sapi.h"

#include "spi_sd.h"

#define CHANEL 2
#define SIZE_SAMPLE 4

// WAVE header structure
uint8_t buffer4[4];
uint8_t buffer2[2];

static long num_samples;
static uint16_t size_of_each_sample;
static long count_num_samples;
struct HEADER header;
static bool parse_stereo_pcm_16;
uint32_t parse_header(){

	 uint32_t read = 0;
	 char msg[256];
	 // read header parts

	 read = read_sd_spi((void *)header.riff,sizeof(header.riff));

	 if(read!=sizeof(header.riff)){
		 printf("ERROR: riff: %d =! %d",read,sizeof(header.riff));
	 }
	 sprintf( msg, "(1-4): %s\r\n",  header.riff);
	 uartWriteString( UART_USB, msg);

	 read = read_sd_spi(buffer4,sizeof(buffer4));
	 if(read!=sizeof(buffer4)){
		 printf("overall_size: %d =! %d",read,sizeof(buffer4));
	 }

	 // convert little endian to big endian 4 byte int
	 header.overall_size  = buffer4[0] |
							(buffer4[1]<<8) |
							(buffer4[2]<<16) |
							(buffer4[3]<<24);
	 printf("(5-8) Overall size: bytes:%u, Kb:%u \n", header.overall_size, header.overall_size/1024);

	 read = read_sd_spi(header.wave,sizeof(header.wave));
	 if(read!=sizeof(header.wave)){
		 printf("wave: %d =! %d",read,sizeof(header.wave));
	 }
	 printf("(9-12) Wave marker: %s \n", header.wave);

	 read = read_sd_spi(header.fmt_chunk_marker,sizeof(header.fmt_chunk_marker));
	 if(read!=sizeof(header.fmt_chunk_marker)){
		 printf("fmt_chunk_marker: %d =! %d",read,sizeof(header.fmt_chunk_marker));
	 }
	 printf("(13-16) Fmt marker: %s \n", header.fmt_chunk_marker);

	 read = read_sd_spi(buffer4,sizeof(buffer4));
	 if(read!=sizeof(buffer4)){
		 printf("length_of_fmt: %d =! %d",read,sizeof(buffer4));
	 }
	 // convert little endian to big endian 4 byte integer
	 header.length_of_fmt = buffer4[0] |
						   (buffer4[1] << 8) |
						   (buffer4[2] << 16) |
						   (buffer4[3] << 24);
	 printf("(17-20) Length of Fmt header: %u  \n", header.length_of_fmt);

	 read = read_sd_spi(buffer2,sizeof(buffer2));
	 if(read!=sizeof(buffer2)){
		 printf("format_type: %d =! %d \r\n",read,sizeof(buffer2));
	 }

	 header.format_type = buffer2[0] | (buffer2[1] << 8);

	 char format_name[10] = "";
	 if (header.format_type == 1)
	   strcpy(format_name,"PCM");
	 else if (header.format_type == 6)
	   strcpy(format_name, "A-law");
	 else if (header.format_type == 7)
	   strcpy(format_name, "Mu-law");

	 printf("(21-22) Format type: %u %s  \n", header.format_type, format_name);

	 read = read_sd_spi(buffer2,sizeof(buffer2));
	 if(read!=sizeof(buffer2)){
		 printf("channels: %d =! %d\r\n",read,sizeof(buffer2));
	 }

	 header.channels = buffer2[0] | (buffer2[1] << 8);
	 printf("(23-24) Channels: %u  \n", header.channels);

	 read = read_sd_spi(buffer4,sizeof(buffer4));
	 if(read!=sizeof(buffer4)){
		 printf("sample_rate: %d =! %d",read,sizeof(buffer4));
	 }

	 header.sample_rate = buffer4[0] |
							(buffer4[1] << 8) |
							(buffer4[2] << 16) |
							(buffer4[3] << 24);
	 printf("(25-28) Sample rate: %u \n", header.sample_rate);

	 read = read_sd_spi(buffer4,sizeof(buffer4));
	 if(read!=sizeof(buffer4)){
		 printf("byterate: %d =! %d",read,sizeof(buffer4));
	 }

	 header.byterate  = buffer4[0] |
							(buffer4[1] << 8) |
							(buffer4[2] << 16) |
							(buffer4[3] << 24);
	 printf("(29-32) Byte Rate: %u , Bit Rate: %u \n", header.byterate, header.byterate*8);

	 read = read_sd_spi(buffer2,sizeof(buffer2));
	 if(read!=sizeof(buffer2)){
		 printf("block_align: %d =! %d",read,sizeof(buffer2));
	 }

	 header.block_align = buffer2[0] |
						(buffer2[1] << 8);
	 printf("(33-34) Block Alignment: %u  \n", header.block_align);

	 read = read_sd_spi(buffer2,sizeof(buffer2));
	 if(read!=sizeof(buffer2)){
		 printf("bits_per_sample: %d =! %d",read,sizeof(buffer2));
	 }

	 header.bits_per_sample = buffer2[0] |
						(buffer2[1] << 8);
	 printf("(35-36) Bits per sample: %u  \n", header.bits_per_sample);

	 read = read_sd_spi(header.data_chunk_header,sizeof(header.data_chunk_header));
	 if(read!=sizeof(header.data_chunk_header)){
		 printf("data_chunk_header: %d =! %d",read,sizeof(header.data_chunk_header));
	 }
	 printf("(37-40) Data Marker: %s  \n", header.data_chunk_header);

	 read = read_sd_spi(buffer4,sizeof(buffer4));
	 if(read!=sizeof(buffer4)){
		 printf("data_size: %d =! %d",read,sizeof(buffer4));
	 }

	 header.data_size = buffer4[0] |
					(buffer4[1] << 8) |
					(buffer4[2] << 16) |
					(buffer4[3] << 24 );

	 printf("(41-44) Size of data chunk: %u  \n", header.data_size);


	 // calculate no.of samples
	 num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
	 printf("Number of samples: %lu  \n", num_samples);
	 size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
	 printf("Size of each sample: %ld bytes \n", size_of_each_sample);

	 if (header.format_type == 1 && SIZE_SAMPLE == size_of_each_sample && header.channels==2) { // PCM //4byte cada muestra, 2 c/canal
		 parse_stereo_pcm_16=true;
	 }
	 return read;

}
//PCM, Stereo, 16bit
uint8_t parse_data(uint32_t *buffer, uint32_t size){
	 uint8_t status = 1;
	 uint32_t read = 0;
		 // read each sample from data chunk if PCM
	 int32_t data_in_sample = 0; //canal 1 y 2
	 uint8_t offset = 0; // move the offset for data_buffer
	 uint8_t data_buffer[SIZE_SAMPLE];

	 for(int i=0;i<size;i++){
		 if(count_num_samples<num_samples){
			  read = read_sd_spi(data_buffer, sizeof(data_buffer));// dump the data read
			  if (read == SIZE_SAMPLE) {
				count_num_samples++;
				// convert data from little endian to big endian based on bytes in each channel sample
				data_in_sample = (data_buffer[offset + 2] & 0x00ff) |
									((data_buffer[offset + 3] & 0x00ff) <<8) |
									((data_buffer[offset ] & 0x00ff) <<16) |
									(data_buffer[offset + 1]<<24);
				buffer[i]=data_in_sample;
			  }else {
					 printf("Error reading file %d bytes \n", read);
					 gpioWrite( LEDB, ON );
					 status=0;
					 stop_parse();
					 break;
			  }
		  }else{
			printf("Finish read file\n");
			status=0;
			stop_parse();
			break;
		 }
	 }

	 return status;
}
void init_parse(){
	count_num_samples=0;
	parse_stereo_pcm_16=false;
    init_sd_spi();
    open_sd_spi();
}

void stop_parse(){
	printf("close file\n");
	close_sd_spi();
}
