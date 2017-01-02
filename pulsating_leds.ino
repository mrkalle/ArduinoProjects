#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
 
#define PIN      6
#define N_LEDS 84
 
SoftwareSerial mySerial(10, 11); // RX, TX

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int loopTime = 10;

int r0start = 0;
int r0end = 31;

int r1start = 32;
int r1end = 55;

int r2start = 56;
int r2end = 71;

int r3start = 72;
int r3end = 83;

float r0degree = 0;
float r1degree = 30;
float r2degree = 60;
float r3degree = 90;
 
void setup() {
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
    
  strip.begin();
  //strip.setBrightness(32);
}

uint32_t getColor(int ring) {
  float degree = 0;
  if (ring == 0) {
    degree = r0degree;
  } else if (ring == 1) {
    degree = r1degree;
  } else  if (ring == 2) {
    degree = r2degree;
  } else  if (ring == 3) {
    degree = r3degree;
  }
  
  float rad = (degree * PI) / 180;
  float sinus = sin(rad);
  float strength = (sinus + 1) / 2; // 0-1 i styrka

  int color = (int)(strength*200) + 12;   
  return strip.Color(color, 0, 0);
}

void setColorsForRings() {   
  for (int i = r0start; i <= r0end; i++) {  
    strip.setPixelColor(i, getColor(0));
  }
  
  for (int i = r1start; i <= r1end; i++) {
    strip.setPixelColor(i, getColor(1));
  }
  
  for (int i = r2start; i <= r2end; i++) {
    strip.setPixelColor(i, getColor(2));
  }
  
  for (int i = r3start; i <= r3end; i++) {
    strip.setPixelColor(i, getColor(3));
  }
}

void setDegreesForRings() {
  r0degree = r0degree + 3;  
  if (r0degree >= 360) {
    r0degree = 0;
  }

  r1degree = r1degree + 3;  
  if (r1degree >= 360) {
    r1degree = 0;
  }

  r2degree = r2degree + 3;  
  if (r2degree >= 360) {
    r2degree = 0;
  }

  r3degree = r3degree + 3;  
  if (r3degree >= 360) {
    r3degree = 0;
  }
}

void loop() {     
  setColorsForRings();
  setDegreesForRings();  
  strip.show();
  delay(loopTime);
} 
