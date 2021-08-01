#include "pic18f4520.h"
#include "config.h"
#include "bits.h"
#include "lcd.h"
#include "keypad.h"
#include "ssd.h"
#include "PF.h"
#include "adc.h"
#include "itoa.h"
#include "pwm.h"
#include <xc.h>

void main() {
    char i, str[6];
    char opcoes[55] = "Escolha o sabor:\\n1-Baunilha\\n2-Chocolate\\n3-Mista";
    unsigned int tecla = 16;
    float tempoSSD;
    int aux = 0;
    unsigned char tmp;
    char desenho[48] = {
        0x0F, 0x08, 0x09, 0x08, 0x0A, 0x0B, 0x08, 0x0F,
        0x1E, 0x02, 0x0A, 0x02, 0x0A, 0x1A, 0x02, 0x1E,
        0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00,
        0x1F, 0x1E, 0x1C, 0x18, 0x10, 0x00, 0x00, 0x00
    };

    //inicializacao
    lcdInit();
    ssdInit();
    kpInit();
    pwmInit();

    //mensagem de inicio
    lcdPosition(0, 2);
    lcd_str("Bem-vindo(a)");
    lcdPosition(2, 0);
    lcd_str("Para comecar");
    lcdPosition(3, 0);
    lcd_str("Press. #");
    while (aux == 0) {
        kpDebounce();
        if (kpRead() != tecla) {
            tecla = kpRead();

            if (bitTst(tecla, 8)) {
                aux = 1;
            }
        }
    }

    //menu de opcoes
    clear(0, 4, 0, 16);
    lcdPosition(0, 0);
    printf(opcoes);
    while (aux == 1) {
        //apertar o botao
        kpDebounce();
        if (kpRead() != tecla) {
            tecla = kpRead();

            if (bitTst(tecla, 3)) { //tecla 1 
                for (tempoSSD = 0; tempoSSD < 300; tempoSSD++) {
                    ssdDigit(0, 0);
                    ssdDigit(3, 1);
                    ssdDigit(5, 2);
                    ssdDigit(0, 3);
                    ssdUpdate();
                    tempo(5);
                }
                aux = 2;
            }
            if (bitTst(tecla, 7)) { //tecla 2
                for (tempoSSD = 0; tempoSSD < 300; tempoSSD++) {
                    ssdDigit(0, 0);
                    ssdDigit(4, 1);
                    ssdDigit(0, 2);
                    ssdDigit(0, 3);
                    ssdUpdate();
                    tempo(5);
                }
                aux = 2;
            }

            if (bitTst(tecla, 11)) { //tecla 3
                for (tempoSSD = 0; tempoSSD < 300; tempoSSD++) {
                    ssdDigit(0, 0);
                    ssdDigit(5, 1);
                    ssdDigit(0, 2);
                    ssdDigit(0, 3);
                    ssdUpdate();
                    tempo(5);
                }
                aux = 2;
            }

        }
    }
    PORTD = 0x00;
    clear(0, 4, 0, 16);
    lcdPosition(0, 3);
    lcd_str("Cremosidade");
    lcdPosition(1, 0);
    lcd_str("[0%]--------[5%]");
    //potenciometro
    for (i = 0; i < 100; i++) {
        tmp = (adc_amostra(0)*10) / 204;
        itoa(tmp, str);
        lcdPosition(2, 6);
        lcdData(str[3]);
        lcdData(',');
        lcdData(str[4]);
        lcdData('%');
        tempo(250);
    }
    clear(0, 4, 0, 16);

    lcdPosition(0, 0);
    lcd_str("Aguarde");

    //testa qual sera a velocidade
    if (str[3] == '1') {
        lcdPosition(2, 0);
        lcd_str("Rapidamente");
        lcdPosition(3, 0);
        lcd_str("estara pronto");
        //liga a ventoinha
        PORTCbits.RC5 = 0;
        pwmSet(50);
        tempo(250);
        tempo(250);
    } else {
        lcdPosition(2, 0);
        lcd_str("Estara pronto");
        lcdPosition(3, 0);
        lcd_str("em instantes...");
        //liga a ventoinha
        PORTCbits.RC5 = 0;
        pwmSet(80);
        tempo(250);
        tempo(250);
    }

    //pisca os leds e o rele 
    TRISCbits.TRISC0 = 0;
    for (i = 0; i < 10; i++) {
        PORTCbits.RC0 ^= 1;
        tempo(250);
        tempo(250);
    }

    PORTD = 0x01;
    LigarLed(0x01, str[3]);

    clear(0, 4, 0, 16);
    PORTCbits.RC0 = 0;
    PORTD = 0x00;

    //desliga a ventoinha
    pwmSet(0);
    //pisca o rele 2 indicando o fim
    TRISEbits.TRISE0 = 0;
    for (i = 0; i < 10; i++) {
        PORTEbits.RE0 ^= 1;
        tempo(250);
        tempo(250);
    }
    PORTEbits.RE0 = 0;

    //mensagem final
    lcdPosition(0, 2);
    lcd_str("Volte sempre");
    lcdCommand(0x40);
    for (i = 0; i < 32; i++) {
        lcdData(desenho[i]);
    }
    lcdPosition(1, 7);
    lcdData(0);
    lcdData(1);
    lcdPosition(2, 7);
    lcdData(2);
    lcdData(3);
    lcdPosition(3, 0);
    lcd_str("PRESS.[RESET]");
    for (;;);
}