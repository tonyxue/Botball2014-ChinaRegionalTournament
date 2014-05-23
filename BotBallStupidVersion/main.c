#include <stdio.h>

#define liftingMotorPort 0
#define catchingServoPort 3
#define lightSensorPortNum 0
#define cubeButtonPort 15
#define sensorLiftingServo 2
#define blackLineSensorPort 1

const int turningSpeed=80, blackLineCriticalValue=700;
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
	msleep(5000);//2700
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
	turnLeftDegrees(84);
	goAlongLine(2.5);
	turnRightDegrees(80);
	create_drive_straight(-100);
	msleep(280);
	create_stop();
}
void putHangers() //checked
{
	//put the hangers on to the PVC
	motor(liftingMotorPort,-100);
	msleep(300);
	off(liftingMotorPort);
	create_drive_straight(200);
	msleep(450);
	create_stop();
	motor(liftingMotorPort,-100);
	msleep(300);
	off(liftingMotorPort);
	set_servo_position(catchingServoPort,400);
	msleep(500);
}
void orangeCube()
{
	int time=0,flag=0;
	set_servo_position(sensorLiftingServo,1100);// extend the sensor to its working position
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
			//create_drive_straight(200);

			turnLeftDegrees(80);
			create_drive_straight(200);
			msleep(50);
			turnRightDegrees(80);
			create_drive_straight(200);
			flag++;
			time=0;
		}
	}
	
	if (flag!=2)// if the attempt(s) didn't fail
	{
		create_drive_straight(-200);
		msleep(510);
		create_stop();

		turnLeftDegrees(75); // turn toward the shelf
		create_drive_straight(-200);
		msleep(310);
		create_stop();

		set_servo_position(catchingServoPort,350); // open the "hand"
		msleep(1000);

		motor(liftingMotorPort,-100); // put the hand down
		msleep(2000);
		off(liftingMotorPort);
		msleep(500);

		set_servo_position(catchingServoPort,1400); // close the "hand"
		motor(liftingMotorPort,100);// lift the hand
		msleep(600);
		off(liftingMotorPort);
		// got the cube

		//create_drive_straight(-200);
		//msleep(500);
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
	int time=0,flag=0;
	create_drive_straight(200);
	while(!digital(cubeButtonPort))
	{
		if (flag==1) break;
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
			flag++;
			time=0;
		}
	}
	if (flag!=1)
	{
		motor(liftingMotorPort,100);// lift the hand a bit
		msleep(3000);
		turnLeftDegrees(80);
		create_drive_straight(-200);
		msleep(1000);
		create_stop();
		set_servo_position(catchingServoPort,900); // open the "hand"
		msleep(1000);
		motor(liftingMotorPort,-100);// put the hand down a bit
		msleep(2800);
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
		set_servo_position(catchingServoPort,1400); // close the "hand" again
	}
}
void main()
{
	printf("Start!\n");
	//wait_for_light(lightSensorPortNum);
	//shut_down_in(120);

	servoInit(); // Supply power to all the servos
	motorInit(); // set the motor position
	create_connect();
	create_full();// FULL mode, the create will execute commands under any circumstance
	printf("Create connected!\n Battery: %d\n",get_create_battery_charge());
	
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
	turnRightDegrees(80);
	goAlongLine(1.8);
	turnLeftDegrees(80);
	create_drive_straight(200);
	while(!get_create_rbump() && !get_create_lbump()){}
	create_drive_straight(-200); // drive away from the PVC on the board
	msleep(600);
	turnRightDegrees(75);
	orangeCube();
	/********************************************************************/
	// go for yellow cube
	create_drive_straight(-200);
	msleep(500);
	turnLeftDegrees(62);
	create_drive_straight(500); //rush
	msleep(2700);
	turnLeftDegrees(74);
	create_drive_straight(200);
	while(!blackLine()){}
	create_drive_straight(-200);
	msleep(500);
	create_stop();
	//motor(liftingMotorPort,100);// lift the hand a bit to avoid collision
	set_servo_position(sensorLiftingServo,50);// retract the button sensor to avoid collision
	msleep(2000);
	//off(liftingMotorPort);
	turnRightDegrees(76);
	create_stop();
	//motor(liftingMotorPort,-100);// put the hand down again
	set_servo_position(sensorLiftingServo,1200);// extend the button sensor again
	msleep(2000);

	yellowCube();

}
