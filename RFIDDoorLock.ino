/**************************************************
Contributors:       Fawaaz Kamali Siddiqui, Parin Patel
---------------------------------------------------
Last Update:        1-August-2023
===================================================
Description:        Using an RFID module, only certain tags
                    and cards were given the ability to unlock
                    a door. The door has a bolt lock which is
                    connected to a servo through a wire. As the 
                    servo moves, the lock moves with it causing 
                    the door to lock or unlock. This project was
                    completed with an Arduino Uno.
===================================================
Connections:        rs:         3
                    en:         2
                    d4:         4
                    d5:         5
                    d6:         6
                    d7:         7
                    servo:      8      
                    SDA:        10
                    SCK:        13
                    MOSI:       11
                    MISO:       12
                    RST:        9
                    buzzer:     A0                                  
**************************************************/

#include <Servo.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>




// Variable Declarations 
int pos = 0;    // keep track of servo position
int servoPin = 8;
int buzzerPin = A0;

int rs = 3;
int e = 2;
int d4 = 4;
int d5 = 5;
int d6 = 6;
int d7 = 7;

#define SS_PIN 10
#define RST_PIN 9
// #define LED 8
byte readCard[4];
String tag_UID = "C3DBF3F6";   
String card_UID = "73B4A1FC";
String tagID = "";

// Instantiations - for devices with seperate libraries
Servo servo;
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance


void setup() {

    // Initialize connections 
    Serial.begin(9600);
    servo.attach(servoPin);

    pinMode(buzzerPin, OUTPUT);

    lcd.begin(16, 2);

    SPI.begin(); // SPI bus
    mfrc522.PCD_Init(); // Initialise MFRC522


}

void loop() {

    // Main program 

    //Wait until new tag is available
    servo.write(pos);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Scan your card:");
    while (readID())
    {
        if (tagID == tag_UID || tagID == card_UID)
        {
            // digitalWrite(LED, !digitalRead(LED));  // Turn on or off the onboard led
            unlockDoor();
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Access:");
            lcd.setCursor(0,1);
            lcd.print("Authorized");
            delay(5000);    // open lock for 5 seconds, and then close it back
            lockDoor();
        }
        else 
        {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Access:");
            lcd.setCursor(0,1);
            lcd.print("Denied");
            analogWrite(buzzerPin, 255);
            delay(2000);
            analogWrite(buzzerPin, LOW);
        }
    }

}

void unlockDoor() {

    // rotate clockwise
    for (pos = 0; pos <= 180; pos++) {
        servo.write(pos);
        delay(15);  
    }
}

void lockDoor() {

    // rotate anti-clockwise
    for (pos = 180; pos >= 0; pos--) {
        servo.write(pos);
        delay(15);
    }
}

bool readID()
{
    //Check if a new tag is detected or not. If not return.
    if ( ! mfrc522.PICC_IsNewCardPresent())
    {
        return false;
    }
    //Check if a new tag is readable or not. If not return.
    if ( ! mfrc522.PICC_ReadCardSerial())
    {
        return false;
    }
    tagID = "";
    // Read the 4 byte UID
    for ( uint8_t i = 0; i < 4; i++)
    {
        //readCard[i] = mfrc522.uid.uidByte[i];
        tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Convert the UID to a single String
    }
    tagID.toUpperCase();
    mfrc522.PICC_HaltA(); // Stop reading
    return true;
  }




