//LICENSE: GUN LGPL v3 (please see "LICENSE" for details)
#include <stdio.h>
#include "initializations.c"


#define liftingMotorPort 2
#define catchingServoPort 3
#define etSensorPort 1
#define lightSensorPortNum 0

const int centerX=80;// define the x and y axis value of the center of the screen
const int centerY=60;
const int giveUpValue=20; // after this many times of trying to get the reading, if no result, the robot will give up
int yellowChannel=1,orangeChannel=0,yellowSize=0,orangeSize=0,orangeStripChannel=1,yellowStripChannel=0;
const int putUpHangerOffset=850;
const int distanceAcrossTheBoard=1450,turningSpeed=80;
const int resLv=3;

int offsetX,offsetY,diffX,diffY;//offset value for the servo
int servoOffsetStatus;

void lightDetection() // Light detection, checked
{
	int reading=1024,lightCriticalValue=512; //define Light Sensor port number, critical value and initialize the reading
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
		camera_update();
		i++;// use this variable to count the times that the sensor detects no object, for debug purpose
		printf ("No colored object found! This is the %d time!\n",i);
		msleep(500);
		if (i==giveUpValue)
		{
			printf("%d times reached, giving up!\n", giveUpValue);
			servoOffsetStatus=-1;
			return -1; // If nothing is detected after several trials, the robot will give up the task
		}
	}
	printf("Found object!\n");
	x=get_object_center(channel,size).x; // get the x axis value of the largest object
	y=get_object_center(channel,size).y; // get the y axis value of the largest object
	diffX=x - centerX;// calculate the differences on x axis
	diffY=y - centerY;// calculate the differences on y axis
	return 0;
}
void turnLeftDegrees(int degrees) //checked
{
	int angle;
	set_create_total_angle(0);
	angle=get_create_total_angle();
	create_spin_CCW(turningSpeed);// turn counter clock wise(CCW)
	while (angle<degrees)
	{
		angle=get_create_normalized_angle();
	}
	create_stop();
}
void turnRightDegrees(int degrees) //checked
{
	int angle;
	set_create_total_angle(0);
	angle=get_create_total_angle();
	create_spin_CW(turningSpeed);// turn clock wise(CW)
	while (angle>degrees*-1)
	{
		angle=get_create_normalized_angle();
	}
	create_stop();
}
void positionOffset(int channel, int size) // cancel the offset of the distance
{
	int x,y,distance,createVelocity,createRunningTime;
	xyDiff(channel,size);// calculate the differences on x and y axis
	while (diffX!=0)
	{
		if(diffX>0)
		{
			//turn 90 degrees to LEFT
			turnLeftDegrees(90);
			create_drive_straight(createVelocity);
			msleep(500); // cancel the horizontal offset
			//create_stop();
			turnRightDegrees(90);//turn back
		}
		if(diffX<0)
		{
			//turn 90 degrees to RIGHT
			turnRightDegrees(90);
			create_drive_straight(createVelocity);
			msleep(500); // cancel the horizontal offset
			//create_stop();
			turnLeftDegrees(90);//turn back
		}
	}
	x=get_object_center(channel,size).x;
	y=get_object_center(channel,size).y;
	//distance=analog10(etSensorPort)*75/1024; //The effective distance is between 5-80 cm

	//the speed is (plus or minus) 20-500mm/sec
	create_drive_straight(createVelocity);
	msleep(distance/createVelocity);
	create_stop();
}
void deliverTheCube()
{
	//move backward for 100 millisecond with the speed of 100 millimeter per second
	create_drive_straight(-100);
	msleep(100);
	//turn 90 degrees to left
	turnLeftDegrees(90);
	//go straightforward, full velocity
	create_drive_straight(500);
	msleep(distanceAcrossTheBoard/500);
	create_stop();
	//turn 90 degrees to left
	turnLeftDegrees(90);
	//go straightforward to get close enough
	create_drive_straight(100); // go straightforward for 1 second with the speed of 100 millimeters per second
	msleep(1000);
	create_stop();
}
void backToTheShelf()
{
	//the reverse of deliverTheCube()
	//move backward for 100 millisecond with the speed of 100 millimeter per second
	create_drive_straight(-100);
	msleep(100);
	//turn 90 degrees to left
	turnLeftDegrees(90);
	//go straightforward, full velocity
	create_drive_straight(500);
	msleep(distanceAcrossTheBoard/500);
	//turn 90 degrees to left
	turnLeftDegrees(90);
	//go straightforward to get close enough
	create_drive_straight(100); // go straightforward for 1 second with the speed of 100 millimeters per second
	msleep(1000);
	create_stop();
}
void goToHangerStand() //checked
{
	create_drive_straight(200);
	msleep(2750);
	create_stop();
	turnLeftDegrees(90);
	create_drive_straight(200);
	msleep(2350);
	create_stop();
	turnRightDegrees(90);
	create_drive_straight(200);
	msleep(2000);
	create_stop();
}
void putHangers() //checked
{
	//put the hangers on to the PVC
	motor(liftingMotorPort,-500);
	msleep(800);
	off(liftingMotorPort);
	set_servo_position(catchingServoPort,putUpHangerOffset);
	msleep(500);
}
void hangerStandToShelf()
{
	create_drive_straight(-500);
	msleep(300);
	turnRightDegrees(90);
	create_drive_straight(500);
	msleep(1200);
	turnLeftDegrees(90);
	create_drive_straight(-100);
	msleep(1000); // stand far enough so the camera can see the whole platform
	create_stop();
}
/*void getCubes(int colorChoice)
{
	servoOffsetStatus=0;
	if (colorChoice==1) // for getting the yellow cubes
	{
		positionOffset(yellowChannel,yellowSize);
		servoOffset(yellowChannel,yellowSize); //calculate the offset
		if(servoOffsetStatus==0) cancelOffset(); // run the motors when successfully identify the cube
	}
	if (colorChoice==0) // for getting the orange cubes
	{
		positionOffset(yellowChannel,yellowSize);
		servoOffset(orangeChannel,orangeSize); // calculate the offset
		if(servoOffsetStatus==0) cancelOffset(); // run the motors when successfully identify the cube
	}
}*/
void getOrangeCubes()
{
	int x,y,distance,correctedDistance;
	camera_update();
	while (get_object_count(orangeChannel)==0)
		{
			turnRightDegrees(1);
			camera_update();
		}
	while (1)
	{
		x=get_object_center(orangeChannel,0).x;
		y=get_object_center(orangeChannel,0).y;
		if (get_object_count(orangeChannel)>0)
		{
			printf("X: %d , Y: %d\n",x,y );
			if (x-80>0)// on the right side of the center
			{
				while (x-80>0)
				{
					turnRightDegrees(1);
					camera_update();
					x=get_object_center(orangeChannel,0).x;
				}
				turnLeftDegrees(8);// correct error
				break;
			}
			if (x-80<0)
			{
				while (80-x>0)
				{
					turnLeftDegrees(1);
					camera_update();
					x=get_object_center(orangeChannel,0).x;
				}
				turnRightDegrees(8);// correct error
				break;
			}
		}
		msleep(200);
		camera_update();
	}
		/*set_servo_position(catchingServoPort,80); // open the "hand"
		msleep(500);
		distance=analog10(etSensorPort)*75/1024; //In centimeter. yThe effective distance is between 5-80 cm. Greatest value when closest.
		correctedDistance=(distance-4)*10;
		create_drive_straight(400);
		msleep(correctedDistance*1000/400);
		create_stop();*/
		//set_servo_position(catchingServoPort,1100); // close the "hand"
		//msleep(500);
}
void getYellowCubes()
{
	int x,y;
	camera_update();
	while (1)
	{
		x=get_object_center(yellowChannel,0).x;
		y=get_object_center(yellowChannel,0).y;
		if (get_object_count(yellowChannel)>0)
		{
			printf("X: %d , Y: %d\n",x,y );
			if (x-80>0)// on the right side of the center
			{
				while (x-80>0)
				{
					turnRightDegrees(1);
					camera_update();
					x=get_object_center(yellowChannel,0).x;
				}
				break;
			}
			if (x-80<0)
			{
				while (80-x>0)
				{
					turnLeftDegrees(1);
					camera_update();
					x=get_object_center(yellowChannel,0).x;
				}
				break;
			}
		}
		msleep(200);
		camera_update();
	}
		/*set_servo_position(catchingServoPort,80); // open the "hand"
		msleep(500);
		create_drive_straight(200);
		msleep(200);
		create_stop();
		set_servo_position(catchingServoPort,1100); // close the "hand"
		msleep(500);*/
}
/*void placeCubes()
{
	int yMax,yCenter,diffY,offset;
	servoOffset(orangeStripChannel,0);//when close enough to the orange strip, calculate the offset
	cancelOffset();
	yMax=get_object_bbox(orangeChannel,0).uly;
	yCenter=get_object_center(orangeChannel,0).y;
	diffY=yMax-yCenter;
	offset=5*diffY;
	//set_servo_position(liftingServoPort,offset);// cancel the offset on y axis
	motor(liftingMotorPort,-500);
	msleep(1000);// wait for the servo to reach the position
	set_servo_position(catchingServoPort,2047);// release the cube
}*/
void main()
{
	printf("Start!\n");
	set_analog_pullup(etSensorPort,0);// set the port type for the ET sensor
	cameraInit(resLv);
	printf("Camera initialized! %d channel(s) found!\n",get_channel_count());
	servoInit(); // Supply power to all the servos
	printf("Servo initialized!\n");
	create_connect();
	create_full();
	printf("Create connected!\n Battery: %d\n",get_create_battery_charge());
	//lightDetection();// wait for the startup light
	//printf("Go!\n");
	//turnLeftDegrees(90);
	//functions for the hangers
	//goToHangerStand();
	//putHangers();
	//hangerStandToShelf();

	getOrangeCubes();//get the first orange cube
	//create_drive_straight(-500);
	//msleep(1000);
	//create_stop();
	//if (get_object_count(yellowChannel)==-1) printf("No such channel!\n");
	//else
	//{
	//	printf("No object found!\n");
	//}
	/*
	if(servoOffsetStatus==-1) printf("get the orange cube failed!\n");
	else
	{
		deliverTheCube();
		backToTheShelf();
	}

	getCubes(0);//get the second orange cube
	if(servoOffsetStatus==-1) printf("get the orange cube failed!\n");
	else
	{ 
		deliverTheCube();
		backToTheShelf();
	}
	getCubes(1);//get the first yellow cubes
	if(servoOffsetStatus==-1) printf("get the yellow cube failed!\n");
	else
	{
		deliverTheCube();
		backToTheShelf();
	}
	getCubes(1);//get the second yellow cubes
	if(servoOffsetStatus==-1) printf("get the yellow cube failed!\n");
	else deliverTheCube();

	//get rival's cubes
	*/
	printf("Done!\n");
}
