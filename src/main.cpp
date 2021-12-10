/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*    Module:       main.cpp                                                          */
/*    Author:       VEX                                                               */
/*    Created:      Thu Sep 26 2019                                                   */
/*    Description:  E.D.G.A.R. (Ergonomicaly Decent Goal-Grabbing Automated Robot)    */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// frontMogo            motor         8               
// rearMogo             motor         6               
// frontHook            digital_out   H               
// inertialSensor       inertial      13              
// Controller1          controller                    
// LeftDriveSmart       motor_group   11, 12          
// RightDriveSmart      motor_group   1, 2            
// ringLift             motor         19              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "cmath"

using namespace vex;
competition Competition;

// define vars and macros here
int leftDrive;
int rightDrive;
int calcVelocity;
int prevTurn;
float heading;
bool autonSide;
#define DEADBAND 15
#define MAXVELOCITY 40
#define MINVELOCITY 10

// define functions here
void pneumaticSwitchFront(void) {
  // on button press, if front hook is pressurized, depresurize, otherwise, pressurize
  if(frontHook.value()) {
    frontHook.set(false);
  }
  else {
    frontHook.set(true);
  }
}

void InertialRight(float targetTurn) {
  prevTurn = inertialSensor.rotation(degrees);

  task::sleep(100);

  heading = targetTurn + prevTurn;

  while(inertialSensor.rotation(degrees) < heading) {
    printf("%lf\n",inertialSensor.rotation());
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

void InertialLeft(float targetTurn) {
  prevTurn = inertialSensor.rotation(degrees);

  task::sleep(100);

  heading = targetTurn * -1 + prevTurn;

  while(inertialSensor.rotation(degrees) > heading) {
    printf("%lf\n",inertialSensor.rotation());
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

void driveIN(int dist, directionType dir,int vel) {
  dist=dist/(5.625);
  if(dir==directionType::rev) {
    dist=0-dist;
  }
  LeftDriveSmart.resetPosition();
  RightDriveSmart.resetPosition();
  LeftDriveSmart.spin(dir,vel,percentUnits::pct);
  RightDriveSmart.spin(dir,vel,percentUnits::pct);
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
void leftAutonLeft(void) {
  // left auton code goes here
  //Step 1: Dump Preloads
  ringLift.setVelocity(100,percentUnits::pct);
  ringLift.spinFor(13,rotationUnits::rev);
  //Step 2: Pick up YeMogo
  InertialRight(90);
  frontHook.set(false);
  driveIN(55,directionType::fwd,100);
  frontHook.set(true);
  //Step 3: Drive away with YeMogo and hide it in corner
  frontMogo.spinFor(300,rotationUnits::deg);
  InertialLeft(180);
  driveIN(40,directionType::fwd,55);
  frontMogo.spinFor(-300,rotationUnits::deg);
  frontHook.set(false);
  //Step 4: Avoid hoarding penalty
  driveIN(15,directionType::rev,55);
  //Step 5: Profit
}

void leftAutonCenter(void) {
  // left auton code goes here
  frontHook.set(false);
  //Step 1: Dump Preloads
  ringLift.setVelocity(100,percentUnits::pct);
  ringLift.spinFor(13,rotationUnits::rev);
  //Step 2: "Mad Dash" for middle Yemogo
  driveIN(6,directionType::fwd,70);
  InertialRight(90);
  driveIN(18,directionType::fwd,70);
  InertialRight(90);
  driveIN(27,directionType::fwd,70);
  InertialLeft(55);
  driveIN(35,directionType::fwd,70);
  frontHook.set(true);
  frontMogo.spinTo(300,rotationUnits::deg);
  driveIN(40,directionType::rev,100);
  InertialRight(30);
  //Step 3: Profit
}

void rightAutonRight(void) {
  // right auton code goes here
  // open claw, drive forward to neutral mogo, latch on and lift
  frontHook.set(false);
  driveIN(60,directionType::fwd,55);
  frontHook.set(true);
  task::sleep(500);
  frontMogo.spinFor(500,rotationUnits::deg);
  // drop lift, back up to aliance mogo, spin to grab it with rear lift
  rearMogo.spinTo(700, rotationUnits::deg);
  driveIN(17,directionType::rev,55);
  InertialLeft(35);
  LeftDriveSmart.setVelocity(25,percentUnits::pct);
  RightDriveSmart.setVelocity(25,percentUnits::pct);
  driveIN(20,directionType::rev,20);
  rearMogo.spinTo(600, rotationUnits::deg);
  ringLift.spinFor(3,timeUnits::sec,100,velocityUnits::pct);
  InertialRight(35);
}

void rightAutonCenter(void) {
  // right auton code goes here
  // open claw, turn left, drive forward to center yemogo, latch on and lift
  frontHook.set(false);
  driveIN(35,directionType::fwd,70);
  InertialLeft(45);
  driveIN(40,directionType::fwd,70);
  frontHook.set(true);
  frontMogo.spinFor(500,rotationUnits::deg);
  // drop lift, back up to aliance mogo, load rings
  rearMogo.spinTo(700, rotationUnits::deg);
  driveIN(70,directionType::rev,55);
  rearMogo.spinTo(600, rotationUnits::deg);
  ringLift.spinFor(3,timeUnits::sec,100,velocityUnits::pct);
}

void soloWinPoint(void){
  frontMogo.spinFor(350,rotationUnits::deg);
  driveIN(7,directionType::rev,55);
  InertialLeft(90);
  driveIN(25,directionType::fwd,55);
  InertialLeft(87);
  rearMogo.spinTo(700,rotationUnits::deg);
  driveIN(105,directionType::rev,55);
  rearMogo.spinTo(550,rotationUnits::deg);
  ringLift.spinFor(3,timeUnits::sec,90,velocityUnits::pct);
  InertialRight(90);
}

void skillsAuton(void) {
  printf("skills\n");
}

// define pre-auton routine here
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // select auton routine
  Brain.Screen.drawRectangle(0,0,200,120);
  Brain.Screen.drawRectangle(280,0,200,120);
  Brain.Screen.drawRectangle(0,120,200,120);
  Brain.Screen.drawRectangle(280,120,200,120);
  Brain.Screen.drawRectangle(200,0,80,240);
  waitUntil(Brain.Screen.pressing());
  //Brain.Screen.xPosition();
  Competition.autonomous(soloWinPoint);
  /*if (Brain.Screen.xPosition()<200){
    if (Brain.Screen.yPosition()<120){
      Competition.autonomous(leftAutonLeft);
    }
    else if (Brain.Screen.yPosition()>120){
      Competition.autonomous(leftAutonCenter);
    }
  }
  else if (Brain.Screen.xPosition()>280){
    if (Brain.Screen.yPosition()<120){
      Competition.autonomous(rightAutonRight);
    }
    else if (Brain.Screen.yPosition()>120){
      Competition.autonomous(rightAutonCenter);
    }
  }
  else{
    Competition.autonomous(skillsAuton);
  }*/
  // Reset important encoders and close the front claw
  frontHook.set(true);
  frontMogo.resetPosition();
  rearMogo.resetPosition();
  // Calibrate Inertial and report status
  inertialSensor.startCalibration();
  waitUntil(!inertialSensor.isCalibrating());
}

// define user control code here
void usercontrol(void) {
  // Bind button x to front hook
  Controller1.ButtonX.pressed(pneumaticSwitchFront);
  while(true) {
      // Spin drivetrain motors
      LeftDriveSmart.spin(vex::directionType::undefined, (abs(Controller1.Axis3.position()-Controller1.Axis4.position()) >= DEADBAND) ? 0-(Controller1.Axis3.position()-Controller1.Axis4.position()) : 0, velocityUnits::pct);
      RightDriveSmart.spin(vex::directionType::undefined, (abs(Controller1.Axis3.position()+Controller1.Axis4.position()) >= DEADBAND) ? 0-(Controller1.Axis3.position()+Controller1.Axis4.position()) : 0, velocityUnits::pct);
      // Rear Lift up/down
      if(Controller1.ButtonL1.pressing()) {
        rearMogo.spin(directionType::rev, 50, percentUnits::pct);
      }
      else if(Controller1.ButtonL2.pressing()) {
        rearMogo.spin(directionType::fwd, 50, percentUnits::pct);
      }
      else {
        rearMogo.stop(brakeType::hold);
      }
      // Front Lift up/down
      if(Controller1.ButtonR1.pressing()&&frontMogo.position(rotationUnits::deg)<=910) {
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
  // run the pre-auton routine, this will set up auton routine
  pre_auton();
  // Set up callbacks for driver control period.
  Competition.drivercontrol(usercontrol);
  // Prevent main from exiting with an infinite loop.
  while(true) {
    task::sleep(100);
  }
}
