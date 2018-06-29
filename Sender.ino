#ifndef humid_h
#define humid_h

#include "Arduino.h"

class humid
{
  public:
    humid(int pin = 2, int updateRate = 5);
    float readH();
    float readT();

  private:
    boolean readData();
    int _pin;
    float _humidity, _tem, _updateRate;
    unsigned int _timeout;
    unsigned long _lastread;
    byte _data[5], _checksum;
    boolean _firstReading = true;
};

#endif

////////////////////////

#include "Arduino.h"


humid::humid(int pin, int updateRate):_pin(pin), _updateRate(updateRate)
{
  if(updateRate < 2){
    Serial.println("TOO FAST UPDATERATE CHOSEN, USING DEAFULT OF 5 SECONDS...");
    _updateRate = 2;
  }
}


float humid::readH()
{
  if(((millis() - _lastread) > (_updateRate * 1000)) || (_firstReading)){
    if(readData()){
      return _humidity;
    }else{
      Serial.println("AN ERROR OCCURED WHILE READING, RETURNING NAN, PLEASE CHECK YOUR CONNECTIONS AND SETTINGS!");
      return NAN;
    }
  }else{
    return _humidity;
  }
}

float humid::readT()
{
  if(((millis() - _lastread) > (_updateRate * 1000)) || (_firstReading)){
    if(readData()){
      return _tem;
    }else{
      Serial.println("AN ERROR OCCURED WHILE READING, RETURNING NAN, PLEASE CHECK YOUR CONNECTIONS AND SETTINGS!");
      return NAN;
    }
  }else{
    return _tem;
  }
}

boolean humid::readData()
{
  //VARIABLES FOR RECEIVEING DATA
  byte _element = 0, _count =7;

  //REGISTERS LASTREAD AND TURNS FIRSTREADING TO FALSE
  if(_firstReading){
    _firstReading = false;
  }
  _lastread = millis();

  //CHECKS IF THERE HAVE BEEN A ROLLOVER
  if(_lastread > millis()){
    _lastread = 0;
  }
  
  //CLEARS DATA IN ARRAY AND ASSIGNS 0 TO EVERY ELEMENT
  for(byte i = 0; i < 5; i++){
    _data[i] = 0;
  }
  
  //SENDS DATA REQUEST TO SENSOR
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delay(20);
  digitalWrite(_pin, HIGH);
  delayMicroseconds(40);
  pinMode(_pin, INPUT);
  
  //READ SENSOR CONFIRMATION OR PRINT TIMEOUT ERROR
  _timeout = 10000;
  while(digitalRead(_pin) == LOW){
    if(_timeout-- == 0){
      Serial.println("ERROR: TIMEOUT at LOW START-SIGNAL");
      return false;
    }
  }
  _timeout = 10000;
  while(digitalRead(_pin) == HIGH){
    if(_timeout-- == 0){
      Serial.println("ERROR: TIMEOUT at HIGH START-SIGNAL");
      return false;
    }
  }
  
  //READ DATA
  for(byte i = 0; i < 40; i++){
    _timeout = 10000;
    while(digitalRead(_pin) == LOW){
      if(_timeout-- == 0){
        Serial.println("ERROR: TIMEOUT at READ SIGNAL LOW");
        return false;
      }
    }
    
    unsigned long length = micros();
    
    _timeout = 10000;
    while(digitalRead(_pin) == HIGH){
      if(_timeout-- == 0){
        Serial.println("ERROR: TIMEOUT at READ SIGNAL HIGH");
        return false;
      }
    }
    
    if((micros() - length) > 43){
      _data[_element] |= (1 << _count);
    }
    if(_count == 0){
      _count = 7;
      _element++;
    }else{
      _count--;
    }
  }
  
  //COMPARE CHECKSUM
  _checksum = (_data[0] + _data[1] + _data[2] + _data[3]);
  if (_data[4] != _checksum){
    Serial.println("ERROR: at CHECKSUM");
    return false;
  }
  
  //CONVERTS AND STORE DATA
  _humidity = word(_data[0], _data[1]) * 0.1;
  if(_data[2] & 0x80){
    _tem = (-0.1 * word(_data[2] & 0x7F, _data[3]));
  }else{
    _tem = (0.1 * word(_data[2], _data[3]));
  }
  return true;
}

////////////////////

#define rate 5

humid sensorOne(2, rate);

void setup(){
  Serial.begin(9600);
  //Serial.println("Booting up.....");
  //Serial.println("Starting to output data.....");
}

void loop(){
  //Serial.println("");
  //delay(1000);
 
  //sensorOne
  //Serial.print("Sensor 1: ");
  String h = String(sensorOne.readH());
  Serial.println(h);
  delay(1000);
  //Serial.print("%\t");
  //Serial.print(sensorOne.readT(), 1);
  //Serial.println("*C");
}