#define rainAnalog A0
#define rainDigital 12
const int rainDropsModuleConstant = 676;

void setup() {
  Serial.begin(9600);
  pinMode(rainAnalog, INPUT);
  pinMode(rainDigital, INPUT);
}

void loop() {
  boolean waterDetect = false;
  int rainIntensity = 0;

  waterDetect = !digitalRead(rainDigital);
  rainIntensity = rainDropsModuleConstant - analogRead(rainAnalog);

  Serial.println(waterOnSensor());
  Serial.println(waterAmount());
  delay(100);
}

boolean waterOnSensor(){
  boolean waterDetect = false;

  waterDetect = !digitalRead(rainDigital);
  return waterDetect;
}

int waterAmount(){
  int rainIntensity = 0;

  rainIntensity = rainDropsModuleConstant - analogRead(rainAnalog);
  return rainIntensity;
}

