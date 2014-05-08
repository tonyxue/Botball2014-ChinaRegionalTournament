/*
Author: Tony Xue
E-mail: xuezhaoxin@rdfz.cn
The code is released under GUN LGPL v3 license, please see "LICENSE" for details.
*/
#include <stdio.h>
#include "globalConstants.h"
#include "initFunc.c"

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
	cameraInit(resLv);
	while(1) // main loop, loop forever
	{
		xyDiff(channel,size);
		msleep(stbInterval);// Standby for a short period of time, avoiding too frequent updates
		camera_update();
	}
	camera_close();
	printf("Done!\n");
}
