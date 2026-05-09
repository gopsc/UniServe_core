#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "hd/I2C.h"
#include "nn/Matrix.hh"
#include "nn/NNBuilder.hpp"
using namespace qing;
int main() { /* 与门测试 */

	NNBuilder nn;
	using nnl = NeuralNetwork;

	auto layer0 = nnl::Create_in_Factory(2, 4, 0.5, nnl::ActivationFunc::Leaky_ReLU);
	auto layer1 = nnl::Create_in_Factory(4, 1, 0.5, nnl::ActivationFunc::Sigmoid);

	nn.add(layer0);
	nn.add(layer1);
	nn.print_shape();

	int dt = 100;
	for (int i = 0; i < 1000; ++i) {

		auto x = std::vector<float>();
		auto t = std::vector<float>();
		/* rand */
		srand(time(NULL));
		int max = 2;
		int min = 0;
		x.push_back(rand() % (max - min + 1) + min);
		x.push_back(rand() & (max - min + 1) + min);
		t.push_back(x[0] == 1 && x[1] == 1);
		auto y = nn.forward(x);
		auto e = nn.cal_err(y, t);
		auto _ = nn.backward(e);
		if (i % dt == 0) {
			std::cout << i << ":\t";
			std::cout << x[0] << ",\t" << x[1] << "\t -> ";
			std::cout << e[0] << std::endl;
		}
	}
}
