#include "../../phi.hpp"

bool* HybridSelfIndex::bitVectorToArray(){
    bool* bitVector = (bool *)malloc(z*sizeof(bool));
    for(int i = 0; i< z; ++i){
        bitVector[i] = BL_il[i];
    }
    return bitVector;
}