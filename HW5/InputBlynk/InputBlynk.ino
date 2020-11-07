  //A simplified version of work begun by Tzapu and chriscook8
  //for saving local WiFi SSID and Passwords to the ESP8266.
  //Uses ESP8266 internal memory instead of the problematic EEPROM.
  //To input local SSID and Password, connect to WiFi "ESP8266",
  //and point your browser to "192.168.4.1".
  
  #include "ESP8266WiFi.h"
  #include <WiFiClient.h>
  #include <BlynkSimpleEsp8266.h>
  
  WiFiServer server(80);
  const char* APssid = "NodeMCU_Rocky";                    // Name of access point                    
  const char* APpass = "12345678";                          // Pass of access point
  String rsid;
  String rpass;
  String rauth;
  boolean newSSID = false;

  int virtualPin;
  int flag=0;
  //Gửi dữ liệu trạng thái Button từ Blynk về Nodemcu
  BLYNK_WRITE(V0)
  {
    virtualPin = param.asInt(); // assigning incoming value from pin V1 to a variable
    Serial.print("V0 Slider value is: ");
    Serial.println(virtualPin);
    flag = 1;
  }

  //Đồng bộ lại trạng thái khi mất nguồn
  BLYNK_CONNECTED()
  {
    Blynk.syncVirtual(V0);  
  }
  
  void setup() 
  {    
    Serial.begin(115200);                                    // for serial test
    delay(10);
    if (testWifi())                                          /*--- if the stored SSID and password connected successfully, exit setup ---*/
    { 
        return;
    }
    else                                                    /*--- otherwise, set up an access point to input SSID and password  ---*/
    {
        Serial.println("");
        Serial.println("Connect timed out, opening AP"); 
        setupAP();
    }
  } //end setup
  
  void loop() 
  {
    // put your main code here, to run repeatedly:
    Blynk.run();
  
  if(flag == 1){
  if(virtualPin == 0){
    digitalWrite(16, LOW);
    }
    else digitalWrite(16, HIGH);
  }
  
  if(digitalRead(0) == LOW){
    while(digitalRead(0) == LOW);
    int ledStatus = digitalRead(16);
    if(ledStatus == 0){
      digitalWrite(16, HIGH);
      Blynk.virtualWrite(V0,1); //Ghi dữ liệu từ Nodemcu lên Blynk
      //led1.off();
      }
      else {
      digitalWrite(16, LOW);
      Blynk.virtualWrite(V0,0); ////Ghi dữ liệu từ Nodemcu lên Blynk
     // led1.on();
      }
    }
  flag = 0;
  }
  
  
  /*--- auto connect with stored wifi ---*/
  int testWifi(void)    
  {
    int c = 0;
    Serial.println("Waiting for Wifi to connect");  
    while ( c < 20 ) 
    {
      if (WiFi.status() == WL_CONNECTED) 
      {
        Serial.println("WiFi connected.");
        Serial.println(WiFi.localIP());
        return(1); 
      }      
      delay(500);
      Serial.print(WiFi.status());    
      c++;
    }
    return(0);
  } //end testwifi
  
  
  void setupAP(void) 
  {
    WiFi.mode(WIFI_STA);                                      //mode STA 
    WiFi.disconnect();                                        //disconnect to scan wifi
    delay(100);
   
    Serial.println("");                     
    delay(100);
    WiFi.softAP(APssid, APpass);                           //change to AP mode with AP ssid and APpass
    Serial.println("softAP");
    Serial.println("");
    launchWeb();                                         //?  
  }
  
  void launchWeb() 
  {
      Serial.println("");
      Serial.println(WiFi.softAPIP());
      server.begin();                                             // Start the server
      Serial.println("Server started");   
      int b = 20;
      int c = 0;
      while(b == 20) 
      { 
         b = mdns1();                                      //mdns1(function: web interface, read local IP SSID, Pass)
         /*--- If a new SSID and Password were sent, close the AP, and connect to local WIFI ---*/
         if (newSSID == true)
         {
            newSSID = false;
            /*--- convert SSID and Password string to char ---*/
            char ssid[rsid.length()];
            rsid.toCharArray(ssid, rsid.length());         
            char pass[rpass.length()];
            rpass.toCharArray(pass, rpass.length());
            char auth[rauth.length()];
            rauth.toCharArray(auth, rauth.length());
  
            Serial.println("Connecting to local Wifi");
            delay(500);
            WiFi.softAPdisconnect(true);                      //disconnet APmode
            delay(500);
            WiFi.mode(WIFI_STA);                              //change to STA mode
            //WiFi.begin(ssid,pass);                            //connect to local wifi
            Blynk.begin(auth, ssid, pass);
            delay(1000);
      ///set -> check -> set
            if ( testWifi())                                  //test connect
            {
              return;
            }
           else                                               //if wrong ssid or pass
           {
              Serial.println("");
              Serial.println("New SSID or Password failed. Reconnect to server, and try again.");
              setupAP();
              return;
           }
        }
    }
  }
  
  int mdns1()
  {
    // Check for any mDNS queries and send responses
    // Check if a client has connected                    //server mode
    WiFiClient client = server.available();               //check client
    if (!client) {
      return(20);
    }
    Serial.println("");
    Serial.println("New client");
  
    // Wait for data from client to become available
    while(client.connected() && !client.available()){    
      delay(1);
     }
    
    // Read the first line of HTTP request
    String req = client.readStringUntil('\r');
    
    // First line of HTTP request looks like "GET /path HTTP/1.1"
    // Retrieve the "/path" part by finding the spaces
    int addr_start = req.indexOf(' ');
    int addr_end = req.indexOf(' ', addr_start + 1);
    if (addr_start == -1 || addr_end == -1) {
      Serial.print("Invalid request: ");
      Serial.println(req);
      return(20);
     }
    req = req.substring(addr_start + 1, addr_end);
    Serial.print("Request: ");
    Serial.println(req);
    client.flush(); 
    String s;
        if (req == "/")
        {
          IPAddress ip = WiFi.softAPIP();
          String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
          s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>TAPIT_IoTs - WIFI - ESP8266<br/>";
          s += "IP: ";
          s += ipStr;
          s += "<p>";
          //s += st;
          s += "<form method='get' action='a'>";
          s += "<table><tr><td>SSID:</td><td><input name='ssid' length=32></td> </tr><tr><td>PASS:</td><td><input name='pass' length=64></td> </tr>";
          s += "<tr><td>AUTH Code:</td><td><input name='auth' length=128></td> </tr></table>"; 
          s += "<input type='submit' style='left: 160px; position: relative;'></form>";
          s += "</html>\r\n\r\n";
          Serial.println("Sending 200");
        }
        else if ( req.startsWith("/a?ssid=") ) 
        {
          newSSID = true;
          String qsid; //WiFi SSID 
          qsid = urldecode(req.substring(8,req.indexOf('&')).c_str()); //correct coding for spaces as "+"
          Serial.println(qsid);
          rsid = qsid;
          
          String qpass; //Wifi Password
          qpass = urldecode(req.substring(req.indexOf('&')+6,req.lastIndexOf("&auth=")).c_str());//correct for coding spaces as "+"
          Serial.println(qpass);
          rpass = qpass;

          String qauth; //Auth Code
          qauth = urldecode(req.substring(req.indexOf("&auth=")+6,req.lastIndexOf(' ')).c_str());//correct for coding spaces as "+"
          Serial.println(qauth);
          rauth = qauth;
          
          s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>TAPIT_IoTs - WIFI - ESP8266";
          s += "<p> New SSID, Password and Auth Code accepted</html>\r\n\r\n"; 
        }
        else
        {
          s = "HTTP/1.1 404 Not Found\r\n\r\n";
          Serial.println("Sending 404");
        }
    client.print(s);
    Serial.println("Done with client");
    return(20);
  }  
  
  String urldecode(const char *src)
  {
    String decoded = "";
    char a, b;
    while (*src) {     
      if ((*src == '%') && ((a = src[1]) && (b = src[2])) && (isxdigit(a) && isxdigit(b))) {      
        if (a >= 'a')
          a -= 'a'-'A';       
        if (a >= 'A')                
          a -= ('A' - 10);                   
        else               
          a -= '0';                  
        if (b >= 'a')                
          b -= 'a'-'A';           
        if (b >= 'A')                
          b -= ('A' - 10);            
        else                
          b -= '0';                        
        decoded += char(16*a+b);            
        src+=3;        
      } 
      else if (*src == '+') {
        decoded += ' ';           
        *src++;       
      }  
      else {
        decoded += *src;           
        *src++;        
      }    
    }
    decoded += '\0';        
    return decoded;
  }
