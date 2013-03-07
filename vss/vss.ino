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

void setup(){
  pinMode(vssOut, OUTPUT); //sets the pin as output
  pinMode(vssIn, INPUT); //sets the pin as input    
  
  Timer1.initialize(); //initializing the Timer1, so we can use pwm function
}
 
void loop(){ 
  
      //reading pulse from vss
      durationHigh = pulseIn(vssIn,HIGH);
      durationLow = pulseIn(vssIn,LOW);
      //divide the pulse to get it "factor" times faster (37 in our case)
      durationHigh = durationHigh/factor;
      durationLow = durationLow/factor;
      //calculing period and duty of the new pulse
      period=durationHigh+durationLow;
      scale = 1024/period; //10bit duty in Timer1, range [0,1023]
      if (scale < 1){ //this is because with low speeds we get duty=0,xx... and this make the pulse all time low (0v)
        scale=1; //fixing this with the minimum reasonable value (1)
      }
      duty=scale*durationHigh;
      Timer1.pwm(vssOut,duty,period); //we set the new custom pwm on vssOut pin
}
