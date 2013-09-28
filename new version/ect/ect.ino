#include <TimerOne.h> //this library will allow us to use pwm signals with custom period
#include <EEPROM.h>

#define adc1Address 1
#define adc2Address 2
#define adc3Address 3
#define adc4Address 4
#define adc5Address 5
#define adc6Address 6

int ectOut = 9;
int ectIn = A1;
int adcValue = 0; //ect voltage in range between 0 and 1023
unsigned long ectPeriod[7] = {1500000,1100000,700000,400000,300000,220000,150000}; //microseconds from 1 to 7 segments (period)
int ectDuty[7] = {20,27,42,71,93,122,171}; //microseconds from 1 to 7 segments (duty)
//int ectADC[6] = {203,156,122,84,53,19}; //correspondence between bars and temperature (in adc units) (71,82,93,104,115,126 celsius)

long waitUntil = 0; //timer
int update = 5000; //reading ect sensor only every 5 seconds

int ectADC1;
int ectADC2;
int ectADC3;
int ectADC4;
int ectADC5;
int ectADC6;

String inputString = "";

void setup(){
  pinMode(ectOut, OUTPUT); //sets the pin as output
  pinMode(ectIn, INPUT); //sets the pin as input
  
  ectADC1 = EEPROM.read(adc1Address);
  ectADC2 = EEPROM.read(adc2Address);
  ectADC3 = EEPROM.read(adc3Address);
  ectADC4 = EEPROM.read(adc4Address);
  ectADC5 = EEPROM.read(adc5Address);
  ectADC6 = EEPROM.read(adc6Address);

  inputString.reserve(200);
  Serial.begin(9600);    
  
  Timer1.initialize(); //initializing the Timer1, so we can use pwm function
  Timer1.pwm(ectOut,ectDuty[0],ectPeriod[0]+30000); //we set the new custom pwm on vssOut pin
}

int ectReadandUpdate(){
  adcValue = analogRead(ectIn); //read the ect voltage
    
  //we match the voltage to his microseconds that represents x segments  
  if (adcValue == 0){
    Timer1.pwm(ectOut,ectDuty[0],ectPeriod[0]+30000); //we set the new custom pwm on ectOut pin
  }
  else if(adcValue < ectADC6){
    Timer1.pwm(ectOut,ectDuty[6],ectPeriod[6]+30000); //we set the new custom pwm on ectOut pin
  }
  else if(adcValue < ectADC5){
    Timer1.pwm(ectOut,ectDuty[5],ectPeriod[5]+30000); //we set the new custom pwm on ectOut pin
  }  
  else if(adcValue < ectADC4){
    Timer1.pwm(ectOut,ectDuty[4],ectPeriod[4]+30000); //we set the new custom pwm on ectOut pin
  }    
  else if(adcValue < ectADC3){  
    Timer1.pwm(ectOut,ectDuty[3],ectPeriod[3]+30000); //we set the new custom pwm on ectOut pin
  }
  else if(adcValue < ectADC2){
    Timer1.pwm(ectOut,ectDuty[2],ectPeriod[2]+30000); //we set the new custom pwm on ectOut pin
  }
  else if(adcValue < ectADC1){
    Timer1.pwm(ectOut,ectDuty[1],ectPeriod[1]+30000); //we set the new custom pwm on ectOut pin
  }
  else{
    Timer1.pwm(ectOut,ectDuty[0],ectPeriod[0]+30000); //we set the new custom pwm on ectOut pin
  }
}
 
void loop(){
  
  //we read ect sensor every 5 seconds
  if (millis() >= waitUntil){ ;
    ectReadandUpdate();
    waitUntil = millis() + update; 
  } 
  delay(800);
}

void serialEvent() {
  
  boolean stringComplete = false;
  inputString="";  
  
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }  
  }

  if (stringComplete){
  
    if (inputString == "ECTREAD\n"){      
      Serial.print("ECTVALUES");
      Serial.print(EEPROM.read(adc1Address));
      Serial.print(".");
      Serial.print(EEPROM.read(adc2Address));
      Serial.print(".");
      Serial.print(EEPROM.read(adc3Address));
      Serial.print(".");
      Serial.print(EEPROM.read(adc4Address));
      Serial.print(".");
      Serial.print(EEPROM.read(adc5Address));
      Serial.print(".");
      Serial.println(EEPROM.read(adc6Address));      
    }
    
    else if(inputString.substring(0,6) == "ECTSET"){
      EEPROM.write(adc1Address,inputString.substring(6,9).toInt());
      EEPROM.write(adc2Address,inputString.substring(9,12).toInt());
      EEPROM.write(adc3Address,inputString.substring(12,15).toInt());
      EEPROM.write(adc4Address,inputString.substring(15,18).toInt());
      EEPROM.write(adc5Address,inputString.substring(18,21).toInt());
      EEPROM.write(adc6Address,inputString.substring(21,24).toInt());
      
      Serial.println("ECTOK");
      
      ectADC1 = EEPROM.read(adc1Address);
      ectADC2 = EEPROM.read(adc2Address);
      ectADC3 = EEPROM.read(adc3Address);
      ectADC4 = EEPROM.read(adc4Address);
      ectADC5 = EEPROM.read(adc5Address);
      ectADC6 = EEPROM.read(adc6Address);
      
      
      
    }
    else{
      Serial.print(inputString);
    }
  }
}
  
