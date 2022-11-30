#include "../../phi.hpp"

void HybridSelfIndex::buildBasicStructures(ulong **Source){
	ulong i, j, k, cont, posX, posXF, antX, antXF, lenS, posPar, posFT, nz, lenArray;
	bool found, prevSM;
	ulong *alpha = new ulong[SIGMA];

	for (i=0; i<SIGMA; i++)
		alpha[i] = n;

	j = k = posPar = 0;
	z = nFT = 0;
	uint maxM = 2*M;
	prevSM=false;
	for(i=0; i<orig_z; i++, j+=lgPOS, k+=lgLEN){
		posX = getNum64(ARR_POS, j, lgPOS);
		lenS = getNum64(ARR_LEN, k, lgLEN);

		if(lenS){
			posPar += lenS;
			if(lenS > maxM){
				nFT+=maxM+1;
				z++;
				prevSM=false;
			}else{
				if (prevSM==false){
					z++;
					prevSM=true;
				}
				nFT+=lenS;
			}
		}else{
			posPar++;
			nFT++;
			if (prevSM==false){
				z++;
				prevSM=true;
			}
		}
	}
	if(n != posPar){
		cout << "ERROR! n = " << n << " != final position of the parser = " << posPar << endl;
		exit(1);
	}
	z++;
	this->lgZ = 1 + (uint)(log(z)/log(2));
	if (TRACE)
		cout << "Length of the filtered text fTxt, nFT = " << nFT << endl;

	bit_vector BL(z, 0);

	// WE REPLACE ANY SIMBOL WHOSE ASCII CODE <= MIN_CHAR BY 'REPLACE_CHAR'
	Txt = new uchar[n+1];		// original text
	j = k = posPar = nz = 0;
	for(i=0; i<orig_z; i++, j+=lgPOS, k+=lgLEN){
		posX = getNum64(ARR_POS, j, lgPOS);
		lenS = getNum64(ARR_LEN, k, lgLEN);

		if(lenS){
			if(lenS == 1){
				if (Txt[posX] <= MIN_CHAR)
					Txt[posPar] = REPLACE_CHAR;
				else
					Txt[posPar] = Txt[posX];
				posPar++;
			}else{
				for(cont=0; cont<lenS; cont++){
					if (Txt[posX+cont] <= MIN_CHAR)
						Txt[posPar] = REPLACE_CHAR;
					else
						Txt[posPar] = Txt[posX+cont];
					posPar++;
				}
			}
		}else{
			if (posX <= MIN_CHAR)
				Txt[posPar] = REPLACE_CHAR;
			else
				Txt[posPar] = (uchar)posX;
			posPar++;
			if (i+1 > orig_z){
				cout << "ERROR.. phrase = " << i+1 << " > orig_z = " << orig_z << endl;
				exit(1);
			}
		}
	}
	Txt[n] = 0;

	// look for the best Mismatch symbol
	found = false;
	for (i=0; i<n; i++){
		if (alpha[Txt[i]] > i)
			alpha[Txt[i]] = i;
	}
	if (alpha['#'] == n){
		// first preference, symbol '#'
		fSymb = '#';
		found = true;
	}else{
		// second preference, symbol '$'
		if (alpha['$'] == n){
			fSymb = '$';
			found = true;
		}else{
			// third preference, a symbol in the ASCII range [32..126]
			found = false;
			for (i=32; (i<=126) && (found==false); i++){
				if (alpha[i] == n){
					found = true;
					fSymb = (uchar)i;
				}
			}

			// Ford preference, a symbol in the ASCII range [128..254]
			for (i=128; (i<=254) && (found==false); i++){
				if (alpha[i] == n){
					found = true;
					fSymb = (uchar)i;
				}
			}

			// Last preference, a symbol in the ASCII range [2..31]
			for (i=31; i && found==false; i--){
				if (alpha[i] == n){
					found = true;
					fSymb = (uchar)i;
				}
			}
		}
	}
	if (!found){
		cout << "Symbol separator not found !!" << endl;
		exit(0);
	}

	if (TRACE && false){
		cout << "Original orig_z = " << orig_z << ", new z = " << z << ", lgZ = " << lgZ << endl;
		cout << "Separator symbol: code = " << (uint)fSymb << ", symbol = " << fSymb << endl;
		cout << endl << ".- T[] = " << endl << Txt << endl;
		cout << "Alphabet: " << endl;
		for (i=0; i<SIGMA; i++)
			cout << alpha[i] << " ";
		cout << endl;
	}
	delete [] alpha;

	// Build the filtered text fTxt[1..nFT] from the original parsing
	ulong *AUX_PhraT, *AUX_PhraFT;
	lenArray = z*lgN / (8*sizeof(ulong));
	if ((z*lgN) % (8*sizeof(ulong)))
		lenArray++;
	AUX_PhraT = new ulong[lenArray];
	AUX_PhraFT = new ulong[lenArray];
	*Source = new ulong[lenArray];

	if (nFT){
		prevSM = false;
		fTxt = new uchar[nFT+1];
		j = k = posPar = posFT = nz = 0;
		for(i=0; i<orig_z; i++, j+=lgPOS, k+=lgLEN){
			posX = getNum64(ARR_POS, j, lgPOS);
			lenS = getNum64(ARR_LEN, k, lgLEN);

			if(lenS){
				if(lenS == 1){
					if (prevSM==false){
						setNum64(*Source, nz*lgN, lgN, posX);
						setNum64(AUX_PhraT, nz*lgN, lgN, posPar);
						setNum64(AUX_PhraFT, nz*lgN, lgN, posFT);
						BL[nz]=1;
						nz++;
						prevSM = true;
					}

					if (Txt[posX] > MIN_CHAR)
						fTxt[posFT] = Txt[posX];
					posPar++;
					posFT++;
				}else{
					if(lenS<=maxM){
						if (prevSM==false){
							setNum64(*Source, nz*lgN, lgN, posX);
							setNum64(AUX_PhraT, nz*lgN, lgN, posPar);
							setNum64(AUX_PhraFT, nz*lgN, lgN, posFT);
							BL[nz]=1;
							nz++;
							prevSM = true;
						}

						for(cont=0; cont<lenS; cont++){
							if (Txt[posX+cont] <= MIN_CHAR)
								fTxt[posFT] = REPLACE_CHAR;
							else
								fTxt[posFT] = Txt[posX+cont];
							posFT++;
						}
					}else{
						setNum64(*Source, nz*lgN, lgN, posX);
						setNum64(AUX_PhraT, nz*lgN, lgN, posPar);
						setNum64(AUX_PhraFT, nz*lgN, lgN, posFT);
						nz++;
						prevSM = false;

						for(cont=0; cont<M; cont++){			// copy M symbol + '$' + M symbols...
							if (Txt[posX+cont] <= MIN_CHAR)
								fTxt[posFT] = REPLACE_CHAR;
							else
								fTxt[posFT] = Txt[posX+cont];
							posFT++;
						}

						fTxt[posFT] = fSymb;
						posFT++;

						for(cont=lenS-M; cont<lenS; cont++){
							if (Txt[posX+cont] <= MIN_CHAR)
								fTxt[posFT] = REPLACE_CHAR;
							else
								fTxt[posFT] = Txt[posX+cont];
							posFT++;
						}
					}
					posPar+=lenS;
				}
			}else{
				if (prevSM==false){
					setNum64(*Source, nz*lgN, lgN, posPar);
					setNum64(AUX_PhraT, nz*lgN, lgN, posPar);
					setNum64(AUX_PhraFT, nz*lgN, lgN, posFT);
					BL[nz]=1;
					nz++;
					prevSM = true;
				}

				if (posX <= MIN_CHAR)
					fTxt[posFT] = REPLACE_CHAR;
				else
					fTxt[posFT]= (uchar)posX;
				posPar++;
				posFT++;
			}
		}

		fTxt[nFT] = 0;
		setNum64(*Source, nz*lgN, lgN, n-1);
		setNum64(AUX_PhraFT, nz*lgN, lgN, nFT);
		setNum64(AUX_PhraT, nz*lgN, lgN, n);
	}

	BL_il = bit_vector_il<512>(BL);
	rankBL = bit_vector_il<512>::rank_1_type(&BL_il);
	sizeDS += size_in_bytes(BL_il);

	// To Create the FMI and FMI_TEST
	//buildFMI();

	/*cout << "AUX_PhraT = " << endl;
	for(i=z-20; i<z; i++)
		cout << getNum64(AUX_PhraT, i*lgN, lgN) << " ";
	cout << endl;
	cout << "BL" << endl;
	for(i=z-10; i<z; i++)
		cout << BL_il[i];
	cout << endl;
	cout << endl << ".- fTxt[] = " << endl << fTxt << endl;
	exit(0);*/
	cout << "Length of the Filtered text, posFT = " << posFT << endl;
	string theFilTxt = (char *)fTxt;
	if (TRACE){
		cout << endl << ".- fTxt[] = " << endl << fTxt << endl;

		cout << endl << ".- Txt[] = " << endl << Txt << endl;

		cout << "AUX_PhraT = " << endl;
		for(i=k=0; i<z; i++, k+=lgN)
			cout << getNum64(AUX_PhraT, k, lgN) << " ";
		cout << endl;

		cout << "AUX_PhraFT = " << endl;
		for(i=k=0; i<z; i++, k+=lgN)
			cout << getNum64(AUX_PhraFT, k, lgN) << " ";
		cout << endl;

		cout << "Source = " << endl;
		for(i=k=0; i<z; i++, k+=lgN)
			cout << getNum64(*Source, k, lgN) << " ";
		cout << endl;

		cout << "BL" << endl;
		for(i=0; i<z; i++)
			cout << BL_il[i];
		cout << endl;

		cout << "rankBL = " << endl;
		for(i=1; i<=z; i++)
			cout << rankBL.rank(i);
		cout << endl;
	}

	delete [] Txt;

	//===========================================================================================================================
	this->nG = z;
	for(i=0; i<z; i++){
		if (BL[i])
			nG--;
	}
	lgNG = 1 + (uint)(log(nG)/log(2));
	nSamP = z/SAMPGC;
	if (z % SAMPGC)
		nSamP++;

	ulong maxLenT, maxLenFT;
	maxLenT = maxLenFT = 0;
	for (i=1; i<z; i++){
		posX = getNum64(AUX_PhraT, i*lgN, lgN) - getNum64(AUX_PhraT, (i-1)*lgN, lgN);
		if (posX > maxLenT)
			maxLenT = posX;

		posXF = getNum64(AUX_PhraFT, i*lgN, lgN) - getNum64(AUX_PhraFT, (i-1)*lgN, lgN);
		if (posXF > maxLenFT)
			maxLenFT = posXF;
	}
	lgPT = 1 + (uint)(log(maxLenT)/log(2));
	lgPFT = 1 + (uint)(log(maxLenFT)/log(2));

	lenArray = z*lgPT / (8*sizeof(ulong));
	if ((z*lgPT) % (8*sizeof(ulong)))
		lenArray++;
	PhraT = new ulong[lenArray];
	sizeDS += lenArray*sizeof(ulong);

	lenArray = z*lgPFT / (8*sizeof(ulong));
	if ((z*lgPFT) % (8*sizeof(ulong)))
		lenArray++;
	PhraFT = new ulong[lenArray];
	sizeDS += lenArray*sizeof(ulong);

	lenArray = nSamP*lgN / (8*sizeof(ulong));
	if ((nSamP*lgN) % (8*sizeof(ulong)))
		lenArray++;
	SGCPT = new ulong[lenArray];
	SGCPFT = new ulong[lenArray];
	sizeDS += 2*lenArray*sizeof(ulong);

	antX = getNum64(AUX_PhraT, 0, lgN);
	antXF = getNum64(AUX_PhraFT, 0, lgN);
	for (i=k=0; i<z; i++){
		posX = getNum64(AUX_PhraT, i*lgN, lgN);
		setNum64(PhraT, i*lgPT, lgPT, posX-antX);
		antX = posX;

		posXF = getNum64(AUX_PhraFT, i*lgN, lgN);
		setNum64(PhraFT, i*lgPFT, lgPFT, posXF-antXF);
		antXF = posXF;

		if (i%SAMPGC == 0){
			setNum64(SGCPT, k, lgN, antX);
			setNum64(SGCPFT, k, lgN, antXF);
			k += lgN;
		}
	}
	delete [] AUX_PhraT;
	delete [] AUX_PhraFT;

	cout << "Length of the grid, nG = " << nG << ", lgNG = " << lgNG << endl;
	if (SHOW_SIZE){
		cout << " ** size of BL_il " << size_in_bytes(BL_il) << " Bytes = " << size_in_bytes(BL_il)*8.0/(float)this->n << " bpc" << endl;

		lenArray = z*lgPT / (8*sizeof(ulong));
		if ((z*lgPT) % (8*sizeof(ulong)))
			lenArray++;
		cout << " ** size of PhraT[0..z] " << lenArray*sizeof(ulong) << " Bytes = " << lenArray*sizeof(ulong)*8.0/(float)this->n << " bpc" << endl;

		lenArray = z*lgPFT / (8*sizeof(ulong));
		if ((z*lgPFT) % (8*sizeof(ulong)))
			lenArray++;
		cout << " ** size of PhraFT[0..z] " << lenArray*sizeof(ulong) << " Bytes = " << lenArray*sizeof(ulong)*8.0/(float)this->n << " bpc" << endl;

		lenArray = nSamP*lgN / (8*sizeof(ulong));
			if ((nSamP*lgN) % (8*sizeof(ulong)))
				lenArray++;
		cout << " ** size of table SGCPT and SGCPFT " << 2*lenArray*sizeof(ulong) << " Bytes = " << 2*lenArray*sizeof(ulong)*8.0/(float)this->n << " bpc" << endl;
	}

	if (TRACE){
		cout << "lgPT = " << lgPT << ", lgPFT = " << lgPFT << endl;
		cout << "# of sampling: nSamP = " << nSamP << endl;

		cout << "PhraT[0.." << z << "] = " << endl;
		for(i=k=0; i<z; i++, k+=lgPT)
			cout << getNum64(PhraT, k, lgPT) << " ";
		cout << endl;

		cout << "PhraFT[0.." << z << "] = " << endl;
		for(i=k=0; i<z; i++, k+=lgPFT)
			cout << getNum64(PhraFT, k, lgPFT) << " ";
		cout << endl;

		cout << "SGCPT[0.." << nSamP-1 << "] = " << endl;
		for(i=k=0; i<nSamP; i++, k+=lgN)
			cout << getNum64(SGCPT, k, lgN) << " ";
		cout << endl;

		cout << "SGCPFT[0.." << nSamP-1 << "] = " << endl;
		for(i=k=0; i<nSamP; i++, k+=lgN)
			cout << getNum64(SGCPFT, k, lgN) << " ";
		cout << endl;
	}
}