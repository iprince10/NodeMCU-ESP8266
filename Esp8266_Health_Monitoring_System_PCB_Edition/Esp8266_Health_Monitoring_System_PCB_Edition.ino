#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ONE_WIRE_BUS D4
#define DEBUG 0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ======= LCD =======
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ======= Cloud Credentials =======
const char DEVICE_LOGIN_NAME[] = "Device_ID_Name";
const char SSID[]              = "Wifi_Name";
const char PASS[]              = "Wifi_Password";
const char DEVICE_KEY[]        = "Secret_Key";

// ======= Cloud Variables =======
CloudHeartRate heart_rate;
CloudTemperatureSensor temperature;

// ======= Pulse Sensor =======
const int pulsePin = A0;
int pulseValue     = 0;
int threshold      = 550;
int lastState      = 0;
int beatCount      = 0;
unsigned int bpm   = 0;

// ======= Finger Detection =======
int peak               = 0;
int trough             = 1023;
const int MIN_AMPLITUDE = 50;

// ======= BPM Timing =======
unsigned long previousBpmTime          = 0;
const unsigned long BPM_INTERVAL       = 10000UL;

// ======= Pulse Sampling =======
unsigned long lastPulseRead            = 0;
const unsigned long PULSE_INTERVAL     = 5UL;

// ======= Temperature — non-blocking =======
bool tempRequested                        = false;
unsigned long tempRequestTime             = 0;
const unsigned long TEMP_CONVERSION_DELAY = 800UL;
unsigned long lastTempUpdate              = 0;
const unsigned long TEMP_UPDATE_INTERVAL  = 5000UL;

// ======= LCD Update Timing =======
unsigned long lastLcdUpdate            = 0;
const unsigned long LCD_INTERVAL       = 1000UL;  // refresh LCD every 1 second
float lastTempC                        = 0.0;     // store last valid temp for LCD

// ======= WiFi Handler =======
WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

void initProperties() {
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(heart_rate, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(temperature, READ, 2 * SECONDS, NULL);
}

void updateLcd() {
  lcd.clear();

  // ======= Row 0: Pulse Rate =======
  lcd.setCursor(0, 0);
  if (bpm == 0) {
    lcd.print("Pulse: No finger");
  } else {
    lcd.print("Pulse: ");
    lcd.print(bpm);
    lcd.print(" BPM");
  }

  // ======= Row 1: Temperature =======
  lcd.setCursor(0, 1);
  if (lastTempC == 0.0) {
    lcd.print("Temp : --.- C");
  } else {
    lcd.print("Temp : ");
    lcd.print(lastTempC, 1);  // 1 decimal place e.g. 36.5
    lcd.print(" C");
  }
}

void setup() {
  Serial.begin(115200);
  delay(3000);

  // ======= LCD Init =======
     Wire.begin(D2, D1);  // SDA=D2, SCL=D1
     lcd.init();
     lcd.backlight();
     // Step 1: Show welcome screen
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("Health Monitor");
     lcd.setCursor(0, 1);
     lcd.print("Hii Prince :)");
     delay(2000);
     // Step 2: Show starting message
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("Initializing....");
     delay(2000);
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("System_Ready");
     delay(1000);

  // ======= Sensors =======
  sensors.begin();
  sensors.setResolution(9);
  sensors.setWaitForConversion(false);

  // ======= WiFi + Cloud =======
  WiFi.mode(WIFI_STA);
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  pinMode(pulsePin, INPUT);

  previousBpmTime = millis();
  lastTempUpdate  = millis();
  lastLcdUpdate   = millis();
}

void loop() {
  ArduinoCloud.update();
  unsigned long now = millis();

  // ======= Pulse Sampling — 200 Hz =======
  if (now - lastPulseRead >= PULSE_INTERVAL) {
    lastPulseRead = now;
    pulseValue = analogRead(pulsePin);

    if (pulseValue > peak)   peak   = pulseValue;
    if (pulseValue < trough) trough = pulseValue;

    #if DEBUG
      Serial.print("Analog: ");
      Serial.println(pulseValue);
    #endif

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
      bpm = 0;
    }

    heart_rate = bpm;
    Serial.print("BPM: ");
    Serial.println(bpm);

    beatCount       = 0;
    previousBpmTime = now;
    peak            = 0;
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
      lastTempC   = tempC;  // save for LCD
    } else {
      Serial.println("Temp sensor error!");
    }
    tempRequested  = false;
    lastTempUpdate = tempRequestTime;
  }

  // ======= LCD Update — every 1 second =======
  if (now - lastLcdUpdate >= LCD_INTERVAL) {
    updateLcd();
    lastLcdUpdate = now;
  }
}

void onHeartRateChange() {}
