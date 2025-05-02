// Importing the Liquid Crystal display library
#include <LiquidCrystal.h>

// === Declaring variables ===
// PINS
const int pump = 13;
const int yellowLed = 6;
const int greenLed = 4;
const int redLed = 3;
const int button = 2;

// TIMER
int seconds = 0;
int minutes = 0;
int totalTime = 0;
bool paused = false;
// Length of each period in seconds
const int onTime = 2; // 2seg
const int breakTime = 30; // 30seg
const int replayPeriod = 1800; // 30min

// DISPLAY
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

void setup() {
  // Set up the number of columns and rows on the LCD.
  lcd.begin(16, 2);

  // Pins
  pinMode(pump, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(button, INPUT);

  // Display initial message to user
  lcd.setCursor(1, 0);
  lcd.print("Aperte o botao");
  lcd.setCursor(2, 1);
  lcd.print("para iniciar!");

  // Wait for button press to start
  while (digitalRead(button) == LOW) {
    // do nothing before click the button
  }
}

void loop() {
  runPumpCycle();

  // ===== 30MIN CYCLE ENDED =====
  startEndPhase();

  // Wait 1 minute before ask for restart process
  delay(60000);
  askForRestart();
}

void startEndPhase() {
  lcd.setCursor(0, 0);
  lcd.print(" Ciclo de 30min    ");
  lcd.setCursor(0, 1);
  lcd.print("   Finalizado!     ");

  digitalWrite(yellowLed, HIGH);
  digitalWrite(pump, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, LOW);
}

void askForRestart() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Aperte o botao");
  lcd.setCursor(1, 1);
  lcd.print("para reiniciar");

  // Wait click in button to restart 
  while (digitalRead(button) == LOW) {
    // do nothing
  }
  delay(50); // debounce 50ms

  while (digitalRead(button) == HIGH) {
    // do nothing
  }
  delay(50);

  totalTime = 0;
  digitalWrite(yellowLed, LOW);
}

void runPumpCycle() {
  while (totalTime < replayPeriod) {
    // ===== PUMP ON =====
    startPumpOnPhase();
    controlDisplayTimer(onTime);

    // ===== PUMP OFF =====
    startPumpOffPhase();
    controlDisplayTimer(breakTime);
  }
}

void controlDisplayTimer(int cycleTime) {
  seconds = cycleTime;

  digitalWrite(button, LOW);
  paused = false;

  while (seconds >= 0) {
    if (cycleTime == onTime) {
      lcd.setCursor(11, 1);
    } else {
      lcd.setCursor(8, 1);
    }

    if (!paused) {
      formatTime(seconds); // Format MM:SS 
    }

    // Break 1seg into ten 100 ms chunks 
    // To still detect a button press
    for (int i = 0; i < 10; i++) {
      delay(100);
      handlePause(cycleTime);
    }

    // Only decrement when not paused
    if (!paused) {
      seconds--;
      totalTime++;
    }
  }
}

void handlePause(int cycleTime) {
  bool prevState = paused;

  if (digitalRead(button) == HIGH) {
    delay(50);
    while (digitalRead(button) == HIGH) {}
    delay(50);
    paused = !paused;
  }

  bool continueProcess = prevState != paused && !paused;

  if (continueProcess) {
    continueBeforePhase(cycleTime);
  }

  if (paused) {
    startPausedPhase();
  }
}

void startPausedPhase() {
  lcd.setCursor(0, 0);
  lcd.print("   PAUSADO no    ");
  lcd.setCursor(0, 1);
  lcd.print("  tempo ");
  lcd.setCursor(8, 1);
  formatTime(totalTime);
  lcd.setCursor(13, 1);
  lcd.print("   ");

  digitalWrite(yellowLed, HIGH);
  digitalWrite(pump, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, LOW);
}

void continueBeforePhase(int cycleTime) {
  if (cycleTime == onTime) {
    startPumpOnPhase();
  } else {
    startPumpOffPhase();
  }
}

void startPumpOnPhase() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bomba LIGADA!");
  lcd.setCursor(0, 1);
  lcd.print("Desliga em");

  digitalWrite(yellowLed, LOW);
  digitalWrite(pump, HIGH);
  digitalWrite(greenLed, HIGH);
  digitalWrite(redLed, LOW);
}

void startPumpOffPhase() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bomba DESLIGADA!");
  lcd.setCursor(0, 1);
  lcd.print("Liga em");

  digitalWrite(yellowLed, LOW);
  digitalWrite(pump, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, HIGH);
}

void formatTime(int seconds) {
  int displayMinutes = seconds / 60;
  int displaySeconds = seconds % 60;

  if (displayMinutes < 10) {
    lcd.print('0');
  }
  lcd.print(displayMinutes);
  lcd.print(':');
  if (displaySeconds < 10) {
    lcd.print('0');
  }
  lcd.print(displaySeconds);
}