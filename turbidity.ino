#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is conntec to the Arduino digital pin 4
#define ONE_WIRE_BUS D1

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "iotgyan";
const char* password = "iotgyan4u";

//Device credentials
#define ORG "3fdgib"
#define DEVICE_TYPE "Turbidity"
#define DEVICE_ID "54321"
#define TOKEN "87654321"
String command;
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);
void PublishData(float temp, float t);
void setup()
{
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  Serial.println("");
 }
  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
  sensors.begin();
}

void loop()
{
  sensors.requestTemperatures();
  Serial.print("Celsius temperature: ");
  // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  float tempc=sensors.getTempCByIndex(0);
  Serial.print(tempc);
  Serial.print(" - Fahrenheit temperature: ");
  float tempf=sensors.getTempFByIndex(0);
  Serial.println(tempf);
  delay(1000);
  float t= analogRead(A0);
  Serial.print("Value: ");
  Serial.println(t);
 PublishData(tempc, tempf, t);
}
void PublishData(float tempc, float tempf, float t)
 {
  if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"tc\":";
  payload += tempc;
  payload+="," "\"tf\":";
  payload += tempf;
  payload+="," "\"turbidity\":";
  payload += t;
  payload += "}}";
  Serial.print("Sending payload: ");
  Serial.println(payload);
if (client.publish(topic, (char*) payload.c_str()))
 {
 Serial.println("Publish ok");
 }
else
{
 Serial.println("Publish failed");
 }
 }
