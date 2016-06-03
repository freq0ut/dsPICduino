/* 
 * File:   LCD.h
 * Author: Zachary
 *
 * Created on May 24, 2015, 12:31 PM
 */
#include <xc.h>

#define LCD_DATA    1
#define LCD_CMD     0
#define PM_DATA     PMDIN1
#define PM_ADDR     PMADDR

#define busyLCD()   readLCD(LCD_CMD) & 0x80
#define addrLCD()   readLCD(LCD_CMD) & 0x7F
#define getLCD()    readLCD(LCD_DATA)
#define putLCD(d)   writeLCD(LCD_DATA,d)
#define cmdLCD(c)   writeLCD(LCD_CMD, (c))
#define homeLCD()   writeLCD(LCD_CMD, 2)
#define clrLCD()    writeLCD(LCD_CMD, 1)
#define setLCDG(a)  writeLCD(LCD_CMD, (a & 0x3F) | 0x40)
#define setLCDC(a)  writeLCD(LCD_CMD, (a & 0x7F) | 0x80)

/*FUNCTION DEFINITIONS*/
void initPMP(void)
{
    PMCON = 0x0000;
    PMCON |= 0x033F;    // Enable the PMP, long waits
    PMMODE = 0x03FF;    // Master Mode 1
    PMAEN = 0x0001; 
    PMCON |= 0x8000;
}

void initLCD(void)
{
    while(PMMODEbits.BUSY);     // wait for PMP to be available
    delay_ms(50);
    PM_ADDR = LCD_CMD;
    PM_DATA = 0b00111000;       // 8-bit, 2 lines, 5x7
    while(busyLCD()){};
    delay_us(50);
    PM_DATA = 0b00001000;       // display off
    while(busyLCD()){};
    delay_us(50);
    PM_DATA = 0b00000001;       // clear display
    while(busyLCD()){};
    delay_ms(1.8);
    PM_DATA = 0b00000110;       // increment cursor, no shift
    while(busyLCD()){};
    delay_us(50);
    PM_DATA = 0b00000010;       // home command
    while(busyLCD()){};
    delay_us(50);
    PM_DATA = 0b00001100;       // display on
    while(busyLCD()){};
    delay_us(50);
    PMMODEbits.BUSY = 0;
}

void putsLCD(char *s)
{
    while(*s != '\0')
    {
        putLCD(*s);
        s++;
    }  
}

char readLCD(int addr)
{
    int i;
    while(PMMODEbits.BUSY);     // wait for PMP to be available
    PM_ADDR = addr;             // select the command address
    i = PM_DATA;                // initiate a read cycle, dummy
    while(PMMODEbits.BUSY);     // wait for PMP to be available
    return(PM_DATA);            // read the status register
}

void writeLCD(int addr, char c)
{
    while(PMMODEbits.BUSY);      // wait for PMP to be available
    PM_ADDR = addr;              // select the command address
    PM_DATA = c;
    while(busyLCD());
    delay_us(50);
    PMMODEbits.BUSY = 0;
}

void clearLCD(void)
{
    while(PMMODEbits.BUSY);
    PM_ADDR = LCD_CMD;
    PM_DATA = 0b00000001; // clear display
    delay_ms(1.8);
    PMMODEbits.BUSY = 0;
}

void setCursor(int row, int col)
{
    while(PMMODEbits.BUSY);
    if(row == 4)
    {
        setLCDC((0x54 + col));
    }   
    else if(row == 3)
    {
        setLCDC((0x14 + col));
    }
    else if(row == 2)
    {
        setLCDC((0x40 + col));
    }  
    else
    {
        setLCDC((0x00 + col));
    }      
    PMMODEbits.BUSY = 0;    
}

void lcdSplashScreen(void)
{
    clearLCD();
    setCursor(1,0);
    putsLCD("     dsPICduino     ");
    setCursor(3,0);
    putsLCD("   an open source   ");
    setCursor(4,0);
    putsLCD("  development tool  ");
}