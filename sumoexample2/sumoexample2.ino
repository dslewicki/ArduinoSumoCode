/*
 Sumo Robot

 This sketch represents a hypothetical program for a sumo robot that 
 has two bidirectional motors, and two floor mounted limit switches.
 
 *Replace the value -3 with the correct parameter for your robot.*

 created 31 Jan 2016
 by Geoffrey Billy

https://github.com/pennstateroboticsclub/arduinosumo/tree/master/sumoexample2
 */

//floor limit switches by their connected digital pin number
const int leftswitch = -3;     
const int rightswitch = -3;

//define motor control by their connected digital pin number
const int MOTOR_A1 = -3;
const int MOTOR_A2 = -3;
const int MOTOR_B1 = -3;
const int MOTOR_B2 = -3;

//variables to store opponent position information
const int backdelay = -3;
bool leftedge = false;
bool rightedge = false;
                          
void setup() {
  //set serial communication rate
  Serial.begin(115200);

  //define switches as inputs
  pinMode(leftswitch, INPUT);
  pinMode(rightswitch, INPUT);

  //define motor control as an output
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);
  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
}

//function to go forward by writing the digital motor pins high and low
void forward() {
    digitalWrite(MOTOR_A1, HIGH);
    digitalWrite(MOTOR_A2, LOW);
    digitalWrite(MOTOR_B1, HIGH);
    digitalWrite(MOTOR_B2, LOW);
}

//function to go in reverse
void backward() {
    digitalWrite(MOTOR_A1, LOW);
    digitalWrite(MOTOR_A2, HIGH);
    digitalWrite(MOTOR_B1, LOW);
    digitalWrite(MOTOR_B2, HIGH);
}

//function to go left
void left() {
    digitalWrite(MOTOR_A1, HIGH);
    digitalWrite(MOTOR_A2, LOW);
    digitalWrite(MOTOR_B1, LOW);
    digitalWrite(MOTOR_B2, HIGH);
}

//function to go right
void right() {
    digitalWrite(MOTOR_A1, LOW);
    digitalWrite(MOTOR_A2, HIGH);
    digitalWrite(MOTOR_B1, HIGH);
    digitalWrite(MOTOR_B2, LOW);
}

//time start delay to allow for humans to back away
delay(5000);

void loop() {
  
  //start by going forward blindly
  forward();
  
  //read the switches
  rightedge = !(digitalRead(rightswitch));
  leftedge = !(digitalRead(leftswitch));
  
  if (leftedge) {
  //if over the left edge
    //move backward to get some distance from the edge
    backward();
    delay(backdelay);
    //turn right to face the circle again
    right();
    delay(turndelay);
  }

  else if (rightedge) {
  //if over the right edge
    //move backward to get some distance from the edge
    backward();
    delay(backdelay);
    //turn right to face the circle again
    left();
    delay(turndelay);
  }
}
