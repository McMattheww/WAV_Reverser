#ifndef __H__FILE_LIB__
#define __H__FILE_LIB__

#include <stdlib.h>
#include <stdio.h>

/**
 * Reads a file at the location specified by path into
 * memory.  Allocates the required number of bytes after
 * checking file size.  Stores location of stored bytes
 * in *file_contents.
 * 
 * @parameters
 * char* path - The path to the file we want read.
 * char** file_contents - The location in memory where we 
 *                        wish to store the file data.
 * @return The number of bytes read.
 */
size_t read_file(char* path, char** file_contents);

/**
 * Writes a file to disk.
 *
 * @parameters
 * char* path - The path where we wish to save the file.
 * char* file_contents - The location of the file's bytes.
 * size_t num_bytes - The number of bytes to save.
 */
size_t write_file(char* path, char* file_contents, size_t num_bytes);

#endif
