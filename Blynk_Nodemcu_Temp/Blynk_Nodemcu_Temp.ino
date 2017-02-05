//#define BLYNK_DEBUG // Optional, this enables lots of prints
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Streaming.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS D4 // DS18B20 pin

char auth[] = "<BLYNK DEVICE AUTH KEY>";
char ssid[] = "<WIFI SSID>";
char pass[] = "<WIFI PASSWORD>";

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
char temperatureString[6];

void setup()
{
  DS18B20.begin();  
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth, ssid, pass);
}

float getTemperature() {
  //Serial << "Requesting DS18B20 temperature..." << endl;
  float temp;
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    delay(100);
    //Serial << "Temp: " << temp << endl;
  } while (temp == 85.0 || temp == (-127.0));
  
  return temp;
}

BLYNK_READ(V4) {  
  float temperature = getTemperature();
  
  // convert temperature to a string with two digits before the comma and 2 digits for precision
  dtostrf(temperature, 2, 2, temperatureString);
  
  Serial << "Sending temperature: " << temperatureString << endl;

  Blynk.virtualWrite(V4, temperature);
}

void loop()
{
  Blynk.run();
}

