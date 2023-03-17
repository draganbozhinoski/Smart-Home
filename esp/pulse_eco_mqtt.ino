// Load Wi-Fi library
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "DHT.h"
#include <SdsDustSensor.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "math.h"


//DHT22
#define DHTPIN D4
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define SENSOR_PIN A0

DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "Dragan's Galaxy S21+ 5G";
const char* password = "pass";
WiFiClientSecure wifiClient;
HTTPClient http;
const char* mqtt_username = "user";
const char* mqtt_password = "pass";
const char* mqtt_topic = "pm";
//MQTT DEFINITIONS
PubSubClient mqttClient(wifiClient); 
const char *mqttServer = "server";
int mqttPort = 8883;
int counter = 0;
static const char *fingerprint PROGMEM = "fingerprint";
const char ca_cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFXjCCBEagAwIBAgISAwqKoRgzKvf6r3r1hXjrqrfaMA0GCSqGSIb3DQEBCwUA
MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD
EwJSMzAeFw0yMzAxMjgwODE5MjFaFw0yMzA0MjgwODE5MjBaMB8xHTAbBgNVBAMT
FGNvbnNvbGUuaGl2ZW1xLmNsb3VkMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIB
CgKCAQEA1zAuyj2nz2w2stp2q0qmb+bZ/ytNLXeVXBmznTxhtP4+XyhU+j7WapH1
u9Dpk++Aygnq/Z5/8gsO+IrdZ1ND3iUC1zRPJw1cPWTJPpsBLvoeG72eWmC3rSEf
bKBqqR9hbUB70LPmsUO0H8nlnlAUN43Ol3pT/BOhPb92Oy0zKj9EpKw/2trzZMYu
ci6UUt8Z/z+TiKAocsP84PfXt9N1JHy76L44v3L4kSc33fM1rIwHtBzJ5x/K1hrj
Uvd1MxBWJERwpG7RPWRbcIo2rDE4DGEmYJuU9C9beAO4gcVY0bw//i63A1BZImwH
mULM/z4Vbk+/CMXFPhKvV1sNfpzgxQIDAQABo4ICfzCCAnswDgYDVR0PAQH/BAQD
AgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAMBgNVHRMBAf8EAjAA
MB0GA1UdDgQWBBSipG82qkK3/rCKWplRHy90ZLfRkzAfBgNVHSMEGDAWgBQULrMX
t1hWy65QCUDmH6+dixTCxjBVBggrBgEFBQcBAQRJMEcwIQYIKwYBBQUHMAGGFWh0
dHA6Ly9yMy5vLmxlbmNyLm9yZzAiBggrBgEFBQcwAoYWaHR0cDovL3IzLmkubGVu
Y3Iub3JnLzBOBgNVHREERzBFghRjb25zb2xlLmhpdmVtcS5jbG91ZIItZnJvbnRl
bmQuY29uc29sZS5wb3J0YWwuZXVjMS5hd3MuaGl2ZW1xLmNsb3VkMEwGA1UdIARF
MEMwCAYGZ4EMAQIBMDcGCysGAQQBgt8TAQEBMCgwJgYIKwYBBQUHAgEWGmh0dHA6
Ly9jcHMubGV0c2VuY3J5cHQub3JnMIIBBQYKKwYBBAHWeQIEAgSB9gSB8wDxAHYA
ejKMVNi3LbYg6jjgUh7phBZwMhOFTTvSK8E6V6NS61IAAAGF96yWKgAABAMARzBF
AiBWLoaT5ggQeiZJ8rKVRzxdmWpikXHKVnI/cA7lsklIwwIhAJsbs2CMrEGq3luD
QWYHQ6YoZwcGQ39EArLfT96Ev3KMAHcArfe++nz/EMiLnT2cHj4YarRnKV3PsQwk
yoWGNOvcgooAAAGF96yWTgAABAMASDBGAiEAqr69Mm++OVzHbDp27JZpeEpsMEyc
HUDSJYLDrhnNcJYCIQCRdAL467SmLnjcGmBpwVtXJt7QnGy8SB619K3W1cVGcTAN
BgkqhkiG9w0BAQsFAAOCAQEAZSPZlFmytxeyRqlJtszIAloa0OvASon+R12P5/i+
M7L3U5uzeUJuenbVZmkii6N6zc7DCHAZsJDvG/oTxOWxRTA5Luu/glc/+Kd1J7ba
mDN5zun2M2znVh1Wf0zw5l5DMzgHRfWdhgJ1aAgCQOtHs6gU/yOJnKpxw8Y9uwUp
Ybs1b8oh9SulGRkLGPFTlHk+rb5majDSLMhSBx8Gg/qAPG9V7i9ZMkwKLHl1TNP7
tWk4DHVCaKjTr2Q+ugBcYFCTd1+yPuESX+luwBf9jjQz9CXdToPIDSFQpkxZljRW
qAzJszgXplGxpycvzn5lGwljUZCR7x342pXBpgAF6KI4ag==
-----END CERTIFICATE-----  
)EOF";

