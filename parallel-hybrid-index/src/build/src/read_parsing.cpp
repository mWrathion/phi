#include "../../phi.hpp"

void HybridSelfIndex::readParsing(char *parserFile){
	ulong i, j, k, pos, len, x;
	ifstream is(parserFile, ios::binary);

	if (is.is_open()==false){
		cout << " File not found !!" << endl;
		exit(0);
	}

	is.read((char*)&n, sizeof(ulong));
	is.read((char*)&orig_z, sizeof(ulong));
	is.read((char*)&lgPOS, sizeof(uint));
	is.read((char*)&lgLEN, sizeof(uint));

	ulong size_ARR = orig_z*lgPOS/(8*sizeof(ulong));		// number of 'ulong' segments for ARR_POS
	if ((orig_z*lgPOS)%(8*sizeof(ulong)))
		size_ARR++;
	ARR_POS = new ulong[size_ARR];
	is.read((char*)ARR_POS, size_ARR*sizeof(ulong));

	size_ARR = orig_z*lgLEN/(8*sizeof(ulong));
	if ((orig_z*lgLEN)%(8*sizeof(ulong)))
		size_ARR++;
	ARR_LEN = new ulong[size_ARR];
	is.read((char*)ARR_LEN, size_ARR*sizeof(ulong));
	is.close();

	if(TRACE){
		cout << " Text with n = " << n << endl;
		cout << " Original Dictionary with orig_z = " << orig_z << endl;

		cout << "Dictionary: " << endl;
		pos = getNum64(ARR_POS, 0, lgPOS);
		len = getNum64(ARR_LEN, 0, lgLEN);

		if(len)
			cout << "0:  (" << pos << "," << len<< ") --> 0" << endl;
		else
			cout << "0:  (" << pos << ", * ) --> 0" << endl;
		j=lgPOS;
		k=lgLEN;
		x=1;
		for(i=1; i<orig_z; i++, j+=lgPOS, k+=lgLEN){
			pos = getNum64(ARR_POS, j, lgPOS);
			len = getNum64(ARR_LEN, k, lgLEN);
			if(len){
			//	if (i>=orig_z-20 || x==1048379)
				cout << i << ":  (" <<pos << "," << pos+len-1 << ") --> " << x << endl;
				x += len;
			}else{
			//	if (i>=orig_z-20 || x==1048379)
				cout << i << ":  (" <<pos << ", * ) --> " << x << endl;
				x++;
			}
		}
		cout << endl;
	}
}