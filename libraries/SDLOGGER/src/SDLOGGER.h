/*
  File:          SDLOGGER.h
  Description:   header file for logging real-time clock , solar power generation and working status to SD card.
  Author:        Lei XU, Jiawei ZHAO
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
#ifndef SDLOGGER_h
#define SDLOGGER_h

#include "Arduino.h"
#include "SD.h"
#include "rtc_clock.h"
#include "Wire.h"


class SDLOGGER
{
  public:
    SDLOGGER(float Ra, float Rb, int SensorPin, int CSpin, int PowerPin, String filename);
    void DateTimeToSD(); 
    void DataToSD();
  private:
    float _Ra;
    float _Rb;
    int _SensorPin;
	int _CSpin;
    int _PowerPin;
    String _filename;
    
};

#endif