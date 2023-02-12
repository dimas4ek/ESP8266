#include <ESP8266WiFi.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

const int ledPin = 2; 

WiFiServer server(80);

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("New client connected");
  while (client.connected()) {
    if (client.available()) {
      String req = client.readStringUntil('\r');
      Serial.println(req);
      client.flush();
      int ledStatus = 0;
      if (req.indexOf("/LED=ON") != -1) {
        digitalWrite(ledPin, HIGH);
        ledStatus = 1;
      }
      if (req.indexOf("/LED=OFF") != -1) {
        digitalWrite(ledPin, LOW);
        ledStatus = 0;
      }
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<html><body><h1>LED is now " + (ledStatus ? "ON" : "OFF") + "</h1></body></html>");
      break;
    }
  }
  client.stop();
  Serial.println("Client disconnected");
}
