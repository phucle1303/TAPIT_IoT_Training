#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Update these with values suitable for your network.
#define ssid "Minh Duc"
#define password "V06 Di.ch"
#define mqtt_server "192.168.1.10"
#define mqtt_topic_pub "/Light/Two"
#define mqtt_topic_sub "/Light/Two"
const uint16_t mqtt_port = 1883;
char* check;
bool state = 0;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() 
{
  Serial.begin(115200);
  pinMode(16,OUTPUT);
  digitalWrite(16,HIGH);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
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

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String p = "";
  for (int i=0; i<length; i++)
  {
    p += (char)payload[i];
  }
  Serial.print(p);
  Serial.println();
  if(p=="L2ON") 
  {
    digitalWrite(16, LOW); 
    Serial.println(" LED is on " );
    state = 1;
  } 
  // if MQTT comes a 1, turn on LED on pin D2
  else if(p=="L2OFF")
  {
    digitalWrite(16, HIGH); 
    Serial.println(" LED is off " );
    state = 0;
  }
  else 
    Serial.println(" Khong hop le");
  Serial.println();
}

void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(mqtt_topic_pub, "ESP_reconnected, press button to see change");
      // ... and resubscribe
      client.subscribe(mqtt_topic_sub);      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() 
{
  if (!client.connected()) 
  {
    reconnect();
  }
  
  client.loop();
  
  if (!digitalRead(0))
  {
    delay(50);
    while (!digitalRead(0));
    delay(50);
    state ^= 1;
    if (state == 1)
    {
      check = "L2ON";
    }
    else
    {
      check = "L2OFF";
    }
    client.publish(mqtt_topic_pub, check);
  }
}
