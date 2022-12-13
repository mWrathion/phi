#include "src/phi.hpp"
#include <iostream>

bool TRACE = false;			// true: print all details for console
bool TEST = 0;			// true: apply exhaustive test
bool SHOW_SIZE = false;

typedef struct {
	ulong n;				// Length of original Text
	uint M;					// maximum pattern length
	uint m; 				// pattern length
	uint nPatt;				// number of patterns to test with
	uint nThreads;			// number of threads to use on the CPU
	char parserFile[400];	// file with the parser
	char pattFile[300];		// directory to read the patterns
	char prefixStore[300];	// Prefix name of results files in the experiments (folder/prefix)

	HybridSelfIndex *index;

	d_data device_data;

	uchar* text;
	uchar** patterns;			// The patterns to search in CPU
} ParProg;

int main(int argc, char *argv[]){
    ParProg *par = new ParProg();

	if(argc != 7){
		cout << "ERROR with parameters!! " << endl;
		cout << "build usage requires 6 parameters:" << endl;
		cout << "<patterns file> <pattern length m> <pattern quantity> <load prefixStore> <# threads> <name of the text>" << endl;
		exit(1);
	}

    HybridSelfIndex::TRACE = TRACE;
	HybridSelfIndex::CREATE_FMI_TEST = TEST;
	HybridSelfIndex::SHOW_SIZE = SHOW_SIZE;

	strcpy(par -> pattFile, argv[1]);
	par -> m = atoi(argv[2]);
	par -> nPatt = atoi(argv[3]);
	strcpy(par -> prefixStore, argv[4]);
	par -> nThreads = atoi(argv[5]);
	char inputText[100];
	strcpy(inputText, argv[6]);

	par -> index = new HybridSelfIndex(par -> prefixStore);
	par -> n = par -> index -> n;
	par -> M = par -> index -> M;

	par -> patterns = loadPatterns(par->pattFile, par->m, par->nPatt);
	par -> device_data = par -> index -> dataToGPU(par -> patterns, par -> m, par -> nPatt);

	par -> index -> locate(par -> patterns, inputText, par -> m, par -> nPatt, par -> nThreads, par -> device_data);

    return 0;
}