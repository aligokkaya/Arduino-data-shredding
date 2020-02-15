#include "Nextion.h"
//#include <SoftwareSerial.h>
#define mySerial Serial3
//SoftwareSerial mySerial(10, 11);
NexNumber n0 = NexNumber(0, 4, "n0");

boolean ok = false;
String realData;
String dati ;
int DatiInt;
String akim, sicaklik[2], gerilim[12];

void setup() {
   Serial.begin(38400);
  mySerial.begin(38400);
  Serial.begin(38400);
  nexSerial.begin(9600);

}


String arayaNokta(String s, uint8_t nereye, uint8_t toplamKacHane)
{
  String yeni = s.substring(0, nereye);
  yeni += ".";
  yeni += s.substring(nereye + 1, toplamKacHane + 1);
  return yeni;
}

void yorumla()
{
  //[1000,24,008,112345,0]174*

  uint8_t basla = 1;
  uint8_t sayac = 0, t;
  int8_t nereye;

  akim = "";
  sicaklik[0] = "";
  sicaklik[1] = "";
  for (uint8_t i = 0; i < 12; i++)
    gerilim[i] = "";

  String opcode;
  while (true)
  {
    nereye = dati.indexOf(",", basla);
    if (nereye == -1) nereye = dati.indexOf("]", basla);
    if (nereye > -1)
    {
      sayac++;
      String parca = dati.substring(basla, nereye);
      basla = nereye + 1;
      if (sayac == 2)
      {
        opcode = parca;
      }
      else if (opcode.equals("22") && (sayac == 4 || sayac == 5))
      {
        t = sayac - 4;
        Serial.print("Sicaklik");
        Serial.print(t);
        Serial.print(":");
        sicaklik[t] = arayaNokta(parca, 2, 1); // 00.0
        Serial.println(sicaklik[t]);
      }
      else if (opcode.equals("24") && sayac == 4)
      {
        akim = arayaNokta(parca, 3, 6); // -000.00
        Serial.print("Akim: ");
        Serial.println(akim);
      }
      else if (opcode.equals("21") && sayac >= 4 && sayac <= 15)
      {
        t = sayac - 4;
        Serial.print("Gerilim");
        Serial.print(t);
        Serial.print(": ");
        gerilim[t] = parca;
        Serial.println(gerilim[t]);
      }
    }
    else break;
  }

}

void loop() {

  serialEvent();
  if (ok) // ok değişkeni true olduysa yani veri gelmiş ve gelen verinin alımı tamamlanmışsa if döngüsüne girer.
  {
    dati = realData; // verimizin tutulduğu realData değişkeninden veriler dati değişkenine aktarılır.
    realData = "";// realData değişkeni sıfırlanır.
    ok = false; // sonsuz bir döngü oluşmasın diye ok değişkeni tekrardan false yapılır.


    yorumla();
   if (gerilim[1].toInt() != 0) {
      n0.setValue((gerilim[1]).toInt());
    }
  }
}
void serialEvent()
{
  while (mySerial.available())
  {
    char Veri = (char)mySerial.read();// veriyi read fonksiyonu ile alır char tipindeki veri değişkenine aktarır aktarır.
    realData += Veri; //char tipindeki her gelen veriyi real data üstüne ekler
    if (Veri == '\n') // satır başı geldiyse ok değişkenini tru yapar bu gelen verinin bittiğine işarettir.
    {
      ok = true;
    }
  }
}
