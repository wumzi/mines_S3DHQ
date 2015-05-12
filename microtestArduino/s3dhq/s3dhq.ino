#include <stdarg.h>
#include <Servo.h>
#include <Stepper.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

/* Fonction émulant le printf classque du C pour la communication serie */
void p(char *fmt, ... ){
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}

//Pins

const int lumPin = 0;
const int laserPin = 2;
const int ledBluePin = 3;
const int ledGreenPin = 4;
const int ledRedPin = 5;

Servo servoX;

#define STEPS 100
Stepper stepper(STEPS, 13, 12, 11, 10);
//LiquidCrystal_I2C lcd(0x27,16,2);

float xMotorPos;
float zMotorPos;
float xCons;
float zCons;


struct panneauLed {
  int red;
  int green;
  int blue;
};

struct panneauLed panneau1, panneau2, panneau3;
struct panneauLed lumiereMes;


boolean laser;

struct Acceleration {
  int x;
  int y;
  int z;
}; // les accelerations sont données en g.

struct Acceleration acceleration;

void mesAcceleration(){
  /* Dans un monde magique */
  acceleration.z = (int)( cos(xMotorPos/180 * 3.14159) * 1000);
  acceleration.y = (int)( sin(xMotorPos/180 * 3.14159) * 1000);
  acceleration.x = 0;
}

void xMotor(float xPos){
  servoX.write(xPos);
  xMotorPos = servoX.read();
}

void zMotor(float zPos){
  stepper.step((zPos - zMotorPos)/360 * 2048);
  zMotorPos = zPos;
}

void captLumiere(){
  /* Les valeurs proposees n ont rien de reel */
  float natural = random(120, 255);
  lumiereMes.red = panneau1.red + panneau2.red + panneau2.red + natural;
  lumiereMes.green = panneau1.green + panneau2.green + panneau2.green + natural;
  lumiereMes.blue = panneau1.blue + panneau2.blue + panneau2.blue + natural;
}

void setLaser(bool consLaser){
  if (consLaser){
    digitalWrite(laserPin, HIGH);
  } else {
    digitalWrite(laserPin, LOW);
  }
  laser = consLaser;
}

void updatePanneau(){
  analogWrite(ledRedPin, panneau1.red);
  analogWrite(ledGreenPin, panneau1.green);
  analogWrite(ledBluePin, panneau1.blue);
}

void setup(){
  /*lcd.init();
  lcd.backlight();*/

  Serial.begin(9600);
  Serial.println("$OK_ready/");

  //Initialisation variables capteurs

  laser = false;

  xMotorPos = 0;
  zMotorPos = 0;

  panneau1 = {255, 255, 255};
  panneau2 = {255, 255, 255};
  panneau3 = {255, 255, 255};

  // Setup pins

  pinMode(laserPin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);

  servoX.attach(9);
  servoX.write(10);
  stepper.setSpeed(90);

  captLumiere();
  mesAcceleration();

  /*lcd.clear();
  lcd.print("m3ch4tr0niCs,");
  lcd.setCursor(0,1);
  lcd.print("lUlZ 4 L1f3 !!!!");*/
}

void handleInput(String input){
  int len = input.length();

  if (input.charAt(0) != '$' or input.charAt(len - 1) != '/'){
    Serial.println("$ERROR_format/");
  }
  else{
    input = input.substring(1, len - 1);
    switch (input.charAt(0)){
      case '0':
        xCons = atof(input.substring(1, 4).c_str());
        zCons = atof(input.substring(4, 7).c_str());
        xMotor(xCons);
        zMotor(zCons);

      case '1':
        xMotorPos = servoX.read();
        p("$OK_moteurs_%+06d_%+06d/\n", (int) (100 * xMotorPos), (int) (100 * zMotorPos));

        break;
      case '2':
        // Laser
        if (input.charAt(1) == '1'){
          setLaser(true);
        } else {
          setLaser(false);
        }

        p("$OK_laser_%d/\n", laser);

        break;
      case '3':
        panneau1 = {input.substring(1, 4).toInt(), input.substring(4, 7).toInt(), input.substring(7, 10).toInt()};
        updatePanneau();
        p("$OK_panneau_1_%03d_%03d_%03d/\n", panneau1.red, panneau1.green, panneau1.blue);

        break;
      case '4':
        // Panneau led 2
        panneau2 = {input.substring(1, 4).toInt(), input.substring(4, 7).toInt(), input.substring(7, 10).toInt()};
        updatePanneau();
        p("$OK_panneau_2_%03d_%03d_%03d/\n", panneau2.red, panneau2.green, panneau2.blue);

        break;
      case '5':
        // Panneau led 3
        panneau3 = {input.substring(1, 4).toInt(), input.substring(4, 7).toInt(), input.substring(7, 10).toInt()};
        updatePanneau();
        p("$OK_panneau_3_%03d_%03d_%03d/\n", panneau3.red, panneau3.green, panneau3.blue);
        break;

      case '6':
        // Accéléromètre
        mesAcceleration();
        p("$OK_acceleration_%+05d_%+05d_%+05d/\n", acceleration.x, acceleration.y, acceleration.z);
        break;

      case '7':
        // Capteur lumière
        captLumiere();
        p("$OK_lumiere_%03d_%03d_%03d/\n", lumiereMes.red, lumiereMes.green, lumiereMes.blue);
        break;
      default:
        Serial.println("$ERROR_format/");
        break;
    }
  }
}

void loop(){
  xMotorPos = servoX.read();
  while(Serial.available()){
    handleInput(Serial.readString());
  }
}