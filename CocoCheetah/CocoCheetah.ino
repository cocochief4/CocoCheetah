#include <math.h>

#define LRpin 12
#define FBpin 13
 
// the actual values for "fast" and "slow" depend on the motor
#define PWM_SLOW 100  // arbitrary slow speed PWM duty cycle
#define PWM_FAST 200 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000 // brief delay for abrupt motor changes

#define motorArrCount 4

struct PolarCoord {
  float r;
  float theta;
};
struct motor {
  int pinDir;
  int pinPwm;
  float thetaShift;
  float force;
};

motor motorArr[4] = {
  { 10, 11, 45, 0 }, //Motor B (FR)
  { 8, 9, 135, 0 }, //Motor A (FL)
  { 3, 4, 45, 0 }, //Motor C (BL)
  { 5, 6, 135, 0 } //Motor D (BR)
  };

int LRval;
int FBval;
String MapCoord;
PolarCoord polar;

void setup() {
  for (int i = 0; i < motorArrCount; i++) {
    pinMode( motorArr[i].pinDir, OUTPUT );
    pinMode( motorArr[i].pinPwm, OUTPUT );
    digitalWrite( motorArr[i].pinDir, LOW );
    digitalWrite( motorArr[i].pinPwm, LOW );
  }
  
  pinMode(LRpin, INPUT);
  pinMode(FBpin, INPUT);
  Serial.begin(9600);
  Serial.println("Serial On");
}
 
void loop() {
  LRval = pulseIn(LRpin, HIGH);
  FBval = pulseIn(FBpin, HIGH);
  polar = EuclidPolar(LRval, FBval);
  MapCoord = String("(") + LRval + "," + FBval + ")";
  Serial.println(String("Polar Coords: ") + polar.r + ", " + polar.theta);
  Serial.println(MapCoord);
  calcSine();
  scale();
  motorDrive();
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

void calcSine() {
  for (int i = 0; i < motorArrCount; i++) {
    motorArr[i].force = polar.r * sin((polar.theta + motorArr[i].thetaShift)*PI/180);
  }
}

void scale() {
  float maxForce = 0;
  for (int i = 0; i < motorArrCount; i++) {
    if (maxForce < abs(motorArr[i].force)) {
      maxForce = abs(motorArr[i].force);
    }
  }
  //add turning here
  if (maxForce > 0) {
    for (int i = 0; i < motorArrCount; i++) {
      motorArr[i].force = motorArr[i].force * polar.r/maxForce;
    }
  }
}

void motorDrive() {
  int intPWM = 0;
  int digital = LOW;
  for (int i = 0; i < motorArrCount; i++) {
    if (motorArr[i].force <= 0) {
      digital = LOW;
      } else {
      digital = HIGH;
      } 
    digitalWrite(motorArr[i].pinDir, digital);
    if (digital == LOW) {
      intPWM = abs(round(motorArr[i].force * 255));
    } else {
      intPWM = 255 - abs(round(motorArr[i].force*255));
    }
    analogWrite(motorArr[i].pinPwm, intPWM);
    Serial.println(String("index ") + i + " " + intPWM + " " + motorArr[i].force + " " + motorArr[i].pinDir);
  }
}
