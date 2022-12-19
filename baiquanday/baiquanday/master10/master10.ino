/*
  Chân VSS, RW, K - GND
  Chân VDD, A - 5v
  V0 - chân tín hiệu biến trở
  RS - 12
  E - 11
  D4 - 5
  D5 - 4
  D6 - 3
  D7 - 2
*/
//https://arduino.stackexchange.com/questions/16292/sending-and-receiving-different-types-of-data-via-i2c-in-arduino
#include <Wire.h>
int table[] = {0, 0, 0, 0, 0, 0, 0, 0};
//#include<LiquidCrystal_I2C.h>
#include<LiquidCrystal.h>
//LiquidCrystal_I2C lcd(0x27, 20, 4);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //Define LCD Module Pins (RS,EN,D4,D5,D6,D7)


//byte data[10];
char i;
char num = 8;
int x0, x1, x2, x3, x4, x5, x6;
float x00;
int nut_menu = 6, nut_tang = 7, nut_giam = 8, nut_run = 9;
int menu;
unsigned long lcd1, lcd2;
unsigned long serial1, serial2;

void setup()
{
  lcd.begin(20, 4);
  //lcd.init(); lcd.backlight();
  Serial.begin(9600);
  Wire.begin();
  pinMode(nut_menu, INPUT_PULLUP);
  pinMode(nut_tang, INPUT_PULLUP);
  pinMode(nut_giam, INPUT_PULLUP);
  pinMode(nut_run, INPUT_PULLUP);

  lcd.setCursor(0, 0); lcd.print("Mon: Thiet Ke Mach  ");
  lcd.setCursor(0, 1); lcd.print(" May Quan Day Dong  ");
  lcd.setCursor(0, 2); lcd.print("       Nhom 4       ");
  lcd.setCursor(0, 3); lcd.print("    Lop: DH18TD     ");
  delay(1000);
}

