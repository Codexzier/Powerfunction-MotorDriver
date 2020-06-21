// ========================================================================================
//      Meine Welt in meinem Kopf
// ========================================================================================
// Projekt:       Motor Driver for Lego Power Function
// Author:        Johannes P. Langner
// Controller:    Arduino Pro Mini
// Receiver:      Can use a RC receiver with PWM outputs
// Actor:         Motor Driver Modul MX1508
// Stand:         21.05.2020
// ========================================================================================

#define PIN_INPUT_A A3                // pwm input 1 for motor A
#define PIN_INPUT_B A2                // pwm input 2 for motor B

#define PIN_OUTPUT_A1 9               // pwm pin to control A1 output
#define PIN_OUTPUT_A2 6               // pwm pin to control A2 output
#define PIN_OUTPUT_B1 5               // pwm pin to control B1 output
#define PIN_OUTPUT_B2 3               // pwm pin to control B2 output

// ========================================================================================
// setup motor driver

bool mSerialMonitor = false;            // Set this value true, for show all Value on Serial Monitor.

int mVehicleMode = 1;                  // 0 = Car, Input 1 for speed and input 2 for steer
                                       //   - Alternate:  Tracked vehicle, Input 1 for speed motor A and 
                                       //                 Input 2 for speed motor B
                                       // 1 = Tracked vehicle, Input 1 for speed and input 2 for steer

boolean mVehicleMode_1_Steer = false;  // invert direction for steering

int mDeathbandPlusMinus = 10;          // Tolerance range in which no reaction should take 
                                       // place when sticks are in the middle position.

boolean mReverseA = true;             // change direction signal output for motor A
int mInputMiddleA = 1500;              // middel state of the input signal. Example: stick on neutral position
int mInputMinA = 987;                  // minimum value of input signal. Example: stick is move complete down
int mInputMaxA = 1988;                 // minimum value of input signal. Example: stick is move complete up

boolean mReverseB = false;              // change direaction sinal output for motor B
int mInputMiddleB = 1490;              // middel state of the input signal. Example: stick on neutral position
int mInputMinB = 981;                  // minimum value of input signal. Example: stick is move complete down
int mInputMaxB = 1999;                 // minimum value of input signal. Example: stick is move complete up

// ========================================================================================
void setup() {

  if(mSerialMonitor) {
    Serial.begin(115200);
  }

  pinMode(PIN_INPUT_A, INPUT);
  pinMode(PIN_INPUT_B, INPUT);

  pinMode(PIN_OUTPUT_A1, OUTPUT);
  pinMode(PIN_OUTPUT_A2, OUTPUT);
  pinMode(PIN_OUTPUT_B1, OUTPUT);
  pinMode(PIN_OUTPUT_B2, OUTPUT);
}

// ========================================================================================
void loop() {

  int readValueA = pulseIn(PIN_INPUT_A, HIGH);
  int readValueB = pulseIn(PIN_INPUT_B, HIGH);

  switch(mVehicleMode) {
    case(0): {
      MotorA(readValueA);
      MotorB(readValueB);
      break;
    }
    case(1): {
      VehicleMode_1(readValueA, readValueB);
      break;
    }
  }
  
  if(mSerialMonitor) {
    Serial.print("Anlaog 1: ");
    Serial.print(readValueA, DEC);
  
    Serial.print(", Anlaog 2: ");
    Serial.println(readValueB, DEC);
    delay(100);
  }
}

// ========================================================================================
// Map the input signal for tracked vehicle.
// Channel 1 is for speed control
// Channel 2 is for steer
// ----------------------------------------------------------------------------------------
// valueA = Signal from channel 1
// valueB = Siganl from channel 2
void VehicleMode_1(int valueA, int valueB){

  int speedValueA = valueA;
  int speedValueB = valueA;
  
  int valueSteer = valueB - mInputMiddleB;

  if(valueA > mInputMiddleA) {
    SetSpeedValueMinus(valueSteer, &speedValueA, &speedValueB);
  }
  else {
    SetSpeedValuePlus(valueSteer, &speedValueA, &speedValueB);
  }
  
  if(mVehicleMode_1_Steer) {
    int tmp = speedValueB;
    speedValueB = speedValueA;
    speedValueA = tmp;
  }
  
  if(mSerialMonitor) {
    Serial.print("SpeedvalueA: ");
    Serial.print(speedValueA, DEC);
    Serial.print(", SpeedvalueB: ");
    Serial.print(speedValueB, DEC);
    Serial.print(", ");
  }

  MotorA(speedValueA);
  MotorB(speedValueB);
}

