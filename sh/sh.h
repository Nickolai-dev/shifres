
#ifdef __cplusplus
extern "C" {
#endif
uint32_t pows(uint32_t a, uint32_t x, uint32_t p);

// shamir`s
void choose_c_d(int *c, int *d, int p);
// el-ghamal`s
int gen_k(int p);
int gen_c(int p);
int eval_d(int c, int g, int p);
int eval_r(int g, int k, int p);
int eval_e(int m, int d, int k, int p);
int eval_m(int e, int r, int c, int p);
#ifdef __cplusplus
}
#endif
