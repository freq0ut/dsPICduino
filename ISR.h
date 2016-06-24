/* 
 * File:   ISR.h
 * Author: Zachary
 *
 * Created on May 24, 2015, 12:31 PM
 */
#include <xc.h>

// this ISR checks for a wifi connection every 15 seconds
void __attribute__((__interrupt__, auto_psv)) _T3Interrupt(void)
{
    if(boolean.bootComplete == TRUE && boolean.stopWifiChecks == FALSE && boolean.failedWifi == FALSE && boolean.repeatMode == FALSE)
    {
        sendCommandToWifi(checkConnection, 1000);
        lookForKeyword("))", &parsedDataBuffer[0], 2, 1);
        unsigned long wifiStatus = strtoul(&parsedDataBuffer[0], NULL, 0);
        if(wifiStatus != 5)
        {
            boolean.stopWifiChecks = TRUE;
            checkForWifiConnection();
            
            clearLCD();
            setCursor(2,0);
            putsLCD("        WiFi        ");
            setCursor(3,0);
            putsLCD("     Connected!     ");
            delay_ms(1000);

            clearLCD();
            setCursor(1,0);
            putsLCD("Getting time and");
            setCursor(2,0);
            putsLCD("weather data from");
            setCursor(3,0);
            putsLCD("the internet...");
            
            getWeather();
            postWeatherToLCD();
            getTime();
            
            boolean.failedWifi = FALSE;
            boolean.stopWifiChecks = FALSE;
        }
    }
    TMR2 = 0; // reset timer 2
    TMR3 = 0; // reset timer 3
    _T3IF = 0; // clear TMR3 interrupt flag
}

// this ISR toggles between weather and time display
void __attribute__((__interrupt__, auto_psv)) _T5Interrupt(void)
{
    boolean.timeMode ^= 1; // flip bit
    
    if(boolean.timeMode == TRUE && boolean.stopWifiChecks == FALSE)
    {
        postTimeToLCD();
    }
    else if(boolean.timeMode == FALSE && boolean.stopWifiChecks == FALSE)
    {
        postWeatherToLCD();
    }
    
    TMR4 = 0; // reset timer 4
    TMR5 = 0; // reset timer 5
    _T5IF = 0; // clear TMR5 interrupt flag
}

// this ISR counts seconds for the time display, only minutes and hours roll over...
void __attribute__((__interrupt__, auto_psv)) _T7Interrupt(void)
{
    SecondInt++;
    if(SecondInt>59)
    {
        SecondInt = 0;
        MinuteInt++;
    }
    if(MinuteInt>59)
    {
        MinuteInt = 0;
        HourInt++;
    }
    if(boolean.timeMode == TRUE && boolean.stopWifiChecks == FALSE)
    {
        setCursor(1,6);
        if(HourInt < 10)
            putLCD('0');
        sprintf(numToString,"%lu",HourInt);
        putsLCD(numToString);
        setCursor(1,8);putLCD(':');

        setCursor(1,9);
        if(MinuteInt < 10)
            putLCD('0');
        sprintf(numToString,"%lu",MinuteInt);
        putsLCD(numToString);
        setCursor(1,11);putLCD(':');

        setCursor(1,12);
        if(SecondInt < 10)
            putLCD('0');
        sprintf(numToString,"%lu",SecondInt);
        putsLCD(numToString);
        setCursor(1,8);
    }
    else{}
    
    
    TMR6 = 0; // reset timer 6
    TMR7 = 0; // reset timer 7
    _T7IF = 0; // clear TMR7 interrupt flag
}

// this ISR gets new time/weather data from the internet every ? minutes.
void __attribute__((__interrupt__, auto_psv)) _T9Interrupt(void)
{
    TMR9 = 0; // reset timer 9
    TMR8 = 0; // reset timer 8
    getWeather();
    getTime();
    TMR4 = 0; // reset timer 4
    TMR5 = 0; // reset timer 5
    TMR6 = 0; // reset timer 6
    TMR7 = 0; // reset timer 7
    _T9IF = 0; // clear TMR9 interrupt flag
}

