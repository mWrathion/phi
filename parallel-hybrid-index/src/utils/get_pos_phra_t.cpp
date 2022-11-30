#include "../phi.hpp"

ulong HybridSelfIndex::getPosPhraT(ulong phra){
	ulong ph = phra>>POT_GC;
	ulong x = getNum64(SGCPT, ph*lgN, lgN);
	if (phra%SAMP_GC == 0)
		return x;

	// extract from the sampled phrase 'ph' to 'phra'...
	ph=(ph<<POT_GC)+1;
	for (ulong c = ph*lgPT; ph<=phra; ph++, c+=lgPT)
		x += getNum64(PhraT, c, lgPT);

	return x;
}