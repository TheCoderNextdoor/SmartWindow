#define motorInput1 5 //pwm pins
#define motorInput2 6 //5 and 6

const int motorsSpeed = 800;


void setup() {
  // put your setup code here, to run once:
  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  motorsRunFwdFull();
}

void motorsRunFwd(){
  analogWrite(motorInput2, motorsSpeed);
  analogWrite(motorInput1, 0);
}

void motorsRunBwd(){
  analogWrite(motorInput1, motorsSpeed);
  analogWrite(motorInput2, 0);
}

void motorsRunFwdFull(){
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
}

