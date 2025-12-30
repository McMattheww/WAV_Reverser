#include "wav.h"
#include "file_lib.h"


wav_header* generate_header(char* data)
{
	//allocate memory for header data
	wav_header* header = malloc(sizeof(wav_header));
	if (header) {

		//assign header member values
		int i;
		for (i = 0;i < 4;i++) {
			header->RIFF[i] = *(data + i);
		}
		header->num_bytes = *((int32_t*)(data + 4));
		for (i = 0;i < 4;i++) {
			header->file_type[i] = *(data + i + 8);
		}
		for (i = 0;i < 4;i++) {
			header->format_chunk[i] = *(data + i + 12);
		}
		header->format_length = *((int32_t*)(data + 16));
		header->format_type = *((int16_t*)(data + 20));
		header->channels = *((int16_t*)(data + 22));
		header->sample_rate = *((int32_t*)(data + 24));
		header->bytes_per_second = *((int32_t*)(data + 28));
		header->multichannel_bytes_per_sample = *((int16_t*)(data + 32));
		header->bits_per_sample = *((int16_t*)(data + 34));
		for (i = 0;i < 4;i++) {
			header->data_header[i] = *(data + i + 36);
		}
		header->data_length = *((int32_t*)(data + 40));
	}
	return header;
}



wav_file* generate_wav(char* file_path) {

	//read in file data to char* raw_bytes
	char* raw_bytes;
	int filesize = read_file(file_path, &raw_bytes);

	//create wav file struct
	wav_file* file = malloc(sizeof(wav_file));
	if (!file) {
		perror("Memory allocation failed");
		free(raw_bytes);
		exit(1);
	}
	file->file_size = filesize;

	//create header for wav file struct
	file->header = generate_header(raw_bytes);
	if (!file->header) {
		perror("Memory allocation failed");
		free(raw_bytes);
		free(file);
		exit(1);
	}

	//assign sound data section for wav file struct
	file->data = malloc(file->header->data_length);
	if (!file->data) {
		perror("Memory allocation failed");
		free(raw_bytes);
		free(file->header);
		free(file);
		exit(1);
	}
	int i;
	for (i = 0;i < file->header->data_length; i++) {
		*(file->data + i) = *(raw_bytes + i + 44);
	}

	//free read file data
	free(raw_bytes);
	//return wav file struct pointer
	return file;
}
