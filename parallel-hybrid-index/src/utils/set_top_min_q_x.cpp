#include "../phi.hpp"

// set Q[k] as top of the queue Q[1..k-1] and move down it until its correct position
void HybridSelfIndex::setTopMinQX(ulong *Q, uint k, ulong *Source){
	ulong val, pVal, x, pX, x2, pX2, m;

	pVal = getNum64(Q, k*lgZ, lgZ);
	val = getNum64(Source, pVal*lgN, lgN);

	m = 2;
	pX = getNum64(Q, m*lgZ, lgZ);
	x = getNum64(Source, pX*lgN, lgN);

	if (m+1 < k){
		pX2 = getNum64(Q, (m+1)*lgZ, lgZ);
		x2 = getNum64(Source, pX2*lgN, lgN);

		if (x2 < x){
			m++;
			x=x2;
			pX=pX2;
		}
	}

	while(m < k && val > x){
		setNum64(Q, (m/2)*lgZ, lgZ, pX);
		m <<= 1;

		if (m < k){
			pX = getNum64(Q, m*lgZ, lgZ);
			x = getNum64(Source, pX*lgN, lgN);
			if (m+1 < k){
				pX2 = getNum64(Q, (m+1)*lgZ, lgZ);
				x2 = getNum64(Source, pX2*lgN, lgN);

				if (x2 < x){
					m++;
					x=x2;
					pX=pX2;
				}
			}
		}
	}

	setNum64(Q, (m/2)*lgZ, lgZ, pVal);
}