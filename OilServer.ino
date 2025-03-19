#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>


const char* ssid = "Access-Point";
const char* password = "12345678";

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 4800;

TinyGPSPlus gps;

String error="error";

SoftwareSerial ss(RXPin, TXPin);

AsyncWebServer server(80);

String readLat() {
  if (gps.location.isValid())
  {
    double readLatFloat=gps.location.lat();
    Serial.print(gps.location.lat(), 6);
    String readLatString=String (readLatFloat, 8);

    return readLatString;
  }
  else
  {
    return error;
  }
  
}

String readLong() {
   if (gps.location.isValid())
  {
    Serial.print(gps.location.lng(), 6);

    return String (gps.location.lng(), 8);
  }
  else
  {
    return error;
  }
}

void setup(){
  Serial.begin(115200);
  ss.begin(GPSBaud);
  Serial.println();
  
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/latitude", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readLat().c_str());
  });
  server.on("/longitude", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readLong().c_str());
  });

  server.begin();
}

void loop()