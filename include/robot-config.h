using namespace vex;

extern brain Brain;

// VEXcode devices
extern digital_out frontHook;
extern inertial inertialSensor;
extern motor frontMogo;
extern motor ringLift;
extern motor_group LeftDriveSmart;
extern motor_group RightDriveSmart;
extern controller Controller1;
extern digital_out rearHook;

// Remove these once their refrences are gone from the code
extern motor rearMogo;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );