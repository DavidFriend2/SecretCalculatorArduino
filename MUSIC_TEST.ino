#include <LiquidCrystal_I2C.h> 
#include <Tone.h>
#include <Keypad.h> //Header file for  Keypad from https://github.com/Chris--A/Keypad
#include <arduion.h>


const byte ROWS = 4; // Four  rows
const byte COLS = 4; // Four columns


// Define the PWM signal of the speaker
#define NOTE_C3   131
#define NOTE_CS3  139
#define NOTE_D3   147
#define NOTE_DS3  156
#define NOTE_E3   165
#define NOTE_F3   175
#define NOTE_FS3  185
#define NOTE_G3   196
#define NOTE_GS3  208
#define NOTE_A3   220
#define NOTE_AS3  233
#define NOTE_B3   247
#define NOTE_C4   262
#define NOTE_CS4  277  // C#
#define NOTE_D4   294
#define NOTE_DS4  311  // D#
#define NOTE_E4   330
#define NOTE_F4   349
#define NOTE_FS4  370  // F#
#define NOTE_G4   392
#define NOTE_GS4  415  // G#
#define NOTE_A4   440
#define NOTE_AS4  466  // A#
#define NOTE_B4   494
#define NOTE_C5   523
#define NOTE_CS5  554
#define NOTE_D5   587
#define NOTE_DS5  622
#define NOTE_E5   659
#define NOTE_F5   698
#define NOTE_FS5  740
#define NOTE_G5   784
#define NOTE_GS5  831
#define NOTE_A5   880
#define NOTE_AS5  932
#define NOTE_B5   988
#define NOTE_C6   1047
#define NOTE_CS6  1109  // C#6 / Db6
#define NOTE_D6   1175
#define NOTE_DS6  1245  // D#6 / Eb6
#define NOTE_E6   1319
#define NOTE_F6   1397
#define NOTE_FS6  1480  // F#6 / Gb6
#define NOTE_G6   1568
#define NOTE_GS6  1661  // G#6 / Ab6
#define NOTE_A6   1760
#define NOTE_AS6  1865  // A#6 / Bb6
#define NOTE_B6   1976

#define BUZZER_PIN A0

void Beep(uint16_t freq, uint16_t duration_ms)
{
  if (freq == 0) {
    noTone(BUZZER_PIN);
    delay(duration_ms);
  } else {
    tone(BUZZER_PIN, freq, duration_ms);
    delay(duration_ms);
    noTone(BUZZER_PIN);
  }
}

void Music_Test(void)
{
  unit16_t song [] = {
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
        NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4,
        NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,
        NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4,
        NOTE_C5, NOTE_C5, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
        NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4,
        NOTE_G4, NOTE_G4, NOTE_C5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_A4,
        NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4
  };
  unit16_t length = sizeof(song)/sizeof(song[0]);
  for (unit16_t i=0; i<length; i+=){
    Beep(song[i], 350);
    delay(10);
  }
}



// Define the Keymap
char  keys[ROWS][COLS] = {

  {'1','2','3','A'},

  {'4','5','6','B'},

  {'7','8','9','C'},

  {'*','0','#','D'}

};

byte rowPins[ROWS]  = { 0, 1, 2, 3 };// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte  colPins[COLS] = { 4, 5, 6, 7 }; // Connect keypad COL0, COL1 and COL2 to these Arduino  pins.

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS  ); //  Create the Keypad

const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 =  12, d7 = 13; //Pins to which LCD is connected
LiquidCrystal_I2C lcd(0x27, 16,  2);

 long Num1,Num2,Number;
 char key,action;
 boolean result = false;
  
void setup() {
  pinMode(BUZZER_PIN,OUTPUT);
  lcd.backlight();
  lcd.init();
  lcd.begin(16, 2); // Initialize the LCD with 16 columns and 2 rows //We are using a 16*2 LCD display
  lcd.print("Calculator  Ready"); //Display a intro message
  lcd.setCursor(0, 1);   // set the cursor  to column 0, line 1
  lcd.print("A=+ B=- C=* D=/"); //Display a intro message  

  delay(3000); //Wait for display to show info
  lcd.clear(); //Then  clean it
}

