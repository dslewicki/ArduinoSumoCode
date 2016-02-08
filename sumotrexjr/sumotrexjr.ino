/*
 For use with TReX Jr based Sumo Robots

 This sketch is for our sample sumo robots in the PSU robotics club with
 a single ultrasonic sensor and the Pololu TReX Jr. motor controller.
 
 created 7 Feb 2016
 by Geoffrey Billy

https://github.com/pennstateroboticsclub/arduinosumo/tree/master/sumotrexjr
 */

//import the NewPing library by Tim Eckel for use with the ultrasonic sensor
#include <NewPing.h>

//import the serial library for use with the serial controlled Pololu motor controller
#include <SoftwareSerial.h>

//define ultrasonic sensor by their connected digital pin number
const int TRIG = 2;     
const int ECHO = 3;
const int MAX_DISTANCE = 200;

//timing pause lengths for a smoother and more accurate movement
const int threshold = 180;
const int forwarddelay = 100;
const int turndelay = 50;

//variables to store opponent position information
int distance = 0;
bool justsaw = false;

//define serial pins for TReX Jr control
const int RX = 7;
const int TX = 8;

//make ultrasonic sensor object to find its distance later 
NewPing sonar(TRIG, ECHO, MAX_DISTANCE); 

//make serial object to communicate with the TReX Jr
SoftwareSerial trex(RX, TX);

void setup() {
  //set serial communication rate for serial monitor
  Serial.begin(115200);
  
  //set serial communication rate for the TReX Jr
  trex.begin(19200);
  
  //standard sumo time start delay to allow for humans to back away
  delay(5000);
}


void loop() {
    
  //read ultrasonic sensor to update distance
  distance = sonar.ping_cm();

  if (distance <= threshold && distance != 0) {
  //sees something in front of it
    //moves forward for forward the delay duration
    trexSerial.write(0xDA);  //alternate is 0xDF
    trexSerial.write(0x7F);
    delay(forwarddelay);
    //changes the state of whether or not the robot just saw its opponent to yes
    justsaw = true;
  }
  else if (justsaw) {
  //just lost sight of the opponent
    //turns left for a short duration to estimate if the opponent went left
    trexSerial.write(0xDB);  //alternate is 0xDE
    trexSerial.write(0x7F);
    delay(turndelay);
  }
  else {
  //assume the opponent when right so spin right until the opponent is found
  trexSerial.write(0xDE);    //alternate is 0xDB
  trexSerial.write(0x7F);
  }
}
