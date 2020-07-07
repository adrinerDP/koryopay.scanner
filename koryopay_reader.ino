#include <SPI.h>
#include <MFRC522.h>

MFRC522 rfid(10, 9);
MFRC522::MIFARE_Key key;

byte nuidPICC[4];
int BUZZ = 3;
int BLUE = 5;
int RED  = 6;

void setup() { 
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  
  digitalWrite(BLUE, HIGH);
  RunBuzzerDouble();
}
 
void loop() {
  if (!rfid.PICC_IsNewCardPresent())
    return;

  if (!rfid.PICC_ReadCardSerial())
    return;

  digitalWrite(BLUE, LOW);
  digitalWrite(RED, HIGH);
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
  
  printHex(rfid.uid.uidByte, rfid.uid.size);

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  RunBuzzerOnce();
  delay(150);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, HIGH);
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? "0" : "");
    Serial.print(buffer[i], HEX);
  }
  Serial.println();
}

void RunBuzzerOnce() {
  digitalWrite(BUZZ, HIGH);
  delay(50);
  digitalWrite(BUZZ, LOW);
}

void RunBuzzerDouble() {
  digitalWrite(BUZZ, HIGH);
  delay(50);
  digitalWrite(BUZZ, LOW);
  delay(50);
  digitalWrite(BUZZ, HIGH);
  delay(50);
  digitalWrite(BUZZ, LOW);
}
