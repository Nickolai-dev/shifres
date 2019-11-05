#include "shifres.hpp"

El_Ghamal::El_Ghamal() {
    Environment &Environment = Environment::Instance();
    hiddenKey = gen_c(Environment.encrMaxNumber);
    sharedKey = eval_d(hiddenKey, Environment.encrSuppNumber, Environment.encrMaxNumber);
}

inline void El_Ghamal::decode(void* data) {
    int r = *((int*)data), e = *((int*)data+1);
    *((int*)data) = eval_m(e, r, hiddenKey, Environment::Instance().encrMaxNumber);
}

inline void El_Ghamal::encode(void* data) {
    Environment &Environment = Environment::Instance();
    int k = gen_k(Environment.encrMaxNumber);
    int b = *((int*)data);
    *((int*)data) = eval_r(Environment.encrSuppNumber, k, Environment.encrMaxNumber);
    *((int*)data+1) = eval_e(b, takenSharedKey, k, Environment.encrMaxNumber);
}