void loop()
{
  x00 = float(x0) / 10;
  inSerial();
  nut_nhan();
  giao_dien();
  Wire.requestFrom(1, num);

  if (Wire.available() > 0)
  {
    for (i = 0; i < num; i++)
    {
      int x = Wire.read();
      table[i] = x;
      x0 = table[0] ;
      x1 = table[1] ;
      x2 = table[2] ;
      x3 = table[3] ;
      x4 = table[4] ;
      x5 = table[5] ;
      x6 = table[6] ;
      menu = table[7];
      //      x0 = data[0] - 48;
      //      x1 = data[1] - 48;
      //      x2 = data[2] - 48;
      //      x3 = data[3] - 48;
      //      x4 = data[4] - 48;
      //      x5 = data[5] - 48;
      //      x6 = data[6] - 48;
    }
  }
}
void nut_nhan()
{
  //nut menu ---------------------------------
  if (digitalRead(nut_menu) == LOW)
  {
    delay(10);
    if (digitalRead(nut_menu) == LOW)
    {
      //      menu++;
      //      if (menu > 6)
      //      {
      //        menu = 0;
      //      }
      Wire.beginTransmission(1);
      Wire.write(1);
      Wire.endTransmission();
      while (digitalRead(nut_menu) == LOW) {}
    }
  }
  // nut tang ---------------------------------
  if (digitalRead(nut_tang) == LOW)
  {
    delay(10);
    if (digitalRead(nut_tang) == LOW)
    {
      Wire.beginTransmission(1);
      Wire.write(2);
      Wire.endTransmission();
      while (digitalRead(nut_tang) == LOW) {}
    }
  }
  // nut giam ----------------------------------
  if (digitalRead(nut_giam) == LOW)
  {
    delay(10);
    if (digitalRead(nut_giam) == LOW)
    {
      Wire.beginTransmission(1);
      Wire.write(3);
      Wire.endTransmission();
      while (digitalRead(nut_giam) == LOW) {}
    }
  }
  // nut run --------------------------------
  if (digitalRead(nut_run) == LOW)
  {
    delay(10);
    if (digitalRead(nut_run) == LOW)
    {
      Wire.beginTransmission(1);
      Wire.write(4);
      Wire.endTransmission();
      while (digitalRead(nut_run) == LOW) {}
    }
  }
}
void giao_dien()
{
  lcd1 = millis();
  if (lcd1 - lcd2 >= 900)
  {
    if (menu == 0)
    {
      lcd.setCursor(0, 0); lcd.print(">DK Day:            ");
      lcd.setCursor(0, 1); lcd.print(" So Vong Cuon 1:    ");
      lcd.setCursor(0, 2); lcd.print(" So Vong Cuon 2:    ");
      lcd.setCursor(0, 3); lcd.print(" Chieu Dai Cuon:    ");
    }
    if (menu == 1)
    {
      lcd.setCursor(0, 0); lcd.print(" DK Day:            ");
      lcd.setCursor(0, 1); lcd.print(">So Vong Cuon 1:    ");
      lcd.setCursor(0, 2); lcd.print(" So Vong Cuon 2:    ");
      lcd.setCursor(0, 3); lcd.print(" Chieu Dai Cuon:    ");
    }
    if (menu == 2)
    {
      lcd.setCursor(0, 0); lcd.print(" DK Day:            ");
      lcd.setCursor(0, 1); lcd.print(" So Vong Cuon 1:    ");
      lcd.setCursor(0, 2); lcd.print(">So Vong Cuon 2:    ");
      lcd.setCursor(0, 3); lcd.print(" Chieu Dai Cuon:    ");
    }
    if (menu == 3)
    {
      lcd.setCursor(0, 0); lcd.print(" DK Day:            ");
      lcd.setCursor(0, 1); lcd.print(" So Vong Cuon 1:    ");
      lcd.setCursor(0, 2); lcd.print(" So Vong Cuon 2:    ");
      lcd.setCursor(0, 3); lcd.print(">Chieu Dai Cuon:    ");
    }
    if (menu == 4)
    {
      lcd.setCursor(0, 0); lcd.print(">                   ");
      lcd.setCursor(0, 1); lcd.print(" Trang Thai:        ");
      lcd.setCursor(0, 2); lcd.print(" So vong:           ");
      lcd.setCursor(0, 3); lcd.print(" C1:       C2:      ");
    }
    if (menu == 5)
    {
      lcd.setCursor(0, 0); lcd.print("                    ");
      lcd.setCursor(0, 1); lcd.print(">Trang Thai:        ");
      lcd.setCursor(0, 2); lcd.print(" So vong:           ");
      lcd.setCursor(0, 3); lcd.print(" C1:       C2:      ");
    }
    if (menu == 6)
    {
      lcd.setCursor(0, 0); lcd.print("                    ");
      lcd.setCursor(0, 1); lcd.print(" Trang Thai:        ");
      lcd.setCursor(0, 2); lcd.print(">So vong:           ");
      lcd.setCursor(0, 3); lcd.print(" C1:       C2:      ");
    }
    if (menu == 0 | menu == 1 | menu == 2 | menu == 3)
    {
      lcd.setCursor(13, 0); lcd.print(x00);
      lcd.setCursor(16, 1); lcd.print(x1);
      lcd.setCursor(16, 2); lcd.print(x2);
      lcd.setCursor(16, 3); lcd.print(x3);
    }
    if (menu == 4 | menu == 5 | menu == 6)
    {
      lcd.setCursor(5, 3); lcd.print(x1);
      lcd.setCursor(15, 3); lcd.print(x2);
      if (x4 == 1)
      {
        lcd.setCursor(1, 0); lcd.print("start");
      }
      if (x4 == 0)
      {
        lcd.setCursor(1, 0); lcd.print("stop");
      }
      if (x5 == 1)
      {
        lcd.setCursor(12, 1); lcd.print("Cuon1");
      }
      if (x5 == 2)
      {
        lcd.setCursor(12, 1); lcd.print("Cuon2");
      }
      if (x5 == 0)
      {
        lcd.setCursor(12, 1); lcd.print("ChuaQuan");
      }
      lcd.setCursor(12, 2); lcd.print(x6);
    }
    lcd2 = lcd1;
  }
}
void inSerial()
{
  serial1 = millis();
  if (serial1 - serial2 >= 800)
  {
    Serial.print(x0);
    Serial.print("|");
    Serial.print(x1);
    Serial.print("|");
    Serial.print(x2);
    Serial.print("|");
    Serial.print(x3);
    Serial.print("|");
    Serial.print(x4);
    Serial.print("|");
    Serial.print(x5);
    Serial.print("|");
    Serial.print(x6);
    Serial.print("|");
    Serial.println(x6);
    serial2 = serial1;
  }
}
