#include <Servo.h>
#define USE_SERIAL Serial

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

Servo lockServo;

int pos = 0;

ESP8266WiFiMulti WiFiMulti;

void setup() {
  USE_SERIAL.begin(115200);
  USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for(uint8_t t = 4; t > 0; t--) {
      USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
      USE_SERIAL.flush();
      delay(1000);
  }

  WiFiMulti.addAP("The Grove", "Inn0vate");

  // put your setup code here, to run once:
  lockServo.attach(2);
}

void loop() {
  USE_SERIAL.printf("Go to 0 degrees...\n");
  lockServo.write(0);              // tell servo to go to position in variable 'pos'
  delay(500);                       // waits 15ms for the servo to reach the position

  USE_SERIAL.printf("Go to 20 degrees...\n");
  lockServo.write(20);              // tell servo to go to position in variable 'pos'
  delay(2500);                       // waits 15ms for the servo to reach the position

  if((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    USE_SERIAL.print("[HTTP] begin...\n");
    http.begin("http://coinlocked.com/here_i_am"); //HTTP
    USE_SERIAL.print("[HTTP] GET...\n");
    int httpCode = http.GET();
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if(httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            USE_SERIAL.println(payload);
        }
    } else {
        USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
}
