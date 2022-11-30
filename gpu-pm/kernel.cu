#include "kernel.cuh"

__global__ void kernel_call(unsigned char *text, unsigned char *pattern, int *occs, int size, int m, int *cont){
	int tid = blockIdx.x * blockDim.x + threadIdx.x;
	bool flag = true;
	int c = 0;
	
	if (tid < size - m){
		for (int i = tid; i < tid+m; ++i){
			if(text[i] != pattern[c]){	
				flag = false;	
			}
			c++;
		}
		if (flag == 1) {occs[tid] = 0; atomicAdd(cont, 1);};
	}
	if (tid == 0){
		printf("pattern: ");
		for (int i = 0; i < m; ++i){
			printf("%c", pattern[i]);
		}
		printf("\n");
	}
}

void calling(unsigned char *text, unsigned char *pattern, int *occs, int size, int m, float &time, long &c){
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	int bytes = size*sizeof(unsigned char);

	//unsigned char *d_text;
	// char	*d_pattern;
	int *d_occs;
	int *d_cont;
	int cont = 0;
	//cudaMalloc((void **) &d_text, size*sizeof(unsigned char));
	// cudaMalloc((void **) &d_pattern, m*sizeof(char));
	cudaMalloc((void **) &d_occs, size*sizeof(int));	
	cudaMalloc((void **) &d_cont, sizeof(int));


	//cudaMemcpy(d_text, text, bytes, cudaMemcpyHostToDevice);
	// cudaMemcpy(d_pattern, pattern, m*sizeof(unsigned char), cudaMemcpyHostToDevice);
	cudaMemcpy(d_occs, occs, size*sizeof(int), cudaMemcpyHostToDevice);	
	cudaMemcpy(d_cont, &cont, 1*sizeof(int), cudaMemcpyHostToDevice);
	cudaEventRecord(start,0);

	kernel_call<<<(size+1023)/1024, 1024>>>(text, pattern, d_occs, size, m, d_cont);
	cudaMemcpy(occs, d_occs, size*sizeof(int), cudaMemcpyDeviceToHost); 
	cudaMemcpy(&cont, d_cont, 1*sizeof(int), cudaMemcpyDeviceToHost);

	
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&time, start, stop);
	time = time/1000.0f;
	
	long acum = 0;
	
	// for(int i = 0; i < size; ++i){
	// 	if(occs[i] == 0){
	// 		c++;
	// 		acum++;
	// 	}
	// }
	c+=cont;
	acum+=cont;
	printf("cont=%d, acum=%d, c=%i\n", cont, acum,c);
	cudaFree(d_occs);
	cudaFree(d_cont);
	// cudaFree(d_pattern);
}


dvc load(unsigned char *text, int size, unsigned char** pattern, int m){
	unsigned char *d_text;
	unsigned char **d_pattern = (unsigned char **) malloc (1000*sizeof(unsigned char*));
	int bytes = size*sizeof(unsigned char);
	cudaMalloc((void **) &d_text, size*sizeof(unsigned char));
	
	for (int i = 0; i < 1000; ++i){
		cudaMalloc((void **) &d_pattern[i], (m+1)*sizeof(unsigned char));
		
		// cudaMemcpy(&d_pattern, &aux, sizeof(unsigned char *), cudaMemcpyHostToDevice);
		// cudaFree(aux);
	}

	for (int i = 0; i < 1000; ++i){
		cudaMemcpy(d_pattern[i], pattern[i],(m+1)*sizeof(unsigned char), cudaMemcpyHostToDevice);
	}
	cudaMemcpy(d_text, text, bytes, cudaMemcpyHostToDevice);

	return dvc{
		d_text,
		d_pattern
	};
}