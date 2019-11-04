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
        virtual void recipient_protocol() = 0;
        virtual void dispatcher_protocol() = 0;
        enum class FileStat{ EXIT_SUCCESS, FAILED, UNNAMED_ERROR };
    protected:
        inline virtual void decode(int &byte) = 0;
        inline virtual void encode(int &byte) = 0;
        virtual void takeSharedKey() = 0;
        virtual void giveSharedKey() = 0;
        void waitTilReady(WaitMode m);
        uint8_t takeByte();
        uint8_t sendByte(uint8_t data);
        void sendEOF();
        void write(int byte);
        int read();
};

class Diffi_Hellman : public Encoded_Structure {
    public:
        int sharedKey;
        Diffi_Hellman();
        void recipient_protocol() override final;
        void dispatcher_protocol() override final;
        int getEvaluatedNumber() { return evaluatedNumber; };
    private:
        void takeSharedKey() override;
        void giveSharedKey() override;
        inline void decode(int &byte) override;
        inline void encode(int &byte) override;
        int hiddenKey, takenSharedKey, evaluatedNumber;

};

class Shamir : public Encoded_Structure {
    public:
        Shamir();
        void recipient_protocol() override final;
        void dispatcher_protocol() override final;
    private:
        void takeSharedKey() override;
        void giveSharedKey() override;
        inline void decode(int &byte) override;
        inline void encode(int &byte) override;
        int hiddenKey1, hiddenKey2;
};

class El_Ghamal : public Encoded_Structure {
    public:
        int sharedKey;
        El_Ghamal();
        void recipient_protocol() override final;
        void dispatcher_protocol() override final;
    private:
        int hiddenKey, takenSharedKey;
        void takeSharedKey() override;
        void giveSharedKey() override;
        inline void decode(int &byte) override;
        inline void encode(int &byte) override;
};

class RSA : public Encoded_Structure {
    public:
        RSA();
<<<<<<< HEAD
        uint1024_t sharedKey;
=======
        boost::multiprecision::uint1024_t sharedKey;
>>>>>>> cb7d8689c56accbfcde010718295b478123384d6
        void recipient_protocol() override final;
        void dispatcher_protocol() override final;
    private:
        void takeSharedKey() override;
        void giveSharedKey() override;
        inline void decode(int &byte) override;
        inline void encode(int &byte) override;
};
<<<<<<< HEAD

//namespace utilities {

//bool ferma(const uint1024_t &num);
uint1024_t pows(const uint1024_t &g, const uint1024_t &x, const uint1024_t &p);
//uint1024_t mul_mod(const uint1024_t &a, const uint1024_t &b, const uint1024_t &m);

//}
=======
>>>>>>> cb7d8689c56accbfcde010718295b478123384d6
