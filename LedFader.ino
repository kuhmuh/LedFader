#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "RgbLeds.h"
#include "credentials.h"

#define DBG_OUTPUT_PORT Serial

const char* ssid = STASSID;
const char* password = STAPSK;
const char* host = "fader";

ESP8266WebServer server(80);

const String rootTemplate = "<html>\
  <head>\
    <title>ESP8266 Web Server POST handling</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Almighty Fader App</h1><br>\
    <form method='post' enctype='application/x-www-form-urlencoded' action='/setLevel/'>\
      <label for='ratioR'>R</label>\
      <input type='number' id='ratioR' name='ratioR' min='0' max='255' value='{redLevel}'><br />\
      <label for='ratioG'>G</label>\
      <input type='number' id='ratioG' name='ratioG' min='0' max='255' value='{greenLevel}'><br />\
      <label for='ratioB'>B</label>\
      <input type='number' id='ratioB' name='ratioB' min='0' max='255' value='{blueLevel}'><br />\
      <input type='submit' value='Submit'>\
    </form>\
    <form method='post' enctype='application/x-www-form-urlencoded' action='/startFading/'>\
      <input type='number' id='speed' name='speed' min='10' max='1000'><br />\
      <input type='submit' value='Start'>\
    </form>\
  </body>\
</html>";

int brightness = 0;  // how bright the LED is
int offset = 5;      // how many points to fade the LED by
bool fading = false;
long previousMillis = 0;
int interval = 50;

RgbLeds leds = RgbLeds(D3, D4, D5);

void handleFade(void) {
  if (!fading) {
    return;
  }

  unsigned long currentMillis = millis();
  if (currentMillis < previousMillis + interval) {
    return;
  }

  previousMillis = currentMillis;

  unsigned long nextLevel = brightness + offset;
  if (nextLevel > 255) {
    nextLevel = 255;
    offset = -1 * offset;
  }
  if (nextLevel < 0) {
    nextLevel = 0;
    offset = -1 * offset;
  }
  leds.setLevels(nextLevel, nextLevel, nextLevel);
  leds.updateView();
  brightness = nextLevel;
}

void handleRoot() {
  String result = rootTemplate;
  result.replace("{redLevel}", String(leds.getRedLevel()));
  result.replace("{greenLevel}", String(leds.getGreenLevel()));
  result.replace("{blueLevel}", String(leds.getBlueLevel()));
  server.send(200, "text/html", result);
}

void handlePlain() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
    server.sendHeader("Location", String("/"), true);
    server.send(301, "text/plain", "");
    int newLevelR = server.arg("ratioR").toInt();
    int newLevelG = server.arg("ratioG").toInt();
    int newLevelB = server.arg("ratioB").toInt();
    if (isValid(newLevelR)) {
      fading = false;
      leds.setLevels(newLevelR, newLevelG, newLevelG);
      leds.updateView();
    }
  }
}

bool isValid(int level) {
  return level >= 0 && level <= 255;
}

void startFading() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
    server.sendHeader("Location", String("/"), true);
    server.send(301, "text/plain", "");
    int speed = server.arg("speed").toInt();
    if (speed >= 10 && speed <= 1000) {
      interval = speed;
    }
    DBG_OUTPUT_PORT.println("start fading");
    fading = true;
  }
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  DBG_OUTPUT_PORT.begin(115200);
  WiFi.begin(ssid, password);
  DBG_OUTPUT_PORT.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DBG_OUTPUT_PORT.print(".");
  }

  ////////////////////////////////
  // MDNS INIT
  if (MDNS.begin(host)) {
    MDNS.addService("http", "tcp", 80);
  }

  server.on("/", handleRoot);
  server.on("/setLevel/", handlePlain);
  server.on("/startFading/", startFading);
  server.onNotFound(handleNotFound);

  server.begin();
  leds.init(pinMode, analogWrite);
}

void loop(void) {
  server.handleClient();
  MDNS.update();
  handleFade();
}
