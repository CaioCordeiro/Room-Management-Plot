#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
int LED_verde = 2;
int LED_amarelo = 3;
int LED_vrtmelho = 4;
char st[20];

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop()
{

    char x = Serial.read(); 
    if (x == '1')
    {
      digitalWrite(2, HIGH);
      delay(5000);
      digitalWrite(2, LOW);
 
    }
    if (x == '2')
    {
      digitalWrite(3, HIGH);
      delay(5000);
      digitalWrite(3, LOW);
    }
    if (x == '3')
    {
      digitalWrite(4, HIGH);
      delay(5000);
      digitalWrite(4, LOW);
    }
  
  
  
  // Procurar por cartao existente
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Selecionar um dos cartoes existentes
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : "");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : ""));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  conteudo.toUpperCase();
  Serial.println();
  digitalWrite(2, HIGH);
  delay(2000);
  digitalWrite(2, LOW);
}
