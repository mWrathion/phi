#include "../../phi.hpp"

void HybridSelfIndex::testPredecessor(){
	cout << "Testing Predecessor of i ..." << endl;
	ulong j, pred, lastVal, x;

	if (nG>100){
		cout << "X[0..99] = " << endl;
		for(j=0; j<100; j++){
			cout << getNum64(X, j*lgN, lgN) << " ";
			if ((j+1) % 20 == 0)
				cout << endl;
		}
		cout << endl;
	}

	lastVal = getNum64(X, (nG-1)*lgN, lgN);
	for (j=0, x=getNum64(X, 0, lgN); x<lastVal ; x++){
		for(; getNum64(X, (j+1)*lgN, lgN) <= x; j++);

		//if (x == 65536)
		//	cout << "";

		if (findPredecessor(x, &pred)==false || pred != j){
			cout << "ERR. predecessor of " << x << " = " << pred << " Not found or it is different than its real position pred = " << j << endl;
			exit(1);
		}
	}

	findPredecessor(lastVal, &pred);
	if (pred != nG-1){
		cout << "ERR. predecessor of " << lastVal << " = " << pred << " != real pos pred = " << nG-1 << endl;
		exit(1);
	}

	cout << "Test Predecessor OK !!" << endl;
}