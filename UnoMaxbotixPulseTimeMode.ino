/* 

   modified by jec to support maxbotix sensor
  adding display printouts, and serial monitor is still there.
   
   The circuit:
	* +V to maxbotix to V pin of UNO
	* GND connection of the maxbotix attached to ground of UNO
	* AN(analog out) connection of the maxbotix  attached to unconnected as we are using pulse width output
  * PWO pulse width output connection of the maxbotix  attached to digital pin 7 of UNO
  * no resistors needed with UNO as it's 5 volt native unless you switch it to 3.3V.

NOTE you must customize the sensor type. The EZ models have different scaling factors than the HRLV models.
See #ifdef sections in loop

   refer to 
   https://learn.adafruit.com/trinket-ultrasonic-rangefinder/debugging-and-going-further?view=all
   https://learn.adafruit.com/ultrasonic-ruler/overview?view=all
   
   
 */
/* Board settings 
 *  board: uno
 *  CPU 80Mhz
 *  flash: 4M (3M SPIFFS) 
 *  upload speed: 115200
 *  programmer AVRISP mkII
 *  */
#include <Wire.h>
extern "C" {
//#include <user_interface.h>
}


// this constant won't change.  It's the pin number
// of the sensor's output:
//const int pingPin = 7;
const int EZ1pin = 7;  // maxbotix pulse duration pin, see loop() section for different scaling depending on sensor.
// These values are for calculating a mathematical median for a number of samples as
// suggested by Maxbotix instead of a mathematical average
int8_t arraysize = 9; // quantity of values to find the median (sample size). Needs to be an odd number
//declare an array to store the samples. not necessary to zero the array values here, it just makes the code clearer
uint16_t rangevalue[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint16_t modE;        // calculated median distance

void setup() {
  
  // initialize serial communication:
  Serial.begin(9600);
  
  Serial.print("Hello pulse duration Maxbotix ");
  pinMode(EZ1pin, INPUT); // Set ultrasonic sensor pin as input
}



void loop() {
 
  int16_t pulse;  // number of pulses from sensor
  int i=0;
  String stringOut = ": ";
  
  while( i < arraysize )
  {                    
    pulse = pulseIn(EZ1pin, HIGH);  // read in time for pin to transition
    // ===== EZ such as MB1040, it is 58uS per mm for range up to 6.45m with 1 inch resolution
    //Pin 2-PW- This pin outputs a pulse width representation of range. 
    //The distance can be calculated using the scale factor of 147uS per inch.
    //Pin 3-AN- Outputs analog voltage with a scaling factor of (Vcc/512) per inch. 
    //A supply of 5V yields ~9.8mV/in. and 3.3V yields ~6.4mV/in. 
    //The output is buffered and corresponds to the most recent range data.
    //Change to ZERO if you don't have an EZ model.
    #if 1   // put 1 here if you using EZ models, and a 0 here if you using HRLV models
    rangevalue[i]=pulse/58;         // pulses to centimeters =58uS (divide by 147uS for inches)
    if( rangevalue[i] < 645 && rangevalue[i] >= 15 ) i++;  // ensure no values out of range
    // ===== HRLV such as 1013, 1023, 1033, it is 1uS per mm for range up to 5000mm with 1mm resolution
    //Pin 2- Pulse Width Output: This pin outputs a pulse width representation of the distance with a scale factor 
    //of 1uS per mm. Output range is 300uS for 300-mm to 5000uS for 5000-mm. Pulse width output is +/- 1% of 
    //the serial data sent.
    //Pin 3- Analog Voltage Output: On power-up, the voltage on this pin is set to 0V, after which, the voltage on this pin has the voltage corresponding to the latest measured distance. 
    //This pin outputs an analog voltage scaled representation of the distance with a scale factor of (Vcc/1024) per 5-mm.
    //Actually, just multiply the number of bits in the value by 5 to yield the range in mm
    #else
    rangevalue[i]=pulse / 10;         // pulses to milliimeters =1uS, to cm =/10 (divide by 147uS for inches)
    if( rangevalue[i] < 500 && rangevalue[i] >= 15 ) i++;  // ensure no values out of range
    #endif
    delay(10);                      // wait between samples
  }
  isort(rangevalue,arraysize);        // sort samples
  modE = mode(rangevalue,arraysize);  // get median


  Serial.print("distance is: ");
  stringOut += modE;
  stringOut += " cm ";
  Serial.println(stringOut);

  delay(500);                        // Read every half second
}


long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}


// Sorting function (Author: Bill Gentles, Nov. 12, 2010)
void isort(uint16_t *a, int8_t n){
  for (int i = 1; i < n; ++i)  {
    uint16_t j = a[i];
    int k;
    for (k = i - 1; (k >= 0) && (j < a[k]); k--) {
      a[k + 1] = a[k];
    }
    a[k + 1] = j;
  }
}

// Mode function, returning the mode or median.
uint16_t mode(uint16_t *x,int n){
  int i = 0;
  int count = 0;
  int maxCount = 0;
  uint16_t mode = 0;
  int bimodal;
  int prevCount = 0;
  while(i<(n-1)){
    prevCount=count;
    count=0;
    while( x[i]==x[i+1] ) {
      count++;
      i++;
    }
    if( count > prevCount & count > maxCount) {
      mode=x[i];
      maxCount=count;
      bimodal=0;
    }
    if( count == 0 ) {
      i++;
    }
    if( count == maxCount ) {      //If the dataset has 2 or more modes.
      bimodal=1;
    }
    if( mode==0 || bimodal==1 ) {  // Return the median if there is no mode.
      mode=x[(n/2)];
    }
    return mode;
  }
}

