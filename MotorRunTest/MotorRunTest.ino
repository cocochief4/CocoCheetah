/*
  HG7881_Motor_Driver_Example - Arduino sketch
   
  This example shows how to drive a motor with using HG7881 (L9110) Dual
  Channel Motor Driver Module.  For simplicity, this example shows how to
  drive a single motor.  Both channels work the same way.
   
  This example is meant to illustrate how to operate the motor driver
  and is not intended to be elegant, efficient or useful.
   
  Connections:
   
    Arduino digital output D10 to motor driver input B-IA.
    Arduino digital output D11 to motor driver input B-IB.
    Motor driver VCC to operating voltage 5V.
    Motor driver GND to common ground.
    Motor driver MOTOR B screw terminals to a small motor.
     
  Related Banana Robotics items:
   
    BR010038 HG7881 (L9110) Dual Channel Motor Driver Module
    https://www.BananaRobotics.com/shop/HG7881-(L9110)-Dual-Channel-Motor-Driver-Module
 
  https://www.BananaRobotics.com
*/
 
// wired connections
#define HG7881_B_IA 10 // D10 --> Motor B Input A --> MOTOR B +
#define HG7881_B_IB 11 // D11 --> Motor B Input B --> MOTOR B -
#define HG7881_A_IA 8 // D10 --> Motor A Input A --> MOTOR A +
#define HG7881_A_IB 9 // D11 --> Motor A Input B --> MOTOR A -
#define HG7881_C_IA 3 // D10 --> Motor C Input A --> MOTOR B +
#define HG7881_C_IB 4 // D11 --> Motor C Input B --> MOTOR B -
#define HG7881_D_IA 5 // D10 --> Motor D Input A --> MOTOR A +
#define HG7881_D_IB 6 // D11 --> Motor D Input B --> MOTOR A -

#define testRecieveA 12
#define testRecieveB 13
 
// functional connections
#define MOTOR_B_PWM HG7881_B_IA // Motor B PWM Speed
#define MOTOR_B_DIR HG7881_B_IB // Motor B Direction
#define MOTOR_A_PWM HG7881_A_IA // Motor A PWM Speed
#define MOTOR_A_DIR HG7881_A_IB // Motor A Direction
#define MOTOR_C_PWM HG7881_B_IA // Motor C PWM Speed
#define MOTOR_C_DIR HG7881_B_IB // Motor C Direction
#define MOTOR_D_PWM HG7881_A_IA // Motor D PWM Speed
#define MOTOR_D_DIR HG7881_A_IB // Motor D Direction
 
// the actual values for "fast" and "slow" depend on the motor
#define PWM_SLOW 100  // arbitrary slow speed PWM duty cycle
#define PWM_FAST 200 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000 // brief delay for abrupt motor changes
 
void setup()
{
  Serial.begin( 9600 );
  pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );
  pinMode( MOTOR_A_DIR, OUTPUT );
  pinMode( MOTOR_A_PWM, OUTPUT);
  digitalWrite( MOTOR_A_DIR, LOW );
  digitalWrite( MOTOR_A_PWM, LOW );
  pinMode( MOTOR_C_DIR, OUTPUT );
  pinMode( MOTOR_C_PWM, OUTPUT );
  digitalWrite( MOTOR_C_DIR, LOW );
  digitalWrite( MOTOR_C_PWM, LOW );
  pinMode( MOTOR_D_DIR, OUTPUT );
  pinMode( MOTOR_D_PWM, OUTPUT);
  digitalWrite( MOTOR_D_DIR, LOW );
  digitalWrite( MOTOR_D_PWM, LOW );
  
  pinMode( testRecieveA, INPUT );
  pinMode( testRecieveB, INPUT );
}
 
