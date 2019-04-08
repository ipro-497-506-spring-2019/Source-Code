#include "application.h"
#include "Adafruit_seesaw.h"

void setup();
void initializeSS();
void loop();

// **** Pin definitions ****
int stemmaGNDPin = D3;
int stemma3VPin = D2;
int stemmaSCLPin = D1;
int stemmaSDAPin = D0;

// **** Moisture sensor ****
Adafruit_seesaw ss;
bool initialized;


void setup() {
  pinMode(D7, OUTPUT);
  pinMode(stemmaGNDPin, OUTPUT);
  pinMode(stemma3VPin, OUTPUT);
  digitalWrite(stemmaGNDPin, LOW);
  digitalWrite(stemma3VPin, HIGH);
  pinMode(stemmaSCLPin, INPUT_PULLUP);
  pinMode(stemmaSDAPin, INPUT_PULLUP);

  Serial.begin(9600);
  initializeSS();
}

void initializeSS() {
  if (!ss.begin(0x36)) {
    Serial.println("Couldn't initialize.");
    initialized = false;
  } else {
    initialized = true;
  }
}

void loop() {
  // "it lives" status blink for testing
  digitalWrite(D7, HIGH);
  delay(500);
  digitalWrite(D7, LOW);
  delay(500);

  if (initialized) {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion());
    Serial.print("Temperature: "); Serial.print(ss.getTemp()); Serial.println("*C");
    Serial.print("Capacitive: "); Serial.println(ss.touchRead(0));
  } else {
    initializeSS();
  }
}