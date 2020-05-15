// ========================================================================================
//      Meine Welt in meinem Kopf
// ========================================================================================
// Projekt:       Motor Driver for Powerfunction
// Author:        Johannes P. Langner
// Controller:    Arduino Pro Mini
// Receiver:      Can use a RC receiver with PWM outputs
// Actor:         Motor Driver Modul L298N
// Stand:         15.05.2020
// ========================================================================================

#define PIN_INPUT_1 A3
#define PIN_INPUT_2 A2

#define PIN_OUTPUT_A1 9              // pwm pin to control M1 output
#define PIN_OUTPUT_A2 6              // pwm pin to control M1 output
#define PIN_OUTPUT_B1 5              // pwm pin to control M1 output
#define PIN_OUTPUT_B2 3              // pwm pin to control M1 output

int DeathbandPlusMinus = 10;

boolean Reverse1 = true;
int InputMiddle1 = 1491;
int InputMin1 = 1073;
int InputMax1 = 1895;

boolean Reverse2 = true;
int InputMiddle2 = 1400;
int InputMin2 = 1102;
int InputMax2 = 1889;

void setup() {

  //Serial.begin(115200);

  pinMode(PIN_INPUT_1, INPUT);
  pinMode(PIN_INPUT_2, INPUT);

  pinMode(PIN_OUTPUT_A1, OUTPUT);
  pinMode(PIN_OUTPUT_A2, OUTPUT);
  pinMode(PIN_OUTPUT_B1, OUTPUT);
  pinMode(PIN_OUTPUT_B2, OUTPUT);
}

void loop() {

  int readValue1 = pulseIn(PIN_INPUT_1, HIGH);
  int readValue2 = pulseIn(PIN_INPUT_2, HIGH);

  MotorA(readValue1);
  MotorB(readValue2);

//  Serial.print("Anlaog 1: ");
//  Serial.println(readValue1, DEC);
//
//  Serial.print("Anlaog 2: ");
//  Serial.println(readValue2, DEC);
//  delay(100);
}

void MotorA(int readValue1) {

  int outputA1 = 0;
  int outputA2 = 0;
  
  if(readValue1 > InputMiddle1 + DeathbandPlusMinus) {
    int valueUp = readValue1 - InputMiddle1;
    float rateUp = GetRate(InputMax1 - InputMiddle1); 
    outputA1 = CutOffOutOfRange(rateUp * valueUp);
  }

  if(readValue1 < InputMiddle1 - DeathbandPlusMinus) {
    int valueDown = InputMiddle1 - readValue1;
    float rateDown = GetRate(InputMiddle1 - InputMin1); 
    outputA2 = CutOffOutOfRange(rateDown * valueDown);
  }

//  Serial.print("outputA1: "); Serial.println(outputA1, DEC);
//  Serial.print("outputA2: "); Serial.println(outputA2, DEC);

  if(Reverse1) {
    analogWrite(PIN_OUTPUT_A2, outputA1);  
    analogWrite(PIN_OUTPUT_A1, outputA2);
  }
  else {
    analogWrite(PIN_OUTPUT_A1, outputA1);  
    analogWrite(PIN_OUTPUT_A2, outputA2);
  }

}

void MotorB(int readValue) {

  int outputB1 = 0;
  int outputB2 = 0;
  
  if(readValue > InputMiddle2 + DeathbandPlusMinus) {
    int valueUp = readValue - InputMiddle2;
    float rateUp = GetRate(InputMax2 - InputMiddle2);
    outputB1 = CutOffOutOfRange(rateUp * valueUp);
  }

  if(readValue < InputMiddle2 - DeathbandPlusMinus) {
    int valueDown = InputMiddle2 - readValue;
    float rateDown = GetRate(InputMiddle2 - InputMin2); 
    outputB2 = CutOffOutOfRange(rateDown * valueDown);
  }

//  Serial.print("outputB1: "); Serial.println(outputB1, DEC);
//  Serial.print("outputB2: "); Serial.println(outputB2, DEC);
  if(Reverse2) {
    analogWrite(PIN_OUTPUT_B2, outputB1);  
    analogWrite(PIN_OUTPUT_B1, outputB2);
  }
  else {
    analogWrite(PIN_OUTPUT_B1, outputB1);  
    analogWrite(PIN_OUTPUT_B2, outputB2);
  }
  
}


float GetRate(int range){
  return 255.0 / (float)range;
}

int CutOffOutOfRange(int value) {
  if(value > 255) {
    value = 255;
  }

  if(value < 0) {
    value = 0;
  }

  return value;
}
