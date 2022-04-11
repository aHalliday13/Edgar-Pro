/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*    Module:       main.cpp                                                          */
/*    Author:       Anthony Halliday, Camden Wolfield                                 */
/*    Created:      Thu Sep 26 2019                                                   */
/*    Description:  Edgar Pro                                                         */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

// "It's always the programmer's fault" - Some genius on the vex fourms

// ---- START SUPPORTED AUTON ROUTES ----
// [Name]               [Support Level]
// LANWP                Incomplete
// SPEED                Complete
// RANWP                Complete
// RAR                  Complete
// SKILL                In Progress
// ZACH1                Complete
// ZACH2                In Progress
// ZACH3                Not Started
// ZACH4                Not Started
// ZACH5                Not Started
// ZACH6                Not Started
// ZACH7                Not Started
// ZACH8                Not Started
// ---- END SUPPORTED AUTON ROUTES ----

#include "vex.h"

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// inertialSensor       inertial      10              
// frontHook            digital_out   H               
// frontMogo            motor         7               
// ringLift             motor         11              
// LeftDriveSmartA      motor         1               
// LeftDriveSmartB      motor         3               
// LeftDriveSmartC      motor         9               
// RightDriveSmartA     motor         2               
// RightDriveSmartB     motor         4               
// RightDriveSmartC     motor         6               
// rearHook             digital_out   G               
// autonHook            digital_out   A               
// frontVision          vision        17              
// rearVision           vision        15              
// ---- END VEXCODE CONFIGURED DEVICES ----
#include "cmath"

using namespace vex;
competition Competition;

motor_group LeftDriveSmart = motor_group(LeftDriveSmartA, LeftDriveSmartB, LeftDriveSmartC);
motor_group RightDriveSmart = motor_group(RightDriveSmartA, RightDriveSmartB, RightDriveSmartC);

// define variables and macros here
int calcVelocity;
int prevTurn;
float heading;
#define DEADBAND 15
#define MAXVELOCITY 40
#define MINVELOCITY 10

// define functions here
void drive2obs(directionType dir){
  LeftDriveSmart.spin(dir,200,velocityUnits::pct);
  RightDriveSmart.spin(dir,200,velocityUnits::pct);
  if(dir==directionType::fwd){
    waitUntil(LeftDriveSmart.velocity(percentUnits::pct)>5 && RightDriveSmart.velocity(percentUnits::pct)>5);
    waitUntil(LeftDriveSmart.velocity(percentUnits::pct)<5 && RightDriveSmart.velocity(percentUnits::pct)<5);
  }
  else{
    waitUntil(LeftDriveSmart.velocity(percentUnits::pct)<5 && RightDriveSmart.velocity(percentUnits::pct)<5);
    waitUntil(LeftDriveSmart.velocity(percentUnits::pct)>5 && RightDriveSmart.velocity(percentUnits::pct)>5);
  }
  LeftDriveSmart.stop();
  RightDriveSmart.stop();
}

void InertialRight(float targetTurn) {
  prevTurn = inertialSensor.rotation(degrees);

  task::sleep(100);

  heading = targetTurn + prevTurn;

  while(inertialSensor.rotation(degrees) < heading) {
    calcVelocity = std::abs(heading - inertialSensor.rotation(degrees));

    if(calcVelocity > MAXVELOCITY) {
      RightDriveSmart.spin(directionType::rev, MAXVELOCITY, velocityUnits::pct);
      LeftDriveSmart.spin(directionType::fwd, MAXVELOCITY, velocityUnits::pct);
    } 
    else if(calcVelocity < MINVELOCITY) {
      RightDriveSmart.spin(directionType::rev, MINVELOCITY, velocityUnits::pct);
      LeftDriveSmart.spin(directionType::fwd, MINVELOCITY, velocityUnits::pct);
    } 
    else {
      RightDriveSmart.spin(directionType::rev, calcVelocity, velocityUnits::pct);
      LeftDriveSmart.spin(directionType::fwd, calcVelocity, velocityUnits::pct);
    }
  }
  RightDriveSmart.stop(brakeType::brake);
  LeftDriveSmart.stop(brakeType::brake);
  task::sleep(100);
}

