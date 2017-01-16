#include "FileStreamGen.h"

void fsg_state_FillBuffer(void * junk , void * state) {
    if(!state) return;

    FileStreamGen_state * f_state = (FileStreamGen_state *) state;

    if(!f_state->binStreamBuffer) {
        f_state->binStreamBuffer = (unsigned char *)
                calloc (FSG_BUFFER_SIZE , sizeof(unsigned char));
        if(!f_state->binStreamBuffer) {
            fprintf(stderr , "[ERROR] Memory allocation error.\n");
            return;
        }
    }

    f_state->bytesReadFromFile += FSG_BUFFER_SIZE;
    /* Reset buffer position */
    f_state->binStreamBufferPos = 0;

    size_t readBytes = fread(f_state->binStreamBuffer , sizeof(unsigned char) ,
                             FSG_BUFFER_SIZE , f_state->binStreamFile);

    while (readBytes != FSG_BUFFER_SIZE) {
        /* Buffer was not entirely filled, rewind file */
        rewind(f_state->binStreamFile);
        readBytes += fread(f_state->binStreamBuffer + readBytes , sizeof(unsigned char) ,
                           FSG_BUFFER_SIZE - readBytes , f_state->binStreamFile);
        /* Increment count of rewinds */
        f_state->fileStreamRewinded++;
    }
}

void FileStreamGen_Write(void * state) {
    if(!state) return;

    FileStreamGen_state * f_state = (FileStreamGen_state *) state;
    printf("========= State of the binary file stream generator =========\n");
    printf("          Bytes needed for testing:     %llu\n" ,
           f_state->bytesReadFromFile - FSG_BUFFER_SIZE + f_state->binStreamBufferPos);
    printf("          Bytes read from file:         %llu\n" ,
           f_state->bytesReadFromFile);
    printf("          Total number of file rewinds: %llu\n\n" ,
           f_state->fileStreamRewinded);
}

unsigned long FileStreamGen_Bits(void * junk, void * state) {
    if(!state) return 0;

    FileStreamGen_state * f_state = (FileStreamGen_state *) state;

    /* Conversion of 4 chars into 32 bit integer */
    unsigned long x = (unsigned long) f_state->binStreamBuffer[f_state->binStreamBufferPos] << 24;
    x |= (unsigned long) f_state->binStreamBuffer[f_state->binStreamBufferPos + 1] << 16;
    x |= (unsigned long) f_state->binStreamBuffer[f_state->binStreamBufferPos + 2] << 8;
    x |= (unsigned long) f_state->binStreamBuffer[f_state->binStreamBufferPos + 3];

    /* Incrementing buffer position by 4 - number of chars in 32 bits */
    f_state->binStreamBufferPos += 4;
    /* Check if we are at the end of buffer */
    /* Reload buffer if so. */
    if (f_state->binStreamBufferPos == FSG_BUFFER_SIZE)
        f_state->FillBuffer(NULL , state);

    return x;
}

double FileStreamGen_U01(void * junk, void * state) {
    if(!state) return 0;

    return FileStreamGen_Bits(NULL , state) * unif01_INV32;
}

unif01_Gen * unif01_CreateFileStreamGen(char * binStreamFileName) {
    if(!binStreamFileName) return NULL;

    if(FSG_BUFFER_SIZE % 4 != 0) {
        fprintf(stderr , "[ERROR] FileStream Generator buffer "
                         "size is not divisible by 4.\n");
        return NULL;
    }
    unif01_Gen * gen;
    gen = (unif01_Gen *) malloc(sizeof(unif01_Gen));
    if (!gen) {
        fprintf(stderr , "[ERROR] Memory allocation error.\n");
        return NULL;
    }
    gen->GetBits = NULL;
    gen->GetU01  = NULL;
    gen->Write   = NULL;
    gen->name    = NULL;
    /* Param is always null and it's okay. */
    gen->param   = NULL;
    gen->state   = NULL;

    char name[] = "Generator providing data from binary file.";
    size_t len = strlen(name);
    gen->name = (char *) calloc(len + 1, sizeof(char));
    if (!gen->name) {
        fprintf(stderr , "[ERROR] Memory allocation error.\n");
        unif01_DeleteFileStreamGen(gen);
        return NULL;
    }
    strncpy(gen->name, name, len);

    gen->Write   = &FileStreamGen_Write;
    gen->GetBits = &FileStreamGen_Bits;
    gen->GetU01  = &FileStreamGen_U01;

    /* State creation */
    FileStreamGen_state * fsg_state =
            (FileStreamGen_state *) malloc (sizeof(FileStreamGen_state));
    if(!fsg_state) {
        fprintf(stderr , "[ERROR] Memory allocation error.\n");
        unif01_DeleteFileStreamGen(gen);
        return NULL;
    }
    gen->state = fsg_state;

    /* Initialize state variables */
    fsg_state->binStreamFile = fopen(binStreamFileName , "rb");
    if(!fsg_state->binStreamFile) {
        fprintf(stderr , "[ERROR] Can't open file %s.\n" ,
                binStreamFileName);
        unif01_DeleteFileStreamGen(gen);
        return NULL;
    }
    fsg_state->binStreamBuffer    = NULL;
    fsg_state->binStreamBufferPos = 0;
    fsg_state->bytesReadFromFile  = 0;
    fsg_state->fileStreamRewinded = 0;
    fsg_state->FillBuffer         = &fsg_state_FillBuffer;

    /* Initializing binStreamBuffer in state */
    fsg_state->FillBuffer(NULL , fsg_state);
    if(!fsg_state->binStreamBuffer) {
        unif01_DeleteFileStreamGen(gen);
        return NULL;
    }

    /* All is well, return generator! */
    return gen;
}

void unif01_DeleteFileStreamGen(unif01_Gen * gen) {
    /* Deallocation of memory */
    if (gen == NULL)
        return;
    if (gen->name) {
        free(gen->name);
        gen->name = NULL;
    }
    if(gen->state) {
        FileStreamGen_state * fsg_state = (FileStreamGen_state *)gen->state;
        if(fsg_state->fileStreamRewinded > 0) {
            fprintf(stderr , "[ERROR] File was rewinded during testing. This could have "
                             "large impact on the results. Use longer file!\n");
        }
        if(fsg_state->binStreamBuffer) {
            free(fsg_state->binStreamBuffer);
            fsg_state->binStreamBuffer = NULL;
        }
        if(fsg_state->binStreamFile) {
            fclose(fsg_state->binStreamFile);
            fsg_state->binStreamFile = NULL;
        }
        free(fsg_state);
    }
    free(gen);
}

