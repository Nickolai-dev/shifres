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
