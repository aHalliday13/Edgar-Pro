using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor frontMogo;
extern motor rearMogo;
extern digital_out frontHook;
extern inertial inertialSensor;
extern controller Controller1;
extern motor_group LeftDriveSmart;
extern motor_group RightDriveSmart;
extern motor ringLift;
extern limit rearMogoSwitch;
extern motor sideHook;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );