#include "locker.h"

#include <stdio.h>
#include <share.h>
#include <string.h>
#include <errno.h>

static FILE *stFile, *invFile;

static char straightFilename[40] = {0}, inverseFilename[40] = {0};

void DLL_EXPORT init(const char* straight, const char* inverse) {
    int i = 0;
    while(straight[i++]);
    memcpy(straightFilename, straight, i);
    i = 0;
    while(inverse[i++]);
    memcpy(inverseFilename, inverse, i);
    fclose(fopen(straightFilename, "wb")), fclose(fopen(inverseFilename, "wb"));
}

void DLL_EXPORT unlock_straight_channel() { fclose(stFile); }
void DLL_EXPORT unlock_reverse_channel() { fclose(invFile); }

void DLL_EXPORT lock_reverse_channel() {
    do {
        errno=0;
        invFile = _fsopen(inverseFilename, "rb", _SH_DENYRW);
    } while(errno!=0);
}

void DLL_EXPORT lock_straight_channel() {
    do {
        errno=0;
        stFile = _fsopen(straightFilename, "wb", _SH_DENYRW);
    } while(errno!=0);
}

void DLL_EXPORT trylock_reverse_channel() {
    FILE *f = _fsopen(inverseFilename, "rb", _SH_DENYNO);
    if(f) fclose(f), errno=0; /// anyway, fclose locks file
    else errno=EACCES;
}

void DLL_EXPORT trylock_straight_channel() {
    FILE *f = _fsopen(straightFilename, "rb", _SH_DENYNO);
    if(f) fclose(f), errno=0;
    else errno=EACCES;
}

int DLL_EXPORT readByte() {
    do{ errno=0;
    invFile = _fsopen(inverseFilename, "rb", _SH_DENYRW);
    }while(errno!=0);
    int n = fgetc(invFile);
    fgetc(invFile);
    return feof(invFile) ? n : EOF;
}
int DLL_EXPORT readBytes(void *bytes, int cnt) {
    do{ errno=0;
    invFile = _fsopen(inverseFilename, "rb", _SH_DENYRW);
    }while(errno!=0);
    for(int i = cnt; i > 0; i--) {
        int n = fgetc(invFile);
        if(feof(invFile)) return EOF;
        *((uint8_t*)bytes+i) = n;
    }
    return 0;
}
void DLL_EXPORT putByte(int data) {
    do{ errno=0;
    stFile = _fsopen(straightFilename, "wb", _SH_DENYRW);
    }while(errno!=0);
    if(0 <= data && data < 256)
        fputc(data, stFile);
    else if (data == EOF) {
        fputc(0xff, stFile);
        fputc(0x01, stFile);
    }
}
void DLL_EXPORT putBytes(void *bytes, int cnt) {
    do{ errno=0;
    stFile = _fsopen(straightFilename, "wb", _SH_DENYRW);
    }while(errno!=0);
    for(int i = cnt; i > 0; i--) {
        fputc( *((uint8_t*)bytes+i), stFile);
    }
}
int __attribute__((__stdcall__)) DllMainCRTStartup(int hinstDLL, int fdwReason, void* lpvReserved){
    if(fdwReason == 0) remove(inverseFilename), remove(straightFilename);
    return 1;
}

void __attribute__((section (".dtors"))) _Destr(){ remove(inverseFilename), remove(straightFilename);}
