#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>
#include <softPwm.h>
#include <unistd.h>
#include <sys/types.h>


#define HW_LEFT		11
#define HW_RIGHT	10
#define Trig		28
#define Echo		29
#define SP 		500


float disMeasure(void)
{
  struct timeval tv1;
  struct timeval tv2;
  long start, stop;
  float dis;

  digitalWrite(Trig, LOW);
  delayMicroseconds(2);

  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);	  //发出超声波脉冲
  digitalWrite(Trig, LOW);

  while(!(digitalRead(Echo) == 1));
  gettimeofday(&tv1, NULL);		   //获取当前时间

  while(!(digitalRead(Echo) == 0));
  gettimeofday(&tv2, NULL);		   //获取当前时间

  start = tv1.tv_sec * 1000000 + tv1.tv_usec;   //微秒级的时间
  stop  = tv2.tv_sec * 1000000 + tv2.tv_usec;

  dis = (float)(stop - start) / 1000000 * 34000 / 2;  //求出距离

  return dis;
}


void run(int time)     // 前进
{
    softPwmWrite(1,SP); //左轮前进
    softPwmWrite(4,0); 
    softPwmWrite(5,SP); //右轮前进
    softPwmWrite(6,0);
    delay(time * 100);
}
void brake(int time)         //刹车，停车
{
    softPwmWrite(1,0); //左轮stop
    softPwmWrite(4,0); 
    softPwmWrite(5,0); //右轮stop
    softPwmWrite(6,0); 
    delay(time * 20);
}
void left(int time)         //左转(左轮不动，右轮前进)
{
    softPwmWrite(1,0); //左轮stop
    softPwmWrite(4,0); 
    softPwmWrite(5,SP); //右轮前进
    softPwmWrite(6,0); 
    delay(time * 100);
}
void right(int time)        //右转(右轮不动，左轮前进)
{
    softPwmWrite(1,SP); //左轮前进
    softPwmWrite(4,0); 
    softPwmWrite(5,0); //右轮stop
    softPwmWrite(6,0); 
    delay(time * 100);
}



int main(int argc, char *argv[])
{
    int SR;
    int SL;
    float dis;
    /*RPI*/
    wiringPiSetup();
    /*WiringPi GPIO*/
    pinMode (1, OUTPUT);	//IN1
    pinMode (4, OUTPUT);	//IN2
    pinMode (5, OUTPUT);	//IN3
    pinMode (6, OUTPUT);	//IN4
    pinMode(HW_LEFT, INPUT);
    pinMode(HW_RIGHT, INPUT);
    pinMode(Echo, INPUT);
    pinMode(Trig, OUTPUT);
    softPwmCreate(1,1,500);
    softPwmCreate(4,1,500);
    softPwmCreate(5,1,500);
    softPwmCreate(6,1,500);

while(1)
  {
    //有信号为LOW  没有信号为HIGH
    SR = digitalRead(HW_RIGHT);
    SL = digitalRead(HW_LEFT);
    dis = disMeasure();
    printf("distance = %0.2f cm\n",dis);//输出当前超声波测得的距离

    if(dis<30&&dis>0.3){   //测得前方障碍的距离小于30cm时做出如下响应
	if(SL == LOW){
	    printf("SL..TurnRight\n");
	    brake(1);
	    right(2);
	}
	else if(SR == LOW){
	    printf("SR..TurnLeft\n");
	    brake(1);
	    left(4);
	}
	else{
	    printf("TurnLeft\n");
	    brake(1);
	    left(2);
	}
    }
    else{
	if(SL == LOW){
	    printf("SL...\n");
	    brake(1);
	    right(2);
	}
	else if(SR == LOW){
	    printf("SR...\n");
	    brake(1);
	    left(2);
	}
	else{
	    printf("RUN\n");
            run(1);  //无障碍时前进
	}
    }


    /*if (SR == LOW){
      printf("RIGHT\n");
    }
    else if (SL == LOW) {
      printf("LEFT\n");
    }
    else {
      printf("GO\n");
      printf("gg..");
    }*/


  }
  return 0;
}

