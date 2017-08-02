#include <SFE_BMP180.h>
#include <Wire.h>

#define test1 A4
#define test2 A5

#define rainAnalog A0
#define rainDigital 12
#define brightness A1
#define windowClosedSensor 2
#define windowOpenSensor 3
#define motorInput1 5 //pwm pins
#define motorInput2 6 //5 and 6
#define manualOveride 7
#define manualOverideControl 8

const int motorsSpeed = 800;
const int rainDropsModuleConstant = 676;
double dP,d2P,calib;
int userLightThreshold = 250;
int setDeviationThreshold = 5;


boolean rai=false, pre=false, bri=false; //flags

SFE_BMP180 p;

void setup() {
  Serial.begin(9600);

  pinMode(manualOveride,INPUT);
  pinMode(manualOverideControl, INPUT);
  pinMode(windowClosedSensor,INPUT);
  pinMode(windowOpenSensor, INPUT);
  pinMode(brightness, INPUT);
  pinMode(rainAnalog, INPUT);
  pinMode(rainDigital, INPUT);
  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);

  Serial.println("REBOOT");
//  if(p.begin()){
//    Serial.println("reboot success");
//    calibration();
//  }
//  else
//    Serial.println("continue without bmp180");
}

void loop() {
  if(digitalRead(manualOveride)==LOW)
  {
    double T,P; //temperature and pressure
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
    deviate = deviation();
    
    setFlags(deviate);
    actionInitiated = takeAction();
  }
  else
  {
    if(digitalRead(manualOverideControl)==LOW)
      windowOpen();
    else
      windowClose();
  }
  Serial.println(analogRead(test1));
  Serial.println(analogRead(test2));
}

boolean takeAction(){                                     //returns 1 if action occurs 0 if not
  boolean pv,nv;
  pv = checkOpen();
  if(rai||bri||pre)
    windowClose();
  else
    windowOpen();
  nv = checkClosed();
  if(pv==nv)
    return 0;
  else 
    return 1;
}

void setFlags(double dev){                                          //sets all flags for algorithm processing
  
  if((checkLight() >= userLightThreshold)&&checkOpen())    //brightness contro
  {
    bri = true; 
  }
  else
  {
    bri = false;
  }
  

  if(waterOnSensor())                                //rain control
  {
    if(waterAmount()>350&&checkOpen())
      rai = true;
    else
      rai = false;
  }

  if(deviation()>setDeviationThreshold||dev == 1010)
  pre = true;
  else
  pre = false;
}

double temperature(){               //all functions for bmp180 start here
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
}                                         //all functions for bmp180 end here



boolean waterOnSensor(){                  //all functions for rainsensor start here
  boolean waterDetect = false;

  waterDetect = !digitalRead(rainDigital);
  return waterDetect;
}

int waterAmount(){
  int rainIntensity = 0;

  rainIntensity = rainDropsModuleConstant - analogRead(rainAnalog);
  return rainIntensity;
}                                         //all functions for rainsensor end here


int checkLight(){                         //all functions for brightness sensor start here
  int bright;

  bright = analogRead(brightness);
  return bright;
}                                         //all functions for brightness sensor end here



boolean checkClosed(){                    //all functions for ir start here
  if(digitalRead(windowClosedSensor)==HIGH&&digitalRead(windowOpenSensor)==LOW)
  return true;
  else
  return false;
}

boolean checkOpen(){
  if(digitalRead(windowOpenSensor)==HIGH&&digitalRead(windowClosedSensor)==LOW)
  return true;
  else
  return false;
}                                           //all functions for ir end here

int windowClose(){                           //window operations start
  if(checkOpen() == true){
    while(!checkClosed()){
      analogWrite(motorInput1, motorsSpeed);
      analogWrite(motorInput2, 0);
    }
  }
  else
  return 1;
}

int windowOpen(){
  if(checkClosed() == true){
    while(!checkOpen()){
      analogWrite(motorInput2, motorsSpeed);
      analogWrite(motorInput1, 0);
    }
  }
  else
  return 1;
}                                             //window operations end


//primitive functions

void motorsRunFwd(){
  analogWrite(motorInput2, motorsSpeed);
  analogWrite(motorInput1, 0);
}

void motorsRunBwd(){
  analogWrite(motorInput1, motorsSpeed);
  analogWrite(motorInput2, 0);
}

