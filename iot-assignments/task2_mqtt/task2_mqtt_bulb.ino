// Task 2: ESP32 bulb control through Adafruit IO (MQTT)
// Dashboard toggle -> feed "Bulb" -> MQTT broker -> ESP32 -> relay on GPIO 26
// Replace the placeholders below. Never commit real credentials.

#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID     "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

#define AIO_SERVER    "io.adafruit.com"
#define AIO_PORT      1883
#define AIO_USERNAME  "kumaraguru_rac"
#define AIO_KEY       "YOUR_ADAFRUIT_IO_KEY"

#define RELAY_PIN     26      // relay input, active-LOW module

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_PORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe bulbFeed(&mqtt, AIO_USERNAME "/feeds/Bulb");

void connectWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nWiFi connected, IP: " + WiFi.localIP().toString());
}

void connectMQTT() {
  if (mqtt.connected()) return;
  Serial.print("Connecting to Adafruit IO MQTT... ");
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    mqtt.disconnect();
    delay(5000);
  }
  Serial.println("MQTT connected");
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);          // HIGH = relay off on an active-LOW module
  connectWiFi();
  mqtt.subscribe(&bulbFeed);              // listen to the Bulb feed
}

void loop() {
  connectMQTT();                          // reconnect if the session dropped

  Adafruit_MQTT_Subscribe *sub;
  while ((sub = mqtt.readSubscription(1000))) {
    if (sub == &bulbFeed) {
      String command = (char *)bulbFeed.lastread;
      Serial.print("Received MQTT command: ");
      Serial.println(command);

      if (command == "ON") {
        digitalWrite(RELAY_PIN, LOW);     // active-LOW: LOW energises the relay
        Serial.println("Bulb ON");
      } else if (command == "OFF") {
        digitalWrite(RELAY_PIN, HIGH);
        Serial.println("Bulb OFF");
      }
    }
  }

  if (!mqtt.ping()) mqtt.disconnect();    // keep-alive
}
