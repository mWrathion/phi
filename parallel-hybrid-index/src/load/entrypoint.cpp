#include "../phi.hpp"
#include "load.cuh"

HybridSelfIndex::HybridSelfIndex(char dirSaveLoad[300]){
	double tLoad = getTime_ms();
	{
		strcpy(dirStore, dirSaveLoad);
		this->sizeDS = 0;

		loadStructure();

		prepareGPU();
	}


	tLoad = getTime_ms() - tLoad;
	cout << "====================================================" << endl;
	cout << " ### Hybrid Self-Index Loaded in = " << tLoad/(1000.0*60.0) << " minutes." << endl;
	cout << "====================================================" << endl << endl;

	cout << "====================================================" << endl;
	cout << " ### Index size " << sizeDS << " bytes = " << sizeDS/(1024.0*1024.0) << " MiB = " << (float)sizeDS*8.0/(float)n << " bpc" << endl;
	cout << "====================================================" << endl << endl;
}