//SDS011 PM Sensor
/* SDS011 Dust Sensor */
const int SDS_RX_PIN = D5; // D3 -> SDS011 TX pin
const int SDS_TX_PIN = D6; // D4 -> SDS011 TX pin
SoftwareSerial softwareSerial(SDS_RX_PIN, SDS_TX_PIN);
SdsDustSensor sds(softwareSerial); //  additional parameters: retryDelayMs and maxRetriesNotAvailable
const int MINUTE = 60000;
const int WAKEUP_WORKING_TIME = 30000; // 30 seconds.
const int MEASUREMENT_INTERVAL = 5 * MINUTE;


//Sound Sensor
const int sampleWindow = 50;                             
unsigned int sample;


void setup() {
  Serial.begin(115200);
  Serial.println(F("DHT22 test!"));

  dht.begin();
  sds.begin();
  pinMode (SENSOR_PIN, INPUT); // Set the signal pin as input  
  setupMQTT();
  connectWifi();
}

void loop() {
  if (!mqttClient.connected()){
    reconnect();
  }
  mqttClient.loop();
  if(WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }
  //DHT22 Sensor
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  //NOVA SDS011 PM Sensor
  // Wake up SDS011
  sds.wakeup();
  delay(WAKEUP_WORKING_TIME);
  // Get data from SDS011
  PmResult pm = sds.queryPm();
  float pm10 = -1;
  float pm25 = -1;
  if (pm.isOk()) {
  pm10 = pm.pm10;
  pm25 = pm.pm25;
  } else {
  Serial.print("Could not read values from sensor, reason: ");
  Serial.println(pm.statusToString());
  }
  WorkingStateResult state = sds.sleep();
//SOUND SENSOR
  unsigned long startMillis= millis();                   // Start of sample window
  float peakToPeak = 0;                                  // peak-to-peak level

  unsigned int signalMax = 0;                            //minimum value
  unsigned int signalMin = 1024;                         //maximum value

                                                        // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(SENSOR_PIN);                    //get reading from microphone
    if (sample < 1024)                                  // toss out spurious readings
    {
        if (sample > signalMax)
        {
          signalMax = sample;                           // save just the max levels
        }
        else if (sample < signalMin)
        {
          signalMin = sample;                           // save just the min levels
        }
    }
  }
  peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
  int db = map(peakToPeak,20,900,49.5,90);             //calibrate for deciBels
  Serial.print("Decibels: ");
  Serial.println(db);
  if(pm.isOk()) {
    mqtt_publish((char*)createJSON(db,temperature,humidity,pm10,pm25).c_str());
  }
}

// MQTT client

//    mqtt_publish((char*)createJSON(lightOn,humanPresence).c_str());
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
  mqttClient.setCallback(callback);
}

void mqtt_publish (char * data) {
  reconnect();
  if (mqttClient.publish(mqtt_topic, data)) {
    Serial.println("Published.");
  }
  else {
    Serial.println("Not published.");
  }
}

void mqtt_subscribe() {
  reconnect();
  if(mqttClient.subscribe(mqtt_topic)) {
    Serial.println("subscribed");
  }  
  else {
    Serial.println("not subscribed");
  }
}
static char msg[50];
void reconnect() {
  while (!mqttClient.connected()) {
      //BearSSL::X509List cert(ca_cert);
      //wifiClient.setTrustAnchors(&cert);
      //wifiClient.setFingerprint(fingerprint);
      wifiClient.setInsecure();
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      counter++;
      if(counter >= 5) {
        ESP.restart();
      }
      if (mqttClient.connect(clientId.c_str(),mqtt_username,mqtt_password)) {
        Serial.println("Connected.");
        mqttClient.subscribe(mqtt_topic);
      }
      else {
      Serial.print("last SSL Error = ");
      Serial.print(wifiClient.getLastSSLError(msg, 50));
      Serial.print(" : ");
      Serial.println(msg);
      }
  }

}

void connectWifi() {
  int counter = 0;
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;
    if(counter >= 30) {
      ESP.restart();
    }
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
String createJSON(int db,float temperature, float humidity,float pm10,float pm25) {
  Serial.println("creation");
  Serial.println("========");
  DynamicJsonDocument doc(1024);
  doc["noise"] = db;
  doc["temperature"] = (float) (((long) temperature*10) / 10);
  doc["humidity"] = (float) (((long) humidity * 10) / 10);
  doc["pm10"] = (float) (((long) pm10 * 10) / 10);
  doc["pm25"] = (float) (((long) pm25 * 10) / 10);
  String jsonString = "";
  serializeJson(doc,jsonString);
  Serial.println(jsonString);
  return jsonString;
}



