#include "application.h"
#include "Adafruit_seesaw.h"

void setup();
void loop();
void blinkOnOff(int, int, int);

// **** Pin definitions ****
int lightSensor3V3 = A4;
int lightSensor = A5;

Adafruit_seesaw stemma;
char* error;

struct rawDatapoint {
  float tempC;
  uint16_t capacitance;
  int lightLevel;
} rawData;


void setup() {
  pinMode(D7, OUTPUT); // status LED

  pinMode(lightSensor3V3, OUTPUT);
  digitalWrite(lightSensor3V3, HIGH);

  Serial.begin(9600);

  if (!stemma.begin(0x36)) {
    error = "Couldn't initialize stemma!";
  }
}

void loop() {
  if (error) {
    blinkOnOff(D7, 50, 20);
    Serial.println(error);
    // TODO: report this error to the cloud
    return;
  }

  blinkOnOff(D7, 500, 1); // "alive and well" blink

  Serial.print("seesaw started! version: ");
  Serial.println(stemma.getVersion());
  Serial.print("Temperature: "); Serial.print(stemma.getTemp()); Serial.println("*C");
  Serial.print("Capacitive: "); Serial.println(stemma.touchRead(0));
  Serial.print("Light:"); Serial.println(analogRead(lightSensor));
}

/*
 * Blink a pin on and off `count` times, taking `duration` ms per transition.
 */
void blinkOnOff(int pin, int duration, int count) {
  for (int x=0; x<count; x++) {
    digitalWrite(pin, HIGH);
    delay(duration);
    digitalWrite(pin, LOW);
    delay(duration);
  }
}