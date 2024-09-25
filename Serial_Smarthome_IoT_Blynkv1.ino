#include <AccelStepper.h>

// Stepper
#define motorpin1 2
#define motorpin2 3
#define motorpin3 4
#define motorpin4 5
#define MotorInterfaceType 8 // Jenis driver stepper
AccelStepper stepper(MotorInterfaceType, motorpin1, motorpin3, motorpin2, motorpin4);

//Pompa 
#define pompa 7


void setup() {
  // Stepper
  Serial.begin(115200); // Memulai komunikasi serial dengan baud rate 115200
  stepper.setMaxSpeed(1000); // Pengatur kecepatan motor stepper
  stepper.setAcceleration(10000); // Mengatur akselerasi motor stepper

  //Pompa
  pinMode(pompa, OUTPUT);

}

void loop() {
//Membaca komunikasi serial 
  if (Serial.available() > 0) { // Jika ada data yang tersedia di Serial Monitor
    char receivedChar = Serial.read(); // Membaca data yang diterima dari Serial Monitor
    //Stepper---------------------------------------------------------------------------
    if (receivedChar == '1') {
      stepper.runToNewPosition(10050); //posisi 1000
    } else if (receivedChar == '0') {
      stepper.runToNewPosition(0); //posisi 0
    } 
  
    //Pompa---------------------------------------------------------------------------
    else if (receivedChar == '2') { 
      digitalWrite(pompa, HIGH);
    } else if (receivedChar == '3') { 
      digitalWrite(pompa, LOW);
    }
  }
}


