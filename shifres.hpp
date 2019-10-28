#include "sh.h"
#include "locker.h"
#include <fstream>
#include <iostream>
#ifdef EXIT_SUCCESS
#   undef EXIT_SUCCESS
#endif

enum Mode {RECIPIENT=1, DISPATCHER};

using namespace std;

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
        virtual void decode();// = 0;
        virtual void encode();// = 0;
        virtual void takeSharedKey();// = 0;
        virtual void giveSharedKey();// = 0;
        void waitTilReady(WaitMode m);
        uint8_t takeByte();
        void sendByte(uint8_t data);
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
    protected:
        void takeSharedKey() override final;
        void giveSharedKey() override final;
        void decode() override final;
        void encode() override final;
    private:
        int hiddenKey, takenSharedKey, evaluatedNumber;

};

class Shamir : public Encoded_Structure {
    public:
        void recipient_protocol() override final{};
        void dispatcher_protocol() override final{};
};

class El_Ghamal : public Encoded_Structure {
    public:
        void recipient_protocol() override final{};
        void dispatcher_protocol() override final{};
};

class RSA : public Encoded_Structure {
    public:
        void recipient_protocol() override final{};
        void dispatcher_protocol() override final{};
};

