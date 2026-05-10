
/*
 * FIXME: 1. 传感器数据校准
        2. 可采用卡尔曼滤波

   FIXME: 采用类进行封装，因为可能需要一对
 */
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <unistd.h>
//#include "cn/中文化.hpp"
#include "hd/I2C.h"
#include "hd/MPU6050.h"

static constexpr uint8_t MPU6050_ADDR_0 = 0x68;
static constexpr uint8_t MPU6050_ADDR_1 = 0x69;
static constexpr uint8_t MPU6050_电源管理寄存器1 = 0x6B;
static constexpr uint8_t MPU6050_电源管理_非睡眠模式 = 0x00;
static constexpr uint8_t MPU6050_陀螺仪配置寄存器 = 0x1B;
static constexpr uint8_t MPU6050_陀螺仪量程_正负500度每秒 = 0x08;
static constexpr uint8_t MPU6050_陀螺仪横轴数据_高字节 = 0x43; /* x */
static constexpr uint8_t MPU6050_陀螺仪横轴数据_低字节 = 0x44;
static constexpr uint8_t MPU6050_陀螺仪纵轴数据_高字节 = 0x45; /* y */
static constexpr uint8_t MPU6050_陀螺仪纵轴数据_低字节 = 0x46;
static constexpr uint8_t MPU6050_陀螺仪竖轴数据_高字节 = 0x47; /* z */
static constexpr uint8_t MPU6050_陀螺仪竖轴数据_低字节 = 0x48;
static constexpr uint8_t MPU6050_加速度计配置寄存器 = 0x1C;
static constexpr uint8_t MPU6050_加速度计量程_正负8g = 0x10;
static constexpr uint8_t MPU6050_加速度计横轴数据_高字节 = 0x3B;
static constexpr uint8_t MPU6050_加速度计横轴数据_低字节 = 0x3C;
static constexpr uint8_t MPU6050_加速度计纵轴数据_高字节 = 0x3D;
static constexpr uint8_t MPU6050_加速度计纵轴数据_低字节 = 0x3E;
static constexpr uint8_t MPU6050_加速度计竖轴数据_高字节 = 0x3F;
static constexpr uint8_t MPU6050_加速度计竖轴数据_低字节 = 0x40;
static bool flag = false;

bool MPU6050_check() {
    I2C_lock();
    bool tmp = flag;
    I2C_unlock();
    return tmp;
}

/* 初始化 MPU6050 */
void MPU6050_init() {
    I2C_lock();
    I2C_setAddr(MPU6050_ADDR_0);
    I2C_writeReg(MPU6050_电源管理寄存器1,    MPU6050_电源管理_非睡眠模式);	/* 退出睡眠 */
    I2C_writeReg(MPU6050_陀螺仪配置寄存器,   MPU6050_陀螺仪量程_正负500度每秒);	/* ±500°/s */
    I2C_writeReg(MPU6050_加速度计配置寄存器, MPU6050_加速度计量程_正负8g);	/*  ±8g */
    flag = true;
    I2C_unlock();
    usleep(100000);	/* 等待稳定 */
}

/* 读取 16 位传感器数据（带符号） */
static int16_t read16bitReg(uint8_t high, uint8_t low) {
    return (I2C_readReg(high) << 8) | I2C_readReg(low);
}

/* 获取原始MPU6050传感器数据 */
MPU6050_sensorData MPU6050_readSensor() {
    MPU6050_sensorData data;
    I2C_lock();
    I2C_setAddr(MPU6050_ADDR_0);
    data.accel_x = read16bitReg(MPU6050_加速度计横轴数据_高字节, MPU6050_加速度计横轴数据_低字节);
    data.accel_y = read16bitReg(MPU6050_加速度计纵轴数据_高字节, MPU6050_加速度计纵轴数据_低字节);
    data.accel_z = read16bitReg(MPU6050_加速度计竖轴数据_高字节, MPU6050_加速度计竖轴数据_低字节);
    data.gyro_x  = read16bitReg(MPU6050_陀螺仪横轴数据_高字节,   MPU6050_陀螺仪横轴数据_低字节);
    data.gyro_y  = read16bitReg(MPU6050_陀螺仪纵轴数据_高字节,   MPU6050_陀螺仪纵轴数据_低字节);
    data.gyro_z  = read16bitReg(MPU6050_陀螺仪竖轴数据_高字节,   MPU6050_陀螺仪竖轴数据_低字节);
    I2C_unlock();
    return data;
}

/* FIXME: 当设备剧烈运动时（如加速度远大于重力），加速度计数据会包含运动加速度，此时角度计算可能不准确。实际应用中需结合陀螺仪数据（如互补滤波或卡尔曼滤波）来提高稳定性。 */
/* FIXME: 适合静态测量或学习用途。如果需要实际应用，建议添加传感器融合算法 */
void MPU6050_calcAngle(const MPU6050_sensorData& data, float& pitch, float& roll) {
    /* - ±2g → 16384 LSB/g
       - ±4g → 8192 LSB/g
       - ±8g → 4096 LSB/g
       - ±16g → 2048 LSB/g */
    constexpr float scale = 4096.0;    /* 加速度数据转重力单位（±8g 量程 4096 LSB/g） */
    float angle_x = data.accel_x / scale;	/* 横角 */
    float angle_y = data.accel_y / scale;	/* 纵角 */
    float angle_z = data.accel_z / scale;	/* 竖角 */

    /* 计算俯仰角(Pitch)和翻滚角(Roll) */
    pitch = atan2(-angle_x, sqrt(angle_y * angle_y + angle_z * angle_z)) * 180.0 / M_PI;
    roll = atan2(angle_y, angle_z) * 180.0 / M_PI;
}