void loop() {
  key = kpd.getKey(); //storing pressed  key value in a char

  if (key != NO_KEY) {
    Music_Test(); //Play a 85 note for 100 miliseconds
    DetectButtons();
  }

  if (result==true)
    CalculateResult();

  DisplayResult();   
}

void DetectButtons()
{ 
    lcd.clear(); //Then clean it
    if (key == '*') //If cancel Button is pressed
    {
      Serial.println ("Button  Cancel"); 
      Number=Num1=Num2=0; 
      result=false;
    }
    
    if (key == '1') //If  Button 1 is pressed
    {
      Serial.println ("Button 1"); 
      if (Number==0)
        Number=1;
      else
        Number = (Number*10) + 1; //Pressed twice
    }
    
    if (key == '4') //If Button 4 is pressed
    {
      Serial.println ("Button  4"); 
      if (Number==0)
        Number=4;
      else
        Number = (Number*10)  + 4; //Pressed twice
    }
    
    if (key == '7') //If Button 7 is pressed
    {
      Serial.println ("Button 7");
      if (Number==0)
        Number=7;
      else
        Number = (Number*10) + 7; //Pressed twice
    } 

    if (key  == '0')
    {
      Serial.println ("Button 0"); //Button 0 is Pressed
      if (Number==0)
        Number=0;
      else
        Number = (Number*10) + 0; //Pressed twice
    }
    
    if (key == '2') //Button 2 is Pressed
    {
      Serial.println ("Button  2"); 
      if (Number==0)
        Number=2;
      else
        Number = (Number*10)  + 2; //Pressed twice
    }
    
    if (key == '5')
    {
      Serial.println  ("Button 5"); 
      if (Number==0)
        Number=5;
      else
        Number  = (Number*10) + 5; //Pressed twice
    }
    
    if (key == '8')
    {
      Serial.println ("Button 8"); 
      if (Number==0)
        Number=8;
      else
        Number = (Number*10) + 8; //Pressed twice
    }   
  

    if (key == '#')
    {
      Serial.println ("Button Equal"); 
      Num2=Number;
      result = true;
    }
    
    if (key == '3')
    {
      Serial.println  ("Button 3"); 
      if (Number==0)
        Number=3;
      else
        Number  = (Number*10) + 3; //Pressed twice
    }
    
    if (key == '6')
    {
      Serial.println ("Button 6"); 
      if (Number==0)
        Number=6;
      else
      Number = (Number*10) + 6; //Pressed twice
    }
    
    if  (key == '9')
    {
      Serial.println ("Button 9");
      if (Number==0)
        Number=9;
      else
        Number = (Number*10) + 9; //Pressed twice
    }  

    if  (key == 'A' || key == 'B' || key == 'C' || key == 'D') //Detecting Buttons on Column  4
    {
      Num1 = Number;    
      Number = 0;
      if (key == 'A')
      {
        Serial.println("Addition"); 
        action = '+';
      }
      if (key == 'B')
      {
        Serial.println("Subtraction"); 
        action = '-'; 
      }
      if (key == 'C')
      {
        Serial.println("Multiplication"); 
        action = '*';
      }
      if (key == 'D')
      {
        Serial.println("Division");
        action = '/';
      }  

      delay(100);
  }
  
}

void  CalculateResult()
{
  if (action=='+')
    Number = Num1+Num2;

  if (action=='-')
    Number = Num1-Num2;

  if (action=='*')
    Number  = Num1*Num2;

  if (action=='/')
    Number = Num1/Num2; 
}

void  DisplayResult()
{
  lcd.setCursor(0, 0);   // set the cursor to column 0,  line 1
  lcd.print(Num1); lcd.print(action); lcd.print(Num2); 
  
  if  (result==true) {
    //{lcd.print(" ="); lcd.print(Number);} //Display the result
    lcd.print(" =");
    lcd.setCursor(0, 1);   // set the cursor to column 0, line 1
    lcd.print(Number);  //Display the result
  }
}
