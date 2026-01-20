#include "SR04.h"

//specify pins
int trig_pin1 = 12;
int echo_pin1 = 11;
int trig_pin2 = 4;
int echo_pin2 = 3;
int led_pin = 8;

SR04 sensor1 = SR04(echo_pin1, trig_pin1);
SR04 sensor2 = SR04(echo_pin2, trig_pin2);

long distance1, distance2;

void setup() {
  // put your setup code here, to run once:
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
  delay(1000);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  distance1 = sensor1.Distance();
  distance2 = sensor2.Distance();
  Serial.print("Distance 1: ");
  Serial.println(distance1);
  Serial.print("Distance 2: ");
  Serial.println(distance2);

  if (distance1 > distance2) {
    digitalWrite(led_pin, HIGH); // Turn LED on
  } else {
    digitalWrite(led_pin, LOW);  // Turn LED off
  }
  
  delay(800);

}
