/* 
 * File:   wifi.h
 * Author: Zachary
 *
 * Created on May 24, 2015, 12:31 PM
 */
#include <xc.h>

/*FUNCTION DEFINITIONS*/
void initWifi(void)
{
    // this creates a function for listing access points on start up.
    sendCommandToWifi(listAP_function, 500);
    sendCommandToWifi(listAP_for, 500);
    sendCommandToWifi(listAP_print, 500);
    sendCommandToWifi(listAP_end, 500);
    sendCommandToWifi(listAP_end, 500);
}

void checkForWifiConnection(void)
{
    BOOL doOnce;

    sendCommandToWifi(checkConnection, 1000); // check to see if connection exists
    lookForKeyword("))", &parsedDataBuffer[0], 2, 1); // prase for status value
    unsigned long wifiStatus = strtoul(&parsedDataBuffer[0], NULL, 0);
    if(wifiStatus != 5) // if connection doesn't exist, set the "do once" bit
    {
        doOnce = FALSE;
    }

    while(wifiStatus != 5 && boolean.wifiEntry == FALSE) // while failed connection and not attempting to make a connection
    {
        if(doOnce == FALSE)
        {
            clearLCD(); // print to the display one time
            setCursor(1,0);
            putsLCD("   WiFi connection  ");
            setCursor(2,0);
            putsLCD("      failed...     ");
            setCursor(3,0);
            putsLCD("   Configure WiFi   ");
            setCursor(4,0);
            putsLCD("    through UART    ");
            doOnce = TRUE;
            boolean.failedWifi = TRUE;
        }
        if(boolean.bootComplete == TRUE && boolean.wifiEntry == FALSE)  // if setup has completed, repeat checks
        {                                                               // wifi entry prevents checks to be made when entering info
            sendCommandToWifi(checkConnection, 1000);
            lookForKeyword("))", &parsedDataBuffer[0], 2, 1);
            wifiStatus = strtoul(&parsedDataBuffer[0], NULL, 0);
        }
        else
        {
            boolean.stopWifiChecks = TRUE;
            boolean.connectOnBoot = FALSE;
            break; // continue with rest of set up, user needs to input connection info.
        }
    }
    
    if(boolean.failedWifi != TRUE)
    {
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
    }
}

// 1st argument: command to send - this is a static array of chars defined in globals
// 2nd argument: amount of time to delay while UART is processing
void sendCommandToWifi(char* command, int delayTime)
{
    U2_rx_fifo.i_last = 0; // roll over the index counter
    U2_rx_fifo.num_bytes = 0; // roll over the byte counter
    clearUART2dataBuff(); // clear buffer
    
    txString2(command); // prepare command
    delay_ms(250);
    txUART2(10); // send command
    
    delay_ms(delayTime); // allow time to pass for echo to be fed into buffer
}

void weatherFromThingSpeak(void)
{
    sendCommandToWifi(skCreateConn, 500);
    sendCommandToWifi(skOn, 500);
    sendCommandToWifi(skConn, 500);
    sendCommandToWifi(skSendWeather, 5000);
}

void timeFromThingSpeak(void)
{
    sendCommandToWifi(skCreateConn, 500);
    sendCommandToWifi(skOn, 500);
    sendCommandToWifi(skConn, 500);
    sendCommandToWifi(skSendTime, 5000);
}

void getWeather(void)
{
    static char tempKey[5] = "temp";
    static char feelsLikeKey[9] = "RealFeel";
    static char humidityKey[9] = "Humidity";
    static char conditionKey[5] = "cond";
    
    httpError = 1;
    while(httpError != 0) // make sure HTTP GET request is OK
    {
        // scrape HTML from ThingSpeak
        weatherFromThingSpeak(); // make HTTP GET request for weather
        httpError = checkHTTPrequest();
        if(httpError == 0)
        {
            lookForKeyword(tempKey, &displayTemp[0], 2, 2);
            lookForKeyword(feelsLikeKey, &displayFeel[0], 3, 2);
            lookForKeyword(humidityKey, &displayHumidity[0], 10, 3);
            lookForKeyword(conditionKey, &displayCondition[0], 2, 20);
        }
    }
}

void getTime(void)
{
    static char timeKey[16] = "value";
    time_t c;
    int i = 0;
    httpError = 1;
    
    while(httpError != 0) // make sure HTTP GET request is OK
    {
        timeFromThingSpeak(); // make GET requests for UNIX time
        httpError = checkHTTPrequest(); // make sure HTTP GET request is OK
        // scrape HTML from ThingSpeak
        if(httpError == 0)
        {
            lookForKeyword(timeKey, &unixTime[0], 2, 10);
            c = strtoul(unixTime, NULL, 0);
            c = c - 14400; // convert to US East time zone
            c_time_string = ctime(&c);
            *(c_time_string + strlen(c_time_string)) = '\0';
            
            // note:    ASCII 32 = space
            //          ASCII 58 = :
            //          ASCII 0  = NULL
            i = convertUNIXTime(&i, c_time_string, &weekDay[0], 32);
            i = convertUNIXTime(&i, c_time_string, &Month[0], 32);
            i = convertUNIXTime(&i, c_time_string, &Date[0], 32);
            i = convertUNIXTime(&i, c_time_string, &Hour[0], 58);
            i = convertUNIXTime(&i, c_time_string, &Minute[0], 58);
            i = convertUNIXTime(&i, c_time_string, &Second[0], 32);
            i = convertUNIXTime(&i, c_time_string, &Year[0], 0);
            
            // convert date, hour, minute, second, and year from string to ul data type
            if(Date[0] == '0')
                DateInt = strtoul(&Date[1], NULL, 0);
            else
                DateInt = strtoul(Date, NULL, 0);
            if(Hour[0] == '0')
                HourInt = strtoul(&Hour[1], NULL, 0);
            else
                HourInt = strtoul(Hour, NULL, 0);
            if(Minute[0] == '0')
                MinuteInt = strtoul(&Minute[1], NULL, 0);
            else
                MinuteInt = strtoul(Minute, NULL, 0);
            if(Second[0] == '0')
                SecondInt = strtoul(&Second[1], NULL, 0);
            else
                SecondInt = strtoul(Second, NULL, 0); 
            YearInt = strtoul(&Year[0], NULL, 0);
        } 
    }
}

