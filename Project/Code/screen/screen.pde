//import processing.serial.*;
//Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port
PFont d4;
String pressure="na";
String temperature="na";
int rain=0;
String light="na";
String rai ="na";
String warning= " ";
PImage logo;
int setAlpha = 255;
void setup()
{
  // I know that the first port in the serial list on my mac
  // is Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  //String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
  //myPort = new Serial(this, portName, 9600);
  logo = loadImage("smartWindow2.jpg");

  d4 = createFont("Arial", 16, true);
  size(1378, 700);
  frameRate(30);
  noStroke();
}

void draw()
{
  background(0);

  fill(0);
  pressure = "998";
  temperature ="27";
  rai ="on";
  light = "low";
  warning = "Pressure has\ndropped.\n\nIts raining.\n\n";
  render();
  //checkSerial();
  //getInput();
}
void getInput() {
  if (val!=null) {
    if (val.length()>=15) {

      char[]  press = new char[7];
      press[0] = val.charAt(0);
      press[1] = val.charAt(1);
      press[2] = val.charAt(2);
      press[3] = val.charAt(3);
      press[4] = val.charAt(4);
      press[5] = val.charAt(5);
      press[6] = val.charAt(6);
      pressure = new  String(press);
      println(pressure);

      char[]  temp = new char[5];
      temp[0] = val.charAt(7);
      temp[1] = val.charAt(8);
      temp[2] = val.charAt(9);
      temp[3] = val.charAt(10);
      temp[4] = val.charAt(11);
      temperature = new  String(temp);
      println(temperature);


      rain = val.charAt(12);
      if (rain == 48)
        rai = "Off";
      else
        rai = "On";

      if (val.length()>15) {
        char lig[] = new char[1];
        lig[0] = val.charAt(15);

        if (lig[0]=='1')
          light = "Low";
        else if (lig[0]=='2')
          light= "Medium";
        else
          light = "Intense";
      }

      if (val.length()>16) {
        char warn;
        warn = val.charAt(16);

        if (warn =='1')
          warning="Low atmospheric\npressure detected.\nChance of rain";
      }
    }
  }
}
void render() {
  fill(200, setAlpha);
  rect(0, 0, 540, 360);
  fill(200, 40, 60, setAlpha);
  rect(540, 0, 540, 360);
  fill(0, 30, 255, setAlpha);
  rect(0, 360, 540, 360);
  fill(255, 255, 0, setAlpha);
  rect(540, 360, 540, 360);

  textFont(d4, 32);
  fill(0);
  text("Pressure", 180, 90);
  text(pressure, 180, 180);
  text("      mB", 250, 180);
  fill(255, 255, 0);
  text("Temperature", 750, 90);
  text(temperature, 800, 180);
  text("        ^C", 800, 180);
  fill(255);
  text("Rain sensor", 150, 500);
  text(rai, 200, 600);
  fill(0);
  text("Light intensity", 750, 500);
  text(light, 800, 600);

  fill(245, 194, 10, setAlpha);
  rect(1080, 0, 298, 720);
  fill(0);
  textFont(d4, 28);
  text(warning, 1100, 300);
  image(logo, 1220, 50, 250, 50);
  imageMode(CENTER);
}

/* void checkSerial() {
  if ( myPort.available() > 0) 
  {  // If data is available,
    val = myPort.readStringUntil('\n');
  }
}*/