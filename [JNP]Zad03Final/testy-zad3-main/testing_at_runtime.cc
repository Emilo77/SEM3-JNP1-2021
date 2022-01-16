#include "../fuzzy.h"

#include <iostream>
#include <random>

std::mt19937 rng(0);
int rd(int l, int r) {
	return std::uniform_int_distribution<int>(l, r)(rng);
}

TriFuzzyNum random_tri() {
	return TriFuzzyNum(rd(-5, 5), rd(-5, 5), rd(-5, 5));
}

int main() {
	for(int i = 0; i < int(1e4); ++i) {
		TriFuzzyNum a = random_tri(), b = random_tri();

		std::cout << (a + (b * (a - b))) << '\n';
		std::cout << (((a += b) *= b) -= b) << '\n';
		std::cout << (a < b) << (a > b) << (a <= b) << (a >= b) << (a == b) << (a != b) << '\n';
		std::cout << a.lower_value() << ' ' << a.modal_value() << ' ' << a.upper_value() << '\n';
		std::cout << (a == crisp_number(-1)) << (a == crisp_zero) << '\n';
	}

	for(int i = 0; i < int(1e4); ++i) {
		TriFuzzyNumSet s;
		for(int oper = 0; oper < 10; ++oper) {
			int type = rd(0, 2);
			if(type == 0)
				s.insert(random_tri());
			else if(type == 1)
				s.remove(random_tri());
			else if(type == 2) {
				try {
					std::cout << s.arithmetic_mean() << '\n';
				} catch(std::exception &e) {
					std::cout << e.what() << '\n';
				}
			}
		}
	}
}
