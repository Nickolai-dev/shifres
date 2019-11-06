#include "shifres.hpp"

const int Caesar::key = -28;

Caesar::Caesar() {  }

inline void Caesar::decode(void* data) {
     uint8_t r = *((int*)data) - key;
     *((int*)data) = r;
}

inline void Caesar::encode(void* data) {
     uint8_t r = *((int*)data) + key;
     *((int*)data) = r;
}

Atbash::Atbash() {  }

inline void Atbash::decode(void* data) {
    uint8_t r = *((int*)data);
    *((int*)data) = 255-r;
}

inline void Atbash::encode(void* data) {
    uint8_t r = *((int*)data);
    *((int*)data) = 255-r;
}

const int Gronsfield::key[5] = { 250, 14, -6, -84, 55 };

Gronsfield::Gronsfield() {  }

inline void Gronsfield::decode(void* data) {
    static int i = 0;
    i%=5;
    uint8_t r = *((int*)data) + key[i++];
    *((int*)data) = r;
}

inline void Gronsfield::encode(void* data) {
    static int i = 0;
    i%=5;
    uint8_t r = *((int*)data) - key[i++];
    *((int*)data) = r;
}

Tablet::Tablet() {  }

const uint8_t Tablet::table[256] = { 34, 50, 37, 129, 68, 251, 159, 147, 130, 145, 55, 153, 170, 205, 217, 72, 4, 51, 244, 146, 137, 167, 41, 250, 181, 240, 122, 178, 192, 124, 189, 195, 21, 242, 112, 121, 19, 107, 82, 25, 224, 148, 39, 17, 202, 57, 52, 154, 23, 155, 143, 190, 20, 234, 0, 218, 235, 200, 97, 71, 177, 74, 201, 172, 152, 80, 33, 7, 60, 253, 81, 119, 40, 111, 193, 239, 183, 207, 27, 125, 133, 123, 219, 213, 75, 56, 59, 241, 254, 30, 61, 255, 157, 160, 236, 194, 89, 228, 106, 141, 101, 163, 67, 38, 11, 100, 73, 65, 215, 144, 247, 196, 24, 70, 16, 211, 231, 180, 149, 93, 118, 36, 222, 86, 42, 83, 126, 139, 77, 66, 95, 5, 108, 94, 88, 32, 103, 233, 249, 162, 114, 214, 165, 173, 22, 131, 151, 48, 26, 232, 128, 46, 156, 79, 150, 223, 216, 203, 227, 185, 221, 191, 140, 92, 96, 1, 225, 187, 58, 78, 2, 109, 105, 18, 120, 243, 179, 9, 209, 238, 166, 54, 136, 115, 14, 85, 208, 169, 226, 76, 15, 206, 47, 63, 220, 174, 53, 64, 245, 117, 199, 90, 138, 104, 252, 182, 210, 229, 29, 84, 158, 28, 69, 188, 43, 113, 164, 87, 12, 168, 45, 134, 197, 31, 116, 132, 230, 44, 184, 171, 10, 142, 175, 6, 98, 212, 246, 237, 161, 3, 198, 248, 110, 176, 99, 135, 186, 8, 127, 91, 62, 102, 13, 49, 35, 204 };

inline void Tablet::decode(void* data) {
    uint8_t r = *((int*)data);
    int i = -1;
    while(table[++i]!=r);
    *((int*)data) = i;
}

inline void Tablet::encode(void* data) {
    uint8_t r = *((int*)data);
    *((int*)data) = table[r];
}
