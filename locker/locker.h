

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

#include <stdint-gcc.h>

#define EOF (-1)

enum WaitMode { OREV_LOCKED=1, REV_LOCKED=2, REV_UNLOCKED=3, REV_STRT_UNLOCKED=5 };

#ifdef __cplusplus
extern "C"
{
#endif
void DLL_EXPORT init(const char* straightChannel, const char* inverseChannel);
int DLL_EXPORT readByte(); /// only from reverse channel        /// !
void DLL_EXPORT putByte(int data); /// only to straight channel /// !
void DLL_EXPORT unlock_straight_channel();                      /// theese six methods should be used with caution (inf-loop)
void DLL_EXPORT unlock_reverse_channel();                       /// !
void DLL_EXPORT lock_reverse_channel();                         /// !
void DLL_EXPORT lock_straight_channel();                        /// !
void DLL_EXPORT trylock_reverse_channel();
void DLL_EXPORT trylock_straight_channel();
#ifdef __cplusplus
}
#endif

