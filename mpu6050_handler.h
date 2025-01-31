#ifndef MPU6050_HANDLER_H
#define MPU6050_HANDLER_H

#include <Wire.h>
#include <MPU6050_tockn.h>

struct MPU6050Data {
    float temperature;
    float accel_x, accel_y, accel_z;
    float gyro_x, gyro_y, gyro_z;
};

void initMPU6050();
MPU6050Data getMPU6050Data();

#endif
