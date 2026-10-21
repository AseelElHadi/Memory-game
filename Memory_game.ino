// --- Pin Definitions ---
const int redLed = 8;
const int blueLed = 9;
const int whiteLed = 10;
const int greenLed = 11;

const int redBtn = 7;
const int blueBtn = 6;
const int whiteBtn = 5;
const int greenBtn = 4;

const int buzzer = 3;

// --- Game Variables ---
int sequence[100];   // Stores the game sequence (up to 100 rounds)
int gameLength = 0;  // Current length of the sequence

void setup() {
  // Set LED pins as outputs
  pinMode(redLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(whiteLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  
  // Set Button pins as inputs with internal pull-ups
  pinMode(redBtn, INPUT_PULLUP);
  pinMode(blueBtn, INPUT_PULLUP);
  pinMode(whiteBtn, INPUT_PULLUP);
  pinMode(greenBtn, INPUT_PULLUP);
  
  // Set Buzzer pin as output
  pinMode(buzzer, OUTPUT);
  
  // Seed random number generator using an unconnected analog pin
  randomSeed(analogRead(A0));
  
  // Play startup greeting
  playStartupSequence();
  delay(1000);
}

void loop() {
  // Add a new random step to the sequence (0: Red, 1: Blue, 2: White, 3: Green)
  sequence[gameLength] = random(0, 4);
  gameLength++;
  
  // Play the sequence back to the player
  playSequence();
  
  // Wait for player input and verify correctness
  if (!getPlayerInput()) {
    // If the player fails or presses the wrong button
    playGameOverSound();
    delay(1500);
    gameLength = 0; // Reset the game back to round 1
  } else {
    // If the player successfully completed the sequence, wait before next round
    delay(1000);
  }
}

// --- Helper Functions ---

// Plays back the entire sequence generated so far
void playSequence() {
  for (int i = 0; i < gameLength; i++) {
    lightColor(sequence[i], 450);
    delay(200); // Pause between flashes in the sequence
  }
}

// Lights up an LED and plays a corresponding pitch
void lightColor(int colorIndex, int duration) {
  int ledPin = 0;
  int freq = 0;
  
  switch(colorIndex) {
    case 0: ledPin = redLed;   freq = 262; break; // Red (C4)
    case 1: ledPin = blueLed;  freq = 330; break; // Blue (E4)
    case 2: ledPin = whiteLed; freq = 392; break; // White (G4)
    case 3: ledPin = greenLed; freq = 523; break; // Green (C5)
  }
  
  digitalWrite(ledPin, HIGH);
  tone(buzzer, freq);
  delay(duration);
  digitalWrite(ledPin, LOW);
  noTone(buzzer);
}

// Listens for player button presses and validates them in real time
bool getPlayerInput() {
  for (int i = 0; i < gameLength; i++) {
    int pressedColor = -1;
    
    // Wait until one of the buttons is pressed
    while (pressedColor == -1) {
      if (digitalRead(redBtn) == LOW) {
        pressedColor = 0;
        lightColor(0, 250);
        waitForButtonRelease(redBtn);
      } else if (digitalRead(blueBtn) == LOW) {
        pressedColor = 1;
        lightColor(1, 250);
        waitForButtonRelease(blueBtn);
      } else if (digitalRead(whiteBtn) == LOW) {
        pressedColor = 2;
        lightColor(2, 250);
        waitForButtonRelease(whiteBtn);
      } else if (digitalRead(greenBtn) == LOW) {
        pressedColor = 3;
        lightColor(3, 250);
        waitForButtonRelease(greenBtn);
      }
    }
    
    // Check if the pressed button matches the expected sequence item
    if (pressedColor != sequence[i]) {
      return false; // Wrong button pressed!
    }
  }
  return true; // Sequence completed successfully
}

// Simple debounce helper to wait until the button is let go
void waitForButtonRelease(int btnPin) {
  while (digitalRead(btnPin) == LOW) {
    delay(10);
  }
  delay(50); // Small debounce pause
}

// Startup animation
void playStartupSequence() {
  for (int i = 0; i < 4; i++) {
    lightColor(i, 150);
    delay(80);
  }
}

// Game over buzzer tone and flash all LEDs
void playGameOverSound() {
  tone(buzzer, 120);
  digitalWrite(redLed, HIGH);
  digitalWrite(blueLed, HIGH);
  digitalWrite(whiteLed, HIGH);
  digitalWrite(greenLed, HIGH);
  delay(800);
  noTone(buzzer);
  digitalWrite(redLed, LOW);
  digitalWrite(blueLed, LOW);
  digitalWrite(whiteLed, LOW);
  digitalWrite(greenLed, LOW);
}