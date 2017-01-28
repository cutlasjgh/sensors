#include <Pixy.h>
#include <LIDARLite.h>
#include <Wire.h>

Pixy pixy;
LIDARLite lidar;
#define X_CENTER ((PIXY_MAX_X-PIXY_MIN_X)/2)
void setup() {
  Serial.begin(9600);
  Serial.print("Starting...\n");
  pixy.init();
  lidar.begin(0, true);
  
}

void loop() {
  uint16_t blocks;
  char buffer[100];
  blocks = pixy.getBlocks();
  // PIXY STUFF
  int dif = X_CENTER - pixy.blocks[0].x;
  String dir;
  if(dif < 0) {
    sprintf(buffer, "^%s~%s~%s", "PIXY", "direction", "right");
    Serial.println(buffer);
  }else if (dif > 0) {
    sprintf(buffer, "^%s~%s~%s", "PIXY", "direction", "left");
    Serial.println(buffer);
  }

  

  //LIDAR STUFF
  sprintf(buffer, "^%s~%s~%d", "LIDAR", "cm", lidar.distance());
  Serial.println(buffer);
}
