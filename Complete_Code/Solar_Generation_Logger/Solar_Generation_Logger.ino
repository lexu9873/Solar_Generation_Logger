/* 
  File:          Solar Generation Logger.ino 
  Main Function: Log solar generation real-time data to LCD, SD card, Android phone(Bluetooth). 
  Other:         It can also Transmit data to plotly dashboard from Serial port through node.js server built on the mac terminal 
  Author:        Lei XU, Jiawei ZHAO 
  Date:          1st. JUNE, 2016. 
  License:       This work is licensed under the Creative Commons Attribution 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/. 
*/  
  
/*    
   Libraries cited as follows: 
 * <rtc_clock.h>:     https://github.com/MarkusLange/Arduino-Due-RTC-Library (No license found in this library folder, but it should be copyleft or permissive use.) 
 * <SD.h>:            https://github.com/arduino-libraries/SD (GNU General Public License, for more details see:http://www.gnu.org/licenses/) 
                      Copyright (C) 2009 by William Greiman  Copyright (c) 2010 SparkFun Electronics 
 * <LiquidCrystal.h>: https://github.com/arduino-libraries/LiquidCrystal (GNU General Public License, for more details see:http://www.gnu.org/licenses/) 
                      Copyright Â© 2006-2008 Hans-Christoph Steiner. All rights reserved. Copyright (c) 2010 Arduino LLC. All right reserved. 
   Part of code is referenced to: https://github.com/Electromania/Arduino-Due/blob/master/Quick-Projects/Clock%20and%20calendar%20on%20LCD%20using%20Arduino%20Due%20internal%20RTC 
*/  
#include <SD.h>  
#include <rtc_clock.h>  
#include <LiquidCrystal.h>    
  
RTC_clock rtc_clock(XTAL);  
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);   
  
char* daynames[]={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};  
int hh,mm,ss,dow,dd,mon,yyyy;  
  
#define BlueToothSerial Serial2  
  
File myFile;  
String filename = "datalog.csv";  
  
int SensorPin = A0;  
int PowerPin = A7;  
int InterruptPin = 3;  
int CSpin = 4;  
  
volatile int state = 0;  
  
float volt; // Solar panel output voltage (V)  
float power; // Solar panel output power (uW)  
float sensor; // Light sensor voltage (V)  
  
void setup()  
  
{  
    Serial.begin(9600);//setup serial baud rate for data transmission to mac terminal  
  
    //set up bluetooth connection  
    BlueToothSerial.begin(38400);                           // Set BluetoothBee BaudRate to default baud rate 38400  
    BlueToothSerial.print("\r\n+STWMOD=0\r\n");             // set the bluetooth work in slave mode  
    BlueToothSerial.print("\r\n+STNA=Solar panel\r\n");    // set the bluetooth name as "My Name is V"  
    BlueToothSerial.print("\r\n+STOAUT=1\r\n");             // Permit Paired device to connect me  
    BlueToothSerial.print("\r\n+STAUTO=0\r\n");             // Auto-connection should be forbidden here  
    BlueToothSerial.print("\r\n+STPIN=0000\r\n");           // Input pincode:0000  
    //wait for 2s and then make the slave bluetooth inquirable  
    delay(2000);                                              
    BlueToothSerial.print("\r\n+INQ=1\r\n");                  
    Serial.println("The slave bluetooth is inquirable");  
  
    // wait for 2s and flush the serial buffer  
    delay(2000);                                              
    BlueToothSerial.flush();  
  
    //setup interrupt with debouncing filter  
    attachInterrupt(InterruptPin , InterruptHandler, FALLING);  
    REG_PIOC_WPMR=0x50494F00; // PIO Write Protect Mode Register: Disable the write protection  
    REG_PIOC_IFER |= 1<<28; // Input Filter Enable Register: Digital pin 3 is mapped to PC28  
    PIOC->PIO_DIFSR |= 1<<28; // Debouncing Input Filter Select Register: Digital pin 3 is mapped to PC28  
    PIOC->PIO_SCDR |= 0x14; // Slow Clock Divider Register: Tclk=2(DIV+1)Tsl, where Tclk = 1.3us, Tsl=1/32KHz. Thus, DIV in decimal is 20, in binary is 00010100, in hex is 14  
  
    //setup lcd to show two lines  
    lcd.begin(16, 2);  
  
    //setup rtc to get real time clock  
    rtc_clock.init();  
    rtc_clock.set_clock(__DATE__, __TIME__);  
      
    //initialize sd card  
    Serial.print("Initializing SD card...");  
    if (!SD.begin(CSpin)) {  
  
        Serial.println("initialization failed!");  
  
        return;  
  
    }  
    Serial.println("initialization done.");  
}  
  
  
void loop(){  
     lcdtime();    //display time on LCD  
     sddatetime(); //log date and time on SD card  
     volt = analogRead(PowerPin) * (3.3 / 1023.0); // output voltage (V)  
     sensor = analogRead(SensorPin) * (3.3 / 1023.0); // read VOLT from light sensor  
   
     if ((volt<0.1&&sensor>0.05)||(volt>0.1&&sensor<0.05 )){  
         //error checking using if-else 
         warning(); //display the warning  
     }  
     else{  
         power =  450 * volt * volt ; //uW  
         powerdisplay(); //display power on SD card, dashboard, LCD and bluetooth terminal  
     }     
  
}  
void InterruptHandler(){  
    state=!state;  
}  
void warning(){  
  
    //warning on SD card  
    myFile = SD.open(filename,FILE_WRITE);  
    if(myFile){  
    myFile.print("not working");  
    myFile.println(sensor);  
    myFile.close();  
    }  
    else {  
    Serial.println("error opening file");  
    }  
  
    //refresh rate is 5 seconds

    if(ss%5==0){  
       Serial.println(0); //dashboard data logging:when error occurs, equal to 0     
    //print power on bt  
       if(BlueToothSerial.available()){  
          BlueToothSerial.print("\r\n-not working-\r\n");  
       }  
    }  
      
    switch(state)  
    {  
    case 0:  
        //warning on LCD  
        lcd.setCursor(0,1);  
        lcd.print("not working     ");  
        delay(1000);  
        lcd.clear();  
        break;  
    case 1:      
        lcddate();  
        break;  
    }  
}//display warning on SD, LCD & BLUETOOTH  
void powerdisplay(){  
    //warning on SD card  
    myFile = SD.open(filename,FILE_WRITE);  
    if(myFile){  
       myFile.print(power);  
       myFile.println(sensor);  
       myFile.close();  
    }  
    else {  
       Serial.println("error opening file");  
    }  
    //refresh rate is 5 seconds
    if(ss%5==0){  
       Serial.println(power);
       //print power on ANDROID app
       if(BlueToothSerial.available()){  
            
          BlueToothSerial.println(power);  
       }  
    }      
    //INTERRUPT DRIVEN  
    switch(state)  
    {  
    case 1:  
        // Print day and date   
        lcddate();  
        break;  
    case 0:  
        //print power on LCD  
        lcd.setCursor(0,1);  
        lcd.print("power:");  
        lcd.print(power);  
        lcd.print("uW");  
        delay(1000);  
        lcd.clear();  
        break;  
    }  
}//display power on SD, LCD, SERIAL&BLUETOOTH
  
