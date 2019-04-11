#include "application.h"
#include "Adafruit_seesaw.h"

void setup();
void initializeSS();
void loop();

// **** Pin definitions ****
int lightSensor3V3 = A4;
int lightSensor = A5;

// **** Moisture sensor ****
Adafruit_seesaw stemma;
char* error;


void setup() {
  pinMode(D7, OUTPUT); // hello world

  pinMode(lightSensor3V3, OUTPUT);
  digitalWrite(lightSensor3V3, HIGH);

  Serial.begin(9600);

  if (!stemma.begin(0x36)) {
    error = "Couldn't initialize stemma!";
  }
}

void loop() {
  if (error) {
    digitalWrite(D7, HIGH);
    delay(50);
    digitalWrite(D7, LOW);
    delay(50);
    digitalWrite(D7, HIGH);
    delay(50);
    digitalWrite(D7, LOW);
    delay(50);
    Serial.println(error);
    return;
  }


  // "it lives" status blink for testing
  digitalWrite(D7, HIGH);
  delay(500);
  digitalWrite(D7, LOW);
  delay(500);

  Serial.print("seesaw started! version: ");
  Serial.println(stemma.getVersion());
  Serial.print("Temperature: "); Serial.print(stemma.getTemp()); Serial.println("*C");
  Serial.print("Capacitive: "); Serial.println(stemma.touchRead(0));
  Serial.print("Light:"); Serial.println(analogRead(lightSensor));
}
