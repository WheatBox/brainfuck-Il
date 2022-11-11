// gcc bfil.c -o bfil

#include <stdio.h>
#include <string.h>

unsigned char val[1024] = {0};
unsigned char * p = val;

void BFinterpreter(char * buf, size_t size);

void BFILtoBF(char * dest, const char * src);

int main(int argc, char * argv[]) {
    const char * BFfilepath = argv[1];

    FILE * fp = fopen(BFfilepath, "r");
    if(fp == NULL) {
        printf("Can not find the File!\n");
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0 ,SEEK_SET);

    char buf[size + 1];
    fread(buf, size, 1, fp);

    char bufbf[size + 1];
    BFILtoBF(bufbf, buf);
    BFinterpreter(bufbf, size);

    fclose(fp);
    
    return 0;
}

void BFinterpreter(char * buf, size_t size) {
    for(int i = 0; i < size; i++) {
        switch(buf[i]) {
            case '>':
                p++;
                break;
            case '<':
                p--;
                break;
            case '+':
                *p = *p >= 255 ? 0 : *p + 1;
                break;
            case '-':
                *p = *p <= 0 ? 255 : *p - 1;
                break;
            case '.':
                putchar(*p);
                break;
            case ',':
                *p = getchar();
                break;
            case '[':
                i++;
                int begin = i;
                int j = 0;
                int looplv = 1;
                while(1) {
                    if(buf[i + j] == ']') {
                        looplv--;
                        if(looplv == 0) {
                            break;
                        }
                    }
                    if(buf[i + j] == '[') {
                        looplv++;
                    }
                    j++;
                }

                if(!*p) {
                    i += j;
                }
                while(*p) {
                    i = begin;
                    BFinterpreter(buf + i, j);
                    i += j;
                }
        }
    }
}

void BFILtoBF(char * dest, const char * src) {
    const char * trans[8] = { "I", "l", "|", "‖", "│", "︳", "︱", "｜" };
    const char desttrans[8] = { '>', '<', '+', '-', '.', ',', '[', ']'};

    size_t size = strlen(src);
    for(size_t idest = 0, isrc = 0; isrc < size; idest++) {
        int j = 7;

        for(; j >= 0; j--) {
            size_t charlen = ((j < 3) ? 1 : 3);

            if(strncmp(src + isrc, trans[j], charlen) == 0) {
                dest[idest] = desttrans[j];

                isrc += charlen;
                break;
            }
        }

        if(j < 0) {
            dest[idest] = src[isrc];
            isrc++;
        }
    }
}
