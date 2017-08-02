#define windowClosedSensor 2
#define windowOpenSensor 3

void setup() {
  Serial.begin(9600);
  pinMode(windowClosedSensor,INPUT);
  pinMode(windowOpenSensor, INPUT);
}

void loop() {
  Serial.print(checkClosed());
  Serial.println(checkOpen());
}

boolean checkClosed(){
  if(digitalRead(windowClosedSensor)==HIGH)
  return true;
  else
  return false;
}

boolean checkOpen(){
  if(digitalRead(windowOpenSensor)==HIGH)
  return true;
  else
  return false;
}


