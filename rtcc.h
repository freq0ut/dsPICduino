/* 
 * File:   rtcc.h
 * Author: Zachary
 *
 * Created on May 24, 2015, 12:31 PM
 */
#include <xc.h>

void RTCC_Initialize(void)
{
   // Turn on the secondary oscillator
   __builtin_write_OSCCONL(0x02);

   // Set the RTCWREN bit
   __builtin_write_RTCWEN();

   RCFGCALbits.RTCEN = 0;

   // set Mon Mar 23 09:57:41 MST 2015
   RCFGCALbits.RTCPTR = 3;        // start the sequence
   RTCVAL = 0x16;    // YEAR
   RTCVAL = 0x0529;    // MONTH-1/DAY-1
   RTCVAL = 0x0613;    // WEEKDAY/HOURS
   RTCVAL = 0x2500;    // MINUTES/SECONDS

   // RTSECSEL Alarm Pulse; 
   PADCFG1 = 0x0000;
           
   // Enable RTCC, clear RTCWREN
   RCFGCALbits.RTCEN = 1;
   // RCFGCALbits.RTCOE = 1;
   // PADCFG1bits.RTSECSEL = 1;
   RCFGCALbits.RTCWREN = 0;
   RTCC_TimeGet();
}

void RTCC_TimeGet(void)
{
    uint16_t register_value;

    // Set the RTCWREN bit
    __builtin_write_RTCWEN();

    RCFGCALbits.RTCPTR = 3;
    register_value = RTCVAL;
    times.year = ((((register_value & 0x00FF) & 0xF0) >> 4)* 10 + ((register_value & 0x00FF) & 0x0F));
    RCFGCALbits.RTCPTR = 2;
    register_value = RTCVAL;
    times.month = (((((register_value & 0xFF00) >> 8) & 0xF0) >> 4)* 10 +  (((register_value & 0xFF00) >> 8) & 0x0F));
    times.date = ((((register_value & 0x00FF) & 0xF0) >> 4)* 10 + ((register_value & 0x00FF) & 0x0F));
    RCFGCALbits.RTCPTR = 1;
    register_value = RTCVAL;
    times.day = (((((register_value & 0xFF00) >> 8) & 0xF0) >> 4)* 10 +  (((register_value & 0xFF00) >> 8) & 0x0F));
    times.hour = ((((register_value & 0x00FF) & 0xF0) >> 4)* 10 + ((register_value & 0x00FF) & 0x0F));
    RCFGCALbits.RTCPTR = 0;
    register_value = RTCVAL;
    times.min = (((((register_value & 0xFF00) >> 8) & 0xF0) >> 4)* 10 +  (((register_value & 0xFF00) >> 8) & 0x0F));
    times.sec = ((((register_value & 0x00FF) & 0xF0) >> 4)* 10 + ((register_value & 0x00FF) & 0x0F));
    
    RCFGCALbits.RTCWREN = 0;
}