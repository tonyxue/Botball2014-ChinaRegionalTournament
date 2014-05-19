/*
The code is released under GUN LGPL v3 license, please see "LICENSE" for details.
*/
#define liftingMotorPort 2
#define catchingServoPort 3
#define etSensorPort 1
#define lightSensorPortNum 0
static void cameraInit(int resLv)
{
	if (resLv==1) camera_open(LOW_RES);// Start the camera and set the resolution to LOW
	if (resLv==2) camera_open(MED_RES);// Start the camera and set the resolution to MEDIUM
	if (resLv==3) camera_open(HIGH_RES);// Start the camera and set the resolution to HIGH
	camera_load_config("botball.conf");
	camera_update();// Get the latest data from camera | Initialization of data
	printf("Camera OK.\n");
}

static void servoInit()
{
	//int liftingServoPort=2, rotationMotorPort=1, catchingServoPort=3, rotationMotorVelocity=50;// add the port number for these motors and servos here
	enable_servo(catchingServoPort);
	//enable_servo(liftingServoPort);
	set_servo_position(catchingServoPort,1024);
	//set_servo_position(liftingServoPort,900);
}
