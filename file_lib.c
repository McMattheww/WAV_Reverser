#include "file_lib.h"

size_t read_file(char* path, char** file_contents){
    FILE *file = fopen(path, "rb"); // Open the file in binary mode
    if (file == NULL) {
        perror("File opening failed");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    if (size == -1) {
        perror("ftell failed");
        fclose(file);
        return 1;
    }
    fseek(file, 0, SEEK_SET);

    *file_contents = malloc(size * sizeof(char));
    fread(*file_contents, 1, size, file);
    fclose(file);
    return size;
}

size_t write_file(char* path, char* file_contents, size_t num_bytes){
    FILE * file = fopen(path, "wb");
    if(file == NULL){
        perror("File opening for write failed.");
        return 1;
    }
    size_t bytes_written = fwrite(file_contents, 1, num_bytes, file);
    fclose(file);

    return bytes_written;
}
