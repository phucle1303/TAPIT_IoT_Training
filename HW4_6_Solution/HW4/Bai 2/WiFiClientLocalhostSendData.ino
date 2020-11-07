/*
 Lê Công Vĩnh Khải - TAPIT Technology Training
 Desc:
 + Khi nhấn nút GPIO0 thì sẽ đảo trạng thái LED GPIO16, đồng thời sẽ là WiFiClient gửi HTTP request đến site control.php ở Xampp Server để thay đổi trạng thái và lưu vào database.
 */
#include <ESP8266WiFi.h>
const char* ssid     = "TP-Link_D70A";
const char* password = "24397074";
const char* host = "192.168.0.177";  //Laptop's IP Address
  
String Req="";
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
void setup() {
  pinMode(0,INPUT_PULLUP);
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
}
void loop() {
  boolean connectState = false;
  if(!digitalRead(0)) {
    delay(100);
    while(!digitalRead(0));
    if(digitalRead(16)) {
      digitalWrite(16, LOW);
      Req = "?LED=ON";
    }
    else {
      digitalWrite(16, HIGH);
      Req = "?LED=OFF";    
    }
    Serial.println("Starting create the HTTP request");
    connectState = true;
    }

  while(connectState) {
  Serial.print("connecting to ");
  Serial.println(host);
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {  
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/iot_source/control.php";
         url += Req; 
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: c lose\r\n\r\n");
               
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      connectState = false;
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readString();
    Serial.print(line);  
  }
  Serial.println();
  Serial.println("closing connection");
  client.stop();
  connectState = false;
  }
}