#pragma once
#include <iostream>
#include <stdexcept>
#include "nn/NeuralNetwork.hpp"
namespace qing {
/*  TODO: 增加将一层非线性层分解为两层的算法 */

class NNBuilder {  /* 神经网络构建器 */
public:
	void add(NeuralNetwork &layer);	/* 向神经网络中添加全连接层 */
	std::vector<std::pair<long, long>> get_shape() const;	/* 获取变换层的形状 */
	void print_shape() const;	/* 向标准输出打印形状 */
	void save(std::ostream& out);	/* 向输出流储存模型 */
	void load(std::istream& in);	/* 从输入流加载模型 */
	std::vector<float> forward(std::vector<float>& r);	/* 前向反馈 */
	std::vector<float> backward(std::vector<float>& errs, float discount = 1.0);	/* 反向传播 */
	std::vector<float> cal_err(std::vector<float>& r, std::vector<float>& t);	/* 计算误差 */
private:
	std::vector<NeuralNetwork> nn;
};
}
