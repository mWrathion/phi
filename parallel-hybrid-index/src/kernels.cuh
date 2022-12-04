#pragma once
#include <stdio.h>
typedef unsigned int u_int;
typedef unsigned char u_char;
typedef unsigned long u_long;

/**
 * @brief Struct that contains all data that will be stored in GPU.
 * 
 * @param text LZ77 parsed text with literal phrases (S'').
 * @param patterns Patterns to find inside S''.
 * @param bitVector BitVector that check if a phrase in S'' is a literal phrase.
 * @param SGCPFT Sampling GapCoding of PhraFT.
 * @param PhraFT Table of length z that stores the positions of S'' of each phrase boundary
 */
struct d_data
{
    u_char *text;
    u_char **patterns;
    bool *bitVector;
    ulong *SGCPFT;
    ulong *PhraFT;
    ulong *SGCPT;
    ulong *PhraT;
    u_long *occs;
};

d_data loadGPUStructures(u_char *text, u_int size, 
                         u_char** patterns, u_int m, u_int nPatt, 
                         bool* bitVector, u_int z,
                         u_long* SGCPFT, u_int nSamP, u_long* PhraFT,
                         u_long* SGCPT, u_int lgPT, u_long* PhraT);

void locatePrimaryOccurrences(u_char* text, u_int size, u_char* pattern, u_int m, int &nOcc, 
                              bool* BL_il,
                              u_int nSamP, u_int lgN, u_int POT_GC, u_long* SGCPFT, 
                              u_int lgPFT, u_long* PhraFT,
                              u_long* SGCPT, u_int lgPT, u_long* PhraT
                              , u_long* occs, u_long* h_occs);


void prepareOccs(u_long* d_occs, u_long* occs, u_int size);