void loop()
{
  boolean isValidInput;
  // draw a menu on the serial port
  Serial.println( "-----------------------------" );
  Serial.println( "MENU:" );
  Serial.println( "1) Fast forward" );
  Serial.println( "2) Forward" );
  Serial.println( "3) Soft stop (coast)" );
  Serial.println( "4) Reverse" );
  Serial.println( "5) Fast reverse" );
  Serial.println( "6) Hard stop (brake)" );
  Serial.println( "7) Left forward diagonal ");
  Serial.println( "8) Right forward diagonal ");
  Serial.println( "-----------------------------" );
  do
  {
    byte c;
    // get the next character from the serial port
    Serial.print( "?" );
    while( !Serial.available() )
      ; // LOOP...
    c = Serial.read();
    // execute the menu option based on the character recieved
    switch( c )
    {
      case '1': // 1) Fast forward
        Serial.println( "Fast forward..." );
        // always stop motors briefly before abrupt changes
        digitalWrite( MOTOR_B_DIR, LOW );
        digitalWrite( MOTOR_B_PWM, LOW );
        digitalWrite( MOTOR_A_DIR, LOW );
        digitalWrite( MOTOR_A_PWM, LOW );
        digitalWrite( MOTOR_C_DIR, LOW );
        digitalWrite( MOTOR_C_PWM, LOW );
        digitalWrite( MOTOR_D_DIR, LOW );
        digitalWrite( MOTOR_D_PWM, LOW );
        delay( DIR_DELAY );
        // set the motor speed and direction
        digitalWrite( MOTOR_B_DIR, HIGH ); // direction = forward
        analogWrite( MOTOR_B_PWM, 255-PWM_FAST ); // PWM speed = fast
        digitalWrite( MOTOR_A_DIR, HIGH ); // direction = forward
        analogWrite( MOTOR_A_PWM, 255-PWM_FAST ); // PWM speed = fast
        digitalWrite( MOTOR_C_DIR, HIGH ); // direction = forward
        analogWrite( MOTOR_C_PWM, 255-PWM_FAST ); // PWM speed = fast
        digitalWrite( MOTOR_D_DIR, HIGH ); // direction = forward
        analogWrite( MOTOR_D_PWM, 255-PWM_FAST ); // PWM speed = fast
        isValidInput = true;
        break;      
       
      case '2': // 2) Forward      
        Serial.println( "Forward..." );
        // always stop motors briefly before abrupt changes
        digitalWrite( MOTOR_B_DIR, LOW );
        digitalWrite( MOTOR_B_PWM, LOW );
        digitalWrite( MOTOR_A_DIR, LOW );
        digitalWrite( MOTOR_A_PWM, LOW );
        digitalWrite( MOTOR_C_DIR, LOW );
        digitalWrite( MOTOR_C_PWM, LOW );
        digitalWrite( MOTOR_D_DIR, LOW );
        digitalWrite( MOTOR_D_PWM, LOW );
        delay( DIR_DELAY );
        // set the motor speed and direction
        digitalWrite( MOTOR_B_DIR, HIGH ); // direction = forward
        analogWrite( MOTOR_B_PWM, 255-PWM_SLOW ); // PWM speed = slow
        digitalWrite( MOTOR_A_DIR, HIGH ); // direction = forward
        analogWrite( MOTOR_A_PWM, 255-PWM_SLOW ); // PWM speed = slow
        digitalWrite( MOTOR_C_DIR, HIGH ); // direction = forward
        analogWrite( MOTOR_C_PWM, 255-PWM_SLOW ); // PWM speed = slow
        digitalWrite( MOTOR_D_DIR, HIGH ); // direction = forward
        analogWrite( MOTOR_D_PWM, 255-PWM_SLOW ); // PWM speed = slow
        isValidInput = true;
        break;      
         
      case '3': // 3) Soft stop (preferred)
        Serial.println( "Soft stop (coast)..." );
        digitalWrite( MOTOR_B_DIR, LOW );
        digitalWrite( MOTOR_B_PWM, LOW );
        digitalWrite( MOTOR_A_DIR, LOW );
        digitalWrite( MOTOR_A_PWM, LOW );
        digitalWrite( MOTOR_C_DIR, LOW );
        digitalWrite( MOTOR_C_PWM, LOW );
        digitalWrite( MOTOR_D_DIR, LOW );
        digitalWrite( MOTOR_D_PWM, LOW );
        isValidInput = true;
        break;      
 
      case '4': // 4) Reverse
        Serial.println( "Reverse" );
        // always stop motors briefly before abrupt changes
        digitalWrite( MOTOR_B_DIR, LOW );
        digitalWrite( MOTOR_B_PWM, LOW );
        digitalWrite( MOTOR_A_DIR, LOW );
        digitalWrite( MOTOR_A_PWM, LOW );
        digitalWrite( MOTOR_C_DIR, LOW );
        digitalWrite( MOTOR_C_PWM, LOW );
        digitalWrite( MOTOR_D_DIR, LOW );
        digitalWrite( MOTOR_D_PWM, LOW );
        delay( DIR_DELAY );
        // set the motor speed and direction
        digitalWrite( MOTOR_B_DIR, LOW ); // direction = reverse
        analogWrite( MOTOR_B_PWM, PWM_SLOW ); // PWM speed = slow
        digitalWrite( MOTOR_A_DIR, LOW ); // direction = reverse
        analogWrite( MOTOR_A_PWM, PWM_SLOW ); // PWM speed = slow
        digitalWrite( MOTOR_C_DIR, LOW ); // direction = reverse
        analogWrite( MOTOR_C_PWM, PWM_SLOW ); // PWM speed = slow
        digitalWrite( MOTOR_D_DIR, LOW ); // direction = reverse
        analogWrite( MOTOR_D_PWM, PWM_SLOW ); // PWM speed = slow
        isValidInput = true;
        break;      
         
      case '5': // 5) Fast reverse
        Serial.println( "Fast reverse..." );
        // always stop motors briefly before abrupt changes
        digitalWrite( MOTOR_B_DIR, LOW );
        digitalWrite( MOTOR_B_PWM, LOW );
        digitalWrite( MOTOR_A_DIR, LOW );
        digitalWrite( MOTOR_A_PWM, LOW );
        digitalWrite( MOTOR_C_DIR, LOW );
        digitalWrite( MOTOR_C_PWM, LOW );
        digitalWrite( MOTOR_D_DIR, LOW );
        digitalWrite( MOTOR_D_PWM, LOW );
        delay( DIR_DELAY );
        // set the motor speed and direction
        digitalWrite( MOTOR_B_DIR, LOW ); // direction = reverse      
        analogWrite( MOTOR_B_PWM, PWM_FAST ); // PWM speed = fast
        digitalWrite( MOTOR_A_DIR, LOW ); // direction = reverse      
        analogWrite( MOTOR_A_PWM, PWM_FAST ); // PWM speed = fast
        digitalWrite( MOTOR_C_DIR, LOW ); // direction = reverse      
        analogWrite( MOTOR_C_PWM, PWM_FAST ); // PWM speed = fast
        digitalWrite( MOTOR_D_DIR, LOW ); // direction = reverse      
        analogWrite( MOTOR_D_PWM, PWM_FAST ); // PWM speed = fast
        isValidInput = true;
        break;
         
      case '6': // 6) Hard stop (use with caution)
        Serial.println( "Hard stop (brake)..." );
        digitalWrite( MOTOR_B_DIR, HIGH );
        digitalWrite( MOTOR_B_PWM, HIGH );
        digitalWrite( MOTOR_A_DIR, HIGH );
        digitalWrite( MOTOR_A_PWM, HIGH );
        digitalWrite( MOTOR_C_DIR, HIGH );
        digitalWrite( MOTOR_C_PWM, HIGH );
        digitalWrite( MOTOR_D_DIR, HIGH );
        digitalWrite( MOTOR_D_PWM, HIGH );
        isValidInput = true;
        break;
 
      default:
        // wrong character! display the menu again!
        isValidInput = false;
        break;
    }
  } while( isValidInput == true );
 
  // repeat the main loop and redraw the menu...
}
/*EOF*/
