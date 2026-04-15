#include <Arduino_HS300x.h> 
#include <Arduino_BMI270_BMM150.h>
#include <Arduino_APDS9960.h>

// ===== Sensor values =====
float humidity = 0;
float temperature = 0;
float mag = 0;
int r, g, b, clear;

// ===== Previous values  =====
float prevHumidity = 0;
float prevTemp = 0;
float prevMag = 0;
int prevClear = 0;

// ===== Thresholds =====
const float HUMID_JUMP_THRESH = 2.0;
const float TEMP_RISE_THRESH = 0.5;
const float MAG_SHIFT_THRESH = 5.0;
const int LIGHT_CHANGE_THRESH = 20;

// ===== Cooldown =====
unsigned long lastEventTime = 0;
const int COOLDOWN_MS = 2000;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!HS300x.begin()) {
    Serial.println("Failed to init HS3003");
    while (1);
  }

  if (!IMU.begin()) {
    Serial.println("Failed to init IMU");
    while (1);
  }

  if (!APDS.begin()) {
    Serial.println("Failed to init APDS");
    while (1);
  }
}

void loop() {

  // ===== READ SENSORS =====
  humidity = HS300x.readHumidity();
  temperature = HS300x.readTemperature();

  float mx, my, mz;
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(mx, my, mz);
    mag = sqrt(mx*mx + my*my + mz*mz);
  }

  if (APDS.colorAvailable()) {
    APDS.readColor(r, g, b);
    clear = (r + g + b) / 3;
  }

  // ===== EVENT FLAGS =====
  int humid_jump = abs(humidity - prevHumidity) > HUMID_JUMP_THRESH;
  int temp_rise = (temperature - prevTemp) > TEMP_RISE_THRESH;
  int mag_shift = abs(mag - prevMag) > MAG_SHIFT_THRESH;
  int light_or_color_change = abs(clear - prevClear) > LIGHT_CHANGE_THRESH;

  // ===== CLASSIFICATION =====
  String event = "BASELINE_NORMAL";

  unsigned long now = millis();
  bool cooldownActive = (now - lastEventTime < COOLDOWN_MS);

  if (!cooldownActive) {
    if (humid_jump || temp_rise) {
      event = "BREATH_OR_WARM_AIR_EVENT";
      lastEventTime = now;
    }
    else if (mag_shift) {
      event = "MAGNETIC_DISTURBANCE_EVENT";
      lastEventTime = now;
    }
    else if (light_or_color_change) {
      event = "LIGHT_OR_COLOR_CHANGE_EVENT";
      lastEventTime = now;
    }
  }

  // ===== PRINT FORMAT ===== (Chat helped format these)
  Serial.print("raw,");
  Serial.print("rh="); Serial.print(humidity);
  Serial.print(",temp="); Serial.print(temperature);
  Serial.print(",mag="); Serial.print(mag);
  Serial.print(",r="); Serial.print(r);
  Serial.print(",g="); Serial.print(g);
  Serial.print(",b="); Serial.print(b);
  Serial.print(",clear="); Serial.println(clear);

  Serial.print("flags,");
  Serial.print("humid_jump="); Serial.print(humid_jump);
  Serial.print(",temp_rise="); Serial.print(temp_rise);
  Serial.print(",mag_shift="); Serial.print(mag_shift);
  Serial.print(",light_or_color_change="); Serial.println(light_or_color_change);

  Serial.print("event,");
  Serial.println(event);

  // ===== UPDATE PREVIOUS =====
  prevHumidity = humidity;
  prevTemp = temperature;
  prevMag = mag;
  prevClear = clear;

  delay(200);
}
