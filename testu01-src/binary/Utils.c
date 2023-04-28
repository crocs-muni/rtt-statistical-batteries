#include "Utils.h"

int u_strtoi(const char * str , int * rval) {
    if(sscanf(str , "%d" , rval) != 1) {
        fprintf(stderr , "[ERROR] Can't convert %s to integer.\n" , str);
        return -1;
    }
    return 0;
}


int u_strtod(const char * str, double * rval) {
    if(sscanf(str , "%lf" , rval) != 1) {
        fprintf(stderr , "[ERROR] Can't convert %s to double.\n" , str);
        return -1;
    }
    return 0;
}

int u_dtolong(double src, long * rval) {
    if(src > LONG_MAX || src < LONG_MIN) {
        fprintf(stderr , "[ERROR] Number %f is out of long type range.\n" , src);
        return -1;
    }
    *rval = (long)src;
    return 0;
}

int u_dtoi(double src, int * rval) {
    if(src > INT_MAX || src < INT_MIN) {
        fprintf(stderr , "[ERROR] Number %f is out of int type range.\n" , src);
        return -1;
    }
    *rval = (int)src;
    return 0;
}
