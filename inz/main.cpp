#include "container.h"

int main() {

    container<float,3> container1(3);
    container<int16_t> container2(1,2,3);
    container2.set_data(9,8,7,6,5,4);
    container<int16_t> container3(container2);

    bool saving = container2.save_to_file("container2");
    std::cout << saving << "\n";

    auto myf1 = container<int16_t>::load_from_file("container2");
    //container<int16_t> myf2("container2");

    system("pause");
    return 0;
}