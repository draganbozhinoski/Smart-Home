// #include <Arduino_JSON.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
// Load Wi-Fi library
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>
#define windowPin 21
#define relayPin 22

const char* ssid     = "Dragan's Galaxy S21+ 5G";
const char* password = "urpv0342";
WiFiClientSecure wifiClient;
HTTPClient http;
const char* mqtt_username = "dragan";
const char* mqtt_password = "123456789Dd";
const char* mqtt_topic = "window";
//MQTT DEFINITIONS
PubSubClient mqttClient(wifiClient); 
char *mqttServer = "0e2b872d30104e009127c52455bcd6be.s2.eu.hivemq.cloud";
int mqttPort = 8883;
int counter = 0;

const char* test_root_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"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\n" \
"-----END CERTIFICATE-----\n";

bool windowOpen = false;
bool klimaOn = false;
bool lastWindowState = true;
bool lastKlimaState = true;

void setup() {
  Serial.begin(115200);
  delay(1000); 
  pinMode(windowPin, INPUT_PULLUP); // Sets the trigPin as an Output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
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
  windowOpen = digitalRead(windowPin);
  if(windowOpen && lastWindowState != windowOpen) { //pendzerot se otvoril, klimata se gasi
    klimaOn = false;
    mqtt_publish((char*)createJSON(windowOpen,klimaOn).c_str());
  }
  if(!windowOpen && lastWindowState != windowOpen) {
    mqtt_publish((char*)createJSON(windowOpen,klimaOn).c_str());
  }

  if(klimaOn && klimaOn != lastKlimaState) {
    digitalWrite(relayPin,LOW);   
    mqtt_publish((char*)createJSON(windowOpen,klimaOn).c_str()); 
  }
  else if(!klimaOn && klimaOn != lastKlimaState) {
    digitalWrite(relayPin,HIGH);
    mqtt_publish((char*)createJSON(windowOpen,klimaOn).c_str());
  }
  lastWindowState = windowOpen;
  lastKlimaState = klimaOn;
}
// MQTT client

//    mqtt_publish((char*)createJSON(lightOn,humanPresence).c_str());
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  setJSON(payload,length);
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

void reconnect() {
  while (!mqttClient.connected()) {
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
  }
}

void connectWifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  wifiClient.setCACert(test_root_ca);
}
void setJSON(byte* payload,unsigned int length) {
  char newArray[length+1];
  for(int i=0;i<length;i++) {
    newArray[i] = (char)payload[i];
  }
  DynamicJsonDocument doc(1024);
  deserializeJson(doc,newArray);
  if(doc["klimaOn"] == true) {
    klimaOn = true;
  }
  else if(doc["klimaOn"] == false) {
    klimaOn = false;
  }
  Serial.print("Klima state now: ");
  Serial.println(klimaOn);
}
String createJSON(bool windowOpen,bool klimaOn) {
  Serial.println("creation");
  Serial.println("========");
  DynamicJsonDocument doc(1024);
  doc["windowOpen"] = windowOpen?"true":"false";
  doc["klimaOn"] = klimaOn?"true":"false";
  String jsonString = "";
  serializeJson(doc,jsonString);
  Serial.println(jsonString);
  return jsonString;
}


