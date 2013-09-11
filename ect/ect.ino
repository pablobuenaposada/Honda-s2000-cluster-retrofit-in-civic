//ECT converter from civic eg sensor to s2000 cluster
//Version 1.0
//read.the.disclaimer@gmail.com

#include <TimerOne.h> //this library will allow us to use pwm signals with custom period

int ectOut = 9;
int ectIn = A1;
int adcValue=0; //ect voltage in range between 0 and 1023
unsigned long ectBars[7]={1500000,1100000,700000,400000,300000,220000,150000}; //microseconds from 1 to 7 segments
int ectADC[6]={203,156,122,84,53,19}; //correspondence between bars and temperature (in adc units) (71,82,93,104,115,126 celsius)

long waitUntil=0; //timer
int update = 5000; //reading ect sensor only every 5 seconds

void setup(){
  pinMode(ectOut, OUTPUT); //sets the pin as output
  pinMode(ectIn, INPUT); //sets the pin as input    
  
  Timer1.initialize(); //initializing the Timer1, so we can use pwm function
  Timer1.pwm(ectOut,20,1530000); //we set the new custom pwm on vssOut pin
}

int ectReadandUpdate(){
  adcValue = analogRead(ectIn); //read the ect voltage
    
  //we match the voltage to his microseconds that represents x segments  
  if (adcValue == 0){
    Timer1.pwm(ectOut,20,ectBars[0]+30000); //we set the new custom pwm on ectOut pin
  }
  else if(adcValue < ectADC[5]){
    Timer1.pwm(ectOut,171,ectBars[6]+30000); //we set the new custom pwm on ectOut pin
  }
  else if(adcValue < ectADC[4]){
    Timer1.pwm(ectOut,122,ectBars[5]+30000); //we set the new custom pwm on ectOut pin
  }  
  else if(adcValue < ectADC[3]){
    Timer1.pwm(ectOut,93,ectBars[4]+30000); //we set the new custom pwm on ectOut pin
  }    
  else if(adcValue < ectADC[2]){  
    Timer1.pwm(ectOut,71,ectBars[3]+30000); //we set the new custom pwm on ectOut pin
  }
  else if(adcValue < ectADC[1]){
    Timer1.pwm(ectOut,42,ectBars[2]+30000); //we set the new custom pwm on ectOut pin
  }
  else if(adcValue < ectADC[0]){
    Timer1.pwm(ectOut,27,ectBars[1]+30000); //we set the new custom pwm on ectOut pin
  }
  else{
    Timer1.pwm(ectOut,20,ectBars[0]+30000); //we set the new custom pwm on ectOut pin
  }
}
 
void loop(){
  
  //we read ect sensor every 5 seconds
  if (millis() >= waitUntil){ ;
    ectReadandUpdate();
    waitUntil = millis() + update; 
  } 
}
