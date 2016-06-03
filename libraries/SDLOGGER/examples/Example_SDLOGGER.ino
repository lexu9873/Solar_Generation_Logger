/*
  File:          Example_SDLOGGER.ino
  Description:   example file for logging real-time clock , solar power generation and working status to SD card. 
  Date:          1st. JUNE, 2016.
  License:       This work is licensed under the Creative Commons Attribution 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
*/

#include "SDlogger.h"

SDlogger(5000/3,10000/3,A0,A7,"DATALOG.csv");//R1,R2,light sensor pin, solar panel pin, filename

void setup() {

}

void loop() {
  SDlogger.DateTimeToSD();
  SDlogger.DataToSD();
  delay(1000); //refresh rate:1s
}
