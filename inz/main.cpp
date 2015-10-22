#include <cstdio>
#include <iostream>
#include "container.h"

int main() {
	
    container<float,3> container1(3);
    container<int16_t> container2(3, 5, 7);
    container<int16_t> container3(container2);

    /*
    container2.set_single_container(2, 0);
    for (int i = 0; i < 3; ++i) {
        std::cout << container2.get_single_container(i) << "\n";
    }
    std::vector<int16_t> vector;
    vector = container2.get_container();
    for (int i = 0; i < 3; ++i) {
        std::cout << vector[i] << "\n";
    }*/
    system("pause");
    return 0;
}