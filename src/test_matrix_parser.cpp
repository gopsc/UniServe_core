#include <code/MatrixParser.h>
using namespace qing;
int main() {
	auto parser = MatrixParser();
	parser.fromFile("scripts/test.mt");
}
