#include <Tone.h>
#include <LiquidCrystal_I2C.h> 
#include <Keypad.h>
#include "Hardware.h"

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
int counter = 0; 

// Variables to switch between modes
boolean isMusicMode = false;
int musicModeCount = 0;
int arr[100];
int pos = 0;
int bpm = 100;

void musicMode();
void processKey();
void updateDisplay();

void setup() {
  initializeHardware();
}

void loop() {
  key = kpd.getKey();

  if (key != NO_KEY) {
    // Check if '*' is pressed 3 times in a row
    handleStarPress();

    // Music Mode
    if (isMusicMode) {
      musicMode();
    } else {
      // Calculator Mode
      processKey();
      updateDisplay();
    }
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
  if (result == true && key >= '0' && key <= '9') {
    clearAll();
  }

  // --- Number Keys ---
  if (key >= '0' && key <= '9') {
    Number = (Number * 10) + (key - '0');
  }

  // --- Operator Keys ---
  if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
    Num1 = Number;
    Number = 0;
    actionPressed = true;
    result = false;
    if (key == 'A') action = '+';
    if (key == 'B') action = '-';
    if (key == 'C') action = '*';
    if (key == 'D') action = '/';
  }

  // --- Equals ---
  if (key == '#') {
    Num2 = Number;
    result = true;
    CalculateResult();
  }

  // --- Clear ---
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

// Function to switch between music mode and calculator mode
void handleStarPress() {
  if (key == '*') {
    musicModeCount++;

    // If user pressed '*' three times in a row, toggle between modes and reset counter
    if (musicModeCount >= 3) {
      isMusicMode = !isMusicMode;
      musicModeCount = 0;

      lcd.clear();

      if (isMusicMode) {
        counter = 0;
        lcd.setCursor(0, 0);
        lcd.print("MUSIC MODE ON");
      }
      else {
        lcd.setCursor(0, 0);
        lcd.print("MUSIC MODE OFF");
      }

      delay(1000);
      lcd.clear();
    }
  }
  else {
    // Reset the counter if another key is pressed
    musicModeCount = 0;
  }
}

// Function to change the octaces based on the counter
int musicChange(int note) {
  float new_note = note;

  // Statements to increase/decrease the octaves
  if (counter > 0) {
    new_note = note * pow(2, counter);
  }
  else if (counter < 0) {
    new_note = note / pow(2, abs(counter));
  }

  speaker.stop();
  speaker.play((int)new_note, 200);
  return (int)new_note;
}

void playMusic() {
  int noteCount = sizeof(arr) / sizeof(arr[0]);

  for (int i = 0; i < noteCount; i++) {
    speaker.play(arr[i], bpm);
    delay(250);
  }

  speaker.stop();
}

void add_To_Array(int note) {
  arr[pos] = musicChange(note);
  pos++;
}

// Function for music mode
void musicMode() {
  // Statements to increase of decrease the octave
  if (key == 'A' && counter < 3) {
    counter++;
  }
  if (key == 'B' && counter > -3) {
    counter--;
  }
  if (key == 'C') {
    bpm += 10;
  }
  if (key == 'D') {
    bpm -= 10;
  }

  // Statements to make each number its own pitch
  if (key == '1') add_To_Array(musicChange(NOTE_C4));
  if (key == '2') add_To_Array(musicChange(NOTE_CS4));
  if (key == '3') add_To_Array(musicChange(NOTE_D4));
  if (key == '4') add_To_Array(musicChange(NOTE_DS4));
  if (key == '5') add_To_Array(musicChange(NOTE_E4));
  if (key == '6') add_To_Array(musicChange(NOTE_F4));
  if (key == '7') add_To_Array(musicChange(NOTE_FS4));
  if (key == '8') add_To_Array(musicChange(NOTE_G4));
  if (key == '9') add_To_Array(musicChange(NOTE_GS4));
  if (key == '0') add_To_Array(musicChange(NOTE_A4));

  if (key == '#') playMusic();
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
    //playSong();
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