// ========================================================================================
// Substract from speed value for steering
// ----------------------------------------------------------------------------------------
// valueSteer  = value signal from channel 2
// speedValueA = value signal from channel 1 for motor A
// speedValueB = value signal from channel 1 for motor B
void SetSpeedValueMinus(int valueSteer, int *speedValueA, int *speedValueB) {
  if(valueSteer > 0) {
    *speedValueB -= valueSteer;
  }
  else {
    *speedValueA -= (valueSteer * -1);
  }
}

// ========================================================================================
// Adds to the speed value for steering
// ----------------------------------------------------------------------------------------
// valueSteer  = value signal from channel 2
// speedValueA = value signal from channel 1 for motor A
// speedValueB = value signal from channel 1 for motor B
void SetSpeedValuePlus(int valueSteer, int *speedValueA, int *speedValueB) {
  if(valueSteer > 0) {
    *speedValueB += valueSteer;
  }
  else {
    *speedValueA += (valueSteer * -1);
  }
}

// ========================================================================================
// write the speed value for motor A
// ----------------------------------------------------------------------------------------
// readValue = raw signal value from channel 1 or mapping with vehicle mode.
void MotorA(int readValue) {

  int outputA1 = 0;
  int outputA2 = 0;
  
  if(readValue > mInputMiddleA + mDeathbandPlusMinus) {
    int valueUp = readValue - mInputMiddleA;
    float rateUp = GetRate(mInputMaxA - mInputMiddleA); 
    outputA1 = CutOffOutOfRange(rateUp * valueUp);
  }

  if(readValue < mInputMiddleA - mDeathbandPlusMinus) {
    int valueDown = mInputMiddleA - readValue;
    float rateDown = GetRate(mInputMiddleA - mInputMinA); 
    outputA2 = CutOffOutOfRange(rateDown * valueDown);
  }

  if(mReverseA) {
    analogWrite(PIN_OUTPUT_A2, outputA1);  
    analogWrite(PIN_OUTPUT_A1, outputA2);
  }
  else {
    analogWrite(PIN_OUTPUT_A1, outputA1);  
    analogWrite(PIN_OUTPUT_A2, outputA2);
  }

  if(mSerialMonitor) {
    Serial.print("Output A1: ");
    Serial.print(outputA1, DEC);
    Serial.print(", Output A2: ");
    Serial.print(outputA2, DEC);
    Serial.print(", ");
  }
}

// ========================================================================================
// write the speed value for motor B
// ----------------------------------------------------------------------------------------
// readValue = raw signal value from channel 1 or mapping with vehicle mode.
void MotorB(int readValue) {

  int outputB1 = 0;
  int outputB2 = 0;
  
  if(readValue > mInputMiddleB + mDeathbandPlusMinus) {
    int valueUp = readValue - mInputMiddleB;
    float rateUp = GetRate(mInputMaxB - mInputMiddleB);
    outputB1 = CutOffOutOfRange(rateUp * valueUp);
  }

  if(readValue < mInputMiddleB - mDeathbandPlusMinus) {
    int valueDown = mInputMiddleB - readValue;
    float rateDown = GetRate(mInputMiddleB - mInputMinB); 
    outputB2 = CutOffOutOfRange(rateDown * valueDown);
  }

  if(mReverseB) {
    analogWrite(PIN_OUTPUT_B2, outputB1);  
    analogWrite(PIN_OUTPUT_B1, outputB2);
  }
  else {
    analogWrite(PIN_OUTPUT_B1, outputB1);  
    analogWrite(PIN_OUTPUT_B2, outputB2);
  }

  if(mSerialMonitor) {
    Serial.print("Output B1: ");
    Serial.print(outputB1, DEC);
    Serial.print(", Output B2: ");
    Serial.print(outputB2, DEC);
    Serial.print(", ");
  }
}

// ========================================================================================
// Get from range value the rate value.
// ----------------------------------------------------------------------------------------
// range = set value from 0 to 255
float GetRate(int range){
  return 255.0 / (float)range;
}

// ========================================================================================
// Cut the value if it higher than 255 or lower than 0.
// ----------------------------------------------------------------------------------------
// value = proof this value to cut transferred speed value
int CutOffOutOfRange(int value) {
  if(value > 255) {
    value = 255;
  }

  if(value < 0) {
    value = 0;
  }

  return value;
}
