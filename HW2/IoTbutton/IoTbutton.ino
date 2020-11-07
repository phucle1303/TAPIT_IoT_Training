//TAPIT - Technology Training
//Modifier: Phuc Le

#include <ESP8266WiFi.h>
const char* ssid = "CoE-C1";
const char* password = "publicwifi";

const int led = 16;
const int button = 0;
bool check = 0;
WiFiServer server(80);  //Tạo một server có khả năng lắng nghe các kết nối từ client ở port 80
 
void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(led, HIGH);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) { //mã WL_CONNECTED = 3;
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin(); //Server bắt đầu lắng nghe các kết nối đến từ Client
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());           //Địa chỉ IP của NodeMCU
  Serial.println("/");
 
}
 
void loop() {
//   Check if a client has connected
  WiFiClient client = server.available(); //Khi có client kết nối đến server thì sẽ trả về một đối tượng Client

  // Change button state
  if (digitalRead(button) == 0)
  {
    delay(50);
    while (digitalRead(button) == 0);
    delay(50); 
    check^=1;
    Serial.println("Button pressed, refresh the page to see the light changes.");
  }
  
  if (!client) {                          //Nếu ko có client kết nối -> trả về false -> !client trả về true
    return; 
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){             // Server đang lắng nghe -> client.available() trả về số byte được Request từ Client
    delay(1);
  }
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
 
  client.print("Light bulb shows button state");
  client.println("<br><br>");
  if(check == 0) 
  {
    client.println("<img src=\"https://www.w3schools.com/js/pic_bulboff.gif\" width=\"100\" height=\"180\">");
    client.println("<br>");
    client.print("Light is off");
    digitalWrite(led, HIGH);
  } 
  else 
  {
    client.println("<img src=\"https://www.w3schools.com/js/pic_bulbon.gif\" width=\"100\" height=\"180\">");
    client.println("<br>");
    client.print("Light is on");
    digitalWrite(led, LOW);
  }
  client.println("<br><br>");
 
  Serial.println("Client disonnected"); 
  delay(1);
  Serial.println("");
}
 
