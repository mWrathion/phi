#include "../../phi.hpp"

// Giving newX: source of the phrase i = Q[k], we move 'newX' go up until its correct position in the partial minimum queue Q[1..k]
void HybridSelfIndex::swimInMinQX(ulong *Q, ulong newX, ulong i, ulong k, ulong *Source){
	ulong x, pX;

	pX = getNum64(Q, (k/2)*lgZ, lgZ);
	x = getNum64(Source, pX*lgN, lgN);

	while(k > 1 && x > newX){
		setNum64(Q, k*lgZ, lgZ, pX);
		k /= 2;

		pX = getNum64(Q, (k/2)*lgZ, lgZ);
		x = getNum64(Source, pX*lgN, lgN);
	}

	setNum64(Q, k*lgZ, lgZ, i);	// 'i' represents the source newX
}