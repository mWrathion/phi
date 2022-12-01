#include "../phi.hpp"

void HybridSelfIndex::locate(uchar **patterns, uint m, uint nPatts, uint nThreads, d_data device){
    double tt = 0;
    ulong nOccs = 0;
    for(int i = 0; i < nPatts; ++i){
        double t;
        t = omp_get_wtime();
        int nOcc = 0;
        locatePrimaryOccurrences(device.text, nFT, device.patterns[i], m, nOcc, device.bitVector, 
                                nSamP, lgN, POT_GC, device.SGCPFT,
                                lgPFT, device.PhraFT);
        t = omp_get_wtime() - t;
        tt += t/(nPatts*1.0f);
        nOccs += nOcc;
        cout << nOcc << "  " << t <<endl;
    }
    cout << "avg_time = " << tt*1000.0f << " microS" << endl;
    cout << "tt = " << (tt*1000.f)/(nOccs*1.0f) << " microS" << endl; 
    cout << "nOccs = " << nOccs << endl;
}