/*
 For use with TReX Jr based Sumo Robots

 This sketch is for our sample sumo robots in the PSU robotics club with
 a single ultrasonic sensor and the Pololu TReX Jr. motor controller.
 
 The hex code 7F is 127 or the max motor speed. 
 The rest of the hex code meanings can be fond at this link:
 https://www.pololu.com/file/0J12/TReXJr_Commands_v1.2.pdf
 
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

//make ultrasonic sensor object to find its distance later 
NewPing sonar(TRIG, ECHO, MAX_DISTANCE);

//timing pause lengths for a smoother and more accurate movement
const int threshold = 50;
const int forwarddelay = 1000;
const int turndelay = 300;
const int spindelay = 100;

//variables to store opponent position information
int distance = 0;
bool justsaw = false;

//define serial pins for TReX Jr control
const int RX = 7;
const int TX = 8;

//make serial object to communicate with the TReX Jr
SoftwareSerial trexSerial(RX, TX);

void setup() {
  //set serial communication rate for serial monitor
  Serial.begin(115200);
  
  delay(1000);
  
  //set serial communication rate for the TReX Jr
  trexSerial.begin(19200);
  
  //standard 5 second sumo time start delay to allow for humans to back away
  delay(3950);

}


void loop() {
    
  //read ultrasonic sensor to update distance
  distance = sonar.ping_cm();

  if (distance <= threshold && distance != 0) {
  //sees something in front of it
    //moves forward for forward the delay duration by writing max 127 acceleration value to both motors
    
    trexSerial.write(0xC6);
    trexSerial.write(0x7F);
    //write motor 1 forward to TReX Jr
    
    delay(10);
    
    trexSerial.write(0xCE);
    trexSerial.write(0x7F);
    //write motor 2 forward to TReX Jr
    
    //changes the state of whether or not the robot just saw its opponent to yes
    justsaw = true;
    
    delay(forwarddelay);
  }
  
  else if (justsaw) {
  //just lost sight of the opponent
    //turns left for a short duration to estimate if the opponent went left
    
    //write motor 1 backward to TReX Jr
    trexSerial.write(0xC5);
    trexSerial.write(0x7F);
    
    delay(10);
    
    //write motor 2 forward to TReX Jr
    trexSerial.write(0xCE);
    trexSerial.write(0x7F);
    
    justsaw = false;
    
    delay(turndelay);
  }
  
  else {
  //assume the opponent when right so spin right until the opponent is found
  
    //write motor 1 forward to TReX Jr
    trexSerial.write(0xC6);
    trexSerial.write(0x7F);
    
    delay(10);
    
    //write motor 2 backward to TReX Jr
    trexSerial.write(0xCD);
    trexSerial.write(0x7F);
    
    delay(spindelay);
    
  }
}
