/**
 * The goal of this program is to make a simple Arduino music player that plays an 
 * arbitrary-length melody without using the tone() call with the third argument 
 * (duration), which blocks all other processing while playing. This program achieves 
 * that, and also simultaneously blinks the board LED, to prove that other processing 
 * continues while the music plays.
 *
 * The music is the opening bars of Bach's Bwv 565 Toccata in d minor. Why? Why not.
 */

#include "pitches.h"

// Pin Assignments:
const int speaker = 8;
// Choosing pin 13 because that's the onboard LED; no need for any other ciruit.
const int boardled = 13;

/**
 * Defines a single note in a song.
 */
typedef struct _onenote {
  // The Pitch, in Hz.
  int pitch;

  // The duration. 4 = a quarter note; 2 = half note.
  int duration;
} onenote;

/**
 * Opening notes to Bach's Bwv565 Toccata in d minor.
 */
onenote melody[] = {
  {NOTE_A5, 4}, {NOTE_G5, 4}, {NOTE_A5, 4}, {0, 4},
  {NOTE_G5, 12}, {NOTE_F5, 12}, {NOTE_E5, 12}, {NOTE_D5, 12}, {NOTE_CS5, 4}, {NOTE_D5, 4}, {0, 2},
  {NOTE_A4, 4}, {NOTE_G4, 4}, {NOTE_A4, 4}, {0, 4},
  {NOTE_E4, 4}, {NOTE_F4, 4}, {NOTE_CS4, 4}, {NOTE_D4, 4}, {0, 2},
  {NOTE_A3, 4}, {NOTE_G3, 4}, {NOTE_A3, 4}, {0, 4},
  {NOTE_G3, 12}, {NOTE_F3, 12}, {NOTE_E3, 12}, {NOTE_D3, 12}, {NOTE_CS3, 4}, {NOTE_D3, 4}
};

/**
 * Blinks the LED on the specified pin.
 */
class Blinkenlights
{
  int ledPin;
  long onTime;
  long offTime;

  // State:
  int ledState;
  unsigned long prevMillis;

  public:

  /**
   * Class Constructor.
   * pin: Which pin to operate on
   * on: How long to keep the LED on, in milliseconds
   * off: How long to keep the LED off, in milliseconds
   */
  Blinkenlights(int pin, long on, long off)
  {
    ledPin = pin;
    pinMode(ledPin, OUTPUT);

    onTime = on;
    offTime = off;

    ledState = LOW;
    prevMillis = 0;
  }

  /**
   * Updates the pin's state when called.
   */
  void Update()
  {
    unsigned long currMillis = millis();

    if ((ledState == HIGH) && ((currMillis - prevMillis) >= onTime))
    {
      ledState = LOW;
      prevMillis = currMillis;
      digitalWrite(ledPin, ledState);
    }
    else if ((ledState == LOW) && ((currMillis - prevMillis) >= offTime))
    {
      ledState = HIGH;
      prevMillis = currMillis;
      digitalWrite(ledPin, ledState);    
    }
  }
};


/**
 * Play the opening statements of Bach Bwv565 Toccata in d minor.
 */
class Bwv565
{
  int pin;
  int songlength;

  // State:
  bool quiet;
  bool playingnow;
  int note;
  int prevMillis;
  int currDuration;

  /**
   * Class Constructor.
   * pin: Which pin has the speaker attached.
   */
  public:
  Bwv565(int speakerPin) {
    songlength = sizeof(melody) / sizeof(onenote);
    pin = speakerPin;
    note = 0;
    prevMillis = 0;
    currDuration = 0;
    quiet = true;
    playingnow = false;
  }

  void Play() {
    quiet = false;
    prevMillis = millis();
    // Note duration is one second divided by the note type.
    // A quarter note = 1000 / 4, eighth note = 1000/8, etc.
    currDuration = 1000 / melody[note].duration;
    // A pitch of 0 is a rest. Don't call tone.
    if (melody[note].pitch != 0) {
      tone(pin, melody[note].pitch);
    }
    playingnow = true;
  }

  /**
   * Is this playing notes?
   */
  bool IsPlaying() {
    return !quiet;
  }

  /**
   * Pause playing. If Play() is called next, it will resume playback
   * where stopped. To reset to the beginning of the song, call the
   * FromTheTop() method.
   */
  void Pause() {
    noTone(pin);
    quiet = true;
  }

  /**
   * Reset playback to the beginning of the song.
   */
  void FromTheTop() {
    note = 0;
    prevMillis = 0;
    currDuration = 0;
  }

  /**
   * Call this periodically from loop() while you want the notes to play.
   */
  void Update() {
    // If not currently playing, exit method ASAP.
    if (quiet) {
// Don't believe this is necessary after all:      noTone(pin);
      return;
    }

    int currMillis = millis();
    // Is it time to play the next note?
    if ((currMillis - prevMillis) >= currDuration) {
      // Stop playing the last note.
      noTone(pin);
      note++;

      // Might be the end of the song.
      if (note >= songlength) {
        // Yes, it's the end. Reset for next time.
        quiet = true;
        note = 0;
        prevMillis = 0;
        currDuration = 0;
        playingnow = false;
      } else {
        // No, play the next note.
        prevMillis = currMillis;
        // Note duration is one second divided by the note type.
        // A quarter note = 1000 / 4, eighth note = 1000/8, etc.
        currDuration = 1000 / melody[note].duration;
        // A pitch of 0 is a rest. Don't call tone.
        if (melody[note].pitch != 0) {
          tone(pin, melody[note].pitch);
        }
        playingnow = true;
      }
    }
  }
};

Blinkenlights blinker(boardled, 50, 450);
Bwv565 bwv565(speaker);

/**
 * Run Once.
 */
void setup() {
  bwv565.Play();
}

/**
 * Main Loop.
 */
void loop() {
  blinker.Update();
  bwv565.Update();
}

