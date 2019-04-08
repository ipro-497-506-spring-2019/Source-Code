#include "application.h"
#include "Adafruit_seesaw.h"

void setup();
void initializeSS();
void loop();

Adafruit_seesaw ss;
bool initialized;
int stemmaGNDPin = D3;
int stemma3VPin = D2;
int stemmaSCLPin = D1;
int stemmaSDAPin = D0;

void setup() {
  Serial.begin(9600);
  pinMode(D7, OUTPUT);
  pinMode(stemmaGNDPin, OUTPUT);
  pinMode(stemma3VPin, OUTPUT);
  digitalWrite(stemmaGNDPin, LOW);
  digitalWrite(stemma3VPin, HIGH);
  pinMode(stemmaSCLPin, INPUT_PULLUP);
  pinMode(stemmaSDAPin, INPUT_PULLUP);
  initializeSS();
}

void initializeSS() {
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    initialized = false;
  } else {
    initialized = true;
  }
}

void loop() {
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