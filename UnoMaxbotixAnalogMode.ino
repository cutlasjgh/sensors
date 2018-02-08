/* uno reading maxbotix proximity sensors

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


// this constant won't change.  It's the pin number
// of the sensor's output:
//const int pingPin = 7;
const int EZ1pin = A1;  // maxbotix analog voltage out pin. see loop() section for different scaling depending on sensor.
int8_t arraysize = 9; // quantity of values to find the median (sample size). Needs to be an odd number
//declare an array to store the samples. not necessary to zero the array values here, it just makes the code clearer
uint16_t rangevalue[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint16_t modE;        // calculated median distance

float VCC=5000.0; // mv here, and mv from analog read.. keep same units.
    // ===== EZ such as MB1040, it is 58uS per mm for range up to 6.45m with 1 inch resolution
    //Pin 2-PW- This pin outputs a pulse width representation of range. 
    //The distance can be calculated using the scale factor of 147uS per inch.
    //Pin 3-AN- Outputs analog voltage with a scaling factor of (Vcc/512) per inch. 
    //A supply of 5V yields ~9.8mV/in = 3.85mV/cm. and 3.3V yields ~6.4mV/in. 
    //The output is buffered and corresponds to the most recent range data.
    //Change to ZERO if you don't have an EZ model.
#if 0  // put 1 here if you using EZ models, and a 0 here if you using HRLV models
//float scalingFactor; // = (VCC / 512 )/2.54  ; // for greenboard maxbotix maxsonar ez, volts per cm
float scalingFactor = 0.77519; // for 5V VCC, and cm conversions. 
#else
    // ===== HRLV such as 1013, 1023, 1033, it is 1uS per mm for range up to 5000mm with 1mm resolution
    //Pin 2- Pulse Width Output: This pin outputs a pulse width representation of the distance with a scale factor 
    //of 1uS per mm. Output range is 300uS for 300-mm to 5000uS for 5000-mm. Pulse width output is +/- 1% of 
    //the serial data sent.
    //Pin 3- Analog Voltage Output: On power-up, the voltage on this pin is set to 0V, after which, the voltage 
    //on this pin has the voltage corresponding to the latest measured distance. 
    //This pin outputs an analog voltage scaled representation of the distance with a scale factor of (Vcc/1024) per 5-mm.
    //Actually, just multiply the number of bits in the value by 5 to yield the range in mm
float scalingFactor = VCC / 1024  /2.466 ; // for blackboard maxbotix maxsonar HRLV, !! volts per cm for HRLV models
// VCC/1024 = volts / 5mm but i div by 2 to make it volts/cm
#endif

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // wake screen
  Serial.print("Analog voltage test Maxbotix ");
  //pinMode(EZ1pin, INPUT); // Set ultrasonic sensor pin as input
}



void loop() {
 
  String stringOut = ": ";
  
  // read the input on analog pin 1 is gryo temp: (value is mV)
  float tempIn = analogRead(EZ1pin);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float distance = tempIn / scalingFactor;

  Serial.print("distance is: ");
  stringOut += distance;
  stringOut += " cm, raw: ";
  stringOut += tempIn;
  Serial.println(stringOut);

  delay(500);                        // Read every half second
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

