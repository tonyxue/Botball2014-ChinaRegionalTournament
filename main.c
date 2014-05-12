/*
Author: Tony Xue
E-mail: xuezhaoxin@rdfz.cn
LICENSE: GUN LGPL v3 (please see "LICENSE" for details)
*/
#include <stdio.h>
#include "initializations.c"

int centerX=80;// define the value of x axis of the center of the screen
int centerY=60;// define the value of y axis of the center of the screen
int diffX,diffY;
int giveUpValue=20; // after this many times of trying to get the reading, if no result,the robot will give up

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

void xyDiff(int channel,int size)
{
	int x,y,diffX,diffY,i=0;
	while (get_object_count(channel) == 0)
	{
		i++;// use this variable to count the times that the sensor detects no object, for debug purpose
		printf ("No colored object found! This is the &d time!\n",i);
		if (i==giveUpValue) printf("%d times reached, giving up!", giveUpValue);break; // If nothing is detected after serval times, the robot will give up the task
	}
	printf("Found object!\n");
	x=get_object_center(channel,size).x; // get the x axis value of the largest object
	y=get_object_center(channel,size).y; // get the y axis value of the largest object
	diffX=x-centerX;// calculate the differences on x axis
	diffY=y-centerY;// calculate the differences on y axis
}

int getCubes(char color,int yellowChannel=1, int orangeChannel=0)
{
	int yellowCount=get_object_count(yellowChannel),orangeCount=get_object_count(orangeChannel),yellowSize=0,orangeSize=0;
	int i=0,j=0;
	if (color=="yellow") // for yellow cubes
	{
		while(yellowCount=0 && i<=giveUpValue)
		{
			msleep(500); // wait 0.5 second to avoid too frequent detections
			i++;
			yellowCount=get_object_count(yellowChannel);
			if (i==giveUpValue) printf("No yellow object detected!\n");
		}
		
		xyDiff(yellowChannel,yellowSize);
		while (diffX!=0) //adjust the x axis
		{
			if (diffX > centerX)
			{
				// Turn LEFT
			}
			if (diffX < centerX)
			{
				// Turn RIGHT
			}
			xyDiff(yellowChannel,yellowSize);
		}
		while (diffY!=0) //adjust the y axis
		{
			if (diffY > centerY)
			{
				// Turn DOWN
			}
			if (diffY < centerY )
			{
				// Turn UP
			}
			xyDiff(yellowChannel,yellowSize);
		}
	}
	if (color=="orange")// for orange cubes
	{
		while(orangeCount=0 && j<=giveUpValue)
		{
			msleep(500); // wait 0.5 second to avoid too frequent detections
			j++;
			orangeCount=get_object_count(orangeChannel);
			if (i==giveUpValue) printf("No orange object detected!\n");
		}
		xyDiff(orangeChannel,orangeSize);
		while (diffX!=0) //adjust the x axis
		{
			// ###Use the projection method to calculate the offset!!!###
			if (diffX > centerX)
			{
				// Turn LEFT
			}
			if (diffX < centerX)
			{
				// Turn RIGHT
			}
			xyDiff(orangeChannel,orangeSize);
		}
		while (diffY!=0) //adjust the y axis
		{
			if (diffY > centerY)
			{
				// Turn DOWN
			}
			if (diffY < centerY )
			{
				// Turn UP
			}
			xyDiff(orangeChannel,orangeSize);
		}
	}


}

int main()
{
	int resLv=1,channel=0,size=0,stbyInterval=2000;
	lightDetection();// wait for the startup light
	//
	
	printf("Done!\n");
	
	return 0;
}
