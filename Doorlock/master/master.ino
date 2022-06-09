#include <Wire.h>
#include <Servo.h>
#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

#define PICC_0 0x51
#define PICC_1 0xA2
#define PICC_2 0x5E
#define PICC_3 0x1A
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

byte nuidPICC[4];

Servo myServo;
int angle = 90;

const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {8, 7, 6, 5};
byte colPins[COLS] = {4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys),rowPins, colPins, ROWS, COLS);

//비밀번호
String password = "0426";
String userInput = "";

int count = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  
  readkeypad();
  readRFID();
}

void readkeypad(){
  
  char key = keypad.getKey();

  if(key){
    userInput += key;
    count += 1;
    tone(14, 523, 200);
    delay(500);
    
    if(count == 5)
    {
      count = 0;
      if(userInput == password + '#')
      {
        Wire.beginTransmission(8);
        Wire.write(1);
        Wire.endTransmission();
        tone(14, 523, 200);
        delay(100);
        tone(14, 587, 200);
        delay(100);
        tone(14, 659, 200);
        delay(3000);
        Wire.beginTransmission(8);
        Wire.write(2);
        Wire.endTransmission();
        tone(14, 659, 200);
        delay(100);
        tone(14, 587, 200);
        delay(100);
        tone(14, 523, 200);
      }
      else if(userInput == '#' + password)
      {
        tone(14, 659, 200);
        delay(100);
        tone(14, 587, 200);
        delay(100);
        tone(14, 523, 200);
        count = 0;
        userInput = "";
        while(count < 4)
        {
          key = keypad.getKey();
          if(key)
          {
            tone(14, 523, 200);
            delay(500);
            userInput += key;
            count += 1;
          }
        }
        tone(14, 523, 200);
        delay(100);
        tone(14, 587, 200);
        delay(100);
        tone(14, 659, 200);
        password = userInput;
        count = 0;
      }
      else
      {
        Wire.beginTransmission(8);
        Wire.write(2);
        Wire.endTransmission();
        tone(14, 523, 200);
        delay(500);
        tone(14, 523, 200);
        delay(500);
        tone(14, 523, 200);
        delay(500);
        tone(14, 523, 200);
      }
      userInput = "";
    }
  }
}

void readRFID()
{
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  if ( ! rfid.PICC_ReadCardSerial())
    return;


  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));


  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    return;
  }
  
  if (rfid.uid.uidByte[0] == PICC_0 || 
      rfid.uid.uidByte[1] == PICC_1 || 
      rfid.uid.uidByte[2] == PICC_2 || 
      rfid.uid.uidByte[3] == PICC_3 ) {
        
        Wire.beginTransmission(8);
        Wire.write(1);
        Wire.endTransmission();
        tone(14, 523, 200);
        delay(100);
        tone(14, 587, 200);
        delay(100);
        tone(14, 659, 200);
        delay(3000);
        Wire.beginTransmission(8);
        Wire.write(2);
        Wire.endTransmission();
        tone(14, 659, 200);
        delay(100);
        tone(14, 587, 200);
        delay(100);
        tone(14, 523, 200); 
  }
  
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
