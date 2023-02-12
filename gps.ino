#include <ESP8266WiFi.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;
SoftwareSerial ss(2, 3);

const char *ssid = "your-ssid";
const char *password = "your-password";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  ss.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  server.begin();
}

void loop() {
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();
    if (request.indexOf("/getLocation") != -1) {
      String response = "{\"latitude\":" + String(gps.location.lat(), 6) + ",\"longitude\":" + String(gps.location.lng(), 6) + "}";
      client.print("HTTP/1.1 200 OK\r\n");
      client.print("Content-Type: application/json\r\n");
      client.print("Connection: close\r\n");
      client.print("\r\n");
      client.print(response);
    } else {
      client.print("HTTP/1.1 404 Not Found\r\n");
      client.print("Connection: close\r\n");
      client.print("\r\n");
    }
    client.stop();
  }
}