void lcdtime(){  
    rtc_clock.get_time(&hh,&mm,&ss);  // get time from Arduino RTC  
    // Print time on LCD  
    lcd.print("Time: ");  
    if(hh <10) {lcd.print("0"); }      // to display in 01,02,03... format  
    lcd.print(hh);                      // display hours  
    lcd.print(":");  
  
    if(mm <10) {lcd.print("0"); }  
    lcd.print(mm);                     // display minutes  
    lcd.print(":");  
  
    if(ss <10) {lcd.print('0'); }  
    lcd.print(ss);                       // display seconds  
}  
  
void lcddate(){  
    lcd.setCursor(0,1);  
    lcd.print(daynames[dow-1]);   // display Day name  
    lcd.print(" ");  
    if(dd <10) {lcd.print("0"); } 
    lcd.print(dd);                // display date  
    lcd.print("/");  
    if(mm <10) {lcd.print("0"); }  
    lcd.print(mon);               // display month  
    lcd.print("/");  
    lcd.print(yyyy);              // display year  
    lcd.print("   ");  
    delay(1000);  
    lcd.clear();  
}//print date on LCD
  
void sddatetime(){  
      //log date time on SD card  
    rtc_clock.get_date(&dow,&dd,&mon,&yyyy);   // get date from Arduino RTC  
    myFile = SD.open(filename ,FILE_WRITE);  
    if(myFile){  
    
          // Print day and date  
          myFile.print(daynames[dow-1]);   // display Day name  
          myFile.print(",");  
          if(dd <10) {myFile.print("0"); } // to display in 01,02,03... format  
          myFile.print(dd);                // display date  
          myFile.print("/");  
          if(mm <10) {myFile.print("0"); }  
          myFile.print(mon);               // display month  
          myFile.print("/");  
          myFile.println(yyyy);  
          myFile.print(",");  
   
          // Print time  
          if(hh <10) {myFile.print("0"); }      // to display in 01,02,03... format  
          myFile.print(hh);                      // display hours  
          myFile.print(":");  
      
          if(mm <10) {myFile.print("0"); }  
          myFile.print(mm);                     // display minutes  
          myFile.print(":");  
  
          if(ss <10) {myFile.print('0'); }  
          myFile.print(ss);                       // display seconds  
  
          myFile.print(",");  
  
          myFile.close();  
  
    }  
    else {  
    Serial.println("error opening file");  
    }  
}  