int checkHTTPrequest(void)
{
    static char httpOKkey[9] = "HTTP/1.1";
    static char httpOKcheck[7] = "200 OK";
    
    lookForKeyword(httpOKkey, &httpOKBuffer[0], 1, 6);
    
    int test;
    test = compareStrings(httpOKcheck, &httpOKBuffer[0]);
    
    if(test == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }   
}

void clearUART2dataBuff(void)
{
    int i;
    for(i = 0; i<4096; i++)
    {
        U2_rx_fifo.data_buf[i] = '\0'; 
    }
}

void clearTimeBuff(void)
{
    int i;
    for(i = 0; i<32; i++)
    {
        c_time_string[i] = '\0'; 
    }
}

int convertUNIXTime(int* i, char* cTime, char* dstString, char delim)
{
    int j = 0;
    int cTimeIndex = *i;
    if(cTime[cTimeIndex] == 32)
        cTimeIndex++;
    while(cTime[cTimeIndex] != delim)
    {
        dstString[j] = cTime[cTimeIndex];
        cTimeIndex++;
        j++;
    }
    dstString[j] = '\0';
    cTimeIndex++;
    return cTimeIndex;
}

void postWeatherToLCD(void)
{
    clearLCD();
    
    setCursor(1,0);putsLCD("Temperature: ");
    setCursor(1,13);putsLCD(displayTemp);setCursor(1,15);putLCD(0);putLCD('F');
    
    setCursor(2,0);putsLCD("Feels like:  ");
    setCursor(2,13);putsLCD(displayFeel);setCursor(2,15);putLCD(0);putLCD('F');
    
    setCursor(3,0);putsLCD("Humidity:    ");
    setCursor(3,13);putsLCD(displayHumidity);
    
    setCursor(4,0);putsLCD(displayCondition);
}

void postTimeToLCD(void)
{
    clearLCD();
    //////xx:xx:xx//////
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
    //xxx xxx, xx xxxx//
    setCursor(3,2);
    putsLCD(weekDay);putsLCD(" ");putsLCD(Month);putsLCD(", ");
    sprintf(numToString,"%lu",DateInt);
    putsLCD(Date);
    sprintf(numToString,"%lu",YearInt);
    putsLCD(" ");
    putsLCD(numToString);
}

void listAPs(void)
{
    boolean.directCommand = TRUE; // direct command
    boolean.repeatMode = TRUE; // repeat on
    boolean.stopWifiChecks = TRUE;
    
    txString1("Sending: \"");
    txString1(listAP_getAP);
    txString1("\" to WiFi module... \n\r\n\r");
    txString1("Please wait... \n\r\n\r");

    sendCommandToWifi(listAP_getAP, 5000);
    
    boolean.directCommand = FALSE;
    boolean.repeatMode = FALSE;
    boolean.apListEntry = FALSE;
    if(boolean.connectOnBoot == TRUE)
        boolean.stopWifiChecks = FALSE;
    txString1("\r\nDone!\n\n\r");
}

void connectToAP(void)
{
    boolean.stopWifiChecks = TRUE;
    clearUART1DataBuff();
    
    // prompt user for SSID:
    txString1("\r\nEnter the SSID: ");
    boolean.wifiInfoSSID = TRUE;
    
    // wait for user to enter SSID and press enter
    while(boolean.wifiInfoSSID){}

    // prompt user for PW:
    clearUART1DataBuff(); 
    txString1("Enter the password for ");
    txString1(&SSID[0]);
    txString1(": ");
    boolean.wifiInfoPW = TRUE;
    
    // wait for user to enter password and press enter
    while(boolean.wifiInfoPW){}
    
    txString1("\n\rConnecting to: ");
    txString1(&SSID[0]);
    txString1(" / ");
    txString1(&password[0]);
    txString1(" ...\n\r\n\r");
    
    concatStrings(&SSID[0],&password[0]);
    
    sendCommandToWifi(setMode, 500);
    sendCommandToWifi(wifiDisconnect, 500);
    sendCommandToWifi(wifiInfo, 500);
    sendCommandToWifi(wifiConnect, 500);
    
    boolean.wifiEntry = FALSE;
    checkForWifiConnection();
    boolean.failedWifi = FALSE;
    
    txString1("Connected to ");
    txString1(&SSID[0]);
    txString1("!\r\n");
    
    clearLCD();
    setCursor(2,0);
    putsLCD("        WiFi        ");
    setCursor(3,0);
    putsLCD("     Connected!     ");
    delay_ms(1000);
    
    txString1("\n\rGetting time and weather data...\n\r");

    clearLCD();
    setCursor(1,0);
    putsLCD("Getting time and");
    setCursor(2,0);
    putsLCD("weather data from");
    setCursor(3,0);
    putsLCD("the internet...");
    
    getWeather();           // get weather data from the internet
    postWeatherToLCD();     // show weather on LCD
    getTime();              // get time data from the internet
    postTimeToLCD();        // show time on LCD
    
    boolean.stopWifiChecks = FALSE;
    boolean.connectOnBoot = TRUE;
    txString1("\n\rDone!\n\r\n\r");
}