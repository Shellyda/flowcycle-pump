// Importing the Liquid Crystal display library
#include <LiquidCrystal.h>

// === Declaring variables ===
// PINS
const int motorPin = 13;
const int yellowLed = 6;
const int greenLed = 4;
const int redLed = 3;
const int button = 2;

// TIMER
int seconds = 0;
int minutes = 0;
int count = 0;
// Length of each period in seconds
const int onTime = 2; // 2seg
const int breakTime = 30; // 30seg
const int replayPeriod = 1800; // 30min

// DISPLAY
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);


void setup()
{
  // Set up the number of columns and rows on the LCD.
  lcd.begin(16, 2); 

  pinMode(motorPin, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  
  // Display initial message to user
  lcd.setCursor(1,0);
  lcd.print("Aperte o botao");
  lcd.setCursor(2,1);
  lcd.print("para iniciar!");
  
  // Wait for button press to start
  while(digitalRead(button) == LOW){
    // do nothing before click the button
  }
}

void loop()
{  
    runPumpCycle();
  
  // ===== 30MIN CYCLE ENDED =====
    printEndMessage();
  
  // === Wait 1 minute before ask for restart process ===
  	delay(60000);
	askForRestart();
}

void printEndMessage()
{
   lcd.setCursor(0, 0);
   lcd.print(" Ciclo de 30min    ");
   lcd.setCursor(0, 1);
   lcd.print("   Finalizado!     ");
   
   digitalWrite(yellowLed, HIGH);
   digitalWrite(motorPin, LOW);
   digitalWrite(greenLed, LOW);
   digitalWrite(redLed, LOW);
}
  
void askForRestart() 
{
   lcd.clear();
   lcd.setCursor(1, 0);
   lcd.print("Aperte o botao");
   lcd.setCursor(1, 1);
   lcd.print("para reiniciar");

  // === Wait click in button to restart ===
   while (digitalRead(button) == LOW) {
    // do nothing
   }
   delay(50); // debounce 50ms
  
   while (digitalRead(button) == HIGH) {
     // do nothing
   }
  delay(50);
  count = 0; 
  digitalWrite(yellowLed, LOW);
}

void runPumpCycle() 
{  
  while (count < replayPeriod) {
    // ===== PUMP ON =====
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bomba LIGADA!");  
    lcd.setCursor(0, 1);
    lcd.print("Desliga em");

    digitalWrite(motorPin, HIGH);
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
    controlDisplayTimer(onTime);

    // ===== PUMP OFF =====
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bomba DESLIGADA!");
    lcd.setCursor(0, 1);
    lcd.print("Liga em");

    
    digitalWrite(motorPin, LOW);
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, HIGH);
    controlDisplayTimer(breakTime);
  }
}

void controlDisplayTimer(int cycleTime) 
{
  seconds = cycleTime;

  while (seconds >= 0) {
      // Calculate minutes and seconds
      int displayMinutes = seconds / 60;
      int displaySeconds = seconds % 60;

      // Shows in MM:SS format
      if(cycleTime == 2){ 
        lcd.setCursor(11, 1); 
      } else {
        lcd.setCursor(8, 1);
      }

      if (displayMinutes < 10) {
        lcd.print("0");
      }
      lcd.print(displayMinutes);
      lcd.print(":");
      if (displaySeconds < 10) {
        lcd.print("0");
      }
      lcd.print(displaySeconds);
	
      // Wait 1 second to decrement seconds
      delay(1000);
      seconds--;
      count++;
  }  
}