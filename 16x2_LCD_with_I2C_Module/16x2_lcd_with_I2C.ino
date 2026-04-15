#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int counter = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  
  Serial.begin(115200);
  Serial.println("Working");
  
  Wire.begin(D2,D1);

  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Pulse Rate : ");

  lcd.setCursor(0, 1);
  lcd.print("Temparature: ");

  delay(500);
}
