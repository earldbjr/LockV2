//Fuses H:0xDE L:0xE2 E:0x05

#include <LiquidCrystal.h>
#include <SPI.h>
#include "MFRC522.h"
#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"

#define SS_PIN 10
#define RST_PIN 9
#define lcdBacklight 5
#define FPSPower 4
#define lockPin1 7
#define lockPin2 8

int isLocked = 0;
String card1 = "2454512237";   //Given to Sun Bay office
String card2 = "401641025450129"; //Zahrah's Purse
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
  rfid.PCD_Init();
  lcd.begin(16,2);
  pinMode(FPSPower, OUTPUT);
  digitalWrite(FPSPower, HIGH); //To be toggled later...
  delay(100);
  fps.Open();
  delay(100);
  fps.SetLED(true);
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

  lcd.setCursor(0,0);
  lcd.print("RFIDReady");
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

     lcd.setCursor(0,0);
     lcd.print("Found!");
    for (byte i = 0; i < rfid.uid.size; i++) 
    { // Dump UID for authentication
      idRead.concat(rfid.uid.uidByte[i]);
    }
    digitalWrite(lcdBacklight, HIGH);
    lcd.setCursor(0,1);
    lcd.print(idRead);
    delay(5000);

    if (idRead == card1)
    {
      lcd.print("Card 1 read!");
      lcd.setCursor(0,1);
      lcd.print(idRead);
      if(isLocked == 1){
        unlock();
      }
    }
    if (idRead == card2)
    {
      lcd.print("Card 2 read!");
      lcd.setCursor(0,1);
      lcd.print(idRead);
      if(isLocked == 1){
        unlock();
      }
    }    
    if (idRead == card3)
    {
      lcd.print("Card 3 read!");
      lcd.setCursor(0,1);
      lcd.print(idRead);
      if(isLocked == 1){
        unlock();
      }
    }   
    if (idRead == card4)
    {
      lcd.print("Card 4 read!");
      lcd.setCursor(0,1);
      lcd.print(idRead);
      if(isLocked == 1){
        unlock();
      }
    }    
    if (idRead == card5)
    {
      lcd.print("Card 5 read!");
      lcd.setCursor(0,1);
      lcd.print(idRead);
      if(isLocked == 1){
        unlock();
      }
    }    
    if (idRead == card6)
    {
      lcd.print("Card 6 read!");
      lcd.setCursor(0,1);
      lcd.print(idRead);
      if(isLocked == 1){
        unlock();
      }
    }

    delay(5000);
    digitalWrite(lcdBacklight, LOW);
    lcd.clear();
  }
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

