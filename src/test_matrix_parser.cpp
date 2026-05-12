#include <code/MatrixParser.h>
using namespace qing;
int main() {
	auto parser = MatrixParser();
	parser.fromFile("scripts/test.mt");
	auto len = parser.size();
	std::cout << "==========================" << std::endl;
	std::cout << "Hash-map length is " << len << std::endl;
}
