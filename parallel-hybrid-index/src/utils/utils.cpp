#include "../phi.hpp"

bool HybridSelfIndex::TRACE;
bool HybridSelfIndex::SHOW_SIZE;
bool HybridSelfIndex::CREATE_FMI_TEST;

uint HybridSelfIndex::SAMPGC = SAMP_GC;
uint HybridSelfIndex::POT_GC = log(SAMP_GC)/log(2);

uchar HybridSelfIndex::MIN_CHAR = '\1';
uchar HybridSelfIndex::REPLACE_CHAR = '\n';
