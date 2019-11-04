#include "shifres.hpp"

Diffi_Hellman::Diffi_Hellman() {
    Environment &Environment = Environment::Instance();
    hiddenKey = gen_k(Environment.encrMaxNumber);
    sharedKey = pows(Environment.encrSuppNumber, hiddenKey, Environment.encrMaxNumber);
};

inline void Diffi_Hellman::decode(int &byte) {  }

inline void Diffi_Hellman::encode(int &byte) {  }

void Diffi_Hellman::recipient_protocol() {
    Environment &Environment = Environment::Instance();
    takeSharedKey();
    evaluatedNumber = pows(takenSharedKey, hiddenKey, Environment.encrMaxNumber);
}

void Diffi_Hellman::dispatcher_protocol() {
    Environment &Environment = Environment::Instance();
    giveSharedKey();
    evaluatedNumber = pows(takenSharedKey, hiddenKey, Environment.encrMaxNumber);
}
