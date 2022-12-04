#include "../../phi.hpp"

void HybridSelfIndex::locateSecondaryOccurrences(ulong l, ulong r, ulong posX, uint m, vector<vector<ulong>> &occ_tid, ulong *cont){
    ulong pos, x, y, phra, xPr, rp;
    int tid = omp_get_thread_num();

    pos = rmqY -> queryRMQ(l, r);
    x = getNum64(X, pos*lgN, lgN);
    phra = getNum64(Pr, pos*lgZ, lgZ);
    y = x + getNum64(PhraT, (phra+1)*lgPT, lgPT);

    if(y < posX+m) return;

    xPr = getPosPhraT(phra) + posX - x;
    occ_tid[tid].push_back(xPr);
    cont[tid]++;
    if(findPredecessor(xPr, &rp)){
        #pragma omp task shared(occ_tid) if(rp > 512) affinity(cont[tid])
        locateSecondaryOccurrences(0, rp, xPr, m, occ_tid, cont);
    }
    if(pos>l){
        locateSecondaryOccurrences(l, pos-1, posX, m, occ_tid, cont);
    }
    if(pos<r){
        locateSecondaryOccurrences(pos+1, r, posX, m, occ_tid, cont);
    }
}
