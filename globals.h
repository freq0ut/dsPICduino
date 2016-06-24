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
    volatile char data_buf[FIFO_BUFFER_SIZE]; // FIFO buffer
    volatile int i_first;                    // index of oldest data byte in buffer
    volatile int i_last;                     // index of newest data byte in buffer
    volatile int num_bytes;                  // number of bytes currently in buffer
}fifo_UART1;

fifo_UART1 U1_rx_fifo = { {0}, 0, 0, 0 }; // declare a receive software buffer
fifo_UART1 U1_tx_fifo = { {0}, 0, 0, 0 }; // declare a transmit software buffer

typedef volatile struct 
{
    volatile char data_buf[4096];            // FIFO buffer
    volatile int i_first;                    // index of oldest data byte in buffer
    volatile int i_last;                     // index of newest data byte in buffer
    volatile int num_bytes;                  // number of bytes currently in buffer
}fifo_UART2;

fifo_UART2 U2_rx_fifo = { {0}, 0, 0, 0 }; // declare a receive software buffer
fifo_UART2 U2_tx_fifo = { {0}, 0, 0, 0 }; // declare a transmit software buffer

typedef unsigned char BOOL; // typedef unsigned char to BOOL
typedef struct my_booleans  // create a structure of booleans
{
  volatile BOOL repeatMode:1;
  volatile BOOL timeMode:1;
  volatile BOOL directCommand:1;
  volatile BOOL failedWifi:1;
  volatile BOOL bootComplete:1;
  volatile BOOL reconnectAttempt:1;
  volatile BOOL wifiEntry:1;
  volatile BOOL wifiInfoSSID:1;
  volatile BOOL wifiInfoPW:1;
  volatile BOOL stopWifiChecks:1;
  volatile BOOL apListEntry:1;
  volatile BOOL connectOnBoot:1;
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

volatile int httpError = 1;

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
static char* skConnThingSpeak = "sk:connect(80,\"api.thingspeak.com\")";
static char* skConnServer = "sk:connect(8000,\"10.0.0.10\")";
static char* skSendWeather = "sk:send(\"GET /apps/thinghttp/send_request?api_key=PT5411BJZQ166HUD HTTP/1.1\\r\\nHost: api.thingspeak.com\\r\\nConnection: keep-alive\\r\\nAccept: */*\\r\\n\\r\\n\")";
static char* skSendTime = "sk:send(\"GET /apps/thinghttp/send_request?api_key=3SJCIKZD34KKFIQB HTTP/1.1\\r\\nHost: api.thingspeak.com\\r\\nConnection: keep-alive\\r\\nAccept: */*\\r\\n\\r\\n\")";
static char* skPOST_0 = "sk:send(\"POST /brewing/log HTTP/1.1\\r\\nHost: 10.0.0.10:8000\\r\\nContent-Type: application/x-www-form-urlencoded\\r\\nContent-Length: ";
static char* skPOST_1 = "\\r\\n\\r\\ntemp=";

volatile char *c_time_string;
volatile char unixTime[32];
volatile char weekDay[32];
volatile char Month[32];
volatile char Date[32];
volatile char Hour[32];
volatile char Minute[32];
volatile char Second[32];
volatile char Year[32];

volatile char SSID[64];
volatile char password[64];
volatile char wifiInfo[256];
static char* tempLength = "7";

volatile unsigned long YearInt = 0;
volatile unsigned long DateInt = 0;
volatile unsigned long HourInt = 0;
volatile unsigned long MinuteInt = 0;
volatile unsigned long SecondInt = 0;

volatile char numToString[32];

volatile char displayTemp[32];
volatile char displayFeel[32];
volatile char displayHumidity[32];
volatile char displayCondition[32];

volatile char parsedDataBuffer[32];

volatile char httpOKBuffer[32];