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
// rearMogoSwitch       limit         G               
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

void driveIN(int dist, directionType dir,int vel,bool waitForComplete) {
  dist=dist/(5.625);
  if(dir==directionType::rev) {
    dist=0-dist;
  }
  LeftDriveSmart.resetPosition();
  RightDriveSmart.resetPosition();
  LeftDriveSmart.spinFor(dist,rotationUnits::rev,vel,velocityUnits::pct,false);
  RightDriveSmart.spinFor(dist,rotationUnits::rev,vel,velocityUnits::pct,waitForComplete);
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
  driveIN(55,directionType::fwd,100,true);
  frontHook.set(true);
  //Step 3: Drive away with YeMogo and hide it in corner
  frontMogo.spinFor(300,rotationUnits::deg);
  InertialLeft(180);
  driveIN(40,directionType::fwd,55,true);
  frontMogo.spinFor(-300,rotationUnits::deg);
  frontHook.set(false);
  //Step 4: Avoid hoarding penalty
  driveIN(15,directionType::rev,55,true);
  //Step 5: Profit
}

void leftAutonCenter(void) {
  
  // left auton code goes here
  frontHook.set(false);
  //Step 1: Dump Preloads
  ringLift.setVelocity(100,percentUnits::pct);
  ringLift.spinFor(13,rotationUnits::rev);
  //Step 2: "Mad Dash" for middle Yemogo
  driveIN(6,directionType::fwd,70,true);
  InertialRight(90);
  driveIN(18,directionType::fwd,70,true);
  InertialRight(90);
  driveIN(27,directionType::fwd,70,true);
  InertialLeft(55);
  driveIN(35,directionType::fwd,70,true);
  frontHook.set(true);
  frontMogo.spinTo(300,rotationUnits::deg);
  driveIN(40,directionType::rev,100,true);
  InertialRight(30);
  //Step 3: Profit
}

void rightAutonRight(void) {
  
  // right auton code goes here
  // open claw, drive forward to neutral mogo, latch on and lift
  frontHook.set(false);
  driveIN(60,directionType::fwd,55,true);
  frontHook.set(true);
  task::sleep(500);
  frontMogo.spinFor(500,rotationUnits::deg);
  // drop lift, back up to aliance mogo, spin to grab it with rear lift
  rearMogo.spinTo(700, rotationUnits::deg,false);
  driveIN(17,directionType::rev,55,true);
  InertialLeft(35);
  LeftDriveSmart.setVelocity(25,percentUnits::pct);
  RightDriveSmart.setVelocity(25,percentUnits::pct);
  driveIN(20,directionType::rev,20,true);
  rearMogo.spinTo(600, rotationUnits::deg);
  ringLift.spinFor(3,timeUnits::sec,100,velocityUnits::pct);
  InertialRight(35);
}

void rightAutonCenter(void) {
  
  // right auton code goes here
  // open claw, turn left, drive forward to center yemogo, latch on and lift
  frontHook.set(false);
  driveIN(35,directionType::fwd,70,true);
  InertialLeft(45);
  driveIN(40,directionType::fwd,70,true);
  frontHook.set(true);
  frontMogo.spinFor(500,rotationUnits::deg);
  // drop lift, back up to aliance mogo, load rings
  rearMogo.spinTo(700, rotationUnits::deg);
  driveIN(70,directionType::rev,55,true);
  rearMogo.spinTo(600, rotationUnits::deg);
  ringLift.spinFor(3,timeUnits::sec,100,velocityUnits::pct);
}

void soloWinPoint(void){
  
  frontMogo.spinFor(350,rotationUnits::deg);
  driveIN(7,directionType::rev,55,true);
  InertialLeft(90);
  driveIN(22,directionType::fwd,55,true);
  InertialLeft(87);
  rearMogo.spinTo(700,rotationUnits::deg);
  driveIN(85,directionType::rev,100,true);
  InertialLeft(15);
  driveIN(23,directionType::rev,55,true);
  rearMogo.spinTo(550,rotationUnits::deg);
  ringLift.spinFor(3,timeUnits::sec,90,velocityUnits::pct);
  InertialRight(90);
}

void skillsAuton(void) {
  Controller1.Screen.print("SKILL");
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
  if (Brain.Screen.xPosition()<200){
    if (Brain.Screen.yPosition()<120){
      Controller1.Screen.print("LL");
      Competition.autonomous(leftAutonLeft);
    }
    else if (Brain.Screen.yPosition()>120){
      Controller1.Screen.print("LC");
      Competition.autonomous(leftAutonCenter);
    }
  }
  else if (Brain.Screen.xPosition()>280){
    if (Brain.Screen.yPosition()<120){
      Controller1.Screen.print("RR");
      Competition.autonomous(rightAutonRight);
    }
    else if (Brain.Screen.yPosition()>120){
      Controller1.Screen.print("RC");
      Competition.autonomous(rightAutonCenter);
    }
  }
  else{
    Controller1.Screen.print("SAWP");
    Competition.autonomous(soloWinPoint);
  }
  Brain.Screen.clearScreen();
  // Reset important encoders and close the front claw
  frontHook.set(false);
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
      LeftDriveSmart.spin(vex::directionType::undefined, 2*(abs(Controller1.Axis3.position()-Controller1.Axis4.position()) >= DEADBAND) ? 0-(Controller1.Axis3.position()-Controller1.Axis4.position()) : 0, velocityUnits::pct);
      RightDriveSmart.spin(vex::directionType::undefined, 2*(abs(Controller1.Axis3.position()+Controller1.Axis4.position()) >= DEADBAND) ? 0-(Controller1.Axis3.position()+Controller1.Axis4.position()) : 0, velocityUnits::pct);
      // Rear Lift up/down
      if(Controller1.ButtonL1.pressing()&&!rearMogoSwitch.value()) {
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
