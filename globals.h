/* 
 * File:   globals.h
 * Author: Zachary
 *
 * Created on May 24, 2015, 12:31 PM
 */
#include <xc.h>

#define FIFO_BUFFER_SIZE 256
#define TRUE    1           // define a true value of 1
#define FALSE   0           // define a false value of 0

typedef struct 
{
    char data_buf[FIFO_BUFFER_SIZE]; // FIFO buffer
    int i_first;                    // index of oldest data byte in buffer
    int i_last;                     // index of newest data byte in buffer
    int num_bytes;                  // number of bytes currently in buffer
}fifo_UART1;

fifo_UART1 U1_rx_fifo = { {0}, 0, 0, 0 }; // declare a receive software buffer
fifo_UART1 U1_tx_fifo = { {0}, 0, 0, 0 }; // declare a transmit software buffer

typedef struct 
{
    char data_buf[4096];            // FIFO buffer
    int i_first;                    // index of oldest data byte in buffer
    int i_last;                     // index of newest data byte in buffer
    int num_bytes;                  // number of bytes currently in buffer
}fifo_UART2;

fifo_UART2 U2_rx_fifo = { {0}, 0, 0, 0 }; // declare a receive software buffer
fifo_UART2 U2_tx_fifo = { {0}, 0, 0, 0 }; // declare a transmit software buffer

typedef unsigned char BOOL; // typedef unsigned char to BOOL
typedef struct my_booleans  // create a structure of booleans
{
  BOOL repeatMode:1;
  BOOL timeMode:1;
  BOOL directCommand:1;
  BOOL failedWifi:1;
  BOOL bootComplete:1;
  BOOL reconnectAttempt:1;
  BOOL wifiEntry:1;
  BOOL wifiInfoSSID:1;
  BOOL wifiInfoPW:1;
  BOOL stopWifiChecks:1;
  BOOL apListEntry:1;
  BOOL connectOnBoot:1;
}
my_booleans_t;
my_booleans_t boolean;

typedef struct
{	/* date and time components */
	uint16_t sec;
	uint16_t min;
	uint16_t hour;
	uint16_t date;
	uint16_t month;
	uint16_t year;
	uint16_t day;
}
tm;
tm times;

int pos = 0;


int httpError = 1;

static char* checkConnection = "print(wifi.sta.status())";

static char* setMode = "wifi.setmode(wifi.STATION)";
static char* listAP_function = "function listap(t)";
static char* listAP_for = "for k,v in pairs(t) do";
static char* listAP_print = "print(k..\" : \"..v)";
static char* listAP_end = "end";
static char* listAP_getAP = "wifi.sta.getap(listap)";

static char* wifiInfo_0 = "wifi.sta.config(\"";
static char* wifiInfo_1 = "\",\"";
static char* wifiInfo_2 = "\")";
static char* wifiDisconnect = "wifi.sta.disconnect()";
static char* wifiConnect = "wifi.sta.connect()";

static char* skCreateConn = "sk=net.createConnection(net.TCP, 0)";
static char* skOn = "sk:on(\"receive\", function(sck, c) print(c) end )";
static char* skConn = "sk:connect(80,\"api.thingspeak.com\")";
static char* skSendWeather = "sk:send(\"GET /apps/thinghttp/send_request?api_key=PT5411BJZQ166HUD HTTP/1.1\\r\\nHost: api.thingspeak.com\\r\\nConnection: keep-alive\\r\\nAccept: */*\\r\\n\\r\\n\")";
static char* skSendTime = "sk:send(\"GET /apps/thinghttp/send_request?api_key=3SJCIKZD34KKFIQB HTTP/1.1\\r\\nHost: api.thingspeak.com\\r\\nConnection: keep-alive\\r\\nAccept: */*\\r\\n\\r\\n\")";

char *c_time_string;
char unixTime[32];
char weekDay[32];
char Month[32];
char Date[32];
char Hour[32];
char Minute[32];
char Second[32];
char Year[32];

char SSID[64];
char password[64];
char wifiInfo[256];

unsigned long YearInt = 0;
unsigned long DateInt = 0;
unsigned long HourInt = 0;
unsigned long MinuteInt = 0;
unsigned long SecondInt = 0;

char numToString[32];

char displayTemp[32];
char displayFeel[32];
char displayHumidity[32];
char displayCondition[32];

char parsedDataBuffer[32];

char httpOKBuffer[32];