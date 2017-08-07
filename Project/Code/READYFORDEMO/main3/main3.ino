//////////////////////////////////////////////
//SFE_BMP180.h is for BMP module
//Wire.h required for serial comm with arduino
//////////////////////////////////////////////
#include <SFE_BMP180.h>
#include <Wire.h>
//////////////////////////////////////////////
//all pin assignments are done here
//////////////////////////////////////////////
#define rainAnalog A0
#define rainDigital 12
#define brightness A1
#define windowClosedSensor 13
#define windowOpenSensor 3
#define motorInput1 5 //pwm pins
#define motorInput2 6 //5 and 6
#define manualOveride 7
#define manualOverideControl 8
//////////////////////////////////////////////
//a few constants defined for uses:
//motorsSpeed changes motorspeed
//
//rainDropsModuleConstant sets threshold value
//for ADC
//
//userLightThreshold changes sensitivity to
//light from the photodiode
//higher value lowers sensitivity
//
//setRainAmount##
//
//setDeviationThreshold##
//
//////////////////////////////////////////////
const int motorsSpeed = 1000;
const int rainDropsModuleConstant = 1023;
double dP, d2P, calib;
int userLightThreshold = 250;
int setDeviationThreshold = 5;
int setRainAmount = 300;

//////////////////////////////////////////////////
//these variables check whether the corresponding
//sensors have been activated
//////////////////////////////////////////////////
boolean rai = false, pre = false, bri = false; //flags
//////////////////////////////////////////////////
//BMP module object
//////////////////////////////////////////////////
SFE_BMP180 p;
//////////////////////////////////////////////////
//Initial setup
//////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);

  pinMode(manualOveride, INPUT);
  pinMode(manualOverideControl, INPUT);
  pinMode(windowClosedSensor, INPUT);
  pinMode(windowOpenSensor, INPUT);
  pinMode(brightness, INPUT);
  pinMode(rainAnalog, INPUT);
  pinMode(rainDigital, INPUT);
  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);

  Serial.println("REBOOT");
  if (p.begin()) {
    Serial.println("reboot success");
    calibration();
  }
  else
    Serial.println("continue without bmp180");
}

void loop() {
  if (digitalRead(manualOveride) == LOW)
  {
    double T, P; //temperature and pressure
    int lightLevel; //brightness level 0-1023 (int)
    boolean closed, openw; //closed and open ir sensors flags
    boolean rainDropsActive; //rain drops module
    int rainAmount;  //analog value of rain intensity
    double deviate; //pressure deviation from normal
    int actionInitiated;

    P = pressure();
    T = temperature();
    lightLevel = checkLight();
    closed = checkClosed();
    openw = checkOpen();
    rainDropsActive = waterOnSensor();
    rainAmount = waterAmount();

    setFlags();
    takeAction();

    int retLevel = 1;
    int chaos = 1;

    if (lightLevel < 350) {
      retLevel = 1;
    }
    else if (lightLevel < 700) {
      retLevel = 2;
    }
    else
      retLevel = 3;

    if (deviate >= 5)
      chaos = 1;
    else
      chaos = 0;

    Serial.print(P, 2);

    Serial.print(T, 2);

    Serial.print(rainDropsActive);

    Serial.print(closed);

    Serial.print(openw);

    Serial.print(retLevel);

    Serial.println(chaos);


  }
  else
  {
    Serial.println("enter manual mode");
    if (digitalRead(manualOverideControl) == LOW) {
      windowOpen();
      Serial.println("open");
    }
    else {
      windowClose();
      Serial.println("close");
    }
  }
}

boolean displayFlags() {
  Serial.println("");
  Serial.print(bri); Serial.print(" "); Serial.print(rai); Serial.print(" "); Serial.println(pre);
}

void takeAction() {
  if (rai || bri || pre)
    windowClose();
  else
    windowOpen();
}

void setFlags() {                                         //sets all flags for algorithm processing

  if ((checkLight() >= userLightThreshold))   //brightness contro
  {
    bri = true;
  }
  else
  {
    bri = false;
  }


  if (waterAmount() > setRainAmount)                    //rain control
    rai = true;
  else
    rai = false;

  if (deviation() > setDeviationThreshold)
    pre = true;
  else
    pre = false;
}

double temperature() {              //all functions for bmp180 start here
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

double pressure() {
  char status;
  double T, P;

  T = temperature();
  status = p.startPressure(3);
  if (status != 0)
  {
    delay(status);

    status = p.getPressure(P, T);
  }
  return P;
}

void pressureGradient() {
  double previousValue;
  double nextValue;
  double k;

  previousValue = pressure();
  delay(50);
  nextValue = pressure();
  k = nextValue - previousValue;
  dP += k;

}

void d2p() {
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

void normalisePressure() {
  if (dP >= 5 || dP <= -10)
    dP = 0;
  d2P = 0;
}

void calibration() {
  calib = pressure();
}

float deviation() {
  float d;
  if ((calib - pressure()) < 0) {
    d = abs(calib - pressure());
    return d;
  }
  else
    return 0;
}                                         //all functions for bmp180 end here



boolean waterOnSensor() {                 //all functions for rainsensor start here
  boolean waterDetect = false;

  waterDetect = !digitalRead(rainDigital);
  return waterDetect;
}

int waterAmount() {
  int rainIntensity = 0;

  rainIntensity = rainDropsModuleConstant - analogRead(rainAnalog);
  return rainIntensity;
}                                         //all functions for rainsensor end here


int checkLight() {                        //all functions for brightness sensor start here
  int bright;

  bright = analogRead(brightness);
  return bright;
}                                         //all functions for brightness sensor end here



boolean checkClosed() {                   //all functions for ir start here
  if (digitalRead(windowClosedSensor) == HIGH && digitalRead(windowOpenSensor) == LOW)
    return true;
  else
    return false;
}

boolean checkOpen() {
  if (digitalRead(windowOpenSensor) == HIGH && digitalRead(windowClosedSensor) == LOW)
    return true;
  else
    return false;
}                                           //all functions for ir end here

void windowClose() {                          //window operations start
  if (checkOpen() == true) {
    while (!digitalRead(windowClosedSensor)==HIGH) {
      analogWrite(motorInput1, motorsSpeed);
      analogWrite(motorInput2, 0);
    }
    analogWrite(motorInput2, 0);
    analogWrite(motorInput1, 0);
  }
}

void windowInaction() {
  analogWrite(motorInput2, 0);
  analogWrite(motorInput1, 0);
}

void windowOpen() {
  if (checkClosed() == true) {
    while (!digitalRead(windowOpenSensor)==HIGH) {
      analogWrite(motorInput2, motorsSpeed);
      analogWrite(motorInput1, 0);
    }
    analogWrite(motorInput2, 0);
    analogWrite(motorInput1, 0);
  }
}                                             //window operations end


//primitive functions

void motorsRunFwd() {
  analogWrite(motorInput2, motorsSpeed);
  analogWrite(motorInput1, 0);
}

void motorsRunBwd() {
  analogWrite(motorInput1, motorsSpeed);
  analogWrite(motorInput2, 0);
}

