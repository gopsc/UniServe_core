/*
 *  该文件是i2c用户界面
 *
 * TODO:  以C++的方式处理异常可能会更好（能够打开多个I2C设备）
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
#include "hd/I2C.h"

namespace qing {
	I2C::I2C(const char* path) {	/* FIXME: 动态指定设备文件 */
		if ((fd = open(path, O_RDWR)) < 0)
			throw std::runtime_error(	/* FIXME: 使用专用的异常类 */
				"Failed to open I2C bus: " + std::string(strerror(errno)));
	}

	I2C::~I2C() { close(fd); }	/* 释放资源 */

	bool I2C::check() { return !(fd < 0); }

	void I2C::setAddr(const uint8_t addr) {	/* 设置地址 */
		if (ioctl(fd, I2C_SLAVE, addr) < 0)
			throw std::runtime_error(
				"Failed to set I2C address: " + std::string(strerror(errno)));
	}

	void I2C::writeReg(uint8_t reg, uint8_t val) {	/* 写寄存器 */
		uint8_t buf[] = {reg, val};
		if (write(fd, buf, 2) != 2)
			throw std::runtime_error(
 				"I2C write error: " + std::string(strerror(errno)));
	}

	uint8_t I2C::readReg(uint8_t reg) {	/* 读寄存器 */

		uint8_t val;

		if (write(fd, &reg, 1) != 1)
			throw std::runtime_error(
				"I2C read error: " + std::string(strerror(errno)));

		if (read(fd, &val, 1)!= 1)
			throw std::runtime_error(
				"I2C read error: " + std::string(strerror(errno)));

		return val;

	}


	void I2C::lock() {	/* 获取锁 */
		pthread_mutex_lock(&mutex); /* 加锁 */
	}

	void I2C::unlock() {	/* 释放锁 */
		pthread_mutex_unlock(&mutex);   /* 解锁 */
	}

}
