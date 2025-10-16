#include "Arduino.h"
#include "button.h"
#include "pitches.h"

// --- Pin Definitions ---
const int buttonPin = 2;  // The number of the pushbutton pin
const int ledPin = 8;     // The number of the LED pin
const int buzzerPin = A0;  // The number of the buzzer pin

// --- Song: "Twinkle Twinkle Little Star" ---
int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4
};

// Note durations: 4 = quarter note, 8 = eighth note, etc.
int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2
};

// --- State Variables ---
int lastButtonState = LOW; // Stores the previous state of the button

// This function plays the song defined above.
void playSong() {
  for (int thisNote = 0; thisNote < 14; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    
    // stop the tone playing:
    noTone(buzzerPin);
  }
}

// This function sets up the pins. It's called once from the main setup().
void button_setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

// This function contains the logic that runs repeatedly.
void button_loop() {
  // Read the current state of the pushbutton
  int currentButtonState = digitalRead(buttonPin);

  // Turn the LED on whenever the button is held down
  if (currentButtonState == HIGH) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // Check if the button state has changed from LOW to HIGH (i.e., it was just pressed)
  if (currentButtonState == HIGH && lastButtonState == LOW) {
    playSong(); // Play the song once
  }

  // Save the current button state for the next loop iteration
  lastButtonState = currentButtonState;
}
