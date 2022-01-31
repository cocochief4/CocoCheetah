// Copyright 2021 Christopher Kao
// This is the Arduino program for my robo-soccer car. See it in action at https://www.youtube.com/watch?v=4JXyyVH9lPo.
// This code designed for Arduino Mega 2560, though a Arduino Mega should work as well.
// This code allows you to recieve input from a RF recievers and drive a mecanum wheel car.
#include <math.h>

#define LRpin 12 // Pin for coordinate for the x (left to right) from reciever on car (from 1000 - 2000)
#define FBpin 13 // Pin for coordinate for the y (forward to backward) from reciever on car (from 1000 - 2000)
#define turnPin 45 // Pin for value for how far to turn (left to right) from reciever on car (from 1000 - 2000)
#define pistonInput 7 // kept in here in case added piston, you can ignore
#define pistonPin 47  // ignore as well

#define motorArrCount 4 // Make it easier to change number of motors in case number increases or decreases.
#define MAX_MOTOR_SPEED 1 // Make it easier to change max speed cap.

struct PolarCoord {
  float r;
  float theta;
  float turn;
}; // struct to give polar coordinates in one variable
struct motor {
  int a;
  int b;
  int speedControl;
  float thetaShift;
  float force;
  float turnMultiplier;
}; // struct to give motor specs in one variable

motor motorArr[4] = {
  { 22, 23, 2, 45, 0, -1 }, //Motor B (FR)
  { 24, 25, 3, 135, 0, 1 }, //Motor A (FL)
  { 26, 27, 4, 45, 0, 1 }, //Motor C (BL)
  { 28, 29, 5, 135, 0, -1 } //Motor D (BR)
  }; // Array of struct motor

int LRval; // Acutal value of left-right
int FBval; // Actual value of forward-backward
int turnVal; // Actual value of how much to turn
int pistonVal; // Ignore!!!!
String MapCoord;
PolarCoord polar;

void setup() {
  for (int i = 0; i < motorArrCount; i++) {
    pinMode( motorArr[i].a, OUTPUT );
    pinMode( motorArr[i].b, OUTPUT );
    digitalWrite( motorArr[i].a, LOW );
    digitalWrite( motorArr[i].b, LOW );
    pinMode( motorArr[i].speedControl, OUTPUT );
    digitalWrite( motorArr[i].speedControl, LOW );
  } // setup input from controller
  
  pinMode(LRpin, INPUT);
  pinMode(FBpin, INPUT);
  pinMode(turnPin, INPUT);
  pinMode(pistonInput, INPUT);
  pinMode(pistonPin, OUTPUT);
  digitalWrite(pistonPin, LOW);
  Serial.begin(9600);
  Serial.println("Serial On");
} // more setups
 
void loop() {
  //pistonVal = pulseIn(pistonInput, HIGH); uncomment if add piston
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
} // looping input from controller, constantly recieving input

 // Converts the coordinates recieved from controller to polar coordinates.
 // The way you find a point in Polar coordinates is with a vector, which is direction and magnitude.
 // This is exactly what we want, as we want to know which direction the controller wants the robot to go, and how fast.
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

// Plots the direction onto a sinewave, showing what the output of each motor should be based on direction.
// Not relevant, but the sine function is a never ending wave, and this is what leads to triangle ambiguity when using law of sines.
void calcSine() {
  for (int i = 0; i < motorArrCount; i++) {
    motorArr[i].force = polar.r * sin((polar.theta + motorArr[i].thetaShift)*PI/180);
  }
}

// Scale the force based on the magnitude of the polar coordinate and scale it so that the maximum power is 1 or lower, depending on how big the magnitude is.
void scale() {
  float maxForce = 0;
  float maxTurnForce = 0;
  for (int i = 0; i < motorArrCount; i++) {
    if (maxForce < abs(motorArr[i].force)) {
      maxForce = abs(motorArr[i].force);
    }
  }
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

// Directly controlling the H-Bridge. This is where the motors are moved.
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
}

// Ignore, this is for the piston we never added.
void pistonDrive(int controller) {
  if (controller > 1100) {
    digitalWrite(pistonPin, HIGH);
  } else {
    digitalWrite(pistonPin, LOW);
  }
}
