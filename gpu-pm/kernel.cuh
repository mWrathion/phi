#pragma once
#include <stdio.h>


struct dvc{
	unsigned char* text;
	unsigned char** pattern;
};


void calling(unsigned char *text, unsigned char *pattern, int *occs, int size, int m, float &time, long &c);


dvc load(unsigned char *text, int size, unsigned char** pattern, int m);