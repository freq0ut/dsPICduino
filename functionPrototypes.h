/* 
 * File:   UART.h
 * Author: Zachary
 *
 * Created on May 24, 2015, 12:31 PM
 */
#include <xc.h>

/*----------------------------FUNCTIONS IN: wifi.h----------------------------*/
void initWifi(void);
void checkForWifiConnection(void);
void sendCommandToWifi(char* command, int delayTime);
void weatherFromThingSpeak(void);
void timeFromThingSpeak(void);
void getWeather(void);
void getTime(void);
int checkHTTPrequest(void);
void clearUART2dataBuff(void);
void clearTimeBuff(void);
int convertUNIXTime(int* i, char* cTime, char* dstString, char delim);
void postWeatherToLCD(void);
void postTimeToLCD(void);
void listAPs(void);
void connectToAP(void);
void postToServer(void);
/*----------------------------------------------------------------------------*/



/*--------------------------FUNCTIONS IN: parsing.h---------------------------*/
void lookForKeyword(char* keyword, char* dstBuff, int numToSkip, int numToPrint);
void moveBuffToString();
int compareStrings(char s1[FIFO_BUFFER_SIZE], char* s2);
void concatStringsWifi(char *ssid, char *pw);
void concatStringsServer(char *length, char *temp);
/*----------------------------------------------------------------------------*/



/*----------------------------FUNCTIONS IN: rtcc.h----------------------------*/
void RTCC_Initialize(void);
void RTCC_TimeGet(void);
/*----------------------------------------------------------------------------*/



/*---------------------------FUNCTIONS IN: timers.h---------------------------*/
void init32bitTimer9(void);
void init32bitTimer7(void);
void init32bitTimer5(void);
/*----------------------------------------------------------------------------*/



/*----------------------------FUNCTIONS IN: LCD.h-----------------------------*/
void initLCD(void);
char readLCD(int addr);
void writeLCD(int addr, char c);
void putsLCD(char *s);
void initPMP(void);
void setCursor(int row, int col);
void clearLCD(void);
void lcdSplashScreen(void);
/*----------------------------------------------------------------------------*/



/*----------------------------FUNCTIONS IN: UART.h----------------------------*/
void initUART1(void);
void txUART1(char c);
void txString1(char *s);
void initUART2(void);
void txUART2(char c);
void txString2(char *s);
void serialMode(void);
void clearUART1DataBuff(void);

/*----------------------------------------------------------------------------*/




