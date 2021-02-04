#include <Servo.h>
int servoPin = 5;
Servo s;
#define echoPin 2
#define trigPin 3
long duration;
int d; //distance
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(7,OUTPUT); //motors
  pinMode(8,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);//motors
  pinMode(13,OUTPUT);//buzzer
  pinMode(9,INPUT);
  pinMode(10,INPUT);//flame sensor
  s.attach(servoPin);
  s.write(90);
  Serial.begin(9600);
}

void loop(){
  Serial.print("distance");
  Serial.println(d);
  s.write(90);
  int r=0;
  int l=0;
  flame();
  if(d<=50){
    stop1();
    backward();
    delay(500);
    stop1();
    Serial.print("distance");
    Serial.println(d);
    s.write(50);
    ultrasonic();
    delay(500);
    r=d;
    Serial.print("right");
    Serial.println(r);
    s.write(140);
    ultrasonic();
    delay(500);
    l=d;
    Serial.print("left");
    Serial.println(l);
    ultrasonic();
    delay(500);
    if(l<40){
      right();
      Serial.println("right turn");
      delay(1000);
      stop1();
    }
    else if(r<40){
      left();
      Serial.println("left turn");
      delay(1000);
      stop1();
    }
  }
  else{
    forward();
  }
  s.write(90);
  ultrasonic();
  delay(500);
}
void flame(){
  int l=digitalRead(9);
  int r=digitalRead(10);
  Serial.print("Flame value left: ");
  Serial.println(l);
  Serial.print("Flame value right: ");
  Serial.println(r);
  if(l == LOW || r == LOW){
    digitalWrite(13,HIGH);
    stop1();
    Serial.println("High flame");
  }
  else{
      digitalWrite(13,LOW);
      Serial.println("No flame");
  }
}
void ultrasonic(){
  d=0;
  duration=0;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  d = duration * 0.034 / 2;
}
void forward(){
    digitalWrite(7,HIGH);
    digitalWrite(8,LOW);
    digitalWrite(11,HIGH);
    digitalWrite(12,LOW);
}
void backward(){
    digitalWrite(7,LOW);
    digitalWrite(8,HIGH);
    digitalWrite(11,LOW);
    digitalWrite(12,HIGH);  
}
void right(){
    digitalWrite(7,LOW);
    digitalWrite(8,LOW);
    digitalWrite(11,HIGH);
    digitalWrite(12,LOW);
}
void left(){
    digitalWrite(7,HIGH);
    digitalWrite(8,LOW);
    digitalWrite(11,LOW);
    digitalWrite(12,LOW);  
}
void stop1(){
    digitalWrite(7,LOW);
    digitalWrite(8,LOW);
    digitalWrite(11,LOW);
    digitalWrite(12,LOW);
}
