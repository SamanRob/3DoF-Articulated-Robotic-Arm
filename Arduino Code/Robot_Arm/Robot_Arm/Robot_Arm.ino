#include <AccelStepper.h>
#include <Stepper.h>
#include <Servo.h>
#include <Arduino.h>
#include <math.h>
#include <MultiStepper.h>

#define limitSwitch1 3
#define limitSwitch2 2
#define limitSwitch3 14

#define X_ENABLE_PIN 38
#define Y_ENABLE_PIN 56
#define Z_ENABLE_PIN 62

int currentValue1;
int currentValue2;
int previousValue1;
int previousValue2;
int pos;

String content = "";
float data[100];

// Inverse Kinematic variables (It is Important that the endeffector doest touch the ground so the base must somehow be adjusted to it, otherwise the computed numbers will have error)
float l_1=286; // 20.5 mm must be added to the base
float l_2=232.5;
float l_3=220;

float theta_1, theta_2, theta_3;
float th1, th2, th3;

float d, D, beta, alpha;

// we need to calculate the gear ratio and steps of each stepper to have accurate movement
// for each motore to increase the resolution 3 pins for microstepping were added it means 3200 steps for whole revolution is needed
// Base has a ratio of 9.25:1>> 9.25x3200=29600 steps for 360 degree rotation >> 82.22 steps for one degree
// Middle axis, Stepper motor has gear ratio of 13.73:1 and a pulley belt transmission of 9 which comes to 123.57>>>> 123.57x3200=395424 steps for a 360 degree rotation >> 1098.4 steps for one degree rotation
// Angle to Step
// Last has a ratio of 8:1>> 8x3200=25600 steps for 360 degree rotation >> 71.11 steps for one degree

const float JOG1 = 82.22;
const float JOG2 = 1098.4;
const float JOG3 = 71.11;
//Ramp 1.4

AccelStepper BaseStepper(1, 54, 55);
AccelStepper MiddleStepper(1, 60, 61);
AccelStepper LastStepper(1, 46, 48);

//CNC SHIELD UNO when using enable these lines
/*AccelStepper BaseStepper(1, 2, 5);
AccelStepper MiddleStepper(1, 3, 6);
AccelStepper LastStepper(1, 4, 7);*/
MultiStepper steppers;
Servo gripperservo1;
Servo gripperservo2;

int theta1Array[100];
int theta2Array[100];
int theta3Array[100];
int gripperArray[100];
int positionsCounter = 0;

