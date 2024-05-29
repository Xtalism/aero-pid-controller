#include <Wire.h>
#include <Servo.h>

Servo right_prop;
Servo left_prop;

// MPU-6050 gives you 16 bits data so you have to create some 16int constants to store the data for accelerations and gyro*/

int16_t Acc_rawX, Acc_rawY, Acc_rawZ, Gyr_rawX, Gyr_rawY, Gyr_rawZ;

float Acceleration_angle[2];
float Gyro_angle[2];
float Total_angle[2];

float elapsedTime, time, timePrev;
int i;
float rad_to_deg = 180 / 3.141592654;

float PID, pwmLeft, pwmRight, error, previous_error;
float pid_p = 0;
float pid_i = 0;
float pid_d = 0;

// PID Constants
double kp = 3.55;
double ki = 0.003;
double kd = 0.0125;

double throttle = 1300;   // Initial throttle value
float desired_angle = -5; // Setpoint

void setup()
{
  Wire.begin(); // Begin the wire comunication between the Arduino and the MPU-6050
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(250000);

  right_prop.attach(8, 1000, 2000); // Attach the right motor to pin 8
  left_prop.attach(9, 1000, 2000);  // Attach the left motor to pin 9
  right_prop.write(180);
  left_prop.write(180);
  delay(5000);
  right_prop.write(0);
  left_prop.write(0);
  delay(2000);
  right_prop.write(10);
  left_prop.write(10);

  time = millis(); // Start counting time in milliseconds
  left_prop.writeMicroseconds(1000);
  right_prop.writeMicroseconds(1000);
  delay(7000); // Time to connect the propellers
}

void loop()
{

  // MPU-6050 signal
  timePrev = time; // the previous time is stored before the actual time read
  time = millis(); // actual time read
  elapsedTime = (time - timePrev) / 1000;

  Wire.beginTransmission(0x68);
  Wire.write(0x3B); // Ask for the 0x3B register- correspond to AcX
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 6, true);

  Acc_rawX = Wire.read() << 8 | Wire.read(); // each value needs two registres
  Acc_rawY = Wire.read() << 8 | Wire.read();
  Acc_rawZ = Wire.read() << 8 | Wire.read();

  /*---X---*/
  Acceleration_angle[0] = atan((Acc_rawY / 16384.0) / sqrt(pow((Acc_rawX / 16384.0), 2) + pow((Acc_rawZ / 16384.0), 2))) * rad_to_deg;
  /*---Y---*/
  Acceleration_angle[1] = atan(-1 * (Acc_rawX / 16384.0) / sqrt(pow((Acc_rawY / 16384.0), 2) + pow((Acc_rawZ / 16384.0), 2))) * rad_to_deg;

  Wire.beginTransmission(0x68);
  Wire.write(0x43); // Gyro data first adress
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 4, true); // Just 4 registers

  Gyr_rawX = Wire.read() << 8 | Wire.read(); // Once again we shif and sum
  Gyr_rawY = Wire.read() << 8 | Wire.read();

  /*---X---*/
  Gyro_angle[0] = Gyr_rawX / 131.0;
  /*---Y---*/
  Gyro_angle[1] = Gyr_rawY / 131.0;

  /*---X axis angle---*/
  Total_angle[0] = 0.98 * (Total_angle[0] + Gyro_angle[0] * elapsedTime) + 0.02 * Acceleration_angle[0];
  /*---Y axis angle---*/
  Total_angle[1] = 0.98 * (Total_angle[1] + Gyro_angle[1] * elapsedTime) + 0.02 * Acceleration_angle[1];

  // PID Control system
  error = Total_angle[1] - desired_angle;

  pid_p = kp * error;

  if (-3 < error < 3)
  {
    pid_i = pid_i + (ki * error);
  }

  pid_d = kd * ((error - previous_error) / elapsedTime);

  PID = pid_p + pid_i + pid_d;

  if (PID < -1000)
  {
    PID = -1000;
  }
  if (PID > 1000)
  {
    PID = 1000;
  }

  // We calculate the PWM width. We sum the desired throttle and the PID value
  pwmLeft = throttle + PID;
  pwmRight = throttle - PID;

  // Right motor limir
  if (pwmRight < 1000)
  {
    pwmRight = 1000;
  }
  if (pwmRight > 2000)
  {
    pwmRight = 2000;
  }
  // Left motor limit
  if (pwmLeft < 1000)
  {
    pwmLeft = 1000;
  }
  if (pwmLeft > 2000)
  {
    pwmLeft = 2000;
  }

  // Using the servo function we create the PWM pulses
  left_prop.writeMicroseconds(pwmLeft);
  right_prop.writeMicroseconds(pwmRight);
  previous_error = error; // Store the previous error.

  // Print the data in the Serial Monitor and we plot it
  Serial.print(",");
  Serial.print(Total_angle[1]);
  Serial.print(",");
  Serial.print(error);
  Serial.print(",");
  Serial.print(PID);
  Serial.print(",");
  Serial.println(previous_error);
  Serial.print(",");
}
