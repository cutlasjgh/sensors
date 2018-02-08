# FRC Team 4533 Sensors

This repository contains all of the information and source code related to the
sensors utilized by Team 4533.  The documentation and source code found here is
meant to serve as a resusable resource for the team as well as others that
might find it helpful.

## Range Finders

### Ultrasonic

#### Maxbotix

* LV-MaxSonar-EZ4 
* HRLV family

2 Uno Sketch files: 
one for Analog Voltage measurement of distance **UnoMaxbotixAnalogMode.ino** 
and one sketch for Pulse Duration method of distance measurement **UnoMaxbotixPulseTimeMode.ino** 

Both sketches have #if 1 compiler switches in them, to allow you to use either the EZ or HRLV families of Maxbotix ultrasonic distance sensors. Set the #if to 1 to allow the EZ family and set the #if to 0 to use the HRLV family. The compiler switches are similar to #define or #ifdef #else #endif statements, read up on compiler if confused.

EZ sensors I tested with were the MB1040 only. 
HRLV sensor I tested with were the MB1013 MB1023 and MB1033. 

The reason a compiler switch was used is that the EZ is less precise, and so uses different scaling factor. Interesting fact is that this code uses the statistical mode/mean functions borrowed from Adafruit, thereby throwing out most erroneous readings.
I think the EZ accuracy is about 1inch, whereas the HRLV has about 5mm precision (5x that of the EZ).
Note you will find that if you use the Analog voltage method, you will have to calibrate your readings for EACH SENSOR, whereas if you use Pulse duration pin and methods, you will not have to calibrate (at least that's what I found).

#### Rockwell Automation

* Allen-Bradley 873M

### LIDAR

#### Garmin

* [LIDAR-Lite v3](lidar-lite-v3.md)

###IR

####Sharp GP2Y0A21YK0F
