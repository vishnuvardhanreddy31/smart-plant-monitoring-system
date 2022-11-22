/*Plant watering system with new blynk update
   
*/
//Include the library files
#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//Initialize the LCD display
//LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "_7dCKwiZc08gMEfwTf805ZA8tnS2MoXF";//Enter your Auth token
char ssid[] = "vishnu";//Enter your WIFI name
char pass[] = "vishnu@123";//Enter your WIFI password

BlynkTimer timer;
bool Relay = 0;

//Define component pins
#define sensor A0
#define waterPump D3
#define Dht D7
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH);
  dht.begin();
  //lcd.init();
  //lcd.backlight();

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  //lcd.setCursor(1, 0);
  //lcd.print("System Loading");
  //for (int a = 0; a <= 15; a++) {
    //lcd.setCursor(a, 1);
    //lcd.print(".");
    //delay(500);
  //}
  //lcd.clear();

  //Call the function
  timer.setInterval(100L, soilMoistureSensor);
}

//Get the button value
BLYNK_WRITE(V1) {
  Relay = param.asInt();

  if (Relay == 1) {
    digitalWrite(waterPump, LOW);
    //lcd.setCursor(0, 1);
    //lcd.print("Motor is ON ");
  } else {
    digitalWrite(waterPump, HIGH);
    //lcd.setCursor(0, 1);
    //lcd.print("Motor is OFF");
  }
}

//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value- 100) * -1;
  /**if(value<40)
  {
    digitalWrite(waterPump,LOW);
  }
  if(value>60)
  {
    digitalWrite(waterPump,HIGH);
  }
  Serial.println(value);**/

  Blynk.virtualWrite(V0, value);
  //lcd.setCursor(0, 0);
  //lcd.print("Moisture :");
  //lcd.print(value);
  //lcd.print(" ");
    float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  Blynk.virtualWrite(V3, h);
  Blynk.virtualWrite(V2, t);

  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

}

void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}
