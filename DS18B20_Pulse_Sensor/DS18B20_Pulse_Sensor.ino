#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <ESP8266WiFi.h>

#define ONE_WIRE_BUS D4
#define DEBUG 0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const char DEVICE_LOGIN_NAME[] = "Device_ID_Name";
const char SSID[]              = "Wifi_Name";
const char PASS[]              = "Wifi_Password";
const char DEVICE_KEY[]        = "Secret_Key";

CloudHeartRate heart_rate;
CloudTemperatureSensor temperature;

const int pulsePin   = A0;
int pulseValue       = 0;
int threshold        = 550;
int lastState        = 0;
int beatCount        = 0;
unsigned int bpm     = 0;

// ======= Finger Detection =======
int peak                  = 0;
int trough                = 1023;
const int MIN_AMPLITUDE   = 50;  // raise this if noise triggers beats with no finger

unsigned long previousBpmTime          = 0;
const unsigned long BPM_INTERVAL       = 10000UL;

unsigned long lastPulseRead            = 0;
const unsigned long PULSE_INTERVAL     = 5UL;

bool tempRequested                     = false;
unsigned long tempRequestTime          = 0;
const unsigned long TEMP_CONVERSION_DELAY = 800UL;
unsigned long lastTempUpdate           = 0;
const unsigned long TEMP_UPDATE_INTERVAL  = 5000UL;

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

void initProperties() {
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(heart_rate, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(temperature, READ, 2 * SECONDS, NULL);
}

void setup() {
  Serial.begin(115200);
  delay(3000);

  sensors.begin();
  sensors.setResolution(9);
  sensors.setWaitForConversion(false);

  WiFi.mode(WIFI_STA);
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  pinMode(pulsePin, INPUT);

  previousBpmTime = millis();
  lastTempUpdate  = millis();
}

void loop() {
  ArduinoCloud.update();
  unsigned long now = millis();

  // ======= Pulse Sampling — 200 Hz =======
  if (now - lastPulseRead >= PULSE_INTERVAL) {
    lastPulseRead = now;
    pulseValue = analogRead(pulsePin);

    // Track peak and trough for finger detection
    if (pulseValue > peak)   peak   = pulseValue;
    if (pulseValue < trough) trough = pulseValue;

    #if DEBUG
      Serial.print("Analog: ");
      Serial.println(pulseValue);
    #endif

    // Only count beats if a finger is actually present
    if ((peak - trough) >= MIN_AMPLITUDE) {
      if (pulseValue > threshold && lastState == 0) {
        beatCount++;
        lastState = 1;
        Serial.println("Beat!");
      }
      if (pulseValue < threshold) {
        lastState = 0;
      }
    }
  }

  // ======= BPM Calculation — every 10 seconds =======
  if (now - previousBpmTime >= BPM_INTERVAL) {
    if ((peak - trough) >= MIN_AMPLITUDE) {
      bpm = (beatCount * 60000UL) / BPM_INTERVAL;
    } else {
      bpm = 0;  // no finger — report 0
    }

    heart_rate = bpm;
    Serial.print("BPM: ");
    Serial.println(bpm);

    // Reset for next window
    beatCount       = 0;
    previousBpmTime = now;
    peak            = 0;    // reset so amplitude re-evaluates each window
    trough          = 1023;
  }

  // ======= Temperature Phase 1 =======
  if (!tempRequested && (now - lastTempUpdate >= TEMP_UPDATE_INTERVAL)) {
    sensors.requestTemperatures();
    tempRequestTime = now;
    tempRequested   = true;
  }

  // ======= Temperature Phase 2 =======
  if (tempRequested && (now - tempRequestTime >= TEMP_CONVERSION_DELAY)) {
    float tempC = sensors.getTempCByIndex(0);
    if (tempC != DEVICE_DISCONNECTED_C) {
      Serial.print("Temp: ");
      Serial.print(tempC);
      Serial.println(" C");
      temperature = tempC;
    } else {
      Serial.println("Temp sensor error!");
    }
    tempRequested  = false;
    lastTempUpdate = tempRequestTime;
  }
}

void onHeartRateChange() {}
