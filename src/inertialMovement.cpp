//it's all broken, wooo!
#include "vex.h"
#include "cmath"
// define vars and macros here
int targetHeading;
int calcVelocity;
int maxVelocity = 40;
int minVelocity = 10;
directionType lDir;
directionType rDir;

void inertialTurn(int targetAngle){
  inertialSensor.resetHeading();
  
}

/*
void inertialRight(float targetTurn){
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("Calculating...");   
    task::sleep(1000);

    prevTurn = inertialSensor.rotation(degrees);
    
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("Current position %i",prevTurn);   
    task::sleep(1000);

    targetAngle = targetTurn + prevTurn;
    //targetAngle = targetAngle>360 ? targetAngle-360 : targetAngle;

    Controller1.Screen.clearScreen();
    Controller1.Screen.print("Turning to %i",targetAngle);   
    task::sleep(1000);

    Controller1.Screen.clearScreen();
    Controller1.Screen.print("Beginning Turn");   
    task::sleep(1000);
    
    while(inertialSensor.rotation(degrees) < targetAngle){

    Controller1.Screen.clearScreen();

    calcVelocity = std::abs(targetAngle - inertialSensor.rotation(degrees));

    if(calcVelocity>maxVelocity)
    {    
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("Too Fast, slowing to %i",maxVelocity);

    RightDriveSmart.spin(directionType::rev,maxVelocity,velocityUnits::pct); 
    LeftDriveSmart.spin(directionType::fwd,maxVelocity,velocityUnits::pct); 
    }
    else if(calcVelocity<minVelocity)
    {
    RightDriveSmart.spin(directionType::rev,minVelocity,velocityUnits::pct); 
    LeftDriveSmart.spin(directionType::fwd,minVelocity,velocityUnits::pct); 
    Controller1.Screen.print("Too Slow, increasing to %i",minVelocity);
    }
    else
    {
    RightDriveSmart.spin(directionType::rev,calcVelocity,velocityUnits::pct); 
    LeftDriveSmart.spin(directionType::fwd,calcVelocity,velocityUnits::pct); 
    Controller1.Screen.print("Calculated speed: %i",calcVelocity);
    }
  }
    LeftDriveSmart.stop();
    RightDriveSmart.stop();
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("Turning Complete");
    task::sleep(1000);
 }
*/
/*

void InertialLeft(float targetTurn)    
{   
    prevTurn = InertialSensor.rotation(degrees);

    task::sleep(100);

    targetAngle = targetTurn*-1 + prevTurn;
    
    while(InertialSensor.rotation(degrees) > targetAngle)
    {

    calcVelocity = std::abs(targetAngle - InertialSensor.rotation(degrees));

    if(calcVelocity>maxVelocity)
    {    
    RRear.spin(directionType::fwd,maxVelocity,velocityUnits::pct); 
    LRear.spin(directionType::rev,maxVelocity,velocityUnits::pct); 
    RFront.spin(directionType::fwd,maxVelocity,velocityUnits::pct); 
    LFront.spin(directionType::rev,maxVelocity,velocityUnits::pct); 
    }
    else if(calcVelocity<minVelocity)
    {
    RRear.spin(directionType::fwd,minVelocity,velocityUnits::pct); 
    LRear.spin(directionType::rev,minVelocity,velocityUnits::pct); 
    RFront.spin(directionType::fwd,minVelocity,velocityUnits::pct); 
    LFront.spin(directionType::rev,minVelocity,velocityUnits::pct); 
    }
    else
    {
    RRear.spin(directionType::fwd,calcVelocity,velocityUnits::pct); 
    LRear.spin(directionType::rev,calcVelocity,velocityUnits::pct); 
    RFront.spin(directionType::fwd,calcVelocity,velocityUnits::pct); 
    LFront.spin(directionType::rev,calcVelocity,velocityUnits::pct); 
    }
  }

    task::sleep (100);
 }


 void moveIN(int targetDistanceIN, int numVelocity)
{
    int targetEncoderCount = 0;//sets count to zero *reset*
    float wheelCircumferenceIN = 12.9;//set wheel circuference float variable allows for decimal percision
        
    targetEncoderCount = (targetDistanceIN / wheelCircumferenceIN)*360;//target calculation
    //motor commands using encoder count calculations and velocity integers
    LFront.rotateFor(targetEncoderCount,rotationUnits::deg,numVelocity,velocityUnits::pct,false); 
    LRear.rotateFor(targetEncoderCount,rotationUnits::deg,numVelocity,velocityUnits::pct,false);
    RFront.rotateFor(targetEncoderCount,rotationUnits::deg,numVelocity,velocityUnits::pct,false); 
    RRear.rotateFor(targetEncoderCount,rotationUnits::deg,numVelocity,velocityUnits::pct,true);
}*/