#ifndef I2C_H
#define I2C_H
/*
 * # 安装工具
 * apt install i2c-tools libi2c-dev
 *
 * # 扫描设备
 * i2cdetect -y 1
 */

namespace qing {
class I2C {	/* 基本I2C */
private:
	int fd = -1;
	pthread_mutex_t mutex;	/* FIXME: 这个锁我们只提供接口给外部使用？ */
public:
	I2C(const char* path);
	I2C(const I2C&) = delete;	/* 删除复制构造函数 */
	~I2C();
	bool check();
	void lock(); /* FIXME: 使用锁类 */
	void unlock();
	void setAddr(const uint8_t addr);
	void writeReg(uint8_t reg, uint8_t val);
	uint8_t readReg(uint8_t reg);
};
}
#endif
