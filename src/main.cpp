/*------------------------------------------------------------------------------------*/
/*                                                                                    */
/*    Module:       main.cpp                                                          */
/*    Author:       Anthony Halliday, Camden Wolfield                                 */
/*    Created:      Thu Sep 26 2019                                                   */
/*    Description:  Edgar Pro                                                         */
/*                                                                                    */
/*------------------------------------------------------------------------------------*/

// "It's always the programmer's fault" - Some genius on the vex fourms

#include "vex.h"
#include "cmath"

using namespace vex;
competition Competition;

// Manualy create a motor group because the vex config wizzard can't have more than two motors for some reason
motor_group LeftDriveSmart = motor_group(LeftDriveSmartA, LeftDriveSmartB, LeftDriveSmartC);
motor_group RightDriveSmart = motor_group(RightDriveSmartA, RightDriveSmartB, RightDriveSmartC);

// define variables and macros here
int calcVelocity;
int prevTurn;
float heading;
#define DEADBAND 15
#define MAXVELOCITY 40
#define MINVELOCITY 10

// Essential driving functions
void drive2obs(directionType dir){
  LeftDriveSmart.spin(dir,12.0,voltageUnits::volt);
  RightDriveSmart.spin(dir,12.0,voltageUnits::volt);
  if(dir==directionType::fwd){
    waitUntil(LeftDriveSmart.velocity(percentUnits::pct)>5 && RightDriveSmart.velocity(percentUnits::pct)>5);
    waitUntil(LeftDriveSmart.velocity(percentUnits::pct)<5 && RightDriveSmart.velocity(percentUnits::pct)<5);
  }
  else{
    waitUntil(LeftDriveSmart.velocity(percentUnits::pct)<-5 && RightDriveSmart.velocity(percentUnits::pct)<-5);
    waitUntil(LeftDriveSmart.velocity(percentUnits::pct)>-5 || RightDriveSmart.velocity(percentUnits::pct)>-5);
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
void SKILL() {
  // insert camden code here
}

void SPEED() {
  Controller1.Screen.print(" PRANKD LOL");
  driveIN(47,directionType::fwd,12.0);
  LeftDriveSmart.spin(directionType::rev,200,velocityUnits::pct);
  RightDriveSmart.spin(directionType::rev,200,velocityUnits::pct);
  frontHook.set(true);
  frontMogo.spinFor(90,rotationUnits::deg,false);
}

// Right Side
void RANWP(){
  // open claw, drive forward to neutral mogo, latch on and lift
  driveIN(55,directionType::fwd,150);
  frontHook.set(true);
  frontMogo.setBrake(brakeType::hold);
  frontMogo.startSpinTo(800,rotationUnits::deg);
  InertialRight(35);
  driveIN(25,directionType::rev,12.0);
  InertialRight(65);
  driveIN(20,directionType::rev,7.0);
  rearHook.set(true);
  driveIN(50,directionType::fwd,12.0);
}

void RADWA(){
  // start on right, speed for right yellow with front, grab center yellow with back, place center yellow in corner, grab alliance with back, get field rings
  driveIN(55,directionType::fwd,150);
  frontHook.set(true);
  frontMogo.setBrake(brakeType::hold);
  frontMogo.startSpinTo(800,rotationUnits::deg);
  InertialRight(35);
  driveIN(25,directionType::rev,12.0);
  InertialRight(65);
  driveIN(18,directionType::rev,7.0);

  LeftDriveSmart.spin(directionType::rev,6.0,voltageUnits::volt);
  RightDriveSmart.spin(directionType::rev,6.0,voltageUnits::volt);
  LeftDriveSmart.resetPosition();
  RightDriveSmart.resetPosition();
  // 5.75 in/rot
  
  waitUntil(LeftDriveSmart.position(rotationUnits::rev)<2.875 && RightDriveSmart.position(rotationUnits::rev)<2.875);
  rearHook.set(true);
  waitUntil(LeftDriveSmart.position(rotationUnits::rev)<3.5 && RightDriveSmart.position(rotationUnits::rev)<3.5);
  
  driveIN(32,directionType::fwd,12.0);
  InertialLeft(110);
  driveIN(12,directionType::rev,7.0);
  rearHook.set(false);
  task::sleep(1000);
  InertialLeft(85);
  drive2obs(directionType::rev);
  rearHook.set(true);
  ringLift.spin(directionType::fwd,12.0,voltageUnits::volt);
}

void RARNR() {
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

void RARWR(){
  // Start on right, grab right neutral with front, grab alliance goal with back, load with rings from field
  frontHook.set(false);
  driveIN(47,directionType::fwd,12.0);
  frontHook.set(true);
  frontMogo.setBrake(brakeType::hold);
  frontMogo.startSpinTo(800,rotationUnits::deg,200,velocityUnits::pct);
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
}

void RACWR(){
  // start on right, grab yellow center with front, grab alliance goal with rear, load with rings from field
  driveIN(60,directionType::fwd,12.0);
  frontHook.set(true);
  frontMogo.setBrake(brakeType::hold);
  frontMogo.startSpinTo(800,rotationUnits::deg,200,velocityUnits::pct);
  driveIN(43,directionType::rev,12.0);
  task::sleep(500);
  InertialLeft(42.5);
  driveIN(15,directionType::rev,7.0);
  rearHook.set(true);
  driveIN(5,directionType::fwd,12.0);
  InertialRight(95);
  ringLift.spin(directionType::fwd,12.0,voltageUnits::volt);
  driveIN(55,directionType::fwd,7.0);
  driveIN(70,directionType::rev,12.0);
}

void RACRF(){
  // same as RACWR, but fake for right goal instead of center
  InertialLeft(30);
  driveIN(60,directionType::fwd,12.0);
  frontHook.set(true);
  frontMogo.setBrake(brakeType::hold);
  frontMogo.startSpinTo(800,rotationUnits::deg,200,velocityUnits::pct);
  driveIN(43,directionType::rev,12.0);
  task::sleep(500);
  InertialLeft(42.5);

  LeftDriveSmart.spin(directionType::rev,6.0,voltageUnits::volt);
  RightDriveSmart.spin(directionType::rev,6.0,voltageUnits::volt);
  task::sleep(100);
  LeftDriveSmart.spin(directionType::rev,8.0,voltageUnits::volt);
  RightDriveSmart.spin(directionType::rev,8.0,voltageUnits::volt);
  task::sleep(100);
  LeftDriveSmart.spin(directionType::rev,10.0,voltageUnits::volt);
  RightDriveSmart.spin(directionType::rev,10.0,voltageUnits::volt);
  task::sleep(100);
  drive2obs(directionType::rev);

  rearHook.set(true);
  driveIN(5,directionType::fwd,12.0);
  InertialRight(115);
  ringLift.spin(fwd,12.0,voltageUnits::volt);
  driveIN(50,directionType::fwd,7.0);
  driveIN(60,directionType::rev,12.0);
}

// Left side
void LANWP(){
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

void LALWR(){
  // this routine is still very much a work in progress, so don't be supprised if it doesn't perform as expected
  // start on left, speed for left yellow, back up to alliance on platform, grab, load with match load rings
  driveIN(48,directionType::fwd,12.0);
  frontHook.set(true);
  frontMogo.startSpinTo(800,rotationUnits::deg);
  driveIN(34,directionType::rev,12.0);
  InertialRight(10);
  driveIN(15,directionType::rev,6.0);
  InertialLeft(120);
  drive2obs(directionType::fwd);
  driveIN(26,directionType::rev,6.0);
  task::sleep(500);
  rearHook.set(true);
  driveIN(20,directionType::fwd,7.0);
  InertialLeft(90);
  driveIN(24,directionType::rev,12.0);
  driveIN(24,directionType::fwd,7.0);
}

void LACRF(){
  // start on left, fake for left yellow, go for center, bring it back, grab alliance with rear lift, load with match loads
  InertialRight(25);

  ringLift.spin(directionType::fwd,12.0,voltageUnits::volt);
  frontMogo.startSpinTo(200, rotationUnits::deg, 200, velocityUnits::pct);
  driveIN(30,directionType::fwd,12.0);
  driveIN(30,directionType::fwd,3.0);
  frontMogo.startSpinTo(0, rotationUnits::deg, 200, velocityUnits::pct);
  driveIN(15,directionType::fwd,6.0);
  frontHook.set(true);
  frontMogo.startSpinTo(300, rotationUnits::deg, 200, velocityUnits::pct);

  driveIN(70,directionType::rev,12.0);
  InertialLeft(100);

  drive2obs(directionType::rev);
  rearHook.set(true);
}

// define user control code here
void pneumaticSwitchFront() {
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

void usercontrol() {
  // Bind button x to front hook
  Controller1.ButtonX.pressed(pneumaticSwitchFront);
  while(true) {
      // Spin drivetrain motors
      LeftDriveSmart.spin(vex::directionType::undefined, (2*abs(Controller1.Axis3.position()+Controller1.Axis4.position()) >= DEADBAND) ? 0-(Controller1.Axis3.position()+Controller1.Axis4.position()) : 0, velocityUnits::pct);
      RightDriveSmart.spin(vex::directionType::undefined, (2*abs(Controller1.Axis3.position()-Controller1.Axis4.position()) >= DEADBAND) ? 0-(Controller1.Axis3.position()-Controller1.Axis4.position()) : 0, velocityUnits::pct);
      // Test code, don't uncomment unless you know what you're doing
      //RightDriveSmart.spin(directionType::undefined,Controller1.Axis3.position(),velocityUnits::pct);
      //LeftDriveSmart.spin(directionType::undefined,Controller1.Axis3.position(),velocityUnits::pct);
      
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
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // BIG FONT so everyone can read my debug status
  Brain.Screen.setFont(fontType::mono60);

  // Reset important encoders, open the front claw, and close the back claw
  frontHook.set(false);
  rearHook.set(true);
  frontMogo.resetPosition();

  // Calibrate Inertial and wait until complete
  inertialSensor.startCalibration();
  waitUntil(!inertialSensor.isCalibrating());

  // Super Fancy Auton Selector (C) Anthony Halliday, 2022, Under the license "pls dont steal my code, thx"
  // jk lol, its just completley broken, welcome to vex!
  
  // Set up driver control period.
  Competition.drivercontrol(usercontrol);

  // Prevent main from exiting with an infinite loop.
  while(true) {
    if (LeftDriveSmart.temperature(temperatureUnits::celsius)>50 || RightDriveSmart.temperature(temperatureUnits::celsius)>50){
      Controller1.rumble(".. .. .. ");
    }
  }
}

