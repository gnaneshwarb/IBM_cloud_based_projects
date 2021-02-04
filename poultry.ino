#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define SSD1306_LCDHEIGHT 64 //128*64
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
#include "DHT.h"
#include <Servo.h>
Servo myservo;
#define  DHTPIN D0
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);
int temp_led=D7;
#define echopin D3
#define trigpin D4
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "iotgyan";
const char* password = "iotgyan4u";

#define ORG "3fdgib"
#define DEVICE_TYPE "Node"
#define DEVICE_ID "12345"
#define TOKEN "12345678"
String command;
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);
void PublishData(float t, float distance, float analogSensor, float h);
void setup()
{
  Serial.begin(9600);
  dht.begin();
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
  pinMode(temp_led,OUTPUT);
  pinMode(D3,INPUT);
  pinMode(D4,OUTPUT);
  myservo.attach(D5);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,10);
  display.print("poultry farm");
  Serial.println("poultry farm");
  display.display();
}

void loop()
{
 
  //gas sensor start
  int a = analogRead(A0);
  Serial.print("Ammonia Gas Value: ");
  Serial.println(a);
  //dht11 start
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  Serial.println(t);
  //ultrasoinc start
  digitalWrite(D4,HIGH);
  delay(1000);
  digitalWrite(D4,LOW);
  int duration = pulseIn(echopin,HIGH);
  int distance = (duration/2)*0.0343;
  Serial.println(distance);
  String dis;
  if(distance<4){
    dis="FULL";
  }
  else if(distance>10 && distance<25){
    dis="MEDIUM";
  }
  else if(distance>40){
    dis="LOW";
  }
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.print("Ammonia Gas: ");
  display.println(a);
  display.print("Temperature: ");
  display.println(t);
  display.print("Humidity: ");
  display.println(h);
  display.print("Water Level: ");
  display.println(dis);
  delay(500);
  if(a > 760){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,45);
    display.println("Gas Value is High");
    Serial.println("Gas Value is High");
  }
  else{
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,45);
    display.println("Gas Value in Control");  
    Serial.println("Gas Value in Control");
  }
  if(t > 27){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,55);
    display.println("Temperature is High");
    Serial.println("Temperature is High");
  }
  else{
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,55);
    display.println("Temperature in Control");
    Serial.println("Temperature in Control ");
  }
    display.display();
 PublishData(t, distance, a,h);
}
void PublishData(float t, float distance, float analogSensor, float h)
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
  String payload = "{\"d\":{\"t\":";
  payload += t;
  payload+="," "\"distance\":";
  payload += distance;
  payload+="," "\"analogSensor\":";
  payload += analogSensor;
  payload+="," "\"humidity\":";
  payload += h;
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
