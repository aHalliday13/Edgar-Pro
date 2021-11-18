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
int prevTurn = 0;
#define DEADBAND 15
#define maxVelocity 40
#define minVelocity 10
float heading;

// define functions here
void pneumaticSwitchFront(void){
  // on button press, if front hook is pressurized, depresurize, otherwise, pressurize
  if(frontHook.value()){
    frontHook.set(false);
  }
  else{
    frontHook.set(true);
  }
}

void InertialRight(float targetTurn) 
{
  prevTurn = inertialSensor.rotation(degrees);

  task::sleep(100);

  heading = targetTurn + prevTurn;

  while (inertialSensor.rotation(degrees) < heading){
    printf("%lf\n",inertialSensor.rotation());
    calcVelocity = std::abs(heading - inertialSensor.rotation(degrees));

    if (calcVelocity > maxVelocity) {
      RightDriveSmart.spin(directionType::fwd, maxVelocity, velocityUnits::pct);
      LeftDriveSmart.spin(directionType::rev, maxVelocity, velocityUnits::pct);
    } else if (calcVelocity < minVelocity) {
      RightDriveSmart.spin(directionType::fwd, minVelocity, velocityUnits::pct);
      LeftDriveSmart.spin(directionType::rev, minVelocity, velocityUnits::pct);
    } else {
      RightDriveSmart.spin(directionType::fwd, calcVelocity, velocityUnits::pct);
      LeftDriveSmart.spin(directionType::rev, calcVelocity, velocityUnits::pct);
    }
  }
  RightDriveSmart.stop(brakeType::brake);
  LeftDriveSmart.stop(brakeType::brake);
  task::sleep(100);
}

void InertialLeft(float targetTurn) 
{
  prevTurn = inertialSensor.rotation(degrees);

  task::sleep(100);

  heading = targetTurn * -1 + prevTurn;

  while (inertialSensor.rotation(degrees) > heading){
    printf("%lf\n",inertialSensor.rotation());
    calcVelocity = std::abs(heading - inertialSensor.rotation(degrees));
    if (calcVelocity > maxVelocity) {
      RightDriveSmart.spin(directionType::rev, maxVelocity, velocityUnits::pct);
      LeftDriveSmart.spin(directionType::fwd, maxVelocity, velocityUnits::pct);
    } else if (calcVelocity < minVelocity) {
      RightDriveSmart.spin(directionType::rev, minVelocity, velocityUnits::pct);
      LeftDriveSmart.spin(directionType::fwd, minVelocity, velocityUnits::pct);
    } else {
      RightDriveSmart.spin(directionType::rev, calcVelocity, velocityUnits::pct);
      LeftDriveSmart.spin(directionType::fwd, calcVelocity, velocityUnits::pct);
    }
  }
  RightDriveSmart.stop(brakeType::brake);
  LeftDriveSmart.stop(brakeType::brake);
  task::sleep(100);
}

void driveIN(int dist, directionType dir){
  dist=dist/(5.625);
  if(dir==directionType::rev){
    dist=0-dist;
  }
  LeftDriveSmart.resetPosition();
  RightDriveSmart.resetPosition();
  LeftDriveSmart.spin(dir,55,percentUnits::pct);
  RightDriveSmart.spin(dir,55,percentUnits::pct);
  if(dir==directionType::rev){
    waitUntil(LeftDriveSmart.position(rotationUnits::rev)<dist && RightDriveSmart.position(rotationUnits::rev)<dist);
  }
  else{
    waitUntil(LeftDriveSmart.position(rotationUnits::rev)>dist && RightDriveSmart.position(rotationUnits::rev)>dist);
  }
  
  LeftDriveSmart.stop();
  RightDriveSmart.stop();
}


// define pre-auton routine here
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Pre-auton goes here
  frontHook.set(true);
  frontMogo.resetPosition();
  rearMogo.resetPosition();
  // Calibrate Inertial and report status
  inertialSensor.startCalibration();
  waitUntil(!inertialSensor.isCalibrating());
}

// define auton routine here
void auton(void) {
  frontHook.set(false);
  driveIN(50,directionType::fwd);
  frontHook.set(true);
  task::sleep(3000);
  frontMogo.spinFor(500,rotationUnits::deg);
  driveIN(12,directionType::rev);
  InertialLeft(45);

}

// define user control code here
void usercontrol(void) {
  // Bind button x to front hook
  Controller1.ButtonX.pressed(pneumaticSwitchFront);
  while (true) {
      // Spin drivetrain motors
      LeftDriveSmart.spin(vex::directionType::undefined, (abs(Controller1.Axis3.position()-Controller1.Axis4.position()) >= DEADBAND) ? 0-(Controller1.Axis3.position()-Controller1.Axis4.position()) : 0, velocityUnits::pct);
      RightDriveSmart.spin(vex::directionType::undefined, (abs(Controller1.Axis3.position()+Controller1.Axis4.position()) >= DEADBAND) ? 0-(Controller1.Axis3.position()+Controller1.Axis4.position()) : 0, velocityUnits::pct);
      // Rear Lift up/down
      if(Controller1.ButtonL1.pressing()){
        rearMogo.spin(directionType::rev, 50, percentUnits::pct);
      }
      else if(Controller1.ButtonL2.pressing()){
        rearMogo.spin(directionType::fwd, 50, percentUnits::pct);
      }
      else{
        rearMogo.stop(brakeType::hold);
      }
      // Front Lift up/down
      if(Controller1.ButtonR1.pressing()&&frontMogo.position(rotationUnits::deg)<=910){
        frontMogo.spin(directionType::fwd, 100, percentUnits::pct);
      }
      else if(Controller1.ButtonR2.pressing()&&frontMogo.position(rotationUnits::deg)>=0){
        frontMogo.spin(directionType::rev, 100, percentUnits::pct);
      }
      else{
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
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(auton);
  Competition.drivercontrol(usercontrol);
  // Prevent main from exiting with an infinite loop.
  while (true) {
    printf("%lf\n",LeftDriveSmart.position(rotationUnits::rev));
  }
}
