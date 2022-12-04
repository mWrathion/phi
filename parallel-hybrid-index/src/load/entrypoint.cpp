#include "../phi.hpp"

/**
 * @brief Load an stored Hybrid Self Index
 * 
 * @param dirSaveLoad directory of the HSI
 */
HybridSelfIndex::HybridSelfIndex(char dirSaveLoad[300]){
	double tLoad = getTime_ms();
	{
		strcpy(dirStore, dirSaveLoad);
		this->sizeDS = 0;
		loadStructure();

	}
	tLoad = getTime_ms() - tLoad;
	cout << "====================================================" << endl;
	cout << " ### Hybrid Self-Index Loaded in = " << tLoad/(1000.0*60.0) << " minutes." << endl;
	cout << "====================================================" << endl << endl;

	cout << "====================================================" << endl;
	cout << " ### Index size " << sizeDS << " bytes = " << sizeDS/(1024.0*1024.0) << " MiB = " << (float)sizeDS*8.0/(float)n << " bpc" << endl;
	cout << "====================================================" << endl << endl;
}

/**
 * @brief Load to memory all the patterns that will be searched
 * 
 * @param pattFile File that contains all the patterns.
 * @param m Size of the patterns.
 * @param nPatt Number of patterns inside the file.
 * @return uchar** that contains all patterns.
 */
uchar** loadPatterns(char pattFile[300], uint m, uint nPatt){
    uchar **pattern = new uchar* [nPatt];
    string line;
	ifstream input (pattFile);
	int i = 0, j = 0;
	while(input.good() && i < nPatt){
		getline(input, line);
		pattern[i] = new uchar [m+1];
		for(j = 0; j < m; ++j){
			pattern[i][j] = line.at(j);
		}	
		pattern[i][m] = '\0';
		i++;
	}
	input.close();
    return pattern;
}

/**
 * @brief Move all the needed data to GPU in order to solve primary occurences in GPU.
 * 
 * @param patterns Patterns to find.
 * @param m Length of the patterns.
 * @param nPatt Number of patterns.
 * @return d_data All stored Data in GPU.
 */
d_data HybridSelfIndex::dataToGPU(uchar** patterns, uint m, uint nPatt){
	bool* bitVector = bitVectorToArray();
	d_data device_data = loadGPUStructures(fTxt, nFT, 
										   patterns, m, nPatt, 
										   bitVector, z, 
										   SGCPFT, nSamP, PhraFT,
										   SGCPT, lgPT, PhraT);
	return device_data;
}