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

aka our yellow cable proximity sensor

model # 873M-D18AV300-D4 A 

has a 30ms response time, 30mm - 300mm = 3cm-30cm , blind zone 0-3cm.

operating voltage 15-30VDC, output voltage 0..10VDC

uses cable 889D-F4AE-2

http://literature.rockwellautomation.com/idc/groups/literature/documents/in/873m-in001_-en-p.pdf

Power requirements: 12VDC or greater, output scales analog distance 0 to 10VDC.

Note: values are undefined for less than 3cm.

Alternative power methods on a FRC robot: you could try to power it using PMC? module of Robot, usually used for pneumatics, but if you are not using pneumantics, then you could use that power, or custom power ckt.
Note you **must** use a voltage divider on the read side, if you plan to bring power back into roboRio.

### LIDAR

#### Garmin

* [LIDAR-Lite v3](lidar-lite-v3.md)

###IR

####Sharp GP2Y0A21YK0F
