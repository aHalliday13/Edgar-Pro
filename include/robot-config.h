using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern controller Controller1;
extern inertial inertialSensor;
extern digital_out frontHook;
extern motor frontMogo;
extern motor ringLift;
extern motor LeftDriveSmartA;
extern motor LeftDriveSmartB;
extern motor LeftDriveSmartC;
extern motor RightDriveSmartA;
extern motor RightDriveSmartB;
extern motor RightDriveSmartC;
extern digital_out rearHook;
extern digital_out autonHook;
extern signature frontVision__SIG_1;
extern signature frontVision__SIG_2;
extern signature frontVision__SIG_3;
extern signature frontVision__SIG_4;
extern signature frontVision__SIG_5;
extern signature frontVision__SIG_6;
extern signature frontVision__SIG_7;
extern vision frontVision;
extern signature rearVision__SIG_1;
extern signature rearVision__SIG_2;
extern signature rearVision__SIG_3;
extern signature rearVision__SIG_4;
extern signature rearVision__SIG_5;
extern signature rearVision__SIG_6;
extern signature rearVision__SIG_7;
extern vision rearVision;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );