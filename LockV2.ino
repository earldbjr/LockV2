//Fuses H:0xDE L:0xE2 E:0x05

#include <LiquidCrystal.h>
#include <SPI.h>
#include "MFRC522.h"
#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"
#include <FPS.h>;
#include <RFID.h>;

#define SS_PIN 10
#define RST_PIN 9
#define lcdBacklight 5
#define FPSPower 4
#define lockPin1 7
#define lockPin2 8

int isLocked = 0;
String card1 = "2454512237";   //Given to Sun Bay office
String card2 = "4201651025450129";
String card3 = "166204210181"; //Zahrah's Keychain
String card4 = "4201651025450129";
String card5 = "4201651025450129";   //Gizmo's Wallet
String card6 = "2454512237";

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal lcd(A5,A4,A3,A2,A1,A0);
FPS_GT511C3 fps(3,2);

void setup()
{
  SPI.begin();
  lcd.begin(16,2);
  lcd.print("setup");
  pinMode(FPSPower, OUTPUT);
  digitalWrite(FPSPower, HIGH); //To be toggled later...
  delay(100);
  lcd.print("Powered");
  fps.Open();
  fps.SetLED(true);
  lcd.print("true");
}
void checkBiometrics();
void checkRFID();
void unlock();
void lock();
void doorWatcher(); //Blocking!





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

