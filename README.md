# Aero PID Control

PID controller using two brushless motors with its respective ESC, and an Arduino Mega 2560.

In order for the project to work, make sure to calibrate both ESC using the code given in the repository:
    
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

The structure was created using SolidWorks 3D modeling software. We didn't make any mechanical simulation because I thought it was no necessary.

![structure](img/render.png)

Using Fritzing the electronic schematic diagram was made. I had to add some figures from the Fritzing community so I can build up the whole circuit. I used the Arduino Mega 2560 but you can use any other Arduino board as long as SDA and SCL pins are available.

![schematic](img/schematic.png)

This has the main function to compare my error with my variable of control (PID output) and the actual setpoint I wanted to reach, which was a great success all along.

![plot](img/plot.png)
