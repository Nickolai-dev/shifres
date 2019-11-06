#include "shifres.hpp"
#include <boost/integer/common_factor_rt.hpp>
#include <boost/integer/mod_inverse.hpp>
#include <time.h>

Shamir::Shamir() {  }

inline void Shamir::decode(void* data) {
    *((int*)data) = Environment::pows(*((int*)data), hiddenKey2, Environment::Instance().encrMaxNumber);
}

inline void Shamir::encode(void* data) {
    *((int*)data) = Environment::pows(*((int*)data), hiddenKey1, Environment::Instance().encrMaxNumber);
}

void Shamir::recipient_protocol() {
    takeSharedKey();
    while(true){
        choose_c_d(hiddenKey1, hiddenKey2);
        waitTilReady(OREV_LOCKED);
        lock_straight_channel();
        waitTilReady(REV_UNLOCKED);
        int byte;
        if(readBytes(&byte, 4) == EOF) { write(EOF); break; }
        encode(&byte);
        unlock_straight_channel();
        putBytes(&byte, 4);
        unlock_reverse_channel();
        unlock_straight_channel();
        waitTilReady(REV_LOCKED);
        lock_straight_channel();
        waitTilReady(REV_UNLOCKED);
        readBytes(&byte, 4);
        unlock_reverse_channel();
        unlock_straight_channel();
        decode(&byte);
        write(byte);
    }
}

void Shamir::dispatcher_protocol() {
    giveSharedKey();
    try {
        while(true) {
            choose_c_d(hiddenKey1, hiddenKey2);
            int byte = read();
            encode(&byte);
            waitTilReady(REV_STRT_UNLOCKED);
            putBytes(&byte, 4);
            waitTilReady(REV_LOCKED);
            unlock_straight_channel();
            waitTilReady(REV_STRT_UNLOCKED);
            int answer;
            readBytes(&answer, 4);
            decode(&answer);
            unlock_reverse_channel();
            waitTilReady(REV_STRT_UNLOCKED); // as a precaution
            putBytes(&answer, 4);
            waitTilReady(REV_LOCKED);
            unlock_straight_channel();
        }
    } catch(FileStat) {
        sendEOF();
    }
}

void Shamir::choose_c_d(int &c, int &d) {
    static Environment &Environment = Environment::Instance();
    boost::random::uniform_int_distribution<int> rand(1000, Environment.encrMaxNumber-3-1000+time(NULL)%1000);
    err: c = rand(Environment.gen);
    while(boost::integer::gcd_evaluator<int>()(++c, Environment.encrMaxNumber-1) != 1)
        if(c >= Environment.encrMaxNumber-1000) { c = rand(Environment.gen); }
    d = boost::integer::mod_inverse(c, Environment.encrMaxNumber-1);
    if(d==0) goto err;
}
