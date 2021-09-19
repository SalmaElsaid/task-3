#include <Wire.h>
#define imu_adress 0x68
float GyroX, GyroY;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY;
float accAngleX, accAngleY , gyroAngleX, gyroAngleY;
float elapsedTime, previousTime , currentTime;
float roll, pitch;
float AccX, AccY, AccZ;

int x = 0;
const int led =3;
void setup() {
  pinMode(led,OUTPUT);
  Wire.begin(); // initiate i2c system
  Wire.beginTransmission(imu_adress); //  communicate with imu
  Wire.write(0x6B);  //  register of imu
  Wire.write(0x00);     // set to zero 
  Wire.endTransmission(true); 
 

  calculate_IMU_error();  // calculate inital values 
  delay(20);
}

void loop() {
  Wire.beginTransmission(imu_adress);  
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(imu_adress, 4, true);
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0;
  
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - 0.58;  
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 1.58;
 
previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000;
  Wire.beginTransmission(imu_adress);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(imu_adress, 4, true);
  GyroX = Wire.read() << 8 | Wire.read();
  GyroY = Wire.read() << 8 | Wire.read();
  
  gyroAngleX +=  GyroX * elapsedTime; // deg/s * s = deg
  gyroAngleY += GyroY * elapsedTime;

  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
  if ( roll > 60 ||  pitch > 60 )
  {
    digitalWrite(led,HIGH);
    delay(100);
    digitalWrite(led,LOW);
    delay (100);
    }

}
void calculate_IMU_error() {
while (x < 1000) {
    Wire.beginTransmission(imu_adress);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(imu_adress, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    // Sum all readings
    AccErrorX +=  ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
    AccErrorY +=  ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    x++;
  }
  
  AccErrorX = AccErrorX / 1000;
  AccErrorY = AccErrorY / 1000;
  x = 0;
  while (x < 1000)
  { Wire.beginTransmission(imu_adress);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(imu_adress, 4, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroErrorX +=  (GyroX / 131.0);
    GyroErrorY +=  (GyroY / 131.0);
    
    x++;
  }
  GyroErrorX = GyroErrorX / 1000;
  GyroErrorY = GyroErrorY / 1000;
  ;

}
