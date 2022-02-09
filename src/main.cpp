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

// "It's always the programer's fault" - Some genius on the vex fourms

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
  // 3:7 gear ratio motor:wheel
  // 7 rotations of the motor is three rotations of the wheel
  // Wheel circumfrence 12.9590697
  // 7 rotations of the motor is 38.8772091
  // 1 rotation of the wheel is 5.5388701
  // need to ajust all routines to actual inches before changing value in code

  dist=dist/(5);
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
  ringLift.spinFor(1.5,timeUnits::sec);
  //Step 2: Pick up YeMogo
  InertialRight(92);
  frontHook.set(false);
  driveIN(48,directionType::fwd,70);
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
  ringLift.spinFor(1.5,timeUnits::sec);
  //Step 2: "Mad Dash" for middle Yemogo
  driveIN(6,directionType::fwd,70);
  InertialRight(90);
  driveIN(18,directionType::fwd,70);
  InertialRight(90);
  driveIN(27,directionType::fwd,70);
  InertialLeft(55);
  driveIN(35,directionType::fwd,40);
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
  driveIN(47,directionType::fwd,200);
  frontHook.set(true);
  task::sleep(500);
  frontMogo.spinFor(500,rotationUnits::deg);
  // drop lift, back up to aliance mogo, spin to grab it with rear lift
  rearMogo.spinTo(700, rotationUnits::deg);
  driveIN(20,directionType::rev,55);
  InertialLeft(40);
  driveIN(13,directionType::rev,57);
  rearMogo.spin(directionType::rev,70,velocityUnits::pct);
  waitUntil(rearMogoSwitch.value());
  rearMogo.stop(brakeType::hold);
  ringLift.spinFor(3,timeUnits::sec,100,velocityUnits::pct);
  InertialRight(35);
  driveIN(5,directionType::rev,100);
}

void rightAutonCenter(void) {
  // testing code just ignore: driveIN(70,directionType::rev,100);

  // right auton code goes here
  // open claw, turn left, drive forward to center yemogo, latch on and lift
  frontHook.set(false);
  driveIN(29,directionType::fwd,70);
  InertialLeft(45);
  driveIN(40,directionType::fwd,70);
  frontHook.set(true);
  frontMogo.spinFor(500,rotationUnits::deg);
  InertialLeft(5);
  // drop lift, back up to aliance mogo, load rings
  rearMogo.spinTo(700, rotationUnits::deg);
  driveIN(60,directionType::rev,50);
  rearMogo.spinTo(600, rotationUnits::deg);
  ringLift.spinFor(3,timeUnits::sec,100,velocityUnits::pct);
}

void soloWinPoint(void){

  frontMogo.spinFor(directionType::fwd,750,rotationUnits::deg,200,velocityUnits::pct);
  driveIN(7,directionType::rev,70);
  InertialLeft(90);
  driveIN(16,directionType::fwd,70);
  InertialLeft(87);
  rearMogo.spinTo(700,rotationUnits::deg);
  //driveIN(12,directionType::fwd,55);
  drive2obs(directionType::fwd);
  driveIN(105,directionType::rev,100);
  rearMogo.spinTo(550,rotationUnits::deg,200,velocityUnits::pct);
  ringLift.spinFor(3,timeUnits::sec,200,velocityUnits::pct);
  InertialRight(90);
}

void leftAutonNoWP(void){
  driveIN(3,directionType::fwd,70);
  InertialRight(3);
  driveIN(42,directionType::fwd,70);
  frontHook.set(true);
  frontMogo.spinTo(110,rotationUnits::deg);
  InertialLeft(95);
  rearMogo.spinTo(700,rotationUnits::deg,200, velocityUnits::pct);
  driveIN(27,directionType::rev,30);
  rearMogo.spinTo(600,rotationUnits::deg);
  InertialLeft(45);
  driveIN(30,directionType::fwd,70);
  InertialRight(180);
  driveIN(30,directionType::rev,70);
}

void rightAutonNoWP(void){

  // open claw, drive forward to neutral mogo, latch on and lift
  frontHook.set(false);
  driveIN(51,directionType::fwd,55);
  frontHook.set(true);
  task::sleep(500);
  frontMogo.spinFor(500,rotationUnits::deg);
  rearMogo.spinTo(700, rotationUnits::deg);
  InertialRight(82.5);
  driveIN(30,directionType::rev,30);
  rearMogo.spinTo(500, rotationUnits::deg);
  InertialRight(40);
  driveIN(70,directionType::fwd,200);
}

void skillsAuton(void) {
  Controller1.Screen.print("SKILL");
  driveIN(51,directionType::fwd,70);
  frontHook.set(true);
  driveIN(51,directionType::rev,100);
}

// now that autons are defined, we can define the auton selection code

void autonSelect(){
  if(Controller1.ButtonA.pressing()){
    Competition.autonomous(rightAutonRight);
    Controller1.Screen.print("RAR");
  }
  else if(Controller1.ButtonB.pressing()){
    Competition.autonomous(rightAutonCenter);
    Controller1.Screen.print("RAC");
  }
  else if(Controller1.ButtonX.pressing()){
    Competition.autonomous(leftAutonCenter);
    Controller1.Screen.print("LAC");
  }
  else if(Controller1.ButtonY.pressing()){
    Competition.autonomous(leftAutonLeft);
    Controller1.Screen.print("LAL");
  }
  else if(Controller1.ButtonLeft.pressing()){
    Competition.autonomous(leftAutonNoWP);
    Controller1.Screen.print("LANWP");
  }
  else if(Controller1.ButtonRight.pressing()){
    Competition.autonomous(rightAutonNoWP);
    Controller1.Screen.print("RANWP");
  }
  else if(Controller1.ButtonUp.pressing()){
    Competition.autonomous(soloWinPoint);
    Controller1.Screen.print("SAWP");
  }
  else if(Controller1.ButtonDown.pressing()){
    Competition.autonomous(skillsAuton);
    Controller1.Screen.print("SKILL");
  }
  else {
    autonSelect();
  }
}

// define pre-auton routine here
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Auton Selection
  autonSelect();
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
      LeftDriveSmart.spin(vex::directionType::undefined, (2*abs(Controller1.Axis3.position()-Controller1.Axis4.position()) >= DEADBAND) ? 0-(Controller1.Axis3.position()-Controller1.Axis4.position()) : 0, velocityUnits::pct);
      RightDriveSmart.spin(vex::directionType::undefined, (2*abs(Controller1.Axis3.position()+Controller1.Axis4.position()) >= DEADBAND) ? 0-(Controller1.Axis3.position()+Controller1.Axis4.position()) : 0, velocityUnits::pct);
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

void calibrate(){
  //emergency calibration code
  LeftDriveSmart.spinFor(1,rotationUnits::rev,false);
  RightDriveSmart.spinFor(1,rotationUnits::rev,false);
}

// main() called on program start
int main() {
  // run the pre-auton routine, this will set up auton routine
  pre_auton();
  //emergency calibration code, leave commented out unless you know what you're doing
  //Competition.autonomous(calibrate);
  // Set up callbacks for driver control period.
  Competition.drivercontrol(usercontrol);
  // Prevent main from exiting with an infinite loop.
  while(true) {
    task::sleep(100);
  }
}
