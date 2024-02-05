#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
void motion();

#define SS_PIN 10
#define RST_PIN 9

LiquidCrystal_I2C lcd(0x27,16,2);
MFRC522 rfid(SS_PIN,RST_PIN);
Servo myservo;
byte nuidPICC[4]={243,168,146,180};
byte nuidPICC2[4]={194,53,114,32};
int blueled=8;
int redled=7;
int buzzer=6;
int servo=5;
int motionsensorVCC=2;
int motionsensorVOUT=4;
static int k=1; 
int mot,l,p;
static int o=0;

void setup() {
  lcd.init();
  lcd.clear();         
 Serial.begin(9600);
 SPI.begin(); // Init SPI bus
 rfid.PCD_Init(); // Init MFRC522 

pinMode(redled,OUTPUT);
pinMode(blueled,OUTPUT);
pinMode(buzzer,OUTPUT);
pinMode(motionsensorVCC,OUTPUT);
pinMode(motionsensorVOUT,INPUT);
}

void loop() {
   digitalWrite(motionsensorVCC,1);
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;
 bool i=rfid.uid.uidByte[0] == nuidPICC[0] & rfid.uid.uidByte[1] == nuidPICC[1] & rfid.uid.uidByte[2] == nuidPICC[2] & rfid.uid.uidByte[3] == nuidPICC[3];
 bool m=rfid.uid.uidByte[0] == nuidPICC2[0] & rfid.uid.uidByte[1] == nuidPICC2[1] & rfid.uid.uidByte[2] == nuidPICC2[2] & rfid.uid.uidByte[3] == nuidPICC2[3];
 bool n=i|m;
 if (n == 0) {
      

      lcd.backlight();
      lcd.setCursor(3,0);
      lcd.print("Your ID is");
      lcd.setCursor(4,1);
      lcd.print("Incorrect");
      digitalWrite(redled,1); 
      digitalWrite(buzzer,1);
      delay(100);
      digitalWrite(buzzer,0);
      delay(100);
      digitalWrite(buzzer,1);
      delay(100);
      digitalWrite(buzzer,0);
      delay(2000);
      lcd.clear(); 
      lcd.noBacklight(); 
      digitalWrite(redled,0);
 }

    else{
      close:
      Serial.println(k);      
      lcd.backlight();
      lcd.setCursor(3,0);
      lcd.print("Your ID is");
      lcd.setCursor(4,1);
      lcd.print("Correct");
      digitalWrite(blueled,1);
      digitalWrite(buzzer,1);
      delay(500);
      digitalWrite(buzzer,0);
      delay(2000);  
      lcd.clear(); 
      digitalWrite(blueled,0);
      if(k==1){
        lcd.setCursor(1,0);
        lcd.print("System Status:");
        lcd.setCursor(5,1);
        lcd.print("Close");
        delay(3000);
        lcd.clear();
        delay(500);
        lcd.noBacklight(); 
        myservo.attach(servo);
        myservo.write(110);
        delay(2050);
        myservo.detach();
        --k;
        Serial.println(k);    
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();   
        goto statusskip;     
        }
      if(k==0){ 
        lcd.setCursor(1,0);
        lcd.print("System Status:");
        lcd.setCursor(5,1);
        lcd.print("Open");
        delay(3000);
        lcd.clear();
        delay(500);
        lcd.noBacklight();        
        myservo.attach(servo);
        myservo.write(60);
        delay(1500);
        myservo.detach();
        ++k;
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
        delay(3000);
        mot=0;
        for(l=1;l>0;l++){
          if(l==1){mot=0; goto motskip;}          
        mot=digitalRead(motionsensorVOUT);
        if(mot==1){
         
           for(p=1;p>0;p++){
               rfid.uid.uidByte[0]=0;rfid.uid.uidByte[1]=0;rfid.uid.uidByte[2]=0;rfid.uid.uidByte[3]=0; 
               rfid.PCD_AntennaOn();
               rfid.PICC_IsNewCardPresent();
               rfid.PICC_ReadCardSerial();
               bool i=rfid.uid.uidByte[0] == nuidPICC[0] & rfid.uid.uidByte[1] == nuidPICC[1] & rfid.uid.uidByte[2] == nuidPICC[2] & rfid.uid.uidByte[3] == nuidPICC[3];
               bool m=rfid.uid.uidByte[0] == nuidPICC2[0] & rfid.uid.uidByte[1] == nuidPICC2[1] & rfid.uid.uidByte[2] == nuidPICC2[2] & rfid.uid.uidByte[3] == nuidPICC2[3];
               bool n=i|m;
               if(n==1){
                 rfid.uid.uidByte[0]=0;rfid.uid.uidByte[1]=0;rfid.uid.uidByte[2]=0;rfid.uid.uidByte[3]=0; 
                 rfid.PICC_HaltA();
                 rfid.PCD_StopCrypto1();
                 k=1;             
                 goto close;
               }
               bool e=rfid.uid.uidByte[0]!=0 & rfid.uid.uidByte[1]!=0 & rfid.uid.uidByte[2]!=0 & rfid.uid.uidByte[3]!=0;
               bool j=(n==0);
                 if(e && j){
                  lcd.backlight();
                  lcd.setCursor(3,0);
                  lcd.print("Your ID is");
                  lcd.setCursor(4,1);
                  lcd.print("Incorrect");
                  digitalWrite(redled,1); 
                  digitalWrite(buzzer,1);
                  delay(100);
                  digitalWrite(buzzer,0);
                  delay(100);
                  digitalWrite(buzzer,1);
                  delay(100);
                  digitalWrite(buzzer,0);
                  delay(2000);
                  lcd.clear(); 
                  lcd.noBacklight(); 
                  digitalWrite(redled,0);                 
                 }           
               digitalWrite(buzzer,1);
               digitalWrite(blueled,1);
               delay(200);
               digitalWrite(blueled,0);
               digitalWrite(buzzer,0);
               digitalWrite(redled,1);
               delay(200);
               digitalWrite(redled,0);
               digitalWrite(buzzer,1);
               digitalWrite(blueled,1);               
               delay(200);             
               digitalWrite(buzzer,0);
               digitalWrite(blueled,0);
               digitalWrite(redled,1);
               delay(100);
               digitalWrite(redled,0);
               rfid.PICC_HaltA();
               rfid.PCD_StopCrypto1(); 
                                  
           }
          
        }
       if(mot==0){ 
               rfid.uid.uidByte[0]=0;rfid.uid.uidByte[1]=0;rfid.uid.uidByte[2]=0;rfid.uid.uidByte[3]=0; 
               rfid.PCD_AntennaOn();
               rfid.PICC_IsNewCardPresent();
               rfid.PICC_ReadCardSerial();
               bool i=rfid.uid.uidByte[0] == nuidPICC[0] & rfid.uid.uidByte[1] == nuidPICC[1] & rfid.uid.uidByte[2] == nuidPICC[2] & rfid.uid.uidByte[3] == nuidPICC[3];
               bool m=rfid.uid.uidByte[0] == nuidPICC2[0] & rfid.uid.uidByte[1] == nuidPICC2[1] & rfid.uid.uidByte[2] == nuidPICC2[2] & rfid.uid.uidByte[3] == nuidPICC2[3];
               bool n=i|m;
               bool e=rfid.uid.uidByte[0]!=0 & rfid.uid.uidByte[1]!=0 & rfid.uid.uidByte[2]!=0 & rfid.uid.uidByte[3]!=0;
               bool j=(n==0);
                 if(e && j){
                  lcd.backlight();
                  lcd.setCursor(3,0);
                  lcd.print("Your ID is");
                  lcd.setCursor(4,1);
                  lcd.print("Incorrect");
                  digitalWrite(redled,1); 
                  digitalWrite(buzzer,1);
                  delay(100);
                  digitalWrite(buzzer,0);
                  delay(100);
                  digitalWrite(buzzer,1);
                  delay(100);
                  digitalWrite(buzzer,0);
                  delay(2000);
                  lcd.clear(); 
                  lcd.noBacklight(); 
                  digitalWrite(redled,0);                 
               } 
               if(n==1)
                goto close;
               if(n==0)
                goto mot0skip;                     
                    
        } 
         mot0skip:                       
         motskip:
         l=3;
         delay(2000);             
        }
     
    }   
    }

    statusskip: ;

}    
  
      
     

    
      
  
     
     
  


