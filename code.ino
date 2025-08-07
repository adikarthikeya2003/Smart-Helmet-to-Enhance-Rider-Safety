#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
#include <TinyGPS++.h>
String phone_no1 = "+91**********";
TinyGPSPlus gps;
#include <LiquidCrystal.h>
LiquidCrystal lcd(2,3,4,5,6,7);
#include <SoftwareSerial.h>


SoftwareSerial ss(8,9); // RX, TX pins for GSM module
#define em_sw A1
#define hel_sw A0
#define dis_sw A2
#define alcohol_sensor A3
#define rf 11
#define buzzer 10
float l,n;
int m;
int flag=0,count=0;
void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
  lcd.begin(16,2);
  pinMode(rf,OUTPUT);
  pinMode(buzzer,OUTPUT);
  digitalWrite(rf,LOW);
  lcd.setCursor(0,0);
  lcd.print("Welcome");
  lcd.setCursor(0,1);
  lcd.print("Project Smart Helmet");
  if(!accel.begin())
   {
      //Serial.println("No valid sensor found");
      while(1);
   }

  delay(1000);
  lcd.clear();
}
void loop()
{
  sensors_event_t event; 
  accel.getEvent(&event);
  int x = event.acceleration.x;
  int y = event.acceleration.y;
  int a = analogRead(alcohol_sensor);
  int h = digitalRead(hel_sw);
  int d = digitalRead(dis_sw);
  int e = digitalRead(em_sw);
   
  
  
  if(h==0)
  {
    Serial.println("Helmet Worn");
    lcd.clear();
    delay(500);
    if(a<400)
    {
       lcd.clear();
       Serial.println("Alcohol Alert");
       lcd.setCursor(0, 0);
       lcd.print("Helmet Worn");
       lcd.setCursor(0, 1);
       lcd.print("Alcohol Alert");
       digitalWrite(rf,LOW);
       digitalWrite(buzzer,HIGH);
       delay(1000);
       digitalWrite(buzzer,LOW);
    }
    else
    {
      lcd.clear();
      Serial.println("Alcohol ok");
      lcd.setCursor(0, 0);
      lcd.print("Helmet Worn");
      lcd.setCursor(0, 1);
      lcd.print("No Alcohol");
      digitalWrite(rf,HIGH);
      delay(1000);
       lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("lat:");
        lcd.print(gps.location.lat(), 6);
        lcd.setCursor(0,1);
        lcd.print("lon:");
        lcd.print(gps.location.lng(), 6);
      if((e==0)||(x<-5)||(x>5)||(y<-5)||(y>5))
      {
        flag=1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Emergency Alert");
          digitalWrite(rf,LOW);
          Serial.println("count"+String(count)+" ");
          digitalWrite(buzzer,HIGH);
          delay(1000);
          digitalWrite(buzzer,LOW);
          if(d==0)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Disable switch");
            lcd.setCursor(0,1);
            lcd.print("Activated");
            delay(500);
            count=0;
            flag=0;
          }
          if(flag==1)
          {
            count++;
          }
          if(count>10)
          {
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("Sending SMS");
            sendSMS(phone_no1,"***Emergency Alert***\nLocation - 
            http://maps.google.com/maps?&z=15&mrt=yp&t=k&q="+
            String(gps.location.lat(), 6)+
            "+"+String(gps.location.lng(), 6)+"");
            count=0;
            flag=0;
            MakeCall();
          }
        
      }
    }
  }
  else
  {
    digitalWrite(rf,LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Helmet Not Worn");
    lcd.setCursor(0,1);
    lcd.print("Please wear");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("lat:");
    lcd.print(gps.location.lat(), 6);
    lcd.setCursor(0,1);
    lcd.print("lon:");
    lcd.print(gps.location.lng(), 6);

  }
Serial.println("ATA\r\n");
ss.print(gps.location.lat(), 6);
ss.print(",");
ss.print(gps.location.lng(), 6);
ss.print(",");
ss.println(flag);
Serial.print(gps.location.lat(), 6);
Serial.print(",");
Serial.print(gps.location.lng(), 6);
Serial.print(",");
Serial.println(flag);
delay(500);
} 


void sendSMS(String number, String msg)
{
  Serial.println("AT+CMGF=1");    
  //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.print("AT+CMGS=\""); 
  Serial.print(number); Serial.println("\"\r\n");
  delay(1000);
  Serial.println(msg);
  delay(1000);
  Serial.println((char)26);
  delay(2000);
}

void MakeCall()
{
  Serial.println("AT+CLVL=1\r\n");
    delay(1000);

  Serial.println("AT+CRSL=1\r\n");
    delay(1000);

  Serial.println("ATD"+String(phone_no1)+";"); 
  // ATDxxxxxxxxxx; -- watch out here 
  //for semicolon at the end!!
  Serial.println("Calling"); 
  // print response over serial port
  delay(1000);
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    l=gps.location.lat(), 6;
    n=gps.location.lng(), 6;
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
