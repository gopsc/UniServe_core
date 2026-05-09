#ifndef I2C_H
#define I2C_H
/* 基本I2C */
bool I2C_check();
void I2C_init(const char* path);
void I2C_free();
void I2C_lock(); /* FIXME: 使用锁类 */
void I2C_unlock();
void I2C_setAddr(const uint8_t addr);
void I2C_writeReg(uint8_t reg, uint8_t val);
uint8_t I2C_readReg(uint8_t reg);
#endif
