#include "../../phi.hpp"

void HybridSelfIndex::loadStructure(){
	char *fileName = new char[400];
	ulong lenArray;
	sizeDS = 0;

	strcpy(fileName, "");
	strcpy(fileName, dirStore);
	strcat(fileName, "selfHI.hsi");
	ifstream is(fileName, ios::binary);

	// ======================================================================================
	// Loading variables...
	is.read((char*)&n, sizeof(ulong));
	is.read((char*)&nFT, sizeof(ulong));

	is.read((char*)&nG, sizeof(uint));
	is.read((char*)&z, sizeof(uint));
	is.read((char*)&M, sizeof(uint));
	is.read((char*)&lgPT, sizeof(uint));
	is.read((char*)&lgPFT, sizeof(uint));
	is.read((char*)&nSamP, sizeof(uint));
	is.read((char*)&b_X, sizeof(uint));
	is.read((char*)&nSampX, sizeof(uint));

	this->lgN = 1 + (uint)(log(n+1)/log(2));
	this->lgNG = 1 + (uint)(log(nG+1)/log(2));
	this->lgM = 1 + (uint)(log(M)/log(2));
	this->lgZ = 1 + (uint)(log(z)/log(2));

	lenArray = z*lgPT / (8*sizeof(ulong));
	if ((z*lgPT) % (8*sizeof(ulong)))
		lenArray++;
	PhraT = new ulong[lenArray];
	is.read((char*)PhraT, lenArray*sizeof(ulong));
	sizeDS += lenArray*sizeof(ulong);

	lenArray = nSamP*lgN / (8*sizeof(ulong));
	if ((nSamP*lgN) % (8*sizeof(ulong)))
		lenArray++;
	SGCPT = new ulong[lenArray];
	is.read((char*)SGCPT, lenArray*sizeof(ulong));
	sizeDS += lenArray*sizeof(ulong);

	lenArray = z*lgPFT / (8*sizeof(ulong));
	if ((z*lgPFT) % (8*sizeof(ulong)))
		lenArray++;
	PhraFT = new ulong[lenArray];
	is.read((char*)PhraFT, lenArray*sizeof(ulong));
	sizeDS += lenArray*sizeof(ulong);

	lenArray = nSamP*lgN / (8*sizeof(ulong));
	if ((nSamP*lgN) % (8*sizeof(ulong)))
		lenArray++;
	SGCPFT = new ulong[lenArray];
	is.read((char*)SGCPFT, lenArray*sizeof(ulong));
	sizeDS += lenArray*sizeof(ulong);

	if(nG){
		// Loading arrays of the grid...
		lenArray = nG*lgN / (8*sizeof(ulong));
		if ((nG*lgN) % (8*sizeof(ulong)))
			lenArray++;
		X = new ulong[lenArray];
		is.read((char*)X, lenArray*sizeof(ulong));
		sizeDS += lenArray*sizeof(ulong);

		LOOKUP_X = new uint[nSampX];
		is.read((char*)LOOKUP_X, nSampX*sizeof(uint));
		sizeDS += nSampX*sizeof(uint);

		COUNT_X = new uint[nSampX];
		is.read((char*)COUNT_X, nSampX*sizeof(uint));
		sizeDS += nSampX*sizeof(uint);

		lenArray = nG*lgZ / (8*sizeof(ulong));
		if ((nG*lgZ) % (8*sizeof(ulong)))
			lenArray++;
		Pr = new ulong[lenArray];
		is.read((char*)Pr, lenArray*sizeof(ulong));
		sizeDS += lenArray*sizeof(ulong);

		lenArray = nG*lgNG / (8*sizeof(ulong));
		if ((nG*lgNG) % (8*sizeof(ulong)))
			lenArray++;
		Src = new ulong[lenArray];
		is.read((char*)Src, lenArray*sizeof(ulong));
		sizeDS += lenArray*sizeof(ulong);
	}
	is.close();

	// ======================================================================================
	strcpy(fileName, "");
	strcpy(fileName, dirStore);
	strcat(fileName, "BL_il.hsi");
	load_from_file(BL_il, fileName);
	sizeDS += size_in_bytes(BL_il);

	strcpy(fileName, "");
	strcpy(fileName, dirStore);
	strcat(fileName, "rankBL.hsi");
	load_from_file(rankBL, fileName);
	util::init_support(rankBL, &BL_il);

	// ======================================================================================
	if(nG){
		// Loading rmqY...
		strcpy(fileName, "");
		strcpy(fileName, dirStore);
		strcat(fileName, "rmqY.hsi");
		rmqY = new RMQRMM64(fileName);
		sizeDS += rmqY->getSize();
	}

    strcpy(fileName, "");
	strcpy(fileName, dirStore);
	strcat(fileName, "spp.hsi");
	ifstream is2(fileName);
    stringstream buffer;
    buffer << is2.rdbuf();
	const string s = buffer.str();
	int size = s.size();
	fTxt = (uchar*) malloc(size*sizeof(uchar));
	for(int i = 0; i < size; ++i){
		fTxt[i] = (uchar) s[i];
	}

	sizeDS += size*sizeof(uchar);

	if (SHOW_SIZE || TRACE){
		cout << "BreakDown --size of the structures:" << endl;

		cout << " ** load BL_il[] of size " << size_in_bytes(BL_il) << " Bytes" << endl;

		lenArray = z*lgPT / (8*sizeof(ulong));
		if ((z*lgPT) % (8*sizeof(ulong)))
			lenArray++;
		cout << " ** load PhraT[] of size " << lenArray*sizeof(ulong) << " Bytes" << endl;

		lenArray = nSamP*lgN / (8*sizeof(ulong));
		if ((nSamP*lgN) % (8*sizeof(ulong)))
			lenArray++;
		cout << " ** load SGCPT[] of size " << lenArray*sizeof(ulong) << " Bytes" << endl;

		lenArray = z*lgPFT / (8*sizeof(ulong));
		if ((z*lgPFT) % (8*sizeof(ulong)))
			lenArray++;
		cout << " ** load PhraFT[] of size " << lenArray*sizeof(ulong) << " Bytes" << endl;

		lenArray = nSamP*lgN / (8*sizeof(ulong));
		if ((nSamP*lgN) % (8*sizeof(ulong)))
			lenArray++;
		cout << " ** load SGCPFT[] of size " << lenArray*sizeof(uint) << " Bytes" << endl;

		if(nG){
			lenArray = nG*lgN / (8*sizeof(ulong));
			if ((nG*lgN) % (8*sizeof(ulong)))
				lenArray++;
			cout << " ** load X[] of size " << lenArray*sizeof(ulong) << " Bytes" << endl;

			cout << " ** load LOOKUP_X[]+COUNT_X[] of size " << 2*nSampX*sizeof(uint) << " Bytes" << endl;

			lenArray = nG*lgZ / (8*sizeof(ulong));
			if ((nG*lgZ) % (8*sizeof(ulong)))
				lenArray++;
			cout << " ** load Pr[] of size " << lenArray*sizeof(ulong) << " Bytes" << endl;

			lenArray = nG*lgNG / (8*sizeof(ulong));
			if ((nG*lgNG) % (8*sizeof(ulong)))
				lenArray++;
			cout << " ** load Src[] of size " << lenArray*sizeof(ulong) << " Bytes" << endl;

			cout << " ** load rmqY of size " << rmqY->getSize() << " Bytes" << endl;
		}

	}

	cout << "______________________________________________________________" << endl;
	cout << "   Index Structures loaded. sizeDS = " << sizeDS/(1024.0*1024.0) << " MiB = " << (float)sizeDS*8.0/(float)n << " bpc" << endl;
	cout << "______________________________________________________________" << endl;
}