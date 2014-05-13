/*
Author: Tony Xue
E-mail: xuezhaoxin@rdfz.cn
LICENSE: GUN LGPL v3 (please see "LICENSE" for details)
*/
#include <stdio.h>
#include "initializations.c"

int centerX=80,centerY=60;// define the x and y axis value of the center of the screen
int offsetX,offsetY;//offset value for the servo
int giveUpValue=20; // after this many times of trying to get the reading, if no result,the robot will give up
int offsetStatus=0;
int yellowChannel=1,orangeChannel=0,yellowSize=0,orangeSize=0,orangeStripChannel,yellowStripChannel;
int liftingServoPort, rotationMotorPort, catchingServoPort, rotationMotorVelocity;// add the port number for these motors and servos here
int putUpHangerOffset;

void lightDetection() // Light detection
{
	int lightSensorPortNum=0,reading=1024,lightCriticalValue=512; //define Light Sensor port number, critical value and initialize the reading
	while (reading>lightCriticalValue)
	{
		reading=analog10(lightSensorPortNum);
	}
}

int blackLine() // Black line detection
{
	int blackLineSensorPort=1,blackLineCriticalValue=512; //define the port for black line sensor and the critical value
	if(analog10(blackLineSensorPort)<blackLineCriticalValue) return 1;// Return TRUE when black color is detected
	if(analog10(blackLineSensorPort)>blackLineCriticalValue) return 0;// Return FALSE when white color is detected
}

void offset(int channel,int size)
{
	int x,y,diffX,diffY,i=0;
	while (get_object_count(channel) == 0)
	{
		i++;// use this variable to count the times that the sensor detects no object, for debug purpose
		printf ("No colored object found! This is the &d time!\n",i);
		if (i==giveUpValue) printf("%d times reached, giving up!", giveUpValue);offsetStatus=-1;return -1; // If nothing is detected after serval times, the robot will give up the task
	}
	printf("Found object!\n");
	x=get_object_center(channel,size).x; // get the x axis value of the largest object
	y=get_object_center(channel,size).y; // get the y axis value of the largest object
	diffX=x-centerX;// calculate the differences on x axis
	diffY=y-centerY;// calculate the differences on y axis
	offsetX=5*(diffX);// The algorithm is provided by the sample code, might be wrong, IDK
	offsetY=5*(diffY);
}

void run()
{
	mrp(rotationMotorPort,rotationMotorVelocity,offsetX);
	set_servo_position(liftingServoPort,offsetY);
}

void getCubes(int colorChoice)
{
	if (colorChoice==1) // for getting the yellow cubes
	{
		offset(yellowChannel,yellowSize);
		//run the servo and motor here
		run();
	}
	if (colorChoice==0) // for getting the orange cubes
	{
		offset(orangeChannel,orangeSize);
		//run the servo and motor here
		run();
	}
//
}
void seekHangerStand()
{
	
}
void putHangers() //put the hangers on the PVC
{
	set_servo_position(liftingServoPort,putUpHangerOffset);
}

void placeCubes()
{
	int yMax,yCenter,diffY,offset;
	offset(orangeStripChannel,0);//when close enough to the orange strip, calculate the offset
	run();
	yMax=get_object_bbox(orangeChannel,0).uly;
	yCenter=get_object_center.y;
	diffY=yMax-yCenter;
	offset=5*diffY;
	set_servo_position(liftingServoPort,offset);// cancel the offset on y axis
	msleep(1000);// wait for the servo to reach the position
	set_servo_position(catchingServoPort,2047);// release the cube
}

void main()
{
	int resLv=1,channel=0,size=0,stbyInterval=2000;
	lightDetection();// wait for the startup light
	//
	//yijia function 
	//zhua cube
	//song cube
	//yuanlufanhui
	//zhua cube
	//song cube
	//yuanlufanhui
	//zhua cube
	//song cube
	
	
	getCubes(0);//get the orange cubes
	if (offsetStatus==-1) printf("get orange cubes failed!\n");
	getCubes(1);//get the yellow cubes
	if (offsetStatus==-1) printf("get yellow cubes failed!\n");
	
	printf("Done!\n");
	return 0;
}
