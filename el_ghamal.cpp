#include "shifres.hpp"
#include <time.h>

El_Ghamal::El_Ghamal() {
    Environment &Environment = Environment::Instance();
    boost::random::uniform_int_distribution<int> rand(1000, Environment.encrMaxNumber-2-1000+time(NULL)%1000);
    hiddenKey = rand(Environment.gen);
    sharedKey = Environment.pows(Environment.encrSuppNumber, hiddenKey, Environment.encrMaxNumber);
}

inline void El_Ghamal::decode(void* data) {
    Environment &Environment = Environment::Instance();
    int r = *((int*)data), e = *((int*)data+1);
    *((int*)data) = ( (uint64_t)e*(uint64_t)Environment.pows(r, Environment.encrMaxNumber-1-hiddenKey, Environment.encrMaxNumber) )%Environment.encrMaxNumber;
}

inline void El_Ghamal::encode(void* data) {
    Environment &Environment = Environment::Instance();
    boost::random::uniform_int_distribution<int> rand(1000, Environment.encrMaxNumber-2-1000+time(NULL)%1000);
    int k = rand(Environment.gen);
    int b = *((int*)data);
    *((int*)data) = Environment.pows(Environment.encrSuppNumber, k, Environment.encrMaxNumber);
    *((int*)data+1) = ((int64_t)b*(int64_t)Environment.pows(takenSharedKey, k, Environment.encrMaxNumber))%Environment.encrMaxNumber;
}
