#include "../../kernels.cuh"

__device__ u_long getNum64(u_long* A, u_int ini, u_int len){
    u_long i=ini>>6, j=ini-(i<<6);
    u_long result = (A[i] << j) >> (64-len);

    if (j+len > 64)
        result = result | (A[i+1] >> (128-j-len));

    return result;
}

__device__ bool isPrimary(u_long x, u_int len, u_long *pIni, u_int *dIni, 
                          u_int nSamP, u_int lgN, u_int POT_GC, u_long* SGCPFT, 
                          u_int lgPFT, u_long* PhraFT){
	u_long l=0, r=nSamP-1, m=nSamP>>1, x1, x2, phr;

	while (l<=r){
		x1 = getNum64(SGCPFT, m*lgN, lgN);
		if (x1 <= x){
			if (m<r){
				x2 = getNum64(SGCPFT, (m+1)*lgN, lgN);
				if (x2 > x){
					phr = m<<POT_GC;
					break;
				}else{
					l=m+1;
					m=(l+r)>>1;
				}
			}else{
				phr = m<<POT_GC;
				break;
			}
		}else {
			m--;
			if (l<=m){
				x1 = getNum64(SGCPFT, m*lgN, lgN);
				if (x1 <= x){
					phr = m<<POT_GC;
					break;
				}else{
					r=m-1;
					m=(l+r)>>1;
				}
			}else {
				phr = m<<POT_GC;
				break;
			}
		}
	}

	// retrieve gaps from x1 to x
	for(r = (phr+1)*lgPFT; x1<x; phr++, r+=lgPFT)
		x1 += getNum64(PhraFT, r, lgPFT);

	if (x1 == x){
		*dIni = 0;
		*pIni = phr;
		x1 += getNum64(PhraFT, r, lgPFT);
	}else {
		*pIni = phr;
		*dIni = x1-x;
	}

	if (x+len <= x1)
		return false;

	return true;
}

__global__ void getPrimaryOccurrences(u_char* text, u_int size, u_char* pattern, u_int m, int *nOcc, 
                                      bool* BL_il,
                                      u_int nSamP, u_int lgN, u_int POT_GC, u_long* SGCPFT, 
                                      u_int lgPFT, u_long* PhraFT){
    u_int tid = blockIdx.x * blockDim.x + threadIdx.x;
	bool flag = true;
	u_int c = 0;
	
	if (tid < size - m){
		for (int i = tid; i < tid+m; ++i){
			if(text[i] != pattern[c]){	
				flag = false;	
			}
			c++;
		}
		if (flag == 1) {  
			long id = atomicAdd(nOcc, 1);
 
           /* u_long pr;
            u_int dx;
            long id = -1;
            if(isPrimary(tid, m, &pr, &dx,nSamP, lgN, POT_GC, SGCPFT, lgPFT, PhraFT)){
                
                id = atomicAdd(nOcc, 1);
            }
            else{
                if(dx){
                    if(BL_il[pr-1]){
                        id = atomicAdd(nOcc, 1);
                    }
                }
                else{
                    if(BL_il[pr]){
                        id = atomicAdd(nOcc, 1);
                    }
                }
            }*/
            //occs[tid] = 0; 
        }
    }
}

void locatePrimaryOccurrences(u_char* text, u_int size, u_char* pattern, u_int m, int &nOcc, 
                              bool* BL_il,
                              u_int nSamP, u_int lgN, u_int POT_GC, u_long* SGCPFT, 
                              u_int lgPFT, u_long* PhraFT){
    int *d_cont;
    cudaMalloc((void **) &d_cont, sizeof(int));
    cudaMemcpy(d_cont, &nOcc, sizeof(int), cudaMemcpyHostToDevice);


    getPrimaryOccurrences<<<(size+1023)/1024, 1024>>>(text, size, pattern, m, d_cont, BL_il,
    nSamP, lgN, POT_GC, SGCPFT, lgPFT, PhraFT);

    cudaMemcpy(&nOcc, d_cont, sizeof(int), cudaMemcpyDeviceToHost);
}