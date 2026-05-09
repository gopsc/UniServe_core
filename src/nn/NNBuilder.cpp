#include "nn/NNBuilder.hpp"
namespace qing {

void NNBuilder::add(NeuralNetwork &layer) { nn.push_back(layer); }

std::vector<std::pair<long, long>> NNBuilder::get_shape() const
{
	std::vector<std::pair<long, long>> shape;
	for (long j = 0; j < nn.size(); ++j)
		shape.emplace_back(nn[j].get_inputno(), nn[j].get_outputno());
	return shape;
}

void NNBuilder::print_shape() const {
	auto shape = get_shape();
	for (const auto& p : shape)
		std::cout << p.first << " -> " << p.second << std::endl;
}

std::vector<float> NNBuilder::forward(std::vector<float>& r)	/* 前向反馈 */
{
	auto x = r;
	for (long j=0; j< nn.size(); ++j)
		x = nn[j].forward(x);
	return x;
}

/* TODO: 带折扣衰减的反向传播 */
std::vector<float> NNBuilder::backward(std::vector<float>& errs, float discount)	/* 反向传播 */
{
	auto e = errs;
	for (long j=nn.size()-1; j>=0;--j) {
		e = nn[j].backward(e);
		nn[j].update(discount);
	}
	return e;
}

std::vector<float> NNBuilder::cal_err(std::vector<float>& r, std::vector<float>& t)	/* 计算误差 */
{
	auto res = r;
	for (long j=0; j<r.size(); ++j)
		res[j] = t[j] - r[j];
	return res;
}

void NNBuilder::save(std::ostream& out) {
	for (auto& layer: nn)
		layer.save(out);
}


void NNBuilder::load(std::istream& in) {
	nn.clear();
	while(true) {
		try{
			auto layer = NeuralNetwork::Load_in_Factory(in);
			add(layer);
		}
		catch (std::runtime_error& e) {
			break; }
	}
}


}
