#include "shifres.hpp"
//#define DEB

#ifdef DEB
#define LOG(N) cout << N;
#else
#define LOG(N)
#endif

void Encoded_Structure::waitTilReady(WaitMode m) {
    switch(m) {
        case REV_STRT_UNLOCKED: {
            do {
                    LOG(1);
                trylock_reverse_channel();
            } while(errno != 0);
            do {
                    LOG(2);
                trylock_straight_channel();
            } while(errno != 0);
            break;
        }
        case OREV_LOCKED: {
            do {
                    LOG(3);
                trylock_reverse_channel();
            } while(errno != EACCES);
            do {
                    LOG(4);
                trylock_straight_channel();
            } while(errno != 0);
            break;
        }
        case REV_LOCKED: {
            do {
                    LOG(5);
                trylock_reverse_channel();
            } while(errno != EACCES);
            break;
        }
        case REV_UNLOCKED: {
            do {
                    LOG(6);
                trylock_reverse_channel();
            } while(errno != 0);
            break;
        }
    }
}

int Encoded_Structure::read() {
    Environment &Environment = Environment::Instance();
    static ifstream file(Environment.input, ios::binary|ios::in);
    uint8_t data;
    file.get((char&)data);
    if(file.eof()) {
        file.close();
        throw FileStat::EXIT_SUCCESS;
    }
    return data;
}

void Encoded_Structure::write(int data) {
    Environment &Environment = Environment::Instance();
    static ofstream file(Environment.output, ios::binary|ios::out);
    if (data == EOF) {
        file.close();
        return;
    }
    file << static_cast<uint8_t>(data);
}

uint8_t Encoded_Structure::takeByte() { /// deprecated, dye to the impossibility of mixed use with sendByte
    waitTilReady(OREV_LOCKED);
    lock_straight_channel();
    waitTilReady(REV_UNLOCKED);
    int data;
    data = readByte();
    unlock_reverse_channel();
    unlock_straight_channel();
    if (data == EOF)
        throw FileStat::EXIT_SUCCESS;
    return data;
}

uint8_t Encoded_Structure::sendByte(uint8_t data) { /// deprecated, dye to the impossibility of mixed use with takeByte
    // not turned out to write nice abstraction of transmission protocol
    waitTilReady(REV_STRT_UNLOCKED);
    putByte(data);
    waitTilReady(REV_LOCKED);
    unlock_straight_channel();
    waitTilReady(REV_STRT_UNLOCKED);
    uint8_t answer = readByte();
    unlock_reverse_channel();
    return answer;
}

void Encoded_Structure::sendEOF() {
    waitTilReady(REV_STRT_UNLOCKED);
    putByte(EOF);
    waitTilReady(REV_LOCKED);
    unlock_straight_channel();
}

void Encoded_Structure::recipient_protocol() {
    int sz = 1;
    Encoded_Structure* r = dynamic_cast<RSA*>(this);
    if(r)sz = 34;
    r = dynamic_cast<El_Ghamal*>(this);
    if(r)sz = 2;
    takeSharedKey();
    while(true) {
        waitTilReady(OREV_LOCKED);
        lock_straight_channel();
        waitTilReady(REV_UNLOCKED);
        int byte[sz] = {0};
        if(readBytes(byte, sz*4) == EOF) { write(EOF); unlock_reverse_channel(); unlock_straight_channel(); break; }
        unlock_reverse_channel();
        unlock_straight_channel();
        decode(byte);
        write(byte[0]);
    }
}

void Encoded_Structure::dispatcher_protocol() {
    int sz = 1;
    Encoded_Structure* r = dynamic_cast<RSA*>(this);
    if(r)sz = 34;
    r = dynamic_cast<El_Ghamal*>(this);
    if(r)sz = 2;
    giveSharedKey();
    try {
        while(true) {
            int byte[sz] = {0};
            byte[0] = read();
            encode(byte);
            waitTilReady(REV_STRT_UNLOCKED);
            putBytes(byte, sz*4);
            waitTilReady(REV_LOCKED);
            unlock_straight_channel();
        }
    }
    catch (FileStat) {
        sendEOF();
    }
}

void Encoded_Structure::takeSharedKey() {
    waitTilReady(OREV_LOCKED);
    putBytes(&sharedKey, 4);
    waitTilReady(REV_UNLOCKED);
    readBytes(&takenSharedKey, 4);
    unlock_reverse_channel();
    unlock_straight_channel();
}

void Encoded_Structure::giveSharedKey() {
    waitTilReady(REV_STRT_UNLOCKED);
    putBytes(&sharedKey, 4);
    waitTilReady(REV_LOCKED);
    unlock_straight_channel();
    waitTilReady(REV_STRT_UNLOCKED);
    readBytes(&takenSharedKey, 4);
    unlock_reverse_channel();
}

uint32_t Environment::pows(uint32_t a, uint32_t x, uint32_t p) {
    uint8_t offset = 31;
    uint64_t y = 1;
    for(;;offset--) {
        y*=y;
        y%=p;
        if((x>>offset)&0x1) {
            y*=a;
            y%=p;
        }
        if(offset==0)
            break;
    }
    return y;
}
