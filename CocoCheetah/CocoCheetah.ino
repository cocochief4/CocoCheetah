#include <math.h>

#define LRpin 12
#define FBpin 13
#define turnPin 45
 
// the actual values for "fast" and "slow" depend on the motor
#define PWM_SLOW 100  // arbitrary slow speed PWM duty cycle
#define PWM_FAST 200 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000 // brief delay for abrupt motor changes

#define motorArrCount 4

struct PolarCoord {
  float r;
  float theta;
  float turn;
};
struct motor {
  int pinDir;
  int pinPwm;
  float thetaShift;
  float force;
  float turnMultiplier;
};

motor motorArr[4] = {
  { 11, 10, 45, 0, -1 }, //Motor B (FR)
  { 9, 8, 135, 0, 1 }, //Motor A (FL)
  { 4, 3, 45, 0, 1 }, //Motor C (BL)
  { 6, 5, 135, 0, -1 } //Motor D (BR)
  };

int LRval;
int FBval;
int turnVal;
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
  pinMode(turnPin, INPUT);
  Serial.begin(9600);
  Serial.println("Serial On");
}
 
void loop() {
  LRval = pulseIn(LRpin, HIGH);
  FBval = pulseIn(FBpin, HIGH);
  turnVal = pulseIn(turnPin, HIGH);
  polar = EuclidPolar(LRval, FBval, turnVal);
  MapCoord = String("(") + LRval + "," + FBval + ")" + " " + turnVal;
  Serial.println(String("Polar Coords: ") + polar.r + ", " + polar.theta);
  Serial.println(MapCoord);
  calcSine();
  scale();
  motorDrive();
}

PolarCoord EuclidPolar(int x, int y, int z) {
  float LRCoord = x - 1500;
  float FBCoord = y - 1500;
  float turnCoord = z - 1500;
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
  float turnPower = turnCoord/500;
  if (turnPower < 0.05 && turnPower > -0.05) {
    turnPower = 0;
  }
  PolarCoord coord = { r, theta, turnPower };
  return coord;
}

void calcSine() {
  for (int i = 0; i < motorArrCount; i++) {
    motorArr[i].force = polar.r * sin((polar.theta + motorArr[i].thetaShift)*PI/180);
  }
}

void scale() {
  float maxForce = 0;
  float maxTurnForce = 0;
  for (int i = 0; i < motorArrCount; i++) {
    if (maxForce < abs(motorArr[i].force)) {
      maxForce = abs(motorArr[i].force);
    }
  }
  //for (int i = 0; i < motorArrCount; i++) {
    //motorArr[i].force = motorArr[i].force + polar.turnPower * motorArr[i].turnMultiplier;
    //}
  if (maxForce > 0) {
    for (int i = 0; i < motorArrCount; i++) {
      motorArr[i].force = motorArr[i].force * polar.r/maxForce;
    }
  }
  //Adding in Turning
  for (int i = 0; i < motorArrCount; i++) {
    motorArr[i].force = motorArr[i].force + polar.turn * motorArr[i].turnMultiplier;
    if (maxTurnForce < abs(motorArr[i].force)) {
      maxTurnForce = abs(motorArr[i].force);
    }
  }
  if (maxTurnForce > 1) {
    for (int i = 0; i < motorArrCount; i++) {
      motorArr[i].force = motorArr[i].force * 1/maxTurnForce;
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
