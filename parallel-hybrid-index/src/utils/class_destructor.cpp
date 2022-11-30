#include "../phi.hpp"

HybridSelfIndex::~HybridSelfIndex() {
	delete [] PhraT;
	delete [] PhraFT;
	delete [] SGCPT;
	delete [] SGCPFT;

	decltype(BL_il) emptyBL;
	BL_il.swap(emptyBL);

	if(nG){
		delete [] X;
		delete [] LOOKUP_X;
		delete [] COUNT_X;

		delete [] Pr;
		delete [] Src;

		rmqY->~RMQRMM64();
	}

	// if (nFT){
	// 	decltype(FMI) emptyFMI;
	// 	FMI.swap(emptyFMI);
	// }
	// if (CREATE_FMI_TEST){
	// 	decltype(FMI_TEST) emptyFMITEST;
	// 	FMI_TEST.swap(emptyFMITEST);
	// }
}