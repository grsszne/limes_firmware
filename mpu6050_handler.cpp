#include "mpu6050_handler.h"

MPU6050 mpu(Wire);

void initMPU6050() {
    Wire.begin();
    mpu.begin();
    mpu.calcGyroOffsets(true);
}

MPU6050Data getMPU6050Data() {
    MPU6050Data data;
    mpu.update();

    data.temperature = mpu.getTemp();
    data.accel_x = mpu.getAccX();
    data.accel_y = mpu.getAccY();
    data.accel_z = mpu.getAccZ();
    data.gyro_x = mpu.getGyroX();
    data.gyro_y = mpu.getGyroY();
    data.gyro_z = mpu.getGyroZ();

    return data;
}
