#include "SR04.h"
#include <EEPROM.h>

//specify pins
int trig_pin1 = 12;
int echo_pin1 = 13;
int trig_pin2 = 4;
int echo_pin2 = 3;
int passive_buzzer_pin = 8;
int current_address = 0;
bool file_done = 0;

SR04 sensor1 = SR04(echo_pin1, trig_pin1);
SR04 sensor2 = SR04(echo_pin2, trig_pin2);

long distance1, distance2;
int bad_count = 0; // records how long you are slouching
int good_count = 0; // after you fix posture
int num_alerts = 0; // number of times you received the buzzer alert

void setup() {
  // put your setup code here, to run once:
  sessionStartTime = millis(); // begins recording time for session
  pinMode(passive_buzzer_pin, OUTPUT);
  Serial.begin(9600);
  delay(1000);
  
}


void dumpData() {

  int count = 0;

    Serial.println("Address, Value");

    for (int address = 0; address < 1024; address++){

      byte value = EEPROM.read(address);

      Serial.print(address);

      Serial.print(",");

      Serial.println(value);

    }

    Serial.print("-----EOF------");

    filedone = 1;
  }

void loop() {
  // put your main code here, to run repeatedly:
  distance1 = sensor1.Distance();
  distance2 = sensor2.Distance();
  Serial.print("Distance 1: ");
  Serial.println(distance1);
  Serial.print("Distance 2: ");
  Serial.println(distance2);

  EEPROM.write(currentaddress,distance1);
  currentaddress++;

  if (Serial.available() > 0){

    char command = Serial.read();
        if (command == 'd') {
      dumpData();
      if(filedone){
        exit(0);
      }
    if (command == 'd') {
      dumpData();
      if(filedone){
        exit(0);
      }
    }
        }
  }

  if (bad_count > 5) {
    tone(passive_buzzer_pin, 90); // plays tone
    num_alerts += 1;
    if (distance1 < distance2) {
      good_count += 1;
    }
    if (good_count > 5) {
      bad_count = 0;
      pinMode(passive_buzzer_pin, LOW);
      good_count = 0;
    }
  } else {
    pinMode(passive_buzzer_pin, LOW);
  }

  if (distance1 > distance2 + 1) {
    bad_count += 1; 
  } 

  // tone(passive_pin, 90); // turn passive buzzer on
  delay(800);

  Serial.print("Bad Count: ");
  Serial.println(bad_count);
  Serial.print("Good Count: ");
  Serial.println(good_count);
}

//right side goes to shoulder
//left side goes to neck