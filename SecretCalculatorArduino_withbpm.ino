#include <Tone.h>
#include <LiquidCrystal_I2C.h> 
#include <Keypad.h>
#include "Hardware.h"
#include "button.h"

// --- Keypad Definitions ---
int song [] = {
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
        NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4
};

// --- Calculator Variables ---
long Num1, Num2, Number;
char key, action;
boolean result = false;
// NEW: A flag to know when an operator has been pressed
boolean actionPressed = false; 

void setup() {
  initializeHardware();
}

void loop() {
  key = kpd.getKey();

  if (key != NO_KEY) {
    // Process the key press first
    processKey(); 
    // Then, update the display based on the new state
    updateDisplay(); 
  }
}

void playSong() {
  int noteCount = sizeof(song) / sizeof(song[0]);

  for (int i = 0; i < noteCount; i++) {
    // Play each note for 200 milliseconds
    speaker.play(song[i], 200);
    // Pause briefly between notes to make them distinct
    delay(250);
  }
  speaker.stop();
}

// RENAMED and REWRITTEN from DetectButtons()
void processKey() {
    // If we just calculated a result, any new number should start a new calculation
    if (result == true) {
      if (key >= '0' && key <= '9') {
        clearAll();
      }
    }

    // --- Number Keys ---
    if (key >= '0' && key <= '9') {
      // Convert char '1' to long 1, etc.
      Number = (Number * 10) + (key - '0'); 
    }

    // --- Operator Keys (A, B, C, D) ---
    if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
      Num1 = Number;
      Number = 0;
      actionPressed = true;
      result = false; // Make sure we are not in the result state anymore
      if (key == 'A') action = '+';
      if (key == 'B') action = '-';
      if (key == 'C') action = '*';
      if (key == 'D') action = '/';
    }

    // --- Equals Key ---
    if (key == '#') {
      Num2 = Number;
      result = true;
      CalculateResult();
    }
    
    // --- Clear Key ---
    if (key == '*') {
      clearAll();
    }
}

// This function now holds the final calculation logic
void CalculateResult() {
  if (action == '+') Number = Num1 + Num2;
  if (action == '-') Number = Num1 - Num2;
  if (action == '*') Number = Num1 * Num2;
  // Add a check to prevent division by zero
  if (action == '/') {
    if (Num2 == 0) {
      Number = 0; // Or display an error
    } else {
      Number = Num1 / Num2;
    }
  }
}

// NEW: This is a dedicated function to reset everything
void clearAll() {
  Num1 = 0;
  Num2 = 0;
  Number = 0;
  action = ' ';
  result = false;
  actionPressed = false;
}


// NEW: This single function handles all screen writing
void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (result) {
    // Show the full equation and result
    lcd.print(Num1);
    lcd.print(action);
    lcd.print(Num2);
    lcd.print("=");
    lcd.setCursor(0, 1);
    lcd.print(Number);
    playSong();
  } else if (actionPressed) {
    // Show the first number and the operator
    lcd.print(Num1);
    lcd.print(action);
    // And show the second number as it's being typed
    lcd.print(Number);
  } else {
    // No operator yet, just show the first number being typed
    lcd.print(Number);
  }
}
