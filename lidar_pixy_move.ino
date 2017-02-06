#include <SPI.h>
#include <Pixy.h>
#include <LIDARLite.h>
#include <Wire.h>
#include <Timer.h>

#define X_CENTER ((PIXY_MAX_X-PIXY_MIN_X)/2)
Pixy pixy;
LIDARLite lidar;
uint16_t blocks;
boolean debug = false;
Timer t;

void no() {
  int dif;
  blocks = -1;
  char buffer[100];
  blocks = pixy.getBlocks();
  // PIXY STUFF
  String dir;
  static int i = 0;
  int CenterOfBlocks;

  if (blocks) {
    //Serial.println("Block Count: " + String(blocks));


    if (blocks == 1) {
      CenterOfBlocks = pixy.blocks[0].x;
    } else if (blocks == 2) {
      CenterOfBlocks = (pixy.blocks[0].x + pixy.blocks[1].x) / 2;
    } else {
      CenterOfBlocks = 0;
    }
    //if (blocks == 1 || blocks == 2) {
    dif = X_CENTER - CenterOfBlocks;
    if (dif < -5) {
      sprintf(buffer, "^%s~%s~%s~", "PIXY", "direction", "right");
      Serial.print(buffer);
    } else if (dif > 5) {
      sprintf(buffer, "^%s~%s~%s~", "PIXY", "direction", "left");
      Serial.print(buffer);
    } else {
      sprintf(buffer, "^%s~%s~%s~", "PIXY", "direction", "straight");
      Serial.print(buffer);
    }
    if (debug) {
      Serial.println("");
    }

  }


  //LIDAR STUFF
  sprintf(buffer, "^%s~%s~%d~", "LIDAR", "cm", lidar.distance());
  Serial.print(buffer);
  if (debug) {
    Serial.println("");
  }
}
void setup() {
  Serial.begin(74880);
  pixy.init();
  if (debug) {
    Serial.println("PIXY ONLINE");
  }
  lidar.begin(0, true);
  if (debug) {
    Serial.println("LIDAR ONLINE");
  }
  t.every(20, no);

}

void loop() {

  t.update();

}





