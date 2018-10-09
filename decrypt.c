#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "gmp.h"
#include "RSA.h"
extern mpz_t p, q, n, e, d;

void Decrypt(FILE *fp_cipher, FILE *fp_plain){
    //printf("Decrypt result:\n");
    char cipher[500];
    fscanf(fp_cipher, "%s", cipher);
    while(!feof(fp_cipher)){
    mpz_t C;
    mpz_init_set_str(C, cipher, 16);
    //gmp_printf("C: %Zx\n", C);
//    exp_mod(fp_cipher, C, d, n);
    //CRT MMRC
    mpz_t p1, q1, d1, d2, C1, C2, M1, M2, B, m;
    mpz_init(d1);
    mpz_init(d2);
    mpz_init(p1);
    mpz_init(q1);
    mpz_init(C1);
    mpz_init(C2);
    mpz_init(M1);
    mpz_init(M2);
    mpz_init(B);
    mpz_init(m);

    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mod(d1, d, p1);
    mpz_mod(d2, d, q1);
    mpz_mod(C1, C, p);
    mpz_mod(C2, C, q);
    mpz_powm(M1, C1, d1, p);
    mpz_powm(M2, C2, d2, q);
    mpz_invert(B, p, q);
    mpz_sub(m, M2, M1);
    mpz_mul(m, m, B);
    mpz_mod(m, m, q);
    mpz_mul(m, m, p);
    mpz_add(m, M1, m);
    //gmp_printf("result: %Zx\n", m);

    char m_hex[260] = "0";
    mpz_get_str(m_hex+1, 16, m);
    char text[120];
    Hex2String(m_hex, text);
    fprintf(fp_plain, text);
    fscanf(fp_cipher, "%s", cipher);
    }
    //printf("\n");
}
