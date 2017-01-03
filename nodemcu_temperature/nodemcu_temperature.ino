#include <Streaming.h>

#include <DallasTemperature.h>

#include <OneWire.h>

/*
 Sketch which publishes temperature data from a DS1820 sensor to a MQTT topic.

 This sketch goes in deep sleep mode once the temperature has been sent to the MQTT
 topic and wakes up periodically (configure SLEEP_DELAY_IN_SECONDS accordingly).

 Hookup guide:
 - connect D0 pin to RST pin in order to enable the ESP8266 to wake up periodically
 - DS18B20:
     + connect VCC (3.3V) to the appropriate DS18B20 pin (VDD)
     + connect GND to the appopriate DS18B20 pin (GND)
     + connect D4 to the DS18B20 data pin (DQ)
     + connect a 4.7K resistor between DQ and VCC.
*/

#include <ESP8266WiFi.h>

#define SLEEP_DELAY_IN_SECONDS  30
#define ONE_WIRE_BUS            D4      // DS18B20 pin
 
const char* ssid = "SogetiGuest"; // INSERT WIFI NAME
const char* password = "styrbord"; // INSERT WIFI PASSWORD

const char* api_key = "8A5XAFTY0B7W2R1P"; // INSERT THINGSPEAK API KEY
char thingSpeakAddress[] = "api.thingspeak.com";

WiFiClient client;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

char temperatureString[6];

void setup() {
  // setup serial port
  Serial.begin(115200);

  // setup WiFi
  delay(10);
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // setup OneWire bus
  DS18B20.begin();
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

void updateThingSpeak(String tsData) {  
  if (client.connect(thingSpeakAddress, 80)) {
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: 8A5XAFTY0B7W2R1P\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);
  }
}

void loop() {
  float temperature = getTemperature();
  
  // convert temperature to a string with two digits before the comma and 2 digits for precision
  dtostrf(temperature, 2, 2, temperatureString);
  
  // send temperature to the serial console
  Serial << "Sending temperature: " << temperatureString << endl;
  
  // send temperature to the MQTT topic
  updateThingSpeak("field1=" + String(temperatureString));

  delay(60000); 
   
  //Serial << "Entering deep sleep mode for " << SLEEP_DELAY_IN_SECONDS << " seconds..." << endl;
  //ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
  ////ESP.deepSleep(10 * 1000, WAKE_NO_RFCAL);
  //delay(500);   // wait for deep sleep to happen
}
