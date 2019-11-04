#include "locker.h"
#include <fstream>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp> //1_71_0    // I could place this only in .hpp?
#include <boost/random.hpp>                             //
#undef EXIT_SUCCESS
#include "sh.h"

enum Mode {RECIPIENT=1, DISPATCHER};

using namespace std;
using namespace boost::multiprecision;

class Environment {
    public:
        int encrMaxNumber, encrSuppNumber;
        string input, output;
        static Environment& Instance() { static Environment s; return s; }
    private:
        Environment() { encrMaxNumber = 1000667, encrSuppNumber = 500333; input = "input.txt", output = "output.txt"; }
        ~Environment() {  }
        Environment(Environment const&) = delete;
        Environment& operator= (Environment const&) = delete;
};

class Encoded_Structure {
    public:
        int sharedKey;
        virtual void recipient_protocol(size_t buff_size = 8);
        virtual void dispatcher_protocol(size_t buff_size = 8);
        enum class FileStat{ EXIT_SUCCESS, FAILED, UNNAMED_ERROR };
    protected:
        virtual void decode(int &byte) = 0;
        virtual void encode(int &byte) = 0;
        virtual void takeSharedKey();
        virtual void giveSharedKey();
        void waitTilReady(WaitMode m);
        uint8_t takeByte();
        uint8_t sendByte(uint8_t data);
        void sendEOF();
        void write(int byte);
        int read();
        int takenSharedKey;
};

class Diffi_Hellman : public Encoded_Structure {
    public:
        Diffi_Hellman();
        void recipient_protocol(size_t buff_size = 8) override final;
        void dispatcher_protocol(size_t buff_size = 8) override final;
        int getEvaluatedNumber() { return evaluatedNumber; };
    private:
        inline void decode(int &byte) override;
        inline void encode(int &byte) override;
        int hiddenKey, evaluatedNumber;

};

class Shamir : public Encoded_Structure {
    public:
        Shamir();
        void recipient_protocol(size_t buff_size = 8) override final;
        void dispatcher_protocol(size_t buff_size = 8) override final;
    private:
        inline void decode(int &byte) override;
        inline void encode(int &byte) override;
        int hiddenKey1, hiddenKey2;
};

class El_Ghamal : public Encoded_Structure {
    public:
        El_Ghamal();
    private:
        int hiddenKey;
        inline void decode(int &byte) override;
        inline void encode(int &byte) override;
};

class RSA : public Encoded_Structure {
    public:
        RSA();
        uint1024_t sharedModulus, sharedExponent;
    private:
        uint1024_t hiddenExponent, takenModulus, takenExponent;
        void takeSharedKey() override;
        void giveSharedKey() override;
        inline void decode(int &byte) override;
        inline void encode(int &byte) override;
        uint1024_t pows(const uint1024_t &a, const uint1024_t &x, const uint1024_t &p);
        uint1024_t mod_inverse(const uint1024_t &a, const uint1024_t &p);
        bool ferma(const uint1024_t& num);
        boost::random::mt11213b gen;
};
