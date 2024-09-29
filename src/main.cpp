#include <Arduino.h>
#include <PS4Controller.h>
#include <cmath>

const int SERIAL_SPEED = 9600;
const int LEDC_FREQ = 10000; //ledcの周波数
const int PWM_BITS = 8;

const int FIRST_PWM_PIN = 0;
const int FIRST_DIR_PIN = 24;
const int SECOND_PWM_PIN = 2;
const int SECOND_DIR_PIN = 25;
const int THIRD_PWM_PIN = 4;
const int THIRD_DIR_PIN = 26;

void omuni3(int vector[], int x, int y) {
  vector[0] = x * 1;
  vector[1] = x * 1/2 - y * sqrt(3)/2;
  vector[2] = x * -1/2 - y * sqrt(3)/2;
}

void setup() {
  Serial.begin(SERIAL_SPEED);
  PS4.begin();
  
  pinMode(FIRST_DIR_PIN, OUTPUT);
  pinMode(SECOND_DIR_PIN, OUTPUT);
  pinMode(THIRD_DIR_PIN, OUTPUT);
  ledcSetup(0, LEDC_FREQ, PWM_BITS);
  ledcSetup(1, LEDC_FREQ, PWM_BITS);
  ledcSetup(2, LEDC_FREQ, PWM_BITS);
  ledcAttachPin(FIRST_PWM_PIN, 0);
  ledcAttachPin(SECOND_PWM_PIN, 1);
  ledcAttachPin(THIRD_PWM_PIN, 2);
}

void loop() {
  if(!PS4.isConnected()){
    ledcWrite(0, 0);
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    return;
  }
  int coordinate_x = PS4.LStickX();
  int coordinate_y = PS4.LStickY();
  if(pow(coordinate_x, 2) + pow(coordinate_y, 2) < 90) {
    ledcWrite(0, 0);
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    return;
  }
  int vec[] = {0,0,0};
  omuni3(vec, coordinate_x, coordinate_y);
  for(int i = 0; i < 3; i++) {
    digitalWrite(FIRST_DIR_PIN, vec[i] > 0 ? LOW:HIGH);
    ledcWrite(i, vec[i]);
  }
}