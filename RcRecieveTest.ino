#include <math.h>

#define LRpin 12
#define FBpin 13

struct PolarCoord {
  float r;
  float theta;
};

int LRval;
int FBval;
String MapCoord;
PolarCoord polar;

void setup() {
  pinMode(LRpin, INPUT);
  pinMode(FBpin, INPUT);
  Serial.begin(9600);
}
 
void loop() {
  LRval = pulseIn(LRpin, HIGH);
  FBval = pulseIn(FBpin, HIGH);
  polar = EuclidPolar(LRval, FBval);
  MapCoord = String("(") + LRval + "," + FBval + ")";
  Serial.println(String("Polar Coords: ") + polar.r + ", " + polar.theta);
  Serial.println(MapCoord);
}

PolarCoord EuclidPolar(int x, int y) {
  float LRCoord = x - 1500;
  float FBCoord = y - 1500;
  float r = sqrt(sq(LRCoord) + sq(FBCoord));
  r = r/500;
  float theta = -1 * atan2(LRCoord, FBCoord);
  theta = theta/PI * 180;
  if (r < 0.03) {
    r = 0;
  }
  if (r > 1 || r > 0.95) {
    r = 1;
  }
  PolarCoord coord = { r, theta };
  return coord;
}
