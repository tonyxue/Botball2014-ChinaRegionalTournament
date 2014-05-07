/*
Author: Tony Xue
E-mail: xuezhaoxin@rdfz.cn
No copyright, No license
*/
#include <stdio.h>
int centerX=80;// define the value of x axis of the center of the screen
int centerY=60;// define the value of y axis of the center of the screen

void depthInit()// Initialization of the depth sensor
{
	depth_open();
	depth_update();
	printf("Depth sensor OK.\n");
}
void cameraInit(int resLv)
{
	if (resLv==1) camera_open(LOW_RES);// Start the camera and set the resolution to LOW
	if (resLv==2) camera_open(MED_RES);// Start the camera and set the resolution to MEDIUM
	if (resLv==3) camera_open(HIGH_RES);// Start the camera and set the resolution to HIGH
	camera_update();// Fetech the latest data from camera | Initialization of data
	printf("Camera OK.\n");
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
