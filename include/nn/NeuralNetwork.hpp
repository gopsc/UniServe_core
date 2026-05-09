#pragma once
#include <random>
#include "nn/Matrix.hh"
#include "nn/ILayerFullyConnectted.hpp"

namespace qing {

/* 人工神经网络 - 全连接层 */
class NeuralNetwork: public ILayerFullyConnectted {
public:

    /* 构造函数 使用基本参数构造
     * FIXME: 目前只能输入已经存在的向量变量 */
    NeuralNetwork(
        const long inputno, const long outputno,
        const std::vector<float> &weights,
        const std::vector<float> &bias,
        const float learning_rate,
        const ActivationFunc func_type
    );


    /* 获取激活函数枚举类型 */
    const ActivationFunc& get_f() const {
        return f_type;
    }

    /* 获取学习率 */
    const float& get_lr () const {
    	return learning_rate;
    }

    /* 获取输入层大小 */
    long get_inputno() const;

    /* 获取输出层大小 */
    long get_outputno() const;


    /* 获取权重矩阵 */
    Matrx<float>& get_weights();

    /* 获取偏置矩阵 */
    Matrx<float>& get_bias();

    
    /* 正态分布Xavier初始化 */
    static void xavierNormalInit(std::vector<float> &weights, long fan_in, long fan_out);
    
    /* 均匀分布Xavier初始化函数 */
    static void xavierUniformInit(std::vector<float> &weights, int fan_in, int fan_out);

    /* 工厂模式 通过形状构建全连接层 */
    static NeuralNetwork Create_in_Factory(const long inputno, const long outputno, float learning_rate, ActivationFunc func_type);

    /* 工厂模式 通过输入流读形状构建 */
    static NeuralNetwork Load_in_Factory(std::istream& in);

    /* 从输入流加载向量 */
    static std::vector<float> load_vec(std::istream& in, size_t length);


    /* 前向推理 */
    std::vector<float> forward(const std::vector<float>& x) override;

    /* 反向传播 */
    std::vector<float> backward(const std::vector<float>& errors) override;

    /* 更新权重  可以输入一个折扣系数 */
    void update(float discount = 1.0);
    
    /* 输出神经网络到输出流 */
    void save(std::ostream& out) override；


private:

    /* 神经网络层规格 */
    long inputno, outputno;

    /* 权重与偏置矩阵 */
    Matrx<float> weights; /* (outputs, inputs) */
    Matrx<float> bias;    /* (1, outputs) */

    /* 学习率 */
    float learning_rate;

    /* 激活函数 */
    ActivationFunc f_type;

    /* 存储在类内部的上一次的输入，输出，和激活前输出 */
    Matrx<float> prev_inputs, prev_raw_inputs, prev_outputs;
    /* 存储本次学习的权重梯度以及偏置梯度 */
    Matrx<float> weights_grad, bias_grad;

};
}
