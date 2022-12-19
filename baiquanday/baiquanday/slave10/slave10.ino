#include <AccelStepper.h>
#include <Wire.h>
int table[] = {0, 0, 0, 0, 0, 0, 0, 0};
//const int led1 = 2;
int x;
//char data[] = {'0', '1', '2', '3', '4', '5', '6'};
int duongkinh2, sovongsocap, sovongthucap, chieudaicuon;
float duongkinh;

int CBTC = 2; //cảm biến tiện cận
int nut_nhan = 3; // nút nhấn
int in3 = 4, in4 = 5, PWM = 6; // chân driver L298N
int ena = 7, direct = 8, pulse = 9; // driver TB6600
int cbHome = 10; // cảm biến home
int ctht = 11;
int vehome_step = 0, vehome_dc;
int so_vong , dem_xung, cv = 0, pv = 1;
int dem_nut_nhan;
int bien_tru = 0;
int n;
unsigned long cv1, pv1, cv2, pv2, cv3, pv3;
unsigned long serial1, serial2;
int so_vong_can;
int v1, v2 = 0, chuyen_doi;
int y;
int tinhieu = 0;
int menu, giatri, beginn;
int bienchongdut;

AccelStepper mystepper(1, pulse, direct, ena);

void setup()
{
  Serial.begin(9600);
  Wire.begin(1);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  // pinMode(led1, OUTPUT);

  pinMode(CBTC, INPUT_PULLUP);
  pinMode(nut_nhan, INPUT_PULLUP);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(direct, OUTPUT);
  pinMode(pulse, OUTPUT);
  pinMode(cbHome, INPUT_PULLUP);
  pinMode(ctht, INPUT_PULLUP);

  Serial.begin(9600);
  mystepper.setMaxSpeed(4000);
  attachInterrupt(0, dem_vong, RISING);
  delay(2000);
}

void(* resetfunction) (void) = 0; // nhan nut reset arduino

void receiveEvent(int howMany)
{
  x = Wire.read();
}
void requestEvent()
{
  uint8_t Buffer[8];
  Buffer[0] = table[0];
  Buffer[1] = table[1];
  Buffer[2] = table[2];
  Buffer[3] = table[3];
  Buffer[4] = table[4];
  Buffer[5] = table[5];
  Buffer[6] = table[6];
  Buffer[7] = table[7];
  Wire.write(Buffer, 8);
}

void loop()
{
  table[0] = duongkinh2;
  table[1] = sovongsocap;
  table[2] = sovongthucap;
  table[3] = chieudaicuon;
  table[4] = beginn;
  table[5] = tinhieu;
  table[6] = so_vong;
  table[7] = menu;

  inSerial();
  homing();
  dem_vong();
  dao_chieu();
  du_so_vong_cap();
  doi_so_vong();
  giao_dien();
  thay_gia_tri();
  chay_chuong_trinh();
  chongdut();

}

void homing()
{
  if (sovongsocap > 0 and sovongthucap > 0 and beginn == 1 )
  {
    if (vehome_step == 0)
    {
      mystepper.setSpeed(-3000);
      mystepper.runSpeed();
    }
  }

  if (digitalRead(cbHome) == 0)
  {
    vehome_step = 1;
    y = 1;
  }
}
void dem_vong()
{
  if (chuyen_doi == 0 || chuyen_doi == 1)
  {
    cv = digitalRead(CBTC);
    if (cv != pv)
    {
      dem_xung++;
      pv = cv;
      //Serial.println(so_vong);
    }
  }
  so_vong = (dem_xung / 2) - 1;
}

