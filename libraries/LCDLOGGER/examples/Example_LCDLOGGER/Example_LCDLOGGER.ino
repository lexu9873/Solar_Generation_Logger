/*
  File:          Example_LCDLOGGER.ino
  Description:   example file for logging real-time clock , solar power generation and working status to LCD. 
  Date:          1st. JUNE, 2016.
  License:       This work is licensed under the Creative Commons Attribution 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
*/

#include <LCDLOGGER.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //address: EN - pin 7,RS - pin 8,DB11~14 - pin 9~12
LCDLOGGER lcdlogger(5000/3,10000/3,A0,A7,3,1000,0); //R1,R2,light sensor pin,solar panel pin, interrupt pin, refresh rate, initial state


void setup() {
}

void loop() {
  lcdlogger.TimeToLCD(); //on lcd first line: real time
  lcdlogger.DataToLCD(); //on lcd second line:date or data depending on pushbutton 
}
