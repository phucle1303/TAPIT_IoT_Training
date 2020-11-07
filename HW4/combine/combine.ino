#include <ESP8266WiFi.h>

const int led = 16;

const char* ssid = "Minh Duc";
const char* pass = "V06 Di.ch";
const char* host = "192.168.1.8";

String urlStatus = "/iot_source/status.php";
String urlControl = "/iot_source/control.php";
WiFiClient client;

unsigned long timeoutC;

boolean state = 0;

String check = " ";
String url = " ";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode (led, OUTPUT);

  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  attachInterrupt(digitalPinToInterrupt(0), interrupt, FALLING);
}

void interrupt()
{
  if (digitalRead(led))
    check = "?LED=ON";
  else
    check = "?LED=OFF";
  url = urlControl + check;
  state = 1;
  timeoutC = millis();
  Serial.println("starting create the HTTP reqeust to Control.php");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("connecting to ");
  Serial.println(host);
  
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) 
  {  
    Serial.println("connection failed");
    return;
  }
  if (state == 0)
  {
    url = urlStatus;
  }
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  
  unsigned long timeoutS = millis();
  while (client.available() == 0) 
  {
    if (millis() - timeoutS > 5000) 
    {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  while(client.available())
  {
    String line = client.readString();
    Serial.print(line);  
    if(line.indexOf("|1")  != -1) digitalWrite(led,LOW);
    if(line.indexOf("|0")  != -1) digitalWrite(led,HIGH);
  }
  
  Serial.println();
  Serial.println("closing connection");
  client.stop();

  //Wait for HTTP request send success
  if(state) 
  {
    if(millis() - timeoutC > 2000) 
    {
      state = 0; //Chuyển sang trạng thái request site status.php
    }
  }
}
