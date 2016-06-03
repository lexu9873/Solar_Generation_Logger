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

#ifndef LCDLOGGER_h
#define LCDLOGGER_h

#include "Arduino.h"
#include "LiquidCrystal.h"
#include "rtc_clock.h"
#include "Wire.h"

class LCDLOGGER
{
  public:
    LCDLOGGER(float Ra, float Rb, int SensorPin,int PowerPin, int InterruptPin, int refreshrate,int state);
    void InterruptHandler(); 
    void TimeToLCD();
    void DataToLCD();
  private:
    float _Ra;
    float _Rb;
    int _SensorPin;
    int _InterruptPin;
    int _PowerPin;
    int _refreshrate;
  volatile:
    int _state;
};

#endif