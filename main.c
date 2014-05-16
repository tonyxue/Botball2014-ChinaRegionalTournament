/*
LICENSE: GUN LGPL v3 (please see "LICENSE" for details)
*/
#include <stdio.h>
#include "initializations.c"

int centerX=80,centerY=60;// define the x and y axis value of the center of the screen
int offsetX,offsetY,diffX,diffY;//offset value for the servo
int giveUpValue=20; // after this many times of trying to get the reading, if no result, the robot will give up
int servoOffsetStatus;
int yellowChannel=1,orangeChannel=0,yellowSize=0,orangeSize=0,orangeStripChannel,yellowStripChannel;
int liftingServoPort=2, rotationMotorPort=1, catchingServoPort=3, rotationMotorVelocity=50;// add the port number for these motors and servos here
int putUpHangerOffset=10;
int distanceAcrossTheBoard=1450;
int resLv=1;

void lightDetection() // Light detection
{
	int lightSensorPortNum=0,reading=1024,lightCriticalValue=512; //define Light Sensor port number, critical value and initialize the reading
	while (reading>lightCriticalValue)
	{
		msleep(50); // prevent too frequent operation
		reading=analog10(lightSensorPortNum);
		printf("The reading from light sensor is %d\n",reading );
	}
}

int blackLine() // Black line detection
{
	int blackLineSensorPort=1,blackLineCriticalValue=512; //define the port for black line sensor and the critical value
	if(analog10(blackLineSensorPort)<blackLineCriticalValue) return 1;// Return TRUE when black color is detected
	if(analog10(blackLineSensorPort)>blackLineCriticalValue) return 0;// Return FALSE when white color is detected
}

int xyDiff(int channel,int size)
{
	int x,y,i=0;
	while (get_object_count(channel) == 0)
	{
		i++;// use this variable to count the times that the sensor detects no object, for debug purpose
		printf ("No colored object found! This is the %d time!\n",i);
		if (i==giveUpValue) printf("%d times reached, giving up!", giveUpValue);servoOffsetStatus=-1;return -1; // If nothing is detected after several times, the robot will give up the task
	}
	printf("Found object!\n");
	x=get_object_center(channel,size).x; // get the x axis value of the largest object
	y=get_object_center(channel,size).y; // get the y axis value of the largest object
	diffX=x-centerX;// calculate the differences on x axis
	diffY=y-centerY;// calculate the differences on y axis
	return 0;
}

void servoOffset(int channel,int size)
{
	xyDiff(channel,size);
	offsetX=5*(diffX);// This algorithm is provided by the sample code, might be wrong, IDK
	offsetY=5*(diffY);
}

void turnLeftDegrees(int degrees)
{
	set_create_normalized_angle(0);
	while (get_create_normalized_angle()<degrees)
	{
		create_spin_CCW(100);// turn counter clock wise(CCW)
		msleep(10); //one period
		create_stop();
	}
}
void turnRightDegrees(int degrees)
{
	set_create_normalized_angle(0);
	while (get_create_normalized_angle()<degrees)
	{
		create_spin_CW(100);// turn clock wise(CW)
		msleep(10); //one period
		create_stop();
	}
}
void positionOffset(int channel, int size) // cancel the offset of the distance
{
	int x,y,distance,createVelocity,createRunningTime;
	depth_update();
	xyDiff(channel,size);// calculate the differences on x and y axis
	while (diffX!=0)
	{
		if(diffX>0)
		{
			//turn 90 degrees to LEFT
			turnLeftDegrees(90);
			create_drive_direct(createVelocity,createVelocity);
			msleep(500); // cancel the horizontal offset
			create_stop();
			turnRightDegrees(90);//turn back
		}
		if(diffX<0)
		{
			//turn 90 degrees to RIGHT
			turnRightDegrees(90);
			create_drive_direct(createVelocity,createVelocity);
			msleep(500); // cancel the horizontal offset
			create_stop();
			turnLeftDegrees(90);//turn back
		}
	}
	x=get_object_center(channel,size).x;
	y=get_object_center(channel,size).y;
	distance=get_depth_value(x,y);
	//the speed is (plus or minus) 20-500mm/sec
	create_drive_direct(createVelocity,createVelocity);
	createRunningTime=distance/createVelocity;
	msleep(createRunningTime);
	create_stop();
}

void run()
{
	mrp(rotationMotorPort,rotationMotorVelocity,offsetX);
	set_servo_position(liftingServoPort,offsetY);
}

