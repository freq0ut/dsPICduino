/* 
 * File:   UART.h
 * Author: Zachary
 *
 * Created on May 24, 2015, 12:31 PM
 */
#include <xc.h>

/*FUNCTION DEFINITIONS*/
void initUART1(void) // U1 is used for FTDI.
{  
    U1MODE = 0x8008;    // 8N1, BRGH = 1
    U1BRG = 151;        // baud=115200, BRGH=1
    
    U1STA = 0x0400;     // enable transmission
    
    IFS0bits.U1RXIF = 0;        // clear rx interrupt flag
    IEC0bits.U1RXIE = 1;        // enable Rx interrupts
    IPC2bits.U1RXIP = 6;
}

void initUART2(void) // U2 is used for Wifi module.
{ 
    U2MODE = 0x8000;    // 8N1, BRGH = 1
    U2BRG = 455;        // baud=9600, BRGH=0

    U2STA = 0x0400;     // enable transmission
    
    IFS1bits.U2RXIF = 0;        // clear rx interrupt flag
    IEC1bits.U2RXIE = 1;        // enable Rx interrupts
    IPC7bits.U2RXIP = 7; 
}

void txUART1(char c)
{
    while(U1STAbits.UTXBF); //wait while Tx buffer is full
    U1TXREG = c;            //send the character
}

void txString1(char *s)
{
    while(*s)
        txUART1(*s++);
}

void txUART2(char c)
{
    while(U2STAbits.UTXBF); //wait while Tx buffer is full
    U2TXREG = c;            //send the character
}

void txString2(char *s)
{
    while(*s)
        txUART2(*s++);
}

void serialMode(void)
{
    if(compareStrings(U1_rx_fifo.data_buf, "?") == 0)
    {
        // display help menu
        txString1("\r\n----------------------------------------------------\n\r");
        txString1("------------------------MENU------------------------\n\r");
        txString1("----------------------------------------------------\n\r");
        txString1("?:            Help menu.\n\r");
        txString1("\n\r");
        txString1("REPEAT ON:    Repeat commands to Wifi module. \n\r");
        txString1("              (Only enable for direct Lua scripting.) \n\r");
        txString1("REPEAT OFF:   Do not repeat commands to Wifi module. \n\r");
        txString1("\n\r");
        txString1("REF WEATHER:  Refresh the weather data. \n\r");
        txString1("REF TIME:     Refresh the current UNIX time. \n\r");
        txString1("\n\r");
        txString1("LIST APS:     List near by access points. \n\r");
        txString1("CONNECT AP:   Connect to access point. \n\r");
        txString1("----------------------------------------------------\n\r");
        txString1("      * Note: commands are not case sensitive.\n\r");
        txString1("----------------------------------------------------\n\n\r");
    }
    else if(compareStrings(U1_rx_fifo.data_buf, "repeat off") == 0)
    {
        boolean.repeatMode = FALSE;
        boolean.directCommand = FALSE;
        txString1("\r\nYour input commands will no longer be repeated to the WiFi module.\n\n\r");
    }
    
    else if(compareStrings(U1_rx_fifo.data_buf, "repeat on") == 0)
    {
        txString1("\r\nYour input commands will now be repeated to the WiFi module.\n\n\r");
        txString1("WARNING: Entering invalid nodeMCU commands may\n\r");
        txString1("         cause WiFi module to lock up...\n\r");
        txString1("         Power cycle may be necessary!\n\n\r");
        txString1("         Reference: \r\n");
        txString1("         https://nodemcu.readthedocs.io/en/dev/en/modules/wifi/\r\n");
        clearUART1DataBuff();
        boolean.repeatMode = TRUE; // turn repeat on
        boolean.directCommand = FALSE; // not for direct commands
    }
    
    else if(compareStrings(U1_rx_fifo.data_buf, "ref weather") == 0)
    {
        if(boolean.failedWifi == FALSE)
        {
            txString1("\r\nGetting new weather data from the internet...\n\n\rPlease wait...\n\n\r");
            //boolean.directCommand = TRUE;
            boolean.repeatMode = TRUE;
            getWeather();
            //boolean.directCommand = FALSE;
            boolean.repeatMode = FALSE;
            txString1("Done!\n\n\r");
        }
        else
        {
            txString1("\r\nNo internet connection! Use 'CONNECT AP' command!\n\n\r");
        }
    }
    else if(compareStrings(U1_rx_fifo.data_buf, "ref time") == 0)
    {
        if(boolean.failedWifi == FALSE)
        {
            txString1("\n\rRefreshing UNIX time from the internet...\n\n\rPlease wait...\n\n\r");
            //boolean.directCommand = TRUE;
            boolean.repeatMode = TRUE;
            getTime();
            //boolean.directCommand = FALSE;
            boolean.repeatMode = FALSE;
            txString1("\r\nDone!\n\n\r");
        }
        else
        {
            txString1("\r\nNo internet connection! Use 'CONNECT AP' command!\n\n\r");
        }
    }
    else if(compareStrings(U1_rx_fifo.data_buf, "set time") == 0)
    {
        // do something else
        txString1("Enter the current time in 24 hour format (HH:MM:SS).\n\n\r");
    }
    else if(compareStrings(U1_rx_fifo.data_buf, "show time") == 0)
    {
        RTCC_TimeGet();

        sprintf(numToString,"%d",times.date);
        txString1("\r\nDate: ");
        txString1(numToString);
        txString1("\r\n");
        
        sprintf(numToString,"%d",times.hour);
        txString1("Hour: ");
        txString1(numToString);
        txString1("\r\n");
        
        sprintf(numToString,"%d",times.min);
        txString1("Minute: ");
        txString1(numToString);
        txString1("\r\n");
        
        sprintf(numToString,"%d",times.sec);
        txString1("Second: ");
        txString1(numToString);
        txString1("\r\n");
    }
    else if(compareStrings(U1_rx_fifo.data_buf, "list aps") == 0)
    {
        boolean.apListEntry = TRUE;
    }
    else if(compareStrings(U1_rx_fifo.data_buf, "connect ap") == 0)
    {
        boolean.wifiEntry = TRUE;
    }
    else
    {
        if(boolean.repeatMode == FALSE && boolean.wifiEntry == FALSE)
            txString1("\r\nCommand not recognized.\n\n\r");
    }
}

void clearUART1DataBuff(void)
{
    int i;
    for(i = 0; i<FIFO_BUFFER_SIZE; i++)
    {
        U1_rx_fifo.data_buf[i] = '\0';
    } 
}
