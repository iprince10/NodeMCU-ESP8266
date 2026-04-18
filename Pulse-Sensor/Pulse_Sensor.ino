#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <ESP8266WiFi.h>

// ======= Cloud and WiFi credentials =======
const char DEVICE_LOGIN_NAME[]  = "DEVICELOGINNAME";
const char SSID[]               = "NetworkSSID(name)";
const char PASS[]               = "Network-password";
const char DEVICE_KEY[]         = "SECRETKEY";

// ======= Cloud Variable =======
CloudHeartRate heart_rate;

// ======= Pulse Sensor =======
const int pulsePin = A0;
int pulseValue = 0;

// ======= Heartbeat Logic =======
int threshold = 550; 
int lastState = 0;

// ======= Timing =======
unsigned long previousTime = 0;
unsigned long interval = 5000;   // 5 seconds (stable)
int beatCount = 0;
int bpm = 0;

// ======= WiFi Handler =======
WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

// ======= Cloud Properties =======
void initProperties(){
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(heart_rate, READ, ON_CHANGE, NULL);
}

void setup() {
  Serial.begin(115200);
  delay(3000);

  WiFi.mode(WIFI_STA);   // Fix reconnect issue

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  pinMode(pulsePin, INPUT);
}

void loop() {
  ArduinoCloud.update();

  // ======= Reconnect WiFi if needed =======
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting WiFi...");
    WiFi.begin(SSID, PASS);
    delay(5000);
  }

  // ======= Read Pulse Sensor =======
  pulseValue = analogRead(pulsePin);

  // ======= Debug raw values =======
  Serial.print("Analog Value: ");
  Serial.println(pulseValue);

  delay(200);

  unsigned long currentTime = millis();

  // ======= Edge Detection (CORRECT WAY) =======
  if (pulseValue > threshold && lastState == 0) {
    beatCount++;
    lastState = 1;
    Serial.println("Beat detected!");
  }

  if (pulseValue < threshold) {
    lastState = 0;
  }

  // ======= BPM Calculation =======
  if (currentTime - previousTime >= interval) {
    bpm = beatCount * 12;   // 60 / 5 sec

    heart_rate = bpm;

    Serial.print("Heart Rate (BPM): ");
    Serial.println(bpm);

    beatCount = 0;
    previousTime = currentTime;
  }

  delay(10);
}

void onHeartRateChange() {
  // Optional
}
