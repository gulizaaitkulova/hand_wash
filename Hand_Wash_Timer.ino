#include <Wire.h>
#include "pitches.h"
#include "noteDurations.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include "SevSeg.h"
SevSeg sevseg;
#include <dht.h>

#define dht_apin A0  // The humidity sensor sends its data to this analog pin

dht DHT;

#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//The Ultrasonic sensor's pins
const int trigPin = 36; 
const int echoPin = 38;

float duration, distance;
unsigned long currentTime;
int countDown;


// pitches.h defines all the frequencies for the different notes in different octaves
// Here we pick a default octave for a note without octave specified, so it is easy to change it in one place
// For example, if you want to go down an octave, change NOTE_C7 to NOTE_C6 and so on

#define NOTE_C  NOTE_C7
#define NOTE_CS NOTE_CS7
#define NOTE_D  NOTE_D7
#define NOTE_DS NOTE_DS7
#define NOTE_E  NOTE_E7
#define NOTE_F  NOTE_F7
#define NOTE_FS NOTE_FS7
#define NOTE_G  NOTE_G7
#define NOTE_GS NOTE_GS7
#define NOTE_A  NOTE_A7
#define NOTE_AS NOTE_AS7
#define NOTE_B  NOTE_B7


// The Piezo uses pin 34 in our project
#define TONE_PIN 34

// All of our tunes have the same characteristics 
// A Note in a melody has a frequency and a duration (1 = whole note, 2 = half note, etc)
// A melody consists of an array of such Notes (the melody)
// playTunes takes a pointer to the melody array of Notes, as well as a speedup factor. 
// The speedup factor is to facilitate playing at a higher speed (1 plays at intended speed, 
// 2 at twice the speed and so on) without having to modify the noteDurations array

typedef struct Note {
  int frequency;
  float duration;   
} Note;

typedef struct Melody {
  Note *notes;
  int numNotes;
  int wholeNoteDurationMs;  
} Melody;

// This melody is playing all the notes of the octave from C up to B and back down to C

Note melodyStart[] = {
  {NOTE_C, NOTE_WHOLE}, {NOTE_CS, NOTE_WHOLE}, {NOTE_D, NOTE_WHOLE}, {NOTE_DS, NOTE_WHOLE}, {NOTE_E, NOTE_WHOLE}, {NOTE_F, NOTE_WHOLE}, {NOTE_FS, NOTE_WHOLE}, {NOTE_G, NOTE_WHOLE}, {NOTE_GS, NOTE_WHOLE}, {NOTE_A, NOTE_WHOLE}, {NOTE_AS, NOTE_WHOLE}, {NOTE_B, NOTE_WHOLE},
  {NOTE_AS, NOTE_WHOLE}, {NOTE_A, NOTE_WHOLE}, {NOTE_GS, NOTE_WHOLE}, {NOTE_G, NOTE_WHOLE}, {NOTE_FS, NOTE_WHOLE}, {NOTE_F, NOTE_WHOLE}, {NOTE_E, NOTE_WHOLE}, {NOTE_DS, NOTE_WHOLE}, {NOTE_D, NOTE_WHOLE}, {NOTE_CS, NOTE_WHOLE}, {NOTE_C, NOTE_WHOLE}
};


// Happy Birthday

Note melody[] = { 
  {NOTE_G6, DOTTED(NOTE_EIGHTH)}, {NOTE_G6, NOTE_SIXTEENTH}, {NOTE_A6, NOTE_QUARTER}, {NOTE_G6, NOTE_QUARTER}, {NOTE_C7, NOTE_QUARTER}, {NOTE_B6, NOTE_HALF}, 
  {NOTE_G6, DOTTED(NOTE_EIGHTH)}, {NOTE_G6, NOTE_SIXTEENTH}, {NOTE_A6, NOTE_QUARTER}, {NOTE_G6, NOTE_QUARTER}, {NOTE_D7, NOTE_QUARTER}, {NOTE_C7, NOTE_HALF},
  {NOTE_G6, DOTTED(NOTE_EIGHTH)}, {NOTE_G6, NOTE_SIXTEENTH}, {NOTE_E7, NOTE_QUARTER}, {NOTE_D7, NOTE_QUARTER}, {NOTE_C7, NOTE_QUARTER}, {NOTE_B6, NOTE_QUARTER}, {NOTE_A6, NOTE_HALF}, 
  {NOTE_F7, DOTTED(NOTE_EIGHTH)}, {NOTE_F7, NOTE_SIXTEENTH}, {NOTE_E7, NOTE_QUARTER}, {NOTE_C7, NOTE_QUARTER}, {NOTE_D7, NOTE_QUARTER}, {NOTE_C7, NOTE_HALF},      
};

