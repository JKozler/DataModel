#include "HX711.h"
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
HX711 scale;

double loxSensorVoltage; // variable to store sensor voltage
double fuelSensorVoltage;

float pressureSlope1 = 11.5f; // slope of function voltage-pressure of Honeywell Pressure Transducer
float pressureYintercept1 = 5.75f; // y intercept of the linear function which models the voltage output of the Honeywell Pr. Tr.
double milliVolt = 0.0049609375f; 

int AnalogPinFUEL = A0;
int AnalogPinLOX = A1;

double loxPressure; 
double fuelPressure;

int infoPin = 8;
int reliefPin = 11;

int sequenceState = LOW;
int maxLoxPressure = 16;

bool loopSwitch = false;

int thrust;

void setup() {

  pinMode(LOADCELL_DOUT_PIN, OUTPUT);
  pinMode(LOADCELL_SCK_PIN, INPUT);

  pinMode(infoPin, INPUT);


  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

}


void loop() {

  sequenceState = digitalRead(infoPin);
  
  loxSensorVoltage = analogRead(AnalogPinLOX) * (5.0 / 1023.0); // conversion from analogRead to voltage constant (5V/1023)
  loxPressure = loxSensorVoltage * pressureSlope1 - pressureYintercept1;
  
  fuelSensorVoltage = analogRead(AnalogPinFUEL) * (5.0 / 1023.0);
  fuelPressure = fuelSensorVoltage * pressureSlope1 - pressureYintercept1;
  
  if (loxPressure > 16 && !loopSwitch){
    digitalWrite(reliefPin, HIGH);
    loopSwitch = true;
  } else if(loxPressure < 15.5f && loopSwitch) {
    digitalWrite(reliefPin, LOW);
    loopSwitch = false;
  }
  else if (!loopSwitch){
    digitalWrite(reliefPin, LOW);
  }
  
  Serial.print("FUEL: ");
  Serial.print(fuelPressure);
  Serial.print(" bar     LOX: ");
  Serial.print(loxPressure);
  Serial.print(" bar     THRUST: ");
  long x = scale.read();
  long y_hmotnost = 0.000287f * x - 15;
  Serial.print(y_hmotnost);
  
  Serial.println(" N");
  
  Serial.println(" ");



  delay(100);
}
