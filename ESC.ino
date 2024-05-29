#include <Servo.h>

#define RIGHT_ESC 8
#define LEFT_ESC 9

Servo ESC8;
Servo ESC9;

void setup()
{
  ESC8.attach(RIGHT_ESC, 1000, 2000);
  ESC9.attach(LEFT_ESC, 1000, 2000);
  ESC8.write(180);
  ESC9.write(180);
  delay(5000);
  ESC8.write(0);
  ESC9.write(0);
  delay(2000);
  ESC8.write(10);
  ESC9.write(10);
}

void loop()
{
}