#include "PF.h"
#include "config.h"
#include "bits.h"
#include <pic18f4520.h>
#include "lcd.h"

#define L0 0x80
#define L1 0xC0
#define L2 0x90
#define L3 0xD0
#define CLR 0x01

void lcdPosition(unsigned char linha, unsigned char coluna) {
    char aux;

    if (linha == 0)
        aux = L0 + coluna;
    if (linha == 1)
        aux = L1 + coluna;
    if (linha == 2)
        aux = L2 + coluna;
    if (linha == 3)
        aux = L3 + coluna;
    lcdCommand(aux);
}

void printf(char v[]) {
    char i, linha = 0, coluna = 0;
    for (i = 0; i < 55; i++) {
        if (v[i] == '\\' && v[i + 1] == 'n') {
            linha++;
            lcdPosition(linha, coluna);
            i += 2;
        }
        lcdData(v[i]);
    }
}

void clear(int xi, int xf, int yi, int yf) {
    char i, j;
    for (i = xi; i < xf; i++) {
        for (j = yi; j < yf; j++) {
            lcdPosition(i, j);
            lcdData(CLR);
        }
    }
}

void tempo(unsigned char x) {
    unsigned char i, j, k;
    for (i = 0; i < x; i++) {
        for (j = 0; j < 41; j++) {
            for (k = 0; k < 3; k++);
        }
    }
}

void LigarLed(char num, char pot) {
    unsigned char i, j, a;
    PORTD = 0x00;
    for (a = 0; a < 8; a++) {
        PORTD += num;
        num = num << 1;
        for (i = 0; i < 255; i++) {
            if (pot == '1') {
                for (j = 0; j < 255; j++);
                for (j = 0; j < 255; j++);
                for (j = 0; j < 255; j++);
            } else {
                for (j = 0; j < 255; j++);
                for (j = 0; j < 255; j++);
                for (j = 0; j < 255; j++);
                for (j = 0; j < 255; j++);
                for (j = 0; j < 255; j++);
                for (j = 0; j < 255; j++);
                for (j = 0; j < 255; j++);
                for (j = 0; j < 255; j++);
                for (j = 0; j < 255; j++);
            }
        }
    }
}