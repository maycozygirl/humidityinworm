void Line_Notify1(String txt1);
void Line_Notify1(String txt2);

#include <WiFi.h>
#include <WiFiClientSecure.h>

char ssid[] = "Chaiake";
char pass[] = "thep3379"; 

#define TokenLine "BVkALNbXwWtLJHHISpim2dFJ0PNWQKwePcKF987WAlL"

String txt1 = "%E0%B8%84%E0%B8%A7%E0%B8%B2%E0%B8%A1%E0%B8%8A%E0%B8%B7%E0%B9%89%E0%B8%99%E0%B9%80%E0%B8%97%E0%B9%88%E0%B8%B2%E0%B8%81%E0%B8%B1%E0%B8%9A "; 
String txt2 = " %E0%B8%A3%E0%B8%94%E0%B8%99%E0%B9%89%E0%B8%B3%20!"; 

int sensorPin = 34;


void setup() {

      //Get Input from  Pin
      pinMode(sensorPin, INPUT);
      Serial.begin(115200);
      //Connect to wifi
      WiFi.begin(ssid, pass);
      //Show "WiFi Connecting" if connect
      Serial.print("WiFi Connecting");

      //Check wifi status
      while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
      } 
      
      //show wifi that connect
      Serial.println();
      Serial.print("connected: ");
      Serial.println(WiFi.localIP());
}


void loop() {

      int Humidity = analogRead(sensorPin);

      if (Humidity >= 900 && Humidity < 3000 ) {
         Serial.print("Moisture Sensor Value:");
         Serial.println(Humidity);
         NotifyLine(txt1 + Humidity + txt2);
       }
      else if (Humidity < 900 ) {
         Serial.print("Moisture Sensor Value:");
         Serial.println(Humidity);
       }

      delay(2000);  
}


//Notifyline
void NotifyLine(String t) {
     WiFiClientSecure client;
     client.setInsecure();
     if (!client.connect("notify-api.line.me", 443)) {
         Serial.println("Connection failed");
         delay(200);
         return;
        }
        
      String req = "";
      req += "POST /api/notify HTTP/1.1\r\n";
      req += "Host: notify-api.line.me\r\n";
      req += "Authorization: Bearer " + String(TokenLine) + "\r\n";
      req += "Cache-Control: no-cache\r\n";
      req += "User-Agent: ESP32\r\n";
      req += "Content-Type: application/x-www-form-urlencoded\r\n";
      req += "Content-Length: " + String(String("message=" + t).length()) + "\r\n";
      req += "\r\n";
      req += "message=" + t;
      
      Serial.println(req);
      client.print(req);
      delay(20);
      Serial.println("-------------");
      while (client.connected()) {
          String line = client.readStringUntil('\n');
       if (line == "\r") {
       break;
      }
   } //while
} //loop
