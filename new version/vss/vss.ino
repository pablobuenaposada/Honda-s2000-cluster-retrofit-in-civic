#include <TimerOne.h>
#include <EEPROM.h>

#define factorAddress 1
int vssOut = 9;
int vssIn = 7;
double scale=0;
int factor; 
unsigned long durationHigh=0; 
unsigned long durationLow=0;
unsigned long period=0;
unsigned long duty=0;
String inputString = "";

void setup(){
  pinMode(vssOut, OUTPUT); //sets the pin as output
  pinMode(vssIn, INPUT); //sets the pin as input
  
  factor = EEPROM.read(factorAddress);
  inputString.reserve(200);
  Serial.begin(9600);  
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
    if (inputString == "VSSREAD\n"){
      Serial.print("VSSFACTOR");
      Serial.println(EEPROM.read(factorAddress));       
    }
    else if(inputString.substring(0,6) == "VSSSET"){
      EEPROM.write(factorAddress,inputString.substring(6,inputString.length()-1).toInt());  
      Serial.println("VSSOK");
    }
    else{
      Serial.print(inputString);
    }
  }
  
  
}
  
  
  
  
  
  

