#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

void String2Hex(char *src, char *dst){
    for(int i=0; i<strlen(src); i++){
        sprintf(dst+i*2,"%02x",*(src+i));
    }
}

void Hex2String(char *src, char *dst){
    int src_len, i, j, asc;
    char c[2];
    src_len = strlen(src);
    for(i = 2; i<src_len-2; i = i + 2){
        if(src[i] == '0' && src[i+1] == '0'){
            j = i + 2;
            break;
        }
    }
    for(i = j; i<src_len; i = i + 2){
        c[0] = src[i];
        c[1] = src[i+1];
        sscanf(c, "%x", &asc);
        sprintf(dst++, "%c", asc);
        asc = 0;
    }
}
