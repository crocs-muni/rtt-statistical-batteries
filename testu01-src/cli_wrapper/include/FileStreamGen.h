#ifndef FILESTREAMGEN_H
#define FILESTREAMGEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unif01.h"

/**
 * Buffer will ALWAYS be of length BUFFER_SIZE.
 * BUFFER_SIZE HAVE TO BE DIVISIBLE BY 4!!!
 * Otherwise unimaginable things wil happen!!
 * Otherwise fail at creation of generator.
 */
#define FSG_BUFFER_SIZE 10485760


void fsg_param_LoadBuffer(void * param , void * state);

/**
 * Structure for holding state of
 * FileStream Generator.
 */
typedef struct {
    /**
     * Opened fd of binary file
     * File provides source of bits for generator
     */
    FILE * binStreamFile;
    /**
     * Function for filling buffer with new data from file.
     * Is called at the beginning and then every time, when
     * all data in buffer was read.
     */
    void (*FillBuffer) (void * param , void * state);
    /**
     * Buffer for storing data from binStreamFile.
     * Has lenght BUFFER_SIZE
     */
    unsigned char * binStreamBuffer;
    /**
     * Indicates current position in buffer.
     * Position increments after each reading.
     */
    unsigned long binStreamBufferPos;
    /**
     * Keep track of total number of bytes read
     * from file. Is updated at every buffer update.
     */
    unsigned long long bytesReadFromFile;
    /**
     * Count of file rewinds. This number
     * shouldn't be bigger than total number
     * of tests. It's recommended to use file
     * big enough, so that it won't be rewinded.
     */
    unsigned long long fileStreamRewinded;
} FileStreamGen_state;

/**
 * @brief unif01_CreateFileStreamGen creates new
 *        FileStream Generator, source of bits will
 *        be filename given in argument
 * @param binStreamFileName     file that will be
 *                              source of data
 * @return new FileStream Generator
 */
unif01_Gen * unif01_CreateFileStreamGen(char * binStreamFileName);

/**
 * @brief unif01_DeleteFileStreamGen will free
 *        any memory that was allocated by
 *        unif01_CreateFileStreamGen
 * @param gen   Generator that will be deleted
 */
void unif01_DeleteFileStreamGen(unif01_Gen * gen);

/**
 * @brief FileStreamGen_Write writes (on stdout)
 *        state of the FileStream generator.
 * @param state     FileStreamGen_state variable
 */
void FileStreamGen_Write(void * state);

/**
 * @brief FileStreamGen_Bits returns 32 bit
 *        integer created from 4 bytes from file.
 * @param junk      useless variable, can be NULL
 * @param state     FileStreamGen_state variable
 * @return 32 bit long integer
 */
unsigned long FileStreamGen_Bits(void * junk, void * state);

/**
 * @brief FileStreamGen_U01 returns float between
 *        0 and 1 converted from 32 bit integer.
 * @param junk      useless variable, can be NULL
 * @param state     FileStreamGen_state variable
 * @return Float between [0,1)
 */
double FileStreamGen_U01(void * junk, void * state);

/**
 * @brief fsg_state_FillBuffer fills buffer of passed
 *        FileStreamGen_state with new data from binary file.
 *        If reaches end of file, reloads file and
 *        reads from the beginning.
 * @param junk      useless variable, can be null
 * @param state     FileStreamGen_state variable
 */
void fsg_state_FillBuffer(void * junk , void * state);

#endif //FILESTREAMGEN_H
