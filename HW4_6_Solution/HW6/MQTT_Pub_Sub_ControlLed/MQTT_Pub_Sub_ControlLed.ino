/* 
	Phuc Le
	Last modified: 9/2/2018
	
 **	Desc: It connects to an MQTT server then:
  - Once Press Button 0, publishes '0' or '1' in turn to the topic "Light".
  - subscribes to the topic "Light", printing out any messages
    it receives.
  - If the first character of the topic "Light" is '1', switch ON the ESP Led,
    else if '0' switch it off
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Update these with values suitable for your network.
#define ssid " "        //Wifi name
#define password " "    //Wifi password
#define mqtt_server "10.10.43.102"  //IP address
#define mqtt_topic_pub "Light"     
#define mqtt_topic_sub "Light"
const uint16_t mqtt_port = 1883;
char msg[] = "0";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(0,INPUT);
  pinMode(16,OUTPUT);
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
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
//This function is called when new messages arrive at the client.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char p=(char)payload[0];
  Serial.print((char)payload[0]);
   if(p=='1') 
  {   
    digitalWrite(16, LOW); 
    Serial.println(" Turn On LED! " );
    msg[0]='0';
  } 
  // if MQTT comes a 1, turn on LED on pin D2
  else if(p=='0')
  {
    digitalWrite(16, HIGH); 
    Serial.println(" Turn Off LED! " );
    msg[0]='1';
  }
  else Serial.println(" Khong hop le");
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(mqtt_topic_pub, "ESP_reconnected");
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
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if(digitalRead(0)==LOW){
    while(digitalRead(0)==LOW);
    delay(50);
    Serial.println("Publish message: ");
    Serial.println(msg);
    client.publish(mqtt_topic_pub, msg);
    if(msg[0]=='0') msg[0]='1';
    else msg[0] = '0';
  }
}   