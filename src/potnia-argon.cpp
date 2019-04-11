#include "application.h"
#include "Adafruit_seesaw.h"

// **** Pin definitions ****
int lightSensor3V3 = A4;
int lightSensor = A5;


// **** structs ****
struct rawData {
  float tempC;
  uint16_t capacitance;
  int lightLevel;
  int batteryIn;
};

// this is a struct instead of #defs so users can eventually change it
struct calibration { 
  uint16_t dryAirHumidity; // capacitance reading reported in dry air
  uint16_t waterHumidity; // capacitance reading reported when submerged in water
};

struct readyData {
  float tempF;
  int normLightLevel;
  uint16_t normCapacitance;
  float batteryVoltage;
};

// **** Globals ****
Adafruit_seesaw stemma;
char* error;
struct calibration usercal;

// **** Functions ****
void setup();
void loop();
void blinkOnOff(int, int, int);
void takeMeasurements(struct rawData *);
void normalize(struct rawData *, struct calibration *, struct readyData *);
void upload(struct readyData *);


void setup() {
  usercal = {
    .dryAirHumidity = 330,
    .waterHumidity = 1024
  };

  pinMode(D7, OUTPUT); // status LED

  pinMode(lightSensor3V3, OUTPUT);
  digitalWrite(lightSensor3V3, HIGH);

  Serial.begin(11200);

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

  struct rawData readings;
  takeMeasurements(&readings);

  struct readyData output;
  normalize(&readings, &usercal, &output);

  upload(&output);

  System.sleep(D7, RISING, 900); // sleep for 15m. Restart from setup on resume
}

/*
 * Blink a pin on and off `count` times, taking `duration` ms per transition.
 * TODO: consider using the RGB LED for error states instead:
 * https://docs.particle.io/reference/device-os/firmware/argon/#led-signaling
 */
void blinkOnOff(int pin, int duration, int count) {
  for (int x=0; x<count; x++) {
    digitalWrite(pin, HIGH);
    delay(duration);
    digitalWrite(pin, LOW);
    delay(duration);
  }
}

void takeMeasurements(struct rawData * data) {
  data->tempC = stemma.getTemp();
  data->capacitance = stemma.touchRead(0);
  data->lightLevel = analogRead(lightSensor);
  data->batteryIn = analogRead(BATT);
}

void normalize(struct rawData* data, struct calibration* calib, struct readyData* output) {
  // how do we normalize light level? should be linear with respect to some measurable quantity
  output->normLightLevel = data->lightLevel;

  // this will probably need some experimental calibration with a thermometer since the sensor sucks
  output->tempF = data->tempC*1.8 + 32;

  // again, no idea what should be done here
  output->normCapacitance = data->capacitance;

  // Battery ADC is between 806k and 2M resistors.  Computed constant. Source:
  // https://github.com/kennethlimcp/particle-examples/blob/master/vbatt-argon-boron/vbatt-argon-boron.ino
  output->batteryVoltage = (float) data->batteryIn * 0.0011224;
}

void upload(struct readyData* data) {
  Serial.print("Temperature: ");
  Serial.print(data->tempF);
  Serial.println("F");
  Serial.print("Light level: ");
  Serial.print(data->normLightLevel);
  Serial.println("/4096");
  Serial.print("Humidity: ");
  Serial.println(data->normCapacitance);
  Serial.print("Battery: ");
  Serial.print(data->batteryVoltage);
  Serial.println("V");

  Serial.println();

  char battVoltageStr[5];
  snprintf(battVoltageStr, 5, "%.2f", data->batteryVoltage);
  Particle.publish("battery", battVoltageStr, PRIVATE); // TODO: handle errors when publishing

  char tempStr[6];
  snprintf(tempStr, 6, "%.2f", data->tempF);
  Particle.publish("temperature", tempStr, PRIVATE);

  char lightStr[5];
  snprintf(lightStr, 5, "%.4d", data->normLightLevel);
  Particle.publish("light", lightStr, PRIVATE);

  char moistureStr[5];
  snprintf(moistureStr, 5, "%.4u", data->normCapacitance);
  Particle.publish("moisture", moistureStr, PRIVATE);

  delay(5000); // wait a bit to make sure these actually get published before sleeping
  Serial.println("Published Particle events.");
}