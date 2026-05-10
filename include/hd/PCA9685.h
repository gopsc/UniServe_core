#ifndef PCA9685_H
#define PCA9685_H
/* PCA9685 */
bool PCA9685_check();
void PCA9685_init();
void PCA9685_setPWM(uint8_t channel, uint16_t on, uint16_t off);
void PCA9685_setAngle(uint8_t channel, float angle);
#endif
