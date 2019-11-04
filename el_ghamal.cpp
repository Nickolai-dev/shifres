#include "shifres.hpp"

El_Ghamal::El_Ghamal() {
    Environment &Environment = Environment::Instance();
    hiddenKey = gen_c(Environment.encrMaxNumber);
    sharedKey = eval_d(hiddenKey, Environment.encrSuppNumber, Environment.encrMaxNumber);
}

inline void El_Ghamal::decode(int &byte) {
    int r = *(&byte), e = *(&byte+1);
    byte = eval_m(e, r, hiddenKey, Environment::Instance().encrMaxNumber);
}

inline void El_Ghamal::encode(int &byte) {
    Environment &Environment = Environment::Instance();
    int k = gen_k(Environment.encrMaxNumber);
    int b = byte;
    *(&byte) = eval_r(Environment.encrSuppNumber, k, Environment.encrMaxNumber);
    *(&byte+1) = eval_e(b, takenSharedKey, k, Environment.encrMaxNumber);
}
