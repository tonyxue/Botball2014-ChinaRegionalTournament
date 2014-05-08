/*
Author: Tony Xue
E-mail: xuezhaoxin@rdfz.cn
No copyright, No license.
The author has no guarantee and warrantee on anything here.
*/
static void depthInit()// Initialization of the depth sensor
{
	depth_open();
	depth_update();
	printf("Depth sensor OK.\n");
}
static void cameraInit(int resLv)
{
	if (resLv==1) camera_open(LOW_RES);// Start the camera and set the resolution to LOW
	if (resLv==2) camera_open(MED_RES);// Start the camera and set the resolution to MEDIUM
	if (resLv==3) camera_open(HIGH_RES);// Start the camera and set the resolution to HIGH
	camera_update();// Fetech the latest data from camera | Initialization of data
	printf("Camera OK.\n");
}
