
#ifdef __cplusplus
extern "C" {
#endif
// shamir`s
void choose_c_d(int *c, int *d, int p);
// el-ghamal`s
int g_pow_x_mod_p(int g, int x, int p);
int gen_k(int p);
int gen_c(int p);
int eval_d(int c, int g, int p);
int eval_r(int g, int k, int p);
int eval_e(int m, int d, int k, int p);
int eval_m(int e, int r, int c, int p);
#ifdef __cplusplus
}
#endif
