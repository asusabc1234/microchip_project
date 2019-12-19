/*
 * File:   newmain.c
 * Author: User
 *
 * Created on 2019年12月14日, 上午 11:43
 */
// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)
typedef struct RGB{
    int r;
    int g;
    int b;
}rgbarray;
volatile rgbarray rgb[24];
#define _XTAL_FREQ 20000000
#include <xc.h>
#include <stdio.h>
volatile int original_r = 0;
volatile int original_l = 0;
void ADC_Initialize(void) {
    
    TRISA = 1;		// Set as input port
    ADCON1 = 0x0e;  	// Ref vtg is VDD & Configure pin as analog pin 
    ADCON2 = 0x90;
    ADFM = 1 ;          // Right Justifie
    ADCON2bits.ADCS = 0; // 
    ADRESH=0;  			// Flush ADC output Register
    ADRESL=0;
}
int ADC_Read()
{
    int digital;
    
    ADCON0bits.CHS =  0; 
    ADCON0bits.GO = 1;
    ADCON0bits.ADON = 1;
    
    while(ADCON0bits.GO_nDONE==1);

    digital = (ADRESH*256) | (ADRESL);
    return(digital);
}

void initial(void){
    int i;
    for(i = 0;i < 4;i++){
        rgb[i].r = 0;
        rgb[i].g = 1;
        rgb[i].b = 1;
    }
    for(i = 4;i < 8;i++){
        rgb[i].r = 1;
        rgb[i].g = 0;
        rgb[i].b = 1;
    }
    for(i = 8;i < 12;i++){
        rgb[i].r = 1;
        rgb[i].g = 1;
        rgb[i].b = 0;
    }
    for(i = 12;i < 16;i++){
        rgb[i].r = 0;
        rgb[i].g = 1;
        rgb[i].b = 0;
    }
    for(i = 16;i < 20;i++){
        rgb[i].r = 0;
        rgb[i].g = 0;
        rgb[i].b = 1;
    }
    for(i = 20;i < 24;i++){
        rgb[i].r = 0;
        rgb[i].g = 0;
        rgb[i].b = 0;
    }
}
void rotate_r(int val){
    if(val > 3){
        val = 3;
    }
    if(val>original_r){
        int buf = val-original_r;
        while(buf>0){
            rgbarray tempa = rgb[2];
            rgbarray tempb = rgb[3];
            rgb[2].r = rgb[6].r;rgb[3].r = rgb[7].r;
            rgb[2].g = rgb[6].g;rgb[3].g = rgb[7].g;
            rgb[2].b = rgb[6].b;rgb[3].b = rgb[7].b;
            rgb[6].r = rgb[10].r;rgb[7].r = rgb[11].r;
            rgb[6].g = rgb[10].g;rgb[7].g = rgb[11].g;
            rgb[6].b = rgb[10].b;rgb[7].b = rgb[11].b;
            rgb[10].r = rgb[14].r;rgb[11].r = rgb[15].r;
            rgb[10].g = rgb[14].g;rgb[11].g = rgb[15].g;
            rgb[10].b = rgb[14].b;rgb[11].b = rgb[15].b;
            rgb[14].r = tempa.r;rgb[15].r = tempb.r;
            rgb[14].g = tempa.g;rgb[15].g = tempb.g;
            rgb[14].b = tempa.b;rgb[15].b = tempb.b;
            buf--;
        }
    }
    else if(original_r>val){
        int buf = original_r-val;
        while(buf>0){
            rgbarray tempc = rgb[14];
            rgbarray tempd = rgb[15];
            rgb[14].r = rgb[10].r;rgb[15].r = rgb[11].r;
            rgb[14].g = rgb[10].g;rgb[15].g = rgb[11].g;
            rgb[14].b = rgb[10].b;rgb[15].b = rgb[11].b;
            rgb[10].r = rgb[6].r;rgb[11].r = rgb[7].r;
            rgb[10].g = rgb[6].g;rgb[11].g = rgb[7].g;
            rgb[10].b = rgb[6].b;rgb[11].b = rgb[7].b;
            rgb[6].r = rgb[2].r;rgb[7].r = rgb[3].r;
            rgb[6].g = rgb[2].g;rgb[7].g = rgb[3].g;
            rgb[6].b = rgb[2].b;rgb[7].b = rgb[3].b;
            rgb[2].r = tempc.r;rgb[3].r = tempd.r;
            rgb[2].g = tempc.g;rgb[3].g = tempd.g;
            rgb[2].b = tempc.b;rgb[3].b = tempd.b; 
            buf--;
        }
         
    }
}
void rotate_l(int val){
    //used to rotate left line
    if(val > 3){
        val = 3;
    }
    if(val>original_l){
        int buf = val-original_l;
        while(buf>0){
            rgbarray tempa = rgb[0];
            rgbarray tempb = rgb[1];
            rgb[0].r = rgb[4].r;rgb[1].r = rgb[5].r;
            rgb[0].g = rgb[4].g;rgb[1].g = rgb[5].g;
            rgb[0].b = rgb[4].b;rgb[1].b = rgb[5].b;
            rgb[4].r = rgb[8].r;rgb[5].r = rgb[9].r;
            rgb[4].g = rgb[8].g;rgb[5].g = rgb[9].g;
            rgb[4].b = rgb[8].b;rgb[5].b = rgb[9].b;
            rgb[8].r = rgb[12].r;rgb[9].r = rgb[13].r;
            rgb[8].g = rgb[12].g;rgb[9].g = rgb[13].g;
            rgb[8].b = rgb[12].b;rgb[9].b = rgb[13].b;
            rgb[12].r = tempa.r;rgb[13].r = tempb.r;
            rgb[12].g = tempa.g;rgb[13].g = tempb.g;
            rgb[12].b = tempa.b;rgb[13].b = tempb.b;
            buf--;
        }
    }
    else if(original_l>val){
        int buf = original_l-val;
        while(buf>0){
            rgbarray tempc = rgb[12];
            rgbarray tempd = rgb[13];
            rgb[12].r = rgb[8].r;rgb[13].r = rgb[9].r;
            rgb[12].g = rgb[8].g;rgb[13].g = rgb[9].g;
            rgb[12].b = rgb[8].b;rgb[13].b = rgb[9].b;
            rgb[8].r = rgb[4].r;rgb[9].r = rgb[5].r;
            rgb[8].g = rgb[4].g;rgb[9].g = rgb[5].g;
            rgb[8].b = rgb[4].b;rgb[9].b = rgb[5].b;
            rgb[4].r = rgb[0].r;rgb[5].r = rgb[1].r;
            rgb[4].g = rgb[0].g;rgb[5].g = rgb[1].g;
            rgb[4].b = rgb[0].b;rgb[5].b = rgb[1].b;
            rgb[0].r = tempc.r;rgb[1].r = tempd.r;
            rgb[0].g = tempc.g;rgb[1].g = tempd.g;
            rgb[0].b = tempc.b;rgb[1].b = tempd.b; 
            buf--;
        }
         
    }
}
void main(void) {
    initial();
    ADC_Initialize();
    TRISD =0;
    LATD = 0;
    LATDbits.LATD3 = 1;
    int i = 0;
    while(1){
        int val = ADC_Read();
        rotate_r(val/300);
        original_r = val/300;
        //need to add everything
        LATDbits.LATD0 = rgb[i].r;
        LATDbits.LATD1 = rgb[i].g;
        LATDbits.LATD2 = rgb[i].b;
        __delay_us(50);
        LATDbits.LATD0 = 1;
        LATDbits.LATD1 = 1;
        LATDbits.LATD2 = 1;
        int temp = LATDbits.LATD6;
        LATDbits.LATD6 = LATDbits.LATD5;
        LATDbits.LATD5 = LATDbits.LATD4;
        LATDbits.LATD4 = LATDbits.LATD3;
        LATDbits.LATD3 = temp;
        i++;
        if(i == 4){
            i = 0;
        }
    }
    return;
}
