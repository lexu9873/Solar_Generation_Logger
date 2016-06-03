/*
  File:          server.js
  Description:   node.js server file for logging data from serial port of arduino board through mac terminal to plotly website.
  Author:        Lei XU, Jiawei ZHAO
  Date:          1st. JUNE, 2016.
  License:       This work is licensed under the Creative Commons Attribution 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
*/

/*   
  This part of code is from a tutorial of IoT: http://adilmoujahid.com/posts/2015/07/practical-introduction-iot-arduino-nodejs-plotly/
*/
var serialport = require('serialport'),
    plotly = require('plotly')('user-name','key'),
    token = 'token';

var portName = '/dev/cu.usbmodem1411';
var sp = new serialport.SerialPort(portName,{
    baudRate: 9600,
    dataBits: 8,
    parity: 'none',
    stopBits: 1,
    flowControl: false,
    parser: serialport.parsers.readline("\r\n")
});

// helper function to get a nicely formatted date string
function getDateString() {
    var time = new Date().getTime();
    var datestr = new Date(time +36000000).toISOString().replace(/T/, ' ').replace(/Z/, '');
    return datestr;
}

var initdata = [{x:[], y:[], stream:{token:token, maxpoints: 500}}];
var initlayout = {fileopt : "extend", filename : "filename"};

plotly.plot(initdata, initlayout, function (err, msg) {
    if (err) return console.log(err)

    console.log(msg);
    var stream = plotly.stream(token, function (err, res) {
        console.log(err, res);
    });

    sp.on('data', function(input) {
        if(isNaN(input) || input > 1023) return;

    var streamObject = JSON.stringify({ x : getDateString(), y : input });
    console.log(streamObject);
    stream.write(streamObject+'\n');
    });
});