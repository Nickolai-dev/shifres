#include "shifres.hpp"

Diffi_Hellman::Diffi_Hellman() {
    Environment &Environment = Environment::Instance();
    hiddenKey = gen_k(Environment.p);
    sharedKey = g_in_x_mod_p(Environment.g, hiddenKey, Environment.p);
};

void Diffi_Hellman::takeSharedKey() {
    union {
        int i;
        uint8_t arr[4];
    } sh, tk;
    sh.i = sharedKey;
    for(int i = 0; i < 4; i++) {
        waitTilReady(OREV_LOCKED);
        putByte(sh.arr[i]);
        waitTilReady(REV_UNLOCKED);
        tk.arr[i] = readByte();
        unlock_reverse_channel();
        unlock_straight_channel();
    }
    takenSharedKey = tk.i;
}

void Diffi_Hellman::giveSharedKey() {
    union {
        int i;
        uint8_t arr[4];
    } sh, tk;
    sh.i = sharedKey;
    waitTilReady(REV_STRT_UNLOCKED);
    for(int i = 0; i < 4; i++) {
        putByte(sh.arr[i]);
        waitTilReady(REV_LOCKED);
        unlock_straight_channel();
        waitTilReady(REV_STRT_UNLOCKED);
        tk.arr[i] = readByte();
        unlock_reverse_channel();
    }
    takenSharedKey = tk.i;
}

inline void Diffi_Hellman::decode() {
}

inline void Diffi_Hellman::encode() {
}

void Diffi_Hellman::recipient_protocol() {
    takeSharedKey();
    try {
        while(true){
            uint8_t byte = takeByte();
            // decoding...
            write(byte);
        }
    }
    catch(FileStat) {
        write(EOF);
    }
}

void Diffi_Hellman::dispatcher_protocol() {
    giveSharedKey();
    try {
        while(true) {
            uint8_t byte = read();
            // encoding...
            sendByte(byte);
        }
    }
    catch(FileStat) {
        sendEOF();
    }
}