void setup() {
  // put your setup code here, to run once:
  //for Ramp 1.4. When using in CNC shield disable it
  BaseStepper.setEnablePin(X_ENABLE_PIN);
  BaseStepper.setPinsInverted(false, false, true); //invert logic of enable pin
  BaseStepper.enableOutputs();
  MiddleStepper.setEnablePin(Y_ENABLE_PIN);
  MiddleStepper.setPinsInverted(false, false, true); //invert logic of enable pin
  MiddleStepper.enableOutputs();
  LastStepper.setEnablePin(Z_ENABLE_PIN);
  LastStepper.setPinsInverted(false, false, true); //invert logic of enable pin
  LastStepper.enableOutputs();


  pinMode(limitSwitch1, INPUT_PULLUP);
  pinMode(limitSwitch2, INPUT_PULLUP);
  pinMode(limitSwitch3, INPUT_PULLUP);

  Serial.begin(115200); 
  BaseStepper.setMaxSpeed(3000);
  BaseStepper.setSpeed(1000);
  BaseStepper.setAcceleration(1000);
  MiddleStepper.setMaxSpeed(4000);
  MiddleStepper.setSpeed(2000);
  MiddleStepper.setAcceleration(1000);
  LastStepper.setMaxSpeed(3000);
  LastStepper.setSpeed(1000);
  LastStepper.setAcceleration(1000);
  gripperservo1.attach(11); // attaching the servo motors to the related pins on Ramp Board,
  gripperservo2.attach(6);
  gripperservo2.write(95);
  gripperservo1.write(0);
  // setting the initial Position of the Servo to come to the physical zero Position of the gripper. 
  steppers.addStepper(BaseStepper); // adding each stepper to the multipstepper array for synchronous motion
  steppers.addStepper(MiddleStepper);
  steppers.addStepper(LastStepper);
}

 
void loop() {

 if (Serial.available()) {
    content = Serial.readString(); // Read the incomding data from Python
    // Extract the data from the string and put into separate float variables (data[] array)
    for (int i = 0; i < 100; i++) {
      int index = content.indexOf(","); // locate the first ","
      data[i] = atol(content.substring(0, index).c_str()); //Extract the number from start to the ","
      content = content.substring(index + 1); //Remove the number from the string
    }
  }
 if (data[9]==0){
    //calculation of inverse kinematics
    th1 = atan2(data[1],data[0]);
    d = sqrt(data[0]*data[0]+data[1]*data[1]+(data[2]-l_1)*(data[2]-l_1));
    th3 = acos(((d*d-l_2*l_2-l_3*l_3)/(2*l_2*l_3)));
    D = sqrt (data[0]*data[0]+data[1]*data[1]);
    alpha=atan2(data[2]-l_1,D);
    beta=acos((l_3*l_3-d*d-l_2*l_2)/-(2*d*l_2));
    th2=alpha+beta;
    
    // changing the angle in order to fit the zero position of the robot arm
    theta_1=th1*(180/PI);
    theta_2=(th2*180/PI)-90;
    theta_3=-(th3*180/PI);

    if ( theta_1>=-120 && theta_1<=120 && theta_2<=95 && theta_2>=-95 && theta_3<=95 && theta_3>=-95){
    
      // Synchronous Motion
      long positions[3];
      positions[0]= theta_1*JOG1;
      positions[1]= theta_2*JOG2;
      positions[2]= -theta_3*JOG3;
      steppers.moveTo(positions);
      steppers.runSpeedToPosition();
      BaseStepper.setCurrentPosition(theta_1*JOG1);
      MiddleStepper.setCurrentPosition(theta_2*JOG2);
      LastStepper.setCurrentPosition(-theta_3*JOG3);
      // Joint by Joint Motion
      /* BaseStepper.moveTo(theta_1*JOG1);
      BaseStepper.runToPosition();
      delay(100);
      MiddleStepper.moveTo(theta_2*JOG2);
      MiddleStepper.runToPosition();
      delay(100);
      LastStepper.moveTo(-theta_3*JOG3);
      LastStepper.runToPosition();
      delay(100);*/
  }
 }

  
  if (data[9]==1){
    // Synchronous Motion
    if  (data[3]>= -150 && data[3]<= 150 && data[4]>= -100 && data[4]<= 100 && data[5]>= -110 && data[5]<= 110) {
    long positions[3];
    positions[0]= data[3]*JOG1;
    positions[1]= data[4]*JOG2;
    positions[2]= data[5]*JOG3;
    
    steppers.moveTo(positions);
    steppers.runSpeedToPosition();
    BaseStepper.setCurrentPosition(data[3]*JOG1);
    MiddleStepper.setCurrentPosition(data[4]*JOG2);
    LastStepper.setCurrentPosition(data[5]*JOG3); 
  }

    //Joint to Joint Motion, 
    /*if  (data[3]>= -150 && data[3]<= 150) {
      BaseStepper.moveTo(data[3]*JOG1);
      BaseStepper.runToPosition();
      delay(100);
      }
    if  (data[4]>= -100 && data[4]<= 100){
      MiddleStepper.moveTo(data[4]*JOG2);
      MiddleStepper.runToPosition();
      delay(100);
      }
    if  (data[5]>= -150 && data[5]<= 150){ 
      LastStepper.moveTo(-data[5]*JOG3);
      LastStepper.runToPosition();
      delay(100);
      }
  */
  }

  if (data[6]>= 0 && data[6]<= 70){
      previousValue1 = currentValue1;
      gripperservo1.write(data[6]);
      gripperservo2.write(95-data[6]);
      currentValue1 = data[6];
    if (previousValue1 <= currentValue1){
      for (pos = previousValue1; pos <= currentValue1; pos += 1) { 
      gripperservo1.write(pos);              // tell servo to go to position in variable 'pos'
      gripperservo2.write(95-pos);
      delay(10);                       
      }
    }
     if (previousValue1 >= currentValue1){
      for (pos = previousValue1; pos >= currentValue1; pos -= 1) { 
      gripperservo1.write(pos);
      gripperservo2.write(95-pos); 
      delay(10);                       
      }
    }
   }
  // if the servo opens and closes really fast the quality is not as good thats why the below for loop was written to move the servo slower
  /*if (data[6]>= 0 && data[6]<= 70){
      previousValue1 = currentValue1;
      previousValue2 = currentValue2;
      currentValue1 = data[6];
      currentValue2 = 95-data[6];
      if (previousValue1 <= currentValue1){
      for (pos = previousValue1; pos <= currentValue1; pos += 1) { 
      gripperservo1.write(pos);              // tell servo to go to position in variable 'pos'
      delay(50);                       
        }
      }
      if (previousValue2 >= currentValue2){
      for (pos = previousValue2; pos >= currentValue2; pos -= 1) { 
      gripperservo2.write(pos);
      delay(50);                       
        }
      }              
      if (previousValue1 >= currentValue1){
      for (pos = previousValue1; pos >= currentValue1; pos -= 1) { 
      gripperservo1.write(pos);              // tell servo to go to position in variable 'pos'
      delay(50);                       
        }
      }
      if (previousValue1 <= currentValue2){
      for (pos = previousValue2; pos <= currentValue2; pos += 1) { 
      gripperservo2.write(pos);
      delay(50);                       
        }
      }
  } */                     
    // fOR ONE Servo Only
 /*if (data[6]>= 0 && data[6]<= 90){
      previousValue = currentValue;
      currentValue = data[6];
    if (previousValue <= currentValue){
      for (pos = previousValue; pos <= currentValue; pos += 1) { 
      gripperservo1.write(pos);              // tell servo to go to position in variable 'pos'
      
      delay(10);                       
      }
    }
     if (previousValue >= currentValue){
      for (pos = previousValue; pos >= currentValue; pos -= 1) { 
      gripperservo1.write(pos); 
      delay(10);                       
      }
    }
   }*/
 if (data[8]==1){
  Homing(); // data 8 = 0
  }
}


 void Homing() {
  // Base_Joint
  while (digitalRead(limitSwitch1) != 1) {
    BaseStepper.setSpeed(+200);
    BaseStepper.runSpeed();
    BaseStepper.setCurrentPosition(+2600); // When limit switch pressed set position to -5440 steps
  }
  delay(20);

  BaseStepper.moveTo(0);
  while (BaseStepper.currentPosition() != 0) {
    BaseStepper.setSpeed(-200);
    BaseStepper.runSpeed();
  }
  // Homing Middle_Joint
  while (digitalRead(limitSwitch2) != 1) {
    MiddleStepper.setSpeed(-2000);
    MiddleStepper.runSpeed();
    MiddleStepper.setCurrentPosition(-5000); // When limit switch pressed set position to -5440 steps
  }
  delay(20);

  MiddleStepper.moveTo(0);
  while (MiddleStepper.currentPosition() != 0) {
    MiddleStepper.setSpeed(+200);
    MiddleStepper.runSpeed();
  }
  //*Homing Last_Joint
  while (digitalRead(limitSwitch3) != 1) {
    LastStepper.setSpeed(500);
    LastStepper.runSpeed();
    LastStepper.setCurrentPosition(-500); // When limit switch pressed set position to -5440 steps
  }
  delay(20);

  LastStepper.moveTo(0);
  while (LastStepper.currentPosition() != 0) {
    LastStepper.setSpeed(500);
    LastStepper.runSpeed();
  }
  
}
