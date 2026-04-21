#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ONE_WIRE_BUS D4
#define DEBUG 0

// ======= OLED =======
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ======= Cloud Credentials =======
const char DEVICE_LOGIN_NAME[] = "Device_ID_Name";
const char SSID[]              = "Wifi_Name";
const char PASS[]              = "Wifi_Password";
const char DEVICE_KEY[]        = "Secret_Key";

// ======= Cloud Variables =======
CloudHeartRate heart_rate;
CloudTemperatureSensor temperature;

// ======= Pulse Sensor =======
const int pulsePin  = A0;
int pulseValue      = 0;
int threshold       = 550;
int lastState       = 0;
int beatCount       = 0;
unsigned int bpm    = 0;

// ======= Finger Detection =======
int peak                = 0;
int trough              = 1023;
const int MIN_AMPLITUDE = 50;

// ======= BPM Timing =======
unsigned long previousBpmTime      = 0;
const unsigned long BPM_INTERVAL   = 10000UL;

// ======= Pulse Sampling =======
unsigned long lastPulseRead        = 0;
const unsigned long PULSE_INTERVAL = 5UL;

// ======= Temperature — non-blocking =======
bool tempRequested                        = false;
unsigned long tempRequestTime             = 0;
const unsigned long TEMP_CONVERSION_DELAY = 800UL;
unsigned long lastTempUpdate              = 0;
const unsigned long TEMP_UPDATE_INTERVAL  = 5000UL;

// ======= OLED Update Timing =======
unsigned long lastOledUpdate           = 0;
const unsigned long OLED_INTERVAL      = 1000UL;
float lastTempC                        = 0.0;

// ======= WiFi Handler =======
WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

void initProperties() {
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(heart_rate, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(temperature, READ, 2 * SECONDS, NULL);
}

void updateOled() {

  display.clearDisplay();
  // ======= Header line =======
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 0);
  display.print("Health Monitor");

  // ======= Divider line =======
  display.drawLine(0, 10, 127, 10, WHITE);

  // ======= Pulse Rate =======
  display.setCursor(0, 16);
  display.setTextSize(1);
  display.print("Pulse Rate:");

  display.setCursor(0, 28);
  display.setTextSize(2);
  if (bpm == 0) {
    display.setTextSize(1);
    display.setCursor(0, 32);
    display.print("No finger");
  } else {
    display.print(bpm);
    display.setTextSize(1);
    display.setCursor(70, 34);
    display.print("BPM");
  }

  // ======= Divider line =======
  display.drawLine(0, 46, 127, 46, WHITE);

  // ======= Temperature =======
  display.setCursor(0, 50);
  display.setTextSize(1);
  display.print("Temp: ");
  if (lastTempC == 0.0) {
    display.print("--.- C");
  } else {
    display.print(lastTempC, 1);
    display.print(" C");
  }

  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(3000);

  // ======= OLED Init =======
  Wire.begin(D2, D1);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED not found!");
    while (true);
  }
  display.clearDisplay();

  // Line 1: Health Monitor
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 10);
  display.print("Health Monitor");

  // Line 2: Hii Prince :)
  display.setTextSize(1);
  display.setCursor(25, 35);
  display.print("Hii Prince :)");

  display.display();
  delay(3000);  // show for 3 seconds then loop takes over

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
  lastOledUpdate  = millis();
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
      lastTempC   = tempC;  // save for OLED
    } else {
      Serial.println("Temp sensor error!");
    }
    tempRequested  = false;
    lastTempUpdate = tempRequestTime;
  }

  // ======= OLED Update — every 1 second =======
  if (now - lastOledUpdate >= OLED_INTERVAL) {
    updateOled();
    lastOledUpdate = now;
  }
}

void onHeartRateChange() {}
