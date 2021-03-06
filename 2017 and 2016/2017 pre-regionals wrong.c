#pragma config(Sensor, in1,    Pot,            sensorPotentiometer)
#pragma config(Motor,  port2,           rightWheelMotor, tmotorVex393_MC29, openLoop, driveRight)
#pragma config(Motor,  port3,           mannyServo,    tmotorServoStandard, openLoop)
#pragma config(Motor,  port4,           Wall,          tmotorVex269_MC29, openLoop)
#pragma config(Motor,  port7,           leftWheelMotor, tmotorVex393_MC29, openLoop, reversed, driveLeft)
#pragma config(Motor,  port8,           deflectorServo, tmotorServoStandard, openLoop)
#pragma config(Motor,  port9,           Shooter,       tmotorVex269_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

bool driveSpeed = false; //Boolean variable for drive speed
//float wallPos = 0; //Float variable for the wall position

task arcadeDrive()
{
	while(true)
	{
		if(driveSpeed == false)
		{
			if(abs(vexRT[Ch3]) < 10 && abs(vexRT[Ch4]) < 10) //Dead zone when joystick at approximately zero
			{
				motor[leftWheelMotor]  = 0;
				motor[rightWheelMotor] = 0;
			}
			else //Arcade drive on left joystick and full speed with 127 barrier
			{
				float y = vexRT[Ch4];
				float x = vexRT[Ch3];
				float xPlusY = x + y;
				float xMinusY = x - y;
				if(xPlusY > 127)
				{
					xPlusY = 127;
				}
				else if(xPlusY < -127)
				{
					xPlusY = -127;
				}
				if(xMinusY > 127)
				{
					xMinusY = 127;
				}
				else if(xMinusY < -127)
				{
					xMinusY = -127;
				}
				motor[leftWheelMotor]  = xPlusY;
				motor[rightWheelMotor] = xMinusY;
			}
		}
		else if(driveSpeed == true)//Optional half-speed
		{
			motor[leftWheelMotor]  = (vexRT[Ch3] + vexRT[Ch4])/2;
			motor[rightWheelMotor] = (vexRT[Ch3] - vexRT[Ch4])/2;
		}
		if(vexRT[Btn5D] == true)
		{
			driveSpeed = true;
		}
		if(vexRT[Btn5U] == true)
		{
			driveSpeed = false;
		}
		while(vexRT[Btn6D] == true)
		{
			motor[leftWheelMotor]  = 127/2;
			motor[rightWheelMotor] = 127/2;
		}
	}
}

//Task for running the Wall
task wallDrive()
{
	while(true)
	{
		if(vexRT[Ch2] < 10 && vexRT[Ch2] > -10)
		{
			motor[Wall] = 0;
		}
		else
		{
			motor[Wall] = -vexRT[Ch2];
		}
	}
}

//Task for running the shooter
task Shoot()
{
	while(true)
	{
		while(vexRT[Btn6U] == true)//While the top right button on the back of the controller is pressed down, the shooter runs in 1/3 second increments, always ending at the same position
		{
			motor[Shooter] = 127;
		}
		//wait1Msec(339);
		motor[Shooter] = 0;
	}
}

int hotLow = -40;
int hotMid = -127;
int coldLow = 60;
int coldMid = 30;
int deflectorAngle = hotMid;

//Task for the shooter's deflector angle
task Deflector()
{
	while(true)
	{
		if(vexRT[Btn8L] == true)
		{
			deflectorAngle = hotLow;
		}
		if(vexRT[Btn8U] == true)
		{
			deflectorAngle = hotMid;
		}
		if(vexRT[Btn7D] == true)
		{
			deflectorAngle = coldLow;
		}
		if(vexRT[Btn7L] == true)
		{
			deflectorAngle = coldMid;
		}
		if(vexRT[Btn7U] == true)
		{
			deflectorAngle = deflectorAngle++;
		}
		if(vexRT[Btn7R] == true)
		{
			deflectorAngle = deflectorAngle--;
		}
		motor[deflectorServo] = deflectorAngle;
	}
}

//Task for the Manny Getter servo
task mannyGetter()
{
	motor[mannyServo] = 7;
	while(true)
	{
		if(vexRT[Btn8D] == true)
		{
			motor[mannyServo] = -127;
		}
		else if(vexRT[Btn8R] == true)
		{
			motor[mannyServo] = 7;
		}
	}
}

task main()
{
	startTask(arcadeDrive);
	startTask(wallDrive);
	startTask(Deflector);
	startTask(Shoot);
	startTask(mannyGetter);
	while(true)
	{
		wait1Msec(1);
	}
}
