#include <stdio.h>
#include <stdlib.h>
#include <softPwm.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <wiringPi.h>
#define Trig	28
#define Echo	29
#define sp 500

void ultraInit(void)
{
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
}
void run()     // 前进
{
    	softPwmWrite(4,0); //左轮前进
	softPwmWrite(1,sp); 
	softPwmWrite(6,0); //右轮前进
	softPwmWrite(5,sp);
}
void brake()         //刹车，停车
{
    	softPwmWrite(1,0); //左轮stop
	softPwmWrite(4,0); 
	softPwmWrite(5,0); //stop
	softPwmWrite(6,0); 
}
void right()
{
    	softPwmWrite(1,sp);
	softPwmWrite(4,0); 
	softPwmWrite(5,0);
	softPwmWrite(6,0); 
}
void left()
{
    	softPwmWrite(1,0);
	softPwmWrite(4,0); 
	softPwmWrite(5,sp);
	softPwmWrite(6,0);  
}
void back()          //后退
{
    	softPwmWrite(4,sp); //左轮back
	softPwmWrite(1,0); 
	softPwmWrite(6,sp); //右轮back
  	softPwmWrite(5,0); 
}
char getch()
{
	char c;
	system("stty -echo");
	system("stty -icanon");
	c=getchar();
	system("stty icanon");
	system("stty echo");
	return c;
}

int main(int argc, char *argv[])
{
    int key;
    /*RPI*/
    wiringPiSetup();
    /*WiringPi GPIO*/
    pinMode (1, OUTPUT);	//IN1
    pinMode (4, OUTPUT);	//IN2
    pinMode (5, OUTPUT);	//IN3
    pinMode (6, OUTPUT);	//IN4
    softPwmCreate(1,1,250);   
    softPwmCreate(4,1,250);
    softPwmCreate(5,1,250);
    softPwmCreate(6,1,250);
    while(1){
	key=getch();
	printf("Key=%d\n",key);
	if(key==65)
	     run();
	else if(key==66)
	    back();
	else if(key==67)
	    right();
	else if(key==68)
	    left();
	else if(key==32)
	    brake();
    }
    return 0;
}

