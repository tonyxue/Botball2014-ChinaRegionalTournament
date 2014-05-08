static int lightDetection()
{
	if(analog10(lightSensorPort)<lightIntensityCritialValue) run=1; //Change the run indicator to TRUE when light intensity goes below the critical value
	if(analog10(lightSensorPort)<lightIntensityCriticalValue) stop=1;run=0; //Change the stop indicator to TRUE and run indicator to FALSE when light intensity goes above the critical value
}
