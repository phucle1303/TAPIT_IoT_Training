#include <ESP8266WiFi.h>

const int led = 16;
const int button = 0;
bool state=0;
int a;

const char* ssid = "CoE-C1";
const char* pass = "publicwifi";
const char* host = "10.10.40.204";

String check;

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
}

void loop() {
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
    if(millis()-_time>3000)
    { //wait 3 seconds max for response
      Serial.println("No response.");
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
      Serial.print("on");
    }
  if (line.indexOf("|0")!=-1)
    {
      digitalWrite(led, HIGH);
      Serial.print("off");
    }

  client.stop();
  Serial.println("closing connection"); 
}
