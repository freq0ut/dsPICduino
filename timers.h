/* 
 * File:   timers.h
 * Author: Zachary
 *
 * Created on May 24, 2015, 12:31 PM
 */
#include <xc.h>

/*FUNCTION DEFINITIONS*/
void init32bitTimer9(void) // this timer generates interrupt for polling internet for new data
{
    T8CON = 0x0028; // turn on TMR8/9
                    // 32-bit mode
                    // prescale = 64
                    // use system clock (T_CY)
    T8CONbits.TON = 1; //enable Timer

    // 1312500000 counts = 10 minutes.
    
    PR9 = 0x3AAC; // set period register for 15 minutes
    PR8 = 0x5ED8; // set period register for 15 minutes
    
    TMR9 = 0; // reset 32-bit TMR8/9 counter value.
    TMR8 = 0; // reset 32-bit TMR8/9 counter value.

    // Timer 8/9 in 32-bit mode uses Timer 9 interrupt services
    _T9IF = 0; // clear interrupt flag
    _T9IE = 1; // enable TMR9 interrupt source
    _T9IP = 4; // interrupt priority level
}

void init32bitTimer7(void) // used for counting seconds
{
    T6CON = 0x0008; // turn on TMR8/9
                    // 32-bit mode
                    // prescale = 1
                    // use system clock (T_CY)
    T6CONbits.TON = 1; //enable Timer

    // 73201365 counts = 1 second.
    
    PR7 = 0x045C; // set period register for one second
    PR6 = 0xF6D5; // set period register for one second
    
    TMR7 = 0; // reset 32-bit TMR7 counter value.
    TMR6 = 0; // reset 32-bit TMR6 counter value.

    // Timer 6/7 in 32-bit mode uses Timer 7 interrupt services
    _T7IF = 0; // clear interrupt flag
    _T7IE = 1; // enable TMR7 interrupt source
    _T7IP = 6; // interrupt priority level
}

void init32bitTimer5(void) // used for counting 3-5 seconds
{
    T4CON = 0x0008; // turn on TMR8/9
                    // 32-bit mode
                    // prescale = 1
                    // use system clock (T_CY)
    T4CONbits.TON = 1; //enable Timer

    // 219604096 counts = 3 seconds (H = 0x0D16 L = 0xE480)
    // 366006827 counts = 5 seconds (H = 0x15D0 L = 0xD22B)
    
    PR5 = 0x15D0; // set period register for capturing leading edge of pulse.
    PR4 = 0xD22B; // set period register for capturing leading edge of pulse.
    
    TMR5 = 0; // reset 32-bit TMR5 counter value.
    TMR4 = 0; // reset 32-bit TMR4 counter value.

    // Timer 4/5 in 32-bit mode uses Timer 5 interrupt services
    _T5IF = 0; // clear interrupt flag
    _T5IE = 1; // enable TMR5 interrupt source
    _T5IP = 5; // interrupt priority level
}

void init32bitTimer3(void) // used for checking wifi status
{
    T2CON = 0x0008; // turn on TMR8/9
                    // 32-bit mode
                    // prescale = 1
                    // use system clock (T_CY)
    T2CONbits.TON = 1; //enable Timer

    // 1098020340 counts = 15 seconds.
    // 
    
    PR3 = 0x4172; // set period register for one second
    PR2 = 0x75F4; // set period register for one second
    
    TMR3 = 0; // reset 32-bit TMR3 counter value.
    TMR2 = 0; // reset 32-bit TMR2 counter value.

    // Timer 2/3 in 32-bit mode uses Timer 3 interrupt services
    _T3IF = 0; // clear interrupt flag
    _T3IE = 1; // enable TMR3 interrupt source
    _T3IP = 3; // interrupt priority level
}