void InertialLeft(float targetTurn) {
  prevTurn = inertialSensor.rotation(degrees);

  task::sleep(100);

  heading = targetTurn * -1 + prevTurn;

  while(inertialSensor.rotation(degrees) > heading) {
    calcVelocity = std::abs(heading - inertialSensor.rotation(degrees));
    if(calcVelocity > MAXVELOCITY) {
      RightDriveSmart.spin(directionType::fwd, MAXVELOCITY, velocityUnits::pct);
      LeftDriveSmart.spin(directionType::rev, MAXVELOCITY, velocityUnits::pct);
    } 
    else if(calcVelocity < MINVELOCITY) {
      RightDriveSmart.spin(directionType::fwd, MINVELOCITY, velocityUnits::pct);
      LeftDriveSmart.spin(directionType::rev, MINVELOCITY, velocityUnits::pct);
    } 
    else {
      RightDriveSmart.spin(directionType::fwd, calcVelocity, velocityUnits::pct);
      LeftDriveSmart.spin(directionType::rev, calcVelocity, velocityUnits::pct);
    }
  }
  RightDriveSmart.stop(brakeType::brake);
  LeftDriveSmart.stop(brakeType::brake);
  task::sleep(100);
}

void driveIN(float dist, directionType dir,float volt) {
  // 3:7 gear ratio motor:wheel
  // 7 rotations of the motor is three rotations of the wheel
  // Wheel circumfrence 12.9590697
  // 7 rotations of the motor is 38.8772091
  // 1 rotation of the wheel is 5.5388701
  // need to ajust all routines to actual inches before changing value in code
  // current value is 5+3/4 in

  dist=dist/(5.75);
  if(dir==directionType::rev) {
    dist=0-dist;
  }
  LeftDriveSmart.resetPosition();
  RightDriveSmart.resetPosition();
  LeftDriveSmart.spin(dir,volt,voltageUnits::volt);
  RightDriveSmart.spin(dir,volt,voltageUnits::volt);
  if(dir==directionType::rev) {
    waitUntil(LeftDriveSmart.position(rotationUnits::rev)<dist && RightDriveSmart.position(rotationUnits::rev)<dist);
  }
  else {
    waitUntil(LeftDriveSmart.position(rotationUnits::rev)>dist && RightDriveSmart.position(rotationUnits::rev)>dist);
  } 
  LeftDriveSmart.stop(brakeType::brake);
  RightDriveSmart.stop(brakeType::brake);
}

// define auton routines here
void rightAutonRight(void) {
  // right auton code goes here
  // open claw, drive forward to neutral mogo, latch on and lift
  frontHook.set(false);
  driveIN(47,directionType::fwd,12.0);
  frontMogo.spinFor(150,rotationUnits::deg,false);
  frontHook.set(true);
  driveIN(15,directionType::rev,12.0);
  InertialLeft(40);
  driveIN(20,directionType::rev,12.0);
  rearHook.set(true);
  ringLift.spinFor(3,timeUnits::sec,100,velocityUnits::pct);
}

void skillsAuton(void) {
  frontHook.set(false);
  driveIN(10,directionType::fwd,55);
 //rearMogo.spinTo(-700,rotationUnits::deg); // Depreciated [DELETE]
  driveIN(11,directionType::rev,55);
  rearHook.set(true);
  ringLift.spinFor(2,timeUnits::sec,100,velocityUnits::pct);
  driveIN(10,directionType::fwd,55);
  InertialRight(95);
  driveIN(50,directionType::fwd,55);
  frontHook.set(true);
  frontMogo.spinFor(300,rotationUnits::deg);
  InertialRight(10);
  driveIN(30,directionType::fwd,55);
  frontMogo.spinFor(-300,rotationUnits::deg);
  frontHook.set(false);
  InertialLeft(90);
  rearHook.set(false);
  driveIN(10,directionType::fwd,55);
  InertialRight(180);
  driveIN(13,directionType::rev,55);
  rearHook.set(true);
  InertialRight(11);
  driveIN(45,directionType::fwd,55);
  frontHook.set(true);
  driveIN(35,directionType::fwd,55);
  InertialRight(45);
  drive2obs(directionType::fwd);

}

void leftAutonNoWP(void){
  //rearMogo.spinTo(-650,rotationUnits::deg,200, velocityUnits::pct,false); //Depreciated [DELETE]
  driveIN(47,directionType::fwd,12.0);
  frontHook.set(true);
  frontMogo.spinTo(110,rotationUnits::deg);
  InertialLeft(95);
  driveIN(27,directionType::rev,30);
  rearHook.set(true);
  InertialLeft(45);
  driveIN(30,directionType::fwd,70);
  InertialRight(180);
  driveIN(30,directionType::rev,70);
}

