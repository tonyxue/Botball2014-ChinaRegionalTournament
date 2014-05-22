#include <stdio.h>

#define liftingMotorPort 0
#define catchingServoPort 3
#define lightSensorPortNum 0
#define cubeButtonPort 15
#define sensorLiftingServo 2
#define blackLineSensorPort 1

const int turningSpeed=80, putUpHangerOffset=750,blackLineCriticalValue=980;
void servoInit()
{
	enable_servo(catchingServoPort);
	enable_servo(sensorLiftingServo);
	set_servo_position(catchingServoPort,1024);
	set_servo_position(sensorLiftingServo,50);// checked
	msleep(1000);
	printf("Servo initialized!\n");
}
void motorInit()
{
	motor(liftingMotorPort,100);
	msleep(2800);
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
		//reading=analog10(blackLineSensorPort);
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
	//while (angle<degrees)
	{
		angle=get_create_normalized_angle();
	}
}
void goToHangerStand() //checked
{
	int reading;
	reading=analog10(blackLineSensorPort);
	create_drive_straight(200);
	while(reading<blackLineCriticalValue)
	{
		reading=analog10(blackLineSensorPort);
	}
	turnLeftDegrees(86);
	create_drive_straight(200);
	msleep(2340);
	turnRightDegrees(90);
	create_drive_straight(200);
	msleep(2000);
	create_stop();
}
void putHangers() //checked
{
	//put the hangers on to the PVC
	motor(liftingMotorPort,-100);
	msleep(800);
	off(liftingMotorPort);
	set_servo_position(catchingServoPort,putUpHangerOffset);
	msleep(500);
}
void orangeCube()
{
	int time=0,flag=0;
	set_servo_position(sensorLiftingServo,1128);// extend the sensor to its working position
	create_drive_straight(200);// go straight till the button sensor is triggered
	while(!digital(cubeButtonPort))
	{
		if (flag==2) break;
		msleep(1);
		time++;
		if (time==2700) // adjust the position and repeat searching when timeout reached
		{
			create_drive_straight(-200);
			msleep(3000);
			create_drive_straight(200);

			turnLeftDegrees(80);
			create_drive_straight(200);
			msleep(50);
			turnRightDegrees(80);
			create_drive_straight(200);
			flag++;
		}
	}
	
	if (flag!=2)// if the attempt(s) didn't fail
	{
		create_drive_straight(-200);
		msleep(400);
		create_stop();

		motor(liftingMotorPort,100); // lift the hand a bit
		msleep(150);
		off(liftingMotorPort);

		turnLeftDegrees(75); // turn toward the shelf
		create_drive_straight(-200);
		msleep(340);
		create_stop();

		set_servo_position(catchingServoPort,450); // open the "hand"
		msleep(1000);
		create_drive_straight(100);// reach the cube
		msleep(650);
		create_stop();

		motor(liftingMotorPort,-100); // put the hand down
		msleep(600);
		off(liftingMotorPort);
		msleep(500);

		set_servo_position(catchingServoPort,1400); // close the "hand"
		motor(liftingMotorPort,100);// lift the hand a bit
		msleep(300);
		off(liftingMotorPort);
		// got the cube

		create_drive_straight(-200);
		while(!blackLine()){}
		turnLeftDegrees(83);

		create_drive_straight(500);
		while(!get_create_rbump() && !get_create_lbump()){} // reach the other side of the board
		create_drive_straight(-100);
		msleep(200);

		turnLeftDegrees(77); // turn toward the container
		create_drive_straight(100);
		while(!get_create_rbump() && !get_create_lbump()){}
		create_drive_straight(-100);
		msleep(800);
		create_stop();
		motor(liftingMotorPort,-100); // put the hand down
		msleep(1000);
		off(liftingMotorPort);
		set_servo_position(catchingServoPort,300); // open the "hand"
	}
}
void yellowCube()
{
	int time=0,flag=0;
	create_drive_straight(200);
	msleep(200);
	while(!digital(cubeButtonPort))
	{
		msleep(1);
		time++;
		if (time==2700)
		{
			create_drive_straight(-200);
			msleep(3200);
			turnLeftDegrees(80);
			create_drive_straight(100);
			msleep(300);
			turnRightDegrees(80);
		}
		else
		{
			turnLeftDegrees(10);
			create_drive_straight(-200);
			msleep(300);
			motor(liftingMotorPort,100);// lift the hand a bit
			msleep(300);
			turnLeftDegrees(80);
			set_servo_position(catchingServoPort,450); // open the "hand"
			msleep(1000);
			motor(liftingMotorPort,-100);// put the hand down a bit
			msleep(300);
			set_servo_position(catchingServoPort,1400); // close the "hand"
			motor(liftingMotorPort,100);// lift the hand a bit
			msleep(200);

			create_drive_straight(-200);
			msleep(500);
			turnLeftDegrees(80);
			create_drive_straight(500);
			while(!get_create_rbump() && !get_create_lbump()){}
			turnLeftDegrees(80);
			create_drive_straight(-200);
			msleep(300);
			create_stop();
			motor(liftingMotorPort,-100);
			msleep(1000);
			off(liftingMotorPort);
			set_servo_position(catchingServoPort,300); // open the "hand"
		}
	}
}
void main()
{
	printf("Start!\n");
	servoInit(); // Supply power to all the servos
	motorInit(); // set the motor position
	create_connect();
	create_full();// FULL mode, the create will execute commands under any circumstance
	printf("Create connected!\n Battery: %d\n",get_create_battery_charge());
	
	turnLeftDegrees(80);// adjust heading in startup area
	goToHangerStand();
	putHangers();
	
	create_drive_straight(-200);// drive away from hanger stand
	while(!blackLine()){}
	msleep(300);
	turnRightDegrees(80);
	create_drive_straight(200);
	msleep(1700);
	turnLeftDegrees(80);
	create_drive_straight(200);
	while(!get_create_rbump() && !get_create_lbump()){}
	create_drive_straight(-200); // drive away from the PVC on the board
	msleep(420);
	turnRightDegrees(75);
	//create_stop();
	orangeCube();

	// go for yellow cube
	turnLeftDegrees(73);
	create_drive_straight(500);
	while(!get_create_rbump() && !get_create_lbump()){}
	turnLeftDegrees(75);
	create_stop();
	motor(liftingMotorPort,100);// lift the hand a bit to avoid collision
	set_servo_position(sensorLiftingServo,50);// retract the button sensor to avoid collision
	msleep(2000);
	off(liftingMotorPort);
	create_drive_straight(200);
	while(!blackLine()){}
	msleep(200);
	turnRightDegrees(76);
	motor(liftingMotorPort,-100);// put the hand down again
	set_servo_position(sensorLiftingServo,1200);// extend the button sensor again
	msleep(2000);
	yellowCube();

}
