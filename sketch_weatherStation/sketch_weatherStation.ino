#include <DHT.h>        // including the library of DHT11 temperature and humidity sensor
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define DHTTYPE DHT11   // DHT 11

// Define firebase config
#define FIREBASE_HOST "weatherstation-db937.firebaseio.com"
#define FIREBASE_AUTH "kvuMItSkjnhPEofSrdgSfzY3cJ9dRZT8FMj1JSS3"

// Define Wifi config
#define WIFI_SSID "iPhoneValentin2"
#define WIFI_PASSWORD "macbookVal"

// Define pins
#define dht_dpin 0
int relayInputLight = 4;
int relayInputHeater = 5;

DHT dht(dht_dpin, DHTTYPE);

void setup()
{ 
  Serial.begin(9600);
  dht.begin();

  // Wifi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  // Firebase initialization
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //relay pins setup
  pinMode(relayInputLight, OUTPUT);
  pinMode(relayInputHeater, OUTPUT);
}


void loop() {
  //relays
  if(Firebase.getBool("lamp/on")) {
      digitalWrite(relayInputLight, HIGH); // turn relay on
  } else {
      if(Firebase.getFloat("light") < 150) {
        digitalWrite(relayInputLight, HIGH);
      } else {
         digitalWrite(relayInputLight, LOW); // turn relay off
      }
  }
  if(Firebase.getBool("heater/on")) {
      digitalWrite(relayInputHeater, HIGH); // turn relay on
  } else {
      if(Firebase.getFloat("temperature") < 10) {
        digitalWrite(relayInputHeater, HIGH);
      } else {
        digitalWrite(relayInputHeater, LOW); // turn relay off
      }
  }
  
  float light = analogRead(A0);  
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  Serial.println(light);
  Serial.println(hum);
  Serial.println(temp);
  
  // set value in firebase for light
  Firebase.setFloat("light", light);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /light failed:");
      Serial.println(Firebase.error());  
      //return;
  }

  // set value in firebase for moisture
  Firebase.setFloat("moisture", hum);
  if (Firebase.failed()) {
      Serial.print("setting /hum failed:");
      Serial.println(Firebase.error());  
      //return;
  }

  // set value in firebase for temperature
  Firebase.setFloat("temperature", temp);
  if (Firebase.failed()) {
      Serial.print("setting /temp failed:");
      Serial.println(Firebase.error());  
      //return;
  }
}

  