void getCubes(int colorChoice)
{
	servoOffsetStatus=0;
	if (colorChoice==1) // for getting the yellow cubes
	{
		positionOffset(yellowChannel,yellowSize);
		servoOffset(yellowChannel,yellowSize); //calculate the offset
		if(servoOffsetStatus==0) run(); // run the motors when successfully identify the cube
	}
	if (colorChoice==0) // for getting the orange cubes
	{
		positionOffset(yellowChannel,yellowSize);
		servoOffset(orangeChannel,orangeSize); // calculate the offset
		if(servoOffsetStatus==0) run(); // run the motors when successfully identify the cube
	}
}
void deliverTheCube()
{
	//move backward for 100 millisecond with the speed of 100 millimeter per second
	create_drive_direct(-100,-100);
	msleep(100);
	//turn 90 degrees to left
	turnLeftDegrees(90);
	//go straightforward, full velocity
	create_drive_direct(500,500);
	msleep(distanceAcrossTheBoard/500);
	create_stop();
	//turn 90 degrees to left
	turnLeftDegrees(90);
	//go straightforward to get close enough
	create_drive_direct(100,100); // go straightforward for 1 second with the speed of 100 millimeters per second
	msleep(1000);
	create_stop();
}
void backToTheShelf()
{
	//the reverse of deliverTheCube()
	//move backward for 100 millisecond with the speed of 100 millimeter per second
	create_drive_direct(-100,-100);
	msleep(100);
	//turn 90 degrees to left
	turnLeftDegrees(90);
	//go straightforward, full velocity
	create_drive_direct(500,500);
	msleep(distanceAcrossTheBoard/500);
	//turn 90 degrees to left
	turnLeftDegrees(90);
	//go straightforward to get close enough
	create_drive_direct(100,100); // go straightforward for 1 second with the speed of 100 millimeters per second
	msleep(1000);
	create_stop();
}
void hangerStandToShelf()
{
	create_drive_straight(500);
	msleep(200);
	create_stop();
	turnRightDegrees(90);
	create_drive_straight(500);
	msleep(1000);
	create_stop();
	turnLeftDegrees(90);
}
void goToHangerStand()
{
	create_drive_straight(500);
	msleep(500);
	create_stop();
	turnLeftDegrees(90);
	create_drive_straight(500);
	msleep(800);
	create_stop();
	turnRightDegrees(90);
	create_drive_straight(500);
	msleep(1000);
	create_stop();
}
void putHangers() //put the hangers on the PVC
{
	set_servo_position(liftingServoPort,putUpHangerOffset);
}

void placeCubes()
{
	int yMax,yCenter,diffY,offset;
	servoOffset(orangeStripChannel,0);//when close enough to the orange strip, calculate the offset
	run();
	yMax=get_object_bbox(orangeChannel,0).uly;
	yCenter=get_object_center(orangeChannel,0).y;
	diffY=yMax-yCenter;
	offset=5*diffY;
	set_servo_position(liftingServoPort,offset);// cancel the offset on y axis
	msleep(1000);// wait for the servo to reach the position
	set_servo_position(catchingServoPort,2047);// release the cube
}

void main()
{
	printf("Start!\n");
	depthInit();
	printf("Depth initialized!\n");
	cameraInit(resLv);
	printf("Camera initialized!\n");
	servoInit(); // Supply power to all the servos
	printf("Servo initialized!\n");
	create_connect();
	printf("Create connected!\n Battery: %d\n",get_create_battery_charge());
	lightDetection();// wait for the startup light
	printf("Go!\n");
	//
	//functions for the hangers
	goToHangerStand();
	putHangers();
	hangerStandToShelf();
	getCubes(0);//get the first orange cube
	if(servoOffsetStatus==-1) printf("get the orange cube failed!\n");
	else deliverTheCube();
	backToTheShelf();
	getCubes(0);//get the second orange cube
	if(servoOffsetStatus==-1) printf("get the orange cube failed!\n");
	else deliverTheCube();
	backToTheShelf();
	getCubes(1);//get the first yellow cubes
	if(servoOffsetStatus==-1) printf("get the yellow cube failed!\n");
	else deliverTheCube();
	backToTheShelf();
	getCubes(1);//get the second yellow cubes
	if(servoOffsetStatus==-1) printf("get the yellow cube failed!\n");
	else deliverTheCube();

	//zhua cube*
	//song cube*
	
	printf("Done!\n");
}
