//#define BLYNK_DEBUG // Optional, this enables lots of prints
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>

char auth[] = "<BLYNK DEVICE AUTH KEY>";
char ssid[] = "<WIFI SSID>";
char pass[] = "<WIFI PASSWORD>";

WidgetLED led1(V1);
SimpleTimer timer;

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, blinkLedWidget);
}

void blinkLedWidget()
{
  if (digitalRead(6) == LOW) {
    led1.off(); // Sends off signal to Blynk app LED
    Serial.println("LED on V1: off");
  } else {
    led1.on(); // Sends on signal to Blynk app LED
    Serial.println("LED on V1: on");
  }
}

void loop()
{
  Blynk.run();
  timer.run();
}

