#include <Keypad.h>

const int laserPin = 10;

// إعداد الكيباد 4x4
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, LOW);
  Serial.begin(9600);
  Serial.println("Transmitter with MSB-first + Start Bit");
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY) {
    Serial.print("Key Pressed: ");
    Serial.println(key);
    sendAsciiWithStartBit_MSB(key);
  }
}

// إرسال Start Bit + 8 بت MSB أولًا
void sendAsciiWithStartBit_MSB(char character) {
  byte asciiCode = character;

  Serial.print("Sending bits: ");
  Serial.print("1 "); // Start bit
  for (int i = 7; i >= 0; i--) {
    Serial.print((asciiCode >> i) & 1);
  }
  Serial.println();

  // Start bit: 1
  digitalWrite(laserPin, HIGH);
  delay(1000);

  // إرسال 8 بت (MSB أولًا)
  for (int i = 7; i >= 0; i--) {
    bool bit = (asciiCode >> i) & 1;
    digitalWrite(laserPin, bit ? HIGH : LOW);
    delay(1000);
  }

  digitalWrite(laserPin, LOW); // تأكد من إطفاء الليزر بعد الإرسال
  delay(500); // فاصل بين الحروف
}