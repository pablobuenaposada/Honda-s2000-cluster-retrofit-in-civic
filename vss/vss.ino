//VSS converter from civic eg vss to s2000 cluster
//Version 1.0
//read.the.disclaimer@gmail.com

#include <TimerOne.h> //this library will allow us to use pwm signals with custom period

int vssOut = 9;
int vssIn = 7;
double scale=0;
int factor=37; //scaling factor between s2000 pulses and civic pulses (s2000 pulses are around 37 times faster than civic ones)
unsigned long durationHigh=0; //duration of the pulse in High zone (5v)
unsigned long durationLow=0; //duration of the pulse in Low zone (0v)
unsigned long period=0;
unsigned long duty=0;
int i=9000;


void setup(){
  pinMode(vssOut, OUTPUT); //sets the pin as output
  pinMode(vssIn, INPUT); //sets the pin as input      
  Timer1.initialize(); //initializing the Timer1, so we can use pwm function
  Serial.begin(9600);
}
 
void loop(){ 
  int duration=pulseIn(vssIn,LOW);
  Serial.println(int);
  int speed = 1.263654879*pow(10,-2)*pow(duration,2) - 37.71764488*duration + 25133.52801;
  
  //Timer1.pwm(vssOut,512,18650/(0.50*220)); //we set the new custom pwm on vssOut pin
      
      Timer1.pwm(vssOut,512,18650/(0.50*speed));
      
      
     
        
       
      
}
