#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C address (usually 0x3C)
#define OLED_ADDR 0x3C

// Create display object  

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Scanning...");
  
  // Start I2C
  Wire.begin(D2, D1);  // SDA, SCL

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED not found!");
    while (true);
  }

  display.clearDisplay();

  // Text settings
  display.setTextSize(2);
  display.setTextColor(WHITE);
  // display.setCursor(10, 20);

  display.println("");
  display.println("Hii Prince");
  display.println("Good day:)");
  display.display();
}

void loop() {
  Serial.println("Printing again...");
  delay(2000); // wait 2 seconds
}
