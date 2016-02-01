/*
 Sumo Robot

 This sketch represents a hypothetical program for a sumo robot
 that has two bidirectional motors, and one ultrasonic sensor.
 
 *Replace the value -3 with the correct parameter for your robot.*

 created 31 Jan 2016
 by Geoffrey Billy

https://github.com/pennstateroboticsclub/arduinosumo/tree/master/sumoexample1
 */

//import the NewPing library by Tim Eckel for use with the ultrasonic sensor
#include <NewPing.h>  

//define ultrasonic sensor by their connected digital pin number
const int TRIG = -3;     
const int ECHO = -3;
const int MAX_DISTANCE = -3;

//define motor control by their connected digital pin number
const int MOTOR_A1 = -3;
const int MOTOR_A2 = -3;
const int MOTOR_B1 = -3;
const int MOTOR_B2 = -3;

//variables to store opponent position information
const int threshold = -3;
const int forwarddelay = -3;
const int turndelay = -3;
int distance = 0;
bool justsaw = false;

//make ultrasonic sensor object to find its distance later 
NewPing sonar(TRIG, ECHO, MAX_DISTANCE);   

void setup() {
  //set serial communication rate
  Serial.begin(115200);

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
  
  //read ultrasonic sensor to update distance
  distance = sonar.ping();

  if (distance <= threshold) {
  //sees something in front of it
    //moves forward for forward the delay duration
    forward();
    delay(forwarddelay);
    //changes the state of whether or not the robot just saw its opponent to yes
    justsaw = true;
  }

  else if (lastdistance) {
  //just lost sight of the opponent
    //turns left for a short duration to estimate if the opponent went left
    left();
    delay(turndelay);
  }

  else {
  //can assume the opponent went right or a condition for when the match starts
    //assume the opponent when right so spin right until the opponent is found
    right();  
  }
}