// Do a deer - Sound of Music

Note melody2[] = {
  {NOTE_C6, DOTTED(NOTE_QUARTER)}, {NOTE_D6, NOTE_EIGHTH}, {NOTE_E6, DOTTED(NOTE_QUARTER)}, {NOTE_C6, NOTE_EIGHTH}, {NOTE_E6, NOTE_QUARTER}, {NOTE_C6, NOTE_QUARTER}, {NOTE_E6, NOTE_HALF}, 
  {NOTE_D6, DOTTED(NOTE_QUARTER)}, {NOTE_E6, NOTE_EIGHTH}, {NOTE_F6, NOTE_EIGHTH}, {NOTE_F6, NOTE_EIGHTH}, {NOTE_E6, NOTE_EIGHTH}, {NOTE_D6, NOTE_EIGHTH}, {NOTE_F6, NOTE_WHOLE}, 
  {NOTE_E6, DOTTED(NOTE_QUARTER)}, {NOTE_F6, NOTE_EIGHTH}, {NOTE_G6, DOTTED(NOTE_QUARTER)}, {NOTE_E6, NOTE_EIGHTH}, {NOTE_G6, NOTE_QUARTER}, {NOTE_E6, NOTE_QUARTER}, {NOTE_G6, NOTE_HALF}, 
  {NOTE_F6, DOTTED(NOTE_QUARTER)}, {NOTE_G6, NOTE_EIGHTH}, {NOTE_A6, NOTE_EIGHTH}, {NOTE_A6, NOTE_EIGHTH}, {NOTE_G6, NOTE_EIGHTH}, {NOTE_F6, NOTE_EIGHTH}, {NOTE_A6, NOTE_WHOLE}, 
  {NOTE_G6, DOTTED(NOTE_QUARTER)}, {NOTE_C6, NOTE_EIGHTH}, {NOTE_D6, NOTE_EIGHTH}, {NOTE_E6, NOTE_EIGHTH}, {NOTE_F6, NOTE_EIGHTH}, {NOTE_G6, NOTE_EIGHTH}, {NOTE_A6, NOTE_WHOLE},
  {NOTE_A6, DOTTED(NOTE_QUARTER)}, {NOTE_D6, NOTE_EIGHTH}, {NOTE_E6, NOTE_EIGHTH}, {NOTE_F6, NOTE_EIGHTH}, {NOTE_G6, NOTE_EIGHTH}, {NOTE_A6, NOTE_EIGHTH}, {NOTE_B6, NOTE_WHOLE},
  {NOTE_B6, DOTTED(NOTE_QUARTER)}, {NOTE_E6, NOTE_EIGHTH}, {NOTE_F6, NOTE_EIGHTH}, {NOTE_G6, NOTE_EIGHTH}, {NOTE_A6, NOTE_EIGHTH}, {NOTE_B6, NOTE_EIGHTH}, {NOTE_C7, NOTE_WHOLE},
  {NOTE_C7, NOTE_EIGHTH}, {NOTE_B6, NOTE_EIGHTH}, {NOTE_A6, NOTE_QUARTER}, {NOTE_F6, NOTE_QUARTER}, {NOTE_B6, NOTE_QUARTER}, {NOTE_G6, NOTE_QUARTER}, 
  {NOTE_C7, NOTE_QUARTER}, {NOTE_G6, NOTE_QUARTER}, {NOTE_E6, NOTE_QUARTER}, {NOTE_C6, NOTE_QUARTER},
  {NOTE_C6, NOTE_EIGHTH}, {NOTE_D6, NOTE_EIGHTH}, {NOTE_E6, NOTE_EIGHTH}, {NOTE_F6, NOTE_EIGHTH}, {NOTE_G6, NOTE_EIGHTH}, {NOTE_A6, NOTE_EIGHTH}, {NOTE_B6, NOTE_QUARTER}, 
  {NOTE_C7, NOTE_QUARTER}, {NOTE_G6, NOTE_QUARTER}, {NOTE_C7, NOTE_QUARTER}, 
 

};

