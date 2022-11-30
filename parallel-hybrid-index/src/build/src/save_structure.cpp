#include "../../phi.hpp"

// save the Data Structure in the directory 'pathName'. pathName has to end with /
void HybridSelfIndex::saveStructure(){
	char *fileName = new char[400];
	ulong lenArray;

	strcpy(fileName, "");
	strcpy(fileName, dirStore);
	strcat(fileName, "selfHI.hsi");
	ofstream os (fileName, ios::binary);

	os.write((const char*)&n, sizeof(ulong));
	os.write((const char*)&nFT, sizeof(ulong));

	os.write((const char*)&nG, sizeof(uint));
	os.write((const char*)&z, sizeof(uint));
	os.write((const char*)&M, sizeof(uint));
	os.write((const char*)&lgPT, sizeof(uint));
	os.write((const char*)&lgPFT, sizeof(uint));
	os.write((const char*)&nSamP, sizeof(uint));
	os.write((const char*)&b_X, sizeof(uint));
	os.write((const char*)&nSampX, sizeof(uint));

	// Saving arrays o fphrases...
	lenArray = z*lgPT / (8*sizeof(ulong));
	if ((z*lgPT) % (8*sizeof(ulong)))
		lenArray++;
	os.write((const char*)PhraT, lenArray*sizeof(ulong));

	lenArray = nSamP*lgN / (8*sizeof(ulong));
	if ((nSamP*lgN) % (8*sizeof(ulong)))
		lenArray++;
	os.write((const char*)SGCPT, lenArray*sizeof(ulong));

	lenArray = z*lgPFT / (8*sizeof(ulong));
	if ((z*lgPFT) % (8*sizeof(ulong)))
		lenArray++;
	os.write((const char*)PhraFT, lenArray*sizeof(ulong));

	lenArray = nSamP*lgN / (8*sizeof(ulong));
	if ((nSamP*lgN) % (8*sizeof(ulong)))
		lenArray++;
	os.write((const char*)SGCPFT, lenArray*sizeof(ulong));

	if(nG){
		// Saving arrays of the grid...
		lenArray = nG*lgN / (8*sizeof(ulong));
		if ((nG*lgN) % (8*sizeof(ulong)))
			lenArray++;
		os.write((const char*)X, lenArray*sizeof(ulong));

		os.write((const char*)LOOKUP_X, nSampX*sizeof(uint));
		os.write((const char*)COUNT_X, nSampX*sizeof(uint));

		lenArray = nG*lgZ / (8*sizeof(ulong));
		if ((nG*lgZ) % (8*sizeof(ulong)))
			lenArray++;
		os.write((const char*)Pr, lenArray*sizeof(ulong));

		lenArray = nG*lgNG / (8*sizeof(ulong));
		if ((nG*lgNG) % (8*sizeof(ulong)))
			lenArray++;
		os.write((const char*)Src, lenArray*sizeof(ulong));
	}

	os.close();

	// ======================================================================================
	strcpy(fileName, "");
	strcpy(fileName, dirStore);
	strcat(fileName, "BL_il.hsi");
	store_to_file(BL_il, fileName);

	strcpy(fileName, "");
	strcpy(fileName, dirStore);
	strcat(fileName, "rankBL.hsi");
	store_to_file(rankBL, fileName);
    
	// ======================================================================================
	if(nG){
		// Saving rmqY...
		strcpy(fileName, "");
		strcpy(fileName, dirStore);
		strcat(fileName, "rmqY.hsi");
		rmqY->saveDS(fileName);
	}

    strcpy(fileName, "");
    strcpy(fileName, dirStore);
    strcat(fileName, "spp.hsi");
    ofstream os2(fileName);
    os2 << (char *) fTxt;
    os2.close();


	if (SHOW_SIZE || TRACE){
		cout << "Save FastHybridIndex data structures in " << fileName << endl;
		cout << "Saving structures..." << endl;

		cout << " .- BL_il[] " << size_in_bytes(BL_il) << " Bytes" << endl;

		lenArray = z*lgPT / (8*sizeof(ulong));
		if ((z*lgPT) % (8*sizeof(ulong)))
			lenArray++;
		cout << " .- PhraT[] " << lenArray*sizeof(ulong) << " Bytes" << endl;

		lenArray = nSamP*lgN / (8*sizeof(ulong));
		if ((nSamP*lgN) % (8*sizeof(ulong)))
			lenArray++;
		cout << " .- SGCPT[] " << lenArray*sizeof(ulong) << " Bytes" << endl;

		lenArray = z*lgPFT / (8*sizeof(ulong));
		if ((z*lgPFT) % (8*sizeof(ulong)))
			lenArray++;
		cout << " .- PhraFT[] " << lenArray*sizeof(ulong) << " Bytes" << endl;

		lenArray = nSamP*lgN / (8*sizeof(ulong));
		if ((nSamP*lgN) % (8*sizeof(ulong)))
			lenArray++;
		cout << " .- SGCPFT[] " << lenArray*sizeof(ulong) << " Bytes" << endl;

		if(nG){
			lenArray = nG*lgN / (8*sizeof(ulong));
			if ((nG*lgN) % (8*sizeof(ulong)))
				lenArray++;
			cout << " .- X[] " << lenArray*sizeof(ulong) << " Bytes" << endl;

			cout << " .- LOOKUP_X[]+COUNT_X[] " << 2*nSampX*sizeof(uint) << " Bytes" << endl;

			lenArray = nG*lgZ / (8*sizeof(ulong));
			if ((nG*lgZ) % (8*sizeof(ulong)))
				lenArray++;
			cout << " .- Pr[] " << lenArray*sizeof(ulong) << " Bytes" << endl;

			lenArray = nG*lgNG / (8*sizeof(ulong));
			if ((nG*lgNG) % (8*sizeof(ulong)))
				lenArray++;
			cout << " .- Src[] " << lenArray*sizeof(ulong) << " Bytes" << endl;

			cout << " .- rmqY " << rmqY->getSize() << " Bytes" << endl;
		}

		cout << "   Structures saved !" << endl;
		cout << "______________________________________________________________" << endl;
	}
}