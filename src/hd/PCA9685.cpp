/* PCA9685 舵机驱动板 */
#include <cstdint>
#include <unistd.h>
#include <pthread.h>
//#include "cn/中文化.hpp"
#include "hd/I2C.h"
#include "hd/PCA9685.h"

namespace qing {

	bool PCA9685::check()
	{
		i2c.lock();
		bool tmp = flag;
		i2c.unlock();
		return tmp && i2c.check();
	}

	PCA9685::PCA9685(I2C& i2c): i2c(i2c)
	{
		i2c.lock();
		i2c.setAddr(PCA9685_ADDR);
		i2c.writeReg(0x00, 0x20);   /* 开启AI（地址自增）和关闭SLEEP */
		flag = true;  
		i2c.unlock();
		usleep(5000);               /* 等待振荡器启动 */

		constexpr float freq = 50.0;	/* 设置PWM频率为50Hz（舵机标准周期20ms） */
		constexpr float prescale_val = 25000000.0 / (4096 * freq) - 1;    /* 25MHz主频 */
		uint8_t prescale = static_cast<uint8_t>(prescale_val + 0.5);

		i2c.lock();
		i2c.writeReg(0x00, 0x10);       /* 进入SLEEP模式（允许改频率） */
		i2c.writeReg(0xFE, prescale);   /* 写入预分频值 */
		i2c.writeReg(0x00, 0x20);       /* 退出SLEEP */
		i2c.unlock();
		usleep(5000);
	}

	void PCA9685::setPWM(	/* 设置指定通道的PWM信号（0-4095） */
			uint8_t channel, uint16_t on, uint16_t off)
	{
		uint8_t reg = 0x06 + 4 * channel;   /* LED0 寄存器起始地址 */
		i2c.lock();
		i2c.setAddr(PCA9685_ADDR);
		i2c.writeReg(reg, on & 0xFF);
		i2c.writeReg(reg + 1, on >> 8);
		i2c.writeReg(reg + 2, off & 0xFF);
		i2c.writeReg(reg + 3, off > 8);
		i2c.unlock();
	}

	void PCA9685::setAngle(	/* 角度转PWM值（0°~180° → 0.5ms~2.5ms脉宽）*/
			uint8_t channel, float angle)
	{
		constexpr float min = 0.5;		/* 最小脉冲，0°对应0.5ms */
		constexpr float max = 2.5;		/* 最大脉冲，180°对应2.5ms */
		float pulse = min + (max - min) * (angle / 180.0);		/* 脉冲宽度 */
		uint16_t off = static_cast<uint16_t>(pulse * 4096 / 20);	/* 关断值，20ms 周期 */
		this->setPWM(channel, 0, off);				/* ON=0, OFF=计算值 */
	}

}