// This ISR handles received data coming from USB
void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0; // clear rx interrupt flag 
    char bReceived;
    int i;
    if(U1_rx_fifo.num_bytes < FIFO_BUFFER_SIZE) // if there's room in the buffer
    { 
        while(U1STAbits.URXDA)
        {
            bReceived = U1RXREG;
            if ((U1STAbits.PERR == 0) && (U1STAbits.FERR == 0))
            {
                // check for BACKSPACE (ASCII DECIAML 8)
                if(bReceived == 8) 
                {
                    if(U1_rx_fifo.i_last > 0)
                    {
                        txUART1(8);             // increment back one space
                        txUART1(32);            // fill in a blank space
                        txUART1(8);             // increment back another space
                        U1_rx_fifo.data_buf[U1_rx_fifo.i_last] = '\0'; // replace with null
                        U1_rx_fifo.i_last--;    // decrement the index of the most recently added element
                        U1_rx_fifo.num_bytes--; // decrement the bytes counter 
                    }
                }
                // check for CARRIAGE RETURN (ASCII DECIAML 13)
                else if(bReceived == 13)
                {
                    txUART1(10); // tx a Line Feed
                    txUART1(13); // tx a Carriage Return
                    
                    if(boolean.wifiInfoSSID == TRUE)
                    {
                        i = 0;
                        while(U1_rx_fifo.data_buf[i] != '\0')
                        {
                            SSID[i] = U1_rx_fifo.data_buf[i];
                            i++;
                        }
                        SSID[i] = '\0';
                        boolean.wifiInfoSSID = FALSE;
                    }
                    else if(boolean.wifiInfoPW == TRUE)
                    {
                        i = 0;
                        while(U1_rx_fifo.data_buf[i] != '\0')
                        {   
                            password[i] = U1_rx_fifo.data_buf[i];
                            i++;
                        }
                        password[i] = '\0';
                        boolean.wifiInfoPW = FALSE;
                    }
                    else
                    {
                        serialMode(); // check entered command for keywords
                    }
                    //txString1("\r\n exited serial mode! \r\n");
                    // this if statement allows the listAP function to send direct commands to U2's rx
                    if(boolean.repeatMode == TRUE && boolean.directCommand == FALSE)
                    {
                        txString2(U1_rx_fifo.data_buf); // transmit the buffer to WIFI
                        txUART2(10); // tx a Line Feed
                    }
                    else
                    {
                        
                    }
                    
                    // clear out the buffer
                    clearUART1DataBuff(); 
                    U1_rx_fifo.i_last = 0; // roll over the index counter
                    U1_rx_fifo.num_bytes = 0; // roll over the byte counter
                }  
                else
                {
                    // put data into a queue
                    U1_rx_fifo.data_buf[U1_rx_fifo.i_last] = bReceived;
                    txUART1(bReceived); // echo back
                    U1_rx_fifo.i_last++; // increment the index of the most recently added element
                    U1_rx_fifo.num_bytes++; // increment the bytes counter
                } 
            }
            //txString1("\r\n exited if statement! \r\n");
        }
        //txString1("\r\n exited while loop! \r\n");
    }
    else
    {
        clearUART1DataBuff();
        U1_rx_fifo.i_last = 0; // roll over the index counter
        U1_rx_fifo.num_bytes = 0;
    }
    //txString1("\r\n exited if statement! \r\n");
}

// This ISR handles received data coming from the WiFi module
void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt(void)
{
    char bReceived;
    while(U2STAbits.URXDA)
    {
        bReceived = U2RXREG;
        if((U2STAbits.PERR == 0) && (U2STAbits.FERR == 0))
        {
            U2_rx_fifo.data_buf[U2_rx_fifo.i_last] = bReceived;
            U2_rx_fifo.i_last++; // increment the index of the most recently added element
            U2_rx_fifo.num_bytes++; // increment the bytes counter
            if(boolean.repeatMode == TRUE)
            {
                txUART1(bReceived); // echo back
            }
            else{}
        }
    }
    IFS1bits.U2RXIF = 0; // clear rx interrupt flag 
}