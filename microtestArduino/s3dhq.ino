#include <stdarg.h>
/* Fonction émulant le printf classque du C pour la communication serie */
void p(char *fmt, ... ){
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}

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
  acceleration.z = (int)( - cos(xMotorPos) * 1000);
  acceleration.y = (int)( - sin(xMotorPos) * 1000);
  acceleration.x = 0;
}

void xMotor(float xPos){
  xMotorPos = xPos;
}

void zMotor(float zPos){
  zMotorPos = zPos;
}

void captLumiere(){
  /* Les valeurs proposees n ont rien de reel */
  float natural = random(120, 255);
  lumiereMes.red = panneau1.red + panneau2.red + panneau2.red + natural;
  lumiereMes.green = panneau1.green + panneau2.green + panneau2.green + natural;
  lumiereMes.blue = panneau1.blue + panneau2.blue + panneau2.blue + natural;
}


void setup(){
  Serial.begin(9600);
  Serial.println("OK/ready");

  //Initialisation variables capteurs

  laser = false;

  xMotorPos = 0;
  zMotorPos = 0;

  panneau1 = {255, 255, 255};
  panneau2 = {255, 255, 255};
  panneau3 = {255, 255, 255};
  captLumiere();
  mesAcceleration();
}

void handleInput(String input){
  int len = input.length();

    switch (input.charAt(0)){
      case '0':

        xCons = atof(input.substring(1, 4).c_str());
        zCons = atof(input.substring(4, 7).c_str());
        xMotor(xCons);
        zMotor(zCons);

        p("OK/moteurs/%+06d/%+06d\n", (int) (100 * xMotorPos), (int) (100 * zMotorPos));

        break;
      case '1':
        // Laser
        laser = !laser;
        p("OK/laser/%d\n", laser);

        break;
      case '2':
        panneau1 = {input.substring(1, 4).toInt(), input.substring(4, 7).toInt(), input.substring(7, 10).toInt()};
        p("OK/panneau1/%03d/%03d/%03d\n", panneau1.red, panneau1.green, panneau1.blue);

        break;
      case '3':
        // Panneau led 2
        panneau2 = {input.substring(1, 4).toInt(), input.substring(4, 7).toInt(), input.substring(7, 10).toInt()};
        p("OK/panneau2/%03d/%03d/%03d\n", panneau2.red, panneau2.green, panneau2.blue);

        break;
      case '4':
        // Panneau led 3
        panneau3 = {input.substring(1, 4).toInt(), input.substring(4, 7).toInt(), input.substring(7, 10).toInt()};
        p("OK/panneau3/%03d/%03d/%03d\n", panneau3.red, panneau3.green, panneau3.blue);
        break;

      case '5':
        // Accéléromètre
        mesAcceleration();
        p("OK/acceleration/%+05d/%+05d/%+05d\n", acceleration.x, acceleration.y, acceleration.z);
        break;

      case '6':
        // Capteur lumière
        captLumiere();
        p("OK/lumiere/%03d/%03d/%03d\n", lumiereMes.red, lumiereMes.green, lumiereMes.blue);
        break;
      default:
        Serial.println("ERROR/format");
        break;
    }

  }

void loop(){
   if(Serial.available()){
     handleInput(Serial.readString());
   }
}