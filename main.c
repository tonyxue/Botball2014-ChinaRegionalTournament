#include <stdio.h>
int centerX=80;
int centerY=60;
int camera()
{
	int channel=0,sizeOrder=0;
	int x,y,objectFound,diffX,diffY;
	camera_open(LOW_RES);// Start the camera and set the resolution to LOW
	camera_update();//Fetech the latest data from camera
	printf("Camera OK.\n");
	for(;;)
	{
		if (get_object_count(channel) == 0) printf ("No colored object found!\n");
		printf("Found object!\n");
		x=get_object_center(channel,sizeOrder).x; //get the x axis value of the largest object
		y=get_object_center(channel,sizeOrder).y; //get the y axis value of the largest object
		diffX=x-centerX;
		diffY=y-centerY;
		printf("the differences on the x-axis is %d and on the y-axis is %d.\n",diffX,diffY);
		msleep(5000);
		camera_update();
	}
	camera_close();
	return 1;//return 1 when success
}

int main()
{
	if(camera()) printf("Done!\n");
	else printf("Error!\n");
	return 0;
}
