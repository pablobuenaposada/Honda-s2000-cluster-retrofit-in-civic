#include <TimerOne.h> //this library will allow us to use pwm signals with custom period
#include <EEPROMEx.h>

#define adc1Address 1
#define adc2Address 5
#define adc3Address 9
#define adc4Address 13
#define adc5Address 17
#define adc6Address 21

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
  
  ectADC1 = EEPROM.readInt(adc1Address);
  ectADC2 = EEPROM.readInt(adc2Address);
  ectADC3 = EEPROM.readInt(adc3Address);
  ectADC4 = EEPROM.readInt(adc4Address);
  ectADC5 = EEPROM.readInt(adc5Address);
  ectADC6 = EEPROM.readInt(adc6Address);

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
  
  if (millis() >= waitUntil){ ;
    ectReadandUpdate();
    waitUntil = millis() + update; 
  } 
}

void serialEvent() {
  
  boolean stringComplete = false;  
  
  while (Serial.available()){    
    char inChar = (char)Serial.read();    
    inputString += inChar;   
    if (inChar == '\n') {
      stringComplete = true;
    }  
  }

  if (stringComplete){
  
    if(inputString == "ECTREAD\n"){      
      Serial.print("ECTVALUES");
      Serial.print(EEPROM.readInt(adc1Address));
      Serial.print(".");
      Serial.print(EEPROM.readInt(adc2Address));
      Serial.print(".");
      Serial.print(EEPROM.readInt(adc3Address));
      Serial.print(".");
      Serial.print(EEPROM.readInt(adc4Address));
      Serial.print(".");
      Serial.print(EEPROM.readInt(adc5Address));
      Serial.print(".");
      Serial.println(EEPROM.readInt(adc6Address));      
    }
    
    else if(inputString.substring(0,6) == "ECTSET"){
      EEPROM.writeInt(adc1Address,inputString.substring(6,10).toInt());
      EEPROM.writeInt(adc2Address,inputString.substring(10,14).toInt());
      EEPROM.writeInt(adc3Address,inputString.substring(14,18).toInt());
      EEPROM.writeInt(adc4Address,inputString.substring(18,22).toInt());
      EEPROM.writeInt(adc5Address,inputString.substring(22,26).toInt());
      EEPROM.writeInt(adc6Address,inputString.substring(26,30).toInt());      
      Serial.println("ECTOK");      
      ectADC1 = EEPROM.readInt(adc1Address);
      ectADC2 = EEPROM.readInt(adc2Address);
      ectADC3 = EEPROM.readInt(adc3Address);
      ectADC4 = EEPROM.readInt(adc4Address);
      ectADC5 = EEPROM.readInt(adc5Address);
      ectADC6 = EEPROM.readInt(adc6Address);     
    }
    
    else if(inputString == "ECTNOW\n"){ 
      Serial.print("ECTACTUAL");
      Serial.println(analogRead(ectIn));  
    }
    
    else{
      Serial.print(inputString);
    }
    
    inputString="";  
  }
}
  
