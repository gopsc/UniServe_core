/*
 *  该文件是i2c用户界面
 *
 * NOTE:  以C++的方式处理异常可能会更好
 * FIXME: 包含询问方法以确定i2c设备的状态
 */

#include <cstring>
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>
#include <pthread.h>
//#include "cn/中文化.hpp"

static int fd = -1;
static pthread_mutex_t mutex;  /* FIXME: 这个锁我们只提供接口给外部使用？ */

bool I2C_check() {
    return !(fd < 0);
}

void I2C_init(const char* path) {  /* FIXME: 动态指定设备文件 */
    if ((fd = open(path, O_RDWR)) < 0)
        throw std::runtime_error( /* FIXME: 使用专用的异常类 */
            "Failed to open I2C bus: " + std::string(strerror(errno)));
}

void I2C_free() {
    close(fd);
}

void I2C_setAddr(const uint8_t addr) {
    if (ioctl(fd, I2C_SLAVE, addr) < 0)
        throw std::runtime_error(
            "Failed to set I2C address: " + std::string(strerror(errno)));
}

void I2C_writeReg(uint8_t reg, uint8_t val) {  /* 写寄存器 */
    uint8_t buf[] = {reg, val};
    if (write(fd, buf, 2) != 2)
        throw std::runtime_error(
            "I2C write error: " + std::string(strerror(errno)));
}

/* 读寄存器 */
uint8_t I2C_readReg(uint8_t reg) {

    uint8_t val;

    if (write(fd, &reg, 1) != 1)
        throw std::runtime_error(
            "I2C read error: " + std::string(strerror(errno)));

    if (read(fd, &val, 1)!= 1)
        throw std::runtime_error(
            "I2C read error: " + std::string(strerror(errno)));

    return val;

}


/* 获取锁 */
void I2C_lock() {
    pthread_mutex_lock(&mutex); /* 加锁 */
}

/* 释放锁 */
void I2C_unlock() {
    pthread_mutex_unlock(&mutex);   /* 解锁 */
}
