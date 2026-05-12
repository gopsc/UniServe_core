#include <code/MatrixParser.h>
using namespace qing;
int main() {
	auto parser = MatrixParser();
	parser.fromFile("scripts/test.mt");
	auto len = parser.size();
	std::cout << "==========================" << std::endl;
	std::cout << "Hash-map length is " << len << std::endl;
	for (auto key: parser.keys()) {
		std::cout << "Matrix name: " << key;
		std::cout << ", ROW: " << parser.row(key);
		std::cout << ", COL: " << parser.col(key);
		std::cout << std::endl;
	}
}
