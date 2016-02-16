//Fuses H:0xDE L:0xE2 E:0x05 For 8MHz internal atmega328p

#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>
#include <FPS_GT511C3.h> // My non-blocking version
#include "SoftwareSerial.h"
//RFID
#define SS_PIN 10
#define RST_PIN 9

#define lcdBacklight 5
#define lockPin1 7
#define lockPin2 8
#define doorSensor 4
/////Forward Declarations/////
void checkBiometrics();
void checkRFID();
void unlockDoor();
void lockDoor();
void doorWatcher(); //Blocking! For 5 sec.
void lcdBacklightChecker();
/////////////////////////////

long lcdBacklightTimer = 0;
int isLocked = 0;
int lockTravelTime = 1200; //Time in mS for relay to be engaged.
String card1 = "2454512237";   //Given to Sun Bay office
String card2 = "101237102";  //Gizmo's Keychain
String card3 = "166204210181"; //Zahrah's Keychain
String card4 = "101237102";  //unused
String card5 = "4171671025450129";   //Gizmo's Wallet
String card6 = "2454512237"; //unused

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0); //4-bit mode
FPS_GT511C3 fps(2, 3);

void setup()
{
  lcd.begin(16, 2);
  pinMode(lcdBacklight, OUTPUT);
  pinMode(lockPin1, OUTPUT);
  pinMode(lockPin2, OUTPUT);
  pinMode(doorSensor, INPUT);
  digitalWrite(lcdBacklight, HIGH);
  SPI.begin();
  rfid.PCD_Init();
  delay(100);
  fps.Open();
  delay(100);
  fps.SetLED(true);
  lockDoor();
}


void loop()
{
  checkBiometrics();
  checkRFID();
  lcdBacklightChecker();
}

void checkBiometrics()
{
  fps.SetLED(true);
  // Identify fingerprint
  if (fps.IsPressFinger())
  {
    fps.CaptureFinger(true); //false indicates faster low-res mode
    int id = fps.Identify1_N();
    if (id < 200) //200 finger profiles
    {
      digitalWrite(lcdBacklight, HIGH);
      lcdBacklightTimer = millis();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Verified ID:");
      lcd.setCursor(0, 1);
      lcd.print(id);
      unlockDoor();
      return;
    }
    else
    {
      digitalWrite(lcdBacklight, HIGH);
      lcdBacklightTimer = millis();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Invalid Finger");
      fps.SetLED(false);
      return;
    }
  }
}

void checkRFID()
{
  String idRead;
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  // New card found && new card selected
  if ( rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Found!");
    for (byte i = 0; i < rfid.uid.size; i++)
    { // Dump UID for authentication
      idRead.concat(rfid.uid.uidByte[i]);
    }
    digitalWrite(lcdBacklight, HIGH);
    lcdBacklightTimer = millis();
    lcd.setCursor(0, 1);
    lcd.print(idRead);
    delay(500);

    if (
      idRead == card1 ||
      idRead == card2 ||
      idRead == card3 ||
      idRead == card4 ||
      idRead == card5 ||
      idRead == card6)
    {
      digitalWrite(lcdBacklight, HIGH);
      lcdBacklightTimer = millis();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Card Accepted!");
      lcd.setCursor(0, 1);
      lcd.print(idRead);
      if (isLocked == 1)
      {
        unlockDoor();
        return;
      }
    } else
    {
      digitalWrite(lcdBacklight, HIGH);
      lcdBacklightTimer = millis();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Invalid Card ID:");
      lcd.setCursor(0, 1);
      lcd.print(idRead);
      return;
    }
  }
}


void doorWatcher()
{
  long startTime = millis();
  while (digitalRead(doorSensor) == 1)
  {
    if (millis() - startTime >= 10000) //Catch door being unlocked, but not opened for 10 seconds, and relock.
    {
      lockDoor();
      return;
    }
  }
doorOpen:
  while (digitalRead(doorSensor) == 0) //Door found to be open. Wait for door close.
  {
    //Add future alarm for door left unsecure? Flash LCDBacklight?
  }
  startTime = millis(); //New timer value for door-closed check.
  while (millis() - startTime <= 2000) //Door re-closed. Scan while waiting, and relock.
  {
    if (digitalRead(doorSensor) == 0)
    {
      goto doorOpen;
    }
  }
  //Door unlocked, opened, closed, and sat for 2 seconds:
  lockDoor();
}

void unlockDoor()
{
  if (digitalRead(doorSensor) == HIGH) //If door is closed...
  {
    digitalWrite(lockPin1, LOW);
    digitalWrite(lockPin2, HIGH);
    isLocked = 0;
    delay(lockTravelTime);
    digitalWrite(lockPin1, LOW);
    digitalWrite(lockPin2, LOW);
    doorWatcher(); //blocking for 10sec
  }
}

void lockDoor()
{
  if (digitalRead(doorSensor) == HIGH) //If door is closed...
  {
    digitalWrite(lockPin1, HIGH);
    digitalWrite(lockPin2, LOW);
    isLocked = 1;
    delay(lockTravelTime); //Allow motor to engage.
    digitalWrite(lockPin1, LOW);
    digitalWrite(lockPin2, LOW);
  }
}

void lcdBacklightChecker()
{
  if (millis() - lcdBacklightTimer >= 5000)
  {
    lcd.clear();
    digitalWrite(lcdBacklight, LOW);
  }
}
