#ifndef PCA9685_H
#define PCA9685_H
namespace qing {
class PCA9685 {	/* PCA9685 */
public:
	PCA9685(I2C& i2c);
	PCA9685(const PCA9685&) = delete;
	bool check();
	void setPWM(uint8_t channel, uint16_t on, uint16_t off);
	void setAngle(uint8_t channel, float angle);
private:
	static constexpr uint8_t PCA9685_ADDR = 0x40;	/* 7位地址 */
	bool flag = false;	/* 启动标志 */
	I2C& i2c;
};
}
#endif
