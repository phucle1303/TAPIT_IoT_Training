/*
 Lê Công Vĩnh Khải - TAPIT Technology Training
 Desc:
 + NodeMCU sẽ là WiFiClient gửi HTTP request đến Xampp Server để đọc trạng thái của site status.php để thay đổi trạng thái của LED GPIO16. 
 + Khi nhấn nút GPIO0 thì sẽ thay đổi trạng thái của LED GPIO16 đồng thời gửi HTTP request đến site control.php ở Xampp Server để thay đổi trạng thái của table 'led'
 */
 
#include <ESP8266WiFi.h>
const char* ssid     = "TP-Link_D70A";
const char* password = "24397074";
const char* host = "192.168.0.177";  //Laptop's IP Address
const String controlSite = "/iot_source/control.php";
const String statusSite = "/iot_source/status.php";
const byte interruptPin = 0;
unsigned long controlTimeout;
String url = "";
String Req = "";
boolean controlState = false;
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
void setup() {
  pinMode(16,OUTPUT);
  Serial.begin(9600);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
}

void handleInterrupt(){
  if(digitalRead(16)){
    Req = "?LED=ON"; //Nếu Led đang OFF thì Request LED=ON
  }
  else Req = "?LED=OFF"; //Nếu Led đang ON thì Request LED=OFF
  url = controlSite + Req;
  controlState = true;  //Chuyển sang trạng thái request site control.php
  controlTimeout = millis();
  Serial.println("starting create the HTTP reqeust to Control.php");
}

void loop() {
  Serial.print("connecting to ");
  Serial.println(host);
  
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {  
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  if(!controlState) url = statusSite;   //Request site status.php
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
               
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readString();
    Serial.print(line);  
    if(line.indexOf("|1")  != -1) digitalWrite(16,LOW);
    if(line.indexOf("|0")  != -1) digitalWrite(16,HIGH);
  }
  Serial.println();
  Serial.println("closing connection");
  client.stop();

  //Wait for HTTP request send success
  if(controlState) {
    if(millis() - controlTimeout > 2000) {
    controlState = false; //Chuyển sang trạng thái request site status.php
    }
  }
}