// We Will Rock You! Queen

Note melody3[] = {
  {NOTE_D6, NOTE_EIGHTH}, {NOTE_E6, NOTE_EIGHTH}, {NOTE_E6, NOTE_EIGHTH}, {NOTE_D6, NOTE_EIGHTH}, {NOTE_E6, NOTE_QUARTER}, {NOTE_E6, NOTE_EIGHTH}, {NOTE_D6, NOTE_EIGHTH}, 
  {NOTE_E6, NOTE_QUARTER}, {NOTE_E6, NOTE_QUARTER}, {NOTE_D6, NOTE_EIGHTH}, {NOTE_E6, NOTE_EIGHTH}, {NOTE_E6, NOTE_EIGHTH}, {NOTE_D6, NOTE_EIGHTH},
  {NOTE_E6, NOTE_QUARTER}, {NOTE_E6, NOTE_EIGHTH}, {NOTE_D6, NOTE_EIGHTH}, {NOTE_E6, NOTE_EIGHTH}, {NOTE_D6, NOTE_EIGHTH}, {NOTE_E6, NOTE_QUARTER},
  {NOTE_A6, NOTE_QUARTER}, {NOTE_G6, NOTE_QUARTER},  {NOTE_G6, NOTE_EIGHTH}, {NOTE_D6, NOTE_EIGHTH}, {NOTE_E6, NOTE_QUARTER},
  
  {NOTE_E6, NOTE_EIGHTH}, {NOTE_E6, NOTE_QUARTER}, {NOTE_D6, NOTE_EIGHTH}, {NOTE_E6, NOTE_QUARTER}, {REST, NOTE_EIGHTH}, {NOTE_D6, NOTE_EIGHTH}, 
  {NOTE_E6, NOTE_QUARTER}, {NOTE_E6, NOTE_QUARTER}, {NOTE_E6, NOTE_QUARTER}, {REST, NOTE_QUARTER}, 
  {NOTE_D6, NOTE_EIGHTH}, {NOTE_D6, NOTE_EIGHTH}, {NOTE_D6, NOTE_QUARTER}, {NOTE_D6, NOTE_QUARTER}, {NOTE_B5, NOTE_QUARTER}, 
     
  {NOTE_A5, NOTE_EIGHTH}, {NOTE_G5, NOTE_EIGHTH}, {NOTE_E5, NOTE_EIGHTH}, {NOTE_E5, NOTE_EIGHTH}, {NOTE_E5, NOTE_QUARTER}, {NOTE_B5, NOTE_EIGHTH}, {NOTE_D6, NOTE_EIGHTH}, 

  {NOTE_G7, NOTE_HALF}, {NOTE_FS7, NOTE_HALF}, {NOTE_E7, NOTE_HALF}, {NOTE_D7, NOTE_HALF}, {NOTE_E7, NOTE_QUARTER}, {NOTE_E7, NOTE_QUARTER}, {REST, NOTE_HALF}, {REST, NOTE_WHOLE},
  {NOTE_G7, NOTE_HALF}, {NOTE_FS7, NOTE_HALF}, {NOTE_E7, NOTE_HALF}, {NOTE_D7, NOTE_HALF}, {NOTE_E7, NOTE_QUARTER}, {NOTE_E7, NOTE_QUARTER}, {REST, NOTE_HALF}, {REST, NOTE_WHOLE},

};

// Jeopardy!

