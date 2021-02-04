#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {
0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetServer server(80);

void setup() {
pinMode(13, OUTPUT);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
digitalWrite(3 ,HIGH);
digitalWrite(4 ,HIGH);
digitalWrite(5 ,HIGH);
Ethernet.begin(mac);
server.begin();
Serial.begin(9600);
Serial.println("Server address:");
Serial.println(Ethernet.localIP());
}

void loop() {
EthernetClient client = server.available();
if (client){
boolean currentLineIsBlank = true;
String buffer = ""; 
while (client.connected()){
if (client.available()) {
char c = client.read(); 
buffer+=c; 
if (c == '\n' && currentLineIsBlank) {
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println(); 
client.print("<center><br><h1>Ethernet based Home Automation</h1><br><br><br><FORM>");
client.print("<P> <INPUT type=\"submit\" name=\"status\" value=\"S1 ON\">");
client.print("<P> <INPUT type=\"submit\" name=\"status\" value=\"S1 OFF\">");
client.print("<P> <INPUT type=\"submit\" name=\"status\" value=\"S2 ON\">");
client.print("<P> <INPUT type=\"submit\" name=\"status\" value=\"S2 OFF\">");
client.print("<P> <INPUT type=\"submit\" name=\"status\" value=\"S3 ON\">");
client.print("<P> <INPUT type=\"submit\" name=\"status\" value=\"S3 OFF\">");
client.print("</FORM></center>"); 
break;
}
if (c == '\n') { 
currentLineIsBlank = true;
buffer=""; 
} 
else 
if (c == '\r') { 
if(buffer.indexOf("GET /?status=S1+ON")>=0)
digitalWrite(13, LOW);


if(buffer.indexOf("GET /?status=S1+OFF")>=0)
digitalWrite(13, HIGH); 


if(buffer.indexOf("GET /?status=S2+ON")>=0)
digitalWrite(4, LOW);

if(buffer.indexOf("GET /?status=S2+OFF")>=0)
digitalWrite(4, HIGH); 


if(buffer.indexOf("GET /?status=S3+ON")>=0)
digitalWrite(5, LOW);


if(buffer.indexOf("GET /?status=S3+OFF")>=0)
digitalWrite(5, HIGH); 

}
else {
currentLineIsBlank = false;
} 
}
}
client.stop();
}
}
