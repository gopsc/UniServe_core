/* TODO(20260510-qing): 修改为输入整个向量，然后做激活、求导
 * 然后设计Softmax激活函数 */


#include "nn/ILayer.hpp"
#include <stdexcept>

/* 获取函数对象 - 获取激活函数 */
f_t ILayer::getf(ActivationFunc type) {
	switch (type) {
		case ActivationFunc::ReLU:
		return relu;
		case ActivationFunc::Leaky_ReLU:
		return leaky_relu;
		case ActivationFunc::Sigmoid:
		return sigmoid;
		case ActivationFunc::Tanh:
		return tanh;
		default:
		throw std::invalid_argument("ActivationFunc function does not support.");
	}
}

/* 获取函数对象 - 求激活函数导数 */
f_t ILayer::getfdot(ActivationFunc type) {
	switch (type) {
		case ActivationFunc::ReLU:
		return relu_derivative;
		case ActivationFunc::Leaky_ReLU:
		return  leaky_relu_derivative;
		case ActivationFunc::Sigmoid:
		return sigmoid_derivative;
		case ActivationFunc::Tanh:
		return  tanh_derivative;
		default:
		throw std::invalid_argument("ActivationFunc Derivative does not support.");
	}
}

const char* ILayer::get_f_type(ActivationFunc& af) {
	switch (af) {
		case ActivationFunc::Sigmoid:
		return "Sigmoid";
		case ActivationFunc::ReLU:
		return "ReLU";
		case ActivationFunc::Leaky_ReLU:
		return "Leaky_ReLU";
		default:
		throw std::runtime_error("What activation function");
	}
}

ILayer::ActivationFunc ILayer::parse_ff_type(const std::string& name)
{
	if(name == "Sigmoid") { return ActivationFunc::Sigmoid; }
	else if (name == "ReLU") { return ActivationFunc::ReLU; }
	else if (name == "Leaky_ReLU") { return ActivationFunc::Leaky_ReLU; }
	else { throw std::runtime_error("What activation function"); }
}

float ILayer::relu(float x) {	/* ReLU 激活函数 */
	return (x > 0.0) ? x : 0.0; }

float ILayer::relu_derivative(float x) {	/* ReLU 导数 */
	return (x > 0) ? 1: 0; }

/* NOTE: alpha太小或者选择使用ReLU函数的情况下，会导致梯度消失 
 * NOTE: Leaky ReLU 不会使梯度完全消失，但也会使它变得很小  */
float ILayer::leaky_relu(float x) {	/* leaky ReLU 激活函数 */
	return (x > 0) ? x : LEAKY_RELU_ALPHA * x; }

float ILayer::leaky_relu_derivative(float x) {	/* leaky ReLU 导数 */
	return (x > 0) ? 1.0f : LEAKY_RELU_ALPHA; }

float ILayer::sigmoid(float x) {	/* Sigmoid 激活函数 */
	return 1.0 / (1.0 + std::exp(-x)); }

float ILayer::sigmoid_derivative(float x)	/* Sigmoid 导数 */
{
    double s = sigmoid(x);
    return s * (1.0 - s);
}

float ILayer::tanh(float x) {	/* Tanh 激活函数 */
	return std::tanh(x); }

float ILayer::tanh_derivative(float x) {	/* Tanh 导数 */
	return 1.0f - x * x; }
