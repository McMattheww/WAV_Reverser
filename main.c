#include <stdio.h>
#include <stdlib.h>
#include "file_lib.h"
#include "wav.h"


/**
 * The contents of the file are a char*, but that doesn't
 * mean it is a "string".  When working with "strings" in C
 * though, we normally NULL terminate.  This is not NULL
 * terminated.  Nor could it be, as we then wouldn't be
 * able to represent NULL in the file.  So, we need to print
 * each character separately.  Here, we are printing as bytes
 * (base 16).
 * 
 * @parameters
 * char* file_contents - The file's bytes.
 * size_t num_bytes - The number of bytes to print.
 */
void print_file(char* file_contents, size_t num_bytes){
    // Note this is printing the bytes in hex
    for(size_t i = 0; i<num_bytes; ++i){
        printf("%x", file_contents[i]);
    }
    printf("\n");
}


int main(int argc, char** argv) {

    //we must ensure the program is run with 2 additional arguments,
    if (argc != 3) {
        perror("2 arguments required: path to wav to reverse, and path to write reversed wav");
        exit(1);
    }

    //reads in wav file designated by argument, creates wav_file struct
    wav_file* wav = generate_wav(argv[1]);


    //these checks are ensuring that the wav file we read has the correct values in it's header, else we won't process
    if (wav->header->RIFF[0] != 'R' || wav->header->RIFF[1] != 'I' || wav->header->RIFF[2] != 'F' || wav->header->RIFF[3] != 'F') {
        perror("RIFF not in header");
        exit(1);
    }
    if (wav->header->file_type[0] != 'W' || wav->header->file_type[1] != 'A' || wav->header->file_type[2] != 'V' || wav->header->file_type[3] != 'E') {
        perror("file type is not WAVE");
        exit(1);
    }
    if (wav->header->format_chunk[0] != 'f' || wav->header->format_chunk[1] != 'm' || wav->header->format_chunk[2] != 't') {
        perror("fmt not in format chunk");
        exit(1);
    }
    if (wav->header->data_header[0] != 'd' || wav->header->data_header[1] != 'a' || wav->header->data_header[2] != 't' || wav->header->data_header[3] != 'a') {
        perror("data not in header");
        exit(1);
    }
    if (wav->header->format_type != 1) {
        perror("format type is not 1");
        exit(1);
    }
    if (wav->header->channels != 2) {
        perror("file is not stereo");
        exit(1);
    }

    // allocating memory that will store the reversed wav file data
    char* reversed = malloc(wav->file_size);
    if (!reversed) {
        perror("Memory allocation failed");
        free(wav->data);
        free(wav->header);
        free(wav);
        exit(1);
    }


    //these loops assign char* reversed with the correct bytes to reverse the original wav.
    int i = 0;
    //this loop assigns the first 44 bytes of char* reversed with the header data.
    for (i = 0;i < 44;i++) {
        *(reversed + i) = *((char*)wav->header + i);
    }
    // this loop assigns the bytes after 44 with the data section of our wav struct, correctly reversed.
    for (i = 0;i < wav->header->data_length;i++) {
        //The additional offset of either +1 or -1 swaps the left and right channels back to the correct order.
        if (i % 2 == 0) {
            *(reversed + 44 + i +1) = *(wav->data + wav->header->data_length - 1 - i);
        }
        else {
            *(reversed + 44 + i -1) = *(wav->data + wav->header->data_length - 1 - i);
        }
    }
    
    //writing the reversed wav file to disk with path given by argument
    write_file(argv[2], reversed, wav->file_size);

    //free memory for the reversed wav data
    free(reversed);

    //free memory for original wav file read from disk
    free(wav->data);
    free(wav->header);
    free(wav);

    return 0;
}



