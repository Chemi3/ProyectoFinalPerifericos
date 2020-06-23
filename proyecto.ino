
#include <SoftwareSerial.h> 
#include <OneWire.h>
#include <dht.h>
#include <DallasTemperature.h>
#define bombillaReleePin 5
#define calefacionPin 6
#define alarmaAltavozPin 8
#define PIRsensorPin 7
#define LDRAnalogPin 2
#define SensorLlamaPin 3
#define SensorGolpePin 12
#define ONE_WIRE_BUS 4

int tempControl=0;
float tempSet = 0.0f, tempActual;
boolean alarmStatus=true;
String inStr = "", st="";



SoftwareSerial BT(10,11);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  BT.begin(9600); 
  Serial.begin(9600); 
  pinMode(calefacionPin, OUTPUT);
  pinMode(bombillaReleePin, OUTPUT);
  pinMode(PIRsensorPin, INPUT);
  sensors.begin();
  attachInterrupt(digitalPinToInterrupt(2), interrupcion, LOW);
  attachInterrupt(digitalPinToInterrupt(SensorLlamaPin), interrupcion2, !LOW);
}

void interrupcion() {
  if (alarmStatus)
    tone(alarmaAltavozPin, 1000, 1000);
}
void interrupcion2() {
    BT.print("l1~\r\n");
    BT.println();
}
void loop() {
  


  inStr = BT.readString();
  Serial.print(inStr);
  if (inStr.indexOf("b") >= 0) {
    if ((inStr.substring(1, 2)).toInt())
      digitalWrite(bombillaReleePin, HIGH);
    else
      digitalWrite(bombillaReleePin, LOW);
  } else if (inStr.indexOf("s") != -1) {
    tempSet = inStr.substring(2, 5).toFloat() / 10;
    Serial.println(tempSet);
  } else if (inStr.indexOf("A") != -1) {
    if ((inStr.substring(1, 2)).toInt())
      alarmStatus = true;
    else
      alarmStatus = false;
  }

  if (tempControl == 0) {
    sensors.requestTemperatures();
    tempActual = sensors.getTempCByIndex(0);
    st = " T" + String((int)(tempActual * 10)) + "~\r\n";
    BT.print(st);
    BT.println();
  }
  tempControl = (tempControl + 1) % 5;
  if (analogRead(LDRAnalogPin) < 500) {
    BT.print("D~\r\n");
    BT.println();
  } else {
    BT.print("N~\r\n");
    BT.println();
  }
  if (tempSet != 0.0f) {
    //Serial.println("setTemp: " + String(tempSet));
    if (tempActual < tempSet) 
      digitalWrite(calefacionPin, HIGH);
    else
      digitalWrite(calefacionPin, LOW);
  }
  if (digitalRead(PIRsensorPin) && alarmStatus) {
    tone(alarmaAltavozPin, 1000, 1000);
  }
  if(!digitalRead(SensorLlamaPin)&&!tempControl){
    BT.print("l0~\r\n");
    BT.println();
  }
  
  
}

