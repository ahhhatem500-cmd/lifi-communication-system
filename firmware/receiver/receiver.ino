#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD I2C address (غالبًا 0x27 أو 0x3F حسب النوع)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int ldrPin = A0;         // LDR متوصلة بـ A0
const int threshold = 800;     // عتبة التحويل بين 0 و 1
const int bitDuration = 1000;  // مدة البت بالمللي ثانية

void setup() {
  Serial.begin(9600);
  pinMode(ldrPin, INPUT);

  // إعداد شاشة LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Receiver Ready");

  delay(1000);
  lcd.clear();
}

void loop() {
  int analogValue = analogRead(ldrPin);
  float voltage = analogValue * (5.0 / 1023.0);  // تحويل القراءة لجهد

  // عرض الجهد على السيريال فقط
  Serial.print("LDR Voltage: ");
  Serial.println(voltage);

  // التحقق من Start Bit
  if (analogValue > threshold) {
    Serial.println("Start Bit Detected");

    delay(bitDuration);  // انتظار نهاية البت

    byte receivedByte = 0;

    // استقبال 8 بت (MSB أولًا)
    for (int i = 7; i >= 0; i--) {
      analogValue = analogRead(ldrPin);
      int bit = (analogValue > threshold) ? 1 : 0;
      receivedByte |= (bit << i);

      Serial.print("Bit ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(bit);

      delay(bitDuration);
    }

    char receivedChar = (char)receivedByte;

    Serial.print("✅ Received Char: ");
    Serial.println(receivedChar);

    // عرض الحرف على الـ LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Received: ");
    lcd.setCursor(10, 0);
    lcd.print(receivedChar);

    delay(1000);  // انتظار قبل المحاولة التالية
  }

  delay(100);  // تقليل عدد القراءات
}