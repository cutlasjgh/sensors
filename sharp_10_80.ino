//Sharp GP2Y0A21YK0F Analog Distance Sensor 10-80cm
//collects data from an analog sensor

/* Project level constants */
#define RANGE_SENSOR_PIN    A0

#define RANGE_SENSOR_MODEL 1080

int val = 0;                 // variable to store the values from sensor(initially zero)
uint8_t distance=0;          //distance in cm

void setup()
{
  pinMode( RANGE_SENSOR_PIN, INPUT);
  Serial.begin(9600);               // starts the serial monitor
}
 
void loop()
{
  distance = 4800/(analogRead(RANGE_SENSOR_PIN)-20);

  if(distance > 80) distance = 81;
  else if(distance < 10) distance = 9;
  
  Serial.println( distance );
  
  delay(400);                    // wait for this much time before printing next value
}
