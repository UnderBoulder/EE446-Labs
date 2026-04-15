#include <PDM.h>
#include <Arduino_BMI270_BMM150.h>
#include <Arduino_APDS9960.h>

// ===== Mic =====
short sampleBuffer[256];
volatile int samplesRead = 0;
int micLevel = 0;

void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2;
}

// ===== Sensors =====
float ax, ay, az;
int clearLight = 0;
int proximity = 0;

// ===== Thresholds (tune these!) =====
const int SOUND_THRESH = 50;
const int LIGHT_THRESH = 5;
const float MOTION_THRESH = 1.2;
const int PROX_THRESH = 50;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Mic
  PDM.onReceive(onPDMdata);
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start mic");
    while (1);
  }

  // IMU
  if (!IMU.begin()) {
    Serial.println("Failed to init IMU");
    while (1);
  }

  // APDS
  if (!APDS.begin()) {
    Serial.println("Failed to init APDS");
    while (1);
  }
}

void loop() {

  // ===== MIC PROCESSING =====
  if (samplesRead) {
    long sum = 0;
    for (int i = 0; i < samplesRead; i++) {
      sum += abs(sampleBuffer[i]);
    }
    micLevel = sum / samplesRead;
    samplesRead = 0;
  }

  // ===== IMU =====
  float motion = 0;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    motion = sqrt(ax * ax + ay * ay + az * az);
  }

  // ===== LIGHT =====
  if (APDS.colorAvailable()) {
    int r, g, b;
    APDS.readColor(r, g, b);
    clearLight = (r + g + b) / 3;  // simple proxy
  }

  // ===== PROX =====
  if (APDS.proximityAvailable()) {
    proximity = APDS.readProximity();
  }

  // ===== BINARY FLAGS =====
  int sound = (micLevel > SOUND_THRESH);
  int dark = (clearLight < LIGHT_THRESH);
  int moving = (motion > MOTION_THRESH);
  int near = (proximity < PROX_THRESH);

  // ===== CLASSIFICATION =====
  String state = "UNKNOWN";

  if (!sound && !dark && !moving && !near) {
    state = "QUIET_BRIGHT_STEADY_FAR";
  }
  else if (sound && !dark && !moving && !near) {
    state = "NOISY_BRIGHT_STEADY_FAR";
  }
  else if (!sound && dark && !moving && near) {
    state = "QUIET_DARK_STEADY_NEAR";
  }
  else if (sound && !dark && moving && near) {
    state = "NOISY_BRIGHT_MOVING_NEAR";
  }

  // ===== PRINTING =====
  Serial.print("raw,");
  Serial.print("mic="); Serial.print(micLevel);
  Serial.print(",clear="); Serial.print(clearLight);
  Serial.print(",motion="); Serial.print(motion);
  Serial.print(",prox="); Serial.println(proximity);

  Serial.print("flags,");
  Serial.print("sound="); Serial.print(sound);
  Serial.print(",dark="); Serial.print(dark);
  Serial.print(",moving="); Serial.print(moving);
  Serial.print(",near="); Serial.println(near);

  Serial.print("state,");
  Serial.println(state);

  delay(200);
}
