#include "SR04.h"
#include <EEPROM.h>

//specify pins
int trig_pin1 = 12;
int echo_pin1 = 13;
int trig_pin2 = 4;
int echo_pin2 = 3;
int passive_buzzer_pin = 8;
int currentaddress = 0;
bool filedone = 0;

SR04 sensor1 = SR04(echo_pin1, trig_pin1);
SR04 sensor2 = SR04(echo_pin2, trig_pin2);

long distance1, distance2;
int bad_count = 0; // records how long you are slouching
int good_count = 0; // after you fix posture
int num_alerts = 0; // number of times you received the buzzer alert

void setup() {
  // put your setup code here, to run once:
  //sessionStartTime = millis(); // begins recording time for session
  pinMode(passive_buzzer_pin, OUTPUT);
  Serial.begin(9600);
  delay(1000);
  
}


void dumpData() {

  int count = 0;

    Serial.println("Number of Alerts |  Bad Count");
    Serial.println("---------------------------");

    for (int address = 0; address < 1024; address++){
      byte value = EEPROM.read(address);

      Serial.print(value);
      if (address%2==0){
        Serial.println("");
      }else{
        Serial.print(", ");
      }
    }
    Serial.print("-----EOF------");
    filedone = 1;
  }

void loop() {
  // put your main code here, to run repeatedly:
  distance1 = sensor1.Distance();
  distance2 = sensor2.Distance();

  if (Serial.available() > 0){

    char command = Serial.read();

    if (command == 'd') {
      dumpData();
      if(filedone){
        exit(0);
      }
    }

    if(command == 'c'){
      Serial.println("Clearing...");
      for(int i=0; i< EEPROM.length(); i++){
        EEPROM.write(i,255);
      }
      currentaddress = 0;
      bad_count = 0;
    }
    Serial.println("Cleared");
  }
  

  if (bad_count == 3) {
    num_alerts += 1;
  }
  
  if (bad_count > 3) {
    tone(passive_buzzer_pin, 90); // plays tone
    if (distance1 < distance2) {
      good_count += 1;
    }
    if (good_count > 3) {
      bad_count = 0;
      pinMode(passive_buzzer_pin, LOW);
      good_count = 0;
    }
  } else {
    pinMode(passive_buzzer_pin, LOW);
  }

  if (distance1 > distance2) {
    bad_count += 1; 
  } 

  // tone(passive_pin, 90); // turn passive buzzer on
  delay(800);

  //Serial.print("Bad Count: ");
  Serial.print(bad_count);
  EEPROM.write(currentaddress,bad_count);
  currentaddress++;

  Serial.print(", ");
  

  //Serial.print("Number of Alerts: ");
  Serial.println(num_alerts);
  EEPROM.write(currentaddress,num_alerts);
  currentaddress++;
}

//right side goes to shoulder
//left side goes to neck
