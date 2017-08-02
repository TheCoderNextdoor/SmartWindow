#include <SFE_BMP180.h>
#include <Wire.h>
double dP,d2P,calib;
SFE_BMP180 p;

void setup() {
  Serial.begin(9600);
  Serial.println("REBOOT");
  p.begin();
  calibration();
//  if (p.begin())
//    Serial.println("BMP180 init success");
//  else
//  {
//    Serial.println("BMP180 init fail\n\n");
//    while(1);
//  }
}

void loop() {
  double T,P;

    Serial.println(deviation());
}

double temperature(){
  char status;
  double T;
  
  status = p.startTemperature();
  if (status != 0)
  {
    delay(status);
    p.getTemperature(T);
  }
  return T;
}

double pressure(){
  char status;
  double T,P;
  
  T = temperature();
   status = p.startPressure(3);
      if (status != 0)
      {
        delay(status);
        
        status = p.getPressure(P,T);
      }
      return P;
}

void pressureGradient(){
    double previousValue;
    double nextValue;
    double k;
  
    previousValue = pressure();
    delay(50);
    nextValue = pressure();
    k = nextValue - previousValue;
    dP += k;
    
}

void d2p(){
  double previousValue;
  double nextValue;
  double k;

  pressureGradient();
  previousValue = dP;
  delay(20);
  pressureGradient();
  nextValue = dP;

  k = nextValue - previousValue;
  d2P += k;
  
}

void normalisePressure(){
  if(dP>=5||dP<=-10)
    dP = 0;
    d2P = 0;
}

void calibration(){
  calib = pressure();
}

float deviation(){
  float d;
  d = calib - pressure();
  return d;
}


