#include <stdio.h>

#include "TestU01Settings.h"
#include "FileStreamGen.h"

int main(int argc , char * argv[]) {
    TestU01Settings * settings = initSettings(argc , argv);
    if(!settings) {
        fprintf(stderr , "[ERROR] CLI arguments parsing failure.\n");
        tu01_printUsage();
        return -1;
    }

    executeTest(settings);

    deleteSettings(settings);
    return 0;
}
