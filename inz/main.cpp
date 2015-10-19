#include <cstdio>
#include <iostream>
#include "data.h"

int main() {

	data<int16_t,3> container1(3);
	data<int16_t> container2(3, 5, 7);
	data<int16_t> container3(4, 4, 4);

	for (int i = 0; i < 3; ++i) {
		std::cout << container2.get_single_data(i) <<"\n";
	}

	/*
	container2.set_single_data(2, 0);
	for (int i = 0; i < 3; ++i) {
		std::cout << container2.get_single_data(i) << "\n";
	}
	std::vector<int16_t> vector;
	vector = container2.get_data();
	for (int i = 0; i < 3; ++i) {
		std::cout << vector[i] << "\n";
	}*/
	system("pause");
	return 0;
}