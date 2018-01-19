/*Communicate with Android App 
  app updates status of plant to Arduino
  App responds by messages written in Arduino 
  like "Happy," "Too Dark", "Thirsty"
  "Drowning (too much water)
  Uno beeps piezo buzzer when plant is in trouble*/
  
#include <Wire.h>

int temp_address = 72;     //records temperature
int sensePin = 0;          //photoresistor to A0, records brightness
int light;                 //level of brightness recorded by Arduino
int troublePin = 13;       //light for troubled plant

void setup()
{
    analogReference(DEFAULT);
    //initializes communication with computer
    Serial.begin(9600);
    Wire.begin();    //listening of I2C on communication
    pinMode(troublePin, OUTPUT);
}

void loop()
{
    //send inputs
    int light = analogRead(sensePin);
    String t;
    String tempStatus;
    String brightStatus;
    t = "";

    //Temperature
    //Starts talking to the device
    Wire.beginTransmission(temp_address);
    //Ask for Register zero
    Wire.write(0);  //Wire.send() on other versions
    //Complete Transmission
    Wire.endTransmission();
    //Request 1 byte
    Wire.requestFrom(temp_address, 1);
    //Wait for response
    while(Wire.available() == 0);
    //get temperature, given in celsius
    int c = Wire.read();  //answer from request and waiting for availablity
                          //Wire.recieve on other versions
    //convert from Celsisus to Farenheit
    int f = round(c * 9.0 / 5.0 + 32.0);  
    
    //String t accumulates trouble messages
    while(Serial.available()) 
    {
      t += (char)Serial.read(); 
    }
    
    if(f < 80 || f > 85 || light < 500 || light > 550)
    {
      delay(500);
      digitalWrite(troublePin, HIGH);
      Serial.write("I'm in trouble.");
      
      if(f < 80) Serial.write("I'm freezing.");
      else Serial.write("I'm burning.");
      
      if(light < 500) Serial.write("I need light");
      else Serial.write("I need shade");
    }
    
    else 
    {
      digitalWrite(troublePin, LOW);
      //Serial.write("I'm fine, thanks.");
      //Serial.println(light);    
    }
    
    //delay, then do it again
    delay(3500);
}

