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

#include <NewPing.h>
#include <SoftwareSerial.h>

//pins for the ultrasonic ping sensor
#define TRIG 2
#define ECHO 3
#define MAX_DISTANCE 200

//serial pins for TReX Jr control
#define RX 7
#define TX 8

//make ultrasonic sensor object to find its distance later 
NewPing sonar(TRIG, ECHO, MAX_DISTANCE);

//make serial object to communicate with the TReX Jr
SoftwareSerial trexSerial(RX, TX);

//timing pause lengths for a smoother and more accurate movement
const int threshold = 50;
const int forwarddelay = 1000;
const int turndelay = 300;
const int writeddelay = 10;

int distance = 0;
bool justsaw = false;
unsigned long t = 0;

void setup() {
  
  //communication rate for serial monitor
  Serial.begin(115200);
  
  delay(1000);
  
  //default ommunication rate for the TReX Jr
  trexSerial.begin(19200);
  
  //5 second sumo time start delay to allow for humans to back away
  delay(3950);
  
  //get held timing when the program starts
  t = millis();
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
    
    delay(writeddelay);
    
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
    
    delay(writeddelay);
    
    //write motor 2 forward to TReX Jr
    trexSerial.write(0xCE);
    trexSerial.write(0x7F);
    
    justsaw = false;
    
    //updates held timing to current timing
    t = millis();
    
    //uses millis to prevent the complete pause by delay
    while(millis() - t <= turndelay) {
      
      if (distance <= threshold && distance != 0) {
        //breaks the turn early if sees the opponent again
        break;
      }
    }
  }
  
  else {
  //assume the opponent when right so spin right until the opponent is found
  
    //write motor 1 forward to TReX Jr
    trexSerial.write(0xC6);
    trexSerial.write(0x7F);
    
    delay(writeddelay);
    
    //write motor 2 backward to TReX Jr
    trexSerial.write(0xCD);
    trexSerial.write(0x7F);
    
    delay(writeddelay);
  }
}
