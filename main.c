/* 
 * File:   main.c
 * Author: Zachary
 *
 * Created on May 24, 2015, 12:31 PM
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include "config.h"
#include "globals.h"
#include "functionPrototypes.h"

#include "timers.h"
#include "rtcc.h"
#include "LCD.h"
#include "parsing.h"
#include "UART.h"
#include "wifi.h"
#include "ISR.h"

// *note: LPOSCEN has to be manually added to the dsPIC's header file in order
//        for the secondary oscillator to work.

// TODO: Write mode for entering wifi ssid/pass over uart
// TODO:     - Connect to new network.

// TODO: write I2C commands for DAC

// TODO: Write GET and POST functions that take strings as arguments

// TODO: Clean up header and C files
// TODO: write comments for code

/*FUNCTION PROTOTYPES*/

void setUp(void);
void blinkDebug1(int blinkCount);

//*MAIN FUNCTION*/
int main(void) 
{
    /*SETUP*/
    setUp(); // this function runs once on start-up
    boolean.bootComplete = TRUE; // boot setup completed
    txString1("\n\rSetup Complete!\n\n\r");
    blinkDebug1(5); // blink debug LED upon setup completion

    /*INFINITE LOOP*/
    while(1) 
    {
        if(boolean.wifiEntry == TRUE) // this is enabled through uart command
        {
            connectToAP();
        }
        else if(boolean.apListEntry == TRUE)
        {
            listAPs();
        }
        else
        {
            LATEbits.LATE12 ^= 1;
            delay_ms(100);
        }
    }
}

void setUp(void)
{
    // Analog/Digital Pin Configuration
    ANSELA = 0x0000; // disable analog functionality
    ANSELB = 0x0000; // disable analog functionality
    ANSELC = 0x0000; // disable analog functionality
    ANSELD = 0x0000; // disable analog functionality
    ANSELG = 0x0000; // disable analog functionality
    
    TRISEbits.TRISE12 = 0; // enable debug LED/test point
    TRISEbits.TRISE13 = 0; // enable debug LED/test point
    TRISEbits.TRISE14 = 0; // enable debug LED/test point
    
    boolean.bootComplete = FALSE;       // this prevents checking from disconnects before boot sequence completes
    boolean.repeatMode = FALSE;         // echo UART1 rx to UART2 tx AND UART2 rx to UART1 tx
    boolean.directCommand = FALSE;      // do not skip UART1 rx buffer (allows for UART1 to UART2 communications)
    boolean.timeMode = TRUE;            // start LCD display sequence with time mode first
    boolean.reconnectAttempt = FALSE;   // this prevents ISR from leaving interrupting itself
    boolean.wifiEntry = FALSE;          // this enables wifi entry menu
    boolean.apListEntry = FALSE;        // this enables the ap list menu
    boolean.stopWifiChecks = FALSE;     // this prevents wifi connection checks from interrupting a menu process
    
    __builtin_write_OSCCONL(OSCCONL & ~0x40);   // set I/O lock to 0
    RPOR7bits.RP57R = 0b10000;                  // configure RP57 as OC1 (PWM Horizontal)
        
    RPOR9bits.RP97R = 0b000001;                 // configure RP97 as Tx (U1TX) (FTDI) 
    RPINR18bits.U1RXR = 96;                     // configure RPI96 as Rx (U1RX) (FTDI)
    
    RPOR8bits.RP70R = 0b000011;                 // configure RP70 as Tx (U2TX) (WIFI) 
    RPINR19bits.U2RXR = 72;                     // configure RPI72 as Rx (U2RX) (WIFI)
    __builtin_write_OSCCONL(OSCCONL | 0x40);    // set I/O lock to 1

    // Enable PLL for 140 MHz
    PLLFBD = 68;                                // M = 70, PLL feedback divisor
    CLKDIVbits.PLLPRE = 2;                      // N2 = 2, PLL post-scalar
    CLKDIVbits.PLLPOST = 0;                     // N1 = 2, PLL pre-scalar
    OSCTUN = 0;
    // initiate clock switch to primary oscillator w/ PLL (NOSC = 0b011)
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(0x01);
    while(OSCCONbits.COSC != 0b011)
    {
        // wait for clock switch to occur
    } 
    while(OSCCONbits.LOCK != 1) 
    {
        // wait for PLL to lock
    } 
    init32bitTimer3(); // this timer is used for checking WiFi connection
    
    // initialize peripherals
    initUART1();delay_ms(50);       // initialize UART1
    initUART2();delay_ms(50);       // initialize UART2
    RTCC_Initialize();delay_ms(50); // initialize real time clock/calendar
    initPMP();delay_ms(50);         // initialize parallel master port for LCD
    initLCD();delay_ms(50);         // initialize LCD

    /*Special LCD Char for temperature degree symbol*/
    setLCDG(0);
    putLCD(0b00100);
    putLCD(0b01010);
    putLCD(0b00100);
    putLCD(0b00000);
    putLCD(0b00000);
    putLCD(0b00000);
    putLCD(0b00000);
    putLCD(0);
    
    /* LCD Splash Screen */
    lcdSplashScreen();
    delay_ms(500);
    initWifi();   // initialize wifi print APs function
    
    /* Check for WIFI */
    checkForWifiConnection();
    
    if(boolean.failedWifi == FALSE) // did not fail
    {
        /* Get weather and time from internet */
        getWeather();           // get weather data from the internet
        postWeatherToLCD();     // show weather on LCD
        getTime();              // get time data from the internet
        postTimeToLCD();        // show time on LCD
    }
    else{}
    
    /* Initialize timer interrupts */
    init32bitTimer9(); // this timer is used for updating weather/time from internet every 15 minutes
    init32bitTimer7(); // this timer is used for counting clock seconds
    init32bitTimer5(); // this timer is used for counting display switch over
    
}

void blinkDebug1(int blinkCount)
{
    int i;
    for(i=0; i<blinkCount*2; i++)
    {
        LATEbits.LATE12 ^= 1;
        delay_ms(100);
    }
}