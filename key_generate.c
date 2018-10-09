#include <stdio.h>
#include <time.h>
#include "gmp.h"
#include "RSA.h"
#define p_len 510
#define q_len 514
extern mpz_t p, q, n, e, d;
void key_generate(FILE *fp){
//���岢��ʼ��p,q,n,d
//mpz_t p, q, n, e, d;
mpz_init(p);
mpz_init(q);
mpz_init(n);
mpz_init(e);
mpz_init(d);
//���������������
gmp_randstate_t grt;
gmp_randinit_default(grt);
gmp_randseed_ui(grt, time(NULL));   //�������������Ϊ��ǰʱ��

//�������key_lenλ�Ĵ������p,q
mpz_t min;
mpz_init_set_ui(min, 2);
mpz_pow_ui(min, min, p_len-1);
while(mpz_cmp(p, min) < 0)
    mpz_urandomb(p, grt, p_len);
mpz_init_set_ui(min, 2);
mpz_pow_ui(min, min, q_len-1);
while(mpz_cmp(q, min) < 0)
    mpz_urandomb(q, grt, q_len);
//���p,qΪ�������1
if(mpz_even_p(p))
    mpz_add_ui(p, p, 1);
if(mpz_even_p(q))
    mpz_add_ui(q, q, 1);
//�������p��������Ƿ�Ϊ������ʹ��GMP�е�������⺯�����˺����ȶ�p���Գ�����������-�������Լ�ⷨ���
while(mpz_probab_prime_p(p, 20) <= 0)
    mpz_add_ui(p, p, 2);
while(mpz_probab_prime_p(q, 20) <= 0)
    mpz_add_ui(q, q, 2);
//�����ɵ�p,q������ı�
gmp_fprintf(fp, "p: %Zx\n", p);
gmp_fprintf(fp, "q: %Zx\n", q);

//����n
mpz_mul(n, p, q);
gmp_fprintf(fp, "n: %Zx\n", n);
//ȡeΪ65537
mpz_set_ui(e, 65537);
//����d
mpz_t t, p2, q2;
mpz_init(t);
mpz_init(p2);
mpz_init(q2);
mpz_sub_ui(p2, p, 1);
mpz_sub_ui(q2, q, 1);
mpz_mul(t, p2, q2);
mpz_invert(d, e, t);

//�����˽Կ
gmp_fprintf(fp, "The public key (n, e) is (%Zx, %Zx)\n", n, e);
gmp_fprintf(fp, "The private key (n, d) is (%Zx, %Zx)\n", n, d);
}
