#include "src/phi.hpp"
#include <iostream>

bool TRACE = false;			// true: print all details for console
bool TEST = 0;			// true: apply exhaustive test
bool SHOW_SIZE = false;

typedef struct {
	ulong n;				// Length of generalize Text = T1$T2$...TD$
	uint M;					// maximum pattern length
	char prefixStore[300];	// directory to save/load the data structure
	char parserFile[300];
	HybridSelfIndex *index;

	ulong* patt;			// data structure are for test and experiments
} ParProg;

int main(int argc, char *argv[]){
    ParProg *par = new ParProg();

	if(argc != 4){
		cout << "ERROR with parameters!! " << endl;
		cout << "build usage requires 3 parameters:" << endl;
		cout << "<lz77 parsed file> <optimal pattern length M> <save prefixStore>" << endl;
		exit(1);
	}

    HybridSelfIndex::TRACE = TRACE;
	HybridSelfIndex::CREATE_FMI_TEST = TEST;
	HybridSelfIndex::SHOW_SIZE = SHOW_SIZE;

    strcpy(par->parserFile, argv[1]);
	par->M = atoi(argv[2]);
	strcpy(par->prefixStore, argv[3]);

    par->index = new HybridSelfIndex(par->parserFile, par->M, par->prefixStore);
	par->index->saveStructure();
	par->index->~HybridSelfIndex();

    return 0;
}