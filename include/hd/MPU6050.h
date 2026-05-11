#ifndef MPU6050_H
#define MPU6050_H
namespace qing {
class MPU6050 {
public:
	struct sensorData {	/*  MPU6050 - 传感器数据  */
		int16_t accel_x, accel_y, accel_z;
		int16_t gyro_x, gyro_y, gyro_z;
	};
	MPU6050(I2C& i2c);
	bool check();
	sensorData readSensor();  /* FIXME: 输入结构体引用（或指针）会更快 */
	void calcAngle(const sensorData& data, float& pitch, float& roll);
private:
	bool flag = false;
	I2C& i2c;
	int16_t read16bitReg(uint8_t high, uint8_t low);	/* 读取 16 位传感器数据（带符号） */
};
}
#endif
