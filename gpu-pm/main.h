#pragma once
#include <stdio.h>
#include "kernel.cuh"
#include <sdsl/suffix_arrays.hpp>

void h_call(unsigned char *text, char *pattern, int *occs, int size, int m, float &time, long &c){
    calling(text, pattern, occs, size, m, time, c);
}

unsigned char* h_load(unsigned char *text, int size){
    return load(text, size);
}