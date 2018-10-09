#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "gmp.h"
#include "RSA.h"
#define block_size 128
#define plain_size 117

extern mpz_t p, q, n, e, d;
// EB = 00 + 02 + PS + 00 + plain   128bytes
void Padding(char *result, char *src){
    char ps[250];
    int ps_len, num;
    //生成ps_len长度的随机填充ps
    ps_len = block_size - 3 - strlen(src)/2;
    srand((unsigned)time(NULL));
    for(int i=0; i<ps_len; i++){
        num = rand() % 254 + 1;
        sprintf(ps + i*2, "%02x", num);
    }
    strcat(result, ps);
    strcat(result, "00");
    strcat(result, src);
}

//base^exp (mod n)
void exp_mod(FILE *fp, mpz_t base, mpz_t exp, mpz_t n){
    char exp_bit[1030];
    mpz_t m;
    mpz_init_set_ui(m, 1);

    mpz_get_str(exp_bit, 2, exp);
    for(int i =0; i<strlen(exp_bit); i++){
        mpz_mul(m, m, m);
        mpz_mod(m, m, n);
        if(exp_bit[i] == '1'){
            mpz_mul(m, m, base);
            mpz_mod(m, m, n);
        }
    }

    //gmp_printf("result: %Zx\n", m);
    gmp_fprintf(fp, "%Zx\n", m);
}

void Encrypt(char *plaintext, FILE *fp_cipher){
    mpz_t M;
    char plain_block[120];
    for(int i=0; i<=strlen(plaintext)/plain_size; i++){
        if(i<strlen(plaintext)/plain_size){
            strncpy(plain_block, plaintext + i*plain_size, plain_size);
            plain_block[plain_size] = '\0';
        }
        else
            strcpy(plain_block, plaintext + i*plain_size);
        char hex_plaintext[240] = "";   //明文转换得来的16进制ASCII码
        char padding_plain[260] = "";   //paddin完成后的分组
        strcat(padding_plain, "0002");
        String2Hex(plain_block, hex_plaintext);
        Padding(padding_plain, hex_plaintext);
        mpz_init_set_str(M, padding_plain, 16);
        //gmp_printf("M: %Zx\n", M);
        exp_mod(fp_cipher, M, e, n);    //加密明文M

    }
}

