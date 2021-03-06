//#include "poyectoLib.h"

int tempControl = 0;
float tempSet = 0.0f, tempActual = 0.0f, tempActualT;
boolean alarmStatus = true, fireStatus = false, brokenGlassFlag = false, flagDia = true, bluetoothFlag = false, bombillaFlag = false, heatingFlag = false;
String inStr = "", st = "";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
SoftwareSerial BT(10, 11);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors( & oneWire);

void setup() {
  BT.begin(9600);
  Serial.begin(9600);
  sensors.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
  }
  display.display();
  delay(1000);
  display.clearDisplay();

  attachInterrupt(digitalPinToInterrupt(2), interrupcionCristal, LOW);
  attachInterrupt(digitalPinToInterrupt(SensorLlamaPin), interrupcionFuego, !LOW);
  pinMode(calefacionPin, OUTPUT);
  pinMode(bombillaReleePin, OUTPUT);
  pinMode(PIRsensorPin, INPUT);
  pinMode(12, INPUT);
}

void interrupcionCristal() {
  if (alarmStatus) {
    tone(alarmaAltavozPin, 1000, 1000);
    brokenGlassFlag = true;
  }
}
void interrupcionFuego() {
  if (BT.available()) {
    BT.print("l1~\r\n");
    BT.println();
  }
  fireStatus = true;
}
void loop() {
  if (fireStatus)
    displayFuego();
  else if (brokenGlassFlag)
    displayBrokenGlass();
  else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.drawCircle(51, 13, 1, WHITE);
    display.setCursor(24, 12);
    if (tempActual == 0.0f) 
      display.print(F("--.- C"));
    else {
      display.print(tempActual, 1);
      display.setCursor(53, 12);
      display.print(F("C"));
    }
    display.drawBitmap(0, 32, tempSetIcon, 23, 31, WHITE);
    display.drawCircle(51, 44, 1, WHITE);
    display.setCursor(24, 43);
    if (tempSet == 0.0f) 
      display.print(F("--.- C"));
    else {
      display.print(tempSet, 1);
      display.setCursor(53, 43);
      display.print(F("C"));
    }

    if (flagDia)
      display.drawBitmap(60, 0, dia, 31, 31, WHITE);
    else
      display.drawBitmap(60, 0, noche, 30, 31, WHITE);

    if (alarmStatus)
      display.drawBitmap(63, 32, alarmaON, 24, 31, WHITE);
    else
      display.drawBitmap(63, 32, alarmaOFF, 24, 31, WHITE);

    if (bluetoothFlag)
      display.drawBitmap(96, 0, bluetoothON, 28, 31, WHITE);
    else
      if(tempControl)
        display.drawBitmap(98, 0, bluetoothOFF, 25, 31, WHITE);

    if(heatingFlag){
      if(tempControl<2)
        display.drawBitmap(0, 0, tempActualIcon, 14, 31, WHITE);
      else if(tempControl ==2)
        display.drawBitmap(0, 0, tempActualIcon1, 14, 31, WHITE);
      else if(tempControl ==3)
        display.drawBitmap(0, 0, tempActualIcon2, 14, 31, WHITE);
      else if(tempControl ==4)
        display.drawBitmap(0, 0, tempActualIcon3, 14, 31, WHITE);
    }else
      display.drawBitmap(0, 0, tempActualIcon, 14, 31, WHITE);
      
    if (bombillaFlag)
      display.drawBitmap(95, 32, bombillaON, 31, 31, WHITE);
    else
      display.drawBitmap(95, 32, bombillaOFF, 31, 31, WHITE);

    display.display();
  }
  if(!digitalRead(12))
    bluetoothFlag=false;
    
  if (BT.available()) {
    inStr = BT.readString();
    if (inStr != NULL) {
      bluetoothFlag = true;
      Serial.println(inStr);
      if (inStr.indexOf("b1") >= 0) {
        digitalWrite(bombillaReleePin, HIGH);
        bombillaFlag = true;
      } else if (inStr.indexOf("b0") >= 0) {
        digitalWrite(bombillaReleePin, LOW);
        bombillaFlag = false;
      } else if (inStr.indexOf("s") != -1) {
        tempSet = inStr.substring(2, 5).toFloat() / 10;
        inStr.trim();
      } else if (inStr.indexOf("A") != -1) {
        if ((inStr.substring(1, 2)).toInt())
          alarmStatus = true;
        else
          alarmStatus = false;
      } else if (inStr.indexOf("x") != -1){
        bluetoothFlag = true;
      } else if (inStr.indexOf("y") != -1){
        bluetoothFlag = false;
      }
    }
  }

  if (!tempControl) {
    sensors.requestTemperatures();
    tempActualT = sensors.getTempCByIndex(0);
    if (tempActualT > 10.0f){
      tempActual = tempActualT;
    }
    BT.print("T");
    BT.print((int)(tempActual*10));
    BT.println();
  }
  tempControl = (tempControl + 1) % 5;
  if (analogRead(LDRAnalogPin) < 500) {
    BT.print("D~\r\n");
    BT.println();
    flagDia = true;
  } else {
    BT.print("N~\r\n");
    BT.println();
    flagDia = false;
  }
  if (tempSet != 0.0f) {
    if (tempActual < tempSet){
      digitalWrite(calefacionPin, HIGH);
      heatingFlag = true;
    }
    else{
      digitalWrite(calefacionPin, LOW);
      heatingFlag=false;
    }
  }
  if (digitalRead(PIRsensorPin) && alarmStatus) {
    tone(alarmaAltavozPin, 1000, 1000);
  }
  if (!digitalRead(SensorLlamaPin) && !tempControl) {
    BT.print("l0~\r\n");
    BT.println();
  }


}
void displayFuego() {
  for (int i = 0; i < 5; i++) {
    display.clearDisplay();
    display.drawBitmap(30, 0, casaFire1, 69, 64, WHITE);
    display.display();
    delay(1000);
    display.clearDisplay();
    display.drawBitmap(30, 0, casaFire2, 69, 64, WHITE);
    display.display();
    delay(1000);
  }
  display.clearDisplay();
  display.display();
  fireStatus = false;
}

void displayBrokenGlass() {
  display.clearDisplay();
  display.drawBitmap(31, 0, brokenGlass, 65, 64, WHITE);
  display.display();
  delay(5000);
  brokenGlassFlag = false;
}