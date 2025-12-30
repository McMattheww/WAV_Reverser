#ifndef __H__WAV__
#define __H__WAV__
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#pragma pack(1)
typedef struct wav_header{
	char RIFF[4];			    //4 bytes- must be 'RIFF'
	int32_t num_bytes;		//number of bytes in file subtracted by 8
	char file_type[4];        //4 bytes- must be 'WAVE', since we are only processing that type
	char format_chunk[4];     //4 bytes- must be 'fmt'
	int32_t format_length;	//length of format data
	int16_t format_type;    // must be 1
	int16_t channels;       //we are only processing files with 2 channels
	int32_t sample_rate;	//samples per second
	int32_t bytes_per_second;				// sample rate * bits per sample * channels / 8
	int16_t multichannel_bytes_per_sample;  // (Bits per sample * channels) / data type size
	int16_t bits_per_sample;
	char data_header[4];       //4 bytes- must be 'data'
	int32_t data_length;	 //size of data section

} wav_header;
#pragma pack()

typedef struct wav_file {
	wav_header* header; //header data
	int file_size;
	char* data; //sound data
} wav_file;

// function takes the first 44 bytes of data read from a wav file,
// allocates memory for, generates, and returns pointer to header section
wav_header* generate_header(char*);

// function takes a file path and returns file data as a wav_file struct
//dynamically allocated memory must be freed for wav_file->data, wav_file->header, and wav_file* itself.
wav_file* generate_wav(char*);

#endif