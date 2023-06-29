/*
 * IoT Based Controlled LED using Blynk and ESP8266 (Nodemcu)
 * This simple system controls the LEDs using Blynk web or apps via the internet. LEDs are controlled using the virtual switch widget.
 * 
 * How its works?
 * - Press switch widget V0 or V1 from the web or apps to turn ON the LED.
 * - Press the switch again to turn OFF the LED.
 * 
 * The circuit:
 * - Connect LED from pin D1 and D2 Nodemcu to the GROUND through a 220 Ω resistor.
 * - Create 2 datastreams (Virtual Pin) and name them LED and LED2. Set pin to V0 (LED) and V1 (LED2); set Data Type to Integer; set Default value 0.
 * - Create 2 switch widgets from the Blynk web dashboard and mobile dashboard. Go to switch settings and select datastream LED(V0) for switch 1 and LED(V1) for switch 2.
 * 
 * Edgent_ESP8266 Code from Blynk <https://blynk.io/>
 * Modified by My Creative Engineering <https://my-creative-engineering.blogspot.com>
 */
// Fill-in information from your Blynk Template here
//#define BLYNK_TEMPLATE_ID           "TMPLxxxxxx"
//#define BLYNK_TEMPLATE_NAME           "TEMPLATE"


#define BLYNK_TEMPLATE_ID "TMPL2GtcqBQM3"
#define BLYNK_TEMPLATE_NAME "Control LED"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
#include <DHT.h>
#define DHTPIN 4 
#define DHTTYPE DHT11


DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void setup()
{
  pinMode(D1,OUTPUT);
  pinMode(D0,OUTPUT);
  Serial.begin(115200);
  delay(100);

  BlynkEdgent.begin();
    dht.begin();
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}



void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V3, t);
  Blynk.virtualWrite(V4, h);
}

BLYNK_WRITE(V0) // Executes when the value of virtual pin 0 changes
{
  if(param.asInt() == 1)
  {
    // execute this code if the switch widget is now ON
    digitalWrite(D1,LOW);  // Set digital pin D1 HIGH
  }
  else
  {
    // execute this code if the switch widget is now OFF
    digitalWrite(D1,HIGH);  // Set digital pin D1 LOW    
  }
}


BLYNK_WRITE(V2) // Executes when the value of virtual pin 1 changes
{
  if(param.asInt() == 1)
  {
    // execute this code if the switch widget is now ON
    digitalWrite(D0,LOW);  // Set digital pin D2 HIGH
  }
  else
  {
    // execute this code if the switch widget is now OFF
    digitalWrite(D0,HIGH);  // Set digital pin D2 LOW    
  }
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);  // will cause BLYNK_WRITE(V0) to be executed
  Blynk.syncVirtual(V2);  // will cause BLYNK_WRITE(V1) to be executed
}

void loop() {
  BlynkEdgent.run();
  timer.run();
}
