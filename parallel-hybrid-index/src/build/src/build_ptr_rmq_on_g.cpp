#include "../../phi.hpp"

void HybridSelfIndex::buildPtrRMQonG(ulong *Source){
	ulong i, k, c, posX, lenS, lenArray, r;

	ulong *QUEUE_G;
	lenArray = (nG+1)*lgZ / (8*sizeof(ulong));
	if (((nG+1)*lgZ) % (8*sizeof(ulong)))
		lenArray++;
	QUEUE_G = new ulong[lenArray];
	setNum64(QUEUE_G, 0, lgZ, 0);

	posX=0;
	c=1;
	for(i=k=0; i<z; i++, k+=lgN){
		//	WE ONLY INCLUDE SOURCES WHAT NOT GENERATE LITERAL PHRASES (ie. BL_il[i] == false)
		if(BL_il[i]==0){
			posX = getNum64(Source, k, lgN);
			setNum64(QUEUE_G, c*lgZ, lgZ, i);
			swimInMinQX(QUEUE_G, posX, i, c, Source);
			c++;
		}
	}

	// for the X array
	lenArray = nG*lgN / (8*sizeof(ulong));
	if ((nG*lgN) % (8*sizeof(ulong)))
		lenArray++;
	X = new ulong[lenArray];
	sizeDS += lenArray*sizeof(ulong);

	// Y array for the RMQ...
	long int *Y = new long int[nG];

	// Pr array
	lenArray = nG*lgZ / (8*sizeof(ulong));
	if ((nG*lgZ) % (8*sizeof(ulong)))
		lenArray++;
	Pr = new ulong[lenArray];
	sizeDS += lenArray*sizeof(ulong);

	//=============================================================================================
	{
		// The additional Src array to solve queries for any pattern length
		lenArray = nG*lgNG / (8*sizeof(ulong));
		if ((nG*lgNG) % (8*sizeof(ulong)))
			lenArray++;
		Src = new ulong[lenArray];
		sizeDS += lenArray*sizeof(ulong);
	}
	//=============================================================================================

	for (i=0, c=nG; i<nG; i++, c--){
		k = getNum64(QUEUE_G, lgZ, lgZ); // phrase from the top of the queue
		setNum64(Pr, i*lgZ, lgZ, k);

		r = k - rankBL.rank(k+1);
		setNum64(Src, r*lgNG, lgNG, i);

		posX = getNum64(Source, k*lgN, lgN);
		setNum64(X, i*lgN, lgN, posX);

		if (k==z-1)
			lenS=1;
		else
			lenS = getNum64(PhraT, (k+1)*lgPT, lgPT);
		Y[i] = posX + lenS - 1;

		if (c > 1)
			setTopMinQX(QUEUE_G, c, Source);	// c is the current length of the queue
	}
	delete [] QUEUE_G;
	delete [] Source;

	//	LOOKUP..
	uint blk;
	ulong blk_X = posX/N_LOOKUP;
	b_X = (uint)(log(blk_X)/log(2));
	nSampX = 1+(posX>>b_X);

	cout << "Final length lookup Table nSampX = " << nSampX << ", bits per segment b_X = " << b_X << endl;

	LOOKUP_X = new uint[nSampX];
	COUNT_X = new uint[nSampX];
	sizeDS += 2*nSampX*sizeof(uint);

	for (i=0; i<nSampX; i++)
		LOOKUP_X[i]=nG+1;

	for (i=0; i<nG; i++){
		posX = getNum64(X, i*lgN, lgN);
		blk = posX>>b_X;

		if (LOOKUP_X[blk] > i)
			LOOKUP_X[blk] = i;
	}

	for (i=0; i<nSampX; i++){
		if (LOOKUP_X[i] > nG)
			LOOKUP_X[i] = LOOKUP_X[i-1];
	}

	for (i=0; i<nSampX-1; i++){
		if (LOOKUP_X[i+1] < LOOKUP_X[i]){
			cout << "ERR.. i = " << i << ", LOOKUP_X[i+1] = " << LOOKUP_X[i+1] << " < LOOKUP_X[i] = " << LOOKUP_X[i] << endl;
			exit(0);
		}
		COUNT_X[i] = LOOKUP_X[i+1] - LOOKUP_X[i];
	}
	COUNT_X[i] = nG - LOOKUP_X[i];

	for (i=0; i<nSampX-1; i++){
		if (COUNT_X[i]==0){
			for (c=i+1; c<nSampX && COUNT_X[c]==0; c++);
			if (c<nSampX && COUNT_X[c])
				COUNT_X[i]=COUNT_X[c];
		}
	}

	float avgCount = 0;
	uint maxCC = 0;
	for (i=0; i<nSampX; i++){
		avgCount += COUNT_X[i];
		if (maxCC < COUNT_X[i]) maxCC = COUNT_X[i];
	}
	avgCount = avgCount/nSampX;
	cout << "maxCC = " << maxCC << ", avgCount = " << avgCount << endl;

	testPredecessor();

	if (TRACE){
		ulong ph;

		cout << "b_X = " << b_X << endl;
		cout << "blk_X = " << blk_X << endl;

		cout << "LOOKUP_X[0.." << nSampX-1 << "](count) = " << endl;
		for(i=0; i<nSampX; i++)
			cout << LOOKUP_X[i] << "(" << COUNT_X[i] << ") ";
		cout << endl;

		cout << "Pr[0.." << nG-1 << "] = " << endl;
		for(i=k=0; i<nG; i++, k+=lgZ)
			cout << getNum64(Pr, k, lgZ) << " ";
		cout << endl;

		cout << "(X, Y) -- > PrX " << endl;
		for(i=k=0; i<nG; i++, k+=lgN){
			ph = getNum64(Pr, i*lgZ, lgZ);
			posX = getPosPhraT(ph);
			cout << i << "(" << getNum64(X, k, lgN) << "," << Y[i] << ") --> " << posX << endl;
		}
		cout << endl;
	}

	for(i=0; i<nG; i++)
		Y[i] = -1*Y[i];

	// Build a RMQ on YG[]
	rmqY = new RMQRMM64(Y, nG);
	sizeDS += rmqY->getSize();

	delete [] Y;

	if (SHOW_SIZE){
		lenArray = nG*lgN / (8*sizeof(ulong));
		if ((nG*lgN) % (8*sizeof(ulong)))
			lenArray++;
		cout << " ** size of X[]  = " << lenArray << " Bytes = " << lenArray*8.0/(float)this->n << " bpc" << endl;

		cout << " ** size of LOOKUP_X[] + COUNT_X[]  = " << 2*nSampX*sizeof(uint) << " Bytes = " << 2.0*nSampX*sizeof(uint)*8.0/(float)this->n << " bpc" << endl;

		lenArray = nG*lgZ / (8*sizeof(ulong));
		if ((nG*lgZ) % (8*sizeof(ulong)))
			lenArray++;
		cout << " ** size of Pr[1..nG] " << lenArray*sizeof(ulong) << " Bytes = " << lenArray*sizeof(ulong)*8.0/(float)this->n << " bpc" << endl;

		lenArray = nG*lgNG / (8*sizeof(ulong));
		if ((nG*lgNG) % (8*sizeof(ulong)))
			lenArray++;
		cout << " ** size of Src[1..nG] " << lenArray*sizeof(ulong) << " Bytes = " << lenArray*sizeof(ulong)*8.0/(float)this->n << " bpc" << endl;

		cout << " ** size of rmqY: " << rmqY->getSize() << " = " << rmqY->getSize()*8.0/(float)n << " bpc" << endl;
	}

	if (TRACE){
		cout << "X[0.." << nG-1 << "] = " << endl;
		for(i=k=0; i<nG; i++, k+=lgN)
			cout << getNum64(X, k, lgN) << " ";
		cout << endl;

		if (false){
			cout << "predForX[0.." << n-1 << "] = " << endl;
			for(i=0; i<n; i++){
				if (findPredecessor(i, &k))
					cout << i << ":" << k << " ";
			}
			cout << endl;
		}

		cout << "Src[0.." << nG-1 << "] = " << endl;
		for(i=k=0; i<nG; i++, k+=lgNG)
			cout << getNum64(Src, k, lgNG) << " ";
		cout << endl;
	}
}
