#include "../phi.hpp"

HybridSelfIndex::HybridSelfIndex(char *parserFile, uint optM, char dirSaveLoad[300]) {
	double tConst = getTime_ms();
	{
		strcpy(dirStore, dirSaveLoad);
		this->M = optM;
		this->sizeDS = 0;

		// retrieving parsing from file
		cout << " Reading... " << parserFile << endl;
		readParsing(parserFile);

		this->lgN = 1 + (uint)(log(n+1)/log(2));
		this->lgM = 1 + (uint)(log(M)/log(2));

		ulong *Source = nullptr;
		buildBasicStructures(&Source);

		if (nG)
		 	buildPtrRMQonG(Source);
		else
			cout << "WARNING.... Length of the grid nG = 0 !!" << endl;
	}

	tConst = getTime_ms() - tConst;
	if (TRACE){
		cout << "====================================================" << endl;
		cout << " ### Hybrid Self-Index built in = " << tConst/(1000.0*60.0) << " minutes." << endl;
		cout << "====================================================" << endl << endl;

		cout << "====================================================" << endl;
		cout << " ### Index size " << sizeDS << " bytes = " << sizeDS/(1024.0*1024.0) << " MiB = " << (float)sizeDS*8.0/(float)n << " bpc" << endl;
		cout << "====================================================" << endl << endl;
	}
}