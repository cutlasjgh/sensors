#include <SPI.h>
#include <Pixy.h>
#include <LIDARLite.h>
#include <Wire.h>

Pixy pixy;
LIDARLite lidar;
uint16_t blocks;


#define X_CENTER ((PIXY_MAX_X-PIXY_MIN_X)/2)
void setup() {
  Serial.begin(9600);
  Serial.print("Starting...\n");
  pixy.init();
  lidar.begin(0, true);

}

void loop() {
  int dif;
  blocks = -1;
  char buffer[100];
  blocks = pixy.getBlocks();
  // PIXY STUFF
  String dir;
  static int i = 0;
  int CenterOfBlocks;
  if (blocks) {
    i++;
    if (i % 30 == 0) {
      //Serial.println("Block Count: " + String(blocks));


      if (blocks == 1) {
        CenterOfBlocks = pixy.blocks[0].x;
      } else if (blocks == 2) {
        CenterOfBlocks = (pixy.blocks[0].x + pixy.blocks[1].x) / 2;
      }
      if (blocks == 1 || blocks == 2) {
        dif = X_CENTER - CenterOfBlocks;
        if (dif < -5) {
          sprintf(buffer, "^%s~%s~%s", "PIXY", "direction", "right");
          Serial.print(buffer);
        } else if (dif > 5) {
          sprintf(buffer, "^%s~%s~%s", "PIXY", "direction", "left");
          Serial.print(buffer);
        } else {
          sprintf(buffer, "^%s~%s~%s", "PIXY", "direction", "straight");
          Serial.print(buffer);
        }

      }
      //LIDAR STUFF
      sprintf(buffer, "^%s~%s~%d", "LIDAR", "cm", lidar.distance());
      Serial.print(buffer);
    }
    
  }
}

