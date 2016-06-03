/*
  File:          SDLOGGER.cpp
  Description:   source file for logging real-time clock , solar power generation and working status to SD card. 
  Date:          1st. JUNE, 2016.
  License:       This work is licensed under the Creative Commons Attribution 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
*/

/*   
   Libraries cited as follows:
 * <rtc_clock.h>:     https://github.com/MarkusLange/Arduino-Due-RTC-Library (No license found in this library folder, but it should be copyleft or permissive use.)
 * <SD.h>:            https://github.com/arduino-libraries/SD (GNU General Public License, for more details see:http://www.gnu.org/licenses/)
                      Copyright (C) 2009 by William Greiman  Copyright (c) 2010 SparkFun Electronics
   Part of code is referenced to: https://github.com/Electromania/Arduino-Due/blob/master/Quick-Projects/Clock%20and%20calendar%20on%20LCD%20using%20Arduino%20Due%20internal%20RTC
*/
#include "Arduino.h"
#include "SDLOGGER.h"
#include "SD.h"
#include "rtc_clock.h"

SDLOGGER::SDLOGGER(float Ra, float Rb, int SensorPin, int CSpin, int PowerPin, String filename)
{    
	_CSpin = CSpin;
    _SensorPin = SensorPin;
    _PowerPin = PowerPin;
    _Ra=Ra;
    _Rb=Rb;
    _filename = filename;
	
    Serial.begin(9600);
    //initialize sd card
    Serial.print("Initializing SD card...");
    if (!SD.begin(_CSpin)) {

        Serial.println("initialization failed!");

        return;

    }
    Serial.println("initialization done.");
 
    //setup rtc to get real time clock
    rtc_clock.init();
    rtc_clock.set_clock(__DATE__, __TIME__);
}

void SDLOGGER::DataToSD()
{  
	String warning = "not working";
	String dataerror = "fail to log data";
    float volt = analogRead(_PowerPin) * (3.3 / 1023.0); // output voltage (V)
    float sensor = analogRead(_SensorPin) * (3.3 / 1023.0); // read from light sensor
    float power = 1000000 * (1/5000) * (1 + _Ra/_Rb) * (1 + _Ra/_Rb) * volt * volt ; //uW, with 5Kohm potentiometer.

	File myFile = SD.open(_filename ,FILE_WRITE);
    if(myFile){
       if ((volt<0.1&&sensor>0.05)||(volt>0.1&&sensor<0.05 )){
         //error checking
         myFile.print(warning);
       }
       else{
         myFile.print(power);
       }
       myFile.println(sensor);
       myFile.close();
    }
    else {
       Serial.println(dataerror);
    }
}

void SDLOGGER::DateTimeToSD()
{
	Char* daynames[]={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
	int hh,mm,ss,dow,dd,mon,yyyy;
	
    rtc_clock.get_time(&hh,&mm,&ss); 
    rtc_clock.get_date(&dow,&dd,&mon,&yyyy);

	String timeerror = "fail to log time";

	File myFile = SD.open(_filename ,FILE_WRITE);	
    if(myFile){
  
          // Print day and date
          myFile.print(daynames[dow-1]);   // display Day name
          myFile.print(",");
          if(dd <10) {myFile.print("0"); } // to display in 01,02,03... format
          myFile.print(dd);                // display date
          myFile.print("/");
          if(mm <10) {myFile.print("0"); }
          myFile.print(mon);               // display month
          myFile.print("/");
          myFile.println(yyyy);
          myFile.print(",");
 
          // Print time
          if(hh <10) {myFile.print("0"); }      // to display in 01,02,03... format
          myFile.print(hh);                      // display hours
          myFile.print(":");
    
          if(mm <10) {myFile.print("0"); }
          myFile.print(mm);                     // display minutes
          myFile.print(":");

          if(ss <10) {myFile.print('0'); }
          myFile.print(ss);                       // display seconds

          myFile.print(",");

          myFile.close();

    }
    else {
    Serial.println(timeerror);
    }
}