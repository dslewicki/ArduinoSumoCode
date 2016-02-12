/*
 For use with TReX Jr based Sumo Robots

 This sketch is for our sample sumo robots in the PSU robotics club with
 a dual front ultrasonic sensor and the Pololu TReX Jr. motor controller.
 
 The hex code 7F is 127 or the max motor speed. 
 The rest of the hex code meanings can be fond at this link:
 https://www.pololu.com/file/0J12/TReXJr_Commands_v1.2.pdf
 
 created 11 Feb 2016
 by Geoffrey Billy

 https://github.com/pennstateroboticsclub/arduinosumo/tree/master/sumotrexjr
 */

#include <NewPing.h>
#include <SoftwareSerial.h>

//pins for ping sensor 1
#define TRIG_1 2
#define ECHO_1 3

//pins for ping sensor 2
#define TRIG_2 4
#define ECHO_2 5

//same for both ping sensors
#define MAX_DISTANCE 200

//serial pins for TReX Jr control
#define RX 7
#define TX 8

//make sensor objects to find their distances later 
NewPing sonarleft(TRIG_1, ECHO_1, MAX_DISTANCE);
NewPing sonarright(TRIG_2, ECHO_2, MAX_DISTANCE);

//make serial object to communicate with the TReX Jr
SoftwareSerial trexSerial(RX, TX);

//timing pause lengths for a smoother and more accurate movement
const int threshold = 150;
const int abouteven = 2;
const int drivedelay = 100;
const int turndelay = 50;
const int writedelay = 10;

int distanceleft = 0;
int distanceright = 0;
int difference = 0;
bool stateleft = false;
unsigned long t = 0;

void setup() {
  
  //communication rate for serial monitor
  Serial.begin(115200);
  
  delay(1000);
  
  //default communication rate for the TReX Jr
  trexSerial.begin(19200);
  
  //5 second sumo time start delay to allow for humans to back away
  delay(3950);
  
  //get held timing when the program starts
  t = millis();
}


void loop() {
    
  //read ultrasonic sensor one at a time to update the distances
  distanceleft = sonarleft.ping_cm();
  
  //pause to prevent interference
  delay(writedelay);
  
  distanceright = sonarright.ping_cm();

  difference = distanceleft - distanceright;
    
  if (distanceright != 0 && distanceleft != 0 && distanceright < threshold && distanceleft < threshold && abs(difference) < abouteven) {
  //sees something in front of both sensors evenly

    //moves forward for forward the delay duration by writing max 127 acceleration value to both motors
     Serial.println("FORWARD");
    
    //write motor 1 forward
    trexSerial.write(0xC6);
    trexSerial.write(0x7F);
    
    //delay between commands sent to the TReX Jr
    delay(writedelay);
    
    //write motor 2 forward
    trexSerial.write(0xCE);
    trexSerial.write(0x7F);
    
    //pause to allow for the movement to take place
    delay(drivedelay);
  }
    
  if (((distanceright != 0 && distanceright < threshold) && (distanceleft > threshold || distanceleft == 0)) ||
      (distanceright != 0 && distanceleft != 0 && distanceright < threshold && distanceleft < threshold && difference < 0) ||
      (!stateleft)) {
  //sees something closer on the right or just on the right or it just passed from left ot right
     
    //spin right to even itself out with its opponent  
    Serial.println("RIGHT");
    
    stateleft = false;
  
    //write motor 1 forward
    trexSerial.write(0xC6);
    trexSerial.write(0x7F);
    
    delay(writedelay);
    
    //write motor 2 backward
    trexSerial.write(0xCD);
    trexSerial.write(0x7F);
    
    delay(turndelay);
  }
    
  if (((distanceright == 0 || distanceright > threshold) && (distanceleft < threshold && distanceleft != 0)) ||
      (distanceright != 0 && distanceleft != 0 && distanceright < threshold && distanceleft < threshold && difference < 0) ||
      (stateleft)) {
  //sees something closer on the left or just on the left or it just passed from right to left
      
    Serial.println("LEFT");
    
    stateleft = true;
  
    //write motor 1 backward
    trexSerial.write(0xC5);
    trexSerial.write(0x7F);
    
    delay(writedelay);
    
    //write motor 2 forward
    trexSerial.write(0xCE);
    trexSerial.write(0x7F);
    
    delay(turndelay);
  } 
  
}
