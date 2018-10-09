#include "gmp.h"
void Sieve(FILE *fp);
void test();
void key_generate(FILE*);
void String2Hex(char *src, char *dst);
void Hex2String(char *src, char *dst);
void exp_mod(FILE *fp, mpz_t base, mpz_t exp, mpz_t n);
void Encrypt(char *plaintext, FILE *fp_cipher);
void Decrypt(FILE *fp_cipher, FILE *fp_plain);

mpz_t p, q, n, e, d;
