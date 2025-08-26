// Program systému DUCK
// Napsal Toník "Kvák" Doležal

// Začneme určením knihoven, které má Arduino používat:
#include <Arduino.h>
#include <SoftwareSerial.h> //pomoc mp3 přehrávači
#include <Servo.h> //Serva
#include <DHT.h> //teploměr a vlhkoměr
#include <DFRobotDFPlayerMini.h> //mp3 přehrávač
#include <IRremote.hpp> //infračervené ovládání
// Teď nastavíme, na co bude který pin využit:
#define Mikrofon A0 //pin mikrofonu
int IRpin = 2; //pin s IR diodou
int Button1 = 6; //pin s prvním dotykovým senzorem
int Button2 = 7; //pin s druhým dotykovým senzorem
DHT meteo (3, DHT11); //pin a typ DHT teploměru
SoftwareSerial mp3player (10, 11); // piny přehrávače s RX TX komunikací přes SoftwareSerial
// Vytvoříme potřebné objekty:
Servo Switch1; //objekt pro servo 1 (zapínání)
Servo Switch2; //objekt pro servo 2 (vypínání)
DFRobotDFPlayerMini MP3; //objekt přehrávače
// Připravíme potřebné proměnné:
bool LightState = 1; //stav vypínače stropního světla
unsigned long interruption; //proměnná pro dvojtlesknutí, více později
int hluk; //proměná pro hladinu hluku mikrofonu
int poz = 90; //základní pozice serv
int sv1 = 120; //pozice serva 1 při rozsvícení
int sv2 = 55; //pozice serva 2 při zhasnutí
int hranice = 30; //hranice hluku, pro mě 30
int vol = 30; //hlasitost modulu

void setup() {
// Nyní přejdeme na první část programu, která bude provedena pouze jednou po zapnutí.
Serial.begin (9600); //Začátek sériové komunikace s počítačem
mp3player.begin (9600); //Začátek komunikace se SoftwareSerial
IrReceiver.begin (IRpin); //nastavení ir přijímače na odpovídající pin
Switch1.attach (4); //připnutí serv na odpovídající pin
Switch2.attach (5);
MP3.begin (mp3player); //začátek komunikace s přehrávačem
meteo.begin (); //začátek komunikace s teploměrem
pinMode (Button1, INPUT); //nastavení dotykového senzoru 1 jako vstup
pinMode (Button2, INPUT); //nastavení dotykového senzoru 2 jako vstup
MP3.volume (vol); //nastavení hlasitosti MP3 modulu na maximum
delay (2000);
Switch1.write (poz); //nastavení základní pozice serv.
Switch2.write (poz);
delay (1500); //Čas na srovnání serv
Switch1.write (sv1); //Rozsvícení
delay (1000);
Switch2.write (poz); //Vrácení serva 1 do původní pozice
delay (1500);
MP3.play (1); //Přehrání uvítací řeči
delay (13000); //Pauza pro přehrání
//Program pro debounce
Switch1.write (91); 
delay (1000);
Switch2.write (poz);
delay (1500);
}

