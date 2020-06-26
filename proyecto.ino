//#include "poyectoLib.h"


int tempControl=0;
float tempSet = 0.0f, tempActual;
boolean alarmStatus=true, fireStatus=false;
String inStr = "", st="";


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
SoftwareSerial BT(10,11);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  BT.begin(9600);
  Serial.begin(9600);
  sensors.begin();
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
  }
  display.display();
  delay(2000);
  
  display.clearDisplay();

 //display.drawBitmap(0, 0, fuego, 18, 18, WHITE);
  display.display();
  delay(2000);
  attachInterrupt(digitalPinToInterrupt(2), interrupcionCristal, LOW);
  attachInterrupt(digitalPinToInterrupt(SensorLlamaPin), interrupcionFuego, !LOW);
  pinMode(calefacionPin, OUTPUT);
  pinMode(bombillaReleePin, OUTPUT);
  pinMode(PIRsensorPin, INPUT);

}

void interrupcionCristal() {
  if (alarmStatus)
    tone(alarmaAltavozPin, 1000, 1000);
}
void interrupcionFuego() {
    BT.print("l1~\r\n");
    BT.println();
	fireStatus=true;
}
void loop() {
  Serial.println(BT.isListening());
	if(fireStatus)
		displayFuego();

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
void displayFuego() {

	for(int i = 0; i<5; i++){
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
	fireStatus=false;
}

//tSet tActual icoLuz icoAlarma icoBombilla
//3fallos pin send señal arduino imprimir aviso por pantalla