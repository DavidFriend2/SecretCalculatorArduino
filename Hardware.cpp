#include "Hardware.h"

const int SPEAKER_PIN = A0;
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = { 0, 1, 2, 3 };
byte colPins[COLS] = { 4, 5, 6, 7 }; 

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// --- LCD Definition ---
LiquidCrystal_I2C lcd(0x27, 16, 2);
Tone speaker;

void initializeHardware() {
  speaker.begin(SPEAKER_PIN);
  
  lcd.backlight();
  lcd.init();
  lcd.print("Calculator Ready");
  lcd.setCursor(0, 1);
  lcd.print("A=+ B=- C=* D=/"); 
  delay(2000);
  lcd.clear();
}