Note melody4[] = {
  {NOTE_G6, NOTE_QUARTER}, {NOTE_C7, NOTE_QUARTER}, {NOTE_G6, NOTE_QUARTER}, {NOTE_C6, NOTE_QUARTER}, 
  {NOTE_G6, NOTE_QUARTER}, {NOTE_C7, NOTE_QUARTER}, {NOTE_G6, NOTE_HALF},
  {NOTE_G6, NOTE_QUARTER}, {NOTE_C7, NOTE_QUARTER}, {NOTE_G6, NOTE_QUARTER}, {NOTE_C7, NOTE_QUARTER}, 
  {NOTE_E7, DOTTED(NOTE_QUARTER)}, {NOTE_D7, NOTE_EIGHTH}, {NOTE_C7, NOTE_EIGHTH}, {NOTE_B6, NOTE_EIGHTH}, {NOTE_AS6, NOTE_EIGHTH}, {NOTE_A6, NOTE_EIGHTH}, 
  
  {NOTE_G6, NOTE_QUARTER}, {NOTE_C7, NOTE_QUARTER}, {NOTE_G6, NOTE_QUARTER}, {NOTE_C6, NOTE_QUARTER}, 
  {NOTE_G6, NOTE_QUARTER}, {NOTE_C7, NOTE_QUARTER}, {NOTE_G6, NOTE_HALF},
  {NOTE_C7, DOTTED(NOTE_QUARTER)}, {NOTE_A6, NOTE_EIGHTH}, {NOTE_G6, NOTE_QUARTER}, {NOTE_F6, NOTE_QUARTER},
  {NOTE_E6, NOTE_QUARTER}, {NOTE_D6, NOTE_QUARTER}, {NOTE_C6, NOTE_QUARTER}, {REST, NOTE_QUARTER}, 

  
  {NOTE_AS6, NOTE_QUARTER}, {NOTE_DS7, NOTE_QUARTER}, {NOTE_AS6, NOTE_QUARTER}, {NOTE_DS6, NOTE_EIGHTH}, {NOTE_DS6, NOTE_EIGHTH},
  {NOTE_AS6, NOTE_QUARTER}, {NOTE_DS7, NOTE_QUARTER}, {NOTE_AS6, NOTE_HALF},  
  {NOTE_AS6, NOTE_QUARTER}, {NOTE_DS7, NOTE_QUARTER}, {NOTE_AS6, NOTE_QUARTER}, {NOTE_DS7, NOTE_QUARTER},  
  {NOTE_G7, DOTTED(NOTE_QUARTER)}, {NOTE_F7, NOTE_EIGHTH}, {NOTE_DS7, NOTE_EIGHTH}, {NOTE_D7, NOTE_EIGHTH}, {NOTE_C7, NOTE_EIGHTH}, {NOTE_B6, NOTE_EIGHTH},

  {NOTE_AS6, NOTE_QUARTER}, {NOTE_DS7, NOTE_QUARTER}, {NOTE_AS6, NOTE_QUARTER}, {NOTE_DS6, NOTE_EIGHTH}, {NOTE_DS6, NOTE_EIGHTH},
  {NOTE_AS6, NOTE_QUARTER}, {NOTE_DS7, NOTE_QUARTER}, {NOTE_AS6, NOTE_HALF},  
  {NOTE_DS7, DOTTED(NOTE_QUARTER)}, {NOTE_C7, NOTE_EIGHTH}, {NOTE_AS6, NOTE_QUARTER}, {NOTE_GS6, NOTE_QUARTER},
  {NOTE_G6, NOTE_QUARTER}, {REST, NOTE_QUARTER}, {NOTE_F6, NOTE_QUARTER}, {REST, NOTE_QUARTER}, 
  {NOTE_DS6, NOTE_QUARTER}, {REST, NOTE_QUARTER}, {REST, NOTE_HALF},
};


#define MELODY_LENGTH(m) (sizeof(m)/sizeof(m[0]))

Melody melodies[] = {
  {melody, MELODY_LENGTH(melody), 1250}, {melody3, MELODY_LENGTH(melody3), 1000}, {melody4, MELODY_LENGTH(melody4), 1000}
};

#define NUM_MELODIES(m) ((sizeof(m)/sizeof(m[0])))

void setup() {
  Serial.begin(9600);

  delay(500);//Delay to let system boot
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Temp C= ");
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor

  Serial.begin(9600);

  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }
  randomSeed(analogRead(0));
  
  
  Serial.println(F("ProtoStax Touchless Handwash Timer Demo")); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  byte numDigits = 2;
  byte digitPins[] = {32, 33};
  byte segmentPins[] = {30, 22, 23, 25, 26, 28, 27};
  
  bool resistorsOnSegments = false;
  byte hardwareConfig = COMMON_ANODE;
  bool updateWithDelays = false;
  bool leadingZeros = true;
  bool disableDecPoint = true;
  byte numberToShow = 25;
  
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);

  sevseg.setBrightness(90);
  
  playTune(melodyStart, 23, 48);
}


