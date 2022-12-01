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
                         u_long* SGCPFT, u_int nSamP, u_long* PhraFT){
	u_char **d_pattern = (u_char **) malloc (nPatt*sizeof(u_char*));
	for (int i = 0; i < nPatt; ++i){
		cudaMalloc((void **) &d_pattern[i], (m+1)*sizeof(u_char));
		cudaMemcpy(d_pattern[i], patterns[i],(m+1)*sizeof(u_char), cudaMemcpyHostToDevice);
	}

    int bytes = size*sizeof(u_char);
    u_char* d_text;
	cudaMalloc((void **) &d_text, bytes);
	cudaMemcpy(d_text, text, bytes, cudaMemcpyHostToDevice);

	
	bool* d_bitVector;
	cudaMalloc((void**) &d_bitVector,z*sizeof(bool));
	cudaMemcpy(d_bitVector, bitVector, z*sizeof(bool), cudaMemcpyHostToDevice);

	u_long* d_sgcpft;
	cudaMalloc((void**) &d_sgcpft, nSamP*sizeof(ulong));
	cudaMemcpy(d_sgcpft, SGCPFT, nSamP*sizeof(ulong), cudaMemcpyHostToDevice);

	u_long* d_phraft;
	cudaMalloc((void**) &d_phraft, z*sizeof(ulong));
	cudaMemcpy(d_phraft, PhraFT, z*sizeof(ulong), cudaMemcpyHostToDevice);

	printf("Data structures loaded into the GPU!\n");

	return d_data{
		d_text,
		d_pattern,
		d_bitVector,
		d_sgcpft,
		d_phraft
	};

}