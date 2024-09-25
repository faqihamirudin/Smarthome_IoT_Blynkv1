#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//Input
//-------------------------------------------------------------------------------------------cahaya
#define cahayaPin V0    //virtual blynk
int statusCahaya;       //trigger
#define sensorCahaya 16 //sensor cahaya 1 = gelap, 0 = terang
//-------------------------------------------------------------------------------------------api
#define apiPin V1
int statusApi;
#define sensorApi 5 //sensor api 0 = min api, 1023 = max api
#define FLAME_THRESHOLD 800 // Nilai ambang batas untuk deteksi api
//-------------------------------------------------------------------------------------------hujan
#define hujanPin V2
int statusHujan;
#define sensorHujan A0 //sensor hujan 1 = gelap, 0 = terang
//-------------------------------------------------------------------------------------------gerak
#define gerakPin V3
int statusGerak;
#define sensorGerak 9 //sensor cahaya 1 = gelap, 0 = terang

//blynk
#define BLYNK_TEMPLATE_ID "TMPL6ujKgm-mY"
#define BLYNK_TEMPLATE_NAME "Sensor"
#define BLYNK_AUTH_TOKEN "nZEyLWr9-4_pPFPmzB9agmJvGWnQRGvo"
//setup WIFI
BlynkTimer timer;
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Beauty"; // Masukkan nama WIFI Anda
char pass[] = "borutoba";   // Masukkan kata sandi WIFI Anda

BLYNK_WRITE(cahayaPin) {
  statusCahaya = param.asInt();
}

BLYNK_WRITE(apiPin) {
  statusApi = param.asInt();
}

BLYNK_WRITE(hujanPin) {
  statusHujan = param.asInt();
}

BLYNK_WRITE(gerakPin) {
  statusGerak = param.asInt();
}

void jemuran(){
  if(statusCahaya == 0 && statusHujan == 0){
    Serial.println("Sistem sensor cahaya dimatikan\n");
    Serial.println("Sistem sensor Hujan dimatikan\n");
  }else if(statusCahaya == 1 && statusHujan == 0){
    Serial.print("Sistem sensor cahaya dinyalakan : ");
    if(digitalRead(sensorCahaya) == 1){
      Serial.println("hari sudah gelap\n");
      Serial.println("1");
      Blynk.logEvent("cahaya","Hari sudah gelap");
    }else{
      Serial.println("0");
      Serial.println("hari masih terang\n");
    }
    Serial.println("Sistem sensor Hujan dimatikan\n");
  }else if(statusCahaya == 0 && statusHujan == 1){
    Serial.println("Sistem sensor cahaya dimatikan\n");
    Serial.print("Sistem sensor hujan dinyalakan : ");
    if(analogRead(sensorHujan) < 900){
      Serial.println("Hujan terdeteksi\n");
      Serial.println("1");
      Blynk.logEvent("hujan","Turun hujan");
      // statusHujan = 0;
    }else{
      Serial.println("0");
      Serial.println("cuaca cerah\n");
    }
  }else if(statusCahaya == 1 && statusHujan == 1){
    if((digitalRead(sensorCahaya) == 1) || (analogRead(sensorHujan) < 900)){
      Serial.println("jemuran bergerak");
      if(digitalRead(sensorCahaya) == 1){
        Serial.println("1");
        Blynk.logEvent("cahaya", "Hari sudah gelap");
      }else{
        Serial.println("0");
        Blynk.logEvent("hujan", "Turun hujan");
      }
    }else{
      Serial.println("0");
    }
  }
}

void blynkApi(){
  if (statusApi == 1){
    Serial.print("Sistem sensor api dinyalakan : ");
    if(analogRead(sensorApi) > FLAME_THRESHOLD){
      Blynk.logEvent("api","Terjadi kebakaran");
      Serial.println("2");
      Serial.println("Api terdeteksi\n");
    }else{
      Serial.println("3");
      Serial.println("situasi aman, tidak ada api\n");
    }
  }else{
    Serial.println("Sistem sensor api dimatikan\n");
  }
}

void blynkGerak(){
  // if (statusGerak == 1){
  //   Serial.print("Sistem sensor Gerak dinyalakan : ");
  //   if(digitalRead(sensorGerak) == 1){
  //     Serial.println("Gerak terdeteksi\n");
  //     Blynk.logEvent("gerak","gerak terdeteksi");
  //   }else{
  //     Serial.println("gerak tidak terdeteksi\n");
  //   }
  // }else{
  //   Serial.println("Sistem sensor Gerak dimatikan\n");
  // }
}

void setup() {
  Serial.begin(115200);
  
  
  //Input sensor
  pinMode(sensorCahaya, INPUT);
  pinMode(sensorApi, INPUT);
  pinMode(sensorHujan, INPUT);
  pinMode(sensorGerak, INPUT);

  //Output alat
  pinMode(LED_BUILTIN, OUTPUT);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  timer.setInterval(5000L, jemuran);
  timer.setInterval(5000L, blynkApi);
  // timer.setInterval(5000L, blynkGerak);
}

void loop() {
  Blynk.run();
  timer.run();
}