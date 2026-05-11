#include <pthread.h>
#include "hd/PCA9685.h"
namespace qing {
class Servo{  /* 180度舵机 */
public:

	Servo(PCA9685& pca9685, int channel, float max, float min, bool reverse=false)	/* FIXME: min只能比max小 */
	: pca9685(pca9685), channel(channel), max(max), min(min), reverse(reverse) {}

	void setAngle(float angle) {	/* 控制舵机转动至一个角度 */
		angle = std::min(max, angle);
		angle = std::max(min, angle);
		pca9685.setAngle(
			channel, (channel) ? MAX_ANGLE - angle : angle
		);
	}

	float get_max() { return max; }

	float get_min() { return min; }


private:
	PCA9685& pca9685;
	int channel;	/* 通道编号 */
	float max, min; 	/* 角度上下限 */
	bool reverse;	/* 是否反向 */
	static constexpr float MAX_ANGLE = 180;
};
}
