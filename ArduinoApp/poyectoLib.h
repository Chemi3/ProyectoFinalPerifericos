
#include <SoftwareSerial.h> 
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DallasTemperature.h>

#define bombillaReleePin 5
#define calefacionPin 6
#define alarmaAltavozPin 8
#define PIRsensorPin 7
#define LDRAnalogPin 2
#define SensorLlamaPin 3
#define SensorGolpePin 12
#define ONE_WIRE_BUS 4
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1


// 'casaFire1', 69x64px
const unsigned char casaFire1 [] PROGMEM = {
	0x00, 0x03, 0xfe, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0xf0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x07, 0xfe, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06, 0x07, 0x9e, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06, 
	0x1e, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06, 0x78, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 
	0x07, 0x07, 0xf0, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0xe0, 0x00, 0x3c, 0x00, 0x00, 
	0x00, 0x00, 0x07, 0x07, 0xc0, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x00, 0x00, 0x0f, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0x02, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 
	0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x07, 
	0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 
	0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x07, 
	0x80, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x31, 0xe0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 
	0x00, 0x78, 0xf0, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x78, 0x78, 0x00, 0x03, 0xc0, 0x00, 
	0x00, 0x00, 0x00, 0xf8, 0x3e, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x0f, 0x00, 0x1e, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x07, 0x80, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x03, 0xb8, 0x03, 
	0xc0, 0x78, 0x00, 0x00, 0x00, 0x00, 0x07, 0x38, 0x01, 0xf0, 0xff, 0x80, 0x00, 0x00, 0x00, 0x07, 
	0x18, 0x0f, 0xf8, 0xff, 0x80, 0x00, 0x00, 0x00, 0x0e, 0x1c, 0x0f, 0xf0, 0x03, 0x80, 0x00, 0x00, 
	0x00, 0x0e, 0x1c, 0x0c, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x0c, 0x0e, 0x0c, 0x00, 0x03, 0x80, 
	0x00, 0x00, 0x00, 0x1c, 0x0e, 0x0c, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x1c, 0x07, 0x0c, 0x00, 
	0x03, 0x80, 0x00, 0x00, 0x00, 0x18, 0x03, 0x80, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x18, 0x03, 
	0x80, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x38, 0x01, 0xc0, 0x00, 0x03, 0x80, 0x00, 0xff, 0x07, 
	0x38, 0x00, 0xe0, 0x00, 0x03, 0x80, 0x00, 0xff, 0x8f, 0x38, 0x00, 0xf0, 0x00, 0x03, 0x80, 0x00, 
	0xe0, 0x1f, 0x38, 0x00, 0x70, 0x00, 0x03, 0x80, 0x00, 0xe0, 0x3f, 0x30, 0x00, 0x38, 0x00, 0x03, 
	0x80, 0x00, 0xe0, 0x7b, 0x30, 0x00, 0x1c, 0x00, 0x03, 0x80, 0x00, 0xe0, 0x73, 0xb0, 0x00, 0x1c, 
	0x00, 0x03, 0x80, 0x00, 0xe0, 0xe3, 0xb0, 0x00, 0x0e, 0x00, 0x03, 0x80, 0x00, 0xe0, 0xe1, 0xf0, 
	0x00, 0x06, 0x00, 0x03, 0x80, 0x00, 0xe1, 0xc1, 0xf0, 0x00, 0x07, 0x00, 0x03, 0x80, 0x00, 0xe1, 
	0xc0, 0xf0, 0x00, 0x07, 0x00, 0x03, 0x80, 0x00, 0xe1, 0x80, 0x78, 0x00, 0x03, 0x00, 0x03, 0x80, 
	0x00, 0xe1, 0x80, 0x78, 0x00, 0x03, 0x80, 0x03, 0x80, 0x00, 0xe3, 0x80, 0x38, 0x00, 0x03, 0x80, 
	0x03, 0x80, 0x00, 0xe3, 0x80, 0x00, 0x00, 0x01, 0x80, 0x03, 0x80, 0x00, 0xe3, 0x80, 0x00, 0x00, 
	0x01, 0x80, 0x03, 0x80, 0x00, 0xe3, 0x80, 0x00, 0x00, 0x01, 0x80, 0x03, 0x80, 0x00, 0xe3, 0x80, 
	0x00, 0x00, 0x01, 0x80, 0x07, 0x80, 0x00, 0xe3, 0x80, 0x00, 0x00, 0x01, 0x80, 0x0f, 0x00, 0x00, 
	0xe3, 0x80, 0x00, 0x00, 0x03, 0x80, 0x1e, 0x00, 0x00, 0xe3, 0x80, 0x00, 0x00, 0x03, 0x80, 0x1c, 
	0x00, 0x00, 0xe1, 0x80, 0x00, 0x00, 0x03, 0x00, 0x18, 0x00, 0x00, 0xe1, 0xc0, 0x00, 0x00, 0x07, 
	0x00, 0x18, 0x00, 0x00, 0xe0, 0xc0, 0x00, 0x00, 0x07, 0x00, 0x18, 0x00, 0x00, 0xe0, 0xe0, 0x00, 
	0x00, 0x0e, 0x00, 0x18, 0x00, 0x00, 0xe0, 0x70, 0x00, 0x00, 0x1e, 0x00, 0x1f, 0xff, 0xff, 0xe0, 
	0x7f, 0xff, 0xff, 0xfc, 0x00, 0x1f, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xf8, 0x00, 0x0f, 0xff, 
	0xff, 0xc0, 0x07, 0xff, 0xff, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'casaFire2', 69x64px
const unsigned char casaFire2 [] PROGMEM = {
	0x00, 0x03, 0xfe, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0xf0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x07, 0xfe, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06, 0x07, 0x9e, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06, 
	0x1e, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06, 0x78, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 
	0x07, 0x07, 0xf0, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0xe0, 0x00, 0x3c, 0x00, 0x00, 
	0x00, 0x00, 0x07, 0x07, 0xc0, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x00, 0x00, 0x0f, 
	0x00, 0x00, 0x00, 0x00, 0x07, 0x02, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 
	0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x07, 
	0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 
	0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x07, 
	0x80, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x18, 0x01, 0xe0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 
	0x1c, 0x00, 0xf0, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x78, 0x00, 0x03, 0xc0, 0x00, 
	0x00, 0x00, 0x1e, 0x00, 0x3e, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x0f, 0x00, 0x1e, 
	0x00, 0x00, 0x00, 0x00, 0x1b, 0x80, 0x07, 0x80, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x1b, 0x80, 0x03, 
	0xc0, 0x78, 0x00, 0x00, 0x00, 0x00, 0x39, 0xc0, 0x01, 0xf0, 0xff, 0x80, 0x00, 0x00, 0x00, 0x38, 
	0xc0, 0x0f, 0xf8, 0xff, 0x80, 0x00, 0x00, 0x00, 0x30, 0xe0, 0x0f, 0xf0, 0x03, 0x80, 0x00, 0x00, 
	0x00, 0x70, 0xe0, 0x0c, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x70, 0x70, 0x0c, 0x00, 0x03, 0x80, 
	0x00, 0x00, 0x00, 0xe0, 0x70, 0x0c, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0xc0, 0x30, 0x0c, 0x00, 
	0x03, 0x80, 0x00, 0x00, 0x01, 0xc0, 0x30, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x03, 0x80, 0x38, 
	0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x07, 0x00, 0x38, 0x80, 0x00, 0x03, 0x80, 0x00, 0xff, 0x07, 
	0x00, 0x39, 0xc0, 0x00, 0x03, 0x80, 0x00, 0xff, 0x8e, 0x00, 0x19, 0xe0, 0x00, 0x03, 0x80, 0x00, 
	0xe0, 0x1c, 0x00, 0x19, 0xf0, 0x00, 0x03, 0x80, 0x00, 0xe0, 0x38, 0x00, 0x19, 0xf8, 0x00, 0x03, 
	0x80, 0x00, 0xe0, 0x38, 0x00, 0x19, 0x9c, 0x00, 0x03, 0x80, 0x00, 0xe0, 0x70, 0x00, 0x19, 0x9c, 
	0x00, 0x03, 0x80, 0x00, 0xe0, 0x60, 0x00, 0x1b, 0x8e, 0x00, 0x03, 0x80, 0x00, 0xe0, 0xe0, 0x00, 
	0x1f, 0x07, 0x00, 0x03, 0x80, 0x00, 0xe0, 0xc0, 0x00, 0x1f, 0x07, 0x00, 0x03, 0x80, 0x00, 0xe1, 
	0xc0, 0x00, 0x1e, 0x03, 0x00, 0x03, 0x80, 0x00, 0xe1, 0x80, 0x00, 0x1e, 0x03, 0x80, 0x03, 0x80, 
	0x00, 0xe1, 0x80, 0x00, 0x1c, 0x03, 0x80, 0x03, 0x80, 0x00, 0xe3, 0x80, 0x00, 0x18, 0x03, 0x80, 
	0x03, 0x80, 0x00, 0xe3, 0x80, 0x00, 0x00, 0x01, 0x80, 0x03, 0x80, 0x00, 0xe3, 0x80, 0x00, 0x00, 
	0x01, 0x80, 0x03, 0x80, 0x00, 0xe3, 0x80, 0x00, 0x00, 0x01, 0x80, 0x03, 0x80, 0x00, 0xe3, 0x80, 
	0x00, 0x00, 0x01, 0x80, 0x07, 0x80, 0x00, 0xe3, 0x80, 0x00, 0x00, 0x01, 0x80, 0x0f, 0x00, 0x00, 
	0xe3, 0x80, 0x00, 0x00, 0x01, 0x80, 0x1e, 0x00, 0x00, 0xe1, 0x80, 0x00, 0x00, 0x03, 0x80, 0x1c, 
	0x00, 0x00, 0xe1, 0x80, 0x00, 0x00, 0x03, 0x00, 0x18, 0x00, 0x00, 0xe1, 0xc0, 0x00, 0x00, 0x07, 
	0x00, 0x18, 0x00, 0x00, 0xe0, 0xc0, 0x00, 0x00, 0x07, 0x00, 0x18, 0x00, 0x00, 0xe0, 0xe0, 0x00, 
	0x00, 0x0e, 0x00, 0x18, 0x00, 0x00, 0xe0, 0x70, 0x00, 0x00, 0x1e, 0x00, 0x1f, 0xff, 0xff, 0xe0, 
	0x7f, 0xff, 0xff, 0xfc, 0x00, 0x1f, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xf8, 0x00, 0x0f, 0xff, 
	0xff, 0xc0, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'cedzcFjXF', 65x64px
const unsigned char brokenGlass [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
	0x30, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x40, 0x10, 0x00, 0x80, 0x20, 0x00, 0x00, 0x00, 
	0x00, 0x60, 0x70, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x2f, 0xf0, 0x01, 0x00, 0x40, 0x00, 
	0x00, 0x00, 0x00, 0x1f, 0x18, 0x01, 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x18, 0x18, 0x41, 0x00, 
	0x80, 0x02, 0x00, 0x00, 0x20, 0x08, 0x08, 0x41, 0xc1, 0xe0, 0x04, 0x00, 0x00, 0x10, 0x0c, 0x0f, 
	0xc3, 0xf3, 0x70, 0x18, 0x00, 0x00, 0x08, 0x04, 0x0c, 0x62, 0x3e, 0x1c, 0x30, 0x00, 0x00, 0x04, 
	0x06, 0x0c, 0x62, 0x0c, 0x0e, 0x60, 0x00, 0x00, 0x02, 0x07, 0x0c, 0x62, 0x0c, 0x03, 0x80, 0x00, 
	0x00, 0x01, 0x8d, 0xfc, 0x66, 0x18, 0x03, 0x00, 0x00, 0x00, 0x01, 0xd9, 0xe7, 0xe6, 0x1c, 0x06, 
	0x00, 0x00, 0x00, 0x01, 0xf0, 0xc7, 0xee, 0x37, 0x1c, 0x00, 0x00, 0x00, 0x01, 0x30, 0xc6, 0x7e, 
	0x63, 0xb8, 0x00, 0x00, 0x00, 0x01, 0x18, 0x66, 0x6f, 0xe1, 0xf8, 0x00, 0x00, 0x00, 0x03, 0x0c, 
	0x66, 0x6c, 0xc1, 0xd8, 0x00, 0x00, 0x00, 0x03, 0x0e, 0xf3, 0xfd, 0xf3, 0x8c, 0x00, 0x00, 0x00, 
	0x33, 0x07, 0x9f, 0xfd, 0xbf, 0x06, 0x00, 0x00, 0x00, 0x0f, 0x07, 0x9e, 0x0f, 0x1e, 0x06, 0x40, 
	0x00, 0x00, 0x03, 0x85, 0xf8, 0x07, 0xbc, 0x07, 0x80, 0x00, 0x00, 0x00, 0xf4, 0xf0, 0x01, 0xf6, 
	0x3c, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x70, 0x00, 0xe3, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x67, 0x30, 
	0x00, 0x6f, 0x98, 0x00, 0x00, 0x00, 0x00, 0x61, 0xf0, 0x00, 0x7e, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x60, 0x60, 0x00, 0x72, 0x10, 0x00, 0x00, 0x00, 0x00, 0x60, 0xe0, 0x00, 0x33, 0x10, 0x00, 0x00, 
	0x0f, 0xff, 0xff, 0xc0, 0x00, 0x7f, 0xff, 0xe0, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0x00, 0x7f, 0xff, 
	0xf8, 0x00, 0x00, 0x08, 0x0c, 0xe0, 0x00, 0xcc, 0x06, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0xe0, 0x00, 
	0xcc, 0x06, 0x00, 0x00, 0x00, 0x0c, 0x07, 0xf8, 0x01, 0xfc, 0x04, 0x00, 0x00, 0x00, 0x0c, 0x1e, 
	0x3c, 0x01, 0xbc, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0xf0, 0x3e, 0x03, 0xfe, 0x0c, 0x00, 0x00, 0x00, 
	0x07, 0xf0, 0xff, 0x1f, 0xe3, 0x8c, 0x00, 0x00, 0x00, 0x1e, 0x31, 0xdb, 0xff, 0x70, 0xe8, 0x00, 
	0x00, 0x00, 0x70, 0x1b, 0xdf, 0xfb, 0xf8, 0xd8, 0x00, 0x00, 0x03, 0x80, 0x0f, 0xff, 0x19, 0x9d, 
	0x84, 0x00, 0x00, 0x0e, 0x00, 0x0e, 0x67, 0x19, 0x8f, 0x00, 0x00, 0x00, 0x30, 0x00, 0x18, 0x46, 
	0x0f, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x86, 0x0e, 0x43, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x60, 0x87, 0xfc, 0x63, 0x80, 0x00, 0x00, 0x00, 0x01, 0x81, 0x06, 0x7c, 0x3f, 0x40, 0x00, 0x00, 
	0x00, 0x03, 0x00, 0x04, 0x04, 0x38, 0x20, 0x00, 0x00, 0x00, 0x06, 0x00, 0x04, 0x04, 0x10, 0x10, 
	0x00, 0x00, 0x00, 0x08, 0x00, 0x04, 0x06, 0x18, 0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x0c, 0x06, 
	0x78, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x07, 0xe4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x03, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x01, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'nights_stay-24px', 30x31px 60,0
const unsigned char noche [] PROGMEM = {
	0x00, 0x07, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 
	0x03, 0xfe, 0x00, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 
	0x1f, 0xfc, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 
	0x3f, 0xfe, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0x00, 0x00, 
	0x00, 0xff, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x1c, 0x1f, 0xc0, 0x00, 0x3f, 0x0f, 0xe0, 0x00, 
	0x7f, 0x83, 0xf0, 0x00, 0xff, 0xc3, 0xf8, 0x00, 0xff, 0xf1, 0xfe, 0x00, 0xff, 0xf1, 0xff, 0xc0, 
	0xff, 0xf9, 0xff, 0xfc, 0xff, 0xf1, 0xff, 0xf8, 0x7f, 0xf1, 0xff, 0xf0, 0x1f, 0xe1, 0xff, 0xe0, 
	0x00, 0x03, 0xff, 0xc0, 0x00, 0x07, 0xff, 0x00, 0x00, 0x07, 0xf8, 0x00
};
// 'bluetooth_disabled-24px', 25x31px 98,0
const unsigned char bluetoothOFF [] PROGMEM = {
	0x00, 0x18, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x20, 0x1f, 0x00, 0x00, 
	0x70, 0x1f, 0x80, 0x00, 0xf8, 0x1f, 0xc0, 0x00, 0x7c, 0x1f, 0xe0, 0x00, 0x3e, 0x1d, 0xf0, 0x00, 
	0x1f, 0x1c, 0xf8, 0x00, 0x0f, 0x8c, 0xf8, 0x00, 0x07, 0xc5, 0xf0, 0x00, 0x03, 0xe1, 0xe0, 0x00, 
	0x01, 0xf1, 0xc0, 0x00, 0x00, 0xf8, 0x80, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 
	0x00, 0x7f, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x03, 0xff, 0xe0, 0x00, 
	0x07, 0xdd, 0xf0, 0x00, 0x0f, 0x9c, 0xf8, 0x00, 0x1f, 0x1c, 0xfc, 0x00, 0x3e, 0x1d, 0xfe, 0x00, 
	0x3c, 0x1f, 0xff, 0x00, 0x18, 0x1f, 0xcf, 0x80, 0x00, 0x1f, 0x87, 0x00, 0x00, 0x1f, 0x02, 0x00, 
	0x00, 0x1e, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00
};
// 'bluetooth_connected-24px', 28x31px 96,0
const unsigned char bluetoothON [] PROGMEM = {
	0x00, 0x0e, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x0f, 0xc0, 0x00, 
	0x00, 0x0f, 0xe0, 0x00, 0x0c, 0x0f, 0xf0, 0x00, 0x1e, 0x0f, 0xf8, 0x00, 0x0f, 0x0f, 0x7c, 0x00, 
	0x07, 0x8f, 0x3e, 0x00, 0x03, 0xcf, 0x3c, 0x00, 0x01, 0xef, 0x78, 0x00, 0x00, 0xff, 0xf0, 0x00, 
	0x10, 0x7f, 0xe0, 0x80, 0x38, 0x3f, 0xc1, 0xc0, 0x7c, 0x1f, 0x83, 0xe0, 0xfc, 0x1f, 0x83, 0xf0, 
	0x7c, 0x1f, 0x83, 0xe0, 0x38, 0x3f, 0xc1, 0xc0, 0x10, 0x7f, 0xe0, 0x80, 0x00, 0xff, 0xf0, 0x00, 
	0x01, 0xef, 0x78, 0x00, 0x03, 0xcf, 0x3c, 0x00, 0x07, 0x8f, 0x3e, 0x00, 0x0f, 0x0f, 0x7c, 0x00, 
	0x0e, 0x0f, 0xf8, 0x00, 0x0c, 0x0f, 0xf0, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x0f, 0xc0, 0x00, 
	0x00, 0x0f, 0x80, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00
};
// 'lock_open-black-48dp', 24x31px 63,32
const unsigned char alarmaOFF [] PROGMEM = {
	0x00, 0x7e, 0x00, 0x01, 0xff, 0x00, 0x03, 0xff, 0x80, 0x07, 0xc3, 0xc0, 0x07, 0x01, 0xe0, 0x07, 
	0x00, 0xe0, 0x0e, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xe0, 0x3f, 0xff, 
	0xfc, 0x7f, 0xff, 0xfe, 0xff, 0xff, 0xfe, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x07, 
	0xe0, 0x00, 0x07, 0xe0, 0x00, 0x07, 0xe0, 0x3c, 0x07, 0xe0, 0x7e, 0x07, 0xe0, 0x7e, 0x07, 0xe0, 
	0x7e, 0x07, 0xe0, 0x7c, 0x07, 0xe0, 0x18, 0x07, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x07, 0xe0, 0x00, 
	0x07, 0xe0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xfc
};
// 'lock-black-48dp', 24x31px 63,32
const unsigned char alarmaON [] PROGMEM = {
	0x00, 0x7e, 0x00, 0x01, 0xff, 0x00, 0x03, 0xff, 0x80, 0x07, 0xc3, 0xc0, 0x07, 0x01, 0xe0, 0x07, 
	0x00, 0xe0, 0x0e, 0x00, 0xe0, 0x0e, 0x00, 0xe0, 0x0e, 0x00, 0xe0, 0x0e, 0x00, 0xe0, 0x3f, 0xff, 
	0xfc, 0x7f, 0xff, 0xfe, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0x83, 0xff, 0xff, 0x81, 0xff, 0xff, 
	0x81, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xfc
};
// 'wb_sunny-24px', 31x31px 60,0
const unsigned char dia [] PROGMEM = {
	0x00, 0x03, 0x80, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x03, 0x80, 0x00, 
	0x0e, 0x00, 0x00, 0xe0, 0x0f, 0x00, 0x01, 0xe0, 0x0f, 0x00, 0x01, 0xe0, 0x06, 0x0f, 0xe0, 0xc0, 
	0x00, 0x3f, 0xf8, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xfe, 0x00, 
	0x01, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0x00, 0xf1, 0xff, 0xff, 0x1e, 0xf1, 0xff, 0xff, 0x1e, 
	0xf1, 0xff, 0xff, 0x1e, 0x01, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0xff, 0xfe, 0x00, 
	0x00, 0x7f, 0xfc, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x06, 0x0f, 0xe0, 0xc0, 
	0x0f, 0x00, 0x01, 0xe0, 0x0f, 0x00, 0x01, 0xe0, 0x06, 0x00, 0x00, 0xc0, 0x00, 0x03, 0x80, 0x00, 
	0x00, 0x03, 0x80, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x03, 0x80, 0x00
};

// 'bombilla_off copia', 31x31px 95,32
const unsigned char bombillaOFF [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x18, 0x30, 0x00, 0x00, 0x20, 0x0c, 0x00, 
	0x00, 0x80, 0x02, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 
	0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x40, 0x04, 0x00, 
	0x00, 0x20, 0x08, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x1f, 0xf0, 0x00, 
	0x00, 0x1f, 0xf0, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x1f, 0xf0, 0x00, 
	0x00, 0x0f, 0xe0, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x03, 0x80, 0x00
};
// 'bombilla_onBW', 31x31px 95,32
const unsigned char bombillaON [] PROGMEM = {
	0x00, 0x0f, 0xe0, 0x00, 0x00, 0x20, 0x08, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0x80, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x08, 0x30, 0x18, 0x20, 0x10, 0x40, 0x04, 0x10, 
	0x10, 0x80, 0x02, 0x10, 0x21, 0x00, 0x01, 0x08, 0x21, 0x00, 0x01, 0x08, 0x23, 0x0e, 0xe0, 0x88, 
	0x02, 0x35, 0x58, 0x80, 0x02, 0x2b, 0xa8, 0x80, 0x02, 0x14, 0x50, 0x80, 0x22, 0x08, 0x20, 0x88, 
	0x23, 0x0e, 0xe1, 0x88, 0x21, 0x06, 0xc1, 0x08, 0x01, 0x86, 0xc3, 0x00, 0x10, 0x86, 0xc2, 0x10, 
	0x10, 0x46, 0xc4, 0x10, 0x08, 0x26, 0xc8, 0x20, 0x04, 0x36, 0xd8, 0x40, 0x00, 0x1f, 0xf0, 0x00, 
	0x01, 0x10, 0x11, 0x00, 0x00, 0x90, 0x12, 0x00, 0x00, 0x30, 0x18, 0x00, 0x00, 0x10, 0x10, 0x00, 
	0x00, 0x08, 0x20, 0x00, 0x00, 0x04, 0x40, 0x00, 0x00, 0x02, 0x80, 0x00
};
// 'temperatureActual', 14x31px 0,0
const unsigned char tempActualIcon [] PROGMEM = {
	0x07, 0x80, 0x1f, 0xc0, 0x18, 0xe0, 0x30, 0x60, 0x32, 0x30, 0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 
	0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 
	0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 0x73, 0x30, 0x63, 0x18, 0xcf, 0x98, 0xcd, 0xc8, 
	0xcc, 0xcc, 0xcf, 0x98, 0xe7, 0x98, 0x60, 0x38, 0x38, 0x70, 0x1f, 0xe0, 0x0f, 0x80
};
// 'temperatureActual1', 14x31px
const unsigned char tempActualIcon1 [] PROGMEM = {
	0x07, 0x80, 0x1f, 0xc0, 0x18, 0xe0, 0x30, 0x60, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 
	0x30, 0x30, 0x30, 0x30, 0x32, 0x30, 0x33, 0x30, 0x73, 0x30, 0x63, 0x18, 0xcf, 0x98, 0xcd, 0xc8, 
	0xcc, 0xcc, 0xcf, 0x98, 0xe7, 0x98, 0x60, 0x38, 0x38, 0x70, 0x1f, 0xe0, 0x0f, 0x80
};
// 'temperatureActual2', 14x31px
const unsigned char tempActualIcon2 [] PROGMEM = {
	0x07, 0x80, 0x1f, 0xc0, 0x18, 0xe0, 0x30, 0x60, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0x30, 
	0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 0x73, 0x30, 0x63, 0x18, 0xcf, 0x98, 0xcd, 0xc8, 
	0xcc, 0xcc, 0xcf, 0x98, 0xe7, 0x98, 0x60, 0x38, 0x38, 0x70, 0x1f, 0xe0, 0x0f, 0x80
};
// 'temperatureActual3', 14x31px
const unsigned char tempActualIcon3 [] PROGMEM = {
	0x07, 0x80, 0x1f, 0xc0, 0x18, 0xe0, 0x30, 0x60, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 
	0x30, 0x30, 0x32, 0x30, 0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 
	0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 0x33, 0x30, 0x73, 0x30, 0x63, 0x18, 0xcf, 0x98, 0xcd, 0xc8, 
	0xcc, 0xcc, 0xcf, 0x98, 0xe7, 0x98, 0x60, 0x38, 0x38, 0x70, 0x1f, 0xe0, 0x0f, 0x80
};

// 'temperatureSet', 23x31px 4,32
const unsigned char tempSetIcon [] PROGMEM = {
	0x07, 0x81, 0xc0, 0x1f, 0xc1, 0xf0, 0x18, 0xe0, 0x38, 0x30, 0x61, 0x18, 0x32, 0x31, 0xcc, 0x33, 
	0x30, 0xec, 0x33, 0x30, 0x6c, 0x33, 0x32, 0x64, 0x33, 0x32, 0x00, 0x33, 0x30, 0x00, 0x33, 0x30, 
	0x00, 0x33, 0x30, 0x00, 0x33, 0x30, 0x00, 0x33, 0x30, 0x00, 0x33, 0x30, 0x00, 0x33, 0x30, 0x00, 
	0x33, 0x30, 0x00, 0x33, 0x30, 0x00, 0x33, 0x30, 0x00, 0x33, 0x30, 0x00, 0x73, 0x30, 0x00, 0x63, 
	0x18, 0x00, 0xcf, 0x98, 0x00, 0xcd, 0xcc, 0x00, 0xcc, 0xcc, 0x00, 0xcf, 0x98, 0x00, 0xe7, 0x98, 
	0x00, 0x60, 0x38, 0x00, 0x38, 0x70, 0x00, 0x1f, 0xe0, 0x00, 0x0f, 0x80, 0x00
};
