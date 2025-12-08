#include <Tone.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "Hardware.h"


// --- Keypad Definitions ---
int song [] = {
  NOTE_G5, NOTE_D5, NOTE_B4,
  NOTE_G5, NOTE_D5, NOTE_B4,
  NOTE_G5, NOTE_D5,
  NOTE_F5, NOTE_D5, NOTE_A4,
  NOTE_F5, NOTE_D5, NOTE_A4,
  NOTE_F5, NOTE_D5,
  NOTE_E5, NOTE_C5, NOTE_A4,
  NOTE_E5, NOTE_C5, NOTE_A4,
  NOTE_E5, NOTE_C5,
  NOTE_E5, NOTE_C5, NOTE_A4,
  NOTE_E5, NOTE_C5, NOTE_A4,
  NOTE_E5, NOTE_C5,




  NOTE_G5, NOTE_D5, NOTE_B4,
  NOTE_G5, NOTE_D5, NOTE_B4,
  NOTE_G5, NOTE_D5,
  NOTE_F5, NOTE_D5, NOTE_A4,
  NOTE_F5, NOTE_D5, NOTE_A4,
  NOTE_F5, NOTE_D5,
  NOTE_E5, NOTE_C5, NOTE_A4,
  NOTE_E5, NOTE_C5, NOTE_A4,
  NOTE_E5, NOTE_C5,
  NOTE_E5, NOTE_C5, NOTE_A4,
  NOTE_E5, NOTE_C5, NOTE_A4,
  NOTE_E5, NOTE_C5
};


int runaway_song [] = {
  NOTE_C6, NOTE_C6, NOTE_C6,
  NOTE_C5,
  NOTE_B5, NOTE_B5, NOTE_B5,
  NOTE_B4,
  NOTE_A5, NOTE_A5, NOTE_A5,
  NOTE_A4,
  NOTE_F5, NOTE_F5, NOTE_E5,
  NOTE_C6
};


int runaway_song2 [] = {
 NOTE_E6, NOTE_E6, NOTE_E6, NOTE_D6, NOTE_C6,
   // "Yeah I always find"
 NOTE_E6, NOTE_E6, NOTE_E6, NOTE_D6, NOTE_C6,




  // "Something wrong"
 NOTE_C6, NOTE_D6, NOTE_E6,
   // "You've been putting up with my..."
 NOTE_E6, NOTE_E6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_C6,




  // "...stuff just way too long"
 NOTE_C6, NOTE_A5, NOTE_C6, NOTE_D6, NOTE_E6,




  // "I'm so gifted at"
 NOTE_E6, NOTE_E6, NOTE_E6, NOTE_D6, NOTE_C6,




  // "Finding what I don't like the most"
 NOTE_C6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_D6, NOTE_E6,




  // --- THE "LOOK AT YA" FAST CHOP (Optional) ---
  // If you want that really fast repeating sound:
 NOTE_E6, NOTE_D6, NOTE_C6,
 NOTE_E6, NOTE_D6, NOTE_C6,
 NOTE_E6, NOTE_D6, NOTE_C6
};


int cornfield [] = {
   // --- PART 1: The Intro Pattern (A -> E) ---
 // "Tick-tock" feel. Repeat 4 times.
 NOTE_A4, NOTE_E5, NOTE_A4, NOTE_E5,
 NOTE_A4, NOTE_E5, NOTE_A4, NOTE_E5,
 NOTE_A4, NOTE_E5, NOTE_A4, NOTE_E5,
 NOTE_A4, NOTE_E5, NOTE_A4, NOTE_E5,




 // --- PART 2: The Shift (B -> E) ---
 // The bass note moves up to B
 NOTE_B4, NOTE_E5, NOTE_B4, NOTE_E5,
 NOTE_B4, NOTE_E5, NOTE_B4, NOTE_E5,
 NOTE_B4, NOTE_E5, NOTE_B4, NOTE_E5,
 NOTE_B4, NOTE_E5, NOTE_B4, NOTE_E5,




 // --- PART 3: The Peak (C -> E) ---
 // The bass note moves up to C
 NOTE_C5, NOTE_E5, NOTE_C5, NOTE_E5,
 NOTE_C5, NOTE_E5, NOTE_C5, NOTE_E5,
 NOTE_C5, NOTE_E5, NOTE_C5, NOTE_E5,
 NOTE_C5, NOTE_E5, NOTE_C5, NOTE_E5,




 // --- PART 4: The Tension (D -> E) ---
 // The bass note moves up to D
 NOTE_D5, NOTE_E5, NOTE_D5, NOTE_E5,
 NOTE_D5, NOTE_E5, NOTE_D5, NOTE_E5,
 NOTE_D5, NOTE_E5, NOTE_D5, NOTE_E5,
 NOTE_D5, NOTE_E5, NOTE_D5, NOTE_E5
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
// Variables for music mode
int arr[100];
String l_arr[100];
int pos = 0;
int bpm = 100;
int octave = 4;


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
       octave = 4;
       for (int i = 0; i < pos; i++) {
         arr[i] = 0;
       }
       pos = 0;
       bpm = 100;
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
   new_note = note * pow(1.0383, counter);
 }
 else if (counter < 0) {
   new_note = note / pow(1.0383, abs(counter));
 }


 speaker.stop();
 speaker.play((int)new_note, 200);
 return (int)new_note;
}


