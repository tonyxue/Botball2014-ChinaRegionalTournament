/*
Author: Tony Xue
E-mail: xuezhaoxin@rdfz.cn
The code is released under GUN LGPL v3 license, please see "LICENSE" for details.
*/
#include <stdio.h>
#include "globalConstants.h"
#include "initializations.c"
#include "lightDetection.c"
int run=0,stop=0;//indicator for whether the game has started and stopped

int blackLine()
{
	if(analog10(blackLineSensorPort)<blackLineCriticalValue) return 1;// Return TRUE when black color is detected
	if(analog10(blackLineSensorPort)>blackLineCriticalValue) return 0;// Return FALSE when white color is detected
}

int xyDiff(int channel,int size)
{
	int x,y,diffX,diffY;
	if (get_object_count(channel) == 0) printf ("No colored object found!\n");
	printf("Found object!\n");
	x=get_object_center(channel,size).x; // get the x axis value of the largest object
	y=get_object_center(channel,size).y; // get the y axis value of the largest object
	diffX=x-centerX;// calculate the differences on x axis
	diffY=y-centerY;// calculate the differences on y axis
	return diffX,diffY;
}
void main()
{
	int resLv=1,channel=0,size=0,stbInterval=2000;
	while(1) // main loop, loop forever
	{
		lightDetection();
		if(run)
		{
			//**********put the tasks that need to be done here**********
			cameraInit(resLv);
			xyDiff(channel,size);
			msleep(stbInterval);// Standby for a short period of time, avoiding too frequent updates
			camera_update();
			camera_close();
		}
		if(stop && !run) break;
	}
	printf("Done!\n");
}
