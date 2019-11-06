#include "shifres.hpp"
#include <time.h>

Diffi_Hellman::Diffi_Hellman() {
    Environment &Environment = Environment::Instance();
    boost::random::uniform_int_distribution<int> rand(1000, Environment.encrMaxNumber-2-1000+time(NULL)%1000);
    hiddenKey = rand(Environment.gen);
    sharedKey = Environment.pows(Environment.encrSuppNumber, hiddenKey, Environment.encrMaxNumber);
};

inline void Diffi_Hellman::decode(void* data) {  }

inline void Diffi_Hellman::encode(void* data) {  }

void Diffi_Hellman::recipient_protocol() {
    Environment &Environment = Environment::Instance();
    takeSharedKey();
    evaluatedNumber = Environment.pows(takenSharedKey, hiddenKey, Environment.encrMaxNumber);
}

void Diffi_Hellman::dispatcher_protocol() {
    Environment &Environment = Environment::Instance();
    giveSharedKey();
    evaluatedNumber = Environment.pows(takenSharedKey, hiddenKey, Environment.encrMaxNumber);
}
