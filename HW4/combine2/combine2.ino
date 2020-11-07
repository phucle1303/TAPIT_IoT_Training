#include <ESP8266WiFi.h>

const int led = 16;
const int button = 0;
bool state=0;
bool stateLED = 0;

const char* ssid = "Minh Duc";
const char* pass = "V06 Di.ch";
const char* host = "192.168.1.8";

String check = "OFF";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode (led, OUTPUT);
  pinMode (button, INPUT);
  digitalWrite(led, HIGH);

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

  attachInterrupt(0, interrupt, FALLING);
}

void interrupt()
{
  delay(30);
  while (digitalRead(button) == 0);
  delay(30);
  state = 1;
  stateLED ^= 1;
}


void loop() {
  // put your main code here, to run repeatedly:

  if (state == 1)
  {
    if (stateLED == 1)
    {
      digitalWrite(led, LOW);
      check = "ON";
    }
    if (stateLED == 0)
    {
      digitalWrite(led, HIGH);
      check = "OFF";
    }
  }

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }

  String url = "/iot_source/status.php";
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1" + "\r\n" +
  "Host: " + String(host) + "\r\n" +
  "Connection: close\r\n\r\n");

  int _time = millis();

  while(!client.available())
  {
    delay(1);
    if(millis()-_time>5000)
    { //wait 3 seconds max for response
     Serial.println("No response.");
     client.stop();
     break;
    }
  }
  String line;
  while(client.available())
  {
    line = client.readStringUntil('\r'); //print response
    Serial.print(line);
  }
  if (line.indexOf("|1")!=-1)
  {
    digitalWrite(led, LOW);
    stateLED = 1;
  }
  if (line.indexOf("|0")!=-1)
  {
    digitalWrite(led, HIGH);
    stateLED = 1;
  }
  Serial.println();
  Serial.println("closing connection");


  if (state == 1)
  {
    String url = "/iot_source/control.php?LED=";
    url += check;
    Serial.print("Requesting URL: ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1" + "\r\n" +
    "Host: " + String(host) + "\r\n" +
    "Connection: close\r\n\r\n");

    int _time = millis();

    while(!client.available())
    {
      delay(1);
      if(millis()-_time>5000)
      { //wait 3 seconds max for response
        Serial.println("No response.");
        client.stop();
        break;
      }
    }
    state = 0;
    Serial.println();
    Serial.println("closing connection");
  }
}
