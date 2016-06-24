/* 
 * File:   parsing.h
 * Author: Zachary
 *
 * Created on May 24, 2015, 12:31 PM
 */
#include <xc.h>

/*FUNCTION DEFINITIONS*/

// 1st argument: the keyword you are searching for, defined above.
// 2nd argument: the address of the first element of the display buffer
// 3rd argument: the number of spaces to skip before storing chars
// 4th argument: the number of chars to store
void lookForKeyword(char* keyword, char* dstBuff, int numToSkip, int numToPrint)
{
    int i = 0;
    int j;
    int counter = 0;
    
    // clear data buffer
    for(j = 0; j<16; j++)
    {
        parsedDataBuffer[j] = '\0';
    }
    for(j = 0; j < 32; j++)
    {
        dstBuff[j] = '\0';
    }
    
    while(U2_rx_fifo.data_buf[i] != '\0')
    {
        if(U2_rx_fifo.data_buf[i] == keyword[0])
        {
            counter = 0;
            while(U2_rx_fifo.data_buf[i] == keyword[counter])
            {
                counter++;
                if(keyword[counter] == '\0')
                {
                    for(j = 0; j<numToSkip; j++)
                    {
                        i++;
                    }
                    for(j = 0; j<numToPrint; j++)
                    {
                        i++;
                        if(U2_rx_fifo.data_buf[i] == '<')
                        {
                            break;
                        }
                        else
                        {
                            parsedDataBuffer[j] = U2_rx_fifo.data_buf[i];
                        } 
                    }
                    break;
                }
                i++;
            }
        }
        else{i++;}
    }
    moveBuffToString(dstBuff); // moves parsedDataBuffer to dstBuff
}

void moveBuffToString(char *stringDestination)
{
    int i = 0;
    while(parsedDataBuffer[i] != '\0')
    {
        stringDestination[i] = parsedDataBuffer[i];
        i++;
    }
}

int compareStrings(char* s1, char* s2)
{
    int i = 0;
    while(s1[i] != '\0')
    {
        if(tolower(s1[i]) != tolower(s2[i]))
        {
            break;
        }
        i++;
        if(s1[i] == '\0')
        {
            return 0;
        }
    }
    return 1;
}

void concatStringsWifi(char *ssid, char *pw)
{
    int i;
    for(i = 0; i < 256; i++)
    {
        wifiInfo[i] = '\0';
    }
    
    //strcpy(result, s1);
    strcpy(&wifiInfo[0], wifiInfo_0);
    strcat(&wifiInfo[0], ssid);
    strcat(&wifiInfo[0], wifiInfo_1);
    strcat(&wifiInfo[0], pw);
    strcat(&wifiInfo[0], wifiInfo_2);
}

void concatStringsServer(char *length, char *temp)
{
    int i;
    for(i = 0; i < 256; i++)
    {
        wifiInfo[i] = '\0';
    }
    
    strcpy(&wifiInfo[0], skPOST_0);
    strcat(&wifiInfo[0], length);
    strcat(&wifiInfo[0], skPOST_1);
    strcat(&wifiInfo[0], temp);
    strcat(&wifiInfo[0], wifiInfo_2);
}