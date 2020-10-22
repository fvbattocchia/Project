/*
 * wave.h
 *
 *  Created on: 3 oct. 2020
 *      Author: root
 */

#ifndef PROJECTS_PROTOCOLO_WAVE_H_
#define PROJECTS_PROTOCOLO_WAVE_H_


// WAVE file header format

struct HEADER {
	uint8_t riff[4];                      // RIFF string
	uint32_t overall_size   ;               // overall size of file in bytes
	uint8_t wave[4];                      // WAVE string
	uint8_t fmt_chunk_marker[4];          // fmt string with trailing null char
	uint32_t length_of_fmt;                 // length of the format data
	uint32_t format_type;                   // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law //2bite
	uint32_t channels;                      // no.of channels //2bite
	uint32_t sample_rate;                   // sampling rate (blocks per second)
	uint32_t byterate;                      // SampleRate * NumChannels * BitsPerSample/8
	uint32_t block_align;                   // NumChannels * BitsPerSample/8
	uint32_t bits_per_sample;               // bits per sample, 8- 8bits, 16- 16 bits etc //2bite
	uint8_t data_chunk_header [4];        // DATA string or FLLR string
	uint32_t data_size;                     // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
};


void init_parse();
uint32_t parse_header();
uint8_t parse_data(uint32_t *buffer, uint32_t size);
void stop_parse();

#endif /* PROJECTS_PROTOCOLO_WAVE_H_ */
