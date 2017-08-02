#define brightness A1

void setup() {
  Serial.begin(9600);
  pinMode(brightness, INPUT);
}

void loop() {
  int b;
  b = checkLight();
  Serial.println(b);
}

int checkLight(){
  int bright;

  bright = analogRead(brightness);
  Serial.println(bright);
  delay(100);
  return bright;
}
