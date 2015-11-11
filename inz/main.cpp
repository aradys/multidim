#include "container.h"

int main() {

    container<float,3> container1(3);
    container<int16_t> container2(1,2,3);
    container2.set_data(9,8,7,6,5,4);
    container<int16_t> container3(container2);

    bool saving = container2.save_to_file("container2");
    std::cout << saving << "\n";

    auto myf1 = container<int16_t>::load_from_file("container2");
    container<int16_t> myf2("container2");

    auto diff = myf1.get_difference(myf2);
    auto abs_err = myf1.get_max_absolute_error(myf2);
    auto mean_err = myf1.get_mean_error(myf2);
    auto std_dev = myf1.get_std_dev(myf2);

    //container for testing histogram
    container<int16_t> container100(2,2,5,5);
    container100.set_data(62, 71, 36, 37, 40, 52, 21, 28, 24, 18, 8, 81, 18, 40, 23, 39, 66, 88, 15, 89, 42, 76, 12, 74, 44, 59, 72, 82, 76, 93, 50, 36, 50, 29, 62, 71, 36, 70, 86, 15, 54, 7, 75, 27, 65, 31, 51, 65, 96, 68, 69, 4, 7, 16, 28, 45, 66, 92, 10, 96, 13, 54, 22, 25, 19, 52, 87, 99, 96, 80, 51, 97, 34, 22, 4, 99, 11, 41, 50, 68, 93, 67, 13, 66, 48, 61, 55, 39, 99, 38, 52, 27, 75, 39, 45, 44, 51, 18, 91, 67);

    container100.draw_histogram("histogram1.html", 7.5);

    system("pause");
    return 0;
}