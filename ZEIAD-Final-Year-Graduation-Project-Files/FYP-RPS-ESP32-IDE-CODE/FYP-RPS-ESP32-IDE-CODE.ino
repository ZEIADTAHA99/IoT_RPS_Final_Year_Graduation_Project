
#include <FirebaseESP32.h>
#include  <WiFi.h>
#include <Servo_ESP32.h>
#include "Arduino.h"
#include "analogWrite.h"
#define WIFI_SSID "ziad-TIME2.4Ghz" // Change the name of your WIFI
#define WIFI_PASSWORD "2zea200299" // Change the password of your WIFI
#define FIREBASE_Authorization_key "szdF8ZE1CDqmBJ5f156uKoAME5hnwrIgQXoMiTpy"
#define FIREBASE_HOST "rps-pallet-conditions-default-rtdb.firebaseio.com"

WiFiClient  client;
FirebaseData firebaseData;
FirebaseData CicularDisc;
FirebaseData DCGearMotor;
FirebaseJson json;

//Declaration oF the (5) IR Sensors (Slots-Status):
int IRsensor1 = 34; 
int IRsensor2 = 12;
int IRsensor3 = 14;
int IRsensor4 = 27;
int IRsensor5 = 35;
int IRsensor6 = 4;  // safety Sensor


//Declaration oF the (5) Micro Servo Motors (Cicular-Disc 180 Rotation):
Servo_ESP32 servo2;
Servo_ESP32 servo3;
Servo_ESP32 servo4;
Servo_ESP32 servo5;

static const int servoPin = 14;
Servo_ESP32 servo1;

//int servoPin1 = ;
//int servoPin2 = ;
//int servoPin3 = ;
//int servoPin4 = ;
//int servoPin5 = ;

//Declaration oF the Mian DC Gear Motor:
int motor1Pin1 = 18;
int motor1Pin2 = 19;
int enable1Pin = 5;

int led = 22;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST, FIREBASE_Authorization_key);
  Firebase.reconnectWiFi(true);

  pinMode(IRsensor1, INPUT);
  pinMode(IRsensor2, INPUT);
  pinMode(IRsensor3, INPUT);
  pinMode(IRsensor4, INPUT);
  pinMode(IRsensor5, INPUT);
  pinMode(IRsensor6, INPUT);
  pinMode(led, OUTPUT);
 


  servo1.attach(servoPin);

  //  pinMode(servoPin2,OUTPUT);
  //  pinMode(servoPin3,OUTPUT);
  //  pinMode(servoPin4,OUTPUT);
  //  pinMode(servoPin5,OUTPUT);
  //  servo1.attach(servoPin1);
  //  servo2.attach(servoPin2);
  //  servo3.attach(servoPin3);
  //  servo4.attach(servoPin4);
  //  servo5.attach(servoPin5);


  // sets the motor pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
  int slot1_status = digitalRead (IRsensor1);
  int slot2_status = digitalRead (IRsensor2);
  int slot3_status = digitalRead (IRsensor3);
  int slot4_status = digitalRead (IRsensor4);
  int slot5_status = digitalRead (IRsensor5);
  int slot6_status = digitalRead (IRsensor6);

  // Code For Set Data Into the Firebase from the (5) IR Sensors (Slot-Status):
  Firebase.setFloat(firebaseData, "/Slot_Status/slot-1", slot1_status);
  Firebase.setFloat(firebaseData, "/Slot_Status/slot-2", slot2_status);
  Firebase.setFloat(firebaseData, "/Slot_Status/slot-3", slot3_status);
  Firebase.setFloat(firebaseData, "/Slot_Status/slot-4", slot4_status);
  Firebase.setFloat(firebaseData, "/Slot_Status/slot-5", slot5_status);
 
  // Code For Read Data From Firebase for the DC-Gear-Motor:
  
  int Rspeed = 120;
  if (Firebase.getString(DCGearMotor, "/Slot_Status/DCMotor"))
  {
    String GearMotor = DCGearMotor.stringData();
    
    if (slot6_status == 1)
    {
        digitalWrite(led, LOW);
    
        if (GearMotor.substring(1, 2) == "1")
        {
    
          analogWrite(enable1Pin, Rspeed);
          digitalWrite(motor1Pin1, LOW); // control the motor's direction in clockwise
          digitalWrite(motor1Pin2, HIGH);  // control the motor's direction in clockwise
          delay (4000);                   // keep rotating for 4 Second.
          digitalWrite(motor1Pin1, LOW); // DC Motor Stop
          digitalWrite(motor1Pin2, LOW);  // DC Motor Stop
          Firebase.setFloat(firebaseData, "/Slot_Status/DCMotor",0);
     
        }
    
        else if (GearMotor.substring(1, 2) == "2")
        {
          digitalWrite(motor1Pin1, LOW); // control the motor's direction in clockwise
          digitalWrite(motor1Pin2, HIGH);  // control the motor's direction in clockwise
          delay (4000*2);                   // keep rotating for 8 Second.
          digitalWrite(motor1Pin1, LOW); // DC Motor Stop
          digitalWrite(motor1Pin2, LOW);  // DC Motor Stop
          Firebase.setFloat(firebaseData, "/Slot_Status/DCMotor",0);  
        }
    
    
        else if (GearMotor.substring(1, 2) == "3")
        {
          
          analogWrite(enable1Pin, Rspeed);
          digitalWrite(motor1Pin1, LOW); // control the motor's direction in clockwise
          digitalWrite(motor1Pin2, HIGH);  // control the motor's direction in clockwise
          delay (4000*3);                   // keep rotating for 12 Second.
          digitalWrite(motor1Pin1, LOW); // DC Motor Stop
          digitalWrite(motor1Pin2, LOW);  // DC Motor Stop
          Firebase.setFloat(firebaseData, "/Slot_Status/DCMotor",0);  
        }
    
    
        else if  (GearMotor.substring(1, 2) == "4") 
        {
          digitalWrite(led, LOW); 
          analogWrite(enable1Pin, Rspeed);
          digitalWrite(motor1Pin1, LOW); // control the motor's direction in clockwise
          digitalWrite(motor1Pin2, HIGH);  // control the motor's direction in clockwise
          delay (4000*4);                   // keep rotating for 16 Second.
          digitalWrite(motor1Pin1, LOW); // DC Motor Stop
          digitalWrite(motor1Pin2, LOW);  // DC Motor Stop
          Firebase.setFloat(firebaseData, "/Slot_Status/DCMotor",0);
       }
    
        
        
        else
        {
          digitalWrite(motor1Pin1, LOW); // DC Motor Stop
          digitalWrite(motor1Pin2, LOW);  // DC Motor Stop
        }
      
    }

    else if (slot6_status == 0)
    {
      digitalWrite(led, HIGH);
      digitalWrite(motor1Pin1, LOW); // DC Motor Stop
      digitalWrite(motor1Pin2, LOW);  // DC Motor Stop
    }

    
  }


  // Code For Read Data From Firebase of the (5) Micro Servo Motors:

  if (Firebase.getString(CicularDisc, "/Slot_Status/Slot-Disc-1"))
  {
    String CicularDisc1 = CicularDisc.stringData();
    if (CicularDisc1.substring(1, 2) == "1")
    {
        servo1.write(180);
        
    }
    
    else if (CicularDisc1.substring(1, 2) == "0")
    {    
          servo1.write(0);  

    }

  }


 
}