void loop() {
  DHT.read11(dht_apin);

  lcd.setCursor(0, 0);
  lcd.print("Temp C= ");
  lcd.print(DHT.temperature);
  lcd.setCursor(0, 1);
  lcd.print("Humidity= ");
  lcd.print(DHT.humidity);
  lcd.print(" %");
 
  Serial.print("Humidity: ");
  Serial.print(DHT.humidity);
  Serial.print("% Temperature ");
 
  Serial.print(DHT.temperature);
  Serial.print(" C ");

  static int melodyNum = 0;
  static int num = 0;
  static int startMusic = 0;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;  

  Serial.println(F("------------------------"));
  Serial.println(trigPin);
  Serial.println(echoPin);
  Serial.println(F("Duration, Distance"));
  Serial.println(duration);
  Serial.println(distance);
  Serial.println(F("------------------------\n"));

  if (distance < 10 && !startMusic) {
    startMusic = 1;
    // initializeTimer1();
    countDown = 20;
    currentTime = millis();
    melodyNum = (melodyNum+1) % (NUM_MELODIES(melodies));
  }
  if (startMusic) {
    // Pick the melody to play
    Melody mel = melodies[melodyNum];
    Note *m = mel.notes;
    int mSize = mel.numNotes;
    // speedUp is an easy way to speed up the playing of the notes. The best way would be to 
    // set the wholeNoteDurationMs appropriately. 
    int speedUp = 1; 
    
    noTone(TONE_PIN); // Start with a clean slate
    for (int thisNote = 0; thisNote < mSize; thisNote++) {
  
      // to calculate the note duration, take the duration of the whole note and multiply by the note type.
      //e.g. quarter note = wholeNoteDurationMs * 0.25, eighth note = wholeNoteDurationMs * 1/8 (0.125), etc.
      // reduce the duration by the speedup factor to increase the speed of playing 
      // by an appropriate amount
      int noteDuration = mel.wholeNoteDurationMs * m[thisNote].duration / speedUp;
      unsigned long noteTime = millis();
      tone(TONE_PIN, m[thisNote].frequency, noteDuration);
  
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      
      unsigned long nowTime = millis();
      countDown = 20 - (int)((nowTime - currentTime)/1000);

      // Look Ma No delay()!
      // Don't use delay(), as we still want to update the countDown timer 
      // and update the display
      while(millis() - noteTime <= pauseBetweenNotes) {
        countDown = 20 - (int)((millis() - currentTime)/1000);

        sevseg.setNumber(countDown,0);
        sevseg.refreshDisplay();

        delay(1);      
      }
      noTone(TONE_PIN);

      sevseg.setNumber(countDown,0);
      sevseg.refreshDisplay();
      
    }
    delay(100); 
    if (countDown <= 0) { 
      startMusic = 0;  

      sevseg.setNumber(1234,0); //after countdown shows endNumber, show this number.
      sevseg.refreshDisplay();

    }
  }

  delay(100);

}

// This function is used to play a given melody
// Arguments are:
// array of Notes for the melody
// size of array
// speedup factor - higher number plays the melody faster
// This uses delay() and is shown for reference
// It is also used in setup() to play a quick initialization music 
// This assumes that the whole note is of duration 1000 milliseconds

void playTune(Note *m, int mSize, int speedUp) {
  noTone(TONE_PIN); // Start with a clean slate
  for (int thisNote = 0; thisNote < mSize; thisNote++) {

    // to calculate the note duration, take one second multiplies by the note type.
    //e.g. quarter note = 1000.0 * 0.25, eighth note = 1000 * 1/8 (0.125), etc.
    // reduce the duration by the speedup factor to increase the speed of playing 
    // by an appropriate amount
    int noteDuration = 1000.0 * m[thisNote].duration / speedUp;
    tone(TONE_PIN, m[thisNote].frequency, noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(TONE_PIN);
  }
  delay(100);
  
}