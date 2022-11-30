#include "../phi.hpp"

// return in '*pred' the greater position pos, such as X[pos] <= x. If x<A[0] then we return false
bool HybridSelfIndex::findPredecessor(ulong x, ulong *pred){
	if (x < getNum64(X, 0, lgN))
		return false;

	//if (x==17907990)
	//	cout<<"";

	uint r, j, pos = x>>b_X;

	if (pos>=nSampX){
		for (r=nG-1, j=r*lgN; getNum64(X, j, lgN) > x; j-=lgN)
			r--;
		*pred = r;
		return true;
	}

	uint c = COUNT_X[pos];
	if (c > N_SCAN){
		// Binary searching in the X-segment...
		ulong u,v;
		uint m, l = LOOKUP_X[pos];

		r = l+c-1;
		m = r>>1;
		if (l) l--;
		while (l<=r){
			u = getNum64(X, m*lgN, lgN);
			if (u<=x){
				if (m<r){
					m++;
					v = getNum64(X, m*lgN, lgN);
					if (v>x){
						*pred = m-1;
						return true;
					}else{
						l=m;
						m=(l+r)>>1;
					}
				}else{
					*pred = m;
					return true;
				}
			}else{
				if (l<m){
					m--;
					v = getNum64(X, m*lgN, lgN);
					if (v<=x){
						*pred = m;
						return true;
					}else{
						r=m;
						m=(l+r)>>1;
					}
				}else
					return false;
			}
		}

		return false;
	}else{
		// Scanning a X-segment of maximum c cells...

		r = LOOKUP_X[pos]+c-1;
		for (j=r*lgN; getNum64(X, j, lgN) > x; j-=lgN)
			r--;
		*pred = r;
	}
	return true;
}