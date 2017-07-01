#include <stdio.h>

#include "TestU01Settings.h"
#include "FileStreamGen.h"

//#define TESTING

int main(int argc , char * argv[]) {
#ifdef TESTING
#else
    /* Simple main function. So elegant. */
    TestU01Settings * settings = initSettings(argc , argv);
    if(!settings) {
        fprintf(stderr , "[ERROR] CLI arguments parsing failure.\n");
        tu01_printUsage();
        return -1;
    }

    executeTest(settings);

    deleteSettings(settings);
    return 0;
#endif
}
