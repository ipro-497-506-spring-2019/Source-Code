#include "application.h"
#include "Adafruit_seesaw.h"

void setup();
void initializeSS();
void loop();

// **** Pin definitions ****

// **** Moisture sensor ****
Adafruit_seesaw stemma;
char* error;


void setup() {
  pinMode(D7, OUTPUT); // hello works


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
}
