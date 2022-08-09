#include <Servo.h>

Servo servoLeft;
Servo servoRight; 

const int rightSensor = 47;
const int middleSensor = 51;
const int leftSensor = 52;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(rightSensor, OUTPUT);
  pinMode(middleSensor, OUTPUT);
  pinMode(leftSensor, OUTPUT);

  servoLeft.attach(11);
  servoRight.attach(12);

  //servoLeft.writeMicroseconds(1300);
  //servoRight.writeMicroseconds(1700);
  //delay(100);
  //pinMode(2, OUTPUT);
  //digitalWrite(2, LOW);
}

void loop() {
    bool colorRight = RCtime(rightSensor);
    bool colorMid = RCtime(middleSensor);
    bool colorLeft = RCtime(leftSensor);

    while (colorMid==1 && colorRight==0 && colorLeft==0){
        forward();
        colorRight = RCtime(rightSensor);
        colorMid = RCtime(middleSensor);
        colorLeft = RCtime(leftSensor);
    }
    if(colorMid==1 && colorRight==1 && colorLeft==1){
      servoLeft.writeMicroseconds(1500);
      servoRight.writeMicroseconds(1500);
      delay(1000);
      forward(); 
    }
    if (!colorMid && !colorLeft && !colorRight) {
      servoLeft.writeMicroseconds(1500);
      servoRight.writeMicroseconds(1500);
    }
    
    while(colorRight==0 && colorMid==0 && colorLeft==1){
//      backLeft();
//      left();
//      forward();
      circleLeft();
      colorRight = RCtime(rightSensor);
      colorMid = RCtime(middleSensor);
      colorLeft = RCtime(leftSensor);
    }
    
    while(colorRight==1 && colorLeft==0 && colorMid==0){
//      backRight();
//      right();
//      forward();
      circleRight();
      colorRight = RCtime(rightSensor);
      colorMid = RCtime(middleSensor);
      colorLeft = RCtime(leftSensor);
    }
    delay(200);
}

//White <100; Black >= 100
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

   Serial.print(result);
   Serial.print("       ");
   Serial.print(sensPin);
   Serial.println();

  if(result >= 120){
      return true;
  }
  return false;
}  

void forward(){
  servoLeft.writeMicroseconds(1000);
  servoRight.writeMicroseconds(2000);
  delay(10);
}

void forwardLeft(){
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(2000);
  delay(10);
}
void forwardRight(){
  servoLeft.writeMicroseconds(1000);
  servoRight.writeMicroseconds(1700);
  delay(10);
}

void backLeft(){
  servoLeft.writeMicroseconds(1800);
  servoRight.writeMicroseconds(1400);
  delay(50);
  
}

void backRight(){
  servoLeft.writeMicroseconds(1600);
  servoRight.writeMicroseconds(1200);
  delay(50);
  
}

void left(){
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1700);
  delay(1000);
}

void right(){
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1500);
  delay(1000);
}

void circleLeft(){
  servoLeft.writeMicroseconds(1400);
  servoRight.writeMicroseconds(2000);
  delay(1000);
}
void circleRight(){
  servoLeft.writeMicroseconds(1000);
  servoRight.writeMicroseconds(1600);
  delay(1000);
}
