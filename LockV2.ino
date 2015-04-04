//Fuses H:0xDE L:0xE2 E:0x05

#include <LiquidCrystal.h>
#include <SPI.h>
#include "MFRC522.h"

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.

int isLocked = 0;
int doorClosed = 2;
int lockPin1 = 7;
int lockPin2 = 8;
String card1 = "2454512237";   //Given to Sun Bay office
String card2 = "401641025450129"; //Zahrah's Purse
String card3 = "166204210181"; //Zahrah's Keychain
String card4 = "4201651025450129";
String card5 = "4201651025450129";   //Gizmo's Wallet
String card6 = "2454512237";
/*Blacklisted numbers:
 2445113213 Blank white card - Lost
 */
 void offLock();
 void unlockDoor();
 void lockDoor();
 void timeoutTimer();

LiquidCrystal lcd(A5,A4,A3,A2,A1,A0);//RS,EN,D4,D5,D6,D7

void setup() 
{
	lcd.begin(16, 2);
  pinMode(5, OUTPUT); //LCD backlight
  digitalWrite(5, LOW);
  SPI.begin();                // Initialize SPI bus
  mfrc522.PCD_Init();        // Initialize MFRC522 card
  pinMode(lockPin1, OUTPUT);
  pinMode(lockPin2, OUTPUT);
  pinMode(doorClosed, INPUT);
}

void loop() 
{
//Check RFID
//Check FPS
//Check Reed
//Check LCD backlight timer
String idRead;
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) 
  {
  	key.keyByte[i] = 0xFF;
  }
  // New card found && new card selected
  if ( mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
  {
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    { // Dump UID for authentication
    	idRead.concat(mfrc522.uid.uidByte[i]);
    }
    digitalWrite(5, HIGH);
    lcd.setCursor(0,0);
    lcd.print("Bad Card!");
    lcd.setCursor(0,1);
    lcd.print(idRead);
    lcd.setCursor(0,0);
    if (idRead == card1)
    {
    	lcd.print("Card 1 read!");
    	lcd.setCursor(0,1);
    	lcd.print(idRead);
    }
    if (idRead == card2)
    {
    	lcd.print("Card 2 read!");
    	lcd.setCursor(0,1);
    	lcd.print(idRead);
    }    
    if (idRead == card3)
    {
    	lcd.print("Card 3 read!");
    	lcd.setCursor(0,1);
    	lcd.print(idRead);
    }   
    if (idRead == card4)
    {
    	lcd.print("Card 4 read!");
    	lcd.setCursor(0,1);
    	lcd.print(idRead);
    }    
    if (idRead == card5)
    {
    	lcd.print("Card 5 read!");
    	lcd.setCursor(0,1);
    	lcd.print(idRead);
    }    
    if (idRead == card6)
    {
    	lcd.print("Card 6 read!");
    	lcd.setCursor(0,1);
    	lcd.print(idRead);
    }

      //if(isLocked == 1){
      //  unlockDoor();
      delay(3000);
      digitalWrite(5, LOW);
      lcd.clear();
  }
      checkdoorClosed();
      timeoutTimer();
}
void offLock()
{
 	digitalWrite(lockPin1, LOW);
  	digitalWrite(lockPin2, LOW);
  	delay(100); 
}

void unlockDoor()
{
  	digitalWrite(lockPin1, LOW);
  	digitalWrite(lockPin2, HIGH);
  	isLocked = 0;
	delay(100); //Allow motor to engage.
	offLock();
}

void lockDoor()
{
	digitalWrite(lockPin1, HIGH);
	digitalWrite(lockPin2, LOW);
	isLocked = 1;
	delay(1200); //Allow motor to engage.
	offLock();
}

void checkdoorClosed()
{
	int safe = 1;
	static int named = 0;
	int sensorReading = digitalRead(doorClosed);
	if(sensorReading == 0){
		isLocked = 0;
		named = 0;
	}
  else if (sensorReading == 1 && isLocked == 0 && named == 0)
  {  //If door is closed(guaranteed, but checked for sanity), and door wasn't locked last iteration

  	unsigned long time = millis();
    while(millis()-time <= 2000)
    	{ //Test for two seconds.
    		if(digitalRead(doorClosed) == 0)
    		{ //If door reads open during two seconds...
        	isLocked = 0;       //door must be unlocked if open
        	safe = 0;
    		}
		}
		if(safe == 1)
		{
    		lockDoor();             //Lock the door. 
    		named = 1;
		}
	}
	else 
	{   
	 //Just to document if Reading == 1, isLocked == 1;
	}
}

void timeoutTimer()
{
	signed static long oldTime = millis();
	if(isLocked == 1 || digitalRead(doorClosed) == 0)
	{
    	oldTime = -1; //lockout this function
    	return;
	}
	if(oldTime == -1)
	{ //Implies door is unlocked
  		oldTime = millis(); //Start the clock
	}
	if(millis()-oldTime >= 10000)
	{ //If door has been unlocked for greater than 10 seconds, but not opened...
  		lockDoor();
    	oldTime = -1; //lockout this function until door is closed but unlocked.
	}
}