// Function to play music sequence
void playMusic() {
 // Variable for space between notes
 int ms = 60000 / bpm;
 lcd.autoscroll();
 lcd.rightToLeft();
 lcd.clear();
 lcd.setCursor(15, 0);


 // Loop to go through note stored
 for (int i = 0; i < pos; i++) {
   int note = arr[i];
   // Play if there is a note
   if (note > 0) {
     lcd.print(l_arr[i]);
     lcd.print(" ");
     speaker.play(arr[i], 200);
     delay(ms);
   }
 }


 // Reset speaker
 speaker.stop();
 lcd.clear();
 lcd.noAutoscroll();


 // Loop to remove all the note in the array
 for (int j = 0; j < pos; j++) {
   arr[j] = 0;
   l_arr[j] = "";
 }


 // Reset the position
 pos = 0;
}


// Function to add notes in the array
void add_To_Array(int note) {
 arr[pos] = musicChange(note);
}


void add_To_Letter_Array(String letter) {
 l_arr[pos] = letter;
 pos++;
}


// Function for music mode
void musicMode() {
 // Statements to increase or decrease the octave
 if (key == 'A' && counter < 4) {
   counter++;
   octave++;
   lcd.clear();
   lcd.print("Octave: ");
   lcd.print(octave);
 }
 if (key == 'B' && counter > -4) {
   counter--;
   octave--;
   lcd.clear();
   lcd.print("Octave: ");
   lcd.print(octave);
 }
 // Statements to increase or decrease bpm
 if (key == 'C' && bpm < 200) {
   bpm += 10;
   lcd.clear();
   lcd.print("BPM: ");
   lcd.print(bpm);
 }
 if (key == 'D' && bpm > 50) {
   bpm -= 10;
   lcd.clear();
   lcd.print("BPM: ");
   lcd.print(bpm);
 }


 // Statements to make each number its own pitch
 if (key == '1') {
   add_To_Array(musicChange(NOTE_C4));
   lcd.clear();
   lcd.print("C");
   add_To_Letter_Array("C");
 }
 if (key == '2') {
   add_To_Array(musicChange(NOTE_CS4));
   lcd.clear();
   lcd.print("C#");
   add_To_Letter_Array("C#");
 }
 if (key == '3') {
   add_To_Array(musicChange(NOTE_D4));
   lcd.clear();
   lcd.print("D");
   add_To_Letter_Array("D");
 }
 if (key == '4') {
   add_To_Array(musicChange(NOTE_DS4));
   lcd.clear();
   lcd.print("D#");
   add_To_Letter_Array("D#");
 }
 if (key == '5') {
   add_To_Array(musicChange(NOTE_E4));
   lcd.clear();
   lcd.print("E");
   add_To_Letter_Array("E");
 }
 if (key == '6') {
   add_To_Array(musicChange(NOTE_F4));
   lcd.clear();
   lcd.print("F");
   add_To_Letter_Array("F");
 }
 if (key == '7') {
   add_To_Array(musicChange(NOTE_FS4));
   lcd.clear();
   lcd.print("F#");
   add_To_Letter_Array("F#");
 }
 if (key == '8') {
   add_To_Array(musicChange(NOTE_G4));
   lcd.clear();
   lcd.print("G");
   add_To_Letter_Array("G");
 }
 if (key == '9') {
   add_To_Array(musicChange(NOTE_GS4));
   lcd.clear();
   lcd.print("G#");
   add_To_Letter_Array("G#");
 }
 if (key == '0') {
  if (octave != 1 && octave != 8) {
    add_To_Array(musicChange(NOTE_A4));
    lcd.clear();
    lcd.print("A");
    add_To_Letter_Array("A");
  }


   if (octave == 1) {
     int songLength = sizeof(song) / sizeof(song[0]);
     for (int i = 0; i < songLength; i++) {
       speaker.play(song[i]);
       delay(300);
     }
   }


   if (octave == 8) {
     int songLength = sizeof(cornfield) / sizeof(cornfield[0]);
     for (int i = 0; i < songLength; i++) {
       speaker.play(cornfield[i], 100);
       delay(150);
     }
   }
 }


 // Statement to delete last entered note
 if (key == '*') {
   pos--;
   arr[pos] = 0;
 }
 // Statement to play the music stored in the array
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
