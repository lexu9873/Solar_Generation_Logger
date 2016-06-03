/*
  File:          LCDLOGGER.cpp
  Description:   source file for logging real-time clock , solar power generation and working status to LCD.
  Author:        Lei XU, Jiawei ZHAO
  Date:          1st. JUNE, 2016.
  License:       This work is licensed under the Creative Commons Attribution 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
*/

/*   
   Libraries cited as follows:
* <rtc_clock.h>:      https://github.com/MarkusLange/Arduino-Due-RTC-Library (No license found in this library folder, but it should be copyleft or permissive use.)
* <LiquidCrystal.h>:  https://github.com/arduino-libraries/LiquidCrystal (GNU General Public License, for more details see:http://www.gnu.org/licenses/)
                      Copyright © 2006-2008 Hans-Christoph Steiner. All rights reserved. Copyright (c) 2010 Arduino LLC. All right reserved.
   Part of code is referenced to: https://github.com/Electromania/Arduino-Due/blob/master/Quick-Projects/Clock%20and%20calendar%20on%20LCD%20using%20Arduino%20Due%20internal%20RTC
*/

#include "Arduino.h"
#include "LCDLOGGER.h"
#include "rtc_clock.h"
#include "LiquidCrystal.h"


LCDLOGGER::LCDLOGGER(float Ra, float Rb, int SensorPin,int PowerPin, int InterruptPin, int refreshrate, int state)
{
    _SensorPin = SensorPin;
    _PowerPin = PowerPin;
    _Ra=Ra;
    _Rb=Rb;
    _InterruptPin = InterruptPin;
    _refreshrate = refreshrate;
	_state = state;
		
    //setup interrupt with debouncing filter
    attachInterrupt(_InterruptPin , LCDLOGGER.InterruptHandler(), FALLING);
    REG_PIOC_WPMR=0x50494F00; // PIO Write Protect Mode Register: Disable the write protection
    REG_PIOC_IFER |= 1<<28; // Input Filter Enable Register: Digital pin 3 is mapped to PC28
    PIOC->PIO_DIFSR |= 1<<28; // Debouncing Input Filter Select Register: Digital pin 3 is mapped to PC28
    PIOC->PIO_SCDR |= 0x14; // Slow Clock Divider Register: Tclk=2(DIV+1)Tsl, where Tclk = 1.3us, Tsl=1/32KHz. Thus, DIV in decimal is 20, in binary is 00010100, in hex is 14

    //setup lcd to show two lines
    lcd.begin(16, 2);

    //setup rtc to get real time clock
    rtc_clock.init();
    rtc_clock.set_clock(__DATE__, __TIME__);
}

void LCDLOGGER::InterruptHandler()
{
    _state=!(_state);
}

void LCDLOGGER::TimeToLCD()
{
	int hh,mm,ss;
    rtc_clock.get_time(&hh,&mm,&ss);  // get time from Arduino RTC
    // Print time on LCD
    lcd.print("Time: ");
    if(hh <10) {lcd.print("0"); }      // to display in 01,02,03... format
    lcd.print(hh);                      // display hours
    lcd.print(":");

    if(mm <10) {lcd.print("0"); }
    lcd.print(mm);                     // display minutes
    lcd.print(":");

    if(ss <10) {lcd.print('0'); }
    lcd.print(ss);                       // display seconds
}

void LCDLOGGER::DataToLCD()
{
	Char* daynames[]={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
	int dow,dd,mon,yyyy;
    rtc_clock.get_date(&dow,&dd,&mon,&yyyy);   // get date from Arduino RTC
 
    String warning = "not working";
 
    volt = analogRead(_PowerPin) * (3.3 / 1023.0); // output voltage (V)
    sensor = analogRead(_SensorPin) * (3.3 / 1023.0); // voltage (V) read from light sensor
    power = 1000000 * (1/5000) * (1 + _Ra/_Rb) * (1 + _Ra/_Rb) * volt * volt ; //uW, with 5Kohm potentiometer.

    lcd.setCursor(0,1);
	
    switch(_state){
    case 1:
          //print date on lCD
          lcd.print(daynames[dow-1]);   // display Day name
          lcd.print(" ");
          if(dd <10) {lcd.print("0"); } // to display in 01,02,03... format
          lcd.print(dd);                // display date
          lcd.print("/");
          if(mm <10) {lcd.print("0"); }
          lcd.print(mon);               // display month
          lcd.print("/");
          lcd.print(yyyy);              // display year
          break;
    case 0:
          //print power on LCD if no error 
          if ((volt<0.1&&sensor>0.05)||(volt>0.1&&sensor<0.05)){
               lcd.print(warning);
          }
          else{
               lcd.print("power:");
               lcd.print(power);
               lcd.print("uW");
          }
          break;
    }
    delay(_refreshrate);
    lcd.clear();
}