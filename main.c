#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo gasServo;

// Pin Definitions
const int gasSensor = 34;      // MQ-2 Analog
const int smokeSensor = 35;    // Smoke Sensor
const int flameSensor = 27;    // IR/Flame Sensor
const int led = 2;
const int buzzer = 4;
const int servoPin = 18;

// Threshold values
const int gasThreshold = 500;
const int smokeThreshold = 500;

void setup()
{
  Serial.begin(115200);

  pinMode(flameSensor, INPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);

  gasServo.attach(servoPin);
  gasServo.write(0);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Safety System");
  lcd.setCursor(0,1);
  lcd.print("Initializing...");
  delay(2000);
}

void loop()
{
  int gasValue = analogRead(gasSensor);
  int smokeValue = analogRead(smokeSensor);
  int flameValue = digitalRead(flameSensor);

  lcd.clear();

  if(gasValue > gasThreshold)
  {
      digitalWrite(led, HIGH);
      digitalWrite(buzzer, HIGH);

      gasServo.write(90);

      lcd.print("Gas Leakage!");
      lcd.setCursor(0,1);
      lcd.print("Valve Closed");

      Serial.println("Gas Detected");
  }
  else if(smokeValue > smokeThreshold)
  {
      digitalWrite(led, HIGH);
      digitalWrite(buzzer, HIGH);

      lcd.print("Smoke Found");

      Serial.println("Smoke Detected");
  }
  else if(flameValue == LOW)
  {
      digitalWrite(led, HIGH);
      digitalWrite(buzzer, HIGH);

      lcd.print("Fire Alert!");

      // Future Feature
      // Activate Water Pump Here

      Serial.println("Fire Detected");
  }
  else
  {
      digitalWrite(led, LOW);
      digitalWrite(buzzer, LOW);

      gasServo.write(0);

      lcd.print("System Normal");
  }

  delay(500);
}