void rightAutonNoWP(void){
  // open claw, drive forward to neutral mogo, latch on and lift
  driveIN(55,directionType::fwd,150);
  frontHook.set(true);
  frontMogo.spinFor(150,rotationUnits::deg,false);
  InertialRight(45);
  driveIN(15,directionType::rev,12.0);
  InertialRight(50);
  driveIN(20,directionType::rev,12.0);
  rearHook.set(true);
  InertialRight(45);
  driveIN(50,directionType::fwd,12.0);
}

void speedyAuton(void) {
  Controller1.Screen.print(" PRANKD LOL");
  driveIN(47,directionType::fwd,12.0);
  LeftDriveSmart.spin(directionType::rev,200,velocityUnits::pct);
  RightDriveSmart.spin(directionType::rev,200,velocityUnits::pct);
  frontHook.set(true);
  frontMogo.spinFor(90,rotationUnits::deg,false);
}

void zach1(){
  // Start on right, grab right neutral with front, grab alliance goal with back, load with rings from field
  frontHook.set(false);
  driveIN(47,directionType::fwd,12.0);
  frontHook.set(true);
  frontMogo.startSpinTo(800,rotationUnits::deg);
  driveIN(50,directionType::rev,12.0);
  InertialLeft(125);
  driveIN(17,directionType::rev,7.0);
  rearHook.set(true);
  task::sleep(500);
  frontMogo.stop(brakeType::hold);
  ringLift.spin(directionType::fwd,50,velocityUnits::pct);
  driveIN(4.75,directionType::fwd,12.0);
  InertialRight(125);
  ringLift.setVelocity(200, percentUnits::pct);
  driveIN(55,directionType::fwd,5.5);
  driveIN(55,directionType::rev,5.5);
  InertialLeft(180);
  ringLift.stop();
}

#define MANUAL zach2

void zach2(){
  // start on right, grab yellow center with front, grab alliance goal with rear, load with rings from field
  driveIN(60,directionType::fwd,12.0);
  frontHook.set(true);
  frontMogo.startSpinTo(400,rotationUnits::deg,200,velocityUnits::pct);
  driveIN(43,directionType::rev,12.0);
  task::sleep(500);
  InertialLeft(42.5);
  driveIN(15,directionType::rev,7.0);
  rearHook.set(true);
  driveIN(5,directionType::fwd,12.0);
  InertialRight(92.5);
  //ringLift.spin(fwd,12.0,voltageUnits::volt);
  //driveIN(60,directionType::fwd,7.0);
  //driveIN(70,directionType::rev,12.0);
}

void zach3(){
  // start on left, speed for left yellow, back up to alliance on platform, grab, load with match load rings
  driveIN(47,directionType::fwd,12.0);
  LeftDriveSmart.spin(directionType::rev,200,velocityUnits::pct);
  RightDriveSmart.spin(directionType::rev,200,velocityUnits::pct);
  frontHook.set(true);
  frontMogo.spinFor(90,rotationUnits::deg,false);
}

void zach4(){
  // start on right, speed for right yellow with front, grab center yellow with back, place center yellow in corner, grab alliance with back, get field rings
}

void zach5(){
  // start on right, speed for center, back up to alliance, load with field rings, grab right yellow with front
}

void zach6(){
  // same as 5, but fake dash for right goal instead of center
}

void zach7(){
  // start on left, fake for left yellow, go for center, bring it back, grab alliance with rear lift, load with match loads
}

void zach8(){
  // open slot for future expansion
}

// now that autons are defined, we can define the auton selection code
//autonRoutes [13] = {"LANWP","SPEED","RANWP","RARNR","SKILL","ZACH1","ZACH2","ZACH3","ZACH4","ZACH5","ZACH6","ZACH7","ZACH8"};

bool waitForComplete = true;
int autonIndex = 0;

void autonSelect(){
  while(waitForComplete){
    if (Controller1.ButtonUp.pressing()){
      autonIndex++;
      waitUntil(!Controller1.ButtonUp.pressing());
    }
    else if (Controller1.ButtonDown.pressing()){
      autonIndex--;
      waitUntil(!Controller1.ButtonDown.pressing());
    }
    else if (Controller1.ButtonX.pressing()) {
      Controller1.Screen.setCursor(4, 1);
      //Controller1.Screen.print("= %s",autonRoutes[autonIndex].c_str());
      waitForComplete = false;
    }
    else{
      //Controller1.Screen.print("> %s",autonRoutes[autonIndex].c_str());
      Controller1.Screen.setCursor(4, 1);
    }

    // prevent overflow
    autonIndex = autonIndex < 0 ? autonIndex+13 : autonIndex;
    autonIndex = autonIndex > 12 ? 0 : autonIndex;

  }
  switch(autonIndex){
    case(0):
      Competition.autonomous(leftAutonNoWP);
    case(1):
      Competition.autonomous(speedyAuton);     
    case(2):
      Competition.autonomous(rightAutonNoWP);
    case(3):
      Competition.autonomous(rightAutonRight);
    case(4):
      Competition.autonomous(skillsAuton);
    case(5):
      Competition.autonomous(zach1);
    case(6):
      Competition.autonomous(zach2);
    case(7):
      Competition.autonomous(zach3);
    case(8):
      Competition.autonomous(zach4);
    case(9):
      Competition.autonomous(zach5);
    case(10):
      Competition.autonomous(zach6);
    case(11):
      Competition.autonomous(zach7);
    case(12):
      Competition.autonomous(zach8);
  }
}

// define pre-auton routine here
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Reset important encoders, close the front claw, and open the back
  frontHook.set(false);
  rearHook.set(false);
  frontMogo.resetPosition();
  // Calibrate Inertial and wait until complete
  inertialSensor.startCalibration();
  waitUntil(!inertialSensor.isCalibrating());
}

// define user control code here
void pneumaticSwitchFront(void) {
  // on button press, check if driver control is active, then if front hook is pressurized, depresurize, otherwise, pressurize
  if(competition().isDriverControl()){
    if(frontHook.value()) {
      frontHook.set(false);
    }
    else {
      frontHook.set(true);
    }
  }
}

void usercontrol(void) {
  // Bind button x to front hook
  Controller1.ButtonX.pressed(pneumaticSwitchFront);
  while(true) {
      // Spin drivetrain motors
      LeftDriveSmart.spin(vex::directionType::undefined, (2*abs(Controller1.Axis3.position()+Controller1.Axis4.position()) >= DEADBAND) ? 0-(Controller1.Axis3.position()+Controller1.Axis4.position()) : 0, velocityUnits::pct);
      RightDriveSmart.spin(vex::directionType::undefined, (2*abs(Controller1.Axis3.position()-Controller1.Axis4.position()) >= DEADBAND) ? 0-(Controller1.Axis3.position()-Controller1.Axis4.position()) : 0, velocityUnits::pct);
      // Rear Lift up/down
      if(Controller1.ButtonL1.pressing()) {
        rearHook.set(true);
      }
      else if(Controller1.ButtonL2.pressing()) {
        rearHook.set(false);
      }
      // Rear auton hook
      if(Controller1.ButtonUp.pressing()) {
        autonHook.set(true);
      }
      else if(Controller1.ButtonDown.pressing()) {
        autonHook.set(false);
      }
      // Front Lift up/down
      if(Controller1.ButtonR1.pressing()) {
        frontMogo.spin(directionType::fwd, 100, percentUnits::pct);
      }
      else if(Controller1.ButtonR2.pressing()&&frontMogo.position(rotationUnits::deg)>=0) {
        frontMogo.spin(directionType::rev, 100, percentUnits::pct);
      }
      else {
        frontMogo.stop(brakeType::brake);
      }

      // Ringle lift controls
      ringLift.spin(vex::directionType::undefined, (abs(Controller1.Axis2.position()) >= DEADBAND) ? 0-(Controller1.Axis2.position()) : 0, velocityUnits::pct);
  }
}

// main() called on program start
int main() {
  // run the pre-auton routine
  pre_auton();
  // Manualy select an auton
  Competition.autonomous(MANUAL);
  // Autonomous Selection
  //autonSelect();
  // Set up callbacks for driver control period.
  Competition.drivercontrol(usercontrol);
  // Prevent main from exiting with an infinite loop.
  while(true) {
    task::sleep(100);
  }
}

