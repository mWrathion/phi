#include "../../kernels.cuh"

/**
 * @brief Preload every needed array to solve PM into the GPU
 * 
 * @param text The LZ77 filtered text
 * @param size size of the LZ77 filtered text
 * @param patterns Array of patterns to search
 * @param m size of the patterns
 * @param nPatt number of patterns to search
 * @param bitVector bit vector to search literal phrases
 * @param z lenght of the bit vector
 * @return d_data 
 */
d_data loadGPUStructures(u_char *text, u_int size, 
                         u_char** patterns, u_int m, u_int nPatt, 
                         bool* bitVector, u_int z,
                         u_long* SGCPFT, u_int nSamP, u_long* PhraFT,
						 u_long* SGCPT, u_int lgPT, u_long* PhraT){

	printf("Loading patterns with size %d\n", nPatt);

	u_char **d_pattern = (u_char **) malloc (nPatt*sizeof(u_char*));
	for (int i = 0; i < nPatt; ++i){
		cudaMalloc((void **) &d_pattern[i], (m+1)*sizeof(u_char));
		cudaMemcpy(d_pattern[i], patterns[i],(m+1)*sizeof(u_char), cudaMemcpyHostToDevice);
	}

	printf("Loading bytes with size %d\n", size);


    int bytes = size*sizeof(u_char);
    u_char* d_text;
	cudaMalloc((void **) &d_text, bytes);
	cudaMemcpy(d_text, text, bytes, cudaMemcpyHostToDevice);

	printf("Loading bit vector with size %d\n", z);


	bool* d_bitVector;
	cudaMalloc((void**) &d_bitVector,z*sizeof(bool));
	cudaMemcpy(d_bitVector, bitVector, z*sizeof(bool), cudaMemcpyHostToDevice);

	printf("Loading sgcpft with size %d\n", nSamP);

	u_long* d_sgcpft;
	cudaMalloc((void**) &d_sgcpft, nSamP*sizeof(u_long));
	cudaMemcpy(d_sgcpft, SGCPFT, nSamP*sizeof(u_long), cudaMemcpyHostToDevice);

	printf("Loading sgcpt with size %d\n", nSamP);

	u_long* d_sgcpt;
	cudaMalloc((void**) &d_sgcpt, nSamP*sizeof(u_long));
	cudaMemcpy(d_sgcpt, SGCPT, nSamP*sizeof(u_long), cudaMemcpyHostToDevice);

	printf("Loading phraFT with size %d\n", z);

	u_long* d_phraft;
	cudaMalloc((void**) &d_phraft, z*sizeof(u_long));
	cudaMemcpy(d_phraft, PhraFT, z*sizeof(u_long), cudaMemcpyHostToDevice);


	u_long lenArray = z*lgPT / (8*sizeof(u_long));
	if ((z*lgPT) % (8*sizeof(u_long)))
		lenArray++;
	u_long* d_phrat;
	printf("Loading phraT with size %d\n", lenArray);
	cudaMalloc((void**) &d_phrat, lenArray*sizeof(u_long));
	cudaMemcpy(d_phrat, PhraT, lenArray*sizeof(u_long), cudaMemcpyHostToDevice);

	u_long* d_occs;
	cudaMalloc((void**) &d_occs, size*sizeof(u_long));


	printf("Data structures loaded into the GPU!\n");

	return d_data{
		d_text,
		d_pattern,
		d_bitVector,
		d_sgcpft,
		d_phraft,
		d_sgcpt,
		d_phrat,
		d_occs
	};
}

void prepareOccs(u_long* d_occs, u_long* occs, u_int nFT){
	for(int i = 0; i < nFT; ++i){
		occs[i] = 0;
	}
	printf("occs de 0 %d y nFT %d\n",occs[0], nFT);
	cudaMemcpy(d_occs, occs, nFT*sizeof(u_long), cudaMemcpyHostToDevice);
}
