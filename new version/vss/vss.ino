#include <FreqMeasure.h>
#include <EEPROM.h>
#define vssOut 12
#define vssIn 8
#define lowFactor 1
#define highFactor 2
#define samples 3

String inputString = "";
float frequency;
float factor; //38.29;
double sum=0;
int count=0;

void setup() {
  pinMode(vssOut, OUTPUT); //sets the pin as output
  pinMode(vssIn, INPUT); //sets the pin as input
  factor = (((EEPROM.read(highFactor)*100)+EEPROM.read(lowFactor)))/100;
  inputString.reserve(200);
  Serial.begin(9600); 
  FreqMeasure.begin();
}

void loop() {  
  
  if (FreqMeasure.available()){
    // average several reading together
    sum = sum + FreqMeasure.read();
    count = count + 1;
    if (count > samples) {
      double frequency = F_CPU / (sum / count);
      tone(vssOut,frequency*factor,100);
      sum = 0;
      count = 0;
    }
  }  
}

void serialEvent() {
  
  boolean stringComplete = false;  
  
  while(Serial.available()){   
    char inChar = (char)Serial.read();
    inputString += inChar; 
    if (inChar == '\n') {
      stringComplete = true;
    }  
  }
  
  if (stringComplete){
    if (inputString == "VSSREAD\n"){      
      Serial.print("VSSFACTOR");
      int factorAux = (EEPROM.read(highFactor)*100)+EEPROM.read(lowFactor); 
      Serial.println(factorAux);       
    }
    else if(inputString.substring(0,6) == "VSSSET"){
      EEPROM.write(highFactor,inputString.substring(6,8).toInt());
      EEPROM.write(lowFactor,inputString.substring(8,inputString.length()-1).toInt());  
      factor = (((EEPROM.read(highFactor)*100)+EEPROM.read(lowFactor)))/100;
      Serial.println("VSSOK");
    }
    else{
      Serial.print(inputString);
    }
    inputString = "";
  }
}




