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

#define PIN_INPUT_A A3                // pwm input 1 for motor A
#define PIN_INPUT_B A2                // pwm input 2 for motor B

#define PIN_OUTPUT_A1 9               // pwm pin to control A1 output
#define PIN_OUTPUT_A2 6               // pwm pin to control A2 output
#define PIN_OUTPUT_B1 5               // pwm pin to control B1 output
#define PIN_OUTPUT_B2 3               // pwm pin to control B2 output

int VehicleMode = 0;                  // 0 = Car, Input 1 for speed and input 2 for steer
                                      // 1 = Tracked vehicle, Input 1 for speed and input 2 for steer
                                      // 2 = Tracked vehicle, Input 1 for speed motor A and Input 2 for speed motor B

int DeathbandPlusMinus = 10;          // Tolerance range in which no reaction should take 
                                      // place when sticks are in the middle position.

boolean ReverseA = true;              // change direction signal output for motor A
int InputMiddleA = 1491;              // middel state of the input signal. Example: stick on neutral position
int InputMinA = 1073;                 // minimum value of input signal. Example: stick is move complete down
int InputMaxA = 1895;                 // minimum value of input signal. Example: stick is move complete up

boolean ReverseB = true;              // change direaction sinal output for motor B
int InputMiddleB = 1400;              // middel state of the input signal. Example: stick on neutral position
int InputMinB = 1102;                 // minimum value of input signal. Example: stick is move complete down
int InputMaxB = 1889;                 // minimum value of input signal. Example: stick is move complete up

void setup() {

  //Serial.begin(115200);

  pinMode(PIN_INPUT_A, INPUT);
  pinMode(PIN_INPUT_B, INPUT);

  pinMode(PIN_OUTPUT_A1, OUTPUT);
  pinMode(PIN_OUTPUT_A2, OUTPUT);
  pinMode(PIN_OUTPUT_B1, OUTPUT);
  pinMode(PIN_OUTPUT_B2, OUTPUT);
}

void loop() {

  int readValueA = pulseIn(PIN_INPUT_A, HIGH);
  int readValueB = pulseIn(PIN_INPUT_B, HIGH);

  MotorA(readValueA);
  MotorB(readValueB);

// out comment it, if you looking the min, max and middle value of the inputs
//  Serial.print("Anlaog 1: ");
//  Serial.println(readValue1, DEC);
//
//  Serial.print("Anlaog 2: ");
//  Serial.println(readValue2, DEC);
//  delay(100);
}

void MotorA(int readValue) {

  int outputA1 = 0;
  int outputA2 = 0;
  
  if(readValue > InputMiddleA + DeathbandPlusMinus) {
    int valueUp = readValue - InputMiddleA;
    float rateUp = GetRate(InputMaxA - InputMiddleA); 
    outputA1 = CutOffOutOfRange(rateUp * valueUp);
  }

  if(readValue < InputMiddleA - DeathbandPlusMinus) {
    int valueDown = InputMiddleA - readValue;
    float rateDown = GetRate(InputMiddleA - InputMinA); 
    outputA2 = CutOffOutOfRange(rateDown * valueDown);
  }

  if(ReverseA) {
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
  
  if(readValue > InputMiddleB + DeathbandPlusMinus) {
    int valueUp = readValue - InputMiddleB;
    float rateUp = GetRate(InputMaxB - InputMiddleB);
    outputB1 = CutOffOutOfRange(rateUp * valueUp);
  }

  if(readValue < InputMiddleB - DeathbandPlusMinus) {
    int valueDown = InputMiddleB - readValue;
    float rateDown = GetRate(InputMiddleB - InputMinB); 
    outputB2 = CutOffOutOfRange(rateDown * valueDown);
  }

  if(ReverseB) {
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
