# Aero PID Control

PID controller using two brushless motors with its respective ESC, and an Arduino Mega 2560.

The structure was created using SolidWorks 3D modeling software. We didn't make any mechanical simulation because I thought it was no necessary.

![structure](img/render.png)

Using Fritzing the electronic schematic diagram was made. I had to add some figures from the Fritzing community so I can build up the whole circuit. I used the Arduino Mega 2560 but you can use any other Arduino board as long as SDA and SCL pins are available.

![schematic](img/schematic.png)

This has the main function to compare my error with my variable of control (PID output) and the actual setpoint I wanted to reach, which was a great success all along.

![plot](img/plot.png)