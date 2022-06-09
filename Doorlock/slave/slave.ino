#include <Wire.h>
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // SPI통신을 위한 (CE, CSN) 핀 선언

byte DoorLock_address[6] = "00002";

Servo myServo;

void setup() {
  Serial.begin(9600);
  myServo.attach(10);
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  radio.begin();
  radio.openReadingPipe(0, DoorLock_address);
  radio.setPALevel(RF24_PA_MIN);

  radio.startListening();
}

void loop() {
  delay(100);
  if(radio.available()){
    int num = 0;
    radio.read(&num, sizeof(num));
    Serial.println(num);
    if(num == 1) myServo.write(170); 
    else if(num == 2) myServo.write(95);
    }
}

void receiveEvent()
{
  int data = Wire.read();
  if (data == 1){
      myServo.write(170);
    }
    else{
      myServo.write(95);
    }
}
