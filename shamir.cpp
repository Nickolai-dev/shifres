#include "shifres.hpp"

Shamir::Shamir() {
    //cout << "H-keys: " << hiddenKey1 << " " << hiddenKey2 << "\n";
    //int h1, h2, p = Environment::Instance().encrMaxNumber;
    choose_c_d(&hiddenKey1, &hiddenKey2, Environment::Instance().encrMaxNumber);
    /*choose_c_d(&h1, &h2, p);
    int a = 42;
    cout << "got: " << a << "\n";
    a = g_pow_x_mod_p(a, hiddenKey1, p);
    cout << "encoded: " << a << "\n";
    a = g_pow_x_mod_p(a, h1, p);
    cout << "encoded: " << a << "\n";
    a = g_pow_x_mod_p(a, hiddenKey2, p);
    cout << "decoded: " << a << "\n";
    a = g_pow_x_mod_p(a, h2, p);
    cout << "decoded: " << a << "\n";*/
}

inline void Shamir::decode(int &byte) {
    byte = g_pow_x_mod_p(byte, hiddenKey2, Environment::Instance().encrMaxNumber);
}

inline void Shamir::encode(int &byte) {
    byte = g_pow_x_mod_p(byte, hiddenKey1, Environment::Instance().encrMaxNumber);
}

void Shamir::recipient_protocol() {
    takeSharedKey();

    while(true){
        waitTilReady(OREV_LOCKED);
        lock_straight_channel();
        waitTilReady(REV_UNLOCKED);
        int byte;
        if(readBytes(&byte, 4) == EOF) { write(EOF); break; }
        cout << "got: " << byte << "\n";
        encode(byte);
        cout << "encoded: " << byte << "\n";
        unlock_straight_channel();
        putBytes(&byte, 4);
        unlock_reverse_channel();
        unlock_straight_channel();
        waitTilReady(REV_LOCKED);
        lock_straight_channel();
        waitTilReady(REV_UNLOCKED);
        readBytes(&byte, 4);
        cout << "got: " << byte << "\n";
        unlock_reverse_channel();
        unlock_straight_channel();
        decode(byte);
        cout << "decoded: " << byte << "\n";
        //if(byte<0||byte>255) throw std::runtime_error("Byte is out of range!\n");
        write(byte);
        //system("pause");
    }
}

void Shamir::dispatcher_protocol() {
    giveSharedKey();
    try {
        while(true) {
            int byte = read();
            cout << "read: " << byte << "\n";
            encode(byte);
            cout << "encoded: " << byte << "\n";
            waitTilReady(REV_STRT_UNLOCKED);
            putBytes(&byte, 4);
            waitTilReady(REV_LOCKED);
            unlock_straight_channel();
            waitTilReady(REV_STRT_UNLOCKED);
            int answer;
            readBytes(&answer, 4);
            cout << "answer: " << answer << "\n";
            decode(answer);
            cout << "decoded: " << answer << "\n";
            unlock_reverse_channel();
            waitTilReady(REV_STRT_UNLOCKED); // as a precaution
            putBytes(&answer, 4);
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
