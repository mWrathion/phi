#include "../phi.hpp"

void HybridSelfIndex::locate(uchar **patterns, char inputText[100], uint m, uint nPatts, uint nThreads, d_data device){
    double tt = 0;
    ulong nOccs = 0;
    ulong* occs = (ulong*) malloc(nFT*sizeof(ulong));
    omp_set_num_threads(nThreads);
    ulong *fOcc;
    for(int i = 0; i < nPatts; ++i){
        prepareOccs(device.occs,occs, nFT);
        double tp, ts;
        int nOcc = 0;
        tp = omp_get_wtime();
        locatePrimaryOccurrences(device.text, nFT, device.patterns[i], m, nOcc, device.bitVector, 
                                nSamP, lgN, POT_GC, device.SGCPFT,
                                lgPFT, device.PhraFT, device.SGCPT, lgPT, device.PhraT,device.occs, occs);
        tp = omp_get_wtime() - tp;
        ts = omp_get_wtime();
        if(nOcc > 0){
            ulong* cont_tid = new ulong[nThreads];
            vector<ulong> ps_tid(nThreads+1, 0);
            vector<vector<ulong>> occ_tid(nThreads);
            ulong cs = nOcc/nThreads;
            #pragma omp parallel shared(occs, nOcc, cs, m, occ_tid, cont_tid, ps_tid, fOcc)
            {
                int tid = omp_get_thread_num();
                cont_tid[tid] = 0;
                int begin = cs*tid;
                int end = tid == nThreads-1 ? nOcc : begin+cs;
                ulong r;

                for(int index = begin; index < end; ++index){                
                    occ_tid[tid].push_back(occs[index]);
                    cont_tid[tid]++;
                    if(findPredecessor(occs[index], &r)){
                        locateSecondaryOccurrences(0, r, occs[index], m, occ_tid, cont_tid);
                    }
                }
                #pragma omp barrier
                #pragma omp single
                {
                    for(int i = 0; i < nThreads; ++i)
                    {
                        ps_tid[i+1] = ps_tid[i] + cont_tid[i];
                    }
                    nOcc = ps_tid[nThreads];
                    fOcc = new ulong[nOcc];
                }
                for(long i = 0; i < occ_tid[tid].size(); ++i)
                {
                    fOcc[(i+ps_tid[tid])] = occ_tid[tid][i];
                }
                #pragma omp barrier
            }
        }
        ts = omp_get_wtime() - ts;
        tt += (tp + ts);
        nOccs += nOcc;
        FILE *fp = fopen("results/res.csv", "a+");
        fprintf(fp, "glocate_%d %s %d %d %lu %f %f %f\n", nThreads, inputText, m, i, nOcc, tp, ts, (float)sizeDS*8.0/(float)n);
        fclose(fp);
        //printf("glocate_%d %s %d %d %lu %f %f %f\n", nThreads, inputText, m, i, nOcc, tp, ts, (float)sizeDS*8.0/(float)n);
        if(nOcc){
            delete [] fOcc;
        }
    }
    double avg_time = (tt)/(nPatts*1.0f);
    double avg_nOccs = (nOccs/(nPatts*1.0f));
    /*double avg_tp = (tp)/(nPatts*1.0f);
    double avg_ts = (ts)/(nPatts*1.0f);*/
    cout << "\ntotal_time = " << tt*1000000.0f << " microS" << endl;
    cout << "avg_time per pattern = " << avg_time*1000000.0f << " microS" << endl;
    cout << "avg time per occurrences =" << (avg_time/avg_nOccs)*1000000.0f << " microS" << endl; 
    cout << "nOccs = " << nOccs << endl;
    cout << "avg nOccs = " << avg_nOccs << endl;
    // cout << "avg_time_prim per occurrence = " << (avg_tp/avg_nOccs)*1000000.0f << " microS" << endl;
    // cout << "avg_time_sec per occurrence = " << (avg_ts/avg_nOccs)*1000000.0f << " microS" << endl;
    cout << nFT << endl;
}