void dao_chieu()
{
  if (vehome_step == 1  and beginn == 1)
  {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(PWM, 100);
    if ( so_vong - bien_tru == 1)
    {
      n++;
      if (n <= (52 / duongkinh))
      {
        digitalWrite(direct, HIGH);
      }
      if (n > (52/ duongkinh) and n <= ( 103/ duongkinh))
      {
        digitalWrite(direct, LOW);
      }
      if (n > (103/ duongkinh))
      {
        n = 0;
      }
      //22-43
      //235
      for (int x = 0; x <= (duongkinh * 200); x++)
      {
        digitalWrite(pulse, HIGH);
        delayMicroseconds(200);
        digitalWrite(pulse, LOW);
        delayMicroseconds(200);
      }
      bien_tru = so_vong;
    }
  }
}
void du_so_vong_cap()
{
  cv1 = millis();
  if (cv1 - pv1 >= 500)
  {
    if (so_vong == so_vong_can)
    {
      vehome_step = 2;
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      analogWrite(PWM, 0);
      so_vong_can = 0;
      so_vong = 0;
      beginn = 0;
    }
    pv1 = cv1;
  }
}
void doi_so_vong()
{
  cv2 = millis();
  if (cv2 - pv2 >= 500)
  {
    v1 = digitalRead(nut_nhan);
    if (v1 == v2)
    {
      delay(10);
      if (digitalRead(nut_nhan) == LOW)
      {
        n = 0;
        bien_tru = 0;
        dem_xung = 0;
        so_vong_can = sovongthucap;
        vehome_step = 0;
        chuyen_doi++;
        if (chuyen_doi > 2)
        {
          chuyen_doi = 2;
        }
        while (digitalRead(nut_nhan) == LOW) {}
      }
    }
    if (chuyen_doi == 0)
    {
      so_vong_can = sovongsocap;
    }
    if (chuyen_doi == 1)
    {
    }
    if (chuyen_doi >= 2)
    {
      resetfunction(); //reset arduino
    }

    if (so_vong_can == sovongsocap and sovongsocap != 0 and chuyen_doi == 0)
    {
      tinhieu = 1;
    }
    if (so_vong_can == sovongthucap and sovongthucap != 0 and chuyen_doi == 1)
    {
      tinhieu = 2;
    }
  }
}

void inSerial()
{
  serial1 = millis();
  if (serial1 - serial2 >= 800)
  {
    //    Serial.print(duongkinh);
    //    Serial.print("|");
    //    Serial.print(sovongsocap);
    //    Serial.print("|");
    //    Serial.print(sovongthucap);
    //    Serial.print("|");
    //    Serial.print(chieudaicuon);
    //    Serial.print("|");
    //    Serial.print(menu);
    //    Serial.print("|");
    //    Serial.println(tinhieu);
    Serial.println(bienchongdut);
    serial2 = serial1;
  }
}
void giao_dien()
{
  if (x == 1)
  {
    menu = menu + 1;
    delay(1);
    if (menu > 6)
    {
      menu = 0;
    }
    x = 0;
  }
}
void thay_gia_tri()
{
  // Tang gia tri
  if (x == 2)
  {
    switch (menu)
    {
      case 0:
        duongkinh = duongkinh + 0.1;
        duongkinh2 = duongkinh2 + 1;
        break;
      case 1:
        sovongsocap = sovongsocap + 5;
        break;
      case 2:
        sovongthucap = sovongthucap + 5;
        break;
      case 3:
        chieudaicuon = chieudaicuon + 2;
        break;
    }
    x = 0;
  }

  // Giam gia tri
  if (x == 3)
  {
    switch (menu)
    {
      case 0:
        duongkinh = duongkinh - 0.1;
        duongkinh2 = duongkinh2 - 1;
        if (duongkinh < 0)
        {
          duongkinh++;
        }
        break;
      case 1:
        sovongsocap = sovongsocap - 5;
        if (sovongsocap < 0)
        {
          sovongsocap = 0;
        }
        break;
      case 2:
        sovongthucap = sovongthucap - 5;
        if (sovongthucap < 0)
        {
          sovongthucap = 0;
        }
        break;
      case 3:
        chieudaicuon = chieudaicuon - 2;
        if (chieudaicuon < 0)
        {
          chieudaicuon = 0;
        }
        break;
    }
    x = 0;
  }
}
void chay_chuong_trinh()
{
  if (x == 4)
  {
    beginn++;
    if (beginn > 1)
    {
      beginn = 0;
    }
    x = 0;
  }
}
void chongdut()
{
  cv3 = millis();
  if (cv3 - pv3 >= 600)
  {
    if (digitalRead(ctht) == 0 )
    {
      beginn = 0;
      //      digitalWrite(in3, LOW);
      //      digitalWrite(in4, LOW);
      //      analogWrite(PWM, 0);
    }
    if (beginn == 0)
    {
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      analogWrite(PWM, 0);
    }
  }
}
