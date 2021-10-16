#include <math.h>

#define LRpin 12
#define FBpin 13
#define turnPin 45
#define pistonInput 7
#define pistonPin 47
 
// the actual values for "fast" and "slow" depend on the motor
#define PWM_SLOW 100  // arbitrary slow speed PWM duty cycle
#define PWM_FAST 200 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000 // brief delay for abrupt motor changes

#define motorArrCount 4
#define MAX_MOTOR_SPEED 1

struct PolarCoord {
  float r;
  float theta;
  float turn;
};
struct motor {
  int a;
  int b;
  int speedControl;
  float thetaShift;
  float force;
  float turnMultiplier;
};

motor motorArr[4] = {
  { 22, 23, 2, 45, 0, -1 }, //Motor B (FR)
  { 24, 25, 3, 135, 0, 1 }, //Motor A (FL)
  { 26, 27, 4, 45, 0, 1 }, //Motor C (BL)
  { 28, 29, 5, 135, 0, -1 } //Motor D (BR)
  };

int LRval;
int FBval;
int turnVal;
int pistonVal;
String MapCoord;
PolarCoord polar;

void setup() {
  for (int i = 0; i < motorArrCount; i++) {
    pinMode( motorArr[i].a, OUTPUT );
    pinMode( motorArr[i].b, OUTPUT );
    digitalWrite( motorArr[i].a, LOW );
    digitalWrite( motorArr[i].b, LOW );
  }
  
  /*pinMode(enable1A, OUTPUT);
  pinMode(enable1B, OUTPUT);
  pinMode(enable2A, OUTPUT);
  pinMode(enable2B, OUTPUT);
  digitalWrite(enable1A, HIGH);
  digitalWrite(enable1B, HIGH);
  digitalWrite(enable2A, HIGH);
  digitalWrite(enable2B, HIGH);*/
  
  pinMode(LRpin, INPUT);
  pinMode(FBpin, INPUT);
  pinMode(turnPin, INPUT);
  pinMode(pistonInput, INPUT);
  pinMode(pistonPin, OUTPUT);
  digitalWrite(pistonPin, LOW);
  Serial.begin(9600);
  Serial.println("Serial On");
}
 
void loop() {
  pistonVal = pulseIn(pistonPin, HIGH);
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
  if (maxTurnForce > MAX_MOTOR_SPEED) {
    for (int i = 0; i < motorArrCount; i++) {
      motorArr[i].force = motorArr[i].force * MAX_MOTOR_SPEED/maxTurnForce;
    }
  }
}


void motorDrive() {
  int intPWM = 0;
  int digitalA = 0;
  int digitalB = 0;
  for (int i = 0; i < motorArrCount; i++)
  {
    if (motorArr[i].force < 0)
    {
      digitalA = LOW;
      digitalB = HIGH;
    } else if (motorArr[i].force > 0) {
      digitalA = HIGH;
      digitalB = LOW;
    } else {
      digitalA = LOW;
      digitalB = LOW;
    }
    
  digitalWrite(motorArr[i].a, digitalA);
  digitalWrite(motorArr[i].b, digitalB);
  intPWM = abs(round(motorArr[i].force * 255));
  analogWrite(motorArr[i].speedControl, intPWM);
  Serial.println(String("index ") + i + " " + motorArr[i].force + " " + digitalA + " " + digitalB + " " + intPWM);
  }
  /*int intPWM = 0;
  int digital = LOW;
  for (int i = 0; i < motorArrCount; i++) {
    if (motorArr[i].force <= 0) {
      digital = LOW;
      } else {
      digital = HIGH;
      } 
    digitalWrite(motorArr[i].a, digital);
    if (digital == LOW) {
      intPWM = abs(round(motorArr[i].force * 255));
    } else {
      intPWM = 255 - abs(round(motorArr[i].force*255));
    }
    analogWrite(motorArr[i].b, intPWM);
    Serial.println(String("index ") + i + " " + intPWM + " " + motorArr[i].force + " " + motorArr[i].a);
  }*/
}

void pistonDrive(int controller) {
  if (controller > 1100) {
    digitalWrite(pistonPin, HIGH);
  } else {
    digitalWrite(pistonPin, LOW);
  }
}
