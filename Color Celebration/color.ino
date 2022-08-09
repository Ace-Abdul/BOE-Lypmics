#include <Servo.h>
/* Here initialize the servomotors, define three pins that will be used by our QTI sensor, and define a boolen variable paused
to indicate if the bot has been paused */
Servo servoLeft;
Servo servoRight;
#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Creating instance of the RGB sensor class*/
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
 

const int rightSensor = 52;
const int middleSensor = 51;
const int leftSensor = 47;
bool detect = false;

void setup() {
  /* Here set the QTI pins to output and attach the servomotors*/
  Serial.begin(9600);                 
  pinMode(rightSensor, OUTPUT);
  pinMode(middleSensor, OUTPUT);
  pinMode(leftSensor, OUTPUT);
  Serial.println("Color View Test!");
  servoLeft.attach(11);
  servoRight.attach(12);
  if (tcs.begin()) {                                              // Printing "Found Sensor" if the sensor detects and object
    Serial.println("Found sensor");

    servoLeft.attach(11);
    servoRight.attach(12);
    pinMode(45, OUTPUT);                                           
    pinMode(46, OUTPUT);
    pinMode(44, OUTPUT);
    analogWrite(44, 255);
    analogWrite(45, 255);
    analogWrite(46, 255);}
}

void loop() {
/* First we identify if the qti sensors are facing a black or white ground using the RCtime function which handles the capacitator aspect of the
QTI sensors. If the threshold is lower than a threshold value corresponding to a white/bright surface the function returns false; if the value
we read exceeds that threshold we return true for a black/dark surface. We then print the values read by each sensor.*/
    bool colorRight = RCtime(rightSensor);            
    bool colorMid = RCtime(middleSensor);
    bool colorLeft = RCtime(leftSensor);
    Serial.print(colorLeft);
    Serial.print(colorMid);
    Serial.print(colorRight);
    Serial.println();
/* If all three sensors are reading black, both servomotors stop rotating -- done in pause function*/
    if(colorMid && colorRight && colorLeft){
      if (detect){ 
        detection();
        uint16_t clear, red, green, blue;

        delay(60);  // takes 60ms to read 
        /* Storing the RGB values read by the sensor into the corresponding variables*/
        tcs.getRawData(&red, &green, &blue, &clear);
        /* Printing the RGB values*/
        Serial.print("C:\t"); Serial.print(clear);
        Serial.print("\tR:\t"); Serial.print(red);
        Serial.print("\tG:\t"); Serial.print(green);
        Serial.print("\tB:\t"); Serial.println(blue);
        /* The code block below controls lighting up built in LED depending on the RGB values read
         8-ball: Flashes red
         Rubber Ducky: Flashes green
         Pencil: Flashes blue*/
        if(red < 600 && blue < 600 && green < 600){            //flash red if 8-ball
          analogWrite(44, 255);
          analogWrite(45, 0);
          analogWrite(46, 255);
          delay(100);
          analogWrite(44, 255);
          analogWrite(45, 255);
          analogWrite(46, 255);
        }
        if(red > 1500){                                         //flash green if duck
          analogWrite(44, 255);
          analogWrite(45, 255);
          analogWrite(46, 0);
          delay(100);
          analogWrite(44, 255);
          analogWrite(45, 255);
          analogWrite(46, 255);
        }
        if(blue > 1500){                                        //flash blue if pencil
          analogWrite(44, 0);
          analogWrite(45, 255);
          analogWrite(46, 255);
          delay(100);
          analogWrite(44, 255);
          analogWrite(45, 255);
          analogWrite(46, 255);
        }
    }
          pause();
          detect = true;
      }
/* Othere wise if the rightmost qti sensor is over a black surface and the leftmost is over a white surface, the bot moves in the right direction*/
    else if(colorRight && !colorLeft){right();}
/* Othere wise if the rightmost qti sensor is over a white surface and the leftmost is over a black surface, the bot moves in the left direction*/
    else if(!colorRight && colorLeft){left();}
/* Othere wise if all sensors are over a white surface the bot moves back for 20 milliseconds then stops */
    else if (colorMid==0 && colorRight==0 && colorLeft==0){
          servoLeft.writeMicroseconds(1700);
          servoRight.writeMicroseconds(1300);
          delay(20);
          servoLeft.writeMicroseconds(1500);
          servoRight.writeMicroseconds(1500);
    }
 /* Othere wise if this line iterates then  all sensors are over a dark surface in which case the bot just moves forward*/
    else{
      forwardMovement();
    }
}

/* We found this RCtime function through a bit of research. It's need to handle the decaying read values from the qti sensor due to the capacitator it uses.*/

bool RCtime(int sensPin){
   bool ans = false;
   long result = 0;
   pinMode(sensPin, OUTPUT);       // make pin OUTPUT
   digitalWrite(sensPin, HIGH);    // make pin HIGH to discharge capacitor - study the schematic
   delay(1);                       // wait a  ms to make sure cap is discharged

   pinMode(sensPin, INPUT);        // turn pin into an input and time till pin goes low
   digitalWrite(sensPin, LOW);     // turn pullups off - or it won't work
   while(digitalRead(sensPin)){    // wait for pin to go low
      result++;
   }

  if(result >= 60){
      ans = true;
  }
  else{
      ans = false;
  }
  return ans;
} 

/* Functoion for rotating servormoters such that the bot moves forward*/
void forwardMovement(){
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1700);
}

/* Functoion for rotating servormoters such that the bot moves forward angled left*/
void left(){
  servoLeft.writeMicroseconds(1400);
  servoRight.writeMicroseconds(1300);
}


/* Functoion for rotating servormoters such that the bot moves forward angled to the right*/
void right(){
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1600);
}


/* Functoion for stopping both servomotors from rotating to stop the bot for half a second then moving the bot forward*/
void pause(){
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  delay(500);
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1700);
  delay(80);
}

void detection(){
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(2000);
  delay(1450);
  servoLeft.writeMicroseconds(1000);
  servoRight.writeMicroseconds(2000);
  delay(970);
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  delay(5000);
}
