// The functions contained in this file are pretty dummy
// and are included only as a placeholder. Nevertheless,
// they *will* get included in the shared library if you
// don't remove them :)
//
// Obviously, you 'll have to write yourself the super-duper
// functions to include in the resulting library...
// Also, it's not necessary to write every function in this file.
// Feel free to add more files in this project. They will be
// included in the resulting library.

// A function adding two integers and returning the result
#include <time.h>
#include <stdlib.h>
#include <stdint-gcc.h>
int __attribute__((__stdcall__)) DllMain(int hinstDLL, int fdwReason, void* lpvReserved){
    srand(time(NULL));
    return 1;
    }


int g_pow_x_mod_p(int g, int x, int p) {
    int64_t s = g%p;
    for (; x > 1; x--) {
        s*=g;
        s%=p;
    }
    return s;
}

int reverse_c_mod_p(int c, int p) {
    #define MOV(A, B, LEN) \
    for (int I = 0; I< LEN; I++)\
        B[I] = A[I];
    int U[2] = {p, 0}, V[2] = {c%p?c:0, 1}, T[2], q;
    while (V[0] != 0) {
        q = U[0]/V[0]; T[0] = U[0]%V[0]; T[1] = U[1]-q*V[1];
        MOV(V, U, 2); MOV(T, V, 2);
    }
    while(U[1] < 0) U[1]+=p;
    return U[1];
    #undef MOV
}
// shamir`s
void choose_c_d(int *c, int *d, int p){ *c=2+rand()%(p-1-2); *d=reverse_c_mod_p(*c, p-1); if(d)return; choose_c_d(c, d, p); }

// el-ghamal`s
int gen_k(int p){ return 1+rand()%(p-2); }
int gen_c(int p){ return 2+rand()%(p-3); }
int eval_d(int c, int g, int p){ return g_pow_x_mod_p(g, c, p); }
int eval_r(int g, int k, int p){ return g_pow_x_mod_p(g, k, p); }
int eval_e(int m, int d, int k, int p){ return (int)( ((int64_t) m * (int64_t) g_pow_x_mod_p(d, k, p))%p ); }
int eval_m(int e, int r, int c, int p){ return (int)( ((int64_t) e * (int64_t) g_pow_x_mod_p(r, p-1-c, p))%p ); }
