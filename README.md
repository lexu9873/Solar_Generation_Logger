# Solar_Generation_Logger
###Introduction

The solar generation logger design in this project is for the users of a solar panel, who desire to check the real-time power and working status by different techniques for various distances, i.e. on-the-spot observation (LCD), portable storage device (SD), application on Android phone (Bluetooth) and online dashboard (Plot.ly).

The system can not only log real-time solar power generation of a solar panel (mW level) to different devices, but it will also give a warning when the generated solar power is not as expected, after comparison to the light sensor value.

###Hardware Design

For hardware design, the high-level schematic diagram (Fig.1 in Appendix A) illustrates a MIMO (multiple-input-multiple-output) system, which contains both bidirectional and unidirectional data flow. Based on Fig.1, the full schematics created by EDA software is shown in Fig.2 of Appendix B.  Note that the power supply is 3.3V for most devices except for LCD, which is driven by 5V. Finally, the prototype is based on pin configuration in Fig,2, which is shown in Fig.3 of Appendix C.

###Software Design

For software design, the flowchart in Fig.4 of Appendix D demonstrates the control algorithm for solar generation logger. 

In initial design, there are three modes: normal mode, warning mode and history mode. Normally, the power will be displayed or logged on each device used. If light sensor returns large voltage (high light intensity) while the solar panel returns a very small value, or inversely, it will be in warning mode. But for history mode, the data storage can be read via a SD card reader directly. So there is no need to read data on SD card via Bluetooth. But Bluetooth can be still used as a real-time observation tool in normal and warning mode.

For LCD part, the highlight is that the power/warning display on LCD is interrupt driven. Initially, LCD displays real time on the first line and the day and date on second line. Once the pushbutton is pressed, the second line will be changed to power or warning according to the comparison result between light sensor and solar panel. If the pushbutton is pressed again, the second line will show date and day again.

For SD card part, it always logs the light sensor voltage value for further checking if a warning is given. For example, if a high voltage is logged, it means in the daytime. Then, check whether the connection of solar panel is broken or the light source is blocked from the solar panel.

For Bluetooth part, the data refresh rate is set at 5 seconds. This data stream can be compared with that sent into Plot.ly dashboard because the refresh rate for both is the same.

For visualized data on dashboard, IoT (Internet of Things) technique is used for sending data from the serial port on Arduino to Plot.ly dashboard. On mac, node.js was installed from https://nodejs.org/en/ and run on mac terminal. By writing a JavaScript file, the data stream on Serial Port can be logged to mac terminal. Also, the data stream can be sent to Plot.ly dashboard by using user-name, API keys, tokens and serial port name on the mac. 

The complete code can be found on the link given in Appendix E. There are two folders on Git Hub. One includes a structured Arduino code file with a server file written in JavaScript, another includes files written in object oriented structure as reusable Arduino libraries with examples of solar power logger for SD card and LCD only. 

###Implementation & Debugging

During the implementation, there are some problems encountered but solved by specific approaches as follows:

1.	Read analog value of solar panel voltage: Initially, the voltage read from solar panel was always the maximum voltage 3.3V. That is because the real voltage of panel is higher than 3.3V. So a 5Kohm potentiometer is used to reduce the input voltage lower than 3.3V. Also, the V- pin of panel was not connected to the ground, which resulted in unstable readings from the analog pin. After debugging, the solar panel could work as expected.
2.	SD card connection to Arduino DUE: Initially, without the knowledge of SPI protocol, the pins of SD card were all connected to digital pins of DUE. But according to the datasheet of DUE, there is a ICSP header for SPI connection. After debugging, SD card worked.
3.	Trouble in writing interrupt handler: Initially, the interrupt flag was not initialized and switched in the main loop. That is why the program failed to go back to the main loop after executing the interrupt handler. After that, the debouncing filter setting was also added to make sure the quick response of LCD to the pushbutton falling edge.
4.	Problems with server.js: Sometimes ‘Serial Port Busy’ error popped up when trying to run this file. The solution is to add some scrolling texts in setup() for LCD, during which the Serial Port is released and accessible for server.js. After scrolling texts, the data can be transmitted to terminal and plotly dashboard. Also note that when trying to see the data stream on terminal, the serial port window should be closed. Otherwise, this kind of error will appear again.

###Conclusion

To summarize, the system design can not only realize the basic function of logging solar power generation onto different devices, including SD card, LCD, Android phone (via Bluetooth), but an interrupt-driven LCD display and light sensor for warning checking are also featured. Furthermore, the mac is used as the server side to transmit real-time data stream from Serial Port to Plot.ly dashboard via a JavaScript written server.

###Reference

Library <rtc_clock.h>:     https://github.com/MarkusLange/Arduino-Due-RTC-Library (No license found in this library folder, but it should be copyleft or permissive use.)

Library  <SD.h>:            https://github.com/arduino-libraries/SD 
(GNU General Public License with Copyright (C) 2009 by William Greiman  Copyright (c) 2010 SparkFun Electronics)

Library <LiquidCrystal.h>:  https://github.com/arduino-libraries/LiquidCrystal 
(GNU General Public License, Copyright © 2006-2008 Hans-Christoph Steiner. All rights reserved. Copyright (c) 2010 Arduino LLC. All right reserved.)
 
Print Date and Time to LCD using Arduino Due internal RTC:
https://github.com/Electromania/Arduino-Due/blob/master/Quick-Projects/Clock and calendar on LCD using Arduino Due internal RTC
(CC0 1.0 Universal license. For more information, please see: http://creativecommons.org/publicdomain/zero/1.0/)

IoT (Internet of Things) tutorial for node.js server: http://adilmoujahid.com/posts/2015/07/practical-introduction-iot-arduino-nodejs-plotly/

###Appendix A High Level Schematics of Hardware Design

![alt tag](https://cloud.githubusercontent.com/assets/19504324/15778726/51e1bf16-29db-11e6-9511-540d4ad64f82.png)

Figure 1 High Level Schematics of Solar Generation Logger

###Appendix B Full Schematics of Hardware Design

![alt tag](https://cloud.githubusercontent.com/assets/19504324/15778724/51dd1d76-29db-11e6-92c3-8f4ce0a5ff3d.png)

Figure 2 Full Schematics of Solar Generation Logger

###Appendix C Prototype Demonstration

![alt tag](https://cloud.githubusercontent.com/assets/19504324/15778725/51e0a838-29db-11e6-8b7d-30c50703570c.png)

Figure 3 Prototype of Solar Generation Logger

Based on the hardware prototype in Fig.3, the demonstration video can be referenced to this YOUTUBE link: https://www.youtube.com/watch?v=zB6wNe_9MjM.

###Appendix D Control Algorithm (Flowchart)

![alt tag](https://cloud.githubusercontent.com/assets/19504324/15778745/78f8eafc-29db-11e6-8b56-8aef00872712.png)

Figure 4 Control Algorithm of Solar Generation Logger

###Appendix E Complete Code of Software Design

The code of this project with full documentation and license has been published on Git Hub, including one folder with a reusable library of SDLOGGER and LCDLOGGER and another folder with the complete structured Arduino code and a JavaScript file of node server. 

For more details, refer to: https://github.com/lexu9873/Solar_Generation_Logger
