#ifndef MPU6050_H
#define MPU6050_H
struct MPU6050_sensorData {	/*  MPU6050 - 传感器数据  */
    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x, gyro_y, gyro_z;
};

bool MPU6050_check();
void MPU6050_init();
MPU6050_sensorData MPU6050_readSensor();  /* FIXME: 输入结构体引用（或指针）会更快 */
void MPU6050_calcAngle(const MPU6050_sensorData& data, float& 俯仰角, float& 翻滚角);

#endif
