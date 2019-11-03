#include "shifres.hpp"

El_Ghamal::El_Ghamal() {
    Environment &Environment = Environment::Instance();
    hiddenKey = gen_c(Environment.encrMaxNumber);
    sharedKey = eval_d(hiddenKey, Environment.encrSuppNumber, Environment.encrMaxNumber);
}

inline void El_Ghamal::decode(int &byte) {
    Environment &Environment = Environment::Instance();
    int r = *(&byte), e = *(&byte+1);
    byte = eval_m(e, r, hiddenKey, Environment.encrMaxNumber);
}

inline void El_Ghamal::encode(int &byte) {
    Environment &Environment = Environment::Instance();
    int k = gen_k(Environment.encrMaxNumber);
    int b = byte;
    *(&byte) = eval_r(Environment.encrSuppNumber, k, Environment.encrMaxNumber);
    *(&byte+1) = eval_e(b, takenSharedKey, k, Environment.encrMaxNumber);
}

void El_Ghamal::recipient_protocol() {
    takeSharedKey();
    while(true) {
        waitTilReady(OREV_LOCKED);
        lock_straight_channel();
        waitTilReady(REV_UNLOCKED);
        int byte[2];
        if(readBytes(byte, 8) == EOF) { write(EOF); unlock_reverse_channel(); unlock_straight_channel(); break; }
        unlock_reverse_channel();
        unlock_straight_channel();
        decode(byte[0]);
        write(byte[0]);
    }
}

void El_Ghamal::dispatcher_protocol() {
    giveSharedKey();
    try {
        while(true) {
            int byte[2] = { read(), 0 };
            encode(byte[0]);
            waitTilReady(REV_STRT_UNLOCKED);
            putBytes(byte, 8);
            waitTilReady(REV_LOCKED);
            unlock_straight_channel();
        }
    }
    catch (FileStat) {
        sendEOF();
    }
}

void El_Ghamal::takeSharedKey() {
    waitTilReady(OREV_LOCKED);
    putBytes(&sharedKey, 4);
    waitTilReady(REV_UNLOCKED);
    readBytes(&takenSharedKey, 4);
    unlock_reverse_channel();
    unlock_straight_channel();
}

void El_Ghamal::giveSharedKey() {
    waitTilReady(REV_STRT_UNLOCKED);
    putBytes(&sharedKey, 4);
    waitTilReady(REV_LOCKED);
    unlock_straight_channel();
    waitTilReady(REV_STRT_UNLOCKED);
    readBytes(&takenSharedKey, 4);
    unlock_reverse_channel();
}
