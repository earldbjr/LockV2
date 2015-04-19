//Fuses H:0xDE L:0xE2 E:0x05

#include <LiquidCrystal.h>
#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"

#define SS_PIN 10
#define RST_PIN 9
#define lcdBacklight 5
#define FPSPower 4
#define lockPin1 7
#define lockPin2 8

int isLocked = 0;
LiquidCrystal lcd(A5,A4,A3,A2,A1,A0);
FPS_GT511C3 fps(3,2);

void setup()
{
  lcd.begin(16,2);
  pinMode(FPSPower, OUTPUT);
  digitalWrite(FPSPower, HIGH); //To be toggled later...
  delay(100);
  fps.Open();
  fps.SetLED(true);
}
void checkBiometrics();
void checkRFID();
void unlock();
void lock();
void doorWatcher(); //Blocking!

void checkBiometrics()
{
  // Identify fingerprint testx
  if (fps.IsPressFinger())
  {
    fps.CaptureFinger(false);
    int id = fps.Identify1_N();
    if (id <200)
    {
      lcd.setCursor(0,0);
      lcd.print("Verified ID:");
      lcd.setCursor(0,1);
      lcd.print(id);
      unlock();
      doorWatcher(); //Blocking!
    }
    else
    {
      lcd.setCursor(0,1);
      lcd.print("F");
    }
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("FPS Ready   ");
  }
  delay(100); 
}

void checkRFID()
{
  
}

void loop()
{
  checkBiometrics();
  checkRFID();
}

void unlock()
{
  digitalWrite(lockPin1, LOW);
  digitalWrite(lockPin2, HIGH);
  isLocked = 0;
  delay(1200);
  digitalWrite(lockPin1, LOW);
  digitalWrite(lockPin2, LOW);
}

void lock()
{
  digitalWrite(lockPin1, HIGH);
  digitalWrite(lockPin2, LOW);
  isLocked = 1;
  delay(1200); //Allow motor to engage.
  digitalWrite(lockPin1, LOW);
  digitalWrite(lockPin2, LOW);
}

void doorWatcher()
{
 //Put 10 second timeout here, and break block.
 //Block code execution until door opened and closed, then run lock. 
}
