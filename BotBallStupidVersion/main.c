#include <stdio.h>

#define liftingMotorPort 0
#define catchingServoPort 3
#define lightSensorPortNum 0
#define cubeButtonPort 15
#define sensorLiftingServo 2
#define blackLineSensorPort 1

const int turningSpeed=80, blackLineCriticalValue=900;
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
void servoInit()
{
	enable_servo(catchingServoPort);
	enable_servo(sensorLiftingServo);
	set_servo_position(catchingServoPort,1024);
	set_servo_position(sensorLiftingServo,100);// checked
	msleep(600);
	printf("Servo initialized!\n");
}
void motorInit()
{
	motor(liftingMotorPort,100);
	msleep(7000);//2700
	//motor(liftingMotorPort,-100);
	//msleep(400);
	off(liftingMotorPort);
	printf("Motor initialized!\n");
}
int blackLine()
{
	int reading;
	reading=analog10(blackLineSensorPort);
	while (1)
	{
		if(analog10(blackLineSensorPort)>blackLineCriticalValue) return 1;// Return TRUE when black color is detected
		if(analog10(blackLineSensorPort)<blackLineCriticalValue) return 0;// Return FALSE when white color is detected
	}
}
void turnLeftDegrees(int degrees) //checked
{
	int angle;
	set_create_normalized_angle(0);
	angle=get_create_normalized_angle();
	create_spin_CCW(turningSpeed);// turn counter clock wise(CCW)
	while (angle<degrees)
	{
		angle=get_create_normalized_angle();
	}
}
void turnRightDegrees(int degrees) //checked
{
	int angle;
	set_create_normalized_angle(0);
	angle=get_create_normalized_angle();
	create_spin_CW(turningSpeed);// turn clock wise(CW)
	while (angle>degrees*-1)
	{
		angle=get_create_normalized_angle();
	}
}
void goAlongLine(double time)
{
	int left,right,speed=200;
	double sTime,cTime=0;
	sTime=seconds();
	create_drive_straight(speed);
	while(cTime-sTime<=time)
	{
		left=get_create_lfcliff_amt();
		right=get_create_rfcliff_amt();
		if (left<800)
		{
			turnLeftDegrees(2);
			cTime=cTime-500;
			create_drive_straight(speed);
		}
		if (right<800)
		{
			turnRightDegrees(2);
			cTime=cTime-500;
			create_drive_straight(speed);
		}
		cTime=seconds();
	}
}
void goToHangerStand() //checked
{
	create_drive_straight(200);
	while(!blackLine()){}
	//create_drive_straight(-100);
	//msleep(50);
	turnLeftDegrees(84);
	goAlongLine(2.9);
	turnRightDegrees(80);
	create_drive_straight(-100);
	msleep(800);
	create_stop();
}
void putHangers() //checked
{
	//put the hangers on to the PVC
	motor(liftingMotorPort,-100);
	msleep(850);
	off(liftingMotorPort);
	create_drive_straight(200);
	msleep(700);
	create_stop();
	set_servo_position(catchingServoPort,600);
	msleep(500);
	motor(liftingMotorPort,-100);
	msleep(250);
	off(liftingMotorPort);
	//motor(liftingMotorPort,100);
	//msleep(2000);
}
void orangeCube()
{
	int flag=0;
	double cTime=0,sTime;
	create_drive_straight(200);// go straight till the button sensor is triggered
	msleep(500);
	create_stop();
	set_servo_position(sensorLiftingServo,1100);// extend the sensor to its working position
	create_drive_straight(200);
	sTime=seconds();
	while(!digital(cubeButtonPort))
	{
		if (flag==2) break;
		cTime=seconds();
		if (cTime-sTime>3.0) // adjust the position and repeat searching when timeout reached
		{
			create_drive_straight(-200);
			msleep(3000);
			create_stop();
			set_servo_position(sensorLiftingServo,50);
			msleep(500);
			turnLeftDegrees(80);
			create_drive_straight(200);
			msleep(80);
			turnRightDegrees(80);
			create_drive_straight(200);
			if(flag==0) set_servo_position(sensorLiftingServo,1110);// extend the sensor to its working position
			if(flag==1) set_servo_position(sensorLiftingServo,1130);// extend the sensor to its working position
			msleep(300);
			flag++;
			sTime=seconds();
		}
	}
	
	if (flag!=2)// if the attempt(s) didn't fail
	{
		create_drive_straight(-200);
		msleep(230);
		create_stop();

		turnLeftDegrees(75); // turn toward the shelf
		create_drive_straight(-200);
		msleep(250);
		create_stop();

		set_servo_position(catchingServoPort,300); // open the "hand"
		msleep(500);

		motor(liftingMotorPort,-100); // put the hand down
		msleep(2000);
		off(liftingMotorPort);
		msleep(500);

		set_servo_position(catchingServoPort,1400); // close the "hand"
		motor(liftingMotorPort,100);// lift the hand
		msleep(1500);
		off(liftingMotorPort);
		// got the cube

		turnLeftDegrees(75);
		goAlongLine(2);

		create_drive_straight(500);
		while(!get_create_rbump() && !get_create_lbump()){} // reach the other side of the board
		create_drive_straight(-100);
		msleep(200);

		turnLeftDegrees(77); // turn toward the container
		create_drive_straight(100);
		while(!get_create_lbump() && !get_create_rbump()){}
		create_drive_straight(-100);
		msleep(1500);
		create_stop();
		motor(liftingMotorPort,-100); // put the hand down
		msleep(1000);
		off(liftingMotorPort);
		set_servo_position(catchingServoPort,300); // open the "hand"
		msleep(500);
		motor(liftingMotorPort,100);
		msleep(5000);
		off(liftingMotorPort);
		set_servo_position(catchingServoPort,1400); // close the "hand" again
		msleep(500);
	}
}
void yellowCube()
{
	int flag=0;
	double cTime=0,sTime;
	create_drive_straight(200);
	msleep(1000);
	create_stop();
	set_servo_position(sensorLiftingServo,1450);// extend the button sensor again
	msleep(1000);
	create_drive_straight(200);
	sTime=seconds();
	while(!digital(cubeButtonPort))
	{
		if (flag==3) break;
		cTime=seconds();
		if (cTime-sTime>2.7)
		{
			create_drive_straight(-200);
			msleep(3200);
			turnLeftDegrees(80);
			create_drive_straight(100);
			msleep(500);
			turnRightDegrees(80);
			create_drive_straight(200);
			flag++;
			sTime=seconds();
		}
	}
	if (flag!=4)
	{
		create_stop();
		motor(liftingMotorPort,100);// lift the hand a bit
		msleep(3000);
		turnLeftDegrees(80);
		create_drive_straight(-200);
		msleep(500);
		create_stop();
		set_servo_position(catchingServoPort,300); // open the "hand"
		msleep(1000);
		motor(liftingMotorPort,-100);// put the hand down
		msleep(2000);
		off(liftingMotorPort);
		msleep(500);
		set_servo_position(catchingServoPort,1400); // close the "hand"
		create_drive_straight(-200);
		msleep(500);
		create_stop();
		motor(liftingMotorPort,100);// lift the hand a bit
		msleep(3000);
		off(liftingMotorPort);
	
		create_drive_straight(200);
		while(!blackLine()){}
		turnLeftDegrees(75);
		goAlongLine(2);

		create_drive_straight(500);
		while(!get_create_rbump() && !get_create_lbump()){}
		turnLeftDegrees(77);
		create_drive_straight(200);
		while(!get_create_rbump() && !get_create_lbump()){}
		create_drive_straight(-200);
		msleep(1300);
		create_stop();
		motor(liftingMotorPort,-100);
		msleep(1000);
		off(liftingMotorPort);
		set_servo_position(catchingServoPort,300); // open the "hand"
		msleep(500);
		set_servo_position(catchingServoPort,1400); // close the "hand" again
		msleep(500);
	}
}
void yellowCubeV2()
{
	int flag=0;
	double cTime=0,sTime;
	create_drive_straight(200);
	msleep(1000);
	create_stop();
	set_servo_position(sensorLiftingServo,1450);// extend the button sensor again
	msleep(1000);
	create_drive_straight(200);
		while(!digital(cubeButtonPort))
	{
		if (flag==3) break;
		cTime=seconds();
		if (cTime-sTime>2.7)
		{
			create_drive_straight(-200);
			msleep(3200);
			turnLeftDegrees(80);
			create_drive_straight(100);
			msleep(500);
			turnRightDegrees(80);
			create_drive_straight(200);
			flag++;
			sTime=seconds();
		}
	}
		if (flag!=4)
	{
		create_drive_straight(500);
		msleep(6000);
		create_stop();
	}
}
void main()
{
	printf("Start!\n");
	lightDetection();
	shut_down_in(120);

	servoInit(); // Supply power to all the servos
	motorInit(); // set the motor position
	create_connect();
	create_full();// FULL mode, the create will execute commands under any circumstance
	printf("Create connected!\n Battery: %d\n",get_create_battery_charge());
	
	msleep(2000);
	turnLeftDegrees(80);// adjust heading in startup area
	goToHangerStand();
	putHangers();
	
	create_drive_straight(-200);// drive away from hanger stand
	msleep(1000);
	create_stop();
	set_servo_position(catchingServoPort,1400); //close the hand
	motor(liftingMotorPort,100);// lift the hand to the highest position
	msleep(2000);
	off(liftingMotorPort);
	create_drive_straight(200);
	while(!blackLine()){}
	create_drive_straight(-200);
	msleep(300);
	turnRightDegrees(80);
	goAlongLine(2.6);
	turnLeftDegrees(80);
	create_drive_straight(200);
	while(!get_create_rbump() && !get_create_lbump()){}
	create_drive_straight(-200); // drive away from the PVC on the board
	msleep(900);
	turnRightDegrees(78);
	orangeCube();
	/********************************************************************/
	// go for yellow cube
	create_drive_straight(-200);
	msleep(500);
	turnLeftDegrees(78);
	create_drive_straight(500); //rush
	msleep(2700);
	turnLeftDegrees(76);
	create_drive_straight(200);
	while(!blackLine()){}
	create_drive_straight(-200);
	msleep(400);
	create_stop();
	set_servo_position(sensorLiftingServo,50);// retract the button sensor to avoid collision
	msleep(2000);
	turnRightDegrees(76);
	//create_stop();

	yellowCubeV2();

}
