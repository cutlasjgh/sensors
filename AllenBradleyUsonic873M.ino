//collects data from an analog sensor
// contributed by team 4533 during 2018 competition build
/*AllenBradley 873M-D18AV300-D4
  30ms response time, 30mm - 300mm = 3cm-30cm , blind zone 0-3cm.
  operating voltage 15-30VDC, output voltage 0..10VDC
  Allen-Bradley 
  uses cable 889D-F4AE-2
  http://literature.rockwellautomation.com/idc/groups/literature/documents/in/873m-in001_-en-p.pdf


  refer to above pdf to see the connections.. we used two 2.2k ohm resistors in a voltage divider, between the 
  sensor's output = black wire
  and ground = blue wire.
  pull out your 5v signal from between the black and blue, but be sure to use voltage divider to change 
  range from 0-10 VDC to become 0-5VDC.
   
  trick is you need to power it by 15or greater DC.
  and then output scales to distance 0 to 10VDC.
  but values are undefined for less than 3cm.
  you could try to power it using PMC? module of Robot, usually used for pneumatics, but if 
  you are not usingpneumantics, then you could use that power, or custom power ckt.
  and a voltage divider on the read side.
  For a factory default rising ramp full range 3cm-30cm, found that:
  using volt meter, seems distance in cm is D = 5* (V+0.9) (where max V is 5)
  but when scaled 255 max comes in.. math is probably
  Dist(cm) = 0.098* val + 4.5
  But found that this worked better:   distance = 0.032 * (analogRead(RANGE_SENSOR_PIN)) + 3.5;
  */

/* constants */
#define RANGE_SENSOR_PIN    A0

int val = 0;                 // variable to store the raw value from sensor(initially zero)
int distance=0;          //distance in cm

void setup()
{
  pinMode( RANGE_SENSOR_PIN, INPUT);
  Serial.begin(9600);               // starts the serial monitor
}
 
void loop()
{
  //val = analogRead(sensorpin);       // reads the value of the sharp sensor
  distance = 0.032 * (analogRead(RANGE_SENSOR_PIN)) + 3.5;

  if(distance > 30) distance = 30;
  else if(distance < 3) distance = 3;
  
  Serial.println( distance );                         // print distance
  //Serial.println(analogRead(RANGE_SENSOR_PIN));            // prints raw value, for DEBUG only
  
  delay(400);                    // wait for this much time before printing next value
}