void loop() {
//Nyní přejdeme na program samotný:
hluk = analogRead (Mikrofon); //proměnnou připojíme na čtení analogového vstupu pinu A0
float teplota = meteo.readTemperature (); //proměná pro výstup z teploměru DHT
float vlhkost = meteo.readHumidity (); //proměnná pro výstup z vlhkoměru DHT

if (hluk > hranice) { //pokud je hladina hluku vyšší jak hranice...
 delay (300); //počkej 0,3s
   if ((millis() - interruption > 500) && (millis() - interruption < 2000) && LightState == 1){
   //a pokud je čas od posledního přerušení mezi 0,5 a 2s a stav světla je 1...
   Switch2.write (sv2); //zhasni
   delay (1000);
   Switch2.write (poz); //vrať se do původní pozice
   delay (1000);
   LightState = 0; //změň stav světla na 0
  }
   else if ((millis() - interruption > 500) && (millis() - interruption < 2000) && LightState == 0){
   //pokud je však stav světla 0...
   Switch1.write (sv1); //rozsviť
   delay (1000);
   Switch1.write (poz); //vrať se do původní pozice
   delay (1000);
   LightState = 1; //změň stav světla na 1
  }
   else {
   interruption = millis (); //kolik uběhlo času od zapnutí Arduina do posledního přerušení
  }
 delay (50); //debounce
}

if (digitalRead (Button1) == HIGH && teplota <=50 && teplota >=5) {
 //pokud je stisnkut dotykový senzor 1 a teplota je v daném rozmezí...
 MP3.play (2); //přehraj úvodní stopu k teplotě
 delay (2100); //pauza na přehrání
 MP3.play (teplota + 2); //přehraj odpovídající stopu 
 delay (3500); //pauza na přehrání
}

if (digitalRead (Button2) == HIGH) {
 //pokud je stisknut dotykový senzor 2...
   if (vlhkost >=20 && vlhkost <=25){
   MP3.play (3);
   delay (3200);
   MP3.play (53);
   delay (3500);
  }
   else if (vlhkost >=26 && vlhkost <=30){
   MP3.play (3);
   delay (3200);
   MP3.play (54);
   delay (3500);
  }
   else if (vlhkost >=31 && vlhkost <=35){
   MP3.play (3);
   delay (3200);
   MP3.play (55);
   delay (3500);
  }
   else if (vlhkost >=36 && vlhkost <=40){
   MP3.play (3);
   delay (3200);
   MP3.play (56);
   delay (3500);
  }
   else if (vlhkost >=41 && vlhkost <=45){
   MP3.play (3);
   delay (3200);
   MP3.play (57);
   delay (3500);
  }
   else if (vlhkost >=46 && vlhkost <=50){
   MP3.play (3);
   delay (3200);
   MP3.play (58);
   delay (3500);
  }
   else if (vlhkost >=51 && vlhkost <=55){
   MP3.play (3);
   delay (3200);
   MP3.play (59);
   delay (3500);
  }
   else if (vlhkost >=56 && vlhkost <=60){
   MP3.play (3);
   delay (3200);
   MP3.play (60);
   delay (3500);
  }
   else if (vlhkost >=61 && vlhkost <=65){
   MP3.play (3);
   delay (3200);
   MP3.play (61);
   delay (3500);
  }
   else if (vlhkost >=66 && vlhkost <=70){
   MP3.play (3);
   delay (3200);
   MP3.play (62);
   delay (3500);
  }
   else if (vlhkost >=71 && vlhkost <=75){
   MP3.play (3);
   delay (3200);
   MP3.play (63);
   delay (3500);
  }
   else if (vlhkost >=76 && vlhkost <=80){
   MP3.play (3);
   delay (3200);
   MP3.play (64);
   delay (3500);
  }
   else if (vlhkost >=81 && vlhkost <=85){
   MP3.play (3);
   delay (3200);
   MP3.play (65);
   delay (3500);
  }
   else if (vlhkost >=86 && vlhkost <=90){
   MP3.play (3);
   delay (3200);
   MP3.play (66);
   delay (3500);
  }
}
if (IrReceiver.decode()){
 //pokud zachytí IR přijámač signál...
  if (IrReceiver.decodedIRData.decodedRawData == 0xBC43FF00 && LightState == 0) {
   Switch1.write (sv1); //rozsviť
   delay (1000);
   Switch1.write (poz); //vrať se do původní pozice
   delay (1000);
   LightState = 1; //změň stav světla na 1
  }
  else if (IrReceiver.decodedIRData.decodedRawData == 0xBC43FF00 && LightState == 1) {
   Switch2.write (sv2); //rozsviť
   delay (1000);
   Switch2.write (poz); //vrať se do původní pozice
   delay (1000);
   LightState = 0; //změň stav světla na 0
  }
  else if (IrReceiver.decodedIRData.decodedRawData == 0xBB44FF00) {
   Switch1.write (sv1); //rozsviť
   delay (1000);
   Switch1.write (poz); //vrať se do původní pozice
   delay (1000);
   LightState = 1; //změň stav světla na 1
  }
  else if (IrReceiver.decodedIRData.decodedRawData == 0xBF40FF00) {
   Switch2.write (sv2); //rozsviť
   delay (1000);
   Switch2.write (poz); //vrať se do původní pozice
   delay (1000);
   LightState = 0; //změň stav světla na 0
  }
  else if (IrReceiver.decodedIRData.decodedRawData == 0xF30CFF00) {
   MP3.play (5); //Přehraj Kváknutí
   delay (4300);
  }
  else if (IrReceiver.decodedIRData.decodedRawData == 0xE718FF00) {
   MP3.play (6); //Přehraj Výběr z efektů zvků prdů
   delay (37500);
  }
  else if (IrReceiver.decodedIRData.decodedRawData == 0xA15EFF00) {
   MP3.play (4); //Přehraj titulky
   delay (18000);
  }
  else if (IrReceiver.decodedIRData.decodedRawData == 0xE916FF00) {
   MP3.play (2); //přehraj úvodní stopu k teplotě
   delay (2100); //pauza na přehrání
   MP3.play (teplota + 2); //přehraj odpovídající stopu 
   delay (3500); //pauza na přehrání
  }
  else if (IrReceiver.decodedIRData.decodedRawData == 0xF609FF00) {
   vol = 30; //nasataví hlasitost na základní
   MP3.play (67);
   delay (1500);
  }
  else if (IrReceiver.decodedIRData.decodedRawData == 0xF087FF00) {
   vol = max (vol - 5, 5); //sníží hlasitost o 5, s nejnižší hodnotou 5
   MP3.play (67);
   delay (1500);
  }
  else if (IrReceiver.decodedIRData.decodedRawData == 0xEA15FF00) {
   vol = min (vol + 5, 30); //sníží hlasitost o 5, s nejnižší hodnotou 5
   MP3.play (67);
   delay (1500);
  }
IrReceiver.resume(); //Přijímač se připraví na další signál
delay (50); //debounce
}

} //dolní závorka void loop
