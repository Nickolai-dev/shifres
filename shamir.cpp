#include "shifres.hpp"

Shamir::Shamir() {
    choose_c_d(&hiddenKey1, &hiddenKey2, Environment::Instance().encrMaxNumber);
    hiddenKey1 = 7;
    hiddenKey2 = 3;
    cout << "H-keys: " << hiddenKey1 << " " << hiddenKey2 << "\n";
    int h1 = 8, h2 = 9, p = 11;//Environment::Instance().encrMaxNumber;
    //choose_c_d(&h1, &h2, p);
    int a = 8;
    cout << "got: " << a << "\n";
    a = g_pow_x_mod_p(a, hiddenKey1, p);
    cout << "encoded: " << a << "\n";
    a = g_pow_x_mod_p(a, h1, p);
    cout << "encoded: " << a << "\n";
    a = g_pow_x_mod_p(a, hiddenKey2, p);
    cout << "decoded: " << a << "\n";
    a = g_pow_x_mod_p(a, h2, p);
    cout << "decoded: " << a << "\n";
}

inline void Shamir::decode(uint8_t &byte) {
    static Environment &Env = Environment::Instance();
    byte = g_pow_x_mod_p(byte, hiddenKey2, Env.encrMaxNumber);
}

inline void Shamir::encode(uint8_t &byte) {
    static Environment &Env = Environment::Instance();
    byte = g_pow_x_mod_p(byte, hiddenKey1, Env.encrMaxNumber);
}

void Shamir::recipient_protocol() {
    takeSharedKey();

    while(true){
        waitTilReady(OREV_LOCKED);
        lock_straight_channel();
        waitTilReady(REV_UNLOCKED);
        int b = readByte();
        if(b == EOF) { write(EOF); break; }
        uint8_t byte = b;
        cout << "got: " << byte << "\n";
        encode(byte);
        cout << "encoded: " << byte << "\n";
        unlock_straight_channel();
        putByte(byte);
        unlock_reverse_channel();
        unlock_straight_channel();
        waitTilReady(REV_LOCKED);
        lock_straight_channel();
        waitTilReady(REV_UNLOCKED);
        byte = readByte();
        cout << "got: " << byte << "\n";
        unlock_reverse_channel();
        unlock_straight_channel();
        decode(byte);
        cout << "decoded: " << byte << "\n";
        write(byte);
        system("pause");
    }
}

void Shamir::dispatcher_protocol() {
    giveSharedKey();
    try {
        while(true) {
            uint8_t byte = read();
            cout << "read: " << byte << "\n";
            encode(byte);
            cout << "encoded: " << byte << "\n";
            waitTilReady(REV_STRT_UNLOCKED);
            putByte(byte);
            waitTilReady(REV_LOCKED);
            unlock_straight_channel();
            waitTilReady(REV_STRT_UNLOCKED);
            uint8_t answer = readByte();
            cout << "answer: " << byte << "\n";
            decode(answer);
            cout << "decoded: " << byte << "\n";
            unlock_reverse_channel();
            waitTilReady(REV_STRT_UNLOCKED); // as a precaution
            putByte(answer);
            waitTilReady(REV_LOCKED);
            unlock_straight_channel();
            system("pause");
        }
    } catch(FileStat) {
        sendEOF();
    }
}

void Shamir::takeSharedKey() {

}

void Shamir::giveSharedKey() {

